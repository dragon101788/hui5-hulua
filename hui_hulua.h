#ifndef __HUI_HULUA_H__
#define __HUI_HULUA_H__
#include "hulib.h"
#include "hulua.h"

class LUA :public thread
{	
public:
        class node
        {
        public:
          node()
          {
              log_d("new node\n");
          }
          ~node()
          {
              log_d("del node\n");
          }
          virtual void DO(lua_State * L)
          {

          }
        };
        class node_dofile :public node
        {
        public:
          hustr m_file;
          node_dofile(const char * file)
          {
            m_file = file;
          }
          void DO(lua_State * L)
          {
              hulua::dofile(L, m_file);
          }
        };
        class node_dostring :public node
        {
        public:
          hustr m_str;
          node_dostring(const char * str)
          {
            m_str = str;
          }
          void DO(lua_State * L)
          {
              hulua::dostring(L, m_str);
          }
        };
        queue< SmartPtr<node> > q;
	lua_State* L;
	int run();
	int destory();
	int init();

	LUA();
	~LUA();
	operator lua_State *()
        {
	    return L;
        }
	void dofile(const char * file)
	{
	  q.addele(new node_dofile(file));
	}
	void dostring(const char * str)
        {
	  q.addele(new node_dostring(str));
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

int lua_command(lua_State * L);
extern LUA lua;

#endif //__HUI_HULUA_H__
