#include "XMLInstal.h"
#include "layer.h"
#include "thread_msg.h"

class lj_screensaver: public touch_element, public timer_element
{
public:
	lj_screensaver()
	{
	}
	void doTouchDown()
	{
		//printf("lj_screensaver doTouchDown\r\n");
		TimerSet(GetUpTimer() + ptimer);
	}

	void doTouchUp()
	{
		//printf("lj_screensaver doTouchUp\r\n");
		TimerSet(GetUpTimer() + ptimer);
	}

	void doTouchActive()
	{
		//printf("lj_screensaver doTouchActive\r\n");
		TimerSet(GetUpTimer() + ptimer);
	}
	int doTimer(int tm)
	{
		printf("tm_exec::doTimer!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%d\r\n",tm);
		//exe.start();
		m_xml->AddExec(0, exe);
		timer_stop = 1;
		return 0;
	}
	xmlproc * m_xml;
	HuExec exe;
	int ptimer;
} ele;

void ParseScreenSaver(HUMap &xmlmp, xmlproc * xml)
{

	printf("install lj_screensaver\r\n");
	ele.m_xml = xml;
	ele.touch_init_area(0,0,fb.u32Width,fb.u32Height);
	ele.ptimer = xmlmp["ptimer"]->getvalue_int();
	ele.exe.parse(xmlmp);
	ele.TimerSet(xml->GetUpTimer() + ele.ptimer);
	ele.TimerParaseXml(xmlmp);
	ele.TouchParaseXml(xmlmp);

	xml->AddTimerElement(&ele);
	xml->AddEleArea(&ele);

}
//XMLinstan["drag"] = Install_Element<drag>;
static InstallXMLinstan install("lj_screensaver", ParseScreenSaver);

hucall int init(HUMap & mp, void * data)
{
	printf("module init\r\n");
}

