#pragma once

#include "singleton.hpp"
#include "config_reader.hpp"
#include "console.hpp"
#include "login_server_proxy.hpp"

namespace rain
{

class App : public Singleton<App>
{
    friend Singleton<App>;
    App() = default;

public:
    void run()
    {
        auto &console = Console::get_instance();
        console.set_name("AccountServer");

        try {
            init_all();
        } catch (std::exception &err) {
            RAIN_ERROR(std::string("Init server occurs Exception: ") + err.what());
        } catch (...) {
            RAIN_ERROR("Note: Init server occurs [Unknown] Exception.");
        }

        console.run();

        try {
            stop_all();
        } catch (std::exception &err) {
            RAIN_ERROR(std::string("Stop server occurs Exception: ") + err.what());
        } catch (...) {
            RAIN_ERROR("Note: Stop server occurs [Unknown] Exception.");
        }
    }

private:
    void init_all()
    {
        RAIN_INFO("Load server config file...");
        if (!ConfigReader::get_instance().load_config("account_server_config.lua")) {
            RAIN_ERROR("Load server config error! Server start failed!");
            return;
        }

        RAIN_INFO("Init login_server proxy...");
        if (!LoginServerProxy::get_instance().init()) {
            RAIN_ERROR("Init login_server proxy error! Server start failed!");
            return;
        }

        // TODO: start logic main thread

        RAIN_INFO("Server start success!");
    }

    void stop_all()
    {
        RAIN_INFO("Stop login_server proxy...");
        LoginServerProxy::get_instance().stop();
    }

private:
};

} // !namespace rain