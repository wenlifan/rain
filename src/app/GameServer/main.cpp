#include <iostream>
#include <chrono>

#include "filesystem.hpp"

#include "server_node.hpp"
#include "base_session.hpp"

int main(int argc, char *argv[])
{
    rain::resetWorkDir(argv[0]);

    auto vec = rain::getSortedDirFiles(".");
    for (auto &v : vec)
        std::cout << v << std::endl;

    auto server = rain::ServerNode<rain::BaseSession>(9997);
    server.start();

    for(;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    std::cout << "End!\n";
}
