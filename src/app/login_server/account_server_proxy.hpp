#pragma once

#include "singleton.hpp"
#include "local_client_proxy.hpp"

namespace rain
{

class AccountServerProxy
    : public LocalClientProxy<AccountServerProxy>
    , public Singleton<AccountServerProxy>
{
    friend Singleton<AccountServerProxy>;
    AccountServerProxy() = default;

public:
    void dispatch_message(TargetSessionPtr session, MessagePackPtr msgp)
    {

    }

    bool init()
    {
        return LocalClientProxy::init("AccountServer");
    }
};

} // !namespace rain
