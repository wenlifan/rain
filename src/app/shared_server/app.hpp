#pragma once

#include "singleton.hpp"
#include "config_reader.hpp"
#include "console.hpp"
#include "game_server_proxy.hpp"
#include "data_server_proxy.hpp"

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
        console.set_name("SharedServer");

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
        if (!ConfigReader::get_instance().load_config("shared_server_config.lua")) {
            RAIN_ERROR("Load server config error! Server start failed!");
            return;
        }

        RAIN_INFO("Init data_server proxy...");
        if (!DataServerProxy::get_instance().init()) {
            RAIN_ERROR("Init data_server proxy error! Server start failed!");
            return;
        }

        RAIN_INFO("Init game_server proxy...");
        if (!GameServerProxy::get_instance().init()) {
            RAIN_ERROR("Init game_server proxy error! Server start failed!");
            return;
        }

        // TODO: start logic main thread

        RAIN_INFO("Server start success!");
    }

    void stop_all()
    {
        RAIN_INFO("Stop game_server proxy...");
        GameServerProxy::get_instance().stop();

        RAIN_INFO("Stop data_server proxy...");
        DataServerProxy::get_instance().stop();
    }

private:
};

} // !namespace rain