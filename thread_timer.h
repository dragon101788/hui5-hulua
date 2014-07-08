#ifndef __THREAD_TIMER_H__
#define __THREAD_TIMER_H__

#include "hulib.h"
#include "xmlproc.h"

extern int go;

class timer_thread: public thread, private Mutex
{
public:
	xmlproc * m_xml;
	int run()
	{
		while (go)
		{
			if (m_xml!=NULL)
			{
				lock();
				m_xml->ProcTimer();
				unlock();
			}

			usleep(1000);
		}
		printf("timer thread exit\r\n");

	}
	void SwitchProc(xmlproc * xml)
	{
		lock();
		m_xml = xml;
		unlock();
	}
	timer_thread()
	{
		name = "timer_thread";
		m_xml = NULL;
	}
};

extern timer_thread g_th_timer;

#endif //__THREAD_TIMER_H__
