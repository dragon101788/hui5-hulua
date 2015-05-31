#ifndef __PARASE_XML_H__
#define __PARASE_XML_H__

#include <map>
#include <list>
using namespace std;

#include "tinyxml/tinyxml.h"
#include "hulib.h"

//class XMLAttribute
//{
//public:
//	void SetAttribute(const char * name, const char * value)
//	{
//		//printf("SetAttribute %s = %s", name, value);
//		mp[name] = value;
//	}
//	const char * GetValue(const char * name)
//	{
//		map<hustr, hustr>::iterator it;
//		it = mp.find(name);
//		if (it != mp.end())
//		{
//			return it->second.c_str();
//		}
//		else
//		{
//			printf("Request attribute [%s] is invalid\r\n", name);
//			return NULL;
//		}
//	}
//
//	const char * operator[](const char * str)
//	{
//		return GetValue(str);
//	}
//
//	int GetValueInt(const char * name)
//	{
//		const char * ret = GetValue(name);
//		if (ret)
//			return strtoul(ret, NULL, 10);
//		else
//			return 0;
//	}
//
//	void debug()
//	{
//		map<hustr, hustr>::iterator it;
//		printf("XMLAttribute::debug \r\n");
//		for (it = mp.begin(); it != mp.end(); ++it)
//		{
//			printf("	%s %s\r\n", it->first.c_str(), it->second.c_str());
//		}
//	}
//	map<hustr, hustr> mp;
//};

//class HUMap: public hustr
//{
//public:
//	HUMap & operator=(const char * _value)
//	{
//		m_value = _value;
//		return *this;
//	}
//	int find(const char * _key)
//	{
//		map<hustr, HUMap>::iterator it;
//		it = m_mp.find(_key);
//		if (it != m_mp.end())
//		{
//			return 1;
//		}
//		else
//		{
//			return 0;
//		}
//	}
//	int find(int num)
//	{
//		map<int, HUMap>::iterator it;
//		it = brother.find(num);
//		if (it != brother.end())
//		{
//			return 1;
//		}
//		else
//		{
//			return 0;
//		}
//	}
//
//	void remove(const char * key)
//	{
//		printf("xmlmap remove %s\r\n",key);
//		m_mp.erase(key);
//	}
//	void remove(const int num)
//	{
//		brother.erase(num);
//	}
//
//	HUMap& getsun(const char * _key)
//	{
//		if (find(_key))
//		{ //add
//			return m_mp[_key];
//		}
//		else
//		{ //new
//			m_mp[_key].father = this;
//			m_mp[_key].assign(_key);
//			return m_mp[_key];
//		}
//	}
//	HUMap& getboter(int num)
//	{
//
//		if (find(num))
//		{ //exten
//			if (num == 0)
//			{
//				return *this;
//			}
//			else
//			{
//				return brother[num];
//			}
//		}
//		else
//		{ //new
//			if (num == 0)
//			{
//				brother[num] = *this;
//				return *this;
//			}
//			else
//			{
//				brother[num].father = this->father;
//				brother[num].format("%s%d", c_str(), num);
//
//				return brother[num];
//			}
//
//			//return brother[num];
//		}
//	}
//	const char * getvalue()
//	{
//		const char * tmp = m_value.c_str();
//		if (tmp[0] == '$')
//		{
//			const char * name = tmp + 1;
//			if (g_var.getvar(name) != NULL)
//			{
//				tmp = g_var.getvar(name);
//			}
//			else
//			{
//				tmp = getenv(name);
//			}
//
//			if (tmp == NULL)
//			{
//				errexitf("$$$HU$$$$ Xml file Variable parameter [%s] undefined!!!\r\n", name);
//			}
//		}
//		return tmp;
//	}
//	operator const char *()
//	{
//		return getvalue();
//	}
//	int getvalue_int()
//	{
//		return strtol(getvalue(), NULL, 10);
//	}
//	const char * getkey()
//	{
//		return c_str();
//	}
//	HUMap& operator[](const char * _key)
//	{
//		return getsun(_key);
//	}
//
//	HUMap& operator[](int num)
//	{
//		return getboter(num);
//	}
//	int BrotherCount()
//	{
//		return brother.size();
//	}
//	int Count()
//	{
//		return m_mp.size();
//	}
//	int retrospect_tree()
//	{ //�������е�λ��
//		int i = 0;
//		HUMap * p = this;
//		while (1)
//		{
//			if (p->father == NULL)
//			{
//				break;
//			}
//			i++;
//			p = p->father;
//		}
//		return i;
//	}
//
//	void display()
//	{
//		int dispspace = retrospect_tree();
//
////		displayspace(dispspace);
////		printf("---> %s\r\n", getkey());
//
//		displayspace(dispspace);
//		printf("%s=%s\r\n", getkey(), getvalue());
//
//		map<hustr, HUMap>::iterator it;
//		for (it = m_mp.begin(); it != m_mp.end(); it++)
//		{
//			it->second.display();
//		}
//
//		if (BrotherCount() > 0)
//		{
//
//			for (int i = 1; i < BrotherCount(); i++)
//			{
//				brother[i].display();
//			}
//		}
//
////		displayspace(dispspace);
////		printf("<--- %s\r\n", getkey());
//	}
//	HUMap & CreateNew(const char * key)
//	{
//		int cnt = getsun(key).BrotherCount();
//		return m_mp[key][cnt];
//	}
//	HUMap()
//	{
//		father = NULL;
//	}
//	~HUMap()
//	{
//		m_mp.clear();
//		brother.clear();
//	}
//
//	HUMap & fetch(HUMap & mp)
//	{
//		m_mp = mp.m_mp;
//		//brother = mp.brother;
//		m_value = mp.m_value;
//		assign(mp.getkey());
//		father = NULL;
//		setfather();
//	}
//
//	typedef map<hustr, HUMap>::iterator iterator;
//	iterator begin()
//	{
//		return m_mp.begin();
//	}
//	iterator end()
//	{
//		return m_mp.end();
//	}
//
//	hustr m_value;
//	HUMap * father;
//	map<int, HUMap> brother;
//	map<hustr, HUMap> m_mp;
//
//	//map< hustr,map< int,XMLMap > > test;
//private:
//	void setfather()
//	{
//		if (BrotherCount() > 0)
//		{
//			for (int i = 1; i < BrotherCount(); i++)
//			{
//				brother[i].father = this->father;
//				brother[i].setfather();
//			}
//		}
//		map<hustr, HUMap>::iterator it;
//		for (it = m_mp.begin(); it != m_mp.end(); it++)
//		{
//			it->second.father = this;
//			it->second.setfather();
//
//		}
//
//	}
//
//	void displayspace(int n, const char * str = "     ")
//	{
//		for (int i = 0; i < n; i++)
//			printf(str);
//	}
//};
//

//class xmlproc;
//class HuMsg
//{
//public:
//	HuMsg()
//	{
//		xml = NULL;
//	}
//	hustr name;
//	XMLMap xmlmp;
//	xmlproc * xml;
//};
//
//typedef HuMsg* AutoMsg;
//
//
//
//extern queue<AutoMsg> g_msg_quque;

//class XMLelement
//{
//public:
//	//virtual void SetConfig(const char * type, const char * key,const char * value)=0;
//	virtual void SetConfig(hustr type, XMLAttribute & attr) =0;
//	virtual void FlushConfig()= 0;
//
//};

//class XMLelement_manager
//{
//public:
//	virtual void AddXMLElement(const char * name, XMLelement * ele)
//	{
//		xmlelem[name] = ele;
//
//	}
//
//	void FlushConfig()
//	{
//		map<hustr, XMLelement *>::iterator it;
//		for (it = xmlelem.begin(); it != xmlelem.end(); ++it)
//		{
//			//printf("XMLelement_manager::FlushConfig %s\r\n", it->first.c_str());
//			//it->fi->FlushConfig();
//			it->second->FlushConfig();
//
//			//printf("XMLelement_manager::FlushConfig %s end\r\n", it->first.c_str());
//		}
//		//printf("$$$HU$$$ FlushConfig end\r\n");
//	}
//
//	void SetConfig(const char * name, const char * type, XMLAttribute &attr)
//	{
//		int i = 0;
//		if (xmlelem[name] != NULL)
//		{
//			xmlelem[name]->SetConfig(type, attr);
//		}
//	}
//
//	XMLelement * GetXMLElementByName(const char * name)
//	{
//		map<hustr, XMLelement *>::iterator it;
//		it = xmlelem.find(name);
//		if (it != xmlelem.end())
//		{
//			return it->second;
//		}
//		else
//		{
//			return NULL;
//		}
//	}
//
//	map<hustr, XMLelement *> xmlelem;
//};

//void ParaseUpdateXml(TiXmlNode* pParent, int l);
//void ParaseTinyXmlFile(const char * filename);

#endif //__PARASE_XML_H__