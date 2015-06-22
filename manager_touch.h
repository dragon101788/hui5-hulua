#ifndef __TOUCH_MGR_H__
#define __TOUCH_MGR_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>
#include <list>
#include <set>
using namespace std;
#include "ParseXML.h"
#include "hulib.h"
#include "element_base.h"

class touch_manager;

class touch_element :virtual public element_base,virtual public Mutex
{
public:
//	virtual void doFlushConfig()
//	{
//		errexit("$$$HU$$$ can't virtual doFlushConfig\r\n");
//	}
	touch_element()
	{
	        m_flag |= ELEMENT_FLAG_TOUCH;
		top = 0;
		bottom = 0;
		left = 0;
		right = 0;
		isdn = 0;
		touch_lock = 0;
		touch_mgr = NULL;
		x_lock = 0;
		y_lock = 0;
		ox = 0;
		oy = 0;
		log_d("new touch_element\n");
		//RegistdoFlushConfig (touch_ele);
	}
	~touch_element()
	{
	}

	virtual void doTouchDown()=0;
	virtual void doTouchUp()=0;
	virtual void doTouchMove() {}
	virtual void doTouchActive() = 0;

	void touch_init_area()
	{
		lock();
		top = GetY();
		left = GetX();
		bottom = GetY() + GetHeight();
		right = GetX() + GetWidth();
		unlock();
	}
	void origin_in()
	{
		if (ox == 0 && oy == 0)
		{
			ox = GetTouchX();
			oy = GetTouchY();
		}
	}

	void origin_out()
	{
		ox = 0;
		oy = 0;
	}
	int move_x() const
	{
		if (x_lock)
			return 0;
		if (isdn == 0)
			return 0;

		return GetTouchX() - ox;
	}
	int move_y() const
	{
		if (y_lock)
			return 0;
		if (isdn == 0)
			return 0;

		return GetTouchY() - oy;
	}
	void touch_area()
	{
		lock();
		origin_in();
		if(isdn==0)
                {
                    doTouchDown();
                }
		else
                {
		    doTouchMove();
                }
		unlock();
	}
	void free_area()
	{
		lock();
		doTouchUp();
		origin_out();
		if (GetTouchP() == 0 && isArea(GetTouchX(), GetTouchY()))
		{
			touch_activate();
		}
		unlock();
	}
	void touch_activate()
	{
		lock();
		doTouchActive();
		unlock();
	}
	void TouchParaseXml(HUMap & mp)
	{
		lock();
		touch_lock = mp["lock"]->getvalue_int();
		x_lock = mp["x_lock"]->getvalue_int();
		y_lock = mp["y_lock"]->getvalue_int();
		log_d("TouchParaseXml %d %d %d\r\n", touch_lock, x_lock, y_lock);
		unlock();
	}

	int isArea(int x, int y)
	{
		return (x > left && x < right && y > top && y < bottom);
	}

	int GetTouchX() const;
	int GetTouchY() const;
	int GetTouchP() const;

	int top;
	int bottom;
	int left;
	int right;
	int isdn;
	int touch_lock;
	int ox, oy;
	int x_lock;
	int y_lock;
	touch_manager * touch_mgr;
};

struct touch_sample
{
	int x;
	int y;
	int pressure;
};
int GetTouchSample(touch_sample * samp);
int TouchInit();
//class touch_panel_sample_base
//{
//public:
//	touch_panel_sample_base()
//	{
//		x = 0;
//		y = 0;
//		pressure = 0;
//	}
//	int operator==(touch_panel_sample_base& other)
//	{
//		return (x == other.x && y == other.y && pressure == other.pressure);
//	}
//	virtual int init_touch_panel()
//	{
//		errexitf("using is virtual touch sample\r\n");
//	}
//
//	virtual int gather()
//	{
//		errexitf("using is virtual touch sample\r\n");
//	}
//};
//
//touch_panel_sample_base * GetTouchSample();


class touch_manager : virtual public Mutex
{
public:

	void AddEleArea(touch_element * ele)
	{
		lock();
		ele->touch_mgr = this;
		mp.insert(ele);
		unlock();
	}

	void init_touch()
	{

	}
	int touch_proc_event(touch_sample * samp)
	{
		if (cur_samp.x == samp->x &&cur_samp.y == samp->y&&cur_samp.pressure == samp->pressure)
		{				//过滤重复坐标
			return 0;
		}
		lock();
		cur_samp = *samp;
		iterator it;
		for (it = mp.begin(); it != mp.end(); ++it)
		{
			touch_element * toe = (*it);
			if (toe->touch_lock == 0)
			{
				if (toe->isArea(cur_samp.x, cur_samp.y)
						&& cur_samp.pressure != 0)
				{
					//printf("%d %d down\r\n",samp.x,samp.y);

					toe->touch_area();
					toe->isdn = 1;
					//ooe = it->second;
				}
				else
				{
					if (toe->isdn == 1)
					{
						toe->free_area();
						toe->isdn = 0;
					}
				}
			}
			else
			{
				//printf("touch lock\r\n");
			}
		}
		unlock();

	}
//	void StopElement(touch_element * ele)
//	{
//		mp[name]->touch_lock = 1;
//	}
	void DelTouchElement(touch_element * ele)
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
	void LockAll()
	{
		lock();
		iterator it;
		for (it = mp.begin(); it != mp.end(); ++it)
		{
			(*it)->touch_lock = 1;
		}
		unlock();
	}
	void UnLockAll()
	{
		lock();
		iterator it;
		for (it = mp.begin(); it != mp.end(); ++it)
		{
			(*it)->touch_lock = 0;
		}
		unlock();
	}
	touch_manager()
	{
	}
	virtual ~touch_manager()
	{
	}
	//ott2001a_sample samp;
	//int ox, oy;
	//touch_element * ooe;
	touch_sample cur_samp;
	typedef set<touch_element *>::iterator iterator;
	set<touch_element *> mp;
};

#endif //__TOUCH_MGR_H__
