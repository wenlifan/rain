#pragma once

#include "singleton.hpp"
#include "local_server_proxy.hpp"
#include "message_dispatcher.hpp"

namespace rain
{

class GatewayServerProxy
    : public LocalServerProxy<GatewayServerProxy>
    , public Singleton<GatewayServerProxy>
{
    friend Singleton<GatewayServerProxy>;
    GatewayServerProxy() = default;

public:
    void dispatch_message(TargetSessionPtr, MessagePackPtr msgp)
    {
        MessageDispatcher::get_instance().dispatch(msgp);
    }

    bool init()
    {
        return LocalServerProxy::init("GatewayServer");
    }
};

} // !namespace rain
