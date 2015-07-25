#pragma once

#include <thread>
#include <memory>
#include <system_error>

#include "asio.hpp"

#include "console.hpp"

namespace rain
{

using asio::ip::tcp;

template<typename Session>
class ServerNode
{
    using IOService = asio::io_service;
    using IOServicePtr = std::shared_ptr<IOService>;
    using ThreadVector = std::vector<std::thread>;
    using ErrorCallBack = std::function<void(std::error_code const &err)>;

public:
    ServerNode(std::size_t thread_count = 1)
        : iosp_(std::make_shared<IOService>())
        , work_keeper_(*iosp_)
        , acceptor_(*iosp_)
    {
        work_threads_.resize(thread_count);
        run();
    }

    ServerNode(ServerNode &&) = default;

    ~ServerNode()
    {
        stop();
    }

public:
    void accept(std::string const &ip, unsigned short port, ErrorCallBack const &call_back)
    {
        std::error_code err;
        tcp::endpoint ep(asio::ip::address_v4::from_string(ip), port);
        acceptor_.open(ep.protocol(), err);
        if (err) {
            RAIN_WARN("Acceptor open failed!");
            call_back(err);
            return;
        }
        acceptor_.bind(ep, err);
        if (err) {
            RAIN_WARN("Bind endpoint failed!");
            call_back(err);
            return;
        }
        acceptor_.listen(asio::socket_base::max_connections, err);
        if (err) {
            RAIN_WARN("Listen failed!");
            call_back(err);
            return;
        }

        do_accept(call_back);
    }

private:
    void do_accept(ErrorCallBack const &call_back)
    {
        auto session = std::make_shared<Session>(iosp_);
        acceptor_.async_accept(session->get_socket(),
                               [this, session, call_back](std::error_code const &err) {
                                   if (!err) {
                                       session->start();
                                       do_accept(call_back);
                                   } else {
                                       RAIN_WARN("Accept socket failed!");
                                       call_back(err);
                                   }
                               });
    }

    void run()
    {
        for (auto &thread : work_threads_)
            if (!thread.joinable())
                thread = std::thread([this]{iosp_->run();});
    }

    void stop()
    {
        // TODO: safe stop (shutdown socket first)
        iosp_->stop();
        for (auto &thread : work_threads_)
            if (thread.joinable())
                thread.join();
    }

private:
    IOServicePtr iosp_;
    IOService::work work_keeper_;
    ThreadVector work_threads_;
    tcp::acceptor acceptor_;
};

} // !namespace rain