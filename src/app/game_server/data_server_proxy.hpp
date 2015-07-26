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

    void session_read_eof(TargetSessionPtr session)
    {

    }

    void send_message_finished(TargetSessionPtr, MessagePackPtr msgp)
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

    void stop()
    {
        stop_reconn_thread();
    }

private:
};

} // !namespace rain