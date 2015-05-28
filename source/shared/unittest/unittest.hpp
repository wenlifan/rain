#pragma once

#ifdef RAIN_UNIT_TEST

#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <exception>

namespace Rn
{

namespace details
{

class TestContainer
{
    TestContainer() = default;
    TestContainer(const TestContainer &) = delete;

public:
    static TestContainer & getInstance()
    {
        static TestContainer instance;
        return instance;
    }

    void push_back(std::function<void()> f)
    {
        mTestVec.push_back(std::move(f));
    }

    std::vector<std::function<void()>> mTestVec;
};

template <typename ...Args>
void testLog(Args && ...args)
{
    (void)std::initializer_list<int>{(std::cout << args, 0)...};
}

template <typename ...Args>
void testLogParams(Args && ...args)
{
    if (sizeof...(args) == 0)
        return;

    std::cout << "Params: { ";
    (void)std::initializer_list<int>{(std::cout << args << ' ', 0)...};
    std::cout << "}";
}

template <typename ...Args>
void testLogBegin(Args && ...args)
{
    testLog("[ TEST ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void testLogOK(Args && ...args)
{
    testLog("[  OK  ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void testLogFail(Args && ...args)
{
    testLog("[ FAIL ] ", std::forward<Args>(args)...);
}

template <typename ...Args> void testSubBegin(Args && ...args) {
    testLog("    [ TEST ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void testSubOK(Args && ...args)
{
    testLog(" [ OK ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void testSubFail(Args && ...args)
{
    testLog("    [ FAIL ] ", std::forward<Args>(args)...);
}

} // !namespace details

} // !namespace Rn


#define TEST_LOG(...)                                                       \
    Rn::details::testLog("        > ", __VA_ARGS__)


#define TEST_BEGIN(name)                                                    \
auto test_func_##name##__ = [] {                                            \
    auto test_name = #name;                                                 \
    auto main_test = [test_name] {                                          \
        std::vector<std::pair<std::string, std::function<void()>>> tests;   \
        auto test_flag__ = true;                                            \
        auto fail_count = 0;                                                \
        (([]{}


#define TEST_END                                                            \
        ));                                                                 \
        Rn::details::testLogBegin(                                          \
                test_name, " -- total: ", tests.size(), " --\n");           \
        for (auto &i : tests) {                                             \
            Rn::details::testSubBegin(i.first);                             \
            test_flag__ = true;                                             \
            try {                                                           \
                i.second();                                                 \
            }                                                               \
            catch (std::exception &e) {                                     \
                Rn::details::testLog('\n');                                 \
                TEST_LOG(e.what());                                         \
                test_flag__ = false;                                        \
            }                                                               \
            if (test_flag__)                                                \
                Rn::details::testSubOK('\n');                               \
            else                                                            \
            {                                                               \
                fail_count++ ;                                              \
                Rn::details::testLog('\n');                                 \
                Rn::details::testSubFail('\n');                             \
            }                                                               \
        }                                                                   \
        if (fail_count > 0) {                                               \
            Rn::details::testLogFail(                                       \
                    "-- total: ",                                           \
                    tests.size(),                                           \
                    " -- failed: ",                                         \
                    fail_count,                                             \
                    " --\n");                                               \
        }                                                                   \
        else                                                                \
            Rn::details::testLogOK('\n');                                   \
    };                                                                      \
    Rn::details::TestContainer::getInstance().push_back(main_test);         \
    return 0;                                                               \
}();


#define TEST(name)                                                          \
    ));                                                                     \
    tests.push_back(std::make_pair(#name, [&test_flag__]


#define EXPECTED(bool_expr, ...)                                            \
do {                                                                        \
if (!(bool_expr))                                                           \
{                                                                           \
    Rn::details::testLog('\n');                                             \
    TEST_LOG(__FILE__, " (line ", __LINE__, "): ", "Expected Fail -- ");    \
    Rn::details::testLogParams(__VA_ARGS__);                                \
    test_flag__ = false;                                                    \
}                                                                           \
} while(false)


#define REQUIRED(bool_expr, ...)                                            \
do {                                                                        \
if (!(bool_expr))                                                           \
{                                                                           \
    Rn::details::testLog('\n');                                             \
    TEST_LOG(__FILE__, " (line ", __LINE__, "): ", "Required Fail -- ");    \
    Rn::details::testLogParams(__VA_ARGS__);                                \
    test_flag__ = false;                                                    \
    return;                                                                 \
}                                                                           \
} while(false)

#endif // !RAIN_UNIT_TEST


