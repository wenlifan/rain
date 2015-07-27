#pragma once

#include <system_error>
#include <memory>
#include <thread>
#include <sstream>

#include "asio.hpp"

namespace rain
{

using asio::ip::tcp;

template <typename Session>
class ClientNode
{
    using IOService = asio::io_service;
    using IOServicePtr = std::shared_ptr<IOService>;
    using ErrorCallBack = std::function<void(std::error_code const &err)>;

public:
    ClientNode()
        : iosp_(std::make_shared<IOService>())
        , work_keeper_(*iosp_)
    {
        run();
    }

    ~ClientNode()
    {
        stop();
    }

public:
    void connect(std::string const &ip, unsigned short port, ErrorCallBack const &call_back)
    {
        auto ep = tcp::endpoint(asio::ip::address::from_string(ip), port);
        auto session = std::make_shared<Session>(iosp_);
        session->get_socket().async_connect(
            ep,
            [session, call_back, ip, port](std::error_code const &err) {
                if (!err) {
                    session->start();
                } else {
                    std::stringstream ss;
                    ss << port;
                    RAIN_WARN("Connect to server " + ip + ':' + ss.str() + " failed.");
                    call_back(err);
                }
            });
    }

private:
    void run()
    {
        if (!work_thread_.joinable())
            work_thread_ = std::thread([this]{iosp_->run();});
    }

    void stop()
    {
        iosp_->stop();
        if (work_thread_.joinable())
            work_thread_.join();
    }

private:
    IOServicePtr iosp_;
    IOService::work work_keeper_;
    std::thread work_thread_;
};

} // !namespace rain