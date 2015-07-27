#pragma once

#include "singleton.hpp"
#include "common_app.hpp"
#include "account_server_proxy.hpp"
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
            AccountServerProxy,
            GatewayServerProxy
        >(
            "LoginServer",
            "login_server_config.lua",
            {
                "account_server",
                "gateway_server"
            }
        );
    }
};

} // !namespace rain
