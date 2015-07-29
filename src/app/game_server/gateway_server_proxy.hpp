#pragma once

#include "singleton.hpp"
#include "local_server_proxy.hpp"
#include "logic_system.hpp"

namespace rain
{

class GatewayServerProxy
    : public LocalServerProxy<GatewayServerProxy, LogicSystem>
    , public Singleton<GatewayServerProxy>
{
    friend Singleton<GatewayServerProxy>;
    GatewayServerProxy() = default;

public:
    bool init()
    {
        return LocalServerProxy::init("GatewayServer");
    }
};

} // !namespace rain
