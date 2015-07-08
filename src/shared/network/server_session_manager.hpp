#pragma once

#include <unordered_set>
#include <unordered_map>
#include <memory>

#include "session.hpp"
#include "message_pack.hpp"

namespace rain
{

class ServerSessionManager
{
    using SessionPtr = std::shared_ptr<Session<ServerSessionManager>>;
    using MessagePackPtr = std::shared_ptr<MessagePack>;

public:
    static ServerSessionManager & getInstance()
    {
        static ServerSessionManager mgr;
        return mgr;
    }

    void addSession(SessionPtr session)
    {

    }

    void removeSession(SessionPtr session)
    {

    }

    void dispatchMessage(SessionPtr session, MessagePackPtr msgp)
    {

    }

private:
    ServerSessionManager() = default;

private:
};

} // !namespace rain