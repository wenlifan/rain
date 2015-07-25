#pragma once

#include <memory>
#include <system_error>
#include <unordered_set>
#include <array>
#include <cstring>
#include <atomic>

#include "asio.hpp"

#include "message_pack.hpp"

namespace rain
{

using asio::ip::tcp;

template <typename SessionManager>
class Session
    : public std::enable_shared_from_this<Session<SessionManager>>
{
    using IOService = asio::io_service;
    using IOServicePtr = std::shared_ptr<IOService>;

public:
    Session(IOServicePtr ptr)
            : iosp_(ptr)
            , socket_(*ptr)
            , timer_(*ptr)
    {
        ping_interval_ = SessionManager::get_instance().get_ping_interval();
        break_times_ = SessionManager::get_instance().get_break_times();
    }

    Session(Session const &) = delete;

    tcp::socket & get_socket()
    {
        return socket_;
    }

    void start()
    {
        socket_.set_option(tcp::no_delay(true));

        SessionManager::get_instance().add_session(this->shared_from_this());

        do_read(std::make_shared<MessagePack>(COMM_NONE), 0);

        do_ping();
    }

    void write(MessagePackPtr msgp)
    {
        do_write(msgp, 0);
    }

private:
    void divide_message_pack(MessagePackPtr msgp,
                             std::size_t offset,
                             std::size_t buf_offset,
                             std::size_t bytes)
    {
        constexpr auto hs = MessagePack::HeaderSize;
        if (offset + bytes < hs) {
            std::memcpy(msgp->data() + offset, read_buf_.data() + buf_offset, bytes);
            do_read(msgp, offset + bytes);
        }
        else if (offset < hs) {
            auto ws = hs - offset;
            std::memcpy(msgp->data() + offset, read_buf_.data() + buf_offset, ws);
            divide_message_pack(msgp, hs, buf_offset + ws, bytes - ws);
        }
        else if (offset + bytes < msgp->size()) {
            msgp->write_size(read_buf_.data() + buf_offset, bytes);
            do_read(msgp, offset + bytes);
        }
        else {
            auto ws = msgp->size() - offset;
            msgp->write_size(read_buf_.data() + buf_offset, ws);

            if (msgp->protocol() == COMM_PONG) {
                ping_times_ = 0;
                //std::cout << "Recieved PONG!" << std::endl;
            }
            else if (msgp->protocol() == COMM_PING) {
                do_pong();
                //std::cout << "Recieved PING!" << std::endl;
            }
            else
                SessionManager::get_instance().dispatch_message(this->shared_from_this(), msgp);

            //std::cout << "Package Data Size: " << msgp->data_size() << std::endl;

            divide_message_pack(std::make_shared<MessagePack>(COMM_NONE), 0, buf_offset + ws, bytes - ws);
        }
    }

    void do_read(MessagePackPtr msgp, std::size_t offset)
    {
        auto self = this->shared_from_this();
        socket_.async_read_some(
            asio::buffer(read_buf_),
            [this, self, msgp, offset](std::error_code const &err, std::size_t bytes) {
                if (!err) {
                    divide_message_pack(msgp, offset, 0, bytes);
                }
                //std::cout << "async_read_some err: " << err.message() << std::endl;
            });
    }

    void do_write(MessagePackPtr msgp, std::size_t offset)
    {
        auto self = this->shared_from_this();
        socket_.async_write_some(
            asio::buffer(msgp->data() + offset, msgp->size() - offset),
            [this, self, msgp, offset](std::error_code const &err, std::size_t bytes) {
                if (!err) {
                    if (bytes < msgp->size() - offset) {
                        do_write(msgp, offset + bytes);
                    }
                }

                //std::cout << "async_write_some err: " << err.message() << std::endl;
            });
    }

    void do_ping()
    {
        auto self = this->shared_from_this();
        if (ping_times_ >= break_times_) {
            SessionManager::get_instance().remove_session(self);
            return;
        }

        timer_.expires_from_now(std::chrono::milliseconds(ping_interval_));
        timer_.async_wait([this, self] (std::error_code const &err) {
            if (!err) {
                write(std::make_shared<MessagePack>(COMM_PING));
                do_ping();
                ping_times_++;
            }
        });
    }

    void do_pong()
    {
        write(std::make_shared<MessagePack>(COMM_PONG));
    }

private:
    IOServicePtr iosp_;
    tcp::socket socket_;
    asio::steady_timer timer_;
    std::array<char, 4096> read_buf_;

    std::size_t ping_interval_; // ms
    std::size_t break_times_;

    std::atomic_size_t ping_times_{0};
};

} // !namespace rain