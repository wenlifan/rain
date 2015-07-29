#pragma once

#include "singleton.hpp"
#include "local_client_proxy.hpp"
#include "logic_system.hpp"

namespace rain
{

class SharedServerProxy
    : public LocalClientProxy<SharedServerProxy, LogicSystem>
    , public Singleton<SharedServerProxy>
{
    friend Singleton<SharedServerProxy>;
    SharedServerProxy() = default;

public:
    bool init()
    {
        return LocalClientProxy::init("SharedServer");
    }
};

} // !namespace rain
