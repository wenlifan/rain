#pragma once

#include <string>
#include <atomic>

#include "singleton.hpp"
#include "message_pack.hpp"
#include "server_proxy.hpp"
#include "thread_safe_queue.hpp"

namespace rain
{

class GameServerProxy
    : public ServerProxy<GameServerProxy>
    , public Singleton<GameServerProxy>
{
    friend Singleton<GameServerProxy>;
    GameServerProxy() = default;

public:
    void add_session(TargetSessionPtr session)
    {
        auto ip = session->get_socket().remote_endpoint().address().to_string();
        if (ip != accept_id_) {
            RAIN_WARN("Illegal access to server, ip: " + ip);
            return;
        }

        session_ = session;
        auto msgs = unsent_msg_.pop_all();
        for (auto &i : msgs) {
            send_message(i);
        }
    }

    void remove_session(TargetSessionPtr)
    {
        session_.reset();
    }

    void send_message(MessagePackPtr msgp)
    {
        unsent_msg_.push(msgp);
        session_->write(msgp);
    }

    void send_message_finished(TargetSessionPtr, MessagePackPtr msgp)
    {
        unsent_msg_.erase(msgp);
    }

    void dispatch_message(TargetSessionPtr session, MessagePackPtr msgp)
    {

    }

public:
    bool init()
    {
        return init_accept_ip() && ServerProxy::init(
            "Server.GameServer.PingInterval",
            "Server.GameServer.BreakTimes",
            "Server.GameServer.Port"
        );
    }

    void stop()
    {
        if (session_ != nullptr)
            session_->remove();
    }

    bool status() const
    {
        return session_ != nullptr;
    }

private:
    bool init_accept_ip()
    {
        auto &reader = ConfigReader::get_instance();
        if (!reader.read_string(accept_id_, "Server.GameServer.AcceptIP")) {
            RAIN_ERROR("Read Server.GameServer.AcceptIP failed");
            return false;
        }
        return true;
    }

private:
    TargetSessionPtr session_;
    ThreadSafeQueue<MessagePackPtr> unsent_msg_;
    std::string accept_id_;
};

} // !namespace rain