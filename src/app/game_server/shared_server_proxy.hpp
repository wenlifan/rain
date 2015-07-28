#pragma once

#include "singleton.hpp"
#include "local_client_proxy.hpp"

namespace rain
{

class SharedServerProxy
    : public LocalClientProxy<SharedServerProxy>
    , public Singleton<SharedServerProxy>
{
    friend Singleton<SharedServerProxy>;
    SharedServerProxy() = default;

public:
    void dispatch_message(TargetSessionPtr, MessagePackPtr msgp)
    {

    }

    bool init()
    {
        return LocalClientProxy::init("SharedServer");
    }
};

} // !namespace rain
