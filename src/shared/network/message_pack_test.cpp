#include "message_pack.hpp"
#include "unittest.hpp"
#include "protocol.hpp"

using namespace rain;

TEST_BEGIN(message_pack)
TEST(size)
{
    rain::MessagePack mp1(GW_GS_BEGIN);
    REQUIRED(mp1.size() == 6, mp1.size());
    REQUIRED(mp1.data_size() == 0, mp1.data_size());
    mp1.write(2);
    REQUIRED(mp1.size() == 10, mp1.size());
    REQUIRED(mp1.data_size() == 4, mp1.data_size());
    mp1.write('a');
    REQUIRED(mp1.size() == 11, mp1.size());
    REQUIRED(mp1.data_size() == 5, mp1.data_size());
    mp1.write('b');
    REQUIRED(mp1.size() == 12, mp1.size());
    REQUIRED(mp1.data_size() == 6, mp1.data_size());
    REQUIRED(mp1.protocol() == GW_GS_BEGIN);
}
TEST(avail)
{
}
TEST(read)
{
}
TEST_END