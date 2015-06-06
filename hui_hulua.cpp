#include "hui_hulua.h"
#include "xmlproc.h"

LUA lua;


static void set_debug_timer(int cnt)
{
    debug_timer_count = cnt;
}

LUA::LUA()
{
        printf("LUA\n");
        L = lua_open();
		luaL_checkversion(L);  /* check that interpreter has correct version */
        luaopen_base(L);
		luaL_openlibs(L);  /* open standard libraries */

        hulua::def(L, "_ALERT", LUA::show_error);
        hulua::def(L, "debug_timer", set_debug_timer);
}
