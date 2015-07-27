#pragma once

#include "singleton.hpp"
#include "common_app.hpp"
#include "game_server_proxy.hpp"
#include "shared_server_proxy.hpp"

namespace rain
{

class App
    : public CommonApp
    , public Singleton<App>
{
    friend Singleton<App>;
    App() = default;

public:
    void run()
    {
        CommonApp::run<
            GameServerProxy,
            SharedServerProxy
        >(
            "DataServer",
            "data_server_config.lua",
            {
                "GameServer",
                "SharedServer"
            }
        );
    }
};

} // !namespace rain
