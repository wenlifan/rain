#pragma once

#include <string>

#include "message_pack.hpp"
#include "server_proxy.hpp"
#include "thread_safe_queue.hpp"

namespace rain
{

template <typename Proxy>
class LocalServerProxy
    : public ServerProxy<Proxy>
{
protected:
    using TargetSessionPtr = typename ServerProxy<Proxy>::TargetSessionPtr;

    LocalServerProxy() = default;

public:

    void add_session(TargetSessionPtr session)
    {
        auto ip = session->get_socket().remote_endpoint().address().to_string();
        if (ip != accept_ip_) {
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
        if (session_)
            session_->write(msgp);
    }

    void send_message_finished(TargetSessionPtr, MessagePackPtr msgp)
    {
        unsent_msg_.erase(msgp);
    }

    void stop()
    {
        if (session_ != nullptr)
            session_->remove();
    }

protected:
    bool init(std::string const &server_name)
    {
        return init_accept_ip(server_name) && ServerProxy<Proxy>::init(
            "Server." + server_name + ".PingInterval",
            "Server." + server_name + ".BreakTimes",
            "Server." + server_name + ".Port"
        );
    }

private:
    bool init_accept_ip(std::string const &server_name)
    {
        auto &reader = ConfigReader::get_instance();
        if (!reader.read_string(accept_ip_, "Server." + server_name + ".AcceptIP")) {
            RAIN_ERROR("Read Server." + server_name + ".AcceptIP failed");
            return false;
        }
        return true;
    }

private:
    TargetSessionPtr session_;
    ThreadSafeQueue<MessagePackPtr> unsent_msg_;
    std::string accept_ip_;
};

} // !namespace rain