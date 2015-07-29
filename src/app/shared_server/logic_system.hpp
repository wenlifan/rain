#pragma once

#include "singleton.hpp"
#include "message_pack.hpp"
#include "thread_safe_queue.hpp"
#include "console.hpp"

namespace rain
{

class LogicSystem
    : public Singleton<LogicSystem>
{
    friend Singleton<LogicSystem>;
    LogicSystem() = default;

public:
    void dispatch(MessagePackPtr msgp)
    {
        msg_queue_.push(msgp);
    }

    bool init()
    {
        return true;
    }

    void stop()
    {

    }

private:
    ThreadSafeQueue<MessagePackPtr> msg_queue_;
};

} // !namespace rain