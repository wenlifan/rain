#pragma once

#include <memory>

#include "singleton.hpp"
#include "session.hpp"
#include "message_pack.hpp"

namespace rain
{

class ServerSessionManager : public Singleton<ServerSessionManager>
{
    friend Singleton<ServerSessionManager>;
    ServerSessionManager() = default;

    using SessionPtr = std::shared_ptr<Session<ServerSessionManager>>;
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

public:
    bool init()
    {
        return true;
    }

private:
};

} // !namespace rain