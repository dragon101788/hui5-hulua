#ifndef __XML_INSTAL_H__
#define __XML_INSTAL_H__

#include "hulib.h"
#include "xmlproc.h"
#include <map>
using namespace std;

typedef void (*XMLinstan_tf)(HUMap & xmlmp, xmlproc * xml);
extern map<hustr, XMLinstan_tf> XMLinstan;

template<class T>
void Install_Element(HUMap &xmlmp, xmlproc * xml)
{
	//xml->mtx.lock();
	T * te = new T;
	//te->m_mp.fetch(xmlmp);
	te->xml_mgr = xml;
	te->mgr = xml;
	te->FlushConfig(xmlmp);
	//xml->mtx.unlock();
}

class InstallXMLinstan
{
public:
	InstallXMLinstan(const char * name, XMLinstan_tf fun)
	{
		XMLinstan[name] = fun;
	}
};

#define InstallXMLinstanWidget(a,b) static InstallXMLinstan install##_##b(a,Install_Element<b>);
#ifdef HUMODULE
#define hucall extern "C"
#else
#define hucall static
#endif

#endif //__XML_INSTAL_H__
