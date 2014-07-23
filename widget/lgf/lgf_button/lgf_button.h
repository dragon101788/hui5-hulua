#ifndef __BUTTON_IMAGE_H__
#define __BUTTON_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"
#include "thread_msg.h"

class lgf_button: public element, public touch_element
{
public:
	int id;
	int timer;
	void doTouchDown()
	{
		//printf("$$$HU$$$ %s %s tx=%d ty=%d t%d b%d l%d r%d\r\n",name,__FUNCTION__,tx,ty,top,bottom,left,right);
		if (isdn == 1)
			return;
			
	timer = GetTickCount();

	printf("down timer is %d \r\n", timer);

		Flush();
	}

	void doTouchUp()
	{
		if (isdn == 0)
			return;
	
			Flush();
		//	printf("up timer is %d \r\n", timer);
		//printf("%s free\r\n", name.c_str());
	}

	void doTouchActive()
	{
		if((GetTickCount()-timer)>3000)
			xml_mgr->AddExec(0, exec);
	}

	void doDelete()
	{
		xml_mgr->DelTouchElement(this);
	}
	
	void doFlushConfig()
	{
		PraseElement();
		exec.parse(m_mp);
		TouchParaseXml(m_mp);
		touch_init_area(x, y, width, height);

		SetRes(0, m_mp["up"]->getvalue());
		SetRes(1, m_mp["dn"]->getvalue());

		xml_mgr->AddEleArea(this);

		ParseModifRes();
		Flush();
	}
	
	void doRender()
	{
		image::Render(&res[isdn], 0, 0);
	}

	lgf_button()
	{
	}

	HuExec exec;
	//hustr cmd;
};

#endif //__STATIC_IMAGE_H__
