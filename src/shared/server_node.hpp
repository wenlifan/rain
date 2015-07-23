#pragma once

#include <thread>
#include <memory>
#include <system_error>

//#include <iostream>

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
    ServerNode(unsigned short port = 59999) :
            iosp_(std::make_shared<IOService>()),
            acceptor_(*iosp_, tcp::endpoint(tcp::v4(), port))
    {
        auto n = std::thread::hardware_concurrency();
        //std::cout << "Threads: " << n << std::endl;
        work_threads_.resize(n > 0 ? n : 2);
    }

    ServerNode(ServerNode &&) = default;

    ~ServerNode()
    {
        stop();
    }

public:
    bool init()
    {
        return true;
    }

    void run()
    {
        for (auto &thread : work_threads_)
            if (!thread.joinable())
                thread = std::thread([this]{iosp_->run();});

        accept();
    }

private:
    void stop()
    {
        iosp_->stop();
        for (auto &thread : work_threads_)
            if (thread.joinable())
                thread.join();
    }

    void accept()
    {
        //std::cout << "Listen to " << acceptor_.local_endpoint() << std::endl;
        auto session = std::make_shared<Session>(iosp_);
        acceptor_.async_accept(session->get_socket(), [this, session](std::error_code const &err) {
            if (!err) {
                session->start();

                accept();
            }
            //std::cout << "accept err: " << err.message() << std::endl;
        });
    }

private:
    IOServicePtr iosp_;
    tcp::acceptor acceptor_;
    ThreadVector work_threads_;
};

} // !namespace rain