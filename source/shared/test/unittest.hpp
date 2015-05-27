#pragma once

#include <vector>
#include <memory>

namespace Rn
{

struct Test;

class TestContainer
{
public:
    static TestContainer & getInstance()
    {
        static TestContainer instance;
        return instance;
    }

    void push_back(std::unique_ptr<Test> t)
    {
        mTestVec.push_back(std::move(t));
    }

private:
    TestContainer() = default;
    TestContainer(const TestContainer &) = delete;

private:
    std::vector<std::unique_ptr<Test>> mTestVec;
};

struct Test
{
    Test()
    {
        auto &c = TestContainer::getInstance();
        c.push_back(std::unique_ptr<Test>(this));
    }

    virtual ~Test()
    {}

    virtual void test() = 0;
};

}

#define BEGIN_TEST(name)        \
class name : public Rn::Test    \
{                               \
public:                         \
    name()                      \
    {                           \
        (void(0)                \

#define END_TEST                                \
        );                                      \
    }                                           \
    virtual void test() override                \
    {                                           \
        for (auto &i : mTests)                  \
            i();                                \
    }                                           \
private:                                        \
    std::vector<std::function<void()>> mTests;  \
};

#define TEST(name)              \
    );                          \
    mTests.push_back([this]



