#ifndef __TIMER_H__
#define __TIMER_H__

#include "hulib.h"
#include "ParaseXML.h"
#include <set>

#define TIMER_CONTINUE 0
#define TIMER_DEL_ELE  9

class timer_manager;
class timer_element: virtual public Mutex
{
public:
	virtual int doTimer(int tm) = 0;
	int Timer(int tm)
	{
		int ret = 0;
		lock();
		if (timer_stop == 0)
			ret = doTimer(tm);
		unlock();
		return ret;
	}
	int TimerSet(int tm)
	{
		m_tm = tm;
		timer_stop = 0;
	}
	virtual int TimerStop()
	{
		timer_stop = 1;
	}
	virtual int TimerStart()
	{
		timer_stop = 0;
	}
	int GetUpTimer();
	timer_element()
	{
		timer_stop = 1;
		m_tm = 0;
	}
	virtual ~timer_element()
	{

	}

	void TimerParaseXml(HUMap & mp)
	{
		timer_stop = mp["stop"]->getvalue_int();
		//printf("TimerParaseXml lock=%d\r\n", timer_stop);
	}
	//pMutex mutex;
	int m_tm;
	int timer_stop;
	timer_manager * timer_mgr;

};

class timer_manager: virtual public Mutex
{
public:
	void UpdateTimer()
	{
		int cur = GetUpTimer();

		lock();
		iterator it;
		for (it = mp.begin(); it != mp.end();)
		{
			//printf("UpdateTimer %dms %dms\r\n", cur, (*it)->m_tm);
			if (cur > (*it)->m_tm && (*it)->timer_stop==0)
			{
				int ret = (*it)->Timer(cur);
				if (ret == TIMER_DEL_ELE)
				{
					delete *it;
					mp.erase(it++);
					continue;
				}

			}
			++it;

		}
		unlock();
	}
	void AddTimerElement(timer_element * ele)
	{
		lock();
		//mp[name] = ele;
		ele->timer_mgr = this;
		mp.insert(ele);
		unlock();
	}
	void DelTimerElement(timer_element * ele)
	{
		lock();
		mp.erase(ele);
		unlock();
	}
	void ClearElement()
	{
		lock();
		mp.clear();
		unlock();
	}
	void StopAll()
	{
		lock();
		iterator it;
		for (it = mp.begin(); it != mp.end(); ++it)
		{
			//printf("current = %dms %dms\r\n",cur,mp[i]->m_tm);
			(*it)->TimerStop();
		}
		unlock();
	}
	void StartAll()
	{
		lock();
		iterator it;
		for (it = mp.begin(); it != mp.end(); ++it)
		{
			//printf("current = %dms %dms\r\n",cur,mp[i]->m_tm);
			(*it)->TimerStart();
		}
		unlock();
	}
	inline int GetUpTimer()
	{
		return GetTickCount() - t;
	}
	timer_manager()
	{
		t = GetTickCount();
	}
	virtual ~timer_manager()
	{

	}
	int t;
	typedef set<timer_element *>::iterator iterator;
	set<timer_element *> mp;
	//map<hustr, timer_element *> mp;
}
;

#endif //__TIMER_H__
