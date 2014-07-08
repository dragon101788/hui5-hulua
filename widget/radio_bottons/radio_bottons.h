#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"
#include "thread_msg.h"

class Radio_buttons: public element
{
public:
	class node: public element, public touch_element
	{
	public:
		void doTouchDown()
		{
			//printf("$$$HU$$$ %s %s GetTouchX()=%d GetTouchY()=%d t%d b%d l%d r%d\r\n",name,__FUNCTION__,GetTouchX(),GetTouchY(),top,bottom,left,right);
			if (isdn == 1)
				return;

			Flush();
		}

		void doTouchUp()
		{
			if (isdn == 0)
				return;

			Flush();
		}

		void doTouchActive()
		{
			parent->cleanSelect();
			on = 1;
			xml_mgr->AddExec(0,exec);//exec.start();
			Flush();
		}
		node()
		{
			on = 0;
		}
		void doDelete()
		{
			xml_mgr->DelTouchElement(this);
		}

		void doFlushConfig()
		{
			PraseElement();
			exec.parse(m_mp);

			img[0].SetResource(m_mp["up"]->getvalue());
			img[1].SetResource(m_mp["dn"]->getvalue());

			img[0].LoadResource();

			touch_init_area(x, y, width, height);

			xml_mgr->AddEleArea(this);

			Flush();

			//printf("init end\r\n");
		}
		void doRender()
		{
			img[on].LoadResource();
			image::Render(&img[on], 0, 0);

		}
		int on;
		image img[2];
		HuExec exec;
		Radio_buttons * parent;
	};
	void doDelete()
	{
	}
	void doGetInfo(info & info)
	{
		GetElementInfo(info);
		info.AddInfo("select", GetSelect());
	}
	const char * GetSelect()
	{
		int sel = -1;
		for (int i = 0; i < nodemp.size(); i++)
		{
			if (nodemp[i]->on)
			{
				return nodemp[i]->name;
			}
		}
	}
	void SetSelect(const char * name)
	{
		for (int i = 0; i < nodemp.size(); i++)
		{
			if (nodemp[i]->name==name)
			{
				nodemp[i]->on = 1;
			}
			else
			{
				nodemp[i]->on = 0;
			}
		}
	}
	void doFlushConfig()
	{
		//m_mp.display();
		name = m_mp["name"]->getvalue();
		x = m_mp["x"]->getvalue_int();
		y = m_mp["y"]->getvalue_int();

		int touch_lock = m_mp["lock"]->getvalue_int();
		for (int i = 0; i < m_mp.count("node"); i++)
		{
			if (nodemp[i] == NULL)
			{
				nodemp[i] = new node;
				nodemp[i]->m_mp.fetch(m_mp["node"][i]);
				nodemp[i]->parent = this;
				nodemp[i]->xml_mgr = xml_mgr;
				nodemp[i]->mgr = mgr;
			}
			nodemp[i]->FlushConfig();
			nodemp[i]->touch_lock = touch_lock;
		}

		if(m_mp.exist("select"))
		{
			SetSelect(m_mp["select"]->getvalue());
			m_mp.erase("select");
		}


		Flush();

		printf("Radio_buttons OK\r\n");
	}
	void doRender()
	{
	}

	void cleanSelect()
	{
		map<int, node*>::iterator it;
		for (it = nodemp.begin(); it != nodemp.end(); it++)
		{
			if (it->second->on)
			{
				it->second->on = 0;
				it->second->Flush();
			}
		}
	}
	Radio_buttons()
	{
	}
	map<int, node *> nodemp;
};

#endif //__STATIC_IMAGE_H__
