#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"
#include "ttf_font.h"
#include "hui_hulua.h"

class static_text: public element
{
public:
	text ttf;

	static_text()
	{
		color = 0;
//		bkcolor=0;
		font = NULL;
		style = FONT_NORMAL;
		size = 16;
		hide = 0;
		lenth = 0;
	}
	~static_text()
	{
	}

	void test(const char * abc)
	{
	}
	void doDelete()
	{

	}
	void doGetInfo(info & info)
	{
		GetElementInfo(info);
		info.AddInfo("txt", txt);
		info.AddInfo("size", size);
		info.AddInfo("font", font);
	}
	void doFlushConfig()
	{
		PraseElement();

		//id = m_mp["id"]->getvalue_int();
		int red = m_mp["red"]->getvalue_int();
		int green = m_mp["green"]->getvalue_int();
		int blue = m_mp["blue"]->getvalue_int();
		color = (red & 0xff) << 16 | (green & 0xff) << 8 | blue & 0xff;
		//bkcolor = m_mp["bkcolor"]->getvalue_int();
		font = m_mp["font"]->getvalue();
		txt = m_mp["txt"]->getvalue();
		style = (unsigned char) m_mp["style"]->getvalue_int();
		size = m_mp["size"]->getvalue_int();

		ttf.m_font = &font_mp[font];
		printf("get font_mp %x %x\r\n", font_mp[font].face,
				font_mp[font].ft_Lib);
		ttf.fontHeight = size;
		ttf.color = color;
		ttf.style = style;
		ttf.SetBuffer(width, height);
		ttf.DrawText("UTF-8", (char *) txt.c_str(), txt.length());

		Flush();
	}
	void doRender()
	{
		image::Render(&ttf, 0, 0);
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
};

#endif //__STATIC_IMAGE_H__
