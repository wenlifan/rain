#pragma once

#include <string>

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
    explicit Path(std::string path)
        : mPath(path)
    {}
    

private:
    struct stat mStat;
    DIR * mDir = nullptr;

    bool mStatus = false;
    std::string mPath;
};

}

