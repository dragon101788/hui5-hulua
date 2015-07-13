#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include "hulib.h"
#include <list>
using namespace std;

class schedule_ele
{
public:
	 virtual void onSchedule()=0;
	inline int schedule(){
		wait_to_render=0;
		onSchedule();
	}

	inline int waitRender()const{
		return wait_to_render;
	}
	schedule_ele():wait_to_render(0){
	};
	virtual ~schedule_ele(){};
	int wait_to_render;
};

class element_manager;
class schedule_draw
{
public:

	virtual int ScheduleProc();

	class schedule_queue: public Mutex
	{
	public:

		void clean()
		{
			lock();
			m_list.clear();
			unlock();
		}

		void addele(schedule_ele * ele)
		{
			lock();
			list<schedule_ele *>::iterator it;
			for (it = m_list.begin(); it != m_list.end(); ++it)
			{
				if (ele == *it)
				{
					if(ele->waitRender()){
						unlock();
						return;
					}
					else{
						continue;
					}
				}
			}
			if (it == m_list.end())
			{
				ele->wait_to_render++;
				m_list.push_back(ele);

			}
			unlock();
		}
		schedule_ele * getele()
		{
			schedule_ele * ret;
			if (m_list.empty() == 1)
			{
				return NULL;
			}
			lock();
			ret = m_list.front();
			m_list.pop_front();
			unlock();
			return ret;
		}
		void delele(schedule_ele * ele)
		{
			lock();
			m_list.remove(ele);
			unlock();
		}

		int getSize(){
			return m_list.size();
		}
		list<schedule_ele *> m_list;
	};

//	class element_queue: public Mutex
//	{
//	public:
//		unsigned int id;
//		element_queue()
//		{
//			id=0;
//		}
//		class Less
//		{
//		public:
//			bool operator()(element * s1, element * s2) const
//			{
//				return s1->schedule_id < s2->schedule_id;
//			}
//		};
//		void clean()
//		{
//			lock();
//			m_set.clear();
//			unlock();
//		}
//		void addele(element * ele)
//		{
//			lock();
//			ele->schedule_id = id++;
//			m_set.insert(ele);
//			unlock();
//		}
//		element * getele()
//		{
//			element * ret;
//			if (m_set.empty() == 1)
//			{
//				return NULL;
//			}
//			lock();
//			set<element *>::iterator it;
//			it = m_set.begin();
//			ret = *it;
//			m_set.erase(it);
//			unlock();
//			return ret;
//		}
//		void delele(element * ele)
//		{
//			lock();
//			m_set.erase(ele);
//			unlock();
//		}
//		void del_elemgr(element_manager * mgr)
//		{
//			lock();
//			element_manager::iterator it;
//			for (it = mgr->begin(); it != mgr->end(); ++it)
//			{
//				delele(it->second);
//			}
//			unlock();
//		}
//		set<element *,Less> m_set;
//	};
	void del_elemgr(element_manager * mgr);
	schedule_draw()
	{
		width = 0;
		height = 0;
		stop = 0;

	}

	void clean()
	{
		que.clean();
	}

	schedule_queue que;
	int width;
	int stop;
	int height;

	//hustr name;
};

//extern schedule_draw g_schedule_draw;

#endif //__SCHEDULE_H__
