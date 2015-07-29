#pragma once

#include "singleton.hpp"
#include "local_server_proxy.hpp"
#include "logic_system.hpp"

namespace rain
{

class GameServerProxy
    : public LocalServerProxy<GameServerProxy, LogicSystem>
    , public Singleton<GameServerProxy>
{
    friend Singleton<GameServerProxy>;
    GameServerProxy() = default;

public:
    bool init()
    {
        return LocalServerProxy::init("GameServer");
    }
};

} // !namespace rain
