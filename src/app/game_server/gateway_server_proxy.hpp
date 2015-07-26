#pragma once

#include <string>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "singleton.hpp"
#include "message_pack.hpp"
#include "server_proxy.hpp"
#include "thread_safe_queue.hpp"

namespace rain
{

class GatewayServerProxy
    : public ServerProxy<GatewayServerProxy>
    , public Singleton<GatewayServerProxy>
{
    friend Singleton<GatewayServerProxy>;
    GatewayServerProxy() = default;

public:
    void add_session(TargetSessionPtr session)
    {
        auto ip = session->get_socket().remote_endpoint().address().to_string();
        if (ip != accept_id_) {
            RAIN_WARN("Illegal access to server, ip: " + ip);
            return;
        }
        session_ = session;
        if (new_session_flag_) {
            auto msgs = unsent_msg_.pop_all();
            for (auto &i : msgs) {
                send_message(i);
            }
        }
    }

    void remove_session(TargetSessionPtr session)
    {
        session_->shutdown(tcp::socket::shutdown_both);
        session_.reset();
    }

    void session_read_eof(TargetSessionPtr)
    {

    }

    void send_message_finished(TargetSessionPtr, MessagePackPtr msgp)
    {
        unsent_msg_.erase(msgp);
    }

    void dispatch_message(TargetSessionPtr session, MessagePackPtr msgp)
    {

    }

public:
    void send_message(MessagePackPtr msgp)
    {
        unsent_msg_.push(msgp);
        session_->write(msgp);
    }

public:
    bool init()
    {
        return init_accept_ip() && ServerProxy::init(
            "Server.GatewayServer.PingInterval",
            "Server.GatewayServer.BreakTimes",
            "Server.GatewayServer.Port"
        );
    }

    bool init_accept_ip()
    {
        auto &reader = ConfigReader::get_instance();
        if (!reader.read_string(accept_id_, "Server.GatewayServer.AcceptIP")) {
            RAIN_ERROR("Read Server.GatewayServer.AcceptIP failed");
            return false;
        }
        return true;
    }

    void stop()
    {
    }

private:
    TargetSessionPtr session_;
    ThreadSafeQueue<MessagePackPtr> unsent_msg_;
    std::string accept_id_;

    std::atomic_bool new_session_flag_{true};

    std::mutex send_all_mutex;
    std::condition_variable send_all_var_;
};

} // !namespace rain