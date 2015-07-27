#pragma once

#include "singleton.hpp"
#include "common_app.hpp"
#include "game_server_proxy.hpp"
#include "login_server_proxy.hpp"

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
            LoginServerProxy
        >(
            "GatewayServer",
            "gateway_server_config.lua",
            {
                "game_server",
                "login_server",
            }
        );
    }
};

} // !namespace rain
