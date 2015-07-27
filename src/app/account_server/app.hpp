#pragma once

#include "singleton.hpp"
#include "common_app.hpp"
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
            LoginServerProxy
        >(
            "AccountServer",
            "account_server_config.lua",
            {
                "LoginServer",
            }
        );
    }
};

} // !namespace rain
