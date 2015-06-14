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
  inline void SetName(const char * name)
  {
    m_name = name;
  }
  inline void SetX(int x)
  {
    m_x = x;
  }
  inline void SetY(int y)
  {
    m_y = y;
  }
  inline void SetWidth(int width)
  {
    m_width = width;
  }
  inline void SetHeight(int height)
  {
    m_height = height;
  }

  inline void SetLay(int lay)
  {
    m_lay = lay;
  }
  inline void SetHide(int hide)
  {
    m_hide = hide;
  }
  inline const char * GetName() const
  {
    return m_name.nstr();
  }
  inline int GetX() const
  {
    return m_x;
  }
  inline int GetY() const
  {
    return m_y;
  }

  inline int GetWidth() const
  {
    return m_width;
  }

  inline int GetHeight() const
  {
    return m_height;
  }

  inline int GetLay() const
  {
    return m_lay;
  }

  inline int GetHide() const
  {
    return m_hide;
  }

  virtual void doLuaCommand(const char * cmd)
  {
      printf("%s virtual doLuaCommand Nothing can be done\n",GetName());
  }
  void LuaCommand(const char * cmd)
  {
      doLuaCommand(cmd);
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
  int m_flag;
private:
  hustr m_name;
  int m_hide;
  int m_x;
  int m_y;
  int m_width;
  int m_height;
  int m_lay;
};

#endif
