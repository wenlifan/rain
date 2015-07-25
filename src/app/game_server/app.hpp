#pragma once

#include "singleton.hpp"
#include "config_reader.hpp"
#include "console.hpp"
#include "gateway_proxy.hpp"
#include "data_server_proxy.hpp"
#include "shared_server_proxy.hpp"

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
        console.set_name("GameServer");

        try {
            init_all();
        } catch (std::exception &err) {
            RAIN_ERROR(std::string("Init server occurs Exception: ") + err.what());
        } catch (...) {
            RAIN_ERROR("Note: Init server occurs [Unknown] Exception.");
        }

        console.run();

        stop_all();
    }

private:
    void init_all()
    {
        // load config
        RAIN_INFO("Load server config file...");
        if (!ConfigReader::get_instance().load_config("game_server_config.lua")) {
            RAIN_ERROR("Load server config error! Server start failed!");
            return;
        }

        // proxys init
        RAIN_INFO("Init gateway_server proxy...");
        if (!GatewayProxy::get_instance().init()) {
            RAIN_ERROR("Init gateway_server proxy error! Server start failed!");
            return;
        }

        RAIN_INFO("Init data_server proxy...");
        if (!DataServerProxy::get_instance().init()) {
            RAIN_ERROR("Init data_server proxy error! Server start failed!");
            return;
        }

        RAIN_INFO("Init shared_server proxy...");
        if (!SharedServerProxy::get_instance().init()) {
            RAIN_ERROR("Init shared_server proxy error! Server start failed!");
            return;
        }

        // TODO: start logic main thread

        RAIN_INFO("Server start success!");
    }

    void stop_all()
    {
    }

private:
};

} // !namespace rain