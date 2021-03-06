cmake_minimum_required(VERSION 3.0)

set(LUA_HEADERS
    lua.h
    luaconf.h
    lualib.h
    lauxlib.h
    lua.hpp
)

set(LUA_LIBRARY_SOURCE
    lapi.c
    lcode.c
    lctype.c
    ldebug.c
    ldo.c
    ldump.c
    lfunc.c
    lgc.c
    llex.c
    lmem.c
    lobject.c
    lopcodes.c
    lparser.c
    lstate.c
    lstring.c
    ltable.c
    ltm.c
    lundump.c
    lvm.c
    lzio.c
    lauxlib.c
    lbaselib.c
    lbitlib.c
    lcorolib.c
    ldblib.c
    liolib.c
    lmathlib.c
    loslib.c
    lstrlib.c
    ltablib.c
    lutf8lib.c
    loadlib.c
    linit.c
)

add_library(lua_static STATIC ${LUA_HEADERS} ${LUA_LIBRARY_SOURCE})
