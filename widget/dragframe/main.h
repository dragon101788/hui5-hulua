#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"

class dragframe: public element , public touch_element ,public touch_manager
{
public:
	dragframe()
	{
		xpos = 0;
		ypos = 0;
		id = 0;
	}
	~dragframe()
	{

	}

	int GetX() const
	{
	   //printf("dragframe GetX %d\n",m_x + move_x());
	   return m_x + move_x();
	}
	int GetY() const
        {
	   //printf("dragframe GetY %d\n",m_y + move_y());
           return m_y + move_y();
        }
	void doTouchDown()
        {
                //printf("$$$HU$$$ %s %s tx=%d ty=%d t%d b%d l%d r%d\r\n",name,__FUNCTION__,tx,ty,top,bottom,left,right);

	        //printf("tx=%d ty=%d\n",GetTouchX(),GetTouchY());


	         touch_sample tp;
	         tp.x = GetTouchX();
	         tp.y = GetTouchY();
	         tp.pressure = GetTouchP();
	         touch_proc_event(&tp);
                //printf("%s touch\r\n", name.c_str());

                Flush();
        }

        void doTouchUp()
        {

                touch_sample tp;
                 tp.x = GetTouchX();
                 tp.y = GetTouchY();
                 tp.pressure = GetTouchP();
                 touch_proc_event(&tp);
                //printf("%s free\r\n", name.c_str());

                Flush();
        }

        void doTouchActive()
        {

        }
	void LuaCommand(const char * cmd,const char * arg)
	{

	}
	void doDelete()
	{

	}
	int SParseXMLFrom_Instan(hustr name, HUMap & xmlmp)
	{
	        log_i("$$$HU$$$ Parse [%s]\r\n", name.c_str());
	        //xmlmp.display();
	        XMLinstan_tf fun = XMLinstan[name];
	        if (fun != NULL)
	        {
	                //xml->done = 0;
	                xmlmp["parent"].value().m_data = (element *)this;
	                xmlmp["touch_mgr"].value().m_data = (touch_manager *)this;
	                printf("$$$$HU$$$$ [%s] sub element [%s]\n",GetName(),xmlmp["name"]->getvalue());
	                fun(xmlmp, xml_mgr);
	                //xml->done = 1;
	        }
	}
	void doFlushConfig(HUMap & mp)
	{

//	    touch_init_area(GetX(), GetY(), GetWidth(), GetHeight());
//	            xml_mgr->AddEleArea(this);

            HUMap::OrderList lst;
            lst.accept(mp);
            for(HUMap::OrderList::iterator it = lst.begin();it!=lst.end();++it)
            {
                hustr name = (*it).m_key;
                SParseXMLFrom_Instan(name, *it);
            }


	}
	void doRender()
	{
		//image::Render(&res[id], xpos, ypos, GetWidth(), GetHeight(), 0, 0);
	}
	int xpos;
	int ypos;

	int id;
};

#endif //__STATIC_IMAGE_H__
