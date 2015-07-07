#pragma once

#include <memory>
#include <system_error>
#include <unordered_set>
#include <array>
#include <cstring>

#include "asio.hpp"

#include "message_pack.hpp"

namespace rain
{

using asio::ip::tcp;

class Session
    : public std::enable_shared_from_this<Session>
{
    using IOService = asio::io_service;
    using IOServicePtr = std::shared_ptr<IOService>;

    using MessagePackPtr = std::shared_ptr<MessagePack>;
    using WriteSet = std::unordered_set<MessagePackPtr>;

public:
    Session(IOServicePtr ptr)
            : iosp_(ptr)
            , socket_(*ptr)
    {
        socket_.set_option(tcp::no_delay(true));
    }

    tcp::socket & getSocket()
    {
        return socket_;
    }

    void start()
    {
        do_read(std::make_shared<MessagePack>(NONE), 0);
    }

    void write(MessagePack pack)
    {
        auto msgp = std::make_shared<MessagePack>(std::move(pack));
        write_set_.insert(msgp);
        do_write(msgp, 0);
    }

private:
    void divide_message_pack(MessagePackPtr msgp,
                             std::size_t offset,
                             std::size_t buf_offset,
                             std::size_t bytes)
    {
        if (offset + bytes < 6)
        {
            std::memcpy(msgp->data() + offset, read_buf_.data() + buf_offset, bytes);
            do_read(msgp, offset + bytes);
        }
        else if (offset < 6)
        {
            auto ws = 6 - offset;
            std::memcpy(msgp->data() + offset, read_buf_.data() + buf_offset, ws);
            divide_message_pack(msgp, 6, buf_offset + ws, bytes - ws);
        }
        else if (offset + bytes < msgp->size())
        {
            msgp->write_size(read_buf_.data() + buf_offset, bytes);
            do_read(msgp, offset + bytes);
        }
        else
        {
            auto ws = msgp->size() - offset;
            msgp->write_size(read_buf_.data() + buf_offset, ws);
            // TODO move msgp into message pack dispatcher
            divide_message_pack(std::make_shared<MessagePack>(NONE), 0, buf_offset + ws, bytes - ws);
        }
    }

    void do_read(MessagePackPtr msgp, std::size_t offset)
    {
        socket_.async_read_some(
            asio::buffer(read_buf_),
            [this, msgp, offset](std::error_code const &err, std::size_t bytes) {
                if (!err)
                {
                    divide_message_pack(msgp, offset, 0, bytes);
                }
            });
    }

    void do_write(MessagePackPtr msgp, std::size_t offset)
    {
        socket_.async_write_some(
            asio::buffer(msgp->data() + offset, msgp->size() - offset),
            [this, msgp, offset](std::error_code const &err, std::size_t bytes) {
                if (!err)
                {
                    if (bytes < msgp->size() - offset)
                    {
                        do_write(msgp, offset + bytes);
                    }
                    else
                    {
                        write_set_.erase(msgp);
                    }
                }
            });
    }

private:
    IOServicePtr iosp_;
    tcp::socket socket_;
    WriteSet write_set_;
    std::array<char, 4096> read_buf_;
};

} // !namespace rain