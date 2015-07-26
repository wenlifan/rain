#pragma once

#include <memory>
#include <chrono>
#include <thread>
#include <atomic>
#include <condition_variable>

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
        return init_reconn_thread()
               && init_params(ping_interval_str, break_times_str)
               && init_node(ip_str, port_str);
    }

    void stop_reconn_thread()
    {
        exit_ = true;
        if (conn_thread_.joinable())
            conn_thread_.join();
    }

private:
    void error_handler(std::error_code const &err)
    {
        RAIN_DEBUG("Proxy client node error: " + err.message());

        conn_mutex_.lock();
        reconn_ = true;
        conn_mutex_.unlock();
        conn_var_.notify_one();
    }

    bool init_reconn_thread()
    {
        if (!conn_thread_.joinable()) {
            conn_thread_ = std::thread([this] {
                std::unique_lock<std::mutex> ulk(conn_mutex_);
                for (;;) {
                    conn_var_.wait(ulk, [this]{return reconn_;});
                    if (exit_)
                        break;

                    reconn_ = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    do_connect();
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

        do_connect();

        return true;
    }

    void do_connect()
    {
        node_->connect(ip_,
                       port_,
                       std::bind(&ClientProxy::error_handler,
                                 this,
                                 std::placeholders::_1
                       )
        );
    }

private:
    TargetNodePtr node_;

    std::atomic_bool exit_{false};

    std::thread conn_thread_;
    std::mutex conn_mutex_;
    std::condition_variable conn_var_;
    bool reconn_{false};

    std::string ip_;
    unsigned short port_;
};

} // !namespace rain