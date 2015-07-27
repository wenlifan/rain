#pragma once

#include <cstdint>

namespace rain
{

enum Protocol : std::uint16_t
{
    // common protocol
    COMM_BEGIN = 0,
    COMM_NONE,
    COMM_PING,
    COMM_PONG,
    COMM_FEEDBACK_WRAPPER,  // pack: header + feedback_id(4 bytes) + other_protocal
    COMM_FEEDBACK,
    COMM_END,

    // client to login_server
    C_LS_BEGIN = 1000,
    C_LS_END,

    // login_server to client
    LS_C_BEGIN = 2000,
    LS_C_END,

    // login_server to account_server
    LS_AS_BEGIN = 3000,
    LS_AS_END,

    // account_server to login_server
    AS_LS_BEGIN = 4000,
    AS_LS_END,

    // login_server to gateway_server
    LS_GW_BEGIN = 5000,
    LS_GW_END,

    // gateway_server to login_server
    GW_LS_BEGIN = 6000,
    GW_LS_END,

    // client to gateway_server
    C_GW_BEGIN = 7000,
    C_GW_END,

    // gateway_server to client
    GW_C_BEGIN = 8000,
    GW_C_END,

    // gateway_server to game_server
    GW_GS_BEGIN = 9000,
    GW_GS_END,

    // game_server to gateway_server
    GS_GW_BEGIN = 10000,
    GS_GW_END,

    // game_server to data_server
    GS_DS_BEGIN = 11000,
    GS_DS_END,

    // data_server to game_server
    DS_GS_BEGIN = 12000,
    DS_GS_END,

    // game_server to shared_server
    GS_SS_BEGIN = 13000,
    GS_SS_END,

    // shared_server to game_server
    SS_GS_BEGIN = 14000,
    SS_GS_END,

    // shared_server to data_server
    SS_DS_BEGIN = 15000,
    SS_DS_END,

    // data_server to shared_server
    DS_SS_BEGIN = 16000,
    DS_SS_END,
};

} // !namespace rain