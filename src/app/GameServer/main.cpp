#include <iostream>
#include <chrono>

#include "asio.hpp"
#include "filesystem.hpp"

int main(int argc, char *argv[])
{
    rain::resetWorkDir(argv[0]);

    auto vec = rain::getSortedDirFiles(".");
    for (auto &v : vec)
        std::cout << v << std::endl;

    std::cout << "Hello, World!\n";
}
