#include <iostream>
#include <chrono>
#include <thread>

#include "asio.hpp"

#include "client_node.hpp"
#include "base_session.hpp"

int main(void)
{
    auto client = rain::ClientNode<rain::BaseSession>();

    for (;;)
    {
        client.connect("127.0.0.1", 9997);

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    std::cout << "Hello, World!\n";
}
