
#include <iostream>


#include "hulua.h"

int cpp_func(int arg1, int arg2)
{
	return arg1 + arg2;
}

int main()
{
	lua_State* L = lua_open();

	luaopen_base(L);

	hulua::def(L, "cpp_func", cpp_func);

	hulua::dofile(L, "sample1.lua");

	int result = hulua::call<int>(L, "lua_func", 3, 4);

	printf("lua_func(3,4) = %d\n", result);

	lua_close(L);

	return 0;
}
