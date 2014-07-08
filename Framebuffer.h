#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <string>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "hulib.h"
#include "codec.h"
#include "autoconf.h"
using namespace std;

#define DEVMEM_GET_STATUS	_IOR('m', 4, unsigned int)
#define DEVMEM_SET_STATUS	_IOW('m', 5, unsigned int)
#define DEVMEM_GET_PHYSADDR	_IOR('m', 6, unsigned int)
#define DEVMEM_GET_VIRADDR	_IOR('m', 7, unsigned int)

#define IOCTL_LCD_ENABLE_INT	_IO('v', 28)
#define IOCTL_LCD_DISABLE_INT	_IO('v', 29)

//fbflush fbf;

#define RGB565(r,g,b)  ( ((r>>3)&(1<<5)-1)<<11 | ((g>>2)&(1<<6)-1)<<5 | ((b>>3)&(1<<5)-1)<<0 )
class framebuffer: public Mutex, public thread
{
public:
	int lcm_fd;
	void * pSrcBuffer;
	unsigned long SrcSize;
	unsigned int u32Width;	// crop width
	unsigned int u32Height;	// crop height

	image put;
	image in_img;
	image out_img;
	hustr snap;
	int run()
	{
		//return 0;
		put.dump_from_buf(pSrcBuffer,u32Width,u32Height);
		//memcpy(put.pSrcBuffer, pSrcBuffer, SrcSize);
		if (!snap.empty())
		{
			printf("fbf snap = %s\r\n", snap.c_str());
			out_img.ReSetResource(snap);
			snap.clear();
		}
		//put.cleanBuf();
		for (int i = 1; i <= 5; i++)
		{
			printf("fbflush run %d\r\n", i);
//			in_img.stransformation.colorMultiplier.i16Alpha = 255 - 32 * i;
//			put.Render(&in_img, 0, 0);
			out_img.setTransp(20 * i);
			put.Render(&out_img, 0, 0);
			RenderImageToFrameBuffer(&put);
			usleep(1000*30);
		}
	}

	void DumpToXml(image & out)
	{
		//cancel();
		wait();
		out_img = out;
		create();
	}
	void DumpToSnap(const char * path)
	{
		//cancel();
		printf("DumpToSnap %s\r\n", path);
		wait();
		snap = path;
		create();
	}
	framebuffer()
	{
		Accept();

		put.path = "framebuffer change put buf";
		in_img.path = "framebuffer change in buf";
		out_img.path = "framebuffer change out buf";

		put.SetBuffer(u32Width, u32Height);
		in_img.SetBuffer(u32Width, u32Height);
		out_img.SetBuffer(u32Width, u32Height);
	}
	~framebuffer()
	{
		Destory();
	}
	int Accept()
	{
		lcm_fd = open("/dev/fb0", O_RDWR);
		if (lcm_fd == -1)
		{
			errexitf("### Error: cannot open LCM device, returns %d\n", lcm_fd);
		}

		struct fb_var_screeninfo var;
		if (ioctl(lcm_fd, FBIOGET_VSCREENINFO, &var) == -1)
		{
			errexitf("### Error: ioctl FBIOGET_VSCREENINFO: %s\n", strerror(errno));
		}

		u32Width = var.xres;
		u32Height = var.yres;

		int lcm_dpp = var.bits_per_pixel;
		printf("FrameBuffer Accept %d %d %d %d %d %d %d\r\n",u32Width,u32Height,lcm_dpp,var.xres_virtual,var.yres_virtual,var.xoffset,var.yoffset);
		SrcSize = u32Width * u32Height * (lcm_dpp / 8);

		pSrcBuffer = mmap(NULL, SrcSize, PROT_READ | PROT_WRITE, MAP_SHARED, lcm_fd, 0);
		if (pSrcBuffer == MAP_FAILED)
		{
			errexitf("### Error: failed to map LCM device to memory: %s\n", strerror(errno));
		}

		return 0;
	}

	void Destory()
	{
		printf("$$$HU$$$ framebuffer destory\r\n");
		if (pSrcBuffer != NULL && pSrcBuffer != MAP_FAILED)
		{
			munmap(pSrcBuffer, SrcSize);
			pSrcBuffer = NULL;
		}
		pSrcBuffer = NULL;
		close(lcm_fd);
		lcm_fd = -1;
	}

	void RenderImageToFrameBuffer(image * img)
	{
		wait();
		if (img == NULL ||img->isNULL())
		{
			huErrExit("RenderFromBuffer Image invalid\r\n");
		}
		lock();
		ioctl(lcm_fd, IOCTL_LCD_DISABLE_INT);


#ifdef CONFIG_FB_SOFT_RGB565
		for(int y=0;y<u32Height;y++)
		{
			for(int x=0;x<u32Width;x++)
			{
				IMG_PIX * pix = img->GetPix(x,y);
				*(((unsigned short*)pSrcBuffer)+(y*u32Width+x)) = RGB565(pix->u8Red,pix->u8Green,pix->u8Blue);
			}
		}
#else
		img->dump_to_buf(pSrcBuffer);
		//memcpy(pSrcBuffer, img->pSrcBuffer, SrcSize);
#endif
		ioctl(lcm_fd, IOCTL_LCD_ENABLE_INT);
		unlock();
	}
};

extern framebuffer fb;

#endif //__FRAMEBUFFER_H__
