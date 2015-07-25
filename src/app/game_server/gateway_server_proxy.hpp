#pragma once

#include "singleton.hpp"
#include "message_pack.hpp"
#include "server_proxy.hpp"

namespace rain
{

class GatewayServerProxy
    : public Singleton<GatewayServerProxy>
{
    friend Singleton<GatewayServerProxy>;
    GatewayServerProxy() = default;

    using TargetSession = Session<GatewayServerProxy>;
    using TargetSessionPtr = std::shared_ptr<TargetSession>;
    using Proxy = ServerProxy<GatewayServerProxy>;
    using ProxyPtr = std::unique_ptr<Proxy>;

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
            "Server.GatewayServer.PingInterval",
            "Server.GatewayServer.BreakTimes",
            "Server.GatewayServer.Port"
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