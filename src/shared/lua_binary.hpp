#pragma once

//0                   total 1 byte long(7bits 0~127) unsigned integer
//1 0                 total 2 bytes long(14bits -8192~8191) integer
//1 1 0               total 3 bytes long(21bits) integer
//1 1 1 0             total 4 bytes long(28bits) integer
//
//1 1 1 1 0 0 0 0     intgeter(other 4bytes)
//1 1 1 1 0 0 0 1     intgeter(other 5bytes)
//1 1 1 1 0 0 1 0     intgeter(other 6bytes)
//1 1 1 1 0 0 1 1     intgeter(other 7bytes)
//1 1 1 1 0 1 0 0     intgeter(other 8bytes)
//1 1 1 1 0 1 0 1     [Unused]
//1 1 1 1 0 1 1 0     number(other 8bytes)
//1 1 1 1 0 1 1 1     string
//1 1 1 1 1 0 0 0     string index(other 1byte)
//1 1 1 1 1 0 0 1     string index(other 2bytes)
//1 1 1 1 1 0 1 0     string index(other 3bytes)
//1 1 1 1 1 0 1 1     string index(other 4bytes)
//1 1 1 1 1 1 0 0     boolean false
//1 1 1 1 1 1 0 1     boolean true
//1 1 1 1 1 1 1 0     table begin
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
    enum : std::uint8_t {
        TWO_BYTES_INT           = 0b1000'0000,
        THREE_BYTES_INT         = 0b1100'0000,
        FOUR_BYTES_INT          = 0b1110'0000,

        FIVE_BYTES_INT          = 0b1111'0000,
        SIX_BYTES_INT           = 0b1111'0001,
        SEVEN_BYTES_INT         = 0b1111'0010,
        EIGHT_BYTES_INT         = 0b1111'0011,
        NINE_BYTES_INT          = 0b1111'0100,

        NUMBER_TYPE             = 0b1111'0110,

        STRING_TYPE             = 0b1111'0111,
        STRING_IDX_TWO_BYTES    = 0b1111'1000,
        STRING_IDX_THREE_BYTES  = 0b1111'1001,
        STRING_IDX_FOUR_BYTES   = 0b1111'1010,
        STRING_IDX_FIVE_BYTES   = 0b1111'1011,

        BOOLEAN_FALSE_TYPE      = 0b1111'1100,
        BOOLEAN_TRUE_TYPE       = 0b1111'1101,
        TABLE_BEGIN_TYPE        = 0b1111'1110,
        TABLE_END_TYPE          = 0b1111'1111,
    };

public:
    bool lua_to_binary(
        lua_State *state,
        char *dest,
        std::size_t &size,      // get dest already alloc size, return final dest length
        std::function<void(std::size_t)> const &alloc
    ) {
        if (!lua_istable(state, -1)) {
            return false;
        }

        L = state;

        total_size_ = size;
        rw_iter_ = 0;
        data_ = dest;
        success_ = true;
        alloc_ = alloc;

        read_lua_table(-1);

        size = rw_iter_;
        str_pool_.clear();
        return success_;
    }

    bool binary_to_lua(
        lua_State *state,
        char const *src,
        std::size_t size
    ) {
        if (*(std::uint8_t*)src != TABLE_BEGIN_TYPE) {
            return false;
        }

        L = state;

        total_size_ = size;
        rw_iter_ = 0;
        data_ = (char*)src;
        success_ = true;

        write_lua_table();

        return success_;
    }

private:
    void read_lua_table(int idx)
    {
        prepare(1);
        *(uint8_t*)(data_ + rw_iter_) = TABLE_BEGIN_TYPE;
        rw_iter_++;

        lua_pushnil(L);
        while (lua_next(L, idx-1)) {
            if (lua_isinteger(L, -2)) {
                read_lua_integer(-2);
            } else if (lua_isstring(L, -2)) {
                read_lua_string(-2);
            } else {
                success_ = false;
            }

            switch (lua_type(L, -1)) {
                case LUA_TTABLE:
                    read_lua_table(-1);
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
                    success_ = false;
                    break;
            }

            if (!success_) {
                return;
            }

            lua_pop(L, 1);
        }

        prepare(1);
        *(uint8_t*)(data_ + rw_iter_) = TABLE_END_TYPE;
        rw_iter_++;
    }

    void read_lua_integer(int idx)
    {
        std::int64_t val = lua_tointeger(L, idx);

        if (val >= 0 && val < 0x80) {
            prepare(1);
            *(std::uint8_t *)(data_ + rw_iter_) = (std::uint8_t)val;
            rw_iter_++;
        } else if (val < (1ll << 14) && val >= -(1ll << 14)) {
            prepare(2);
            *(std::uint8_t*)(data_ + rw_iter_) = (std::uint8_t)(((val >> 8) & 0b0011'1111) | TWO_BYTES_INT);
            std::memcpy(data_ + rw_iter_ + 1, &val, 1);
            rw_iter_ += 2;
        } else if (val < (1ll << 21) && val >= -(1ll << 21)) {
            prepare(3);
            *(std::uint8_t*)(data_ + rw_iter_) = (std::uint8_t)(((val >> 16) & 0b0001'1111) | THREE_BYTES_INT);
            std::memcpy(data_ + rw_iter_ + 1, &val, 2);
            rw_iter_ += 3;
        } else if (val < (1ll << 28) && val >= -(1ll << 28)) {
            prepare(4);
            *(std::uint8_t*)(data_ + rw_iter_) = (std::uint8_t)(((val >> 24) & 0b0000'1111) | FOUR_BYTES_INT);
            std::memcpy(data_ + rw_iter_ + 1, &val, 3);
            rw_iter_ += 4;
        } else if (val < (1ll << 32) && val >= -(1ll << 32)) {
            prepare(5);
            *(std::uint8_t*)(data_ + rw_iter_) = FIVE_BYTES_INT;
            std::memcpy(data_ + rw_iter_ + 1, &val, 4);
            rw_iter_ += 5;
        } else if (val < (1ll << 40) && val >= -(1ll << 40)) {
            prepare(6);
            *(std::uint8_t*)(data_ + rw_iter_) = SIX_BYTES_INT;
            std::memcpy(data_ + rw_iter_ + 1, &val, 5);
            rw_iter_ += 6;
        } else if (val < (1ll << 48) && val >= -(1ll << 48)) {
            prepare(7);
            *(std::uint8_t*)(data_ + rw_iter_) = SEVEN_BYTES_INT;
            std::memcpy(data_ + rw_iter_ + 1, &val, 6);
            rw_iter_ += 7;
        } else if (val < (1ll << 56) && val >= -(1ll << 56)) {
            prepare(8);
            *(std::uint8_t*)(data_ + rw_iter_) = EIGHT_BYTES_INT;
            std::memcpy(data_ + rw_iter_ + 1, &val, 7);
            rw_iter_ += 8;
        } else {
            prepare(9);
            *(std::uint8_t*)(data_ + rw_iter_) = NINE_BYTES_INT;
            std::memcpy(data_ + rw_iter_ + 1, &val, 8);
            rw_iter_ += 9;
        }
    }

    void read_lua_number(int idx)
    {
        double d = lua_tonumber(L, idx);
        prepare(9);
        *(std::uint8_t*)(data_ + rw_iter_) = NUMBER_TYPE;
        std::memcpy(data_ + rw_iter_ + 1, &d, 8);
        rw_iter_ += 9;
    }

    void read_lua_string(int idx)
    {
        std::string str = lua_tostring(L, idx);
        auto iter = str_pool_.find(str);
        if (iter == str_pool_.end()) {
            auto size = str.size();
            prepare(size + 2);
            *(std::uint8_t*)(data_ + rw_iter_++) = STRING_TYPE;
            std::memcpy(data_ + rw_iter_, str.data(), size);
            str_pool_.insert(std::make_pair(std::move(str), rw_iter_));
            rw_iter_ += size;
            *(data_ + rw_iter_++) = '\0';
        } else {
            std::uint32_t str_idx = iter->second;
            if (str_idx < (1u << 8)) {
                prepare(2);
                *(std::uint8_t*)(data_ + rw_iter_++) = STRING_IDX_TWO_BYTES;
                std::memcpy(data_ + rw_iter_, &str_idx, 1);
                rw_iter_ ++;
            } else if (str_idx < (1u << 16)) {
                prepare(3);
                *(std::uint8_t*)(data_ + rw_iter_++) = STRING_IDX_THREE_BYTES;
                std::memcpy(data_ + rw_iter_, &str_idx, 2);
                rw_iter_ += 2;
            } else if (str_idx < (1u << 24)) {
                prepare(4);
                *(std::uint8_t*)(data_ + rw_iter_++) = STRING_IDX_FOUR_BYTES;
                std::memcpy(data_ + rw_iter_, &str_idx, 3);
                rw_iter_ += 3;
            } else {
                prepare(5);
                *(std::uint8_t*)(data_ + rw_iter_++) = STRING_IDX_FIVE_BYTES;
                std::memcpy(data_ + rw_iter_, &str_idx, 4);
                rw_iter_ += 4;
            }
        }
    }

    void read_lua_boolean(int idx) {
        auto b = lua_toboolean(L, idx);
        prepare(1);
        if (b) {
            *(std::uint8_t *) (data_ + rw_iter_) = BOOLEAN_TRUE_TYPE;
        } else {
            *(std::uint8_t *) (data_ + rw_iter_) = BOOLEAN_FALSE_TYPE;
        }
        rw_iter_++;
    }

#define ERROR_RETURN()                      \
    do {                                    \
        success_ = false;                   \
        return;                             \
    } while(false)

#define CHECK_SIZE(sz)                      \
    do {                                    \
        if (rw_iter_ + sz > total_size_) {  \
            ERROR_RETURN();                 \
        }                                   \
    } while(false)

    void write_lua_table()
    {
        rw_iter_++;
        lua_newtable(L);

        for (;;) {
            CHECK_SIZE(1);
            auto v = *(std::uint8_t *)(data_ + rw_iter_);
            if (v == TABLE_END_TYPE) {
                rw_iter_++;
                break;
            } else if (is_integer_type(v)) {
                write_lua_integer();
            } else if (is_string_type(v)) {
                write_lua_string();
            } else {
                ERROR_RETURN();
            }

            if (!success_)
                return;

            CHECK_SIZE(1);
            v = *(std::uint8_t *)(data_ + rw_iter_);
            if (is_integer_type(v)) {
                write_lua_integer();
            } else if (is_string_type(v)) {
                write_lua_string();
            } else if (v == NUMBER_TYPE) {
                write_lua_number();
            } else if (v == BOOLEAN_FALSE_TYPE || v == BOOLEAN_TRUE_TYPE) {
                write_lua_boolean();
            } else if (v == TABLE_BEGIN_TYPE) {
                write_lua_table();
            }

            if (!success_)
                return;

            lua_settable(L, -3);
        }
    }

    void write_lua_integer()
    {

    }

    void write_lua_number()
    {

    }

    void write_lua_string()
    {

    }

    void write_lua_boolean()
    {

    }
#undef ERROR_RETURN
#undef CHECK_SIZE

    bool is_integer_type(std::uint8_t v)
    {
        return v <= NINE_BYTES_INT;
    }

    bool is_string_type(std::uint8_t v)
    {
        return v >= STRING_TYPE && v <= STRING_IDX_FIVE_BYTES;
    }

    void prepare(std::size_t size)
    {
        if (total_size_ - rw_iter_ < size) {
            alloc_(std::max<std::size_t>(size, 4096));
            total_size_ += size;
        }
    }

private:
    lua_State *L;
    std::size_t total_size_;
    std::size_t rw_iter_;
    char *data_;
    bool success_;
    std::function<void(std::size_t)> alloc_;
    std::unordered_map<std::string, std::size_t> str_pool_;
};

} // !namespace rain