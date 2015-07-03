#pragma once

#include <string>
#include <iostream>
#include <cassert>

#include "strutils.hpp"
#include "exception.hpp"

extern "C"
{ 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
}

namespace rain
{

class Path
{
public:
    Path() = default;

    explicit Path(std::string path)
        : path_(std::move(path))
    {}

    //Path(Path const &) = default;
    //Path(Path &&) = default;
    //Path & operator = (Path const &) = default;
    //Path & operator = (Path &&) = default;
    
    friend std::ostream & operator << (std::ostream &os, Path const &p)
    {
        os << p.path_;
        return os;
    }
    
    bool isAbsolute() const
    {
        return !path_.empty() && path_.front() == '/';
    }

    bool isRelative() const
    {
        return !path_.empty() && path_.front() != '/';
    }

    bool isRoot() const
    {
        return path_ == "/";
    }

    Path & tidy()
    {
        path_ = replace(path_, "/./", "/");
        if (!endWith(path_, "/..") && path_.back() == '.')
            path_.pop_back();
        
        auto pos = path_.find("/../");
        while (pos != std::string::npos)
        {
            while (pos == 0)
            {
                path_.erase(0, 3);
                pos = path_.find("/../");
            }

            auto s = path_.rfind('/', pos-1);
            path_.erase(s, pos+3-s);
            pos = path_.find("/../", s);
        }
        
        if (endWith(path_, "/.."))
        {
            auto p = path_.rfind('/', path_.length()-4);
            path_.resize(p);
        }

        return *this;
    }

    Path & absolute()
    {
        assert(isRelative());
        *this = Path().current().append(path_);

        return *this;
    }

    Path getAbsolute() const
    {
        return Path(path_).absolute();
    }

    Path & current()
    {
        char arr[PATH_MAX];
        assert(getcwd(arr, PATH_MAX));

        path_ = arr;
        return *this;
    }

    Path getCurrent() const
    {
        return Path().current();
    }

    Path & append(std::string str)
    {
        auto s = rain::trim(std::move(str));
        if (path_.empty())
        {
            path_ = std::move(s);
            return *this;
        }

        if (path_.back() != '/')
            path_ += '/';
        
        path_ += std::move(s);

        if (path_.length() == 1 && path_.front() == '/')
            path_.pop_back();

        return *this;
    }

    Path getAppend(std::string str) const
    {
        return Path(path_).append(std::move(str));
    }

    Path & parent()
    {
        if (isRoot())
            return *this;

        if (path_.back() == '/')
            path_.pop_back();

        auto pos = path_.rfind('/');

        if (pos == std::string::npos)
            return *this;

        if (pos == 0)
            pos++;

        path_.erase(pos, path_.size()-pos);

        return *this;
    }

    Path getParent() const
    {
        return Path(path_).parent();
    }

    bool exists() const
    {
        if (access(path_.c_str(), F_OK))
            return false;

        return true;
    }

private:
    std::string path_;
};

} // !namespace rain

