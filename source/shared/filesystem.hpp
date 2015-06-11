#pragma once

#include <string>
#include <iostream>
#include <cassert>

#include "utils.hpp"
#include "exception.hpp"

extern "C"
{ 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
}

namespace Rn
{

class Path
{
public:
    Path() = default;

    explicit Path(std::string path)
        : mPath(std::move(path))
    {}

    //Path(Path const &) = default;
    //Path(Path &&) = default;
    //Path & operator = (Path const &) = default;
    //Path & operator = (Path &&) = default;
    
    friend std::ostream & operator << (std::ostream &os, Path const &p)
    {
        os << p.mPath;
        return os;
    }
    
    bool isAbsolute() const
    {
        return !mPath.empty() && mPath.front() == '/';
    }

    bool isRelative() const
    {
        return !mPath.empty() && mPath.front() != '/';
    }

    bool isRoot() const
    {
        return mPath == "/";
    }

    Path & tidy()
    {
        mPath = replace(mPath, "/./", "/");
        if (!endWith(mPath, "/..") && mPath.back() == '.')
            mPath.pop_back();
        
        auto pos = mPath.find("/../");
        while (pos != std::string::npos)
        {
            while (pos == 0)
            {
                mPath.erase(0, 3);
                pos = mPath.find("/../");
            }

            auto s = mPath.rfind('/', pos-1);
            mPath.erase(s, pos+3-s);
            pos = mPath.find("/../", s);
        }
        
        if (endWith(mPath, "/.."))
        {
            auto p = mPath.rfind('/', mPath.length()-4);
            mPath.resize(p);
        }

        return *this;
    }

    Path & absolute()
    {
        assert(isRelative());
        *this = Path().current().append(mPath);

        return *this;
    }

    Path getAbsolute() const
    {
        return Path(mPath).absolute();
    }

    Path & current()
    {
        char arr[PATH_MAX];
        assert(getcwd(arr, PATH_MAX));

        mPath = arr;
        return *this;
    }

    Path getCurrent() const
    {
        return Path().current();
    }

    Path & append(std::string str)
    {
        auto s = Rn::trim(std::move(str));
        if (mPath.empty())
        {
            mPath = std::move(s);
            return *this;
        }

        if (mPath.back() != '/')
            mPath += '/';
        
        mPath += std::move(s);

        if (mPath.length() == 1 && mPath.front() == '/')
            mPath.pop_back();

        return *this;
    }

    Path getAppend(std::string str) const
    {
        return Path(mPath).append(std::move(str));
    }

    Path & parent()
    {
        if (isRoot())
            return *this;

        if (mPath.back() == '/')
            mPath.pop_back();

        auto pos = mPath.rfind('/');

        if (pos == std::string::npos)
            return *this;

        if (pos == 0)
            pos++;

        mPath.erase(pos, mPath.size()-pos);

        return *this;
    }

    Path getParent() const
    {
        return Path(mPath).parent();
    }

    bool exists() const
    {
        if (access(mPath.c_str(), F_OK))
            return false;

        return true;
    }

private:
    std::string mPath;
};

} // !namespace Rn

