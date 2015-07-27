#pragma once

#include "singleton.hpp"
#include "local_client_proxy.hpp"

namespace rain
{

class DataServerProxy
    : public LocalClientProxy<DataServerProxy>
        , public Singleton<DataServerProxy>
{
    friend Singleton<DataServerProxy>;
    DataServerProxy() = default;

public:
    void dispatch_message(TargetSessionPtr session, MessagePackPtr msgp)
    {

    }

    bool init()
    {
        return LocalClientProxy::init("DataServer");
    }
};

} // !namespace rain
