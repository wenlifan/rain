#pragma once

#include "singleton.hpp"
#include "local_server_proxy.hpp"
#include "logic_system.hpp"

namespace rain
{

class SharedServerProxy
    : public LocalServerProxy<SharedServerProxy, LogicSystem>
    , public Singleton<SharedServerProxy>
{
    friend Singleton<SharedServerProxy>;
    SharedServerProxy() = default;

public:
    bool init()
    {
        return LocalServerProxy::init("SharedServer");
    }
};

} // !namespace rain
