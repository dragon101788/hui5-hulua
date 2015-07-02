//#include "trueType/ft2build.h"
//#include "trueType/freetype.h"
//#include "trueType/ftoutln.h"
//#include "trueType/ftsynth.h"

#include "ttf_font.h"
//#include "gb_unicode.h"

//#include"w55fa92_blt.h"
#include <linux/fb.h> 
#include <fcntl.h> 
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string>
#include "XMLInstal.h"
#include "xmlproc.h"
using namespace std;

extern char *ttfAddrBase;
extern unsigned long ttfSize;


unsigned int FontDev::drawText(text * ptext, char * showtxt,unsigned int txt_len){
	return displayUTF8(ptext, showtxt, txt_len, ptext->color, ptext->style);
}
unsigned int FontDev::drawText(text * ptext, char * showtxt,unsigned int txt_len,int padding_left,int padding_top){
	return displayUTF8(ptext, showtxt, txt_len, ptext->color, ptext->style,padding_left,padding_top);
}


unsigned int FontDev::displayUTF8(text * ptext, const char *text, int num, unsigned int color, unsigned char style)
{

	//int final_num=0;
	if (face == NULL)
	{
		errexitf("font not initialize %s\r\n", text);
	}
	int i;
	U16 fontCode;
	FT_GlyphSlot slot = face->glyph;
	//int max_h = face->size->metrics.ascender  >> 6;   // 基线到字符轮廓最高点的距离
	int x = 2;//给点余量吧
	int y = 0;

	log_i("TTF_DisplayUTF8!,num=%d\n", num);
	//log_i("ptext->GetImageWidth()=%d ,ptext->GetImageHeight()=%d\n",ptext->GetImageWidth(),ptext->GetImageHeight());
	lock();
	setPixelSize(ptext->fontWidth, ptext->fontHeight);
	 const char *start=text;
	 const char* stop = text + num;
	 const char* old;
	 int step;
	    while (text < stop)
	    {
			if (style & FONT_BOLD)
			{
				FT_Pos strength = (1 << 6);
				FT_Outline_Embolden(&face->glyph->outline, strength);
			}

			if (style & FONT_ITALIC) //
			{
				FT_Set_Transform(face, &matrix, &pen);
			}
			old=text;
			fontCode= UTF8_NextUnichar(&text);
			step=text-old;
			if(fontCode==0){
				text-=step;
				break;
			}
		ft_error = FT_Load_Char(face, fontCode, TTF_bitmap_type); /*  FT_LOAD_NO_BITMAP | FT_LOAD_RENDER */
		if (ft_error)
		{
			log_e("Error at load char!\n");
			unlock();
			return -1;
		}

		if (style & FONT_BOLD)
		{
			FT_GlyphSlot_Embolden(slot);
		}
		//ptext->fontHeight for prevent overflow
		//final_num++;//显示文字数加一
		int word_w=slot->advance.x >> 6;
		if ((x + slot->bitmap_left+ (word_w) ) > ptext->GetImageWidth())
		{
			if ((y + ptext->fontHeight *  2.4) < ptext->GetImageHeight())//换行
			{
				x = 0;
				y +=ptext->fontHeight*1.2;
				//y +=ptext->fontHeight;
			}
			else{
				log_i("overflow ptext->GetImageHeight() \n");
				text-=step;
				break;
			}
		}
			//ptext->ft_draw_bitmap(&slot->bitmap, x + slot->bitmap_left, y + ptext->fontHeight - slot->bitmap_top, color);
			ptext->ft_draw_bitmap(&slot->bitmap, x + slot->bitmap_left, y +ptext->fontHeight - slot->bitmap_top, color);
			x += word_w; //可以从此处下手添加自动换行功能
	}
	log_i("final_num=%d TTF_Displayutft8 exit!\n",text-start);
	unlock();
	return text-start;
}


unsigned int FontDev::displayUTF8(text * ptext, const char *text, int num, unsigned int color, unsigned char style,
		int padding_left,int padding_top)
{
	if (face == NULL)
	{
		errexitf("font not initialize %s\r\n", text);
	}
	int i;
	U16 fontCode;
	FT_GlyphSlot slot = face->glyph;
	//int max_h = face->size->metrics.ascender  >> 6;   // 基线到字符轮廓最高点的距离
	int x = 2+padding_left;//给点余量吧
	int y = padding_top;
	lock();
	setPixelSize(ptext->fontWidth, ptext->fontHeight);
	 const char *start=text;
	 const char* stop = text + num;
	 const char* old;
	 int step;
    while (text < stop)
    {
		if (style & FONT_BOLD)
		{
			FT_Pos strength = (1 << 6);
			FT_Outline_Embolden(&face->glyph->outline, strength);
		}

		if (style & FONT_ITALIC) //
		{
			FT_Set_Transform(face, &matrix, &pen);
		}

		//fontCode = text[i];
		old=text;
		fontCode= UTF8_NextUnichar(&text);
		step=text-old;
		if(fontCode==0){
			text-=step;
			break;
		}
		ft_error = FT_Load_Char(face, fontCode, TTF_bitmap_type); /*  FT_LOAD_NO_BITMAP | FT_LOAD_RENDER */
		if (ft_error)
		{
			log_e("Error at load char!\n");
			unlock();
			return -1;
		}

		if (style & FONT_BOLD)
		{
			FT_GlyphSlot_Embolden(slot);
		}
		int word_w=slot->advance.x >> 6;

		if ((x + slot->bitmap_left+ (word_w) ) > ptext->GetImageWidth())
		{
			if ((y + (ptext->fontHeight) * 2.4) < ptext->GetImageHeight())//换行
			{
				x = padding_left;
				y +=ptext->fontHeight*1.2;
			//	y +=ptext->fontHeight;
			}
			else{
				log_i("overflow ptext->GetImageHeight() \n");
				text-=step;
				break;
			}
		}
			ptext->ft_draw_bitmap(&slot->bitmap, x + slot->bitmap_left, y +ptext->fontHeight - slot->bitmap_top, color);
			x += word_w; //可以从此处下手添加自动换行功能

	}
    unlock();
	return text-start;
}



void text::ft_draw_bitmap(FT_Bitmap *bitmap, int dst_x, int dst_y,
		unsigned int color)
{
	int y, x;
	unsigned char gray;

	if (dst_x + bitmap->width > u32Width)
	{
		log_w("warning ft_draw_bitmap u32Width[%d] to low\r\n",u32Width);
		return;
	}
	if (dst_y + bitmap->rows > u32Height)
	{
		log_w("warning ft_draw_bitmap u32Height[%d] to low\r\n",u32Height);
		return;
	}
	lock();
	for (y = 0; y < bitmap->rows; y++)
	{
		if (dst_y + y > u32Height)
		{
			break;
		}
		for (x = 0; x < bitmap->width; x++)
		{
			if (dst_x + x > u32Width)
			{
				break;
			}
			gray = bitmap->buffer[y * bitmap->pitch + x];
			//                  log_i("gray=%d\n",gray);
			if (gray > 0)
			{
				LCD_PutPixel(dst_x + x, dst_y + y, color, gray);
			}
		}
	}
	unlock();
}



map<hustr, FontDev> font_mp;


void parseFont(HUMap & xmlmp, xmlproc * xml)
{
	const char * path;
	const char * name = xmlmp["name"]->getvalue();
	path = xmlmp["path"]->getvalue();

	log_i("parseFont %s=%s\n", name, path);
	map<hustr, FontDev>::iterator it;
	for (it = font_mp.begin(); it != font_mp.end(); ++it)
	{
		if(it->first==name){
			log_i("font %s had init before\n", name);
			return ;
		}
	}
	font_mp[name].TTF_Init(path, FT_LOAD_NO_BITMAP | FT_LOAD_RENDER);
}

static InstallXMLinstan install2("font",parseFont);

