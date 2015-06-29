#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"

class dragframe: public element , public touch_element ,public element_manager,public touch_manager
{
public:
        static void lua_instal(lua_State* L)
        {
              element_base::lua_instal<dragframe>(L);
              element::lua_instal<dragframe>(L);
              touch_element::lua_instal<dragframe>(L);
        }
	dragframe() :res(*GetRes("bk"))
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

        void doTouchMove()
        {
            Flush();
        }

        void doTouchActive()
        {

        }
	void doEleLuaCommand(const char * cmd)
	{
	    //printf("doLuaCommand [%s] %d\n",cmd,strncmp(cmd,"parsexml ",strlen("parsexml "));
            //if(strncmp(cmd,"parsexml ",strlen("parsexml "))==0&&(cmd = cmd+strlen("parsexml ")))
            if(luacmd_is("parsexml"))
            {
                HUMap mp;
                HumapFromXmlString(cmd,strlen(cmd),mp);
                //mp.display();
                ParseXML(mp);
            }
	}
	void doDelete()
	{

	}
	int SParseXMLFrom_Instan(hustr name, HUMap & xmlmp)
	{
	        lock();
	        res.lock();
	        log_i("$$$HU$$$ Parse [%s]\r\n", name.c_str());

	        //xmlmp.display();
	        XMLinstan_tf fun = XMLinstan[name];
	        if (fun != NULL)
	        {
	                //xml->done = 0;
	                xmlmp["parent"].value().m_data = (element *)this;
	                xmlmp["touch_mgr"].value().m_data = (touch_manager *)this;
	                xmlmp["element_manager"].value().m_data = (element_manager *)this;
	                int ex = xmlmp["x"]->getvalue_int();
	                int ey = xmlmp["y"]->getvalue_int();
	                int ewidth = xmlmp["width"]->getvalue_int();
	                int eheignt = xmlmp["height"]->getvalue_int();

	                if(ex+ewidth>res.GetImageWidth()||ey+eheignt>res.GetImageHeight())
                        {
	                    //printf("ReSetBuffer%d %d\n",ex+ewidth,ey+eheignt);
	                    res.ReSetBuffer(ex+ewidth>res.GetImageWidth()?ex+ewidth:res.GetImageWidth()
	                                      ,ey+eheignt>res.GetImageHeight()?ey+eheignt:res.GetImageHeight());
                        }
	                printf("$$$$HU$$$$ [%s] sub element [%s]\n",GetName(),xmlmp["name"]->getvalue());
	                fun(xmlmp, m_proc);
	                //xml->done = 1;
	        }
	        res.unlock();
	        unlock();
	}
	int ParseXML(HUMap & mp)
	{
	        lock();
	        HUMap::OrderList lst(mp);
                for(HUMap::OrderList::iterator it = lst.begin();it!=lst.end();++it)
                {
                    hustr name = (*it).MapName();
                    SParseXMLFrom_Instan(name, *it);
                }

                unlock();

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


	    res.SetBuffer(GetWidth(),GetHeight());
	    ParseXML(mp);

            //printf("lua=[%s]\n",mp.m_val.nstr());
            lua.dostring(mp.MapValue());

	}
	void doRender()
	{
	    //printf("RenderFrom %d %d %d %d\n",GetWidth(), GetHeight(), x_pos+move_x(), y_pos+move_y());
	    //RenderFrom(&res,-(x_pos+move_x()), -(y_pos+move_y()), res.GetImageWidth(),res.GetImageHeight(), 0,0);
	    res.RenderTo(this, -(x_pos+move_x()), -(y_pos+move_y()), GetWidth(), GetHeight(), 0, 0);

	}

	//AreaCopyFrom 被外部绘图逻辑占用，此处不能继承
	virtual void RenderFrom(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
	{
	  static int b=0;
	   lock();
            res.lock();
            //printf("RenderFrom [%s] %d %d %d %d %d %d\n",src_img->path.nstr(),src_x,src_y,cp_width,cp_height,dst_x,dst_y);
            res.AreaCopyFrom(src_img,src_x,src_y,cp_width,cp_height,dst_x,dst_y);
            //res.SaveResource(hustr("res%d.png",b++));
            res.unlock();
	   unlock();
	   Flush();
	}
	int id;
	int x_pos;
	int y_pos;
	image & res;
};

#endif //__STATIC_IMAGE_H__
