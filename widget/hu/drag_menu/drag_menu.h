#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"
#include "xmlproc.h"

inline int hu_abs(int number)
{
	return (number >= 0 ? number : -number);
}

class drag_menu: public touch_element, public element, public timer_element
{
public:
	class dm_image
	{
	public:
		dm_image()
		{
			dx = 0;
			dy = 0;
		}
		HuExec exec;
		int dx;
		int dy;
	};
	class dm_page: public image
	{
	public:

	};
	int doTimer(int tm)
	{
		//printf("drag_menu::OnTimer %dms cx=%d dx=%d\r\n", tm,cx,dx);
		for (int i = 0; i < rcn && cx > dx; i++)
			if (cx > dx)
				cx--;

		for (int i = 0; i < rcn && cx < dx; i++)
			if (cx < dx)
				cx++;

		Flush();

		if (dx == cx)
		{
			TimerStop();
		}
		else
		{
			TimerSet(tm + rsp);
		}
		return 0;
	}
	void doTouchDown()
	{

		if (dx != cx)
		{
			return;
		}
		dx = cx;
		if (cx - move_x() < sum_w)
		{
			mx = move_x();
		}
		if (select != NULL)
		{
			xml_mgr->PostCS(hustr("index0"));
			select = NULL;
		}
		Flush();
	}

	void doTouchUp()
	{
		//printf("free_area ori.move_x()=%d\r\n", ori.move_x());

		mx = 0;
		if (hu_abs(move_x()) > remax)
		{ //移动页面
			printf("move\r\n");
			if (move_x() > 0)
			{
				printf("--\r\n");
				if (page - 1 >= 0)
					page--;

			}
			else
			{
				printf("++%d\r\n", page);
				printf("sum_w=%d page_w=%d ind_w=%d\r\n", sum_w, page_w, ind_w);
				if (page + 1 < (sum_w + page_w - 1) / page_w)
					page++;
				printf("++OK\r\n", page);
			}

			printf("page = %d\r\n", page);
			for (int i = 0; i < imgs.size(); i++)
			{

				dm_image * obj = &imgs[i];
				if (i >= page * row)
				{
					//printf("page=%d i=%d\r\n",page,i);
					//if (cx - ori.move_x() > 0 && cx - ori.move_x() < sum_w)
					dx = obj->dx;
					TimerSet(0);
					break;
				}

			}
			xml_mgr->PostCS(hustr("page%d", page + 1));
		}
		else if (hu_abs(move_x()) < remax && hu_abs(move_x()) > remin)
		{					//还原页面
			for (int i = 0; i < imgs.size(); i++)
			{

				dm_image * obj = &imgs[i];
				if (i >= page * row)
				{
					//printf("page=%d i=%d\r\n",page,i);
					//if (cx - ori.move_x() > 0 && cx - ori.move_x() < sum_w)
					cx = cx - move_x();
					dx = obj->dx;
					TimerSet(0);
					break;
				}

			}
		}
		else if (hu_abs(move_x()) < remin)
		{					//确定
//			for (int i = 0; i < imgs.size(); i++)
//			{
//
//				dm_image * obj = &imgs[i];
//				//printf("imgs[%d].dx=%d imgs[%d].dx=%d hu_abs(x)+GetTouchX()=%d\r\n",i,imgs[i].dx,i+1,imgs[i+1].dx,hu_abs(x)+GetTouchX());
//				//if (imgs[i].dx < hu_abs(x) + GetTouchX() && imgs[i + 1].dx > hu_abs(x) + GetTouchX())
//
////				if (imgs[i].dx > cx + GetTouchX())
////				{
////					printf("i=%d\r\n", i);
////
////					index = i;
////					xml_mgr->PostCS(hustr("index%d", 1 + ((index - 1) % row)));
////					select = &imgs[i - 1];
////					break;
////				}
//			}
		//printf("%d %d %d\r\n", imgs[i].dx, cx + GetTouchX(), (cx + GetTouchX()) / ind_w);
			index = (cx + GetTouchX()) / ind_w;
			xml_mgr->PostCS(hustr("index%d", index));
			select = &imgs[index];
		}
		Flush();

	}

	void doTouchActive()
	{
		//g_arg.format("drag_menu=%d", index);
		if (select != NULL)
		{
			xml_mgr->AddExec(0,select->exec);
		}
	}
	void doRender()
	{
		if (isdn && hu_abs(move_x()) < remin && cx + (GetTouchX() - x) < sum_w)
		{
			printf("%d %d %d %d\r\n", cx, GetTouchX(), x, sum_w);
			image::Render(&active,
					move_x() + active_x + ((GetTouchX() - x) / ind_w) * ind_w,
					move_y() + active_y);
		}
		image::Render(&output, cx - mx, 0, cx < 0 ? width + cx : width, height,
				0, 0);

		//image::Render(this, 0, 0);
	}

	void doDelete()
	{
		xml_mgr->DelTimerElement(this);
		xml_mgr->DelTouchElement(this);
	}
	void doFlushConfig()
	{

		PraseElement();

		row = m_mp["row"]->getvalue_int();
		col = m_mp["col"]->getvalue_int();
		spa = m_mp["spa"]->getvalue_int();
		rsp = m_mp["rsp"]->getvalue_int();
		rcn = m_mp["rcn"]->getvalue_int();
		remax = m_mp["remax"]->getvalue_int();
		remin = m_mp["remin"]->getvalue_int();

		active.SetResource(m_mp["active"]->getvalue());
		active_x = m_mp["active"]["x"]->getvalue_int();
		active_y = m_mp["active"]["y"]->getvalue_int();

		if (output.isNULL())
		{
			hustr filename("%s.png", name.c_str());
			if (access_Image(filename))
			{
				output.SetResource(filename);
				output.LoadResource();
				HUMap::iterator mp = m_mp["node"];
				for (int i = 0; i < m_mp.count("node"); i++)
				{
					int num = imgs.size();
					int width = mp[i]["width"]->getvalue_int();
					int height = mp[i]["height"]->getvalue_int();

					ind_w = (width + spa) > ind_w ? (width + spa) : ind_w;
					imgs[num].dx = width * i + spa * i;
					if (height > sum_h)
					{
						sum_h = height;
					}
					sum_w += ind_w;

					imgs[num].exec.parse(m_mp["node"][i]); // = mp["node"][i]["var"]->getvalue();
				}
				//sum_w += ind_w;
				page_w = ind_w * row;
			}
			else
			{

				map<int, image> tmp;
				for (int i = 0; i < m_mp.count("node"); i++)
				{
					int num = imgs.size();
					tmp[i].SetResource(m_mp["node"][i]->getvalue());
					tmp[i].LoadResource();

					ind_w = (tmp[i].GetWidth() + spa) > ind_w ?
							(tmp[i].GetWidth() + spa) : ind_w;
					imgs[num].dx = ind_w * i;

					if (tmp[i].GetHeight() > sum_h)
					{
						sum_h = tmp[i].GetHeight();
					}
					sum_w += ind_w;
					imgs[num].exec.parse(m_mp["node"][i]); // = mp["node"][i]["var"]->getvalue();
				}
				//sum_w += ind_w;
				page_w = ind_w * row;
				output.path.format("drag_menu-%s output", name.c_str());
				output.SetBuffer(sum_w, sum_h);

				for (int i = 0; i < imgs.size(); i++)
				{
					//dm_image * obj = &imgs[i];
					output.Render(&tmp[i], imgs[i].dx, 0);
				}

				output.SaveResource(filename);
			}
		}

		TouchParaseXml(m_mp);
		touch_init_area(x, y, width, height);
		xml_mgr->AddEleArea(this);

		xml_mgr->AddTimerElement(this);

		Flush();

	}

	drag_menu()
	{
		row = 0;
		col = 0;
		sum_w = 0;
		sum_h = 0;
		page_w = 0;
		ind_w = 0;
		index = 0;
		cx = 0; //当前x
		mx = 0; //拖动轴mx
		dx = 0; //时间轴x
		spa = 0;
		rsp = 0;
		rcn = 0;
		page = 0;
		page_max = 0;
		remax = 0;
		remin = 0;
	}
	~drag_menu()
	{
		//printf("~drag_menu\r\n");
		imgs.clear();
	}
	map<int, dm_image> imgs;

	int row;
	int col;
	int sum_w;
	int sum_h;
	int page_w;
	int ind_w;
//	int col_height;
	int index;
	//int op;
	int cx; //当前x
	int mx; //拖动轴mx
	int dx; //时间轴x
	int spa;
	int rsp;
	int rcn;
	int page;
	int page_max;
	int remax;
	int remin;
	dm_image * select;
//	const char * cmd;
//	const char * var;
	//RollBack_old rob;
	image output;
	image active;
	int active_x;
	int active_y;
};

#endif //__STATIC_IMAGE_H__
