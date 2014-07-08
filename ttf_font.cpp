#include "trueType/ft2build.h"
#include "trueType/freetype.h"
#include "trueType/ftoutln.h"
#include "trueType/ftsynth.h"

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
//#define TTC_PATCH "/mnt/mmcblk0p1/simsun.ttc"
//#define TTC_PATCH "/mnt/mmcblk0p1/msyhbd.ttf"
//#define TTC_PATCH "/mnt/mmcblk0p1/msyh.ttf"
#define IOCTL_LCD_ENABLE_INT    _IO('v', 28)
#define IOCTL_LCD_DISABLE_INT   _IO('v', 29)
//extern  unsigned int u32Width;
//extern  unsigned int u32Height;
//extern unsigned long SrcSize;
//extern  char * pSrcBuffer;
//extern void DrawPoint(int x, int y, int color);
//extern  int lcm_fd; 
//void DrawPoint(unsigned int x,unsigned int y,unsigned char color);

//  int truetype::added_txt(unsigned int w,unsigned int h,unsigned int color)
// {
//     Width_text=w;
//   Highth_text=h;
//      text_buffer=(unsigned int *)malloc(sizeof(int)*h*w);
//      memset(text_buffer,color,sizeof(int)*h*w);
//       if (text_buffer==NULL)
//         {
//	 printf("added_txt:malloc failed!\n");
//         return -1;
//       }
//	return 0;
//}
//此函数不再使用
//void truetype:: free_text()
//{
//free(text_buffer);
//FT_Done_FreeType(ft_Lib);
//}

//void  truetype::Disp_txt(unsigned int * pSrcBuffer,unsigned int SrcBuffer_W, unsigned int x,unsigned int y,unsigned int w,unsigned int h)
//{
//int i,j;
//printf("i am in Disp_txt!!\n");
//// ioctl(lcm_fd, IOCTL_LCD_DISABLE_INT);
//    for(j=y;j<y+h;j++)
//      for(i=x;i<x+w;i++)
//	*((unsigned int *) pSrcBuffer + j * SrcBuffer_W +i) =*((unsigned int *) text_buffer+ (j-y) * Width_text + (i-x));
//// ioctl(lcm_fd, IOCTL_LCD_ENABLE_INT);
//
//printf("i will out Disp_txt!!\n");
//}

//void text::ft_draw_bitmap(FT_Bitmap *bitmap, int x, int y, unsigned int color)
//{
//	int i, j;
////	short RGB555;
//	unsigned char gray;
//	//     bitmap->pixel_mode=FT_PIXEL_MODE_GRAY;
////	switch (bitmap->pixel_mode)
////	{
////	case FT_PIXEL_MODE_GRAY:
//	for (i = 0; i < bitmap->rows; i++)
//	{
//		for (j = 0; j < bitmap->width; j++)
//		{
//			gray = bitmap->buffer[i * bitmap->pitch + j];
//			//                  printf("gray=%d\n",gray);
//			if (gray > 0)
//			{
//				LCD_PutPixel(x + j, y + i, color, gray);
//			}
//		}
//	}
//#if 0
//	break;
//	case FT_PIXEL_MODE_MONO:
//	printf("case FT_PIXEL_MODE_MONO\n");
//	for ( i = 0; i < bitmap->rows; i++)
//	{
//		for(j = 0; j < bitmap->width; j++ )
//		{
//			LCD_PutPixel(x + j, y + i,
//					(((bitmap->buffer[i * bitmap->pitch + j / 8]) & (0x80 >> (j & 7))) ? color : 0xfffe)
//			);
//		}
//	}
//	break;
//}
//#endif
//
//}
//#if 0
//static U16 InternalCode2Unicode(U16 iCode)
//{
//#if 1
//	unsigned char hByte, lByte;
//	lByte = (iCode & 0x00ff) - 0xA0;
//	hByte = ((iCode >> 8) & 0x00ff) - 0xA0;
//	return unicodeTable[hByte - 16][lByte];
//#endif
//
//}
//#endif
/*******************************************************
 *******************************************************/
//int text::TTF_DisplayAscii(int x, int y, const unsigned char *text, int num, unsigned int color, unsigned char style)
//{
//	int i;
//	U16 fontCode;
//	FT_GlyphSlot slot = face->glyph;
//
//	for (i = 0; i < num; i++)
//	{
//		if (style & FONT_BOLD)
//		{
//			FT_Pos strength = (1 << 6);
//			FT_Outline_Embolden(&face->glyph->outline, strength);
//		}
//
//		if (style & FONT_ITALIC)
//		{
//			FT_Set_Transform(face, &matrix, &pen);
//		}
//		fontCode = text[i];
//
////		if (fontCode >> 7)
////		{
////			fontCode = text[i];// << 8;
////			fontCode |= text[++i]<<8;
////			fontCode = InternalCode2Unicode(fontCode);
////		}
//
//		ft_error = FT_Load_Char(face, fontCode, TTF_bitmap_type); /*  FT_LOAD_NO_BITMAP | FT_LOAD_RENDER */
//		if (ft_error)
//		{
//			printf("Error at load char!\n");
//			return -1;
//		}
//
//		if (style & FONT_BOLD) //
//		{
//			FT_GlyphSlot_Embolden(slot);
//		}
//		ft_draw_bitmap(&slot->bitmap, x + slot->bitmap_left, y + Font_H - slot->bitmap_top, color);
//
//		x += slot->advance.x >> 6;
//
//	}
//	return 0;
//}
ssize_t FontDev::convert(const char *tocode, const char *fromcode, char *inbufp, size_t inbytesleft, char *outbufp,
		size_t outbytesleft)
{
	iconv_t ic;
	size_t outbytes = outbytesleft;
	int ret;
	ic = iconv_open(tocode, fromcode);
	if (ic == (iconv_t) -1)
	{
		printf("iconv_open failed: from: %s, to: %s: %s", fromcode, tocode, strerror(errno));
		return -1;
	}
	while (inbytesleft > 0)
	{
		ret = iconv(ic, &inbufp, &inbytesleft, &outbufp, &outbytes);
		if (ret == -1)
		{
			printf("iconv failed: from: %s, to: %s: %s", fromcode, tocode, strerror(errno));
			return -1;
		}
	}
	ret = iconv_close(ic);
	if (ret == -1)
	{
		printf("iconv_close failed: from: %s, to: %s: %s", fromcode, tocode, strerror(errno));
		return -1;
	}
	return outbytesleft - outbytes;
}

void FontDev::DrawText(text * ptext, const char *encode, char * showtxt,int buff_width,int buff_height, unsigned int txt_len)
{

	unsigned int final_len = 0;
	unsigned int buf_len = (txt_len + 1) * sizeof(wchar_t) * 2; //当utf8显示字母时有最大占用
	wchar_t *mytxt = (wchar_t *) malloc(buf_len); //用来存放unicode
	if (mytxt == NULL)
	{
		huErrExit("DrawText:: malloc failed!\n");
	}
	memset(mytxt, 0, buf_len);

	final_len = convert("wchar_t", encode, (char *) showtxt, txt_len, (char *) mytxt, buf_len);
	final_len /= 4; //byte to wchar_t

	TTF_DisplayUnicode(ptext, mytxt, final_len, ptext->color, ptext->style,buff_width,buff_height);
	free(mytxt);
}
//int FontDev::TTF_DisplayAscii(text * ptext, const unsigned char *text, int num, unsigned int color, unsigned char style)
//{
//	if (face == NULL)
//	{
//		errexitf("font not initialize %s\r\n", text);
//	}
//	int i;
//	U16 fontCode;
//	FT_GlyphSlot slot = face->glyph;
//	int x = 0;
//	int y = 0;
//	for (i = 0; i < num; i++)
//	{
//		if (style & FONT_BOLD)
//		{
//			FT_Pos strength = (1 << 6);
//			FT_Outline_Embolden(&face->glyph->outline, strength);
//		}
//
//		if (style & FONT_ITALIC)
//		{
//			FT_Set_Transform(face, &matrix, &pen);
//		}
//		fontCode = text[i];
//
//		//		if (fontCode >> 7)
//		//		{
//		//			fontCode = text[i];// << 8;
//		//			fontCode |= text[++i]<<8;
//		//			fontCode = InternalCode2Unicode(fontCode);
//		//		}
//
//		ft_error = FT_Load_Char(face, fontCode, TTF_bitmap_type); /*  FT_LOAD_NO_BITMAP | FT_LOAD_RENDER */
//		if (ft_error)
//		{
//			printf("Error at load char!\n");
//			return -1;
//		}
//
//		if (style & FONT_BOLD) //
//		{
//			FT_GlyphSlot_Embolden(slot);
//		}
//		ptext->ft_draw_bitmap(&slot->bitmap, x + slot->bitmap_left, y + Font_H - slot->bitmap_top, color);
//
//		x += slot->advance.x >> 6;
//
//	}
//	return 0;
//}

int FontDev::TTF_DisplayUnicode(text * ptext, const wchar_t *text, int num, unsigned int color, unsigned char style,int buff_width,int buff_height)
{
	int final_num=0;
	if (face == NULL)
	{
		errexitf("font not initialize %s\r\n", text);
	}
	int i;
	U16 fontCode;
	FT_GlyphSlot slot = face->glyph;
	int x = 0;
	int y = 0;
	printf("TTF_DisplayUnicode!,num=%d\n", num);
	printf("buff_width=%d ,buff_height=%d\n",buff_width,buff_height);

	setPixelSize(ptext->fontWidth, ptext->fontHeight);
	for (i = 0; i < num; i++)
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

		fontCode = text[i];
		ft_error = FT_Load_Char(face, fontCode, TTF_bitmap_type); /*  FT_LOAD_NO_BITMAP | FT_LOAD_RENDER */
		if (ft_error)
		{
			printf("Error at load char!\n");
			return -1;
		}

		if (style & FONT_BOLD)
		{
			FT_GlyphSlot_Embolden(slot);
		}
		//Font_H for prevent overflow
		final_num++;//显示文字数加一
		ptext->ft_draw_bitmap(&slot->bitmap, x + slot->bitmap_left, y + Font_H - slot->bitmap_top, color);
		x += slot->advance.x >> 6; //可以从此处下手添加自动换行功能
		if ((x + Font_H * 2 ) > buff_width)
		{
			if ((y + Font_H * 2 + 4) < buff_height)
			{
				x = 0;
				y +=Font_H*1.2;
			}
			else
				break;
		}
	//	if(Font_H+4>buff_height)
	//		break;

	}

	printf("final_num=%d TTF_DisplayUnicode exit!\n",final_num);
	return final_num;
}

map<hustr, FontDev> font_mp;


void ParseFont(HUMap & xmlmp, xmlproc * xml)
{
	const char * path;
	const char * name = xmlmp["name"]->getvalue();
	path = xmlmp["path"]->getvalue();

	printf("ParseFont %s=%s\n", name, path);
	font_mp[name].TTF_Init(path, FT_LOAD_NO_BITMAP | FT_LOAD_RENDER);
	//printf("get font_mp %x %x\r\n",font_mp[name].face,font_mp[name].ft_Lib);
}
//XMLinstan["font"] = ParseFont;
static InstallXMLinstan install2("font",ParseFont);

