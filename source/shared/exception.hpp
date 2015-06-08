#pragma once

#include <string>
#include <exception>

namespace Rn
{

class Exception : public std::exception
{
public:
    Exception(std::string const &what)
        : mWhat(what)
    {}

    virtual ~Exception() = default;

    virtual const char * what() const noexcept override
    {
        return mWhat.c_str();
    }

private:
    std::string const &mWhat;
};

} // !namespace Rn

#define THROW(name, what)                                   \
    throw name(                                             \
            std::string(#name) + ": " + __FILE__            \
            + " (line " + std::to_string(__LINE__)          \
            + "): " + what)

#define DEFINE_EXCEPTION(name) \
class name : public Rn::Exception \
{ \
public: \
    name(std::string const &what) \
        : Rn::Exception(what) \
    {} \
}

