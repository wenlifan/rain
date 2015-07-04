#include <iostream>

#include "filesystem.hpp"

int main(int argc, char *argv[])
{
    std::cout << "Command: " << argv[0] << std::endl;
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    std::cout << "Current Work Dir: " << buf << std::endl;
    rain::resetWorkDir(argv[0]);
    getcwd(buf, PATH_MAX);
    std::cout << "Current Work Dir: " << buf << std::endl;

    auto vec = rain::getSortedDirFiles(".");
    for (auto &v : vec)
    {
        std::cout << v << std::endl;
    }

    std::cout << "Hello, World!\n";
}
