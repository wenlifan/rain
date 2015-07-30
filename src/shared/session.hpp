#pragma once

#include <memory>
#include <system_error>
#include <array>
#include <cstring>
#include <atomic>
#include <cstdint>

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

    void remove()
    {
        if (!remove_flag_.test_and_set()) {
            ping_times_ = 10000;
            SessionMgr::get_instance().remove_session(this->shared_from_this());
            socket_.close();
        }
    }

private:
    void do_divide_pack(MessagePackPtr msgp,
                        std::size_t offset,
                        std::size_t buf_offset,
                        std::size_t bytes)
    {
        constexpr auto hs = sizeof(MessageHeader);
        if (offset + bytes < hs) {
            msgp->pos_write_size(offset, read_buf_.data() + buf_offset, bytes);
            do_read(msgp, offset + bytes);
        } else if (offset < hs) {
            auto ws = hs - offset;
            msgp->pos_write_size(offset, read_buf_.data() + buf_offset, ws);
            if (msgp->pack_size() > 1024 * 1024 * 256) {
                remove();
                return;
            }
            do_divide_pack(msgp, hs, buf_offset + ws, bytes - ws);
        } else if (offset + bytes < msgp->pack_size()) {
            if (msgp->pack_size() > 1024 * 1024 * 256) {
                remove();
                return;
            }
            msgp->write_size(read_buf_.data() + buf_offset, bytes);
            do_read(msgp, offset + bytes);
        } else {
            do_dispatch(msgp, offset, buf_offset, bytes);
        }
    }

    void do_read(MessagePackPtr msgp, std::size_t offset)
    {
        auto self = this->shared_from_this();
        socket_.async_read_some(
            asio::buffer(read_buf_),
            [this, self, msgp, offset](std::error_code const &err, std::size_t bytes) {
                if (!err) {
                    do_divide_pack(msgp, offset, 0, bytes);
                } else {
                    RAIN_DEBUG("Session read data failed: " + err.message());
                    remove();
                }
            });
    }

    void do_write(MessagePackPtr msgp, std::size_t offset)
    {
        auto self = this->shared_from_this();
        socket_.async_write_some(
            asio::buffer(msgp->data() + offset, msgp->pack_size() - offset),
            [this, self, msgp, offset](std::error_code const &err, std::size_t bytes) {
                if (!err) {
                    if (bytes < msgp->pack_size() - offset) {
                        do_write(msgp, offset + bytes);
                    } else {
                        SessionMgr::get_instance().send_message_finished(self, msgp);
                    }
                } else {
                    RAIN_DEBUG("Session write data failed: " + err.message());
                    remove();
                }
            }
        );
    }

    void do_ping()
    {
        if (ping_times_ >= break_times_) {
            remove();
            return;
        }

        auto self = this->shared_from_this();
        timer_.expires_from_now(std::chrono::milliseconds(ping_interval_));
        timer_.async_wait([this, self] (std::error_code const &err) {
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

    void do_feedback(std::int32_t id)
    {
        auto msgp = std::make_shared<MessagePack>(COMM_FEEDBACK);
        msgp->write(id);
        msgp->flush();
        write(msgp);
    }

    bool do_wrapper(MessagePackPtr msgp)
    {
        std::uint32_t id = 0;
        msgp->read(id);
        if (msgp->avail() < sizeof(MessageHeader)) {
            remove();
            return false;
        }
        do_feedback(id);

        auto nmsgp = std::make_shared<MessagePack>(COMM_NONE, msgp->avail());
        nmsgp->pos_write_size(0, msgp->data() + msgp->read_ptr(), msgp->avail());
        nmsgp->write_ptr(int(msgp->avail() - sizeof(MessageHeader)));
        SessionMgr::get_instance().dispatch_message(this->shared_from_this(), nmsgp);
        return true;
    }

    void do_dispatch(MessagePackPtr msgp,
                     std::size_t offset,
                     std::size_t buf_offset,
                     std::size_t bytes)
    {
        auto ws = msgp->pack_size() - offset;
        msgp->write_size(read_buf_.data() + buf_offset, ws);

        if (msgp->protocol() == COMM_PING) {
            do_pong();
        } else if (msgp->protocol() == COMM_PONG) {
            ping_times_ = 0;
        } else if (msgp->protocol() == COMM_FEEDBACK_WRAPPER) {
            if (!do_wrapper(msgp)) {
                return;
            }
        } else {
            SessionMgr::get_instance().dispatch_message(this->shared_from_this(), msgp);
        }
        //std::cout << "Package Data Size: " << msgp->data_size() << std::endl;

        do_divide_pack(std::make_shared<MessagePack>(COMM_NONE), 0, buf_offset + ws, bytes - ws);
    }

private:
    IOServicePtr iosp_;
    tcp::socket socket_;
    asio::steady_timer timer_;

    std::array<char, 1024> read_buf_;

    std::atomic_flag remove_flag_{ATOMIC_FLAG_INIT};
    std::atomic_size_t ping_times_{0};

    std::size_t ping_interval_; // ms
    std::size_t break_times_;
};

} // !namespace rain