#pragma once

#include <memory>

#include "byte_buffer.hpp"
#include "protocol.hpp"

#include "lua_binary.hpp"

namespace rain
{

#pragma pack(1)
struct MessageHeader
{
    std::uint32_t size;
    Protocol proto;
};
#pragma pack(0)

class MessagePack
    : public ByteBuffer
{
    using PackSizeType = decltype(MessageHeader::size);

public:
    MessagePack(
        Protocol proto = Protocol::COMM_NONE,
        std::size_t init_size = sizeof(MessageHeader),
        std::size_t growth = 128
    )
        : ByteBuffer(init_size, growth)
    {
        write_ptr(sizeof(PackSizeType));
        write(proto);
        flush();
        read_ptr(sizeof(MessageHeader));
    }

    std::size_t pack_size() const
    {
        std::uint32_t s;
        pos_read(0, s);
        return s;
    }

    Protocol protocol() const
    {
        Protocol proto;
        pos_read(sizeof(PackSizeType), proto);
        return proto;
    }

    void flush()
    {
        pos_write(0, PackSizeType(write_ptr()));
    }
};

using MessagePackPtr = std::shared_ptr<MessagePack>;

} // !namespace rain
