#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <functional>

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
    testLog("\n[ TEST ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void testLogOK(Args && ...args)
{
    testLog("\n[  OK  ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void testLogFail(Args && ...args)
{
    testLog("\n[ FAIL ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void testSubBegin(Args && ...args)
{
    testLog("\n    [ TEST ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void testSubOK(Args && ...args)
{
    testLog(" [ OK ] ", std::forward<Args>(args)...);
}

template <typename ...Args>
void testSubFail(Args && ...args)
{
    testLog("\n    [ FAIL ] ", std::forward<Args>(args)...);
}

} // !namespace details

} // !namespace Rn


#define TEST_BEGIN(name)                                                    \
class test__##name : public Rn::details::Test                               \
{                                                                           \
public:                                                                     \
    virtual void test() override                                            \
    {                                                                       \
        Rn::details::testLogBegin(                                          \
                #name, " -- total: ", mTests.size(), " --");                \
        for (auto &i : mTests)                                              \
        {                                                                   \
            Rn::details::testSubBegin(i.first);                             \
            int flag = 0;                                                   \
            i.second(flag);                                                 \
            if (flag == 0)                                                  \
            {                                                               \
                Rn::details::testSubOK();                                   \
            }                                                               \
            else                                                            \
            {                                                               \
                mFailCount++;                                               \
                Rn::details::testSubFail();                                 \
            }                                                               \
        }                                                                   \
        if (mFailCount > 0)                                                 \
        {                                                                   \
            Rn::details::testLogFail(                                       \
                    "-- total: ",                                           \
                    mTests.size(),                                          \
                    " -- failed: ",                                         \
                    mFailCount,                                             \
                    " --");                                                 \
        }                                                                   \
        else                                                                \
        {                                                                   \
            Rn::details::testLogOK();                                       \
        }                                                                   \
        Rn::details::testLog('\n');                                         \
    }                                                                       \
    test__##name()                                                          \
    {                                                                       \
        (void(0


#define TEST_END                                                            \
        ));                                                                 \
    }                                                                       \
private:                                                                    \
    std::size_t mFailCount = 0;                                             \
    std::vector<std::pair<std::string, std::function<void(int &)>>> mTests; \
} test__##__LINE__##__;

#define TEST(name)                                                          \
    ));                                                                     \
    mTests.push_back(std::make_pair(#name, [](int &retflag__)


#define TEST_LOG(...)                                                       \
    Rn::details::testLog("\n        > ", __VA_ARGS__)

#define EXPECTED(bool_expr, ...)                                            \
do { \
if (!(bool_expr))                                                           \
{                                                                           \
    TEST_LOG(__FILE__, "(ln ", __LINE__, "): ", "Expected Fail -- ");       \
    Rn::details::testLogParams(__VA_ARGS__);                                \
    retflag__ = 1;                                                          \
} \
} while(false)

#define REQUIRED(bool_expr, ...)                                            \
if (!(bool_expr))                                                           \
{                                                                           \
    TEST_LOG(__FILE__, "(ln ", __LINE__, "): ", "Required Fail -- ");       \
    Rn::details::testLogParams(__VA_ARGS__);                                \
    retflag__ = 2;                                                          \
    return;                                                                 \
}

