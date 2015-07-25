#pragma once

#include <memory>

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
    using TargetClientNode = ClientNode<TargetSession>;
    using TargetClientNodePtr = std::shared_ptr<TargetClientNode>;

    ClientProxy() = default;

protected:
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
        RAIN_WARN("Proxy client node error: " + err.message());
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

    bool init_node(std::string const &ip_str, std::string const &port_str)
    {
        client_ = std::make_shared<TargetClientNode>();
        auto &reader = ConfigReader::get_instance();

        std::string ip;
        int port;
        if (!reader.read_string(ip, ip_str)) {
            RAIN_ERROR("Read " + ip_str + " failed");
            return false;
        }
        if (!reader.read_int(port, port_str)) {
            RAIN_ERROR("Read " + port_str + " failed");
            return false;
        }

        RAIN_DEBUG("Connect to server: " + ip + ':' + std::to_string(port));

        client_->connect(ip,
                         static_cast<unsigned short>(port),
                         std::bind(&ClientProxy::error_handler,
                                   this,
                                   std::placeholders::_1
                         )
        );

        return true;
    }

protected:
    TargetClientNodePtr client_;
};

} // !namespace rain