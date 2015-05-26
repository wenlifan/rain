#pragma once

#include <string>

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

    bool append(std::string str)
    {
    }

private:
    std::string mPath;
};

}

