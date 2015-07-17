#include <iostream>
#include <chrono>
#include <iostream>

#include "filesystem.hpp"

#include "server_node.hpp"
#include "session.hpp"
#include "server_session_manager.hpp"

#include "lua_environment.hpp"
#include "console.hpp"

static int say_hello(lua_State *L)
{
    std::string str = luaL_checkstring(L, -1);
    RAIN_INFO("C++: Hello, " + str);
    lua_pushstring(L, str.c_str());
    return 1;
}

int main(int, char *argv[])
{
    rain::reset_work_dir(argv[0]);

    auto vec = rain::get_sorted_dir_files(".");
    for (auto &v : vec)
        std::cout << v << std::endl;

//    try
//    {
//        auto server = rain::ServerNode<rain::Session<rain::ServerSessionManager>>(5000);
//        server.start();
//        std::cout << "Server Started!" << std::endl;
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(9999999));
//    }
//    catch (std::exception &e)
//    {
//        std::cout << e.what() << std::endl;
//    }

    rain::LuaEnvironment lenv;
    luaL_Reg reg[] = {
        {"SayHello", say_hello},
        {nullptr, nullptr}
    };
    //lenv.add_function("SayHello", say_hello);
    lenv.add_functions(reg);
    lenv.do_file("test.lua");
    lenv.call("hello", nullptr, "mogu", "tingting");
    rain::Console::get_instance().run();


    //std::cout << "End!\n";
}
