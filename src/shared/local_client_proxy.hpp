#pragma once

#include "message_pack.hpp"
#include "client_proxy.hpp"

namespace rain
{

template <typename Proxy>
class LocalClientProxy
    : public ClientProxy<Proxy>
{
protected:
    using TargetSessionPtr = typename ClientProxy<Proxy>::TargetSessionPtr;

    LocalClientProxy() = default;

public:
    void add_session(TargetSessionPtr session)
    {
        session_ = session;
        auto msgs = unsent_msg_.pop_all();
        for (auto &i : msgs) {
            send_message(i);
        }
        RAIN_INFO("Successful connected to " + target_server_name_);
    }

    void remove_session(TargetSessionPtr)
    {
        RAIN_WARN("Disconnect from " + target_server_name_ + ", reconnect...");
        session_.reset();
        ClientProxy<Proxy>::connect();
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

    void stop()
    {
        ClientProxy<Proxy>::stop_reconn_thread();
    }

protected:
    bool init(std::string const &server_name)
    {
        target_server_name_ = server_name;
        return ClientProxy<Proxy>::init(
            "Client." + server_name + ".PingInterval",
            "Client." + server_name + ".BreakTimes",
            "Client." + server_name + ".IP",
            "Client." + server_name + ".Port"
        );
    }

private:
    TargetSessionPtr session_;
    ThreadSafeQueue<MessagePackPtr> unsent_msg_;
    std::string target_server_name_;
};

} // !namespace rain