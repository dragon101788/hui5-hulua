#ifndef __CODEC_H__
#define __CODEC_H__

#include <string>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
using namespace std;

#include "hui.h"
//#include "jpeg.h"
//#include "jpegcodec.h"
//#include "w55fa92_blt.h"
//#include "blt.h"
#include "hulib.h"
#include "alpha.h"

extern int snapcache;

int bmpCodec_to_image(image * sobj, const char *filename);
int jpegCodec_to_image(image * sobj, const char *filename);
int pngCodec_to_image(image * sobj, const char *filepath);
int pngEndec_to_image(const char *file_name, image * graph);
int codec_to_Image(image * enode, const char * filename);

void Render_img_to_buf(void * buf, image * img, int width, int height);
void ProcArea(image * dst_img, image * rsc_img, int & src_x, int & src_y, int & cp_width, int & cp_height, int & dst_x, int & dst_y);
void AreaCopy(image * dst_img, image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y);

int fill_image(image * img, int a = 255, int r = 128, int g = 128, int b = 128);
int access_Image(const char * filename);

typedef struct
{
	unsigned char u8Blue;
	unsigned char u8Green;
	unsigned char u8Red;
	unsigned char u8Alpha;
} IMG_PIX;

class image: virtual public Mutex
{
protected:
	void * pSrcBuffer;
	unsigned long SrcSize;
	unsigned int u32Width;	// crop width
	unsigned int u32Height;	// crop height
	unsigned int u32Stride;
	int transp;             //透明度

public:
	hustr path;
	friend void Render_img_to_img(image * dst, image * src, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y);
	friend int image_write_to_snap(image * img, const char * rawpath);
	friend int image_read_from_snap(image * img, const char * rawpath);
	friend void AreaCopy(image * dst_img, image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y);
	int getSize()
	{
		return SrcSize;
	}
	int GetImageWidth()
	{
		return u32Width;
	}
	int GetImageHeight()
	{
		return u32Height;
	}

	void setTransp(int n)
	{
		lock();
		transp = n;
		unlock();
	}
	void dump_from_buf(const void * buf, int width, int height)
	{
		lock();
		SetBuffer(width, height);
		memcpy(pSrcBuffer, buf, SrcSize);
		unlock();
	}
	void dump_to_buf(void * buf)
	{
		lock();
		memcpy(buf, pSrcBuffer, SrcSize);
		unlock();
	}
	int isNULL()
	{
		return (pSrcBuffer == NULL);
	}

	image& operator=(const image &img)
	{
		if (SrcSize < img.SrcSize)
		{
			destroy();
		}
		SetBuffer(img.u32Width, img.u32Height);
		memcpy(pSrcBuffer, img.pSrcBuffer, SrcSize);
		path = img.path;
	}

//	inline void SetPix(int pos, int r, int g, int b, int a)
//	{
//		IMG_PIX * tmp = GetPix(pos);
//		tmp->u8Red = r;
//		tmp->u8Green = g;
//		tmp->u8Blue = b;
//		tmp->u8Alpha = a;
//	}
//	inline void SetPix(int x, int y, int r, int g, int b, int a)
//	{
//		IMG_PIX * tmp = GetPix(x,y);
//		tmp->u8Red = r;
//		tmp->u8Green = g;
//		tmp->u8Blue = b;
//		tmp->u8Alpha = a;
//	}
	inline IMG_PIX * GetPix(int x, int y)
	{
		if (x > u32Width || y > u32Height)
		{
			huErrExitf("Get pixels beyond the range %s x=%d y=%d width=%d height=%d\r\n", path.c_str(), x, y, u32Width, u32Height);
		}
		return GetPix(y * u32Width + x);
	}
	inline IMG_PIX * GetPix(int pos)
	{
		if ((pos * 4) > SrcSize)
		{
			huErrExitf("Get pixels beyond the range %s pos=%d width=%d height=%d\r\n", path.c_str(), pos, u32Width, u32Height);
		}
		return ((IMG_PIX *) pSrcBuffer + pos);
	}

	unsigned long SrcGPUAddr()
	{
		return (unsigned long) pSrcBuffer;
	}

	void SetResource(const char * filepath)
	{
		//printf("SetResource %s\r\n",filepath);

		if (path != filepath)
		{
			destroy();
			path = filepath;
		}
	}
	int LoadResource()
	{
		if (pSrcBuffer != NULL)
		{
			return 0;
		}
		if (path.empty())
		{
			debug("warning LoadResource empty path\r\n");
			return -1;
		}
		return codec_to_Image(this, path.nstr());//装载图片
	}
	void ReSetResource(const char * filepath)
	{
		if (path != filepath)
		{
			path = filepath;
			codec_to_Image(this, path.c_str());
		}
	}
	void SaveResource(const char * file)
	{
		path = file;
		pngEndec_to_image(file, this);
	}

	int SetBuffer(int width, int height)
	{

		//path.format("SetBuffer-%dx%d",width,height);
		lock();

		if(width * height == 0)
                {
		    log_w("undefined width height is zero\n");
                }

		int dep = 4;

		int tmpsize = width * height * dep;
		if (tmpsize > SrcSize)
		{
			destroy();

		}
		if (pSrcBuffer == NULL)
		{
		        log_d("SetBuffer %d %d %d\n",width,height,tmpsize);
			pSrcBuffer = malloc(tmpsize);
			if (pSrcBuffer == NULL)
			{
				errexitf("image malloc failed: width=%d height=%d\n", width, height);
			}
		}
		SrcSize = tmpsize;
//		int err = posix_memalign(&pSrcBuffer, 4, SrcSize);
//		if (err)
//		{
//			errexitf("posix_memalign failed: width=%d height=%d %s\n", width, height, strerror(err));
//		}

		u32Width = width;
		u32Height = height;
		u32Stride = width * dep;

		cleanBuf();
		unlock();
		return 0;
	}





	void DrawPoint(int x, int y, int color)
	{
		*((unsigned int *) pSrcBuffer + y * u32Width + x) = color;
	}
	void DrawBrush(int x, int y, int color)
	{
		DrawPoint(x, y, color);
		DrawPoint(x, y + 1, color);
		DrawPoint(x, y + 2, color);
		DrawPoint(x, y - 1, color);
		DrawPoint(x, y - 2, color);

		DrawPoint(x + 1, y, color);
		DrawPoint(x + 2, y, color);

		DrawPoint(x - 1, y, color);
		DrawPoint(x - 2, y, color);
	}

	void AreaCmp(image * img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
	{
		int x;
		int y;
		for (y = 0; y < cp_height; y++)
		{
			for (x = 0; x < cp_width; x++)
			{
				if (*((unsigned int *) pSrcBuffer + (y + dst_y) * u32Width + dst_x + x)
						== *((unsigned int *) img->pSrcBuffer + (y + src_y) * img->u32Width + src_x + x))
				{
					*((unsigned int *) pSrcBuffer + (y + dst_y) * u32Width + dst_x + x) = 0;
				}
			}
		}
	}
	void cleanBuf()
	{
		if (pSrcBuffer)
			memset(pSrcBuffer, 0, SrcSize);
	}

	void debug_info()
	{
		printf("u32Width=%d\r\n", u32Width);
		printf("u32Height=%d\r\n", u32Height);
		printf("u32Stride=%d\r\n", u32Stride);
		printf("SrcGPUAddr=0x%x\r\n", SrcGPUAddr());

	}

	virtual void RenderFrom(image * img, int x, int y)
	{
		//AreaCopy(img, 0, 0, img->u32Width, img->u32Height, x, y);
		//printf("$$$HU$$$ Render %s to %s\r\n",this->path.c_str(),img->path.c_str());

		img->LoadResource();
		Render_img_to_img(this, img, 0, 0, img->u32Width, img->u32Height, x, y);
	}
	virtual void RenderFrom(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
	{
		src_img->LoadResource();
		ProcArea(this, src_img, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
		Render_img_to_img(this, src_img, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
	}

	virtual void RenderTo(image * img, int x, int y)
        {
                //AreaCopy(img, 0, 0, img->u32Width, img->u32Height, x, y);
                //printf("$$$HU$$$ Render %s to %s\r\n",this->path.c_str(),img->path.c_str());

                img->LoadResource();
                Render_img_to_img(img,this, 0, 0, img->u32Width, img->u32Height, x, y);
        }

	virtual void RenderTo(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
        {
                src_img->LoadResource();
                ProcArea(this, src_img, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
                Render_img_to_img(src_img, this, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
        }

	void AreaCopyFrom(image * img, int x, int y)
        {
                ::AreaCopy(this , img, 0, 0, img->u32Width, img->u32Height, x, y);
        }
        void AreaCopyFrom(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
        {

                ::AreaCopy(this, src_img, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
        }

        void AreaCopyTo(image * img, int x, int y)
        {
                ::AreaCopy(img,this, 0, 0, img->u32Width, img->u32Height, x, y);
        }
        void AreaCopyTo(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
        {

                ::AreaCopy(src_img , this, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
        }

//	int SetConfig(const char * key,const char * value)
//	{
//		if(strcasecmp(key,"alpha")==0)
//		{
//			stransformation.colorMultiplier.i16Alpha = strtoul(value,NULL,10);
//		}
//		else
//		{
//			return -1;
//		}
//		return 0;
//	}

//	
	image()
	{

		pSrcBuffer = NULL;
		SrcSize = 0;
		u32Width = 0;       // crop width
		u32Height = 0;      // crop height
		u32Stride = 0;
		transp = 100;		//控件透明度-默认不透明

		//s_sblitop.transformation = &stransformation;

	}

	virtual ~image()
	{
		destroy();
	}

	//释放空间并不改变其他内容
	void Free()
	{
		if (pSrcBuffer != NULL)
		{
			debug("destory image pSrcBuffer [%s] %dx%d\r\n", path.c_str(), u32Width, u32Height);
			free(pSrcBuffer);
			pSrcBuffer = NULL;
		}
	}
	void destroy()
	{

		if (pSrcBuffer != NULL)
		{
			debug("destory image pSrcBuffer [%s] %dx%d\r\n", path.c_str(), u32Width, u32Height);
			free(pSrcBuffer);
			pSrcBuffer = NULL;
			SrcSize = 0;
			u32Width = 0;       // crop width
			u32Height = 0;      // crop height
			u32Stride = 0;
			path.clear();
		}
	}

};

#endif //__CODEC_H__
