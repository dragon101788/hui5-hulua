#ifndef __THREAD_TOUCH_H__
#define __THREAD_TOUCH_H__

#include "manager_touch.h"
#include "xmlproc.h"

//extern touch_manager g_touch_mgr;
extern int go;

class touch_thread: public thread, private Mutex
{
public:
	xmlproc * m_xml;
	//thread_debug tdbg;
	int run()
	{
		while (go)
		{
			touch_sample samp;
			while (GetTouchSample(&samp))
			{
				//tdbg.start("TOUCH",this,20000);
				lock();
				if (m_xml != NULL)
				{
					m_xml->ProcTouch(&samp);
				}
				unlock();
				//tdbg.end();
			}
			//usleep(1000);
		}
		printf("msg thread exit\r\n");
	}
	void SwitchProc(xmlproc * xml)
	{
		lock();
		m_xml = xml;
		unlock();
	}
	void init()
	{
		int ret = TouchInit();
		if (ret == 0)
		{
			create();
		}
		else
		{
			printf("TouchInit failed !!!\r\n");
		}
	}
	touch_thread()
	{
		name = "touch_thread";
		m_xml = NULL;
	}
};

extern touch_thread g_th_touch;

#endif //__THREAD_TOUCH_H__
