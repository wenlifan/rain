#include "filesystem.hpp"
#include "app.hpp"

int main(int, char *argv[])
{
    rain::reset_work_dir(argv[0]);
    rain::App::get_instance().run();
}
