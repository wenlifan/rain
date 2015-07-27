#pragma once

#include "singleton.hpp"
#include "common_app.hpp"
#include "data_server_proxy.hpp"
#include "game_server_proxy.hpp"

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
            DataServerProxy,
            GameServerProxy
        >(
            "SharedServer",
            "shared_server_config.lua",
            {
                "data_server",
                "game_server"
            }
        );
    }
};

} // !namespace rain
