#include <stdio.h>
#include <stdlib.h>
#include "XMLInstal.h"
#include "XMLInstal.h"
#include "layer.h"
#include "thread_msg.h"

class lj_drag_image: public element, public touch_element
{
public:
	class dg_image: public image
	{
	public:
		dg_image()
		{
			pos1 = 0;
			pos2 = 0;
		}
		void init(const char * path)
		{
			if (this->path != path)
			{
				destroy();
				this->path = path;
				image tmp;
				tmp.SetResource(path);
				tmp.LoadResource();
				SetBuffer(tmp.GetWidth(), tmp.GetHeight() * 3);

				pos1 = tmp.GetHeight() * 1;
				pos2 = tmp.GetHeight() * 2;
				AreaCopy(&tmp, 0, 0, tmp.GetWidth(), tmp.GetHeight(), 0, 0);
				AreaCopy(&tmp, 0, 0, tmp.GetWidth(), tmp.GetHeight(), 0, pos1);
				AreaCopy(&tmp, 0, 0, tmp.GetWidth(), tmp.GetHeight(), 0, pos2);
			}
		}
		int pos1;
		int pos2;
	};
	lj_drag_image()
	{
		posx = 0;
		posy = 0;
	}
	void doDelete()
	{
		xml_mgr->DelTouchElement(this);
	}
	void doGetInfo(info & info)
	{
		GetElementInfo(info);
		info.AddInfo("index", index);
		info.AddInfo("posx", posx);
		info.AddInfo("posy", posy);
		info.AddInfo("bright_ture_x", bright_ture_x);
		info.AddInfo("bright_ture_y", bright_ture_y);
		info.AddInfo("bright_ture_w", bright_ture_w);
		info.AddInfo("bright_ture_h", bright_ture_h);
		info.AddInfo("node_w", node_w);
		info.AddInfo("node_h", node_h);
		info.AddInfo("horizontal", horizontal);
		info.AddInfo("vertical", vertical);
	}
	void doFlushConfig()
	{
		PraseElement();

		node_h = m_mp["node_h"]->getvalue_int();
		node_w = m_mp["node_w"]->getvalue_int();
		horizontal = m_mp["horizontal"]->getvalue_int();
		vertical = m_mp["vertical"]->getvalue_int();
		index = m_mp["index"]->getvalue_int();
		TouchParaseXml(m_mp);
		touch_init_area(x, y, width, height);

		dark.init(m_mp["dark"]->getvalue());
		bright.init(m_mp["bright"]->getvalue());
		if (dark.GetHeight() != bright.GetHeight() || dark.GetWidth() != bright.GetWidth())
		{
			huErrExit("drag_image dark an bright Size is not the same\r\n");
		}

		posy = index * node_h + dark.pos1;
			printf("F to posy %d!!!!!!!!!!!!!!!!!!!!\r\n",posy);
			
		bright_ture_x = m_mp["bright_ture"]["x"]->getvalue_int();
		bright_ture_y = m_mp["bright_ture"]["y"]->getvalue_int();
		bright_ture_w = m_mp["bright_ture"]["width"]->getvalue_int();
		bright_ture_h = m_mp["bright_ture"]["height"]->getvalue_int();
		printf("$$$HU$$$ %d %d %d %d\r\n", bright_ture_x, bright_ture_y, bright_ture_w, bright_ture_h);

		xml_mgr->AddEleArea(this);

		//exec.setvar(name, hustr("%d", index));

		Flush();

	}
	void doTouchDown()
	{
		Flush();

	}

	void doTouchUp()
	{
		posy -= move_y();
		if (posy < dark.pos1)
		{
			posy = dark.pos2 - (dark.pos1 - posy);
			//printf("F to pos1 %d!!!!!\r\n",posy);
		}
		if (posy > dark.pos2)
		{
			posy = dark.pos1 + (posy - dark.pos2);
			//printf("F to pos2 %d!!!!!\r\n",posy);
		}
		posy = (posy + node_h / 2) / node_h * node_h;
		index = (posy - dark.pos1) / node_h;
		printf("index=%d\r\n", index);

		Flush();
	}

	void doTouchActive()
	{

	}
	void doRender()
	{
		printf("move_y=%d\r\n", move_y());
		if (vertical > 2)
		image::Render(&dark, 0, posy - move_y() - bright_ture_y, width, vertical / 2 * node_h, 0, 0);
		image::Render(&bright, bright_ture_x, posy - move_y(), bright_ture_w, bright_ture_h, bright_ture_x, bright_ture_y);
		image::Render(&dark, 0, posy - move_y() + bright_ture_h, width, vertical / 2 * node_h, bright_ture_x,
				bright_ture_y + bright_ture_h);
	}
	dg_image dark;
	dg_image bright;
	HuExec exec;
	int bright_ture_x;
	int bright_ture_y;
	int bright_ture_w;
	int bright_ture_h;

	int posx;
	int posy;

	int node_w;
	int node_h;
	int horizontal;
	int vertical;
	int index;
	//RollBack_old rob;
};

static InstallXMLinstan a("lj_drag_image", Install_Element<lj_drag_image>);
//InstallXMLinstanWidget("test_drag_image",test_drag_image);

extern "C" int init(HUMap & mp, void * dat)
{
	printf("luojie init lj_drag_image\r\n");
}
