#pragma once

#include "singleton.hpp"
#include "message_pack.hpp"
#include "client_proxy.hpp"

namespace rain
{

class SharedServerProxy
    : public Singleton<SharedServerProxy>
{
    friend Singleton<SharedServerProxy>;
    SharedServerProxy() = default;

    using TargetSession = Session<SharedServerProxy>;
    using TargetSessionPtr = std::shared_ptr<TargetSession>;
    using Proxy = ClientProxy<SharedServerProxy>;
    using ProxyPtr = std::shared_ptr<Proxy>;

public:
    void add_session(TargetSessionPtr session)
    {

    }

    void remove_session(TargetSessionPtr session)
    {

    }

    void dispatch_message(TargetSessionPtr session, MessagePackPtr msgp)
    {

    }

    std::size_t get_ping_interval() const
    {
        return proxy_->get_ping_interval();
    }

    std::size_t get_break_times() const
    {
        return proxy_->get_break_times();
    }

public:
    bool init()
    {
        proxy_ = std::make_unique<Proxy>();
        return proxy_->init(
            "Client.SharedServer.PingInterval",
            "Client.SharedServer.BreakTimes",
            "Client.SharedServer.IP",
            "Client.SharedServer.Port"
        );
    }

    void stop()
    {
        proxy_.reset();
    }

private:
    ProxyPtr proxy_;
};

} // !namespace rain