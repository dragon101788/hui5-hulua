#ifndef __ELEMENT_BASE__
#define __ELEMENT_BASE__

#include "hui_hulua.h"

#define ELEMENT_FLAG_DRAWLOGIC              (1<<0)
#define ELEMENT_FLAG_TOUCH                  (1<<1)
#define ELEMENT_FLAG_TIMER                  (1<<2)

class element_base
{
public:
  element_base()
  {
    m_hide = 0;
    m_x = 0;
    m_y = 0;
    m_height = 0;
    m_width = 0;
    m_lay = 0;
    m_flag = 0;
  }
  virtual void SetName(const char * name)
  {
    m_name = name;
  }
  virtual void SetX(int x)
  {
    m_x = x;
  }
  virtual void SetY(int y)
  {
    m_y = y;
  }
  virtual void SetWidth(int width)
  {
    m_width = width;
  }
  virtual void SetHeight(int height)
  {
    m_height = height;
  }

  virtual void SetLay(int lay)
  {
    m_lay = lay;
  }
  virtual void SetHide(int hide)
  {
    m_hide = hide;
  }
  virtual const char * GetName() const
  {
    return m_name.nstr();
  }
  virtual int GetX() const
  {
    return m_x;
  }
  virtual int GetY() const
  {
    return m_y;
  }

  virtual int GetWidth() const
  {
    return m_width;
  }

  virtual int GetHeight() const
  {
    return m_height;
  }

  virtual int GetLay() const
  {
    return m_lay;
  }

  virtual int GetHide() const
  {
    return m_hide;
  }



  void LuaCommand(const char * cmd)
  {
      doLuaCommand(cmd);
  }
  virtual void doLuaCommand(const char * cmd)
  {
      printf("%s virtual doLuaCommand Nothing can be done\n",GetName());
  }
  class element_lua_node :public LUA::node
  {
  public:
    element_base * m_ele;
    element_lua_node(element_base * ele)
    {
       m_ele = ele;
    }
    void DO(lua_State * L)
    {
      //if (hulua::get_type(L, m_ele->name) == hulua::lua_nil);
      if(m_ele->GetName()!=NULL)
      {
        printf("lua create %s object\n", m_ele->GetName());
                hulua::class_add<element_base>(L, "element_base");
                hulua::class_mem<element_base>(L, "x", &element_base::m_x);
                hulua::class_mem<element_base>(L, "y", &element_base::m_y);
                hulua::class_mem<element_base>(L, "width", &element_base::m_width);
                hulua::class_mem<element_base>(L, "height", &element_base::m_height);
                hulua::class_mem<element_base>(L, "lay", &element_base::m_lay);
                hulua::class_mem<element_base>(L, "hide", &element_base::m_hide);
                hulua::class_def<element_base>(L, "command", &element_base::LuaCommand);
                hulua::set(L, m_ele->GetName(), m_ele);
      }
    }
  };
  #define defset_element_int(name,member,mp,def) if (mp.exist(name)){member = mp[name]->getvalue_int();} else{member = def;}
  #define defsetf_element_int(name,fun,mp,def) if (mp.exist(name)){fun( mp[name]->getvalue_int());} else{fun(def);}

  void ParseElement(HUMap &m_mp)
  {
          log_i("$$$HU$$$ ElementPrase [%s] x=%d y=%d width=%d height=%d hide=%d\r\n",
                                    GetName(),GetX(), GetY(), GetWidth(), GetHeight(), GetHeight());
          SetName(m_mp["name"]->getvalue());
          defsetf_element_int("x",SetX,m_mp,0);
          defsetf_element_int("y",SetY,m_mp,0);
          defsetf_element_int("width",SetWidth,m_mp,0);
          defsetf_element_int("height",SetHeight,m_mp,0);
          defsetf_element_int("hide",SetHide,m_mp,0);
          defsetf_element_int("lay",SetLay,m_mp,5);

          lua.regele(this);
  }
  int m_flag;
//private:
  hustr m_name;
  int m_hide;
  int m_x;
  int m_y;
  int m_width;
  int m_height;
  int m_lay;
};

#endif
