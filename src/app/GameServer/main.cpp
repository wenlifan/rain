#include <iostream>
#include <chrono>
#include <iostream>

#include "filesystem.hpp"

#include "server_node.hpp"
#include "session.hpp"
#include "server_session_manager.hpp"

int main(int argc, char *argv[])
{
    rain::reset_work_dir(argv[0]);

    auto vec = rain::get_sorted_dir_files(".");
    for (auto &v : vec)
        std::cout << v << std::endl;

    try
    {
        auto server = rain::ServerNode<rain::Session<rain::ServerSessionManager>>(5000);
        server.start();
        std::cout << "Server Started!" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(9999999));
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    //std::cout << "End!\n";
}
