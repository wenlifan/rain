#pragma once

#include <memory>

#include "asio.hpp"

namespace rain
{

class BaseSession : public std::enable_shared_from_this<BaseSession>
{
    using IOService = asio::io_service;
    using IOServicePtr = std::shared_ptr<IOService>;
    using Socket = asio::ip::tcp::socket;

public:
    BaseSession(IOServicePtr ptr) :
            iosp_(ptr),
            socket_(*ptr)
    {
    }

    Socket & getSocket()
    {
        return socket_;
    }

private:
    IOServicePtr iosp_;
    Socket socket_;
};

} // !namespace rain