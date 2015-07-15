#pragma once

#include <memory>

#include "session.hpp"
#include "message_pack.hpp"
#include "singleton.hpp"

namespace rain
{

class ServerSessionManager : public Singleton<ServerSessionManager>
{
    friend Singleton<ServerSessionManager>;
    ServerSessionManager() = default;

    using SessionPtr = std::shared_ptr<Session<ServerSessionManager>>;
    using MessagePackPtr = std::shared_ptr<MessagePack>;

public:
    static ServerSessionManager &get_instance()
    {
        static ServerSessionManager mgr;
        return mgr;
    }

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