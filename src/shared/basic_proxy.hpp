#pragma once

#include "config_reader.hpp"

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
    bool init_params(std::string const &ping_interval_str, std::string const &break_times_str)
    {
        auto &reader = ConfigReader::get_instance();

        int pi, bt;
        if (!reader.read_int(pi, ping_interval_str)) {
            RAIN_ERROR("Read PingInterval failed");
            return false;
        }
        if (!reader.read_int(bt, break_times_str)) {
            RAIN_ERROR("Read PingInterval failed");
            return false;
        }

        ping_interval_ = static_cast<std::size_t>(pi);
        break_times_ = static_cast<std::size_t>(bt);

        return true;
    }

private:
    std::size_t ping_interval_;
    std::size_t break_times_;
};

} // !namespace rain