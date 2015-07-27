#pragma once

#include <memory>
#include <sstream>

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
    using TargetNode = ServerNode<TargetSession>;
    using TargetNodePtr = std::unique_ptr<TargetNode>;

    ServerProxy() = default;

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
        RAIN_ERROR("Proxy server node error: " + err.message());
    }

    bool init_node(std::string const &port_str)
    {
        node_ = std::make_unique<TargetNode>();
        auto &reader = ConfigReader::get_instance();

        int port;
        if (!reader.read_int(port, port_str)) {
            RAIN_ERROR("Read " + port_str + " failed");
            return false;
        }

        std::stringstream ss;
        ss << port;
        RAIN_DEBUG("Listen port: " + ss.str());

        node_->accept(asio::ip::address().to_string(),
                        static_cast<unsigned short>(port),
                        std::bind(&ServerProxy::error_handler,
                                  this,
                                  std::placeholders::_1
                        )
        );

        return true;
    }

private:
    TargetNodePtr node_;
};

} // !namespace rain