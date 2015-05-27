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
        mWhat = "[Exception Occured]\n";

        if (type.empty())
            type = "UnKnow";

        mWhat = mWhat + "Type:\t" + std::move(type) + '\n';
        mWhat = mWhat + "What:\t" + std::move(what) + '\n';

        if (filename != nullptr)
        {
            mWhat = mWhat + "Locate:\tLine "
                    + std::to_string(linenum)
                    + " in " + filename + '\n';
        }
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

