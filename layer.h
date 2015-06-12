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

class element_base
{
public:
  void SetName(const char * name)
  {
    m_name = name;
  }
  void SetX(int x)
  {
    m_x = x;
  }
  void SetY(int y)
  {
    m_y = y;
  }
  void SetWidth(int width)
  {
    m_width = width;
  }
  void SetHeight(int height)
  {
    m_height = height;
  }

  void SetLay(int lay)
  {
    m_lay = lay;
  }
  void SetHide(int hide)
  {
    m_hide = hide;
  }
  const char * GetName() const
  {
    return m_name.nstr();
  }
  int GetX() const
  {
    return m_x;
  }
  int GetY() const
  {
    return m_y;
  }

  int GetWidth() const
  {
    return m_width;
  }

  int GetHeight() const
  {
    return m_height;
  }

  int GetLay() const
  {
    return m_lay;
  }

  int GetHide() const
  {
    return m_hide;
  }

  virtual void doLuaCommand(const char * cmd)
  {
      printf("%s virtual doLuaCommand Nothing can be done\n",GetName());
  }
  void LuaCommand(const char * cmd)
  {
      doLuaCommand(cmd);
  }
  class element_lua_node :public LUA::node
  {
  public:
    element_base * m_ele;
    element_lua_node(element_base * ele)
    {
       m_ele = ele;
    }
    void DO(lua_State * L)
    {
      //if (hulua::get_type(L, m_ele->name) == hulua::lua_nil);
      if(m_ele->GetName()!=NULL)
      {
        printf("lua create %s object\n", m_ele->GetName());
                hulua::class_add<element_base>(L, "element_base");
                hulua::class_mem<element_base>(L, "x", &element_base::m_x);
                hulua::class_mem<element_base>(L, "y", &element_base::m_y);
                hulua::class_mem<element_base>(L, "width", &element_base::m_width);
                hulua::class_mem<element_base>(L, "height", &element_base::m_height);
                hulua::class_mem<element_base>(L, "lay", &element_base::m_lay);
                hulua::class_mem<element_base>(L, "hide", &element_base::m_hide);
                hulua::class_def<element_base>(L, "command", &element_base::LuaCommand);
                hulua::set(L, m_ele->GetName(), m_ele);
      }
    }
  };
//private:
  hustr m_name;
  int m_hide;
  int m_x;
  int m_y;
  int m_width;
  int m_height;
  int m_lay;
};

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
		m_hide = 1;
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
		m_hide = 0;
		m_x = 0;
		m_y = 0;
		m_height = 0;
		m_width = 0;
		m_lay = 0;
		m_parent = NULL;
		xml_mgr = NULL;
		//log_d("new element m_parent=%x\n",m_parent);
	}

	virtual ~element()
	{
		log_i("$$$HU$$$ distroy element %s\r\n", GetName());
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



        #define defset_element_int(name,member,mp,def) if (mp.exist(name)){member = mp[name]->getvalue_int();} else{member = def;}

	void PraseElement(HUMap &m_mp)
	{
	        log_i("$$$HU$$$ ElementPrase [%s] %x x=%d y=%d width=%d height=%d hide=%d\r\n",
	                                  GetName(), m_parent,GetX(), GetY(), GetWidth(), GetHeight(), GetHeight());
		SetName(m_mp["name"]->getvalue());
		defset_element_int("x",m_x,m_mp,0);
		defset_element_int("y",m_y,m_mp,0);
		defset_element_int("width",m_width,m_mp,0);
		defset_element_int("height",m_height,m_mp,0);
		defset_element_int("hide",m_hide,m_mp,0);
		defset_element_int("lay",m_lay,m_mp,5);


		lua.regele(this);

		log_i("$$$HU$$$ ElementPrase [%s] x=%d y=%d width=%d height=%d hide=%d\r\n",
				GetName(),GetX(), GetY(), GetWidth(), GetHeight(), GetHeight());



		if (pSrcBuffer == NULL)
		{
			//printf("%s SetBuffer width=%d height=%d\r\n", name.c_str(), width, height);
			SetBuffer(GetWidth(), GetHeight());
			path.format("ele-%s %dx%d", GetName(), GetWidth(), GetHeight());
		}
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

					AreaCopy(ele, s_ofx, s_ofy, GetWidth(), GetHeight(), d_ofx, d_ofy);
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
	void SetRes(int id, const char * path)
	{
		if (res[id].path != path || res[id].isNULL())
		{
			//printf("SetRes %d %s\r\n",id,path);
			res[id].SetResource(path);
		}
	}



	element * m_parent;
	xmlproc * xml_mgr;

	map<int, image> res;
	//schedule_draw * mgr;
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
