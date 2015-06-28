#ifndef __HUI_HULUA_H__
#define __HUI_HULUA_H__
#include "hulib.h"
#include "hulua.h"


class LUA :public thread ,public Mutex
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
              log_i("lua dofile %s\n",m_file.nstr());
              hulua::dofile(L, m_file);
              log_i("lua dofile %s OK\n",m_file.nstr());
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
              log_i("lua dostring %s\n",m_str.nstr());
              hulua::dostring(L, m_str);
              log_i("lua dostring %s OK\n",m_str.nstr());
          }
        };

        typedef SmartPtr<node> SPNode;
        queue< SPNode > q;
	lua_State* L;
	int run();
	int destory();
	int init();

	LUA();
	~LUA();
//	operator lua_State *()
//        {
//	    return L;
//        }
	void dofile(const char * file)
	{
	  q.addele(new node_dofile(file));
	}
	void dostring(const char * str)
        {
	  q.addele(new node_dostring(str));
        }
        #define regele(ele)     add(new element_lua_node(ele))

	void add(node * ele)
	{
	  q.addele(ele);
	}
	static void show_error(const char* error)
	{
	    errexitf("_ALERT -> %s\n", error);
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
