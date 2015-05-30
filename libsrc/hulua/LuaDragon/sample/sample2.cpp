// sample2.cpp : C++ �� Lua ��ȣ���� ���� ������ �˾ƺ���.
//

extern "C" 
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
};

#include "hulua.h"

static int cpp_int = 100;

int main()
{
	lua_State* L = lua_open();

	luaopen_base(L);

	hulua::set(L, "cpp_int", cpp_int);

	hulua::dofile(L, "sample2.lua");

	int lua_int = hulua::get<int>(L, "lua_int");

	printf("lua_int = %d\n", lua_int);

	lua_close(L);

	return 0;
}
