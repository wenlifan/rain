#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <cstring>

namespace rain
{

class ByteBuffer
{
public:
    ByteBuffer(std::size_t init_size, std::size_t growth = 128)
        : size_(init_size)
        , growth_(growth)
    {
        if (init_size != 0)
            data_ = static_cast<char *>(std::malloc(size_));
    }

    ~ByteBuffer()
    {
        std::free(data_);
    }

public:
    std::size_t avail() const
    {
        return write_ptr_ - read_ptr_;
    }

    template <typename T>
    void write(T const &t)
    {
        write_size((char const *)&t, sizeof(T));
    }

    void write_size(char const *src, std::size_t size)
    {
        raw_write_size(src, size);
        write_ptr_ += size;
    }

    template <typename T>
    void raw_write(T const &t)
    {
        raw_write_size((char const *)&t, sizeof(T));
    }

    void raw_write_size(char const *src, std::size_t size)
    {
        if (size_ < write_ptr_ + size) {
            size_ = std::max(write_ptr_ + size, size_ + growth_);
            data_ = static_cast<char *>(std::realloc(data_, size_));
            assert(data_ != nullptr);
        }
        std::memcpy(data_+write_ptr_, src, size);
    }

    // Note: api unsafe
    template <typename T>
    void pos_write(std::size_t offset, T const &t)
    {
        pos_write_size(offset, (char const *)&t, sizeof(T));
    }

    // Note: api unsafe
    void pos_write_size(std::size_t offset, char const *src, std::size_t size)
    {
        std::memcpy(data_+offset, src, size);
    }

    template <typename T>
    void read(T &t)
    {
        read_size((char *)&t, sizeof(T));
    }

    void read_size(char *dest, std::size_t size)
    {
        raw_read_size(dest, size);
        read_ptr_ += size;
    }

    template <typename T>
    void raw_read(T &t) const
    {
        raw_read_size((char *)&t, sizeof(T));
    }

    void raw_read_size(char *dest, std::size_t size) const
    {
        assert(avail() >= size);
        std::memcpy(dest, data_ + read_ptr_, size);
    }

    // Note: api unsafe
    template <typename T>
    void pos_read(std::size_t offset, T &t) const
    {
        pos_read_size(offset, (char *)&t, sizeof(T));
    }

    // Note: api unsafe
    void pos_read_size(std::size_t offset, char *dest, std::size_t size) const
    {
        std::memcpy(dest, data_+offset, size);
    }

    std::size_t read_ptr() const
    {
        return read_ptr_;
    }

    void read_ptr(int n)
    {
        assert((long long)read_ptr_ + n >= 0);
        read_ptr_ += n;
        assert(read_ptr_ <= write_ptr_);
    }

    std::size_t write_ptr() const
    {
        return write_ptr_;
    }

    void write_ptr(int n)
    {
        assert((long long)write_ptr_ + n >= 0);
        write_ptr_ += n;
        if (write_ptr_ > size_) {
            size_ = std::max(write_ptr_, size_ + growth_);
            data_ = static_cast<char*>(std::realloc(data_, size_));
            assert(data_ != nullptr);
        }
    }

    char * data() const
    {
        return data_;
    }

    void allocate(std::size_t size = 128)
    {
        size_ += size;
        data_ = static_cast<char*>(std::realloc(data_, size_));
        assert(data_ != nullptr);
    }

private:
    char *data_{nullptr};
    std::size_t read_ptr_{0};
    std::size_t write_ptr_{0};
    std::size_t size_{0};
    std::size_t growth_{0};
};

} // !namspace rain