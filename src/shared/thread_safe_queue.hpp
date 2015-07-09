#pragma once

#include <list>
#include <memory>
#include <mutex>
#include <vector>

#include "singleton.hpp"
#include "message_pack.hpp"

namespace rain
{

template <typename T>
class ThreadSafeQueue
{
public:
    void push(T ptr)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        lists_.push_back(std::move(ptr));
    }

    T pop()
    {
        std::lock_guard<std::mutex> guard(mutex_);
        auto ptr = lists_.front();
        lists_.pop_front();
        return ptr;
    }

    std::list<T> popAll()
    {
        std::lock_guard<std::mutex> guard(mutex_);
        if (lists_.empty())
            return std::list<T>();
        else
            return std::move(lists_);
    }

private:
    std::list<T> lists_;
    std::mutex mutex_;
};

} // !namespace rain