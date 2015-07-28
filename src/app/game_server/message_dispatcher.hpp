#pragma once

#include "singleton.hpp"
#include "message_pack.hpp"

namespace rain
{

class MessageDispatcher
    : public Singleton<MessageDispatcher>
{
    friend Singleton<MessageDispatcher>;
    MessageDispatcher() = default;

public:
    void dispatch(MessagePackPtr msgp)
    {
        auto proto = msgp->protocol();
        switch (proto) {

            default:
                break;
        }
    }

};

} // !namespace rain