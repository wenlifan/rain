#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <type_traits>

#include <iostream>

namespace rain
{

class ByteBuffer
{
public:
    ByteBuffer(std::size_t init_size = 128)
    {
        size_ = init_size;
        data_ = std::malloc(size_);
    }

    ~ByteBuffer()
    {
        std::free(data_);
    }

    std::size_t size() const
    {
        return size_;
    }

    std::size_t avail() const
    {
        return writeptr_ - readptr_;
    }

    template <typename T>
    void write(T &&t)
    {
        static_assert(std::is_pod<std::remove_reference_t<T>>::value,
                      "Only accept POD data!");

        write_size(&t, sizeof(T));
    }

    void write_size(void const *data, std::size_t size)
    {
        while (size_-writeptr_ < size)
        {
            if (size_ < 256)
                size_ = nlpo2(size_+1);
            else
                size_ += 256;
        }

        data_ = std::realloc(data_, size_);
        std::memcpy(data_+writeptr_, data, size);
        writeptr_ += size;
    }

    template <typename T>
    T read()
    {
        static_assert(std::is_pod<std::remove_reference_t<T>>::value,
                      "Only accept POD data!");

        assert(avail() >= sizeof(T));

        T t;
        read_size(&t, sizeof(T));
        return t;
    }

    void read_size(void *data, std::size_t size)
    {
        std::memcpy(data, data_+readptr_, size);
        readptr_ += size;
    }

private:
    std::size_t nlpo2(std::size_t s)
    {
        if (s == 1) return 2;
        auto v = float(s-1);
        return (std::size_t)(1 << ((*(std::uint32_t*)&v >> 23) - 126));
    }

private:
    void *data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t readptr_ = 0;
    std::size_t writeptr_ = 0;
};

} // !namespace rain