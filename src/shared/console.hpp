#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <list>
#include <cstdio>
#include <cctype>
#include <array>
#include <ctime>
#include <thread>
#include <atomic>
#include <chrono>
#include <utility>

extern "C"
{
#include <termios.h>
#include <unistd.h>
}

#include "singleton.hpp"
#include "thread_safe_queue.hpp"

namespace rain
{

enum class ConsoleColor : char
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

enum class ConsoleKey : char
{
    Unknown,
    Printable,
    Return,
    Backspace,
    Delete,
    Left,
    Right,
    Up,
    Down,
};

class Console : public Singleton<Console>
{
    friend Singleton<Console>;

    enum
    {
        ReadBufferSize = 32,
        HistoryCount = 100,
    };

private:
    Console()
    {
        history_iter_ = history_.end();

        set_term();
        set_status();
        reset_time();

        time_thread_ = std::thread([this] {
            while (!exit_time_thread_) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                reset_time();
                flush_status_line();
            }
        });
    }

    ~Console()
    {
        reset_term();
        exit_time_thread_ = true;
        if (time_thread_.joinable())
            time_thread_.join();
    }
public:

    void write_line(std::string const &str,
               ConsoleColor fore = ConsoleColor::None,
               ConsoleColor back = ConsoleColor::None,
               bool bold = true)
    {
        write_mutex_.lock();
        std::printf("\r\x1b[K");
        std::fflush(stdout);
        time_mutex_.lock();
        std::printf("%s ", time_.c_str());
        time_mutex_.unlock();
        std::printf("\x1b[");

        if (fore != ConsoleColor::None)
            std::printf("3%d", (int)fore);

        if (back != ConsoleColor::None)
            std::printf(";4%d",(int)back);

        if (bold)
            std::printf(";1");

        std::printf("m%s\x1b[m\n", str.c_str());
        write_mutex_.unlock();

        flush_status_line();
    }

    void set_status(float load = 0.0)
    {
        char buf[32] = {};
        std::sprintf(buf, " %5.2f%% ", load);
        std::lock_guard<std::mutex> guard(status_mutex_);
        status_ = buf;
    }

    void set_name(std::string name)
    {
        name_ = std::move(name);
    }

    void run()
    {
        flush_status_line();
        char c;
        for (;;) {
            auto key = get_key(c);
            if (key == ConsoleKey::Printable)
                input_char(c);
            else if (key == ConsoleKey::Return) {
                if (push_command())
                    break;
            }
            else if (key == ConsoleKey::Backspace)
                back_delete();
            else if (key == ConsoleKey::Delete)
                forward_delete();
            else if (key == ConsoleKey::Up)
                prev_history();
            else if (key == ConsoleKey::Down)
                next_history();
            else if (key == ConsoleKey::Left)
                move_left();
            else if (key == ConsoleKey::Right)
                move_right();
        }
//        auto cmds = commands_.pop_all();
//        for (auto &i : cmds)
//            write_line(i, ConsoleColor::Magenta);
    }

private:
    void set_term()
    {
        tcgetattr(0, &term_tmp_);
        termios term = term_tmp_;
        term.c_lflag &= (~(ICANON | ECHO | ISIG));
        term.c_cc[VTIME] = 0;
        term.c_cc[VMIN] = 1;
        tcsetattr(0, TCSANOW, &term);
    }

    void reset_term()
    {
        tcsetattr(0, TCSANOW, &term_tmp_);
    }

    ConsoleKey get_key(char &ch)
    {
        auto key = ConsoleKey::Unknown;
        auto n = read(0, read_buf_.data(), read_buf_.size());
        if (n == 1) {
            auto c = read_buf_[0];
            if (c == 10)
                key = ConsoleKey::Return;
            else if (c == 127)
                key = ConsoleKey::Backspace;
            else if (std::isprint(c))
                ch = c, key = ConsoleKey::Printable;
        }
        else if (n == 3) {
            auto c = read_buf_[2];
            if (c == 65)
                key = ConsoleKey::Up;
            else if (c == 66)
                key = ConsoleKey::Down;
            else if (c == 67)
                key = ConsoleKey::Right;
            else if (c == 68)
                key = ConsoleKey::Left;
        }
        else if (n == 4) {
            if (read_buf_[3] == 126)
                key = ConsoleKey::Delete;
        }
        return key;
    }

    void move_left()
    {
        if (cursor_pos_ == 0)
            return;

        cursor_pos_--;
        std::lock_guard<std::mutex> guard(write_mutex_);
        std::printf("\x1b[1D");
        std::fflush(stdout);
    }

    void move_right()
    {
        if (cursor_pos_ >= current_buf_.size())
            return;

        cursor_pos_++;
        std::lock_guard<std::mutex> guard(write_mutex_);
        std::printf("\x1b[1C");
        std::fflush(stdout);
    }

    void clear_screen()
    {
        std::lock_guard<std::mutex> guard(write_mutex_);
        std::printf("\x1b[2J\x1b[H");
        std::fflush(stdout);
    }

    void flush_status_line()
    {
        std::lock_guard<std::mutex> guard(write_mutex_);
        std::lock_guard<std::mutex> guard_status(status_mutex_);
        std::lock_guard<std::mutex> guard_time(time_mutex_);

        std::printf("\r%s%s%s > %s\x1b[K",
                    time_.c_str(),
                    status_.c_str(),
                    name_.c_str(),
                    current_buf_.c_str());

        std::fflush(stdout);

        std::printf("\r\x1b[%uC", unsigned(time_.size() +
                                           status_.size() +
                                           name_.size() +
                                           3 +
                                           cursor_pos_));

        std::fflush(stdout);
    }

    bool push_command()
    {
        write_mutex_.lock();
        std::printf("\n");
        write_mutex_.unlock();

        if (current_buf_.empty()) {
            flush_status_line();
            return false;
        }

        if (history_.empty() || history_.back() != current_buf_)
            history_.push_back(std::move(current_buf_));
        else
            current_buf_.clear();

        if (history_.size() > HistoryCount)
            history_.pop_front();

        cursor_pos_ = 0;
        history_iter_ = history_.end();

        if (history_.back() == "quit") {
            flush_status_line();
            return true;
        }
        else if (history_.back() == "clear") {
            clear_screen();
            flush_status_line();
            return false;
        } else {
            commands_.push(history_.back());
        }

        flush_status_line();
        return false;
    }

    void prev_history()
    {
        while (history_iter_ != history_.begin() && current_buf_ == *--history_iter_);

        if (current_buf_ == *history_iter_)
            return;

        current_buf_ = *history_iter_;
        cursor_pos_ = current_buf_.size();

        flush_status_line();
    }

    void next_history()
    {
        while (history_iter_ != history_.end() && current_buf_ == *history_iter_)
            ++history_iter_;

        if (history_iter_ == history_.end())
            current_buf_.clear();
        else
            current_buf_ = *history_iter_;

        cursor_pos_ = current_buf_.size();

        flush_status_line();
    }

    void back_delete()
    {
        if (cursor_pos_ != 0)
            current_buf_.erase(--cursor_pos_, 1);

        flush_status_line();
    }

    void forward_delete()
    {
        if (cursor_pos_ != current_buf_.size())
            current_buf_.erase(cursor_pos_, 1);

        flush_status_line();
    }

    void input_char(char c)
    {
        current_buf_.insert(cursor_pos_++, &c, 1);

        flush_status_line();
    }

    void reset_time()
    {
        char buf[32] = {};
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::sprintf(buf,
                     "%04d-%02d-%02d %02d:%02d:%02d",
                     tm.tm_year + 1900,
                     tm.tm_mon + 1,
                     tm.tm_mday,
                     tm.tm_hour,
                     tm.tm_min,
                     tm.tm_sec);
        std::lock_guard<std::mutex> guard(time_mutex_);
        time_ = buf;
    }


private:
    termios term_tmp_;

    std::array<char, ReadBufferSize> read_buf_;

    std::list<std::string> history_;
    std::list<std::string>::iterator history_iter_;

    std::string name_;

    std::string status_;
    std::mutex status_mutex_;

    std::string time_;
    std::mutex time_mutex_;

    std::thread time_thread_;
    std::atomic_bool exit_time_thread_{false};

    std::string current_buf_;
    std::size_t cursor_pos_{0};

    ThreadSafeQueue<std::string> commands_;
    std::mutex write_mutex_;
};

} // !namespace rain

#if defined(NDEBUG)
#define RAIN_DEBUG(str) ((void)0)
#else
#define RAIN_DEBUG(str) \
    rain::Console::get_instance().write_line \
    (std::string("[ DEBUG ] ") + (str), rain::ConsoleColor::Cyan)
#endif

#define RAIN_INFO(str) \
    rain::Console::get_instance().write_line \
    (std::string("[ INFO  ] ") + (str), rain::ConsoleColor::Green)

#define RAIN_WARN(str) \
    rain::Console::get_instance().write_line \
    (std::string("[ WARN  ] ") + (str), rain::ConsoleColor::Brown)

#define RAIN_ERROR(str) \
    rain::Console::get_instance().write_line \
    (std::string("[ ERROR ] ") + (str), rain::ConsoleColor::Red)


#if defined(NDEBUG)
#define LUA_DEBUG(str) ((void)0)
#else
#define LUA_DEBUG(str) \
    rain::Console::get_instance().write_line \
    (std::string("[ LUA-DEBUG ] ") + (str), rain::ConsoleColor::Cyan)
#endif

#define LUA_INFO(str) \
    rain::Console::get_instance().write_line \
    (std::string("[ LUA-INFO  ] ") + (str), rain::ConsoleColor::Green)

#define LUA_WARN(str) \
    rain::Console::get_instance().write_line \
    (std::string("[ LUA-WARN  ] ") + (str), rain::ConsoleColor::Brown)

#define LUA_ERROR(str) \
    rain::Console::get_instance().write_line \
    (std::string("[ LUA-ERROR ] ") + (str), rain::ConsoleColor::Red)

