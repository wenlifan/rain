
Server = {
    GatewayServer = {
        AcceptIP = "127.0.0.1",
        Port = 11005,
        PingInterval = 2000, -- ms
        BreakTimes = 3,
    },
}

Client = {
    SharedServer = {
        IP = "127.0.0.1",
        Port = 11004,
        PingInterval = 2000, -- ms
        BreakTimes = 3,
    },

    DataServer = {
        IP = "127.0.0.1",
        Port = 11002,
        PingInterval = 2000, -- ms
        BreakTimes = 3,
    },
}

