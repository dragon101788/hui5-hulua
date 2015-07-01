#ifndef __BUTTON_IMAGE_H__
#define __BUTTON_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"

class button: public element, public touch_element
{
public:
        static void lua_instal(lua_State* L)
        {
              element_base::lua_instal<button>(L);
              element::lua_instal<button>(L);
              touch_element::lua_instal<button>(L);
        }
	void doTouchDown()
	{
		//printf("$$$HU$$$ %s %s tx=%d ty=%d t%d b%d l%d r%d\r\n",name,__FUNCTION__,tx,ty,top,bottom,left,right);
		if (isdn == 1)
			return;

		//printf("%s touch\r\n", name.c_str());

		Flush();
	}

	void doTouchUp()
	{
		if (isdn == 0)
			return;

		//printf("%s free\r\n", name.c_str());

		Flush();
	}

	void doTouchActive()
	{
		lua.dostring(ldo);
	}


	void doDelete()
	{
		touch_mgr->DelTouchElement(this);
	}

	void doEleLuaCommand(const char * cmd)
	{
	  if(luacmd_is("aaa"))
          {
              printf("aaa [%s] %d\n",cmd,top);
          }
	}
	void doFlushConfig(HUMap &mp)
	{
		ldo = mp["ldo"]->getvalue();
//		touch_init_area(GetX(), GetY(), GetWidth(), GetHeight());

		SetRes("up", mp["up"]->getvalue());
		SetRes("dn", mp["dn"]->getvalue());
		//xml_mgr->AddEleArea(this);

	}
	void doRender()
	{
		image::RenderFrom(GetRes(isdn?"dn":"up"), 0, 0);
	}

	button()
	{
	}

	hustr ldo;
};

#endif //__STATIC_IMAGE_H__
