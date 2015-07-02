#include "layer.h"
#include "xmlproc.h"
#include "ttf_font.h"
#include "include/autoconf.h"

void element::Flush()
{
	if (m_proc != NULL)
	{
		lock();
		m_proc->que.addele(this);
		unlock();
	}
	else
	{
		errexitf("$$$$HU$$$$ Flush element %s manager is NULL\r\n",
				GetName());
	}
}
void element::revocation()
{
	if (m_proc != NULL)
	{
		lock();
		m_proc->que.delele(this);
		unlock();
	}
	else
	{
		errexitf("$$$$HU$$$$ revocation element %s manager is NULL\r\n",
				GetName());
	}
}

int crossAlgorithm(element * r1, element * r2)
{
	int nMaxLeft = 0;
	int nMaxTop = 0;
	int nMinRight = 0;
	int nMinBottom = 0;

	//计算两矩形可能的相交矩形的边界
	nMaxLeft = r1->GetX() >= r2->GetX() ? r1->GetX() : r2->GetX();
	nMaxTop = r1->GetY() >= r2->GetY() ? r1->GetY() : r2->GetY();
	nMinRight =
			(r1->GetX() + r1->GetWidth()) <= (r2->GetX() + r2->GetWidth()) ?
					(r1->GetX() + r1->GetWidth()) : (r2->GetX() + r2->GetWidth());
	nMinBottom =
			(r1->GetY() + r1->GetHeight()) <= (r2->GetY() + r2->GetHeight()) ?
					(r1->GetY() + r1->GetHeight()) : (r2->GetY() + r2->GetHeight());
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
	for (it = m_mgr->begin(); it != m_mgr->end(); ++it)
	{
		element * ele = it->second;
		//if(x>ele->x&&y>ele->y&&x+width<ele->x+ele->width&&y+height<ele->y+ele->height)
		if (crossAlgorithm(ele, this))
		{

			if (ele->GetLay() < GetLay() && GetLay() != 0)
			{
				addeb(ele);
				ele->addet(this);

			}
			else if (ele->GetLay() > GetLay())
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


	if(m_parent)
        {
	    if (GetHide() == 0)
            {
                 doRender();
            }
	    else
            {
                    log_d("Render %s hide\r\n", GetName());
            }
	    //printf("draw %s parent %s  %d %d %d %d\n",m_parent->GetName(),GetName(),GetWidth(), GetHeight(), GetX(), GetY());
	    m_parent->RenderFrom(this, 0, 0, GetWidth(), GetHeight(), GetX(), GetY());//控件输出到父亲
	    //m_parent->Flush();
        }
	else
        {
	    RenderEB();

            if (GetHide() == 0)
            {
                    doRender();
            }
            else
            {
                    log_d("Render %s hide\r\n", GetName());
            }
	    //printf("draw %s %d %d %d %d\n",GetName(),GetWidth(), GetHeight(), GetX(), GetY());
            m_proc->Draw(this, 0, 0, GetWidth(), GetHeight(), GetX(), GetY());//控件输出到容器

            RenderET();
        }

	unlock();
}

void element::Back()
{
	lock();
	RenderEB();
	m_proc->Draw(this, 0, 0, GetWidth(), GetHeight(), GetX(), GetY());
	RenderET();
	unlock();

}

void element::FlushConfig(HUMap &mp)
{
	lock();
	PraseElement(mp);
	doFlushConfig(mp);
	ParseModifRes(mp);
	Flush();

	m_mgr->AddElement(GetName(), this);
	unlock();
}

void element::ParseModifRes(HUMap &m_mp)
{
	for (int i = 0; i < m_mp.count("res"); ++i)
	{
		HUMap  xmlmp = m_mp["res"][i];
		hustr meth = xmlmp["meth"]->getvalue();
		const char * name = xmlmp["name"]->getvalue();
		if(strlen(name)==0)
		  errexitf("ModifRes %s unknown name\n",GetName());
		int id = xmlmp["id"]->getvalue_int();
		int src_x = xmlmp["src_x"]->getvalue_int();
		int src_y = xmlmp["src_y"]->getvalue_int();
		int cp_width = xmlmp["cp_width"]->getvalue_int();
		int cp_height = xmlmp["cp_height"]->getvalue_int();
		int dst_x = xmlmp["dst_x"]->getvalue_int();
		int dst_y = xmlmp["dst_y"]->getvalue_int();

		res[name][id]->LoadResource();
		if (cp_width + cp_height == 0)
		{
			cp_width = res[name][id]->GetImageWidth();
			cp_height = res[name][id]->GetImageHeight();
		}
		if (meth == "render")
		{
			image file;
			file.SetResource(xmlmp["file"]->getvalue());
			file.LoadResource();

			log_d(
					"$$$HU$$$ XML Draw Render %s to %s res=%d %d %d %d %d %d %d\r\n",
					file.path.c_str(), GetName(), id, src_x, src_y, cp_width,
					cp_height, dst_x, dst_y);

			if (!res[name][id]->isNULL())
			{

				//ele->image::Render(&file, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
				lock();

				res[name][id]->RenderFrom(&file, src_x, src_y, cp_width, cp_height, dst_x,
						dst_y);
				//ele->Render();
				unlock();

			}
			else
			{
				errexitf("%s res %d no init\r\n", GetName(), id);
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
			//log_d("get font_mp %s %x %x\r\n",);
			tmpttf.fontHeight = size;
			tmpttf.color = color;
			tmpttf.style = style;
			tmpttf.SetBuffer(cp_width, cp_height);
			tmpttf.drawText((char *) txt.c_str(), txt.length());

			log_d("ParseModifRes text=%s [%s] <%x %x>\r\n",txt.c_str(),font.nstr() ,tmpttf.m_font->face,tmpttf.m_font->ft_Lib);
			if (!res[name][id]->isNULL())
			{

				//ele->image::Render(&file, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
				lock();

				res[name][id]->RenderFrom(&tmpttf, src_x, src_y, cp_width, cp_height,
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
