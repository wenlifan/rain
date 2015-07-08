#include <iostream>
#include <chrono>
#include <thread>

#include "asio.hpp"

#include "client_node.hpp"
#include "session.hpp"
#include "server_session_manager.hpp"

int main(void)
{
    auto client = rain::ClientNode<rain::Session<rain::ServerSessionManager>>();

    client.connect("127.0.0.1", 5000);

    std::cout << "Connect to Server..." << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(99999999));

    //std::cout << "Hello, World!\n";
}
