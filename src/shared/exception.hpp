#pragma once

#include <string>
#include <exception>

namespace rain
{

class Exception : public std::exception
{
public:
    Exception(std::string const &what)
        : what_(what)
    {}

    virtual ~Exception() = default;

    virtual const char * what() const noexcept override
    {
        return what_.c_str();
    }

private:
    std::string const &what_;
};

} // !namespace rain

#define THROW(name, what)                                   \
    throw name(                                             \
            std::string(#name) + ": " + __FILE__            \
            + " (line " + std::to_string(__LINE__)          \
            + "): " + what)

#define DEFINE_EXCEPTION(name) \
class name : public rain::Exception \
{ \
public: \
    name(std::string const &what) \
        : rain::Exception(what) \
    {} \
}

