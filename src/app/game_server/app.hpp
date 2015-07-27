#pragma once

#include "singleton.hpp"
#include "common_app.hpp"
#include "data_server_proxy.hpp"
#include "shared_server_proxy.hpp"
#include "gateway_server_proxy.hpp"

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
            SharedServerProxy,
            GatewayServerProxy
        >(
            "GameServer",
            "game_server_config.lua",
            {
                "data_server",
                "shared_server",
                "gateway_server"
            }
        );
    }
};

} // !namespace rain