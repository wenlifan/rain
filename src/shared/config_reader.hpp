#pragma once

#include <string>

#include "lua.hpp"

#include "singleton.hpp"
#include "console.hpp"

namespace rain
{

class ConfigReader
    : public Singleton<ConfigReader>
{
    friend Singleton<ConfigReader>;
    ConfigReader()
        : state_(luaL_newstate())
    {
    }

public:
    ~ConfigReader()
    {
        lua_close(state_);
    }

    bool load_config(std::string const &fname)
    {
        if (luaL_dofile(state_, fname.c_str())) {
            RAIN_ERROR("Read file <" + fname + "> occurs error: " + lua_tostring(state_, -1));
            lua_pop(state_, 1);
            return false;
        }
        return true;
    }

    bool read_int(int &i, std::string const &str)
    {
        if (luaL_dostring(state_, ("_read_value_ = " + str).c_str())) {
            RAIN_ERROR("Read int <" + str + "> occurs error: " + lua_tostring(state_, -1));
            lua_pop(state_, 1);
            return false;
        }
        int cvt = 0;
        lua_getglobal(state_, "_read_value_");
        i = (int)lua_tointegerx(state_, -1, &cvt);
        if (!cvt) {
            RAIN_ERROR("Field <" + str + "> is not an int.");
            lua_pop(state_, 1);
            return false;
        }

        lua_pop(state_, 1);
        return true;
    }

    bool read_double(double &d, std::string const &str)
    {
        if (luaL_dostring(state_, ("_read_value_ = " + str).c_str())) {
            RAIN_ERROR("Read double <" + str + "> occurs error: " + lua_tostring(state_, -1));
            lua_pop(state_, 1);
            return false;
        }
        int cvt = 0;
        lua_getglobal(state_, "_read_value_");
        d = lua_tonumberx(state_, -1, &cvt);
        if (!cvt) {
            RAIN_ERROR("Field <" + str + "> is not a double.");
            lua_pop(state_, 1);
            return false;
        }

        lua_pop(state_, 1);
        return true;
    }

    bool read_string(std::string &str, std::string const &field_name)
    {
        if (luaL_dostring(state_, ("_read_value_ = " + field_name).c_str())) {
            RAIN_ERROR("Read string <" + field_name + "> occurs error: " + lua_tostring(state_, -1));
            lua_pop(state_, 1);
            return false;
        }
        lua_getglobal(state_, "_read_value_");
        auto s = lua_tostring(state_, -1);
        if (!s) {
            RAIN_ERROR("Field <" + field_name + "> is not a string.");
            lua_pop(state_, 1);
            return false;
        }
        str = s;

        lua_pop(state_, 1);
        return true;
    }

private:
    lua_State *state_;
};

} // !namespace rain