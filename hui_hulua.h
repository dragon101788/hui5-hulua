#ifndef __HUI_HULUA_H__
#define __HUI_HULUA_H__

#include "hulua.h"

class LUA
{	
public:
	lua_State* L;
	LUA()
	{
	        printf("LUA\n");
		L = lua_open();
		luaopen_base(L);
	}
	~LUA()
	{
	        printf("~LUA");
		lua_close(L);
	}
	operator lua_State *()
        {
	    return L;
        }
	void dofile(const char * file)
	{
		hulua::dofile(L, file);
	}
	void dostring(const char * str)
        {
                hulua::dostring(L, str);
        }

	template<typename F>
	void register_func(const char * name,F func)
	{
	    hulua::def(L, name, func);
	}
	template<typename T>
	T call(const char * fun)
	{
	        return hulua::call<int>(L, fun);
	}

};

extern LUA lua;

#endif //__HUI_HULUA_H__
