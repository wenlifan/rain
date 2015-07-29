#pragma once

#include "singleton.hpp"
#include "local_client_proxy.hpp"
#include "logic_system.hpp"

namespace rain
{

class GatewayServerProxy
    : public LocalClientProxy<GatewayServerProxy, LogicSystem>
    , public Singleton<GatewayServerProxy>
{
    friend Singleton<GatewayServerProxy>;
    GatewayServerProxy() = default;

public:
    bool init()
    {
        return LocalClientProxy::init("GatewayServer");
    }
};

} // !namespace rain
