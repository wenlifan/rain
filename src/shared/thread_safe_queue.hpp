#pragma once

#include <list>
#include <memory>
#include <mutex>
#include <vector>

#include "message_pack.hpp"

namespace rain
{

template <typename T>
class ThreadSafeQueue
{
public:
    void push(T t)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        list_.push_back(std::move(t));
    }

    void erase(T t)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        list_.remove(t);
    }

    T pop()
    {
        std::lock_guard<std::mutex> guard(mutex_);
        auto front = list_.front();
        list_.pop_front();
        return front;
    }

    std::list<T> pop_all()
    {
        std::lock_guard<std::mutex> guard(mutex_);
        if (list_.empty())
            return std::list<T>();
        else
            return std::move(list_);
    }

    bool empty()
    {
        std::lock_guard<std::mutex> guard(mutex_);
        return list_.empty();
    }

private:
    std::list<T> list_;
    std::mutex mutex_;
};

} // !namespace rain