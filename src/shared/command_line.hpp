#pragma once

#include <vector>
#include <mutex>
#include <iostream>
#include <atomic>
#include <chrono>

#include "singleton.hpp"
#include "thread_safe_queue.hpp"

namespace rain
{

enum CommandLineColor : char
{
    Black   = 0,
    Red     = 1,
    Green   = 2,
    Brown   = 3,
    Blue    = 4,
    Magenta = 5,
    Cyan    = 6,
    White   = 7,
    None    = 8,
};

class CommandLine : public Singleton<CommandLine>
{
    friend Singleton<CommandLine>;

    enum { ReadBufferSize = 128 };

private:
    CommandLine()
        : read_buf_(ReadBufferSize)
    {
        std::cout.sync_with_stdio(false);
        std::cin.sync_with_stdio(false);
        read_thread_ = std::thread([this]{
            std::size_t idx = 0;
            while (!close_read_)
            {
                auto lala = commands_.popAll();
                for (auto &i:lala)
                    write(i);
                write("Hehe from commandline read thread!", CommandLineColor::Magenta);
//                std::string a;
//                std::cin >> a;
//                write(a, CommandLineColor::Blue);

                auto rs = std::cin.readsome(read_buf_.data() + idx, read_buf_.size() - idx);
                if (rs > 0)
                {
                    auto find_lb = [this] (std::size_t pos) {
                        while (pos < read_buf_.size() && read_buf_[pos] != '\n')
                            ++pos;
                        return pos;
                    };
                    auto iter = find_lb(idx);
                    std::size_t last_lb_iter = 0;
                    while (iter != read_buf_.size())
                    {
                        commands_.push(std::string(read_buf_.data() + iter, iter));
                        last_lb_iter = iter;
                        iter = find_lb(iter);
                    }
                    auto cpn = idx + rs - last_lb_iter;
                    if (cpn >= read_buf_.size())
                    {
                        idx = 0;
                    }
                    else if (cpn > 0)
                    {
                        for (auto i = cpn; i > 0; --i)
                            read_buf_[cpn-i] = read_buf_[last_lb_iter+cpn-i];
                        idx = cpn;
                    }
                    else
                    {
                        idx += rs;
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        });
    }

    ~CommandLine()
    {
        close_read_ = true;
        read_thread_.join();
    }

public:
    void write(std::string const &str,
               CommandLineColor fore = CommandLineColor::None,
               CommandLineColor back = CommandLineColor::None,
               bool bold = true)
    {
        std::lock_guard<std::mutex> guard(write_mutex_);

        std::cout << "\x1b[";

        if (fore != CommandLineColor::None)
            std::cout << "3" << std::to_string(fore);

        if (back != CommandLineColor::None)
            std::cout << ";4" << std::to_string(back);

        if (bold)
            std::cout << ";1";

        std::cout << "m" << str << "\x1b[m" << std::endl;
    }




private:
    ThreadSafeQueue<std::string> commands_;
    std::mutex write_mutex_;
    std::vector<char> read_buf_;
    std::thread read_thread_;
    std::atomic_bool close_read_;
};

} // !namespace rain