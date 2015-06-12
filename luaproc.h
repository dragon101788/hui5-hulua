#ifndef __LUA_PROC_H__
#define __LUA_PROC_H__

#include "xmlproc.h"
#include "hui_hulua.h"

typedef void (*lua_create)(const char * name);
map<hustr,lua_create> hui_lua_create_Instance;
int register_Instance(const char * classname, const char * funcname)
{
  printf("register_Instance\n");
  if(hui_lua_create_Instance.find(classname)!=hui_lua_create_Instance.end())
  {
      hui_lua_create_Instance[classname](funcname);
      return 0;
  }
  else
  {
      printf("can't find Instance [%s]\n",classname);
      return -1;
  }
}

class lua_test_page : public element
{
public:
        int x;
        int y;
        lua_test_page()
        {
                xpos = 0;
                ypos = 0;
                id = 0;
        }
        ~lua_test_page()
        {

        }
        static void lua_create(const char * name)
        {
            printf("lua create\n");
             //usage: lua call
           /*
            *       register_Instance("lua_test_page","test");
                    test.x=0;
                    test.y=0;
                    test.width=480;
                    test.height=272;
                    test:LuaFlushConfig();
           */

//            hulua::class_add<lua_test_page>(lua, "lua_test_page");
//            hulua::class_mem<lua_test_page>(lua, "x", &lua_test_page::x);
//            hulua::class_mem<lua_test_page>(lua, "y", &lua_test_page::y);
//            hulua::class_mem<lua_test_page>(lua, "width", &lua_test_page::width);
//            hulua::class_mem<lua_test_page>(lua, "height", &lua_test_page::height);
//            hulua::class_def<lua_test_page>(lua, "LuaFlushConfig", &lua_test_page::LuaFlushConfig);
//
//            hulua::set(lua, name, new lua_test_page);
        }
        void LuaFlushConfig()
        {
                printf("LuaFlushConfig x=%d\n",x);
                printf("LuaFlushConfig y=%d\n",y);
                printf("LuaFlushConfig ypos=%d\n",ypos);
                xml_mgr = g_cur_xml;
                mgr = g_cur_xml;
                SetBuffer(width, height);
                SetRes(0,"./a.png");
                Flush();
        }
        void doDelete()
        {

        }
//        void doGetInfo(info & info)
//        {
//                GetElementInfo(info);
//                info.AddInfo("id", id);
//                info.AddInfo("xpos", xpos);
//                info.AddInfo("ypos", ypos);
//        }
//        void doFlushConfig()
//        {
//                PraseElement();
//                id = m_mp["id"]->getvalue_int();
//                xpos = m_mp["xpos"]->getvalue_int();
//                ypos = m_mp["ypos"]->getvalue_int();
//
//                for (int i = 0; i < m_mp.count("node"); i++)
//                {
//                        printf("doFlushConfig %d %s\r\n",i, m_mp["node"][i]->getvalue());
//                        SetRes(i, m_mp["node"][i]->getvalue());
//                }
//
//                ParseModifRes();
//                Flush();
//        }
        void doRender()
        {
                image::Render(&res[id], xpos, ypos, width, height, 0, 0);
        }
        int xpos;
        int ypos;

        int id;
};


class luaproc: public xmlproc
{
public:
        luaproc(const char * dlfile)
        {
            lua.dofile(dlfile);
        }
        void doLoader()
        {
                UnDoneProc();

                hui_lua_create_Instance["lua_test_page"] = lua_test_page::lua_create;
                lua.register_func("register_Instance",register_Instance);
                int result = lua.call<int>("main");
                printf("call--\n");
                DoneProc();
        }
        ~luaproc()
        {
        }
};

#endif //__LUA_PROC_H__
