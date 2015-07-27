#pragma once

#include "singleton.hpp"
#include "local_client_proxy.hpp"

namespace rain
{

class GameServerProxy
    : public LocalClientProxy<GameServerProxy>
    , public Singleton<GameServerProxy>
{
    friend Singleton<GameServerProxy>;
    GameServerProxy() = default;

public:
    void dispatch_message(TargetSessionPtr session, MessagePackPtr msgp)
    {

    }

    bool init()
    {
        return LocalClientProxy::init("GameServer");
    }
};

} // !namespace rain
