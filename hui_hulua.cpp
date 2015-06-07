#include "hui_hulua.h"
#include "xmlproc.h"

LUA lua;


static void set_debug_timer(int cnt)
{
    debug_timer_count = cnt;
}

LUA::LUA()
{
  L = NULL;
  create();
}

LUA::~LUA()
{
  destory();

}

int LUA::destory()
{
  if(L)
  {
    printf("~LUA");
     lua_close(L);
  }
}
int LUA::run()
{
  init();
  while(1)
  {
      q.getele()->DO(L);
  }
  destory();
}

int LUA::init()
{
  printf("LUA\n");
 L = lua_open();
         luaL_checkversion(L);  /* check that interpreter has correct version */
 luaopen_base(L);
         luaL_openlibs(L);  /* open standard libraries */

 hulua::def(L, "_ALERT", LUA::show_error);
 hulua::def(L, "debug_timer", set_debug_timer);

}
