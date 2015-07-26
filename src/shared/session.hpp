#pragma once

#include <memory>
#include <system_error>
#include <unordered_set>
#include <array>
#include <cstring>
#include <atomic>

#include "asio.hpp"

#include "message_pack.hpp"
#include "console.hpp"

namespace rain
{

using asio::ip::tcp;

template <typename SessionMgr>
class Session
    : public std::enable_shared_from_this<Session<SessionMgr>>
{
    using IOService = asio::io_service;
    using IOServicePtr = std::shared_ptr<IOService>;

    enum ShutdownType : char {
        ST_None        = 0,
        ST_Read        = 1,
        ST_Write       = 2,
        ST_Both        = 3,
    };

public:
    Session(IOServicePtr ptr)
            : iosp_(ptr)
            , socket_(*ptr)
            , timer_(*ptr)
    {
    }

    Session(Session const &) = delete;

    tcp::socket & get_socket()
    {
        return socket_;
    }

    void start()
    {
        socket_.set_option(tcp::no_delay(true));
        auto &sm = SessionMgr::get_instance();

        ping_interval_ = sm.get_ping_interval();
        break_times_ = sm.get_break_times();
        sm.add_session(this->shared_from_this());

        do_read(std::make_shared<MessagePack>(COMM_NONE), 0);
        do_ping();
    }

    void write(MessagePackPtr msgp)
    {
        do_write(msgp, 0);
    }

    void shutdown(tcp::socket::shutdown_type type)
    {
        std::error_code err;

        if (type == tcp::socket::shutdown_send) {
            shutdown_type_ |= ST_Write;
        } else if (type == tcp::socket::shutdown_receive) {
            shutdown_type_ |= ST_Read;
        } else if (type == tcp::socket::shutdown_both) {
            shutdown_type_ |= ST_Both;
        }

        socket_.shutdown(type, err);
        if (err) {
            RAIN_ERROR("Socket shutdown error: " + err.message());
        }
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
        } else if (offset < hs) {
            auto ws = hs - offset;
            std::memcpy(msgp->data() + offset, read_buf_.data() + buf_offset, ws);
            divide_message_pack(msgp, hs, buf_offset + ws, bytes - ws);
        } else if (offset + bytes < hs + msgp->data_size()) {
            msgp->raw_write_size(read_buf_.data() + buf_offset, bytes);
            do_read(msgp, offset + bytes);
        } else {
            auto ws = hs + msgp->data_size() - offset;
            msgp->raw_write_size(read_buf_.data() + buf_offset, ws);

            if (msgp->protocol() == COMM_PONG) {
                ping_times_ = 0;
                //std::cout << "Recieved PONG!" << std::endl;
            } else if (msgp->protocol() == COMM_PING) {
                do_pong();
                //std::cout << "Recieved PING!" << std::endl;
            } else {
                SessionMgr::get_instance().dispatch_message(this->shared_from_this(), msgp);
            }
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
                } else if (err == asio::error::eof) {
                    shutdown_type_ |= ST_Read;
                    SessionMgr::get_instance().session_read_eof(self);
                } else if (!(shutdown_type_ & ST_Read)) {
                    RAIN_DEBUG("Session read data failed: " + err.message());
                    do_remove_session();
                }
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
                    } else {
                        SessionMgr::get_instance().send_message_finished(self, msgp);
                    }
                } else if (!(shutdown_type_ & ST_Write)) {
                    RAIN_DEBUG("Session write data failed: " + err.message());
                    do_remove_session();
                }
            }
        );
    }

    void do_ping()
    {
        if (ping_times_ >= break_times_) {
            do_remove_session();
            return;
        }

        auto self = this->shared_from_this();
        timer_.expires_from_now(std::chrono::milliseconds(ping_interval_));
        timer_.async_wait([this, self] (std::error_code const &err) {
            if (shutdown_type_ != ST_None)
                return;

            if (!err) {
                write(std::make_shared<MessagePack>(COMM_PING));
                do_ping();
                ping_times_++;
            } else {
                RAIN_ERROR("Timer error: " + err.message());
            }
        });
    }

    void do_pong()
    {
        write(std::make_shared<MessagePack>(COMM_PONG));
    }

    void do_remove_session()
    {
        if (!remove_flag_) {
            SessionMgr::get_instance().remove_session(this->shared_from_this());
            remove_flag_ = true;
            ping_times_ = 10000;
        }
    }

private:
    IOServicePtr iosp_;
    tcp::socket socket_;
    asio::steady_timer timer_;

    std::array<char, 1024> read_buf_;

    std::atomic_char shutdown_type_{ST_None};
    std::atomic_bool remove_flag_{false};
    std::atomic_size_t ping_times_{0};

    std::size_t ping_interval_; // ms
    std::size_t break_times_;
};

} // !namespace rain