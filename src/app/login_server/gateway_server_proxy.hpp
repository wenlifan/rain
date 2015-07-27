#pragma once

#include "singleton.hpp"
#include "local_client_proxy.hpp"

namespace rain
{

class GatewayServerProxy
    : public LocalClientProxy<GatewayServerProxy>
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
        return LocalClientProxy::init("GatewayServer");
    }
};

} // !namespace rain
