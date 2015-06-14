#ifndef __XML_INSTAL_H__
#define __XML_INSTAL_H__

#include "hulib.h"
#include "xmlproc.h"
#include "element_base.h"
#include <map>
using namespace std;

typedef void (*XMLinstan_tf)(HUMap & xmlmp, xmlproc * xml);
extern map<hustr, XMLinstan_tf> XMLinstan;



template<class T>
void Install_Element(HUMap &xmlmp, xmlproc * xml)
{
	//xml->mtx.lock();
	T * te = new T;
	if(xmlmp.exist("parent"))
        {
	    element * parent = (element *)xmlmp["parent"].value().m_data;
	    log_d("%s sub element\n",parent->GetName());
	    te->m_parent = parent;
        }

	te->xml_mgr = xml;
	printf("$$$dragon$$$ m_flag=%x\n",te->m_flag);
	if(te->m_flag&ELEMENT_FLAG_DRAWLOGIC)
        {
          te->FlushConfig(xmlmp);
        }

	if(te->m_flag&ELEMENT_FLAG_TOUCH)
        {
	    touch_element * touch_ele = (touch_element *)te;
	    touch_ele->touch_init_area();
	    touch_ele->TouchParaseXml(xmlmp);
	    xml->AddEleArea(touch_ele);
        }

	if(te->m_flag&ELEMENT_FLAG_TIMER)
        {
	    timer_element * timer_ele = (timer_element *)te;
	    timer_ele->TimerParaseXml(xmlmp);
	    xml->AddTimerElement(timer_ele);
        }
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
