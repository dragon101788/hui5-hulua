#ifndef __LAYER_H__
#define __LAYER_H__

#include "ParaseXML.h"
#include "codec.h"
#include "schedule.h"
#include <deque>
#include <set>
using namespace std;

class schedule_draw;
class element_manager;
class xmlproc;
//class RollBack
//{
//public:
//	image save;
//	int x;
//	int y;
//	int width;
//	int height;
//	//int isok;
//	int math;
//	static image * src;
//	static image * dst;
//	static int InitRollBack(image * _src, image * _dst)
//	{
//		src = _src;
//		dst = _dst;
//	}
//	RollBack()
//	{
//		x = 0;
//		y = 0;
//		//isok = 0;
//		math = 0;
//	}
//	void init(int cx, int cy, int cwidth, int cheight)
//	{
//		if (dst == NULL || src == NULL)
//		{
//			printf("Undefined InitRollBack src or dst ,You can not use RollBack\r\n");
//			return;
//		}
//
//		//printf("RollBack init %d %d %d %d\r\n", cx, cy, cwidth, cheight);
//		width = cwidth;
//		height = cheight;
//		x = cx;
//		y = cy;
//		if (width + height)
//		{
//			if (save.pSrcBuffer == NULL || save.u32Width != width || save.u32Height != height)
//			{
//				save.SetBuffer(width, height);
//			}
//
//		}
//
//	}
//	void init(XMLMap & mp)
//	{
//		if (mp["RollBack"].Count() == 0)
//		{
//			hustr RollBack = mp["RollBack"]->getvalue();
//			if (RollBack == "no")
//			{
//				math = 0;
//				return;
//			}
//			else if (RollBack == "block")
//			{
//				//printf("init RollBack = block\r\n");
//				math = 2;
//			}
//			else
//			{
//				math = 1;
//			}
//			int cwidth = mp["width"]->getvalue_int();
//			int cheight = mp["height"]->getvalue_int();
//			int cx = mp["x"]->getvalue_int();
//			int cy = mp["y"]->getvalue_int();
//			init(cx, cy, cwidth, cheight);
//		}
//		else if (mp["RollBack"].Count() > 0)
//		{
//			hustr RollBack = mp["RollBack"]["math"]->getvalue();
//			if (RollBack == "no")
//			{
//				math = 0;
//				return;
//			}
//			else if (RollBack == "block")
//			{
//				//printf("init RollBack = block\r\n");
//				math = 2;
//			}
//			else
//			{
//				math = 1;
//			}
//			int cwidth = mp["RollBack"]["width"]->getvalue_int();
//			int cheight = mp["RollBack"]["height"]->getvalue_int();
//			int cx = mp["RollBack"]["x"]->getvalue_int();
//			int cy = mp["RollBack"]["y"]->getvalue_int();
//			init(cx, cy, cwidth, cheight);
//		}
//
//	}
//
//	int RollBackBegin(image * img, int x, int y)
//	{
//		if (math == 1)
//		{
//			RestoreRoback();
//		}
//		else if (math == 2)
//		{
//			//printf("math=2\r\n");
//			RollBackBlock();
//		}
//	}
//	int RollBackEnd(image * img, int x, int y)
//	{
//		if (math == 1)
//		{
//			SaveRoback(img, x, y);
//		}
//		else if (math == 2)
//		{
//
//		}
//	}
//	static int RollBackBlock(image * img, int x, int y, int width, int height)
//	{
//		dst->AreaCopy(img, x, y, width, height, x, y);
//	}
//	int RollBackBlock()
//	{
//		if (save.pSrcBuffer == NULL)
//		{
//			return 0;
//		}
//		//printf("RollBackBlock x=%d y=%d width=%d height=%d\r\n",x, y, width, height);
//		dst->AreaCopy(src, x, y, width, height, x, y);
//	}
//	int SaveRoback(image * img, int dx, int dy)
//	{
//		//debug_timer();
//		if (save.pSrcBuffer == NULL)
//		{
//			return 0;
//		}
//		x = dx;
//		y = dy;
//		unsigned int * src_buf = (unsigned int*) src->pSrcBuffer;
//		unsigned int src_w = src->u32Width;
//		unsigned int * dst_buf = (unsigned int*) save.pSrcBuffer;
//		unsigned int dst_w = save.u32Width;
//		unsigned int * chk_buf = (unsigned int*) img->pSrcBuffer;
//		unsigned int chk_w = img->u32Width;
//
//		for (int sy = 0; sy < img->u32Height; sy++)
//		{
//			for (int sx = 0; sx < img->u32Width; sx++)
//			{
//				S_DRVBLT_ARGB8 * sp = (S_DRVBLT_ARGB8 *) (chk_buf + sx + sy * chk_w);
//				if ((*sp).u8Alpha != 0)
//				{
//
//					S_DRVBLT_ARGB8 * up = (S_DRVBLT_ARGB8 *) (src_buf + dx + sx + (dy + sy) * src_w);
//					S_DRVBLT_ARGB8 * ap = (S_DRVBLT_ARGB8 *) (dst_buf + sx + sy * dst_w);
//					*ap = *up;
//				}
//			}
//		}
//		//debug_timer("SaveRoback");
//	}
//
//	int RestoreRoback()
//	{
//		//debug_timer();
//		if (save.pSrcBuffer == NULL)
//		{
//			return 0;
//		}
//		unsigned int * src_buf = (unsigned int*) save.pSrcBuffer;
//		unsigned int src_w = save.u32Width;
//		unsigned int * dst_buf = (unsigned int*) dst->pSrcBuffer;
//		unsigned int dst_w = dst->u32Width;
//		int dx = x;
//		int dy = y;
//		for (int sy = 0; sy < save.u32Height; sy++)
//		{
//			for (int sx = 0; sx < save.u32Width; sx++)
//			{
//				S_DRVBLT_ARGB8 * sp = (S_DRVBLT_ARGB8 *) (src_buf + sx + sy * src_w);
//				if ((*sp).u8Alpha != 0)
//				{
//					S_DRVBLT_ARGB8 * dp = (S_DRVBLT_ARGB8 *) (dst_buf + dx + sx + (dy + sy) * dst_w);
//
//					//				S_DRVBLT_ARGB8 * up = (S_DRVBLT_ARGB8 *) ((unsigned int*) g_lay_mgr[0]->pSrcBuffer + dx + sx
//					//						+ (dy + sy) * g_lay_mgr[0]->u32Width);
//
//					*dp = *sp;
//
//				}
//			}
//		}
//		//debug_timer("RestoreRoback");
//	}
//};

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

class element: public schedule_ele, public image, virtual public Mutex
{
public:
	HUMap m_mp;
	virtual void doFlushConfig() = 0;
	virtual void doRender() = 0;
	virtual void doGetInfo(info & info)
	{
		printf("warning element bash doGetInfo\r\n");
	}
	virtual void doDelete()
	{
		printf("warning element bash OnDelete\r\n");
	}

	void GetElementInfo(info & info)
	{
		info.AddInfo("name", name);
		info.AddInfo("x", x);
		info.AddInfo("y", y);
		info.AddInfo("width", width);
		info.AddInfo("height", height);
		info.AddInfo("hide", hide);
		info.AddInfo("lay", lay);
	}
	void GetInfo(info & info)
	{
		lock();
		doGetInfo(info);
		unlock();
	}
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
		debug("$$$HU$$$ Render_layer::[%s]\r\n", name.c_str());
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
		debug("###HU### distroy element %s\r\n", name.c_str());
		backstack();
		map<int, image>::iterator it;
		for (it = res.begin(); it != res.end(); ++it)
		{
			it->second.destory();
		}
	}
	void Flush();
	void revocation();

	void Render();
	void Back();

	void FlushConfig();

	void ParseModifRes();
	void PraseElement()
	{
		name = m_mp["name"]->getvalue();
		int tmpX = m_mp["x"]->getvalue_int();
		int tmpY = m_mp["y"]->getvalue_int();
		width = m_mp["width"]->getvalue_int();
		height = m_mp["height"]->getvalue_int();
		hide = m_mp["hide"]->getvalue_int();

		//控件被移动
		if (tmpX != x || tmpY != y)
		{
			Back();
			x = tmpX;
			y = tmpY;
		}
		debug(
				"$$$HU$$$ ElementPrase %s x=%d y=%d width=%d height=%d hide=%d\r\n",
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
	void ModifXmlMap(HUMap &mp)
	{
		lock();
		HUMap::iterator it;
		for (it = mp.begin(); it != mp.end(); ++it)
		{
			m_mp[it.key().c_str()] = it.value().getvalue();
		}
		FlushConfig();
		unlock();
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