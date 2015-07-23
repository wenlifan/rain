#pragma once

#include "singleton.hpp"
#include "config_reader.hpp"
#include "console.hpp"
#include "server_session_manager.hpp"
#include "server_node.hpp"
#include "client_node.hpp"
#include "gateway_proxy.hpp"
#include "data_server_proxy.hpp"
#include "shared_server_proxy.hpp"

namespace rain
{

class App : public Singleton<App>
{
    friend Singleton<App>;
    App() = default;

    using GWServerType = ServerNode<Session<ServerSessionManager>>;
    using DSClientType = ClientNode<Session<DataServerProxy>>;
    using SSClientType = ClientNode<Session<SharedServerProxy>>;

public:
    void run()
    {
        auto &console = Console::get_instance();
        console.set_name("GameServer");

        do_run();
        console.run();
        do_stop();
    }

    ConfigReader & get_reader()
    {
        return reader_;
    }

private:
    void do_run()
    {
        // load config
        RAIN_INFO("Load server config file...");
        if (!reader_.load_config("game_server_config.lua")) {
            RAIN_ERROR("Load server config error! Server start failed!");
            return;
        }
        RAIN_INFO("Success!");

        // session manager init
        RAIN_INFO("Init ServerSessionManager...");
        if (!ServerSessionManager::get_instance().init()) {
            RAIN_ERROR("Init ServerSessionManager error! Server start failed!");
            return;
        }
        RAIN_INFO("Success!");

        // server node init
        RAIN_INFO("Init gateway_server server node...");
        if (!gw_server_.init()) {
            RAIN_ERROR("Init gateway_server server node error! Server start failed!");
            return;
        }
        RAIN_INFO("Success!");

        // client nodes init
        RAIN_INFO("Init data_server client node...");
        if (!ds_client_.init()) {
            RAIN_ERROR("Init data_server client node error! Server start failed!");
            return;
        }
        RAIN_INFO("Success!");

        RAIN_INFO("Init shared_server client node...");
        if (!ss_client_.init()) {
            RAIN_ERROR("Init shared_server client node error! Server start failed!");
            return;
        }
        RAIN_INFO("Success!");

        // start all server
        gw_server_.run();
        ds_client_.run();
        ss_client_.run();

        // load lua scripts
        // start logic main thread
    }

    void do_stop()
    {
    }

private:
    ConfigReader reader_;
    GWServerType gw_server_;
    DSClientType ds_client_;
    SSClientType ss_client_;
};

} // !namespace rain