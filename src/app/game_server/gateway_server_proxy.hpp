#pragma once

#include "singleton.hpp"
#include "local_server_proxy.hpp"

namespace rain
{

class GatewayServerProxy
    : public LocalServerProxy<GatewayServerProxy>
    , public Singleton<GatewayServerProxy>
{
    friend Singleton<GatewayServerProxy>;
    GatewayServerProxy() = default;

public:
    void dispatch_message(TargetSessionPtr session, MessagePackPtr msgp)
    {

    }

    bool init()
    {
        return LocalServerProxy::init("GatewayServer");
    }
};

} // !namespace rain
