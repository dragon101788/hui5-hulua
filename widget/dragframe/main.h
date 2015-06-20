#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"

class dragframe: public element , public touch_element ,public touch_manager
{
public:
	dragframe()
	{
		id = 0;
		x_pos=0;
		y_pos=0;
	}
	~dragframe()
	{

	}

	void doTouchDown()
        {
                //printf("$$$HU$$$ %s %s tx=%d ty=%d t%d b%d l%d r%d\r\n",name,__FUNCTION__,tx,ty,top,bottom,left,right);

	        //printf("tx=%d ty=%d\n",GetTouchX(),GetTouchY());


	         touch_sample tp;
	         tp.x = GetTouchX()-x_pos;
	         tp.y = GetTouchY()-y_pos;
	         tp.pressure = GetTouchP();
	         touch_proc_event(&tp);

                //printf("%s touch\r\n", name.c_str());

                Flush();
        }

        void doTouchUp()
        {

                touch_sample tp;
                 tp.x = GetTouchX()-x_pos;
                 tp.y = GetTouchY()-y_pos;
                 tp.pressure = GetTouchP();
                 touch_proc_event(&tp);
                //printf("%s free\r\n", name.c_str());
                 x_pos+=move_x();
                 y_pos+=move_y();

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

	     //mp.display();

//	     if(mp.exist("linit"))
//             {
//                 printf("exec linit %s\n",mp["linit"]->getvalue());
//                 lua.dostring(mp["linit"]->getvalue());
//             }


	    res[0].SetBuffer(GetWidth(),GetHeight());

            HUMap::OrderList lst;
            lst.accept(mp);
            for(HUMap::OrderList::iterator it = lst.begin();it!=lst.end();++it)
            {
                hustr name = (*it).m_key;
                SParseXMLFrom_Instan(name, *it);
            }

            //lua.dostring(mp.m_val);

	}
	void doRender()
	{
	        res[0].RenderTo(this, 0, 0, GetWidth(), GetHeight(), x_pos+move_x(), y_pos+move_y());
	}
	virtual void RenderFrom(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
	{
	    res[0].AreaCopyFrom(src_img,src_x,src_y,cp_width,cp_height,dst_x,dst_y);
	}
	int id;
	int x_pos;
	int y_pos;
};

#endif //__STATIC_IMAGE_H__
