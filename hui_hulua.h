#ifndef __HUI_HULUA_H__
#define __HUI_HULUA_H__
#include "hulib.h"
#include "hulua.h"

class LUA
{	
public:
	lua_State* L;


	LUA();
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

	static void show_error(const char* error)
	{
	    printf("_ALERT -> %s\n", error);
	    fflush(stdout);
	    exit(-1);
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
