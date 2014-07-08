#ifndef __BUTTON_IMAGE_H__
#define __BUTTON_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"
#include "thread_msg.h"

class button: public element, public touch_element
{
public:
	void doTouchDown()
	{
		//printf("$$$HU$$$ %s %s tx=%d ty=%d t%d b%d l%d r%d\r\n",name,__FUNCTION__,tx,ty,top,bottom,left,right);
		if (isdn == 1)
			return;

		//printf("%s touch\r\n", name.c_str());

		Flush();
	}

	void doTouchUp()
	{
		if (isdn == 0)
			return;

		//printf("%s free\r\n", name.c_str());

		Flush();
	}

	void doTouchActive()
	{
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

	button()
	{
	}

	HuExec exec;
	//hustr cmd;
};

#endif //__STATIC_IMAGE_H__
