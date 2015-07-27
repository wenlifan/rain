#pragma once

#include <string>
#include <vector>

#include "config_reader.hpp"
#include "console.hpp"

namespace rain
{

class CommonApp
{
public:
    template <typename ...Proxys>
    void run(
        std::string const &app_name,
        std::string const &config_name,
        std::vector<std::string> const &proxy_name
    )
    {
        auto &console = Console::get_instance();
        console.set_name(app_name);

        try {
            RAIN_INFO("Load server config file...");
            if (!ConfigReader::get_instance().load_config(config_name)) {
                RAIN_ERROR("Load server config error! Server start failed!");
            } else if (init_all<0, Proxys...>(proxy_name)) {
                RAIN_INFO("Server start success!");
            }
        } catch (std::exception &err) {
            RAIN_ERROR(std::string("Init server occurs Exception: ") + err.what());
        } catch (...) {
            RAIN_ERROR("Note: Init server occurs [Unknown] Exception.");
        }

        console.run();

        try {
            stop_all<0, Proxys...>(proxy_name);
        } catch (std::exception &err) {
            RAIN_ERROR(std::string("Stop server occurs Exception: ") + err.what());
        } catch (...) {
            RAIN_ERROR("Note: Stop server occurs [Unknown] Exception.");
        }
    }

private:
    template <std::size_t N>
    bool init_all(std::vector<std::string> const &)
    {
        return true;
    }

    template <std::size_t N, typename Proxy, typename ...Proxys>
    bool init_all(std::vector<std::string> const &proxy_name)
    {
        auto &pn = proxy_name[N];
        RAIN_INFO("Init " + pn + " proxy...");
        if (!Proxy::get_instance().init()) {
            RAIN_ERROR("Init " + pn + " proxy error! Server start failed!");
            return false;
        }

        return init_all<N+1, Proxys...>(proxy_name);
    }

    template <std::size_t N>
    void stop_all(std::vector<std::string> const &)
    {
    }

    template <std::size_t N, typename Proxy, typename ...Proxys>
    void stop_all(std::vector<std::string> const &proxy_name)
    {
        stop_all<N+1, Proxys...>(proxy_name);

        auto &pn = proxy_name[N];
        RAIN_INFO("Stop " + pn + " proxy...");
        Proxy::get_instance().stop();
    }
};

} // !namespace rain