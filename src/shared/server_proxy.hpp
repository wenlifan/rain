#pragma once

#include <memory>

#include "basic_proxy.hpp"
#include "session.hpp"
#include "server_node.hpp"
#include "config_reader.hpp"

namespace rain
{

template <typename Proxy>
class ServerProxy
    : public BasicProxy
{
protected:
    using TargetSession = Session<Proxy>;
    using TargetSessionPtr = std::shared_ptr<TargetSession>;
    using TargetServerNode = ServerNode<TargetSession>;
    using TargetServerNodePtr = std::shared_ptr<TargetServerNode>;

    ServerProxy() = default;

protected:
    bool init(
        std::string const &ping_interval_str,
        std::string const &break_times_str,
        std::string const &port_str
    )
    {
        return init_params(ping_interval_str, break_times_str)
               && init_node(port_str);
    }

private:
    void error_handler(std::error_code const &err)
    {
        RAIN_WARN("Proxy server node error: " + err.message());
    }

    bool init_params(std::string const &ping_interval_str, std::string const &break_times_str)
    {
        auto &reader = ConfigReader::get_instance();

        int pi, bt;
        if (!reader.read_int(pi, ping_interval_str)) {
            RAIN_ERROR("Read PingInterval failed");
            return false;
        }
        if (!reader.read_int(bt, break_times_str)) {
            RAIN_ERROR("Read PingInterval failed");
            return false;
        }

        ping_interval_ = static_cast<std::size_t>(pi);
        break_times_ = static_cast<std::size_t>(bt);

        return true;
    }

    bool init_node(std::string const &port_str)
    {
        server_ = std::make_shared<TargetServerNode>();
        auto &reader = ConfigReader::get_instance();

        int port;
        if (!reader.read_int(port, port_str)) {
            RAIN_ERROR("Read " + port_str + " failed");
            return false;
        }

        RAIN_DEBUG("Listen port: " + std::to_string(port));

        server_->accept(asio::ip::address().to_string(),
                        static_cast<unsigned short>(port),
                        std::bind(&ServerProxy::error_handler,
                                  this,
                                  std::placeholders::_1
                        )
        );

        return true;
    }

protected:
    TargetServerNodePtr server_;
};

} // !namespace rain