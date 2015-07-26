#pragma once

#include "singleton.hpp"
#include "message_pack.hpp"
#include "server_proxy.hpp"

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
            "Server.GatewayServer.PingInterval",
            "Server.GatewayServer.BreakTimes",
            "Server.GatewayServer.Port"
        );
    }

    void stop()
    {
    }

private:
};

} // !namespace rain