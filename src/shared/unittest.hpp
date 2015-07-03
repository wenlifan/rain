#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <exception>

#include "exception.hpp"

namespace rain
{

namespace details_test
{

class TestContainer
{
    using Tests = std::vector<std::function<void()>>;

    TestContainer() = default;
    TestContainer(const TestContainer &) = delete;
public:
    static TestContainer & getInstance()
    {
        static TestContainer instance;
        return instance;
    }

    void addTest(std::function<void()> f)
    {
        mTestVec.push_back(std::move(f));
    }

    Tests & getTests()
    {
        return mTestVec;
    }

private:
    Tests mTestVec;
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

template <typename ...Args>
void testSubBegin(Args && ...args)
{
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

} // !namespace details_test

} // !namespace rain


#define TEST_LOG(...)                                                       \
    rain::details_test::testLog("        > ", __VA_ARGS__)


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
        rain::details_test::testLogBegin(                                   \
                test_name, " -- total: ", tests.size(), " --\n");           \
        for (auto &i : tests) {                                             \
            rain::details_test::testSubBegin(i.first);                      \
            test_flag__ = true;                                             \
            try {                                                           \
                i.second();                                                 \
            }                                                               \
            catch (std::exception &e) {                                     \
                rain::details_test::testLog('\n');                          \
                TEST_LOG(e.what());                                         \
                test_flag__ = false;                                        \
            }                                                               \
            if (test_flag__)                                                \
                rain::details_test::testSubOK('\n');                        \
            else                                                            \
            {                                                               \
                fail_count++ ;                                              \
                rain::details_test::testLog('\n');                          \
                rain::details_test::testSubFail('\n');                      \
            }                                                               \
        }                                                                   \
        if (fail_count > 0) {                                               \
            rain::details_test::testLogFail(                                \
                    " -- failed: ", fail_count, " --\n");                   \
        }                                                                   \
        else                                                                \
            rain::details_test::testLogOK('\n');                            \
    };                                                                      \
    rain::details_test::TestContainer::getInstance().addTest(main_test);    \
    return 0;                                                               \
}();


#define TEST(name)                                                          \
    ));                                                                     \
    tests.push_back(std::make_pair(#name, [&test_flag__]


#define EXPECTED(bool_expr, ...)                                            \
do {                                                                        \
if (!(bool_expr))                                                           \
{                                                                           \
    rain::details_test::testLog('\n');                                      \
    TEST_LOG(__FILE__, " (line ", __LINE__, "): ", "Expected Fail -- ");    \
    rain::details_test::testLogParams(__VA_ARGS__);                         \
    test_flag__ = false;                                                    \
}                                                                           \
} while(false)


#define REQUIRED(bool_expr, ...)                                            \
do {                                                                        \
if (!(bool_expr))                                                           \
{                                                                           \
    rain::details_test::testLog('\n');                                      \
    TEST_LOG(__FILE__, " (line ", __LINE__, "): ", "Required Fail -- ");    \
    rain::details_test::testLogParams(__VA_ARGS__);                         \
    test_flag__ = false;                                                    \
    return;                                                                 \
}                                                                           \
} while(false)



