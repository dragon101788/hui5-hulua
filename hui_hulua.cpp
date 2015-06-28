#include "hui_hulua.h"
#include "xmlproc.h"
#include "layer.h"

LUA lua;


static void set_debug_timer(int cnt)
{
    debug_timer_count = cnt;
}

LUA::LUA()
{
  L = NULL;
  init();
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
  while(1)
  {
      SPNode ptr = q.getele();
      lock();
      ptr->DO(L);
      unlock();
  }
  destory();
}

extern void Jump(const char * jump);
extern void import_xml(const char * str)
{
    printf("import_xml %s\n",str);
    g_cur_xml->StartParseXML_str(str);
}

int LUA::init()
{
  lock();
  printf("LUA\n");
 L = lua_open();
         luaL_checkversion(L);  /* check that interpreter has correct version */
 luaopen_base(L);
         luaL_openlibs(L);  /* open standard libraries */

 hulua::def(L, "_ALERT", LUA::show_error);
 hulua::def(L, "debug_timer", set_debug_timer);
 hulua::def(L, "jump", Jump);
 hulua::def(L, "import_xml",import_xml);
 unlock();
}

