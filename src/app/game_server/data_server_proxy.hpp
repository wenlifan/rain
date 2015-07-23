#pragma once

#include <memory>

#include "singleton.hpp"
#include "session.hpp"
#include "message_pack.hpp"

namespace rain
{

class DataServerProxy : public Singleton<DataServerProxy>
{
    friend Singleton<DataServerProxy>;
    DataServerProxy() = default;

    using SessionPtr = std::shared_ptr<Session<DataServerProxy>>;
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