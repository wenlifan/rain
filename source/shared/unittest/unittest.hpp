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

struct Test;

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

    void push_back(Test *t)
    {
        mTestVec.push_back(t);
    }

    std::vector<Test *> mTestVec;
};

struct Test
{
    Test()
    {
        TestContainer::getInstance().push_back(this);
    }

    virtual ~Test()
    {}

    virtual void test() = 0;
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

} // !namespace details

} // !namespace Rn

#define TEST_LOG(...)                                                       \
    Rn::details::testLog("        > ", __VA_ARGS__)


#define TEST_BEGIN(name)                                                    \
class test__##name : public Rn::details::Test                               \
{                                                                           \
public:                                                                     \
    test__##name();                                                         \
    virtual void test() override                                            \
    {                                                                       \
        Rn::details::testLogBegin(                                          \
                #name, " -- total: ", mTests.size(), " --\n");              \
        for (auto &i : mTests)                                              \
        {                                                                   \
            Rn::details::testSubBegin(i.first);                             \
            auto flag = true;                                               \
            try                                                             \
            {                                                               \
                flag = i.second();                                          \
            }                                                               \
            catch (std::exception &e)                                       \
            {                                                               \
                Rn::details::testLog('\n');                                 \
                TEST_LOG(e.what());                                         \
                flag = false;                                               \
            }                                                               \
            if (flag)                                                       \
            {                                                               \
                Rn::details::testSubOK('\n');                               \
            }                                                               \
            else                                                            \
            {                                                               \
                mFailCount++;                                               \
                Rn::details::testLog('\n');                                 \
                Rn::details::testSubFail('\n');                             \
            }                                                               \
        }                                                                   \
        if (mFailCount > 0)                                                 \
        {                                                                   \
            Rn::details::testLogFail(                                       \
                    "-- total: ",                                           \
                    mTests.size(),                                          \
                    " -- failed: ",                                         \
                    mFailCount,                                             \
                    " --\n");                                               \
        }                                                                   \
        else                                                                \
        {                                                                   \
            Rn::details::testLogOK('\n');                                   \
        }                                                                   \
    }                                                                       \
private:                                                                    \
    std::size_t mFailCount = 0;                                             \
    std::vector<std::pair<std::string, std::function<bool()>>> mTests;      \
} test__##name##__;                                                         \
test__##name::test__##name()                                                \
{                                                                           \
    (([]{ auto retflag__ = true;


#define TEST_END                                                            \
    return retflag__; }));                                                  \
}                                                                           \


#define TEST(name)                                                          \
    return retflag__; }));                                                  \
    mTests.push_back(std::make_pair(#name, []{ auto retflag__ = true;


#define EXPECTED(bool_expr, ...)                                            \
do {                                                                        \
if (!(bool_expr))                                                           \
{                                                                           \
    Rn::details::testLog('\n');                                             \
    TEST_LOG(__FILE__, " (line ", __LINE__, "): ", "Expected Fail -- ");    \
    Rn::details::testLogParams(__VA_ARGS__);                                \
    retflag__ = false;                                                      \
}                                                                           \
} while(false)


#define REQUIRED(bool_expr, ...)                                            \
do {                                                                        \
if (!(bool_expr))                                                           \
{                                                                           \
    Rn::details::testLog('\n');                                             \
    TEST_LOG(__FILE__, " (line ", __LINE__, "): ", "Required Fail -- ");    \
    Rn::details::testLogParams(__VA_ARGS__);                                \
    retflag__ = false;                                                      \
    return retflag__;                                                       \
}                                                                           \
} while(false)

#endif // !RAIN_UNIT_TEST

