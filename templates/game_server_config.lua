
Server = {
    Gateway = {
        Port = 11005,
        PingInterval = 2000, -- ms
        BreakTimes = 3,
    },
}

Client = {
    SharedServer = {
        IP = "127.0.0.1",
        Port = 11006,
        PingInterval = 2000, -- ms
        BreakTimes = 3,
    },

    DataServer = {
        IP = "127.0.0.1",
        Port = 11007,
        PingInterval = 2000, -- ms
        BreakTimes = 3,
    },
}

