#include "unittest.hpp"

TEST_BEGIN(test1_main)
    TEST(Hello)
    {
        EXPECTED(1==2, "lalala");
        REQUIRED(1==2, "why");
    }
    TEST(World)
    {
        THROW(std::logic_error, "Logic Error!");
    }
TEST_END

TEST_BEGIN(test2_main)
TEST_END

int main()
{
    auto &c = rain::details_test::TestContainer::getInstance();
    for (auto &i : c.getTests())
        i();
}

