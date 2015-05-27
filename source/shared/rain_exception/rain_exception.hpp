#pragma once

#include <string>
#include <exception>

namespace Rn
{

class RainException : public std::exception
{
public:
    RainException(
            std::string what,
            std::string type = "",
            const char *filename = nullptr,
            int linenum = -1)
    {
        if (type.empty())
            type = "UnKnow";

        mWhat = mWhat + std::move(type);

        if (filename != nullptr)
        {
            mWhat = mWhat + ": " + filename + " (line "
                    + std::to_string(linenum)
                    + "): ";
        }
        mWhat = mWhat + std::move(what);
    }

    virtual const char * what() const noexcept override
    {
        return mWhat.c_str();
    }

private:
    std::string mWhat;
};

}

#define THROW(name, what) \
    throw name(what, #name, __FILE__, __LINE__)

#define DEFINE_EXCEPTION(name) \
class name : public Rn::RainException \
{ \
public: \
    name(std::string what, \
         std::string type, \
         const char *filename, \
         int linenum) \
        : Rn::RainException(std::move(what), std::move(type), filename, linenum) \
    {} \
}

