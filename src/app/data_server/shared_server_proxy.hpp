#pragma once

#include "singleton.hpp"
#include "local_server_proxy.hpp"

namespace rain
{

class SharedServerProxy
    : public LocalServerProxy<SharedServerProxy>
    , public Singleton<SharedServerProxy>
{
    friend Singleton<SharedServerProxy>;
    SharedServerProxy() = default;

public:
    void dispatch_message(TargetSessionPtr session, MessagePackPtr msgp)
    {

    }

    bool init()
    {
        return LocalServerProxy::init("SharedServer");
    }
};

} // !namespace rain
