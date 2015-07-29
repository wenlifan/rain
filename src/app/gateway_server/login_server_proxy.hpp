#pragma once

#include "singleton.hpp"
#include "local_server_proxy.hpp"
#include "logic_system.hpp"

namespace rain
{

class LoginServerProxy
    : public LocalServerProxy<LoginServerProxy, LogicSystem>
    , public Singleton<LoginServerProxy>
{
    friend Singleton<LoginServerProxy>;
    LoginServerProxy() = default;

public:
    bool init()
    {
        return LocalServerProxy::init("LoginServer");
    }
};

} // !namespace rain
