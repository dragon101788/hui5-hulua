#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"


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

	int doTimer(int tm)
	{
		//printf("OnTimer time=%d\r\n", fps_time + images[id].ntime);
		TimerSet(tm + fps_time + images[id].ntime);
		if (images[id].exec.doStart())
		{
			debug("id[%d] Exec\r\n", id);
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
	void doFlushConfig(HUMap & mp)
	{
		fps_time = mp["fps"]->getvalue_int();
		id = mp["id"]->getvalue_int();
		release = mp["release"]->getvalue_int();


                for (int i = 0; i < mp.count("node"); i++)
                {
                        HUMap n_mp = mp["node"][i];
                        if (n_mp.exist("id"))
                        {
                                int id = n_mp["id"]->getvalue_int();
                                //printf("node id=%d\r\n", id);
                                images[id].SetResource(n_mp->getvalue());
                                images[id].ntime = n_mp["ntime"]->getvalue_int();
                                images[id].exec.parse(n_mp);
                        }
                        else
                        {
                                int id = images.size();
                                //printf("node id=%d\r\n", id);
                                images[id].SetResource(n_mp->getvalue());
                                images[id].ntime = n_mp["ntime"]->getvalue_int();
                                images[id].exec.parse(n_mp);
                        }

                }

		TimerParaseXml(mp);

		xml_mgr->AddTimerElement(this);


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
