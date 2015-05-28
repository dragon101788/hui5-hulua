// sample5.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include "hulua.h"

void show_error(const char* error)
{
	printf("_ALERT -> %s\n", error);
}

int main( int argc,char *argv[] )
{
	lua_State* L = lua_open();

	luaopen_base(L);


	printf("%s\n","-------------------------- current stack");
	hulua::enum_stack(L);

	lua_pushnumber(L, 1);

	printf("%s\n","-------------------------- stack after push '1'");
	hulua::enum_stack(L);


	hulua::dofile(L, "sample5.lua");

	printf("%s\n","--dragon------------------ calling test_error()");
	hulua::call<void>(L, "test_error");

	printf("%s\n","--dragon--------------------- calling test_error_3()");
	hulua::call<void>(L, "test_error_3");

	hulua::def(L, "_ALERT", show_error);

	hulua::call<void>(L, "_ALERT", "test !!!");

	printf("%s\n","-------------------------- calling test_error()");
	hulua::call<void>(L, "test_error");

	lua_close(L);

	return 0;
}

