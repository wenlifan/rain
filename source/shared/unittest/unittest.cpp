#include "unittest.hpp"

int main()
{
    auto &c = Rn::details::TestContainer::getInstance();
    for (auto &i : c.mTestVec)
        i();
}
