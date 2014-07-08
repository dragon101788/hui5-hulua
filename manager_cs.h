#ifndef __CS_H__
#define __CS_H__

#include "layer.h"

int ParseXMLElement2(hustr name, HUMap & xmlmp, xmlproc * xml);

class CS_manager :virtual public Mutex
{
public:
	void CreateCS(const char * str, HUMap & xmlmp)
	{
		//printf("CreateCS++ %s\r\n",str);
		lock();
		m_cs[str].accept(xmlmp);
//		m_cs[str].fetch(xmlmp);
//		m_cs[str].remove("name");
		unlock();
		//m_cs[str].display();

		//printf("CreateCS--\r\n");
		return;
	}
	void PostScfg(HUMap & mp);
	void PostCS(HUMap::OrderList & mp);
	void PostCS(const char * str);

	void Stop()
	{
		lock();
		stop = 1;
		unlock();
	}
	void Start()
	{
		lock();
		stop = 0;
		unlock();
	}


	CS_manager()
	{
		stop = 0;
	}
	virtual ~CS_manager()
	{

	}
	int stop;
	xmlproc * elemgr;
	//map<hustr, element *> m_ele;
	typedef map<hustr, HUMap::OrderList>::iterator iterator;
	map<hustr, HUMap::OrderList> m_cs;
};

#endif //__CS_H__
