#include "layer.h"
#include "xmlproc.h"
#include "ttf_font.h"
#include "include/autoconf.h"

void element::Flush()
{
	if (mgr != NULL)
	{
		lock();
		mgr->que.addele(this);
		unlock();
	}
	else
	{
		errexitf("$$$$HU$$$$ Flush element %s manager is NULL\r\n",
				name.c_str());
	}
}
void element::revocation()
{
	if (mgr != NULL)
	{
		lock();
		mgr->que.delele(this);
		unlock();
	}
	else
	{
		errexitf("$$$$HU$$$$ revocation element %s manager is NULL\r\n",
				name.c_str());
	}
}

int crossAlgorithm(element * r1, element * r2)
{
	int nMaxLeft = 0;
	int nMaxTop = 0;
	int nMinRight = 0;
	int nMinBottom = 0;

	//计算两矩形可能的相交矩形的边界
	nMaxLeft = r1->x >= r2->x ? r1->x : r2->x;
	nMaxTop = r1->y >= r2->y ? r1->y : r2->y;
	nMinRight =
			(r1->x + r1->width) <= (r2->x + r2->width) ?
					(r1->x + r1->width) : (r2->x + r2->width);
	nMinBottom =
			(r1->y + r1->height) <= (r2->y + r2->height) ?
					(r1->y + r1->height) : (r2->y + r2->height);
	// 判断是否相交
	//printf("crossAlgorithm %d %d %d %d\r\n",nMaxLeft , nMinRight , nMaxTop , nMinBottom);
	if (nMaxLeft >= nMinRight || nMaxTop >= nMinBottom)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void element::initstack()
{
//	for (int i = 0; i < m_mp["eb"].BrotherCount(); i++)
//	{
//		const char * name = m_mp["eb"][i]["name"]->getvalue();
//		element * ele = g_ele_mgr.GetElementByName(name);
//		if (ele != NULL)
//		{
//			addeb(ele);
//		}
//	}
//	for (int i = 0; i < m_mp["et"].BrotherCount(); i++)
//	{
//		const char * name = m_mp["et"][i]["name"]->getvalue();
//		element * ele = g_ele_mgr.GetElementByName(name);
//		if (ele != NULL)
//		{
//			addet(ele);
//		}
//	}
	element_manager::iterator it;
	for (it = xml_mgr->begin(); it != xml_mgr->end(); ++it)
	{
		element * ele = it->second;
		//if(x>ele->x&&y>ele->y&&x+width<ele->x+ele->width&&y+height<ele->y+ele->height)
		if (crossAlgorithm(ele, this))
		{

			if (ele->lay < lay && lay != 0)
			{
				addeb(ele);
				ele->addet(this);

			}
			else if (ele->lay > lay)
			{
				addet(ele);
				ele->addeb(this);
			}
			else
			{
				//printf("$$$HU$$$ warning %s with %s same layer cross\r\n",name.c_str(),ele->name.c_str());
			}
		}
	}
}

//void element_RoolBack::Draw(image * img, int x, int y)
//{
//	rob.RollBackBegin(img, x, y);
//	if (hide == 0)
//	{
//		mgr->DrawIn(img, x, y);
//		rob.RollBackEnd(img, x, y);
//	}
//
//}

void element::Render()
{
	lock();
	RenderEB();

	if (hide == 0)
	{
		doRender();
	}
	else
	{
		printf("Render %s hide\r\n", name.c_str());
	}

	xml_mgr->Draw(this, 0, 0, width, height, x, y);
	RenderET();
	unlock();
}

void element::Back()
{
	lock();
	RenderEB();
	xml_mgr->Draw(this, 0, 0, width, height, x, y);
	RenderET();
	unlock();

}

void element::FlushConfig()
{
	lock();
	doFlushConfig();
	xml_mgr->AddElement(name, this);
	unlock();
}

void element::ParseModifRes()
{
	for (int i = 0; i < m_mp.count("res"); ++i)
	{
		HUMap  xmlmp = m_mp["res"][i];
		hustr meth = xmlmp["meth"]->getvalue();
		int id = xmlmp["id"]->getvalue_int();
		int src_x = xmlmp["src_x"]->getvalue_int();
		int src_y = xmlmp["src_y"]->getvalue_int();
		int cp_width = xmlmp["cp_width"]->getvalue_int();
		int cp_height = xmlmp["cp_height"]->getvalue_int();
		int dst_x = xmlmp["dst_x"]->getvalue_int();
		int dst_y = xmlmp["dst_y"]->getvalue_int();

		res[id].LoadResource();
		if (cp_width + cp_height == 0)
		{
			cp_width = res[id].GetWidth();
			cp_height = res[id].GetHeight();
		}
		if (meth == "render")
		{
			image file;
			file.SetResource(xmlmp["file"]->getvalue());
			file.LoadResource();

			printf(
					"$$$HU$$$ XML Draw Render %s to %s res=%d %d %d %d %d %d %d\r\n",
					file.path.c_str(), name.c_str(), id, src_x, src_y, cp_width,
					cp_height, dst_x, dst_y);

			if (!res[id].isNULL())
			{

				//ele->image::Render(&file, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
				lock();

				res[id].Render(&file, src_x, src_y, cp_width, cp_height, dst_x,
						dst_y);
				//ele->Render();
				unlock();

			}
			else
			{
				errexitf("%s res %d no init\r\n", name.c_str(), id);
			}
		}
		else if (meth == "text")
		{
#ifdef CONFIG_USING_FONT
			int red = xmlmp["red"]->getvalue_int();
			int green = xmlmp["green"]->getvalue_int();
			int blue = xmlmp["blue"]->getvalue_int();
			int color = (red & 0xff) << 16 | (green & 0xff) << 8 | blue & 0xff;
			//bkcolor = m_mp["bkcolor"]->getvalue_int();
			hustr font = xmlmp["font"]->getvalue();
			hustr txt = xmlmp["txt"]->getvalue();
			int style = (unsigned char) m_mp["style"]->getvalue_int();
			int size = xmlmp["size"]->getvalue_int();


			text tmpttf;
			tmpttf.m_font = &font_mp[font];
			printf("get font_mp %s %x %x\r\n",font.nstr() ,tmpttf.m_font->face,
					tmpttf.m_font->ft_Lib);
			tmpttf.fontHeight = size;
			tmpttf.color = color;
			tmpttf.style = style;
			tmpttf.SetBuffer(cp_width, cp_height);
			tmpttf.DrawText("UTF-8", (char *) txt.c_str(), txt.length());

			printf("ParseModifRes text=%s\r\n",txt.c_str());
			if (!res[id].isNULL())
			{

				//ele->image::Render(&file, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
				lock();

				res[id].Render(&tmpttf, src_x, src_y, cp_width, cp_height,
						dst_x, dst_y);
				//ele->Render();
				unlock();

			}
#endif
		}

	}
	m_mp.erase("res");
}
//image * RollBack::dst = NULL;
//image * RollBack::src = NULL;
//map<hustr, SmartPtr<element> > layer::elem; //ʹ������ָ��
