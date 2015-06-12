#ifndef __LAYER_H__
#define __LAYER_H__

#include "ParseXML.h"
#include "codec.h"
#include "schedule.h"
#include <deque>
#include <set>
#include "hui_hulua.h"
using namespace std;

class schedule_draw;
class element_manager;
class xmlproc;


class info: public hustr
{
public:
	void AddInfo(const char * key, const char * value)
	{
		(*this) += key;
		(*this) += "=";
		(*this) += value;
		(*this) += "\r\n";
	}
	void AddInfo(const char * key, int value)
	{
		hustr str("%d", value);
		AddInfo(key, str);
	}
};

class element: public schedule_ele, public image,virtual public Mutex
{
public:
	//HUMap m_mp;
	virtual void doFlushConfig(HUMap &m_mp) = 0;
	virtual void doRender() = 0;
//	virtual void doGetInfo(info & info)
//	{
//		printf("warning element bash doGetInfo\r\n");
//	}
	virtual void doDelete()
	{
		printf("warning element bash OnDelete\r\n");
	}

//	void GetElementInfo(info & info)
//	{
//		info.AddInfo("name", name);
//		info.AddInfo("x", x);
//		info.AddInfo("y", y);
//		info.AddInfo("width", width);
//		info.AddInfo("height", height);
//		info.AddInfo("hide", hide);
//		info.AddInfo("lay", lay);
//	}
//	void GetInfo(info & info)
//	{
//		lock();
//		doGetInfo(info);
//		unlock();
//	}
	void Delete()
	{
		doDelete();
		revocation();
		hide = 1;
		Render();
		ResetEB();
	}
	void onSchedule()
	{
		log_d("$$$HU$$$ Render_layer::[%s]\r\n", name.c_str());
		Render();
		//debug("$$$HU$$$ Render_layer::[%s]OK\r\n", name.c_str());
	}
	element()
	{
		hide = 0;
		x = 0;
		y = 0;
		height = 0;
		width = 0;
		lay = 0;
		mgr = NULL;
		//RegistdoFlushConfig(element);
	}

	virtual ~element()
	{
		log_i("$$$HU$$$ distroy element %s\r\n", name.c_str());
		backstack();
		map<int, image>::iterator it;
		for (it = res.begin(); it != res.end(); ++it)
		{
			it->second.destroy();
		}
	}
	void Flush();
	void revocation();

	void Render();
	void Back();

	void FlushConfig(HUMap &m_mp);

	void ParseModifRes(HUMap &m_mp);

	virtual void doLuaCommand(const char * cmd)
	{
	    printf("%s virtual doLuaCommand Nothing can be done\n",name.nstr());
	}
	void LuaCommand(const char * cmd)
	{
	    doLuaCommand(cmd);
	}
	class element_lua_node :public LUA::node
	{
	public:
	  element * m_ele;
	  element_lua_node(element * ele)
	  {
	     m_ele = ele;
	  }
          void DO(lua_State * L)
          {
            //if (hulua::get_type(L, m_ele->name) == hulua::lua_nil);
              printf("lua create %s object\n", m_ele->name.nstr());
              hulua::class_add<element>(L, "lua_test_page");
              hulua::class_mem<element>(L, "x", &element::x);
              hulua::class_mem<element>(L, "y", &element::y);
              hulua::class_mem<element>(L, "width", &element::width);
              hulua::class_mem<element>(L, "height", &element::height);
              hulua::class_def<element>(L, "command", &element::LuaCommand);
              hulua::set(L, m_ele->name, this);
          }
	};

	void PraseElement(HUMap &m_mp)
	{
		name = m_mp["name"]->getvalue();
		int tmpX = m_mp["x"]->getvalue_int();
		int tmpY = m_mp["y"]->getvalue_int();
		width = m_mp["width"]->getvalue_int();
		height = m_mp["height"]->getvalue_int();
		hide = m_mp["hide"]->getvalue_int();

		lua.regele(this);
                //lua.add(new element_lua_node(this));


		//控件被移动
		if (tmpX != x || tmpY != y)
		{
			Back();
			x = tmpX;
			y = tmpY;
		}
		log_i("$$$HU$$$ ElementPrase %s x=%d y=%d width=%d height=%d hide=%d\r\n",
				name.c_str(), x, y, width, height, hide);

		if (m_mp.exist("lay"))
		{
			lay = m_mp["lay"]->getvalue_int();
		}
		else
		{
			lay = 5;
		}

		if (pSrcBuffer == NULL)
		{
			//printf("%s SetBuffer width=%d height=%d\r\n", name.c_str(), width, height);
			SetBuffer(width, height);
			path.format("ele-%s %dx%d", name.c_str(), width, height);
		}
		initstack();
	}
//	void ModifXmlMap(HUMap &mp)
//	{
//		lock();
//		HUMap::iterator it;
//		for (it = mp.begin(); it != mp.end(); ++it)
//		{
//			m_mp[it.key().c_str()] = it.value().getvalue();
//		}
//		FlushConfig();
//		unlock();
//	}
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
				if (ele->hide == 0)
				{
					//printf("$$$HU$$$ RenderEB %s <-- %s\r\n", name.c_str(), ele->name.c_str());

					int s_ofx = 0; //源x
					int d_ofx = 0; //目标x
					if (ele->x < x)
					{
						s_ofx = x - ele->x;
						d_ofx = 0;
					}
					else if (ele->x > x)
					{
						s_ofx = 0;
						//d_ofx = width - (x + width - ele->x);
						d_ofx = ele->x - x;
					}

					int s_ofy = 0; //源x
					int d_ofy = 0; //目标x
					if (ele->y < y)
					{
						s_ofy = y - ele->y;
						d_ofy = 0;
					}
					else if (ele->y > y)
					{
						s_ofy = 0;
						//d_ofy = height - (y + height - ele->y);
						d_ofy = ele->y - y;
					}

					AreaCopy(ele, s_ofx, s_ofy, width, height, d_ofx, d_ofy);
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
				if (ele->hide == 0)
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
			return st1->lay < st2->lay;
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
	void SetRes(int id, const char * path)
	{
		if (res[id].path != path || res[id].isNULL())
		{
			//printf("SetRes %d %s\r\n",id,path);
			res[id].SetResource(path);
		}
	}

	hustr name;
	int hide;
	int x;
	int y;
	int width;
	int height;
	int lay;
	xmlproc * xml_mgr;
	map<int, image> res;
	schedule_draw * mgr;
	list<element *> et;					//�ϲ�ؼ�
	list<element *> eb;					//�ײ�ؼ�
};

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
