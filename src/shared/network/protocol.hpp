#pragma once

#include <cstdint>

namespace rain
{

enum Protocol : std::uint16_t
{
    // Common protocol
    COMM_NONE = 0,
    COMM_PING,
    COMM_PONG,

    // Client to LoginServer
    C_LS_BEGIN = 1000,
    C_LS_END,

    // LoginServer to Client
    LS_C_BEGIN = 2000,
    LS_C_END,

    // LoginServer to AccountServer
    LS_AS_BEGIN = 3000,
    LS_AS_END,

    // AccountServer to LoginServer
    AS_LS_BEGIN = 4000,
    AS_LS_END,

    // LoginServer to GateWay
    LS_GW_BEGIN = 5000,
    LS_GW_END,

    // GateWay to LoginServer
    GW_LS_BEGIN = 6000,
    GW_LS_END,

    // Client to GateWay
    C_GW_BEGIN = 7000,
    C_GW_END,

    // GateWay to Client
    GW_C_BEGIN = 8000,
    GW_C_END,

    // GateWay to GameServer
    GW_GS_BEGIN = 9000,
    GW_GS_END,

    // GateServer to GateWay
    GS_GW_BEGIN = 10000,
    GS_GW_END,

    // GameServer to DataServer
    GS_DS_BEGIN = 11000,
    GS_DS_END,

    // DataServer to GameServer
    DS_GS_BEGIN = 12000,
    DS_GS_END,

    // GameServer to SharedServer
    GS_SS_BEGIN = 13000,
    GS_SS_END,

    // SharedServer to GameServer
    SS_GS_BEGIN = 14000,
    SS_GS_END,
};

} // !namespace rain