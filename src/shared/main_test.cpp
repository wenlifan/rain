#include "unittest.hpp"

int main()
{
    auto &c = rain::details_test::TestContainer::getInstance();
    for (auto &i : c.getTests())
        i();
}
