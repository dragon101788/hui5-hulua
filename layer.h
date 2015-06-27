#ifndef __LAYER_H__
#define __LAYER_H__

#include "ParseXML.h"
#include "codec.h"
#include "schedule.h"
#include <deque>
#include <set>

#include "element_base.h"
using namespace std;

class schedule_draw;
class element_manager;
class xmlproc;

#define luacmd_is(name) (strncmp(cmd,name,strlen(name))==0&&(cmd = cmd+strlen(name " ")))

class element: virtual public element_base,public schedule_ele, public image,virtual public Mutex
{
public:
	//HUMap m_mp;
	virtual void doFlushConfig(HUMap &m_mp) = 0;
	virtual void doRender() = 0;
	virtual void doDelete()
	{
		printf("warning element bash OnDelete\r\n");
	}

	void Delete()
	{
		doDelete();
		revocation();
		SetHide(1);
		Render();
		ResetEB();
	}
	void onSchedule()
	{
		log_d("$$$HU$$$ Render_layer::[%s]\r\n", GetName());
		Render();
	}
	element()
	{
	        m_flag |= ELEMENT_FLAG_DRAWLOGIC;
		m_parent = NULL;
		m_proc = NULL;
		m_mgr = NULL;
		//log_d("new element m_parent=%x\n",m_parent);
	}

	virtual ~element()
	{
		log_i("$$$HU$$$ distroy element %s\r\n", GetName());
		backstack();
	}
	void Flush();
	void revocation();

	void Render();
	void Back();

	void FlushConfig(HUMap &m_mp);

	void ParseModifRes(HUMap &m_mp);

	virtual void doEleLuaCommand(const char * cmd) {};

	void doLuaCommand(const char * cmd)
	{
            if(luacmd_is("hide"))
            {
                printf("hide [%s]\n",cmd);
                if(strstr(cmd,"on"))
                {
                    SetHide(1);
                    Flush();
                }
                else if(strstr(cmd,"off"))
                {
                    SetHide(0);
                    Flush();
                }
            }
            else if(luacmd_is("move"))
            {
                int x,y;
                sscanf(cmd,"x=%d y=%d",&x,&y);
                printf("x=%d y=%d\n",x,y);
            }
            else
            {
                doEleLuaCommand(cmd);
            }
	}


	void PraseElement(HUMap &m_mp)
	{

		log_i("$$$HU$$$ ElementPrase [%s] x=%d y=%d width=%d height=%d hide=%d\r\n",
				GetName(),GetX(), GetY(), GetWidth(), GetHeight(), GetHeight());

		if(m_mp.exist("element_manager"))
                {
                    m_mgr = (element_manager *)m_mp["element_manager"].value().m_data;
                }
                if(m_mp.exist("parent"))
                {
                    element * parent = (element *)m_mp["parent"].value().m_data;
                    log_d("%s sub element\n",parent->GetName());
                    m_parent = parent;
                }

                //printf("%s SetBuffer width=%d height=%d\r\n", name.c_str(), width, height);
                SetBuffer(GetWidth(), GetHeight());
                path.format("ele-%s %dx%d", GetName(), GetWidth(), GetHeight());

		initstack();
	}
	void ResetEB()
	{
		if (!eb.empty())
		{
			list<element *>::iterator it;
			for (it = eb.begin(); it != eb.end(); ++it)
			{
				element * ele = *it;
				ele->et.remove(this);
			}
		}
	}
	void RenderEB()
	{
		if (!eb.empty())
		{
			list<element *>::iterator it;
			for (it = eb.begin(); it != eb.end(); ++it)
			{
				element * ele = *it;
				if (ele->GetHide() == 0)
				{
					//printf("$$$HU$$$ RenderEB %s <-- %s\r\n", name.c_str(), ele->name.c_str());

					int s_ofx = 0; //源x
					int d_ofx = 0; //目标x
					if (ele->GetX() < GetX())
					{
						s_ofx = GetX() - ele->GetX();
						d_ofx = 0;
					}
					else if (ele->GetX() > GetX())
					{
						s_ofx = 0;
						//d_ofx = width - (x + width - ele->x);
						d_ofx = ele->GetX() - GetX();
					}

					int s_ofy = 0; //源x
					int d_ofy = 0; //目标x
					if (ele->GetY() < GetY())
					{
						s_ofy = GetY() - ele->GetY();
						d_ofy = 0;
					}
					else if (ele->GetY() > GetY())
					{
						s_ofy = 0;
						//d_ofy = height - (y + height - ele->y);
						d_ofy = ele->GetY() - GetY();
					}

					AreaCopyFrom(ele, s_ofx, s_ofy, GetWidth(), GetHeight(), d_ofx, d_ofy);
				}
				//RollBack::RollBackBlock(*it, x, y, width, height);
				//(*it)->Render();
			}
		}
	}
	void RenderET()
	{
		if (!et.empty())
		{
			list<element *>::iterator it;
			for (it = et.begin(); it != et.end(); ++it)
			{
				element * ele = *it;
				if (ele->GetHide() == 0)
				{
					//printf("$$$HU$$$ RenderET %s <-- %s\r\n", name.c_str(), ele->name.c_str());
					ele->Render();
				}
			}
		}

	}
	class Cmpare
	{
	public:
		bool operator()(const element * st1, const element * st2) const
		{
			return (st1->GetLay()) < (st2->GetLay());
		}
	};
	void initstack();
	void addet(element * ele)
	{
		list<element *>::iterator it;
		for (it = et.begin(); it != et.end(); ++it)
		{
			if (*it == ele)
			{
				break;
			}
		}
		if (it == et.end())
		{
			//printf("$$$HU$$$ [%s] add [%s] ET\r\n",name.c_str(),ele->name.c_str());
			et.push_back(ele);
			et.sort(Cmpare());
		}
	}
	void addeb(element * ele)
	{
		list<element *>::iterator it;
		for (it = eb.begin(); it != eb.end(); ++it)
		{
			if (*it == ele)
			{
				break;
			}
		}
		if (it == eb.end())
		{
			//printf("$$$HU$$$ [%s] add [%s] EB\r\n",name.c_str(),ele->name.c_str());
			eb.push_back(ele);
			eb.sort(Cmpare());
		}
	}
	void delet(element * ele)
	{
		et.remove(ele);
	}
	void deleb(element * ele)
	{
		eb.remove(ele);
	}
	void backstack()
	{
		list<element *>::iterator it;
		for (it = eb.begin(); it != eb.end(); ++it)
		{
			(*it)->delet(this);
		}

		for (it = et.begin(); it != et.end(); ++it)
		{
			(*it)->deleb(this);
		}
	}
	void SetRes(const char * name,int id, const char * path)
	{
		if (res[name][id]->path != path || res[name][id]->isNULL())
		{
			//printf("SetRes %d %s\r\n",id,path);
		      res[name][id]->SetResource(path);
		}
	}
	void SetRes(const char * name, const char * path)
        {
                if (res[name]->path != path || res[name]->isNULL())
                {
                        //printf("SetRes %d %s\r\n",id,path);
                      res[name]->SetResource(path);
                }
        }
	image * GetRes(const char * name)
	{
	    return &res[name].value();
	}
	image * GetRes(const char * name,int id)
        {
	    return &res[name][id].value();
        }



	element * m_parent;
	xmlproc * m_proc;

	element_manager * m_mgr;

	humap<image> res;
	//schedule_draw * mgr;
	list<element *> et;					//�ϲ�ؼ�
	list<element *> eb;					//�ײ�ؼ�
};

//后续将绘图元素剥离出来
class element_manager
{
public:
	virtual void AddElement(const char * name, element * ele)
	{

		if (GetElementByName(name) == NULL)
		{
			elem[name] = ele;
		}
		else
		{
			printf("Repeat to add element %s\r\n", name);
		}

	}
	void ClearElement()
	{
		elem.clear();
	}
	void DelElement(const char * name)
	{
		element * ele = GetElementByName(name);
		if (ele == NULL)
		{
			printf("can't del layer element %s\r\n", name);
		}
		else
		{
			ele->Delete();
			elem.erase(name);
		}
	}
	element * GetElementByName(const char * name)
	{
		map<hustr, SmartPtr<element> >::iterator it;
		it = elem.find(name);
		if (it != elem.end())
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}
	typedef map<hustr, SmartPtr<element> >::iterator iterator;
	iterator begin()
	{
		return elem.begin();
	}
	iterator end()
	{
		return elem.end();
	}
	element_manager()
	{
	}
	virtual ~element_manager()
	{
	}
	map<hustr, SmartPtr<element> > elem; //ʹ������ָ��

};

#endif //__LAYER_H__
