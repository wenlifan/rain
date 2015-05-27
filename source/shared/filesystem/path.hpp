#pragma once

#include <string>

#include "utils.hpp"

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
    
    bool absolute() const
    {
        return !mPath.empty() && mPath.front() == '/';
    }

    bool relative() const
    {
        return !mPath.empty() && mPath.front() != '/';
    }

    void append(std::string str)
    {
        auto s = Rn::trim(std::move(str));
        if (mPath.empty())
        {
            mPath = std::move(s);
            return;
        }

        if (mPath[mPath.size()-1] != '/')
            mPath += '/';
        
        mPath += std::move(s);
    }

private:
    std::string mPath;
};

} // !namespace Rn

