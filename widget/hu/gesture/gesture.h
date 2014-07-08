#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"
#include "autoconf.h"
#include "xmlproc.h"

inline int point_in_area(int x, int y, int top, int bottom, int left, int right)
{
	//printf("point_in_area %d %d %d %d %d %d\r\n",x,y,top,bottom,left,right);
	if (x > left && x < right && y > top && y < bottom)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


class gesture: public touch_element, public element
{
public:
	void doTouchDown()
	{
		//printf("$$$HU$$$ %s %s tx=%d ty=%d t%d b%d l%d r%d\r\n",name,__FUNCTION__,tx,ty,top,bottom,left,right);

		pot.push_back(point(GetTouchX(), GetTouchY()));
		ax = GetTouchX();
		ay = GetTouchY();
		res[0].DrawBrush(GetTouchX() - x, GetTouchY() - y, 0xffff7705);

		//printf("%s touch\r\n", name.c_str());
		Flush();
	}

	void doTouchUp()
	{

		//printf("%s free\r\n", name.c_str());
		res[0].cleanBuf();
		Flush();
	}

	void doTouchActive()
	{
		printf("%s touch_activate\r\n", name.c_str());
		list<node>::iterator it;
		for (it = nod.begin(); it != nod.end(); ++it)
		{
			it->matching(pot);
		}

		pot.clear();
	}

	void doDelete()
	{
		xml_mgr->DelTouchElement(this);
	}
	void doFlushConfig()
	{
		PraseElement();
		//cmd = mp["cmd"]->getvalue();
		TouchParaseXml(m_mp);
		touch_init_area(x, y, width, height);

		if (res[0].isNULL())
		{
			res[0].SetBuffer(width, height);
			for (int i = 0; i < m_mp.count("node"); i++)
			{
				HUMap mp1 = m_mp["node"][i];
				nod.push_back(node());
				node & tnod = nod.back();
				tnod.parent = this;
				tnod.exec.parse(mp1);
				tnod.mp = m_mp["node"][i];
				for (int i = 0; i < mp1.count("area"); i++)
				{
					int tmp_x = mp1["area"][i]["x"]->getvalue_int();
					int tmp_y = mp1["area"][i]["y"]->getvalue_int();
					int tmp_w = mp1["area"][i]["width"]->getvalue_int();
					int tmp_h = mp1["area"][i]["height"]->getvalue_int();
					printf("gesture::doFlushConfig area %d %d %d %d\r\n", tmp_x,
							tmp_y, tmp_w, tmp_h);
					tnod.are.push_back(area(tmp_x, tmp_y, tmp_w, tmp_h));
				}
			}
		}

		xml_mgr->AddEleArea(this);

		Flush();

	}
	void doRender()
	{
		//printf("Render %s %d \r\n", name.c_str(), isdn);

		if (isdn)
		{
			image::Render(&res[0], 0, 0, width, height, 0, 0);
		}

//		else
//		{
//			roll_back_static_image(x, y, width, height);
//		}

	}

	gesture()
	{
		ax = x;
		ay = y;
	}
	class area
	{
	public:
//		area()
//		{
//			top = 0;
//			bottom = 0;
//			left = 0;
//			right = 0;
//		}
		area(int x, int y, int width, int height)
		{
			top = y;
			left = x;
			bottom = top + height;
			right = left + width;
		}
		int top;
		int bottom;
		int left;
		int right;
	};
	class point
	{
	public:
		point(int _x, int _y)
		{
			x = _x;
			y = _y;
		}

		int x;
		int y;
	};
	class node
	{
	public:
		node()
		{

		}
		int matching(list<point> & pot)
		{
			list<point>::iterator it;
			list<area>::iterator ita = are.begin();
			for (it = pot.begin(); it != pot.end(); ++it)
			{
				if (point_in_area(it->x, it->y, ita->top, ita->bottom,
						ita->left, ita->right))
				{
					printf("gesture::OK!!!!!!!!!!!!\r\n");
					ita++;
					if (ita == are.end())
					{
						printf("gesture::exec!!!!!!!!!!!!\r\n");
						parent->xml_mgr->AddExec(0,exec);
					}
				}
			}
		}
		list<area> are;
		HUMap * mp;
		gesture * parent;
		HuExec exec;
		//hustr cmd;
	};

	list<node> nod;
	list<point> pot;
	int ax;
	int ay;
	//hustr cmd;
};

#endif //__STATIC_IMAGE_H__
