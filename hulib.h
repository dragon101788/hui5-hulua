#ifndef __HULIB_HU__
#define __HULIB_HU__
#include <unistd.h>
#include <stdarg.h>
#include <string>
#include <pthread.h>
#include <map>
#include <vector>
#include <algorithm>
#include <list>
#include <sys/sem.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/prctl.h>
#include <semaphore.h>
#include <deque>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
using namespace std;

unsigned long GetTickCount();
void debug_timer(const char * str = NULL);
void FPSWaitTimer(int ms);
#define FPSWaitFPS(fps) FPSWaitTimer(1000/fps)

//static inline void errexit(const char * str)
//{
//	printf("**************error exit*****************\r\n");
//	printf("%s\r\n",str);
//	printf("*****************************************\r\n");
//	exit(-1);
//}
void errexit(const char * str);
#define errexitf(str,...) errexit(hustr(str, ##__VA_ARGS__ ))

void debug_usage();
void dumpstack();
int system_noblk(const char *cmdstring);
//#define debug(a,...)	printf(a,##__VA_ARGS__)
//#define debug(a,...)

class hustr: public string
{
public:
	hustr()
	{

	}

//	hustr(const char * str)
//	{
//		assign(str);
//	}
	void Replace(const char * rst, const char * dst)
	{
		replace(find(rst), strlen(rst), dst, strlen(dst));
	}

	hustr str_key(const char * str = "=")
	{
		int len = length() + 1;
		char * tmp = (char *) alloca(len);
		memset(tmp, 0, len);
		strcpy(tmp, c_str());
		char * p = strstr(tmp, str);
		*p = 0;
		char * key = tmp;
		char * value = p + 1;
		return key;
	}
	hustr str_value(const char * str = "=")
	{
		int len = length() + 1;
		char * tmp = (char *) alloca(len);
		memset(tmp, 0, len);
		strcpy(tmp, c_str());
		char * p = strstr(tmp, str);
		*p = 0;
		char * key = tmp;
		char * value = p + 1;
		return value;
	}

	hustr(const char * fmt, ...)
	{

		if (fmt == NULL)
		{
			return;
		}
		char buffer[strlen(fmt)*2];
		va_list argptr;
		int cnt;

		va_start(argptr, fmt);
		cnt = vsprintf(buffer, fmt, argptr);
		va_end(argptr);

		assign(buffer);

	}

	int format(const char * fmt, ...)
	{

		if (fmt == NULL)
		{
			errexitf("hustr format error\r\n");
		}
		char buffer[strlen(fmt)*2];
		va_list argptr;
		int cnt;
		va_start(argptr, fmt);
		cnt = vsprintf(buffer, fmt, argptr);
		va_end(argptr);

		assign(buffer);

		return (cnt);
	}

	const char * nstr() const
	{
		if (empty())
		{
			return NULL;
		}
		else
			return c_str();
	}
	operator const char *()
	{
		return c_str();
	}

};

template<typename T>
class SmartPtr //����ָ��
{
private:
	//T *ptr;
	unsigned int *pUse;
public:
	T *ptr;
	SmartPtr(T *p = NULL) :
			ptr(p), pUse(new unsigned int(1)) //���캯��
	{
	}
	SmartPtr(const SmartPtr& src) :
			ptr(src.ptr), pUse(src.pUse) //�������캯��
	{

		++*pUse;
	}
	SmartPtr& operator=(const SmartPtr& rhs) //��ֵ
	{
		// self-assigning is also right
		++*rhs.pUse;
		DestroyUse(); //����Լ���ԭ��ָ��
		ptr = rhs.ptr;
		pUse = rhs.pUse;

		return *this;
	}
	T *operator->()
	{
		if (!ptr)
		{
			errexitf("SmartPtr access NULL ptr\r\n");
		}
		return ptr;
	}

//	T* operator&()
//	{
//		if (!ptr)
//		{
//			errexit("SmartPtr access NULL ptr\r\n");
//		}
//		return ptr;
//	}
//	const T* operator&() const
//	{
//		if (!ptr)
//		{
//			errexit("SmartPtr access NULL ptr\r\n");
//		}
//		return ptr;
//	}
	const T *operator->() const
	{
		if (!ptr)
		{
			errexitf("SmartPtr access NULL ptr\r\n");
		}
		return ptr;
	}
	T &operator*()
	{
		if (!ptr)
		{
			errexitf("SmartPtr access NULL ptr\r\n");
		}
		return *ptr;
	}
	const T &operator*() const
	{
		if (!ptr)
		{
			errexitf("SmartPtr access NULL ptr\r\n");
		}
		return *ptr;
	}
	operator T()
	{
		if (!ptr)
		{
			errexitf("SmartPtr access NULL ptr\r\n");
		}
		return *ptr;
	}
	operator T*()
	{
		if (!ptr)
		{
			errexitf("SmartPtr access NULL ptr\r\n");
		}
		return ptr;
	}
	operator const T*() const
	{
		if (!ptr)
		{
			errexitf("SmartPtr access NULL ptr\r\n");
		}
		return ptr;
	}

	bool isNULL()
	{
		if (ptr == NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename rT>
	rT Switch()
	{
		return (rT) ptr;
	}

	~SmartPtr()
	{
		DestroyUse();
		//printf("$$$HU$$$ Auto Ptr ~SmartPtr\n");

	}
	void Release()
	{
		DestroyUse();
		ptr = NULL;
		pUse = new unsigned int(1);
	}

private:
	void DestroyUse()
	{
		if (--*pUse == 0)
		{
			if (ptr != NULL)
				debug("$$$HU$$$ Auto Ptr DestroyUse Release content\n");

			delete ptr;
			delete pUse;
			ptr = NULL;
			pUse = NULL;

		}
	}

};

class Mutex
{
public:
	void lock()
	{
		pthread_mutex_lock(&mutex);
	}
	int timed_lock(int usec)
	{
		while (try_lock() != 0)
		{
			usleep(1000);
			usec -= 1000;
			//printf("timed_lock %d\r\n",usec);
			if (usec < 0)
			{
				return -1;
			}
		}
		return 0;
	}
	int try_lock()
	{
		pthread_mutex_trylock(&mutex);
	}
	void unlock()
	{
		pthread_mutex_unlock(&mutex);
	}

	Mutex(int flag = PTHREAD_MUTEX_RECURSIVE_NP)
	{
		pthread_mutexattr_init(&mattr);
		pthread_mutexattr_settype(&mattr, flag);
		pthread_mutex_init(&mutex, &mattr);
	}
	~Mutex()
	{
		//printf("~Mutex\r\n");
		pthread_mutex_destroy(&mutex);
		pthread_mutexattr_destroy(&mattr);
	}
private:
	pthread_mutex_t mutex;
	pthread_mutexattr_t mattr;
};

class pMutex
{
public:

	pMutex()
	{
		mutex = NULL;
	}
	void SetMutex(Mutex * mut)
	{
		mutex = mut;
	}
	void lock()
	{
		if (mutex != NULL)
		{
			mutex->lock();
		}
	}
	int try_lock()
	{
		if (mutex != NULL)
		{
			return mutex->try_lock();
		}
	}
	void unlock()
	{
		if (mutex != NULL)
		{
			mutex->unlock();
		}
	}
	Mutex * mutex;
};

class thread
{
public:
	virtual int run()
	{
		printf("call base thread run\r\n");
	}

	virtual int destory()
	{
		printf("call base thread destory\r\n");
	}
	static void cleanup(void * arg)
	{
		thread * is = (thread *) arg;
		printf("[%s] thread asynchronous interrupt\r\n", is->name.c_str());
		is->destory();
	}
	static void * create_thread(void *arg)
	{
		thread * is = (thread *) arg;

		prctl(PR_SET_NAME, is->name.c_str());
		pthread_cleanup_push(cleanup,is)
			;
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

			is->run();

			pthread_cleanup_pop(0);
		pthread_detach(pthread_self());
		pthread_exit(NULL);
	}
	void create()
	{

		int err = pthread_create(&tid, NULL, create_thread, this);
		if (err != 0)
		{
			errexitf("can't create thread: %s\n", strerror(err));
		}

	}
	void wait()
	{
		if (tid != -1)
		{
			pthread_join(tid, NULL);
		}

	}
	void cancel()
	{
		if (tid != -1)
		{
			if (!pthread_cancel(tid))
			{
				printf("pthread_cancel %s OK\n ", name.c_str());
				wait();
				tid = -1;
			}
			else
			{
				printf("pthread_cancel %s faild: %s\n ", name.c_str(), strerror(errno));
			}
		}
	}
	void kill(int sig)
	{
		pthread_kill(tid, sig);
	}
	thread()
	{
		tid = -1;
	}
	virtual ~thread()
	{
		cancel();
	}
	pthread_t tid;
	hustr name;
};

class thread_debug: public thread
{
public:
	int timer;
	int m_max;
	hustr m_name;
	thread * m_thd;
	int run()
	{
		while (1)
		{
			if (GetTickCount() > timer + m_max)
			{
				printf("%s thread Timer Out\r\n", m_name.nstr());
				m_thd->kill(11);
				return -1;
			}
			usleep(1000);
		}
		return 0;
	}
	int start(const char * name, thread * thd, int max)
	{
		timer = GetTickCount();
		m_max = max;
		m_name = name;
		m_thd = thd;
		create();
	}
	int end()
	{
		cancel();
	}
};
class huvar
{
public:

	void init_arg(int argc, char * argv[])
	{
		for (int i = 0; i < argc; i++)
		{
			char * p = strchr(argv[i], '=');
			if (p != NULL)
			{
				*p = 0;
				mp[argv[i]] = p + 1;
			}
			else
			{
				mp[argv[i]] = "1";
			}
		}
	}

	const char * getvar(const char * str)
	{

		map<hustr, hustr>::iterator it;
		it = mp.find(str);
		if (it != mp.end())
		{
			return it->second.c_str();
		}
		else
		{
			return NULL;
		}

	}
	int getvar_int(const char * str)
	{
		const char * p = getvar(str);
		if (p != NULL)
		{
			return strtol(p, NULL, 10);
		}
		return 0;
	}

	map<hustr, hustr> mp;
};

extern huvar g_var;

//class HuDebug
//{
//public:
//	int checkout(const char * str)
//	{
//		if (str[0] == '<')
//		{
//			char tmp[32] =
//			{ 0 };
//			sscanf(str, "<%[^>]", tmp);
//			if (mp[tmp] == 0)
//			{
//				return 1;
//			}
//			else
//			{
//				return 0;
//			}
//		}
//		return 1;
//	}
//	void output(const char * fmt, ...)
//	{
//		if (checkout(fmt))
//			return;
//
//		if (fmt == NULL)
//		{
//			return;
//		}
//		va_list argptr;
//		int cnt;
//
//		va_start(argptr, fmt);
//		cnt = vprintf(fmt, argptr);
//		va_end(argptr);
//	}
//	void fps(const char * str)
//	{
//		if (checkout(str))
//			return;
//		debug_timer(str);
//	}
//	int & operator[](const char * str)
//	{
//		return mp[str];
//	}
//	map<hustr, int> mp;
//};
//extern HuDebug debug;

class Sem
{
public:
	Sem()
	{
		sem_init(&m_sem, 0, 0);
	}
	int post()
	{
		sem_post(&m_sem);
	}
	int wait()
	{
		sem_wait(&m_sem);
	}
	int no_wait()
	{
		return sem_trywait(&m_sem);
	}
	int wait_timerout(int msecs)
	{
		struct timespec ts;
		int tm = GetTickCount() + msecs * 1000;
		ts.tv_sec = tm / (1000 * 1000 * 1000);
		ts.tv_nsec = tm % (1000 * 1000 * 1000);
		//clock_gettime(CLOCK_MONOTONIC, &ts);

		//ts.tv_nsec += msecs*1000;

		return sem_timedwait(&m_sem, &ts);
		//ETIMEOUT 超时
	}
	sem_t m_sem;
};

template<typename T>
class queue: public Mutex, public Sem
{
public:
        typedef typename list< T >::iterator  iterator;
        iterator begin()
        {
          return m_list.begin();
        }
        iterator end()
        {
          return m_list.end();
        }
	queue()
	{
	}
	void clean()
	{
		lock();
		m_list.clear();
		unlock();
	}
	void addele(T ele)
	{
		lock();
		m_list.push_back(ele);
		post();
		unlock();
	}
	T getele()
	{
		wait();
		lock();
		T ret = m_list.front();
		m_list.pop_front();
		unlock();
		return ret;
	}
	T getele_nowait()
	{
		if (no_wait() != 0)
		{
			if (errno == EAGAIN)
			{
				return NULL;
			}
			else
			{
				errexitf("getele_nowait %d\r\n", errno);
			}
		}
		lock();
		T ret = m_list.front();
		m_list.pop_front();
		unlock();
		return ret;
	}
	T getele_timerout(int msecs)
	{
		if (wait_timerout(msecs) != 0)
		{
			if (errno == ETIMEDOUT)
			{
				return NULL;
			}
			else
			{
				errexitf("getele_timerout %d\r\n", errno);
			}
		}
		lock();
		T ret = m_list.front();
		m_list.pop_front();
		unlock();
		return ret;
	}
	void delele(T ele)
	{
		lock();
		m_list.remove(ele);
		unlock();
	}
	int empty()
	{
		int ret;
		lock();
		m_list.empty();
		unlock();
		return ret;
	}
	list<T> m_list;
};

class DebugTimer
{
	int oldtime;
public:
	DebugTimer()
	{
		oldtime = GetTickCount();

	}
	void debug_timer(const char * str)
	{
		int usetime = GetTickCount() - oldtime;
		if (str != NULL)
			debug("debug_timer [%s] %dfps/S fps/%dms\r\n", str,
					usetime ? 1000 / usetime : 1000, usetime);
		oldtime = GetTickCount();
	}
};



template<typename T>
class humap: public multimap<hustr, humap<T> >
{
public:
	humap(const char * str = NULL, humap * f = NULL, int o = 0)
	{
		m_order = o;
		m_father = f;
		m_key = str;
	}
	typedef typename multimap<hustr, humap>::iterator iterator_base;
	hustr m_key;
	T m_val;
	humap * m_father;
	int m_order;



	class iterator: public iterator_base
	{
	public:
		//iterator m_it;
		humap * m_mgr;
		iterator()
		{
			//(*this) = (*it);
			m_mgr = NULL;
			//m_it = it;
		}
		iterator(iterator_base it) :
				iterator_base(it)
		{
			//(*this) = (*it);
			m_mgr = NULL;
			//m_it = it;
		}
		iterator(iterator_base it, humap * mgr) :
				iterator_base(it)
		{
			//(*this) = (*it);
			m_mgr = mgr;
			//m_it = it;
		}
		hustr & operator=(const char * key)
		{
			(**this).second.m_val = key;
			//return m_val = key;
		}
		iterator operator[](const char * key)
		{
			return (**this).second.Get(key);
		}
		iterator operator[](int id)
		{
			return m_mgr->Get((**this).first.c_str(), id);
		}
		const hustr & key()
		{
			return (**this).first;
		}
		T & value()
		{
			return (**this).second.m_val;
		}
		operator humap &()
		{
			return (**this).second;
		}
		operator humap *()
		{
			return &(**this).second;
		}
		T * operator->()
		{
			return &(**this).second.m_val;
		}
	};

	class OrderList:public vector<humap>
	{
	public:
		typedef typename vector<humap>::iterator iterator;
		static int cmp(const humap & a, const humap & b)
		{
			return a.m_order < b.m_order;
		}
		void display()
		{
			for (iterator it = this->begin(); it != this->end(); it++)
			{
				//printf("%s %s\r\n", (*it).m_key.c_str(), (*it).m_val.c_str());
				(*it).display();
			}
		}
		void accept(humap & mp)
		{
			for (typename humap::iterator it = mp.begin(); it != mp.end(); it++)
			{
				this->push_back((*it).second);
			}
			sort(this->begin(), this->end(), cmp);
		}
		OrderList()
		{

		}
	};
	int exist(const char * key)
	{
		if (this->find(key) != this->end())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	void fetch(humap & mp)
	{
		*this = mp;
	}
	T * operator->()
	{
		return &m_val;
	}
	iterator operator[](const char * key)
	{
		return Get(key);
	}
	iterator CreateMultiLast(const char * key)
	{
		//printf("CreateMultiLast [%s]\r\n", key);
		return iterator(this->insert(pair<hustr, humap>(key, humap(key, this, this->size()))), this);
	}
	iterator Get(const char * key)
	{
		iterator it = this->find(key);
		if (it != this->end())
		{
			return iterator(it, this);
		}
		else
		{
			return iterator(this->insert(pair<hustr, humap>(key, humap(key, this, this->size()))), this);
		}
	}
	iterator Get(const char * key, int id)
	{

		iterator it = this->find(key);
		if (it != this->end())
		{
			int count = this->count(key);
			//printf("count=%d id=%d\r\n", count, id);
			int i;
			for (i = 0; i < count; ++i)
			{
				if (i == id)
				{
					//printf("find %s %d\r\n", key, i);
					return iterator(it, this);
				}
				++it;
			}
			if (i >= count)
			{
				printf("key %s id %d not find\r\n", key, i);
				exit(-1);
			}
		}
		else
		{
			return iterator(this->insert(pair<hustr, humap>(key, humap(key, this, this->size()))), this);
		}
	}
	void display()
	{
		displayspace(retrospect_tree());
		printf("[%s]=[%s]  <%d>\r\n", m_key.c_str(), m_val.c_str(),m_order);
		iterator it;
		for (it = this->begin(); it != this->end(); ++it)	//遍历
		{
			(*it).second.display();
		}
	}

	int retrospect_tree()
	{
		int i = 0;
		humap * p = this;
		while (p->m_father != NULL)
		{
			i++;
			p = p->m_father;
		}
		return i;
	}

	void displayspace(int n, const char * str = "     ")
	{
		for (int i = 0; i < n; i++)
			printf("%s", str);
	}
	void setfather(humap * father)
	{
		m_father = father;
		iterator it;
		for (it = this->begin(); it != this->end(); ++it)
		{
			it->second.setfather(this);
		}

	}

	humap & getfather()
        {
	    if(m_father != NULL)
            {
	         return *m_father;
            }
	    else
            {
	        fprintf(stderr,"$$$dragon$$$ can't get father,return this");
	        return *this;
            }
        }
};
class mapv: public hustr
{
public:
	mapv()
	{
	    m_data = NULL;
	}
	mapv(const char * fmt, ...)
	{
	        m_data = NULL;
		if (fmt == NULL)
		{
			return;
		}
		char buffer[strlen(fmt)*2];
		va_list argptr;
		int cnt;

		va_start(argptr, fmt);
		cnt = vsprintf(buffer, fmt, argptr);
		va_end(argptr);

		assign(buffer);

	}
	const char * getvalue()
	{
		const char * tmp = c_str();
		if (tmp[0] == '$')
		{
			const char * name = tmp + 1;
			if (g_var.getvar(name) != NULL)
			{
				tmp = g_var.getvar(name);
			}
			else
			{
				tmp = getenv(name);
			}

			if (tmp == NULL)
			{
				errexitf("$$$HU$$$$ Xml file Variable parameter [%s] undefined!!!\r\n", name);
			}
		}
		return tmp;
	}
	int getvalue_int()
	{
		return strtol(getvalue(), NULL, 10);
	}
	void * m_data;
};
typedef humap<mapv> HUMap;

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
	class HUTimerContainer: virtual public Mutex
	{
	public:
		typedef typename map<hustr, timer_node>::iterator iterator;
		map<hustr, timer_node> mp;
		void UpdateContainer(int cur)
		{
			lock();
			iterator it;
			for (it = mp.begin(); it != mp.end();)
			{
				if (cur > it->second.m_tm)
				{
					timer_fun_ft fun = it->second.m_fun;
					T data = it->second.m_data;
					mp.erase(it++);

					fun(cur, data);
					continue;
				}
				++it;

			}
			unlock();
		}
		void HUTimerAdd(const char * name, int tm, timer_fun_ft fun, T data)
		{
			lock();
			mp[name].m_data = data;
			mp[name].m_fun = fun;
			mp[name].m_tm = tm;
			unlock();
		}
	};

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
			if (m_Container != NULL)
			{
				m_Container->UpdateContainer(cur);
			}
			unlock();

			usleep(1000);
		}
	}
	void ChangeContainer(HUTimerContainer * c)
	{
		lock();
		m_Container = c;
		unlock();
	}
//	void add(const char * name, int tm, timer_fun_ft fun, T data)
//	{
//		lock();
//		mp[name].m_tm = tm;
//		mp[name].m_fun = fun;
//		mp[name].m_data = data;
//		unlock();
//	}

	HUTimerContainer * m_Container;
};

#endif //__HULIB_HU__
