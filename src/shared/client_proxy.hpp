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
{
protected:
    using TargetSession = Session<Proxy>;
    using TargetSessionPtr = std::shared_ptr<TargetSession>;
    using TargetNode = ClientNode<TargetSession>;
    using TargetNodePtr = std::shared_ptr<TargetNode>;

    bool init(
        std::string const &ping_interval_str,
        std::string const &break_times_str,
        std::string const &ip_str,
        std::string const &port_str
    )
    {
        run_flag_.test_and_set();
        return init_reconn_thread()
               && init_params(ping_interval_str, break_times_str)
               && init_node(ip_str, port_str);
    }

    void connect()
    {
        node_->connect(ip_,
                       port_,
                       std::bind(&ClientProxy::error_handler,
                                 this,
                                 std::placeholders::_1
                       )
        );
    }

    void stop_reconn_thread()
    {
        run_flag_.clear();
        if (reconn_thread_.joinable())
            reconn_thread_.join();
    }

private:
    void error_handler(std::error_code const &err)
    {
        RAIN_DEBUG("Proxy client node error: " + err.message());

        reconn_flag_.clear();
    }

    bool init_reconn_thread()
    {
        if (!reconn_thread_.joinable()) {
            reconn_thread_ = std::thread([this] {
                for (;;) {
                    if (!run_flag_.test_and_set())
                        break;

                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    if (!reconn_flag_.test_and_set())
                        connect();
                }
            });
            return true;
        }
        return false;
    }

    bool init_node(std::string const &ip_str, std::string const &port_str)
    {
        node_ = std::make_shared<TargetNode>();
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

        connect();

        return true;
    }

private:
    TargetNodePtr node_;

    std::atomic_flag run_flag_{ATOMIC_FLAG_INIT};
    std::atomic_flag reconn_flag_{ATOMIC_FLAG_INIT};

    std::thread reconn_thread_;

    std::string ip_;
    unsigned short port_;
};

} // !namespace rain