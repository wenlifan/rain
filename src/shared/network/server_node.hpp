#pragma once

#include <thread>
#include <memory>
#include <system_error>

#include <iostream>

#include "asio.hpp"

namespace rain
{

using asio::ip::tcp;

template<typename Session>
class ServerNode
{
    using IOService = asio::io_service;
    using IOServicePtr = std::shared_ptr<IOService>;
    using ThreadVector = std::vector<std::thread>;

public:
    ServerNode(unsigned short port) :
            iosp_(std::make_shared<IOService>()),
            acceptor_(*iosp_, tcp::endpoint(tcp::v4(), port))
    {
        auto n = std::thread::hardware_concurrency();
        work_threads_.resize(n > 0 ? n : 2);
        accept();
    }

    ServerNode(ServerNode &&) = default;

    ~ServerNode()
    {
        stop();
    }

    void start()
    {
        for (auto &thread : work_threads_)
            if (!thread.joinable())
                thread = std::thread([this]{iosp_->run();});
    }

    void stop()
    {
        iosp_->stop();
        for (auto &thread : work_threads_)
            if (thread.joinable())
                thread.join();
    }

private:
    void accept()
    {
        auto session = std::make_shared<Session>(iosp_);
        acceptor_.async_accept(session->getSocket(), [this, session](std::error_code const &err) {
            if (!err) {
                // TODO process session
                std::cout << "hello, world!" << std::endl;

                accept();
            }
        });
    }

private:
    IOServicePtr iosp_;
    tcp::acceptor acceptor_;
    ThreadVector work_threads_;
};

} // !namespace rain