#pragma once

#include <memory>
#include <chrono>
#include <thread>
#include <atomic>

#include "basic_proxy.hpp"
#include "session.hpp"
#include "client_node.hpp"
#include "config_reader.hpp"

namespace rain
{

template <typename Proxy>
class ClientProxy
    : public BasicProxy
    , public std::enable_shared_from_this<ClientProxy<Proxy>>
{
    using TargetSession = Session<Proxy>;
    using TargetSessionPtr = std::shared_ptr<TargetSession>;
    using TargetClientNode = ClientNode<TargetSession>;
    using TargetClientNodePtr = std::shared_ptr<TargetClientNode>;

public:
    ~ClientProxy()
    {
        RAIN_DEBUG("Come's here");
        exit_ = true;
        if (auto_conn_thread_.joinable())
            auto_conn_thread_.join();

        RAIN_DEBUG("Come's here, too");
    }

public:
    bool init(
        std::string const &ping_interval_str,
        std::string const &break_times_str,
        std::string const &ip_str,
        std::string const &port_str
    )
    {
        return init_params(ping_interval_str, break_times_str)
               && init_node(ip_str, port_str);
    }

private:
    void error_handler(std::error_code const &err)
    {
        RAIN_DEBUG("Proxy client node error: " + err.message());

        if (!exit_)
        {
            if (auto_conn_thread_.joinable())
                auto_conn_thread_.join();

            auto self = this->shared_from_this();
            auto_conn_thread_ = std::thread([this, self] {
                std::this_thread::sleep_for(std::chrono::milliseconds(ping_interval_));
                do_connect();
            });
        }
    }

    bool init_node(std::string const &ip_str, std::string const &port_str)
    {
        client_ = std::make_shared<TargetClientNode>();
        auto &reader = ConfigReader::get_instance();

        int port;
        if (!reader.read_string(ip_, ip_str)) {
            RAIN_ERROR("Read " + ip_str + " failed");
            return false;
        }
        if (!reader.read_int(port, port_str)) {
            RAIN_ERROR("Read " + port_str + " failed");
            return false;
        }
        port_ = static_cast<unsigned short>(port);

        RAIN_DEBUG("Connect to server: " + ip_ + ':' + std::to_string(port_));

        do_connect();

        return true;
    }

    void do_connect()
    {
        client_->connect(ip_,
                         port_,
                         std::bind(&ClientProxy::error_handler,
                                   this,
                                   std::placeholders::_1
                         )
        );
    }

private:
    TargetClientNodePtr client_;
    std::thread auto_conn_thread_;
    std::atomic_bool exit_{false};
    std::string ip_;
    unsigned short port_;
};

} // !namespace rain