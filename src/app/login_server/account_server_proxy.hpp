#pragma once

#include "singleton.hpp"
#include "local_client_proxy.hpp"
#include "logic_system.hpp"

namespace rain
{

class AccountServerProxy
    : public LocalClientProxy<AccountServerProxy, LogicSystem>
    , public Singleton<AccountServerProxy>
{
    friend Singleton<AccountServerProxy>;
    AccountServerProxy() = default;

public:
    bool init()
    {
        return LocalClientProxy::init("AccountServer");
    }
};

} // !namespace rain
