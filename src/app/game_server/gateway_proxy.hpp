#pragma once

#include "singleton.hpp"
#include "message_pack.hpp"
#include "server_proxy.hpp"

namespace rain
{

class GatewayProxy
    : public ServerProxy<GatewayProxy>
    , public Singleton<GatewayProxy>
{
    friend Singleton<GatewayProxy>;
    GatewayProxy() = default;

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

public:
    bool init()
    {
        return ServerProxy::init(
            "Server.Gateway.PingInterval",
            "Server.Gateway.BreakTimes",
            "Server.Gateway.Port"
        );
    }
};

} // !namespace rain