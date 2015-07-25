#pragma once

#include "singleton.hpp"
#include "message_pack.hpp"
#include "client_proxy.hpp"

namespace rain
{

class DataServerProxy
    : public ClientProxy<DataServerProxy>
    , public Singleton<DataServerProxy>
{
    friend Singleton<DataServerProxy>;
    DataServerProxy() = default;

public:
    void add_session(TargetSessionPtr session)
    {

    }

    void remove_session(TargetSessionPtr session)
    {

    }

    void dispatch_message(TargetSessionPtr session, MessagePackPtr msgp)
    {

    }

public:
    bool init()
    {
        return ClientProxy::init(
            "Client.DataServer.PingInterval",
            "Client.DataServer.BreakTimes",
            "Client.DataServer.IP",
            "Client.DataServer.Port"
        );
    }
};

} // !namespace rain