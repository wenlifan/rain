#include "byte_buffer.hpp"
#include "unittest.hpp"

TEST_BEGIN(byte_buffer)
    TEST(size)
    {
        rain::ByteBuffer bf1;
        REQUIRED(bf1.size() == 128);
        rain::ByteBuffer bf2(1);
        REQUIRED(bf2.size() == 1);
        bf2.write('a');
        REQUIRED(bf2.size() == 1);
        bf2.write('a');
        REQUIRED(bf2.size() == 2);
        bf2.write('a');
        REQUIRED(bf2.size() == 4);
        bf2.write('a');
        REQUIRED(bf2.size() == 4);
        bf2.write('a');
        REQUIRED(bf2.size() == 8);
    }
    TEST(avail)
    {
        rain::ByteBuffer bf1(1);
        REQUIRED(bf1.avail() == 0);
        bf1.write(3);
        REQUIRED(bf1.avail() == 4);
        bf1.write('a');
        REQUIRED(bf1.avail() == 5);
        bf1.write('b');
        REQUIRED(bf1.avail() == 6);
        bf1.write('c');
        REQUIRED(bf1.avail() == 7);
        bf1.write(3.1415926);
        REQUIRED(bf1.avail() == 15);
        bf1.write(2.587f);
        REQUIRED(bf1.avail() == 19);
        bf1.write(short(67));
        REQUIRED(bf1.avail() == 21);
        REQUIRED(bf1.size() == 32);
    }
    TEST(read)
    {
        rain::ByteBuffer bf1;
        bf1.write(3);
        bf1.write('a');
        bf1.write('b');
        bf1.write('c');
        bf1.write(3.1415926);
        bf1.write(2.587f);
        bf1.write(short(67));
        auto i = bf1.read<int>();
        REQUIRED(i == 3);
        auto c1 = bf1.read<char>();
        REQUIRED(c1 == 'a', c1);
        auto c2 = bf1.read<char>();
        REQUIRED(c2 == 'b');
        auto c3 = bf1.read<char>();
        REQUIRED(c3 == 'c');
        auto d = bf1.read<double>();
        REQUIRED(d - 3.1415926 < 0.0000001);
        auto f = bf1.read<float>();
        REQUIRED(f - 2.587f < 0.00001);
        auto s = bf1.read<short>();
        REQUIRED(s == 67);
        REQUIRED(bf1.avail() == 0);
    }
TEST_END