#pragma once

#include "singleton.hpp"
#include "message_pack.hpp"
#include "client_proxy.hpp"

namespace rain
{

class DataServerProxy
    : public Singleton<DataServerProxy>
{
    friend Singleton<DataServerProxy>;
    DataServerProxy() = default;

    using TargetSession = Session<DataServerProxy>;
    using TargetSessionPtr = std::shared_ptr<TargetSession>;
    using Proxy = ClientProxy<DataServerProxy>;
    using ProxyPtr = std::shared_ptr<Proxy>;

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

    std::size_t get_ping_interval() const
    {
        return proxy_->get_ping_interval();
    }

    std::size_t get_break_times() const
    {
        return proxy_->get_break_times();
    }

public:
    bool init()
    {
        proxy_ = std::make_unique<Proxy>();
        return proxy_->init(
            "Client.DataServer.PingInterval",
            "Client.DataServer.BreakTimes",
            "Client.DataServer.IP",
            "Client.DataServer.Port"
        );
    }

    void stop()
    {
        proxy_.reset();
    }

private:
    ProxyPtr proxy_;
};

} // !namespace rain