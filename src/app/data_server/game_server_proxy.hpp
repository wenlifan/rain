#pragma once

#include "singleton.hpp"
#include "local_server_proxy.hpp"

namespace rain
{

class GameServerProxy
    : public LocalServerProxy<GameServerProxy>
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
        return LocalServerProxy::init("GameServer");
    }
};

} // !namespace rain
