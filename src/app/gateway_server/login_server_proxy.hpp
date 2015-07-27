#pragma once

#include "singleton.hpp"
#include "local_server_proxy.hpp"

namespace rain
{

class LoginServerProxy
    : public LocalServerProxy<LoginServerProxy>
    , public Singleton<LoginServerProxy>
{
    friend Singleton<LoginServerProxy>;
    LoginServerProxy() = default;

public:
    void dispatch_message(TargetSessionPtr session, MessagePackPtr msgp)
    {

    }

    bool init()
    {
        return LocalServerProxy::init("LoginServer");
    }
};

} // !namespace rain
