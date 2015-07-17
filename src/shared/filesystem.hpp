#pragma once

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include <cassert>

extern "C"
{
#include <dirent.h>
#include <unistd.h>
}

namespace rain
{

bool reset_work_dir(std::string const &cmd)
{
    auto dir = get_current_dir_name();
    assert(dir);

    auto wd = std::string(dir);
    std::free(dir);

    auto pos = cmd.rfind('/');

    if (pos != std::string::npos)
        wd += '/' + cmd.substr(0, pos);
    return !(chdir(wd.c_str()));
}

std::vector<std::string> get_sorted_dir_files(std::string const &path)
{
    using F = std::function<void(std::vector<std::string>&, std::string const &)>;

    std::vector<std::string> vs;
    F f = [&](auto &vec, auto const &dir) {
        auto dirp = opendir(dir.c_str());
        struct dirent *dp;
        while ((dp = readdir(dirp)) != nullptr) {
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

