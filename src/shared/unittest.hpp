#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <exception>

#include "exception.hpp"
#include "singleton.hpp"

namespace rain
{

namespace details_test
{

class TestContainer : public Singleton<TestContainer>
{
    friend Singleton<TestContainer>;
    TestContainer() = default;

    using Tests = std::vector<std::function<void()>>;

public:
    void add_test(std::function<void()> f)
    {
        mTestVec.push_back(std::move(f));
    }

    Tests &get_tests()
    {
        return mTestVec;
    }

private:
    Tests mTestVec;
};

template <typename ...Args>
void test_log(Args &&...args)
{
    (void)std::initializer_list<int>{(std::cout << args, 0)...};
}

template <typename ...Args>
void test_log_params(Args &&...args)
{

    if (sizeof...(args) == 0)
        return;

    std::cout << "Params: { ";
    test_log(std::forward<Args>(args)...);
    std::cout << " }";
}

template <typename ...Args>
void test_log_begin(Args &&...args)
{
    test_log("[ TEST ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void test_log_ok(Args &&...args)
{
    test_log("[  OK  ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void test_log_fail(Args &&...args)
{
    test_log("[ FAIL ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void test_sub_begin(Args &&...args)
{
    test_log("    [ TEST ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void test_sub_ok(Args &&...args)
{
    test_log(" [ OK ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void test_sub_fail(Args &&...args)
{
    test_log("    [ FAIL ] ", std::forward<Args>(args)...);
}

} // !namespace details_test

} // !namespace rain


#define TEST_LOG(...)                                                       \
    rain::details_test::test_log("        > ", __VA_ARGS__)


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
        rain::details_test::test_log_begin(                                 \
                test_name, " -- total: ", tests.size(), " --\n");           \
        for (auto &i : tests) {                                             \
            rain::details_test::test_sub_begin(i.first);                    \
            test_flag__ = true;                                             \
            try {                                                           \
                i.second();                                                 \
            }                                                               \
            catch (std::exception &e) {                                     \
                rain::details_test::test_log('\n');                         \
                TEST_LOG(e.what());                                         \
                test_flag__ = false;                                        \
            }                                                               \
            if (test_flag__)                                                \
                rain::details_test::test_sub_ok('\n');                      \
            else                                                            \
            {                                                               \
                fail_count++ ;                                              \
                rain::details_test::test_log('\n');                         \
                rain::details_test::test_sub_fail('\n');                    \
            }                                                               \
        }                                                                   \
        if (fail_count > 0) {                                               \
            rain::details_test::test_log_fail(                              \
                    " -- failed: ", fail_count, " --\n");                   \
        }                                                                   \
        else                                                                \
            rain::details_test::test_log_ok('\n');                          \
    };                                                                      \
    rain::details_test::TestContainer::get_instance().add_test(main_test);  \
    return 0;                                                               \
}();


#define TEST(name)                                                          \
    ));                                                                     \
    tests.push_back(std::make_pair(#name, [&test_flag__]


#define EXPECTED(bool_expr, ...)                                            \
do {                                                                        \
if (!(bool_expr))                                                           \
{                                                                           \
    rain::details_test::test_log('\n');                                     \
    TEST_LOG(__FILE__, " (line ", __LINE__, "): ", "Expected Fail -- ");    \
    rain::details_test::test_log_params(__VA_ARGS__);                       \
    test_flag__ = false;                                                    \
}                                                                           \
} while(false)


#define REQUIRED(bool_expr, ...)                                            \
do {                                                                        \
if (!(bool_expr))                                                           \
{                                                                           \
    rain::details_test::test_log('\n');                                     \
    TEST_LOG(__FILE__, " (line ", __LINE__, "): ", "Required Fail -- ");    \
    rain::details_test::test_log_params(__VA_ARGS__);                       \
    test_flag__ = false;                                                    \
    return;                                                                 \
}                                                                           \
} while(false)



