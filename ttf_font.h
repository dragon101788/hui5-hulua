#ifndef  __TTF_FONT_H__
#define  __TTF_FONT_H__

#include "codec.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_SYNTHESIS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <iconv.h>
#include <map>
#include "hulib.h"
#include "transcoding.h"
using namespace std;



#define   FONT_NORMAL    0
#define   FONT_ITALIC    1
#define   FONT_BOLD      2
typedef unsigned short U16;

class text;


class FontDev: public Mutex
{ //FontDev字体绘制器 ,阻塞绘制元素，同BLT功能一般
public:
	FontDev():face(NULL),ft_Lib(NULL)
	{
	}
	~FontDev()
	{
		if (face != NULL)
			FT_Done_Face(face);
		if (ft_Lib != NULL)
			FT_Done_FreeType(ft_Lib);

	}
	int TTF_Init(const char * path, unsigned char bitmap)
	{
		FT_UInt glyph_index;
		int i;
		ft_error = FT_Init_FreeType(&ft_Lib);
		if (ft_error)
		{
			log_e("Error at init ft_Lib!\n");
			FT_Done_FreeType(ft_Lib);
			return -1;
		}

		ft_error = FT_New_Face(ft_Lib, path, 0, &face);
		if (ft_error)
		{
			log_e("New face error!\n");
			FT_Done_FreeType(ft_Lib);
			return -1;
		}

		TTF_bitmap_type = bitmap;

		matrix.xx = 1 << 16;
		matrix.xy = 0x5800;
		matrix.yx = 0;
		matrix.yy = 1 << 16;
		log_i("TTF_Init succssed!!!!!!!!!!!!!!!!!\n");
		// logMsg("init ok.\n");

		return 0;
	}
	int setPixelSize(int fontWidth, int fontHeight)
	{
		ft_error = FT_Set_Pixel_Sizes(face, fontWidth, fontHeight);
		if (ft_error)
		{
			errexitf("Error at set fontSize %dx%d \n", fontWidth, fontHeight);
		}

	}
	int setStyle(unsigned char bitmap)
	{
		TTF_bitmap_type = bitmap;
		return 0;
	}
//         ssize_t convert(const char *tocode, const char *fromcode, char *inbufp, size_t inbytesleft, char *outbufp,
//                size_t outbytesleft);

	unsigned int drawText(text * ptext, char * showtxt,unsigned int txt_len);
	unsigned int drawText(text * ptext, char * showtxt,unsigned int txt_len,int padding_left,int padding_top);
	unsigned int displayUTF8(text * ptext, const char *text, int num,unsigned int color, unsigned char style);
	unsigned int displayUTF8(text * ptext, const char *text, int num, unsigned int color, unsigned char style,
			int padding_left,int padding_top);

	FT_Library ft_Lib;
	FT_Face face;
	FT_UInt glyph_index;
	FT_Vector pen;
	FT_Matrix matrix;
	FT_Error ft_error;
	unsigned char TTF_bitmap_type;
};
class text: public image
{
private:
	//void LCD_PutPixel(unsigned int x, unsigned int y, unsigned int color, unsigned char gray);
	//void ft_draw_bitmap(FT_Bitmap *bitmap, int x, int y, unsigned int color);

public:
	//以下几个变量需要外部赋值
	FontDev * m_font;
	
	unsigned int fontWidth;
	unsigned int fontHeight;
	unsigned int color;
	unsigned char style;
	//void *text_buffer;
	text():
		fontWidth(0),fontHeight(16),
		color(0),style(0),
		m_font(NULL)
	{

	}
	~text()
	{
		destroy();
	}
	void destroy()
	{
		//printf("destroy ttf\n");

	}
	void LCD_PutPixel(unsigned int x, unsigned int y, unsigned long color,
			unsigned char gray)
	{
		*((unsigned long *) pSrcBuffer + y * u32Width + x)=color;
		((IMG_PIX *) pSrcBuffer + y * u32Width + x)->u8Alpha = gray;
	}
	void ft_draw_bitmap(FT_Bitmap *bitmap, int dst_x, int dst_y,unsigned int color);


	unsigned int drawText( char * showtxt, unsigned int txt_len)
	{
		if (m_font != NULL)
		{
			return m_font->drawText(this, showtxt,txt_len);
		}
		return 0;
	}

	unsigned int drawText( char * showtxt, unsigned int txt_len,int padding_left,int padding_top)
		{
			if (m_font != NULL)
			{

				return m_font->drawText(this, showtxt,txt_len,padding_left,padding_top);

			}
			return 0;
		}


};

extern map<hustr, FontDev> font_mp;

#endif    /* __TTF_FUNC_H__ */
