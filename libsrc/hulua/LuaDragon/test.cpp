#include <stdio.h>
#include "hulua.h"



class window
{
public:
    int x;
    int y;
    char path[256];
    window()
    {
      printf("init window\n");
    }
    ~window()
     {
       printf("destroy window\n");
     }
    static void lua_create(lua_State* L,const char * name)
    {
	printf("lua create\n");
        hulua::class_add<window>(L, "window");
        hulua::class_mem<window>(L, "x", &window::x);
        hulua::class_mem<window>(L, "y", &window::y);
        hulua::class_def<window>(L, "showinfo", &window::showinfo);
        hulua::set(L, name, new window);
    }
    void showinfo()
    {
        printf("x=%d y=%d path=%s\n",x,y,path);
    }
};


lua_State* L;
int register_class(const char * name)
{
    window::lua_create(L,name);
}

int main(int argc,char * argv[])
{
  L = lua_open();
  luaopen_base(L);
  luaL_dofile(L,"hu.lua");

  hulua::def(L, "register_class", register_class);


  int result = hulua::call<int>(L, "main");

  lua_close(L);                   //关闭虚拟机
}
