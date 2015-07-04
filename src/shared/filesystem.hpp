#pragma once

#include <string>
#include <vector>
#include <functional>
#include <algorithm>

extern "C"
{
#include <dirent.h>
#include <unistd.h>
}

namespace rain
{

bool resetWorkDir(std::string const &cmd)
{
    char buf[PATH_MAX] = {};
    getcwd(buf, PATH_MAX);

    auto wd = std::string(buf);
    auto pos = cmd.rfind('/');

    if (pos != std::string::npos)
        wd += '/' + cmd.substr(0, pos);
    return !(chdir(wd.c_str()));
}

std::vector<std::string> getSortedDirFiles(std::string const &path)
{
    using F = std::function<void(std::vector<std::string>&, std::string const &)>;

    std::vector<std::string> vs;
    F f = [&](auto &vec, auto const &dir) {
        auto dirp = opendir(dir.c_str());
        struct dirent *dp;
        while ((dp = readdir(dirp)) != nullptr)
        {
            std::string name = dp->d_name;
            if (name == "." || name == "..")
                continue;

            if (dp->d_type == DT_DIR)
                f(vec, dir + '/' + name);
            else if (dp->d_type == DT_REG)
                vec.push_back(dir + '/' + name);
        }
        closedir(dirp);
    };

    f(vs, path);
    std::sort(vs.begin(), vs.end());
    return vs;
}

} // !namespace rain

