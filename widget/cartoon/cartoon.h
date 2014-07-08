#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"
#include "thread_msg.h"


class cartoon: public element, public timer_element
{
public:

	class ca_image: public image
	{
	public:
		int ntime;
		HuExec exec;
		ca_image()
		{
			ntime = 0;
		}
	};

	virtual int doTimer(int tm)
	{
		//printf("OnTimer time=%d\r\n", fps_time + images[id].ntime);
		TimerSet(tm + fps_time + images[id].ntime);
		if (images[id].exec.have)
		{
			xml_mgr->AddExec(0,images[id].exec);
			debug("id[%d] PostCS\r\n", id);
		}
		else
		{

			id++;
			if (id >= images.size())
			{
				id = 0;
			}
			//debug_timer();
			Flush();
			//debug_timer("flush");
		}

		return 0;

	}
	void doDelete()
	{
		xml_mgr->DelTimerElement(this);
	}
	void doFlushConfig()
	{
		PraseElement();
		fps_time = m_mp["fps"]->getvalue_int();
		id = m_mp["id"]->getvalue_int();
		release = m_mp["release"]->getvalue_int();

		//m_mp.display();
		if (m_mp.exist("autonode"))
		{
			for (int i = 0; i < m_mp.count("autonode"); i++)
			{
				//printf("autonode %d %d\r\n", i, m_mp["autonode"].BrotherCount());
				HUMap & mp = m_mp["autonode"][i];
				hustr fmt = mp["format"]->getvalue();
				hustr cnt = mp["id"]->getvalue();
				int ntime = mp["ntime"]->getvalue_int();

				int min = strtoul(cnt.str_key("-"), NULL, 10);
				int max = strtoul(cnt.str_value("-"), NULL, 10);
				fmt.Replace("$", "%");
				//printf("autonode %s %d-%d %d\r\n", fmt.c_str(), min, max, ntime);
				hustr path;
				for (int i = min; i <= max; i++)
				{
					path.format(fmt.c_str(), i);

					images[i].SetResource(path);
					images[id].ntime = ntime;

					//printf("%i %s\r\n", i, path.c_str());
				}

			}
			m_mp.erase("autonode");
		}
		if (m_mp.exist("node"))
		{
			for (int i = 0; i < m_mp.count("node"); i++)
			{
				HUMap mp = m_mp["node"][i];
				if (mp.exist("id"))
				{
					int id = mp["id"]->getvalue_int();
					//printf("node id=%d\r\n", id);
					images[id].SetResource(mp->getvalue());
					images[id].ntime = mp["ntime"]->getvalue_int();
					images[id].exec.parse(mp);
				}
				else
				{
					int id = images.size();
					//printf("node id=%d\r\n", id);
					images[id].SetResource(mp->getvalue());
					images[id].ntime = mp["ntime"]->getvalue_int();
					images[id].exec.parse(mp);
				}

			}
			m_mp.erase("node");
		}

		TimerParaseXml(m_mp);

		xml_mgr->AddTimerElement(this);

		Flush();

	}
	cartoon()
	{
		x = 0;
		y = 0;
		id = 0;
		release = 0;
		fps_time = 0;
	}
	~cartoon()
	{
	}

	void doRender()
	{
		//rob.roll_back_render();
		if (images[id].LoadResource() == 0)
		{
			//Draw(&images[id], x, y);
			image::Render(&images[id], 0, 0);

			if (release)
			{
				images[id].Free();
			}
		}

	}
	map<int, ca_image> images;
	int id;
	int fps_time;
	int release;
};
#endif //__STATIC_IMAGE_H__
