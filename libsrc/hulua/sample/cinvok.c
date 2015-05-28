extern "C" {
#include <lua.h>                        //Lua语言解析器
#include <lualib.h>                     //Lua标准库
#include <lauxlib.h>                    //Lua辅助工具
#include <string.h>
#include <stdlib.h>
}

const char *code = "for i=0, 5 do print(\'胡鑫培!\') end";

static int average(lua_State *L) {
	int n = lua_gettop(L);
	double sum = 20;

	int i;
	for (i = 1; i <= n; i++) {
		if (!lua_isnumber(L, i)) {
			lua_pushstring(L, "Incorrect arugument to 'average'");
		}
		sum += lua_tonumber(L, i);
	}
	printf("C progam sum=%f n=%d!!\n", sum, n);
	lua_pushnumber(L, sum / n);
	lua_pushnumber(L, sum);
	return 2;
}

static int dragon_lua(lua_State *L) {
	int n = lua_gettop(L);
	double a = lua_isnumber(L, 1) ? lua_tonumber(L, 1) : 0;
	double b = lua_isnumber(L, 2) ? lua_tonumber(L, 2) : 0;
	double c = lua_isnumber(L, 3) ? lua_tonumber(L, 3) : 0;

	printf("a=%f b=%f c=%f!!\n", a, b, c);
	return 0;
}

struct A {
	int a;
	char str[256];
	struct c {
		int b;
	} c;
};

void test_struct(lua_State * L, struct A arg) {
	lua_getglobal(L, "test_struct");

	lua_newtable(L);
	lua_pushinteger(L, arg.a);
	lua_setfield(L, -2, "a");

	lua_newtable(L);
	lua_pushinteger(L, arg.c.b);
	lua_setfield(L, -2, "b");
	lua_setfield(L, -2, "c");

	lua_pushstring(L, arg.str);
	lua_setfield(L, -2, "str");

	lua_pcall(L, 1, 1, 0);

	printf("lua:%s\n", lua_tostring(L, -1));
}

static void ReadTableFromItem(lua_State * L, const char * lpszTableName,
		const char * lpszTableItem) {
	lua_getglobal(L, lpszTableName);
	lua_pushstring(L, lpszTableItem);
	lua_gettable(L, -2);
	printf("%s.%s=%d\n", lpszTableName, lpszTableItem,
			(int) lua_tonumber(L, -1));
	//lua_pop(L,2);
}

static void ReadTableFromItem2(lua_State * L, const char * lpszTableName,
		const char * str, const char * lpszTableItem) {
	lua_getglobal(L, lpszTableName);
	lua_pushstring(L, str);
	lua_gettable(L, -2);
	lua_pushstring(L, lpszTableItem);
	lua_gettable(L, -2);
	printf("%s->%s.%s=%d\n", lpszTableName, str, lpszTableItem,
			(int) lua_tonumber(L, -1));
	//lua_pop(L,2);
}
static void ReadTableFromIndex2(lua_State * L, const char * lpszTableName,
		const char * str, int index) {
	lua_getglobal(L, lpszTableName);
	lua_pushstring(L, str);
	lua_gettable(L, -2);
	lua_rawgeti(L, -1, index);
	printf("%s[%d]=%d\n", lpszTableName, index, (int) lua_tonumber(L, -1));
	//lua_pop(L,2);
}

static void ReadTableFromIndex(lua_State * L, const char * lpszTableName,
		int index) {
	lua_getglobal(L, lpszTableName);
	lua_rawgeti(L, -1, index);
	printf("%s[%d]=%d\n", lpszTableName, index, (int) lua_tonumber(L, -1));
	//lua_pop(L,2);
}

static void EnumTableItem(lua_State *L, const char * lpszTableName) {
	lua_getglobal(L, lpszTableName);
	int it = lua_gettop(L);
	lua_pushnil(L);
	printf("Enum %s:\n", lpszTableName);
	while (lua_next(L, it)) {
		printf(" %d", (int) lua_tonumber(L, -1));
		lua_pop(L, 1);
	}
	printf("\n");

}

int main() {


	lua_State *s = luaL_newstate();         //建立一个虚拟机
	luaL_openlibs(s);                       //打开Lua附加库

	luaL_dostring(s, code);         	//执行字符串中的源代码
	luaL_dofile(s, "cinvok.lua");

	//调用函数
	lua_getglobal(s, "showinfo");
	lua_pcall(s, 0, 0, 0);

	//调用函数(传参)
	const char * pstr = "胡鑫培如此美好";
	lua_getglobal(s, "showstr");
	lua_pushstring(s, pstr);
	lua_pcall(s, 1, 0, 0);

	//调用函数(返回值)
	lua_getglobal(s, "add");
	lua_pushinteger(s, 2);
	lua_pushinteger(s, 3);
	lua_pcall(s, 2, 1, 0);
	printf("lua add function return val is %d\n", (int) lua_tointeger(s, -1));

	//lua调用C
	lua_register(s, "average", average);
	lua_register(s, "dragon_lua", dragon_lua);
	lua_getglobal(s, "laverage");
	lua_pcall(s, 0, 0, 0);

	//C传递结构体给LUA
	struct A arg;
	arg.a = 10;
	strcpy(arg.str, "你好");
	arg.c.b = 20;
	test_struct(s, arg);

	//
	ReadTableFromItem(s, "ele", "x");
	ReadTableFromItem2(s, "ele", "attribute", "e");
	ReadTableFromIndex(s, "ele", 1);
	ReadTableFromIndex2(s, "ele", "node", 3);
	EnumTableItem(s, "ele");

	printf("**************************");
	ReadTableFromItem(s, "ele", "e");

	lua_close(s);                   //关闭虚拟机
	return 0;
}
