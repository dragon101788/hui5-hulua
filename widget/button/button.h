#ifndef __BUTTON_IMAGE_H__
#define __BUTTON_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"

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
		exec.doStart();
	}

	void doDelete()
	{
		touch_mgr->DelTouchElement(this);
	}
	void doFlushConfig(HUMap &mp)
	{
		exec.parse(mp);
//		touch_init_area(GetX(), GetY(), GetWidth(), GetHeight());

		SetRes(0, mp["up"]->getvalue());
		SetRes(1, mp["dn"]->getvalue());
		//xml_mgr->AddEleArea(this);

	}
	void doRender()
	{
		image::RenderFrom(&res[isdn], 0, 0);
	}

	button()
	{
	}

	HuExec exec;
	//hustr cmd;
};

#endif //__STATIC_IMAGE_H__
