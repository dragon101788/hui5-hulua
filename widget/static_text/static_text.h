#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "drawlogic.h"
#include "ttf_font.h"
#include "hui_hulua.h"

class static_text: public element
{
public:
        static void lua_instal(lua_State* L)
        {
              element_base::lua_instal<static_text>(L);
              element::lua_instal<static_text>(L);
        }

	text ttf;

	static_text()
	{
		color = 0;
//		bkcolor=0;
		font = NULL;
		style = FONT_NORMAL;
		size = 16;
		lenth = 0;
		align_center=0;
	}
	~static_text()
	{
	}
	void doEleLuaCommand(const char * cmd)
	{
	   if(luacmd_is("set_text"))
           {
               txt = cmd;
               ttf.m_font = &font_mp[font];
               log_d("get font_mp %x %x\r\n", font_mp[font].face,
                               font_mp[font].ft_Lib);
               ttf.fontHeight = size;
               ttf.color = color;
               ttf.style = style;
               ttf.SetBuffer(GetWidth(), GetHeight());
               ttf.drawText( (char *) txt.c_str(), txt.length());

               Flush();
           }

	}
	void test(const char * abc)
	{
	}
	void doDelete()
	{

	}
//	void doGetInfo(info & info)
//	{
//		GetElementInfo(info);
//		info.AddInfo("txt", txt);
//		info.AddInfo("size", size);
//		info.AddInfo("font", font);
//	}
	void doFlushConfig(HUMap & mp)
	{

		//id = m_mp["id"]->getvalue_int();
		int red = mp["red"]->getvalue_int();
		int green = mp["green"]->getvalue_int();
		int blue = mp["blue"]->getvalue_int();
		color = (red & 0xff) << 16 | (green & 0xff) << 8 | blue & 0xff;
		/**
		 * padding_left:左边留白距离
		 * padding_top:顶部留白距离
		 * align_center：是否中心对齐
		 */
		int padding_left=mp["padding_left"]->getvalue_int();
		int padding_top=mp["padding_top"]->getvalue_int();
		if (mp.exist("align_center"))
		align_center=mp["align_center"]->getvalue_int();
		//bkcolor = m_mp["bkcolor"]->getvalue_int();
		font = mp["font"]->getvalue();
		txt = mp["txt"]->getvalue();
		style = (unsigned char) mp["style"]->getvalue_int();
		size = mp["size"]->getvalue_int();

		ttf.m_font = &font_mp[font];
		log_d("get font_mp %x %x\r\n", font_mp[font].face,
				font_mp[font].ft_Lib);
		ttf.fontHeight = size;
		ttf.color = color;
		ttf.style = style;
		ttf.SetBuffer(GetWidth(), GetHeight());
		if(align_center){
			padding_left+=GetWidth()/2-(txt.length())*size/4;
			padding_left>0?padding_left:0;
			padding_top+=(GetHeight()-size)/2;
			padding_top>0?padding_top:0;
			}

		ttf.drawText( (char *) txt.c_str(), txt.length(),padding_left,padding_top);

	}
	void doRender()
	{
		image::RenderFrom(&ttf, 0, 0);
		//	image::Render(&img, 0, 0, width, height, 0, 0);
	}
//	int xpos;
//	int ypos;
	//text * pfont;
	map<hustr, text>::iterator font_it;
	unsigned int color;
	//unsigned int bkcolor;
	hustr font;	//保存路径
	hustr txt;	//i
	//wchar_t *mytxt;
	unsigned char style;
	int size;
	//int id;
	int lenth;
	int buf_len;
	int align_center;
};

#endif //__STATIC_IMAGE_H__
