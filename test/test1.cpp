#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hulib.h"


template<typename T>
class HUTimer: public thread, public Mutex
{
	int t;
public:
	typedef int (*timer_fun_ft)(int tm, T data);
	class timer_node
	{
	public:
		int m_tm;
		timer_fun_ft m_fun;
		T m_data;
	};
	typedef typename map<hustr, timer_node>::iterator iterator;
	map<hustr, timer_node> mp;

	HUTimer()
	{
		name = "timer_thread";
		t = GetTickCount();
		create();
	}
	unsigned long GetTickCount()
	{
		struct timespec ts;

		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
	}
	inline int GetUpTimer()
	{
		return GetTickCount() - t;
	}
	int run()
	{
		while (1)
		{
			lock();
			int cur = GetUpTimer();
			iterator it;
			for (it = mp.begin(); it != mp.end();)
			{
				if (cur > it->second.m_tm)
				{
					timer_fun_ft fun = it->second.m_fun;
					void * data = it->second.m_data;
					mp.erase(it++);

					fun(cur,data);
					continue;
				}
				++it;

			}
			unlock();

			usleep(1000);
		}
	}
	void add(const char * name, int tm, timer_fun_ft fun, void * data)
	{
		lock();
		mp[name].m_tm = tm;
		mp[name].m_fun = fun;
		mp[name].m_data = data;
		unlock();
	}
};

HUTimer<void *> g_th_timer;

int test1(int tm, void * data)
{
	printf("test1 in\r\n");
	//g_th_timer.add("test", tm+1000, test1, NULL);
}

int test2(int tm, void * data)
{
	printf("test2 in\r\n");
	sleep(1);
	//g_th_timer.add("test", tm+1000, test1, NULL);
}

int test3(int tm, void * data)
{
	printf("test3 in\r\n");
	//g_th_timer.add("test", tm+1000, test1, NULL);
}

int test4(int tm, void * data)
{
	printf("test4 in\r\n");
	//g_th_timer.add("test", tm+1000, test1, NULL);
}

int main()
{
	g_th_timer.add("tes1", 100, test1, NULL);
	g_th_timer.add("tes2", 200, test2, NULL);
	g_th_timer.add("tes3", 300, test3, NULL);
	g_th_timer.add("tes4", 400, test4, NULL);
	sleep(10000);
}
