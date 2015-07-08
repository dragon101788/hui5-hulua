#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "drawlogic.h"


class cartoon: public element, public timer_element
{
public:
        static void lua_instal(lua_State* L)
        {
              element_base::lua_instal<cartoon>(L);
              element::lua_instal<cartoon>(L);
        }

	int doTimer(int tm)
	{
		//printf("OnTimer time=%d\r\n", fps_time + images[id].ntime);
	        TimerSet(tm + fps_time + m_ntimes[id]);
		if (!m_nexec[id].empty())
		{
		        lua.dostring(m_nexec[id]);
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
	void doEleLuaCommand(const char * cmd)
	{
              if(luacmd_is("stop"))
              {
                  printf("stop\n");
                  timer_stop = 1;
              }
              else if(luacmd_is("start"))
              {
                  printf("start\n");
                  timer_stop = 0;
              }
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
                        m_nexec[i] = n_mp["ldo"]->getvalue();
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

	map<int, int> m_ntimes;
	map<int, hustr> m_nexec;
	int id;
	int fps_time;
	int release;
};
#endif //__STATIC_IMAGE_H__
