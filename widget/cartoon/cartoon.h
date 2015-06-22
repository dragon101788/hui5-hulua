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
	        TimerSet(tm + fps_time + m_ntimes[id]);
		if (m_nexec[id].doStart())
		{
			debug("id[%d] Exec\r\n", id);
		}
		else
		{
			id++;
			if (id > max_id)
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
		timer_mgr->DelTimerElement(this);
	}
	void doFlushConfig(HUMap & mp)
	{
		fps_time = mp["fps"]->getvalue_int();
		id = mp["id"]->getvalue_int();
		release = mp["release"]->getvalue_int();

		mp.display();
                for (int i = 0; i < mp.count("node"); i++)
                {

                        HUMap n_mp = mp["node"][i];
                        printf("init node = %d %s\n",i,n_mp.MapValue().nstr());
                        res["node"][i]->SetResource(n_mp->getvalue());
                        m_ntimes[i] = n_mp["ntime"]->getvalue_int();
                        m_nexec[i].parse(n_mp);
                        max_id = i;
                }

	}
	cartoon()
	{
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
		if (res["node"][id]->LoadResource() == 0)
		{
		      //printf("doRender %d %s\n",id,res["node"][id]->path.nstr());
                      RenderFrom(&res["node"][id].value(), 0, 0,GetWidth(),GetHeight(),0,0);

                      if (release)
                      {
                          res["node"][id]->Free();
                      }
		}

	}
	int max_id;
	humap<image> res;
	map<int, int> m_ntimes;
	map<int, HuExec> m_nexec;
	int id;
	int fps_time;
	int release;
};
#endif //__STATIC_IMAGE_H__
