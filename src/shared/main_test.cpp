#include "unittest.hpp"

int main()
{
    auto &c = rain::details_test::TestContainer::get_instance();
    for (auto &i : c.get_tests())
        i();
}
