#pragma once

#include "singleton.hpp"
#include "local_client_proxy.hpp"
#include "logic_system.hpp"

namespace rain
{

class DataServerProxy
    : public LocalClientProxy<DataServerProxy, LogicSystem>
        , public Singleton<DataServerProxy>
{
    friend Singleton<DataServerProxy>;
    DataServerProxy() = default;

public:
    bool init()
    {
        return LocalClientProxy::init("DataServer");
    }
};

} // !namespace rain
