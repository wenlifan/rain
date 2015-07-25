#pragma once

#include <memory>
#include <cstring>
#include <cassert>
#include <vector>
#include <utility>
#include <type_traits>

#include "protocol.hpp"

namespace rain
{

class MessagePack
{
public:
    enum { HeaderSize = 6 };

public:
    MessagePack(Protocol proto, std::size_t init_size = 0)
            : data_(init_size + HeaderSize)
    {
        auto s = std::uint32_t(0);
        std::memcpy(data_.data(), &s, 4);
        auto p = std::uint16_t(proto);
        std::memcpy(data_.data() + 4, &p, 2);
    }

    MessagePack(MessagePack &&mp) = default;
    MessagePack & operator = (MessagePack &&) = default;

    std::size_t capacity() const
    {
        return data_.capacity();
    }

    std::size_t size() const
    {
        return data_.size();
    }

    std::size_t data_size() const
    {
        return std::size_t(*(std::uint32_t*)(data_.data()));
    }

    std::size_t avail() const
    {
        return data_.size() - readptr_;
    }

    Protocol protocol() const
    {
        return Protocol(*(std::uint16_t*)(data_.data() + 4));
    }

    char * data()
    {
        return data_.data();
    }

    template <typename T>
    void write(T &&t)
    {
        static_assert(std::is_pod<std::remove_reference_t<T>>::value,
                      "Only accept POD data!");

        write_size((char *)&t, sizeof(T));
    }

    void write_size(char const *data, std::size_t size)
    {
        data_.resize(data_.size() + size);
        std::memcpy(data_.data() + data_.size() - size, data, size);

        auto s = std::uint32_t(data_.size() - HeaderSize);
        std::memcpy(data_.data(), &s, 4);
    }

    template <typename T>
    T read()
    {
        static_assert(std::is_pod<std::remove_reference_t<T>>::value,
                      "Only accept POD data!");

        assert(avail() >= sizeof(T));

        T t;
        read_size((char *)&t, sizeof(T));
        return t;
    }

    void read_size(char *data, std::size_t size)
    {
        std::memcpy(data, data_.data() + readptr_, size);
        readptr_ += size;
    }

private:
    std::vector<char> data_;
    std::size_t readptr_ = 0;
};

using MessagePackPtr = std::shared_ptr<MessagePack>;

} // !namespace rain
