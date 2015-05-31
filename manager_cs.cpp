#include "xmlproc.h"

void CS_manager::PostCS(HUMap::OrderList & mp)
{
	if (stop)
		return;

	lock();
	//mp.display();
	HUMap::OrderList::iterator it;
	for (it = mp.begin(); it != mp.end(); ++it)
	{
		if((*it).m_key=="name")
		{
			continue;
		}
		//printf("parse %s\r\n",(*it).m_key.c_str());
		//(*it).display();
		ParseXMLFrom_Instan((*it).m_key, (*it), elemgr);
		//printf("CS_manager::PostCS %s\r\n", it->first.c_str());
//		for(int i=0;i<it->second.BrotherCount();i++)
//		{
//			printf("CS_manager::PostCS %s[%d]\r\n", it->first.c_str(),i);
//			HUMap & tmp = it->second[i];
//			//tmp.display();
//			ParseXMLElement2(it->first, tmp, elemgr);
//		}

	}
	unlock();
}

void CS_manager::PostCS(const char * str)
{
	if (stop)
		return;

	lock();
	printf("PostCS %s ++\r\n", str);
	iterator it;

	it = m_cs.find(str);
	if (it != m_cs.end())
	{
		//printf("PostCS find %s\r\n", str);
		//it->second.display();
		//printf("count=%d\r\n",cnt);
		PostCS(it->second);

	}
	else
	{
		printf("PostCS can't find cs [%s]\r\n", str);
	}
	printf("PostCS %s--\r\n", str);
	unlock();
}

void CS_manager::PostScfg(HUMap & mp)
{
	lock();
	hustr name = mp["name"]->getvalue();
	//printf("PostCS name = %s\r\n", name.c_str());

	element * ele = elemgr->GetElementByName(name);
	if (ele != NULL)
	{
		ele->ModifXmlMap(mp);
	}
	else
	{
		printf("PostCS can't find element [%s]\r\n", name.c_str());
	}
	unlock();
}
