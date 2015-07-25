#pragma once

namespace rain
{

class BasicProxy
{
protected:
    BasicProxy() = default;

public:
    std::size_t get_ping_interval() const
    {
        return ping_interval_;
    }

    std::size_t get_break_times() const
    {
        return break_times_;
    }

protected:
    std::size_t ping_interval_;
    std::size_t break_times_;
};

} // !namespace rain