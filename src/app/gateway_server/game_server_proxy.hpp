#pragma once

#include "singleton.hpp"
#include "local_client_proxy.hpp"
#include "logic_system.hpp"

namespace rain
{

class GameServerProxy
    : public LocalClientProxy<GameServerProxy, LogicSystem>
    , public Singleton<GameServerProxy>
{
    friend Singleton<GameServerProxy>;
    GameServerProxy() = default;

public:
    bool init()
    {
        return LocalClientProxy::init("GameServer");
    }
};

} // !namespace rain
