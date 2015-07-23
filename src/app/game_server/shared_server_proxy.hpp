#pragma once

#include <memory>

#include "singleton.hpp"
#include "session.hpp"
#include "message_pack.hpp"

namespace rain
{

class SharedServerProxy : public Singleton<SharedServerProxy>
{
    friend Singleton<SharedServerProxy>;
    SharedServerProxy() = default;

    using SessionPtr = std::shared_ptr<Session<SharedServerProxy>>;
    using MessagePackPtr = std::shared_ptr<MessagePack>;

public:
    void add_session(SessionPtr session)
    {

    }

    void remove_session(SessionPtr session)
    {

    }

    void dispatch_message(SessionPtr session, MessagePackPtr msgp)
    {

    }

private:
};

} // !namespace rain