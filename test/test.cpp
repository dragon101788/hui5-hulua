#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <deque>
#include <vector>
#include <algorithm>
#include <string.h>
#include <stdarg.h>

using namespace std;

void errexit(const char * str)
{
	printf("**************error exit*****************\r\n");
	printf("%s\r\n", str);
	printf("*****************************************\r\n");
	exit(-1);
}
#define errexitf(str,...) errexit(hustr(str, ##__VA_ARGS__ ))

class hustr: public string
{
public:
	hustr()
	{

	}

//	hustr(const char * str)
//	{
//		assign(str);
//	}
	void Replace(const char * rst, const char * dst)
	{
		replace(find(rst), strlen(rst), dst, strlen(dst));
	}

	hustr str_key(const char * str = "=")
	{
		int len = length() + 1;
		char * tmp = (char *) alloca(len);
		memset(tmp, 0, len);
		strcpy(tmp, c_str());
		char * p = strstr(tmp, str);
		*p = 0;
		char * key = tmp;
		char * value = p + 1;
		return key;
	}
	hustr str_value(const char * str = "=")
	{
		int len = length() + 1;
		char * tmp = (char *) alloca(len);
		memset(tmp, 0, len);
		strcpy(tmp, c_str());
		char * p = strstr(tmp, str);
		*p = 0;
		char * key = tmp;
		char * value = p + 1;
		return value;
	}

	hustr(const char * fmt, ...)
	{
		char buffer[256];
		if (fmt == NULL)
		{
			return;
		}
		va_list argptr;
		int cnt;

		va_start(argptr, fmt);
		cnt = vsprintf(buffer, fmt, argptr);
		va_end(argptr);

		assign(buffer);

	}

	int format(const char * fmt, ...)
	{
		char buffer[256];
		if (fmt == NULL)
		{
			errexitf("hustr format error\r\n");
		}
		va_list argptr;
		int cnt;
		va_start(argptr, fmt);
		cnt = vsprintf(buffer, fmt, argptr);
		va_end(argptr);

		assign(buffer);

		return (cnt);
	}

	const char * nstr()
	{
		if (empty())
		{
			return NULL;
		}
		else
			return c_str();
	}
	operator const char *()
	{
		return c_str();
	}

};

//template<typename T>
//class HUMap
//{
//	HUMap & operator=(HUMap & mp)
//	{
//	}
////	HUMap(HUMap & mp)
////	{
////	}
//public:
//	class mapkey
//	{
//	public:
//		int m_id;
//		char str[256];
//		mapkey(const char * key, int cnt)
//		{
//			memset(str, 0, 256);
//			strcpy(str, key);
//			m_id = cnt;
//		}
////		mapkey(const char * fmt, ...)
////		{
////			char buffer[256];
////			if (fmt == NULL)
////			{
////				return;
////			}
////			va_list argptr;
////			va_start(argptr, fmt);
////			int n = vsprintf(buffer, fmt, argptr);
////			va_end(argptr);
////
////			assign(buffer);
////
////		}
//	};
//	class Compare
//	{
//	public:
//		bool operator ()(const mapkey & a, const mapkey &b) const
//		{
//			if (strcmp(a.str, b.str) < 0)
//			{
//				return 1;
//			}
//			else
//			{
//				return a.m_id < b.m_id;
//			}
//
//		}
//	};
//
//	typedef typename map<mapkey, HUMap>::iterator iterator;
//	//typedef pair<mapkey, HuMapBrother> pair;
//	HUMap(const char * key, HUMap * father)
//	{
//		m_key = key;
//		m_father = father;
//	}
//	void fetch(HUMap & mp)
//	{
//		m_mp = mp.m_mp;
//		m_key = mp.m_key;
//		m_val = mp.m_val;
//	}
//	T & operator=(const char * _value)
//	{
//		m_val = _value;
//	}
//
//	HUMap & GetSun(mapkey key)
//	{
//		iterator it = find(key);
//		if (it == this->end())
//		{ //new
//			this->insert(pair<mapkey, HUMap>(key, HUMap(key.str, this)));
//		}
//		return this->at(key);
//	}
//	HUMap & CreateMultiLast(const char * key)
//	{
//		return GetSun(key, 0).CreateMultiLast();
//	}
//	HUMap & operator[](mapkey key)
//	{
//		return GetSun(key);
//	}
//	HUMap & operator[](const char * key)
//	{
//		return GetSun(mapkey(key, 0));
//	}
//	iterator find(mapkey & key)
//	{
//		return this->find(key);
//	}
//	iterator end()
//	{
//		return this->end();
//	}
//	iterator begin()
//	{
//		return this->begin();
//	}
//	void display()
//	{
//		displayspace(retrospect_tree());
//		printf("[%s]=[%s]\r\n", m_key.c_str(), m_val.c_str());
//
//		iterator it;
//		for (it = this->begin(); it != this->end(); ++it)
//		{
//			//printf("HUMap:: display%x\r\n", &(it->second));
//			it->second.display();
//		}
//	}
//	const char * getkey()
//	{
//		return m_key.c_str();
//	}
//	const char * getvalue()
//	{
//		const char * tmp = m_val.c_str();
////		if (tmp[0] == '$')
////		{
////			const char * name = tmp + 1;
////			if (g_var.getvar(name) != NULL)
////			{
////				tmp = g_var.getvar(name);
////			}
////			else
////			{
////				tmp = getenv(name);
////			}
////
////			if (tmp == NULL)
////			{
////				errexitf("$$$HU$$$$ Xml file Variable parameter [%s] undefined!!!\r\n", name);
////			}
////		}
//		return tmp;
//	}
//	int getvalue_int()
//	{
//		return strtol(getvalue(), NULL, 10);
//	}
//	void remove(const char * key)
//	{
//		m_mp.erase(key);
//	}
//	int exist(const char * key)
//	{
//		iterator it = m_mp.find(key);
//		if (it == m_mp.end())
//		{
//			return 0;
//		}
//		else
//		{
//			return 1;
//		}
//	}
//
//private:
//	int retrospect_tree()
//	{
//		int i = 0;
//		HUMap * p = this;
//		while (p->m_father != NULL)
//		{
//			i++;
//			p = p->m_father;
//		}
//		return i;
//	}
//
//	void displayspace(int n, const char * str = "     ")
//	{
//		for (int i = 0; i < n; i++)
//			printf("%s", str);
//	}
//
//	hustr m_key;
//	T m_val;
//	map<mapkey, HUMap, Compare> m_mp;
//	HUMap * m_father;
//	int m_bro_id;
//};

//template<typename T>
//class humap :multimap<hustr, humap<T> >
//{
//public:
//	humap(const char * str, humap * f)
//	{
//		m_father = f;
//		m_key = str;
//	}
//	typedef typename multimap<hustr, humap>::iterator iterator;
//	hustr m_key;
//	T m_val;
//	humap * m_father;
//	// m_mp;
//
//	class Iterator
//	{
//	public:
//		iterator m_it;
//		humap * m_mgr;
//		Iterator(iterator it, humap * mgr)
//		{
//			m_mgr = mgr;
//			m_it = it;
//		}
//		hustr & operator=(const char * key)
//		{
//			(*m_it).second.m_val = key;
//			//return m_val = key;
//		}
//		Iterator operator[](const char * key)
//		{
//			return (*m_it).second.Get(key);
//		}
//		Iterator operator[](int id)
//		{
//			return m_mgr->Get((*m_it).first.c_str(), id);
//		}
//		Iterator Multi(const char * value)
//		{
//			const char * key = (*m_it).first.c_str();
//			Iterator it(m_mgr->insert(pair<hustr, humap>(key, humap(key, m_mgr))), m_mgr);
//			it = value;
//			return it;
//		}
//	};
//
//	Iterator operator[](const char * key)
//	{
//		return Get(key);
//	}
//	Iterator Get(const char * key)
//	{
//		iterator it = this->find(key);
//		if (it != this->end())
//		{
//			return Iterator(it, this);
//		}
//		else
//		{
//			return Iterator(this->insert(pair<hustr, humap>(key, humap(key, this))), this);
//		}
//	}
//	Iterator Get(const char * key, int id)
//	{
//
//		iterator it = this->find(key);
//		if (it != this->end())
//		{
//			int count = this->count(key);
//			printf("count=%d id=%d\r\n", count, id);
//			int i;
//			for (i = 0; i < count; ++i)
//			{
//				if (i == id)
//				{
//					printf("find %s %d\r\n", key, i);
//					return Iterator(it, this);
//				}
//				++it;
//			}
//			if (i >= count)
//			{
//				printf("key %s id %d not find\r\n", key, i);
//				exit(-1);
//			}
//		}
//		else
//		{
//			return Iterator(this->insert(pair<hustr, humap>(key, humap(key, this))), this);
//		}
//	}
//	void display()
//	{
//		displayspace(retrospect_tree());
//		printf("[%s]=[%s]\r\n", m_key.c_str(), m_val.c_str());
//		iterator it;
//		for (it = this->begin(); it != this->end(); ++it)	//遍历
//		{
//			(*it).second.display();
//		}
//	}
//
//	int retrospect_tree()
//	{
//		int i = 0;
//		humap * p = this;
//		while (p->m_father != NULL)
//		{
//			i++;
//			p = p->m_father;
//		}
//		return i;
//	}
//
//	void displayspace(int n, const char * str = "     ")
//	{
//		for (int i = 0; i < n; i++)
//			printf("%s", str);
//	}
//
//};

template<typename T>
class humap: public multimap<hustr, humap<T> >
{
public:
	humap(const char * str = NULL, humap * f = NULL, int o = 0)
	{
		m_order = o;
		m_father = f;
		m_key = str;
	}
	typedef typename multimap<hustr, humap>::iterator iterator_base;
	hustr m_key;
	T m_val;
	humap * m_father;
	int m_order;

	class iterator: public iterator_base
	{
	public:
		//iterator m_it;
		humap * m_mgr;
		iterator()
		{
			//(*this) = (*it);
			m_mgr = NULL;
			//m_it = it;
		}
		iterator(iterator_base it) :
				iterator_base(it)
		{
			//(*this) = (*it);
			m_mgr = NULL;
			//m_it = it;
		}
		iterator(iterator_base it, humap * mgr) :
				iterator_base(it)
		{
			//(*this) = (*it);
			m_mgr = mgr;
			//m_it = it;
		}
		hustr & operator=(const char * key)
		{
			(**this).second.m_val = key;
			//return m_val = key;
		}
		iterator operator[](const char * key)
		{
			return (**this).second.Get(key);
		}
		iterator operator[](int id)
		{
			return m_mgr->Get((**this).first.c_str(), id);
		}
		operator humap &()
		{
			return (**this).second;
		}
		operator humap *()
		{
			return &(**this).second;
		}
		T * operator->()
		{
			return &(**this).second.m_val;
		}
	};

	class OrderList: vector<humap>
	{
	public:
		typedef typename vector<humap<hustr> >::iterator iterator;
		static int cmp(const humap & a, const humap & b)
		{
			return a.m_order < b.m_order;
		}
		void display()
		{
			for (iterator it = this->begin(); it != this->end(); it++)
			{
				//printf("%s %s\r\n", (*it).m_key.c_str(), (*it).m_val.c_str());
				(*it).display();
			}
		}
		OrderList(humap & mp)
		{
			for (humap::iterator it = mp.begin(); it != mp.end(); it++)
			{
				this->push_back((*it).second);
			}
			sort(this->begin(), this->end(), cmp);
		}
	};
	int exist(const char * key)
	{
		if (this->find(key) != this->end())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	void fetch(humap & mp)
	{
		*this = mp;
	}
	iterator operator[](const char * key)
	{
		return Get(key);
	}
	iterator CreateMultiLast(const char * key)
	{
		//printf("CreateMultiLast [%s]\r\n", key);
		return iterator(this->insert(pair<hustr, humap>(key, humap(key, this, this->size()))), this);
	}
	iterator Get(const char * key)
	{
		iterator it = this->find(key);
		if (it != this->end())
		{
			return iterator(it, this);
		}
		else
		{
			return iterator(this->insert(pair<hustr, humap>(key, humap(key, this, this->size()))), this);
		}
	}
	iterator Get(const char * key, int id)
	{

		iterator it = this->find(key);
		if (it != this->end())
		{
			int count = this->count(key);
			//printf("count=%d id=%d\r\n", count, id);
			int i;
			for (i = 0; i < count; ++i)
			{
				if (i == id)
				{
					//printf("find %s %d\r\n", key, i);
					return iterator(it, this);
				}
				++it;
			}
			if (i >= count)
			{
				printf("key %s id %d not find\r\n", key, i);
				exit(-1);
			}
		}
		else
		{
			return iterator(this->insert(pair<hustr, humap>(key, humap(key, this, this->size()))), this);
		}
	}
	void display()
	{
		displayspace(retrospect_tree());
		printf("[%s %d]=[%s]\r\n", m_key.c_str(), m_order, m_val.c_str());
		iterator it;
		for (it = this->begin(); it != this->end(); ++it)	//遍历
		{
			(*it).second.display();
		}
	}

	int retrospect_tree()
	{
		int i = 0;
		humap * p = this;
		while (p->m_father != NULL)
		{
			i++;
			p = p->m_father;
		}
		return i;
	}

	void displayspace(int n, const char * str = "     ")
	{
		for (int i = 0; i < n; i++)
			printf("%s", str);
	}
	void setfather(humap * father)
	{
		m_father = father;
		iterator it;
		for (it = this->begin(); it != this->end(); ++it)
		{
			it->second.setfather(this);
		}

	}
};

int cmp(const humap<hustr> & a, const humap<hustr> & b)
{
	return a.m_order < b.m_order;
}

class abc: public hustr
{
public:
	abc(const char * fmt, ...)
	{
		char buffer[256];
		if (fmt == NULL)
		{
			return;
		}
		va_list argptr;
		int cnt;

		va_start(argptr, fmt);
		cnt = vsprintf(buffer, fmt, argptr);
		va_end(argptr);

		assign(buffer);

	}
	int a;
	int b;
	int c;
};
int main(int argc, char * argv[])
{
//	HUMap<hustr> mp("newmp", NULL);
	//HuMapOld mp("oldmp");
//	mp["b1"] = "1";
//	mp["b1"] = "1.1";
//
//	mp["b3"] = "3";
//	mp["b2"] = "2";
//	mp["a1"][0] = "a1.0";
//	mp["a1"][1] = "a1.1";
//	mp["a1"][2] = "a1.2";
//	mp[HUMap<hustr>::mapkey("b2", 0)][HUMap<hustr>::mapkey("c1", 0)] = "c1.1";
//	mp["c1"]["c1-2"] = "c1.2";
//	mp["c1"]["c1-3"] = "c1.3";
//	mp["c2"]["c2-1"]["c2-1-1"] = "c1.1";
//	mp["c2"]["c2-2"]["c2-1-2"] = "c1.1";
//	mp["c2"]["c2-3"]["c2-1-3"] = "c1.1";
//	mp["c3"]["c3-1"]["c3-1-1"]["c3-1-1-1"] = "c1.1";
//	mp["c3"]["c3-2"]["c3-1-2"]["c3-1-1-2"] = "c1.1";
//	mp["c3"]["c3-3"]["c3-1-3"]["c3-1-1-3"] = "c1.1";
//	mp["c1"]["c1-3"].n = 50;
//	mp["c1"]["c1-3"].b = 100;
//	mp["c1"]["c1-3"].abc = "abcdefg";
//	mp.display();

	humap<abc> mp("root", NULL);
	mp["z1"] = "a";
	mp["e1"] = "a";
	mp["d1"] = "a";
	mp["c1"] = "a";
	mp["a1"] = "a";
	mp["b1"]->a = 5;
	mp["b1"] = "a";
	mp["b1"]["b1.1"] = "a1";
	mp.CreateMultiLast("c1") = "abc1";
	mp.CreateMultiLast("c1") = "abc2";
	mp.CreateMultiLast("c1")[1] = "abc3";
	mp.CreateMultiLast("c1") = "abc4";
	//mp["c1"] = "z.1";
	//mp["c1"] = "a.3";
	//mp["c1"][1] = "a.2";
	mp["c2"] = "b";
	mp["c3"] = "c";
	mp["c2"]["t1"] = "d";
	mp["c1"]["c1-2"] = "c1.2";
	mp["c1"][2]["c1-3"] = "c1.3";
	mp["c2"]["c2-1"]["c2-1-1"] = "c1.1";
	mp["c2"]["c2-2"]["c2-1-2"] = "c1.1";
	mp["c2"]["c2-3"]["c2-1-3"] = "c1.1";
	mp["c3"]["c3-1"]["c3-1-1"]["c3-1-1-1"] = "c1.1";
	mp["c3"]["c3-2"]["c3-1-2"]["c3-1-1-2"] = "c1.1";
	mp["c3"]["c3-3"]["c3-1-3"]["c3-1-1-3"] = "c1.1";
	mp.display();
	printf("******************\r\n");

//	vector<humap<hustr> > s;
//	for (humap<hustr>::iterator it = mp.begin(); it != mp.end(); it++)
//	{
//		s.push_back((*it).second);
//	}
//	sort(s.begin(), s.end(), cmp);
//
//	for (vector<humap<hustr> >::iterator it = s.begin(); it != s.end(); it++)
//	{
//		printf("%s %s\r\n", (*it).m_key.c_str(), (*it).m_val.c_str());
//	}
	humap<hustr>::OrderList olst(mp);
	olst.display();

	printf("******************\r\n");
	printf("[%s]\r\n", mp["c1"]->c_str());
//	multimap<hustr, hustr> mp;
//	mp.insert(pair<hustr, hustr>("Jack", "abc"));
//	mp.insert(pair<hustr, hustr>("Jack", "def"));
//	mp.insert(pair<hustr, hustr>("Jack", "aaa"));
//	mp.insert(pair<hustr, hustr>("Jack", "bbb"));
//
//	mp.insert(pair<hustr, hustr>("hxp", "abc"));
//	mp.insert(pair<hustr, hustr>("hxp", "def"));
//	mp.insert(pair<hustr, hustr>("hxp", "aaa"));
//	mp.insert(pair<hustr, hustr>("hxp", "bbb"));
//
//	multimap<hustr, hustr>::iterator it;
//	for (it = mp.find("Jack"); it != mp.end(); ++it)	//遍历
//	{
//		printf("%s %s\r\n", (*it).first.c_str(), (*it).second.c_str());
//	}

	//printf("%d %s %d\r\n", mp["c1"]["c1-3"].n, mp["c1"]["c1-3"].abc, mp["c1"]["c1-3"].b);
	return 0;
}
