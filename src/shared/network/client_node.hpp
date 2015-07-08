#pragma once

#include <system_error>

#include <iostream>

#include "asio.hpp"

namespace rain
{

using asio::ip::tcp;

template <typename Session>
class ClientNode
{
    using IOService = asio::io_service;
    using IOServicePtr = std::shared_ptr<IOService>;

public:
    ClientNode()
            : iosp_(std::make_shared<IOService>())
            , trivial_work_(*iosp_)
    {
        start();
    }

    ClientNode(ClientNode &&) = default;

    ~ClientNode()
    {
        stop();
    }

    void connect(std::string const &ip, unsigned short port)
    {
        auto ep = tcp::endpoint(asio::ip::address::from_string(ip), port);
        auto session = std::make_shared<Session>(iosp_);
        session->getSocket().async_connect(ep, [session](std::error_code const &err) {
            if (!err)
            {
                session->start();
            }

            std::cout << "connect err: " << err.message() << std::endl;
        });
    }

private:
    void start()
    {
        work_thread_ = std::thread([this]{iosp_->run();});
    }

    void stop()
    {
        iosp_->stop();
        work_thread_.join();
    }

private:
    IOServicePtr iosp_;
    IOService::work trivial_work_;
    std::thread work_thread_;
};

} // !namespace rain