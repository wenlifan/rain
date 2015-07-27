#pragma once

#include "singleton.hpp"
#include "message_pack.hpp"
#include "client_proxy.hpp"

namespace rain
{

class AccountServerProxy
    : public ClientProxy<AccountServerProxy>
    , public Singleton<AccountServerProxy>
{
    friend Singleton<AccountServerProxy>;
    AccountServerProxy() = default;

public:
    void add_session(TargetSessionPtr session)
    {
        session_ = session;
        auto msgs = unsent_msg_.pop_all();
        for (auto &i : msgs) {
            send_message(i);
        }
        RAIN_INFO("Connected to account_server");
    }

    void remove_session(TargetSessionPtr)
    {
        RAIN_WARN("Disconnect from account_server, reconnect...");
        session_.reset();
        connect();
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
        return ClientProxy::init(
            "Client.AccountServer.PingInterval",
            "Client.AccountServer.BreakTimes",
            "Client.AccountServer.IP",
            "Client.AccountServer.Port"
        );
    }

    void stop()
    {
        stop_reconn_thread();
    }

    bool status() const
    {
        return session_ != nullptr;
    }

private:
    TargetSessionPtr session_;
    ThreadSafeQueue<MessagePackPtr> unsent_msg_;
};

} // !namespace rain