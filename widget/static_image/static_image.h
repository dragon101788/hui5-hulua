#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"

class static_image: public element
{
public:
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
	void doGetInfo(info & info)
	{
		GetElementInfo(info);
		info.AddInfo("id", id);
		info.AddInfo("xpos", xpos);
		info.AddInfo("ypos", ypos);
	}
	void doFlushConfig()
	{
		PraseElement();
		id = m_mp["id"]->getvalue_int();
		xpos = m_mp["xpos"]->getvalue_int();
		ypos = m_mp["ypos"]->getvalue_int();

		for (int i = 0; i < m_mp.count("node"); i++)
		{
			printf("doFlushConfig %d %s\r\n",i, m_mp["node"][i]->getvalue());
			SetRes(i, m_mp["node"][i]->getvalue());
		}

		ParseModifRes();
		Flush();
	}
	void doRender()
	{
		image::Render(&res[id], xpos, ypos, width, height, 0, 0);
	}
	int xpos;
	int ypos;

	int id;
};

#endif //__STATIC_IMAGE_H__
