#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"
#include "thread_msg.h"

class lj_drag_vertical_explain: public touch_element, public element
{
public:
	lj_drag_vertical_explain()
	{
		sx = 0;
		sy = 0;
	}

	void doTouchDown()
	{
		Flush();
	}

	void doTouchUp()
	{
		sx -= move_x();
		sy -= move_y();
		if (sy < 0)
		{
			sy = 0;
		}
		if (sy + height > put.GetHeight())
		{
			sy = put.GetHeight() - height;
		}
		Flush();
	}

	void doDelete()
	{
		xml_mgr->DelTouchElement(this);
	}
	void doTouchActive()
	{

	}
	void doRender()
	{
		put.LoadResource();
		int py = sy - move_y();
		if (py < 0)
		{
			py = 0;
		}
		if (py + height > put.GetHeight())
		{
			py = put.GetHeight() - height;
		}
		image::Render(&put, sx - move_x(), py, width, height, 0, 0);

	}

	void doFlushConfig()
	{
		PraseElement();

		printf("x=%d y=%d w=%d h=%d\r\n", x, y, width, height);

		x_lock = 1;

		put.SetResource(m_mp["img"]->getvalue());

		touch_init_area(x, y, width, height);
		xml_mgr->AddEleArea(this);

		Flush();
	}
	int sx;
	int sy;
	image put;
};

#endif //__STATIC_IMAGE_H__
