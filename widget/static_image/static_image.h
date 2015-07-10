#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "drawlogic.h"

class static_image: public element
{
public:
        static void lua_instal(lua_State* L)
        {
              element_base::lua_instal<static_image>(L);
              element::lua_instal<static_image>(L);
        }

	static_image()
	{
		xpos = 0;
		ypos = 0;
		id = 0;
	}
	~static_image()
	{

	}
	void LuaCommand(const char * cmd,const char * arg)
	{

	}
	void doDelete()
	{

	}
//	void doGetInfo(info & info)
//	{
//		GetElementInfo(info);
//		info.AddInfo("id", id);
//		info.AddInfo("xpos", xpos);
//		info.AddInfo("ypos", ypos);
//	}
	void doFlushConfig(HUMap & mp)
	{
		id = mp["id"]->getvalue_int();
		xpos = mp["xpos"]->getvalue_int();
		ypos = mp["ypos"]->getvalue_int();

		//mp.display();
		for (int i = 0; i < mp.count("node"); i++)
		{
			printf("doFlushConfig %d %s\r\n",i, mp["node"][i]->getvalue());
			SetRes("node", mp["node"][i]->getvalue());
		}

	}
	void doRender()
	{
		RenderToSelf(GetRes("node",id), xpos, ypos, GetWidth(), GetHeight(), 0, 0,0);
	}
	int xpos;
	int ypos;

	int id;
};

#endif //__STATIC_IMAGE_H__
