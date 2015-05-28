// sample3.cpp : LuaTinker �� Ŭ���� ����� �˾ƺ���.
//

#include "hulua.h"

struct A
{
	A(int v) : value(v) {}
	int value;
};

struct base
{
	base() {}

	const char* is_base(){ return "this is base"; }
};

class test : public base
{
public:
	test(int val) : _test(val) {}
	~test() {}

	const char* is_test(){ return "this is test"; }

	void ret_void() {}
	int ret_int()				{ return _test;			}
	int ret_mul(int m) const	{ return _test * m;		}
	A get()						{ return A(_test);		}
	void set(A a)				{ _test = a.value;		}

	int _test;
};

test g_test(11);

int main()
{
	lua_State* L = lua_open();


	luaopen_base(L);
	hulua::open_string(L);

	hulua::class_add<base>(L, "base");
	hulua::class_def<base>(L, "is_base", &base::is_base);

	hulua::class_add<test>(L, "test");
	hulua::class_inh<test, base>(L);
	hulua::class_con<test>(L, hulua::constructor<test,int>);
	hulua::class_def<test>(L, "is_test", &test::is_test);
	hulua::class_def<test>(L, "ret_void", &test::ret_void);
	hulua::class_def<test>(L, "ret_int", &test::ret_int);
	hulua::class_def<test>(L, "ret_mul", &test::ret_mul);
	hulua::class_def<test>(L, "get", &test::get);
	hulua::class_def<test>(L, "set", &test::set);
	hulua::class_mem<test>(L, "_test", &test::_test);
	
	hulua::set(L, "g_test", &g_test);

	hulua::dofile(L, "sample3.lua");

	lua_close(L);

	return 0;
}

