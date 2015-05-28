
#include <cstdio>
#include "hulua.h"

int main( int argc,char *argv[] )
{
	hulua::lua_context L;

//	lua_State* L = lua_open();

	luaopen_base(L);

	hulua::table haha(L, "haha");

	haha.set("value", 1);

	haha.set("inside", hulua::table(L));

	hulua::table inside = haha.get<hulua::table>("inside");

	inside.set("value", 2);

	hulua::dofile(L, "sample4.lua");

	const char* test = haha.get<const char*>("test");
	printf("haha.test = %s\n", test);

	hulua::table temp(L);

	temp.set("name", "local table !!");

	hulua::call<void>(L, "print_table", temp);

	hulua::table ret = hulua::call<hulua::table>(L, "return_table", "give me a table !!");
	printf("ret.name =\t%s\n", ret.get<const char*>("name"));

//	lua_close(L);

	return 0;
}

