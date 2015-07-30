#pragma once

//0                   total 1 byte long(7bit 0~127) unsigned integer
//1 0                 total 2 bytes long(14bit -8192~8191) integer
//1 1 0               total 3 bytes long(21bit) integer
//1 1 1 0             total 4 bytes long(28bit) integer
//
//1 1 1 1 0 0 0 0     intgeter(other 4bytes)
//1 1 1 1 0 0 0 1     intgeter(other 5bytes)
//1 1 1 1 0 0 1 0     intgeter(other 6bytes)
//1 1 1 1 0 0 1 1     intgeter(other 7bytes)
//1 1 1 1 0 1 0 0     intgeter(other 8bytes)
//1 1 1 1 0 1 0 1     [Unused]
//1 1 1 1 0 1 1 0     [Unused]
//1 1 1 1 0 1 1 1     [Unused]
//1 1 1 1 1 0 0 0     [Unused]
//1 1 1 1 1 0 0 1     number(other 8bytes)
//1 1 1 1 1 0 1 0     string
//1 1 1 1 1 0 1 1     string index(other 4bytes)
//1 1 1 1 1 1 0 0     boolean false
//1 1 1 1 1 1 0 1     boolean true
//1 1 1 1 1 1 1 0     table start
//1 1 1 1 1 1 1 1     table end

#include <algorithm>
#include <unordered_map>
#include <string>
#include <functional>

#include "lua.hpp"

namespace rain
{

class LuaBinary
{
    LuaBinary() = delete;

    enum : std::uint8_t {
        TWO_BYTES_INT       = 0b1000'0000,
        THREE_BYTES_INT     = 0b1100'0000,
        FOUR_BYTES_INT      = 0b1110'0000,

        FIVE_BYTES_INT      = 0b1111'0000,
        SIX_BYTES_INT       = 0b1111'0001,
        SEVEN_BYTES_INT     = 0b1111'0010,
        EIGHT_BYTES_INT     = 0b1111'0011,
        NINE_BYTES_INT      = 0b1111'0100,

        NUMBER_TYPE         = 0b1111'1001,
        STRING_TYPE         = 0b1111'1010,
        STRING_IDX_TYPE     = 0b1111'1011,
        BOOLEAN_FALSE_TYPE  = 0b1111'1100,
        BOOLEAN_TRUE_TYPE   = 0b1111'1101,
        TABLE_START_TYPE    = 0b1111'1110,
        TABLE_END_TYPE      = 0b1111'1111,
    };

public:
    static bool lua_to_binary(
        lua_State *state,
        char *dest,
        std::size_t &size,      // get dest already alloc size, return final dest length
        std::function<void(std::size_t)> const &alloc
    ) {
        if (!lua_istable(state, -1))
            return false;

        L = state;

        write_iter_ = 0;
        total_size_ = size;
        dest_ = dest;
        alloc_ = alloc;

        auto ret = read_lua_table(-1);

        str_pool_.clear();
        return ret;
    }

    static bool binary_to_lua(
        lua_State *L,
        char const *src,
        std::size_t size
    ) {

        return true;
    }

private:
    static bool read_lua_table(int idx)
    {
        prepare(1);
        *(uint8_t*)(dest_ + write_iter_) = 0b1111'1110;
        write_iter_++;

        lua_pushnil(L);
        while (lua_next(L, idx-1)) {
            if (lua_isinteger(L, -2)) {
                read_lua_integer(-2);
            } else if (lua_isstring(L, -2)) {
                read_lua_string(-2);
            } else {
                return false;
            }

            switch (lua_type(L, -1)) {
                case LUA_TTABLE:
                    if (!read_lua_table(-1)) {
                        return false;
                    }
                    break;
                case LUA_TNUMBER:
                    if (lua_isinteger(L, -1)) {
                        read_lua_integer(-1);
                    } else {
                        read_lua_number(-1);
                    }
                    break;
                case LUA_TBOOLEAN:
                    read_lua_boolean(-1);
                    break;
                case LUA_TSTRING:
                    read_lua_string(-1);
                    break;
                default:
                    return false;
            }

            lua_pop(L, 1);
        }

        prepare(1);
        *(uint8_t*)(dest_ + write_iter_) = 0b1111'1111;
        write_iter_++;

        return true;
    }

    static void read_lua_integer(int idx)
    {
        std::int64_t val = lua_tointeger(L, idx);

        if (val >= 0 && val < 0x80) {
            prepare(1);
            *(std::uint8_t *)(dest_ + write_iter_) = (std::uint8_t)val;
            write_iter_++;
        } else if (val < (1ll << 14) && val >= -(1ll << 14)) {
            prepare(2);
            *(std::uint8_t*)(dest_ + write_iter_) = (std::uint8_t)(((val >> 8) & 0b0011'1111) | TWO_BYTES_INT);
            std::memcpy(dest_ + write_iter_ + 1, &val, 1);
            write_iter_ += 2;
        } else if (val < (1ll << 21) && val >= -(1ll << 21)) {
            prepare(3);
            *(std::uint8_t*)(dest_ + write_iter_) = (std::uint8_t)(((val >> 16) & 0b0001'1111) | THREE_BYTES_INT);
            std::memcpy(dest_ + write_iter_ + 1, &val, 2);
            write_iter_ += 3;
        } else if (val < (1ll << 28) && val >= -(1ll << 28)) {
            prepare(4);
            *(std::uint8_t*)(dest_ + write_iter_) = (std::uint8_t)(((val >> 24) & 0b0000'1111) | FOUR_BYTES_INT);
            std::memcpy(dest_ + write_iter_ + 1, &val, 3);
            write_iter_ += 4;
        } else if (val < (1ll << 32) && val >= -(1ll << 32)) {
            prepare(5);
            *(std::uint8_t*)(dest_ + write_iter_) = FIVE_BYTES_INT;
            std::memcpy(dest_ + write_iter_ + 1, &val, 4);
            write_iter_ += 5;
        } else if (val < (1ll << 40) && val >= -(1ll << 40)) {
            prepare(6);
            *(std::uint8_t*)(dest_ + write_iter_) = SIX_BYTES_INT;
            std::memcpy(dest_ + write_iter_ + 1, &val, 5);
            write_iter_ += 6;
        } else if (val < (1ll << 48) && val >= -(1ll << 48)) {
            prepare(7);
            *(std::uint8_t*)(dest_ + write_iter_) = SEVEN_BYTES_INT;
            std::memcpy(dest_ + write_iter_ + 1, &val, 6);
            write_iter_ += 7;
        } else if (val < (1ll << 56) && val >= -(1ll << 56)) {
            prepare(8);
            *(std::uint8_t*)(dest_ + write_iter_) = EIGHT_BYTES_INT;
            std::memcpy(dest_ + write_iter_ + 1, &val, 7);
            write_iter_ += 8;
        } else {
            prepare(9);
            *(std::uint8_t*)(dest_ + write_iter_) = NINE_BYTES_INT;
            std::memcpy(dest_ + write_iter_ + 1, &val, 8);
            write_iter_ += 9;
        }
    }

    static void read_lua_number(int idx)
    {
        double d = lua_tonumber(L, idx);
        prepare(9);
        *(std::uint8_t*)(dest_ + write_iter_) = NUMBER_TYPE;
        std::memcpy(dest_ + write_iter_ + 1, &d, 8);
        write_iter_ += 9;
    }

    static void read_lua_string(int idx)
    {
        std::string str = lua_tostring(L, idx);
        auto iter = str_pool_.find(str);
        if (iter == str_pool_.end()) {
            auto size = str.size();
            prepare(size + 2);
            *(std::uint8_t*)(dest_ + write_iter_++) = STRING_TYPE;
            std::memcpy(dest_ + write_iter_, str.data(), size);
            str_pool_.insert(std::make_pair(std::move(str), write_iter_));
            write_iter_ += size;
            *(dest_ + write_iter_++) = '\0';
        } else {
            prepare(5);
            *(std::uint8_t*)(dest_ + write_iter_++) = STRING_IDX_TYPE;
            std::uint32_t str_idx = iter->second;
            std::memcpy(dest_ + write_iter_, &str_idx, 4);
            write_iter_ += 4;
        }
    }

    static void read_lua_boolean(int idx)
    {
        auto b = lua_toboolean(L, idx);
        prepare(1);
        if (b) {
            *(std::uint8_t*)(dest_ + write_iter_) = BOOLEAN_TRUE_TYPE;
        } else {
            *(std::uint8_t*)(dest_ + write_iter_) = BOOLEAN_FALSE_TYPE;
        }
        write_iter_++;
    }

    static void prepare(std::size_t size)
    {
        if (total_size_ - write_iter_ < size) {
            alloc_(std::max<std::size_t>(size, 128));
            total_size_ += size;
        }
    }

private:
    static lua_State *L;
    static std::size_t total_size_;
    static std::size_t write_iter_;
    static char *dest_;
    static std::function<void(std::size_t)> alloc_;
    static std::unordered_map<std::string, std::size_t> str_pool_;
};

} // !namespace rain