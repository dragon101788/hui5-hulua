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


	if (m_parent)
	{
		m_parent->lock();
		if (GetHide() == 0)
		{
			log_d("%s doRender",GetName());
			doRender();
		}
		else
		{
			log_d("Render %s hide\r\n", GetName());
		}
		//printf("draw %s parent %s  %d %d %d %d\n",m_parent->GetName(),GetName(),GetWidth(), GetHeight(), GetX(), GetY());
		m_parent->RenderFrom(this, 0, 0, GetWidth(), GetHeight(), GetX(), GetY());	//控件输出到父亲
		m_parent->unlock();
		//m_parent->Flush();
	}
	else
	{
		RenderEB();

		if (GetHide() == 0)
		{
			log_d("%s doRender",GetName());
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



//image * RollBack::dst = NULL;
//image * RollBack::src = NULL;
//map<hustr, SmartPtr<element> > layer::elem; //ʹ������ָ��
