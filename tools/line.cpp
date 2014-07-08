#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>
#include <list>
using namespace std;

class touch_panel_sample_base
{
public:
	touch_panel_sample_base()
	{
		x = 0;
		y = 0;
		pressure = 0;
	}

	virtual int init_touch_panel() = 0;

	virtual int gather() = 0;
	int x;
	int y;
	int pressure;
};

class huts_sample: public touch_panel_sample_base
{
public:
	int init_touch_panel()
	{
		if ((fd = open("/dev/input/event0", O_RDONLY)) < 0)
		{
			printf("Error open %s\n\n", "/dev/input/event0");
			exit(-1);
		}
	}

	int gather()
	{
		static int pres = 0;
		static int cnt = 0;

		while (1)
		{
			struct input_event data;
			int ret = read(fd, &data, sizeof(data));
			if (ret < 0)
			{
				if (errno == EAGAIN)
				{
					if (pres == 1)
					{
						cnt++;
						if (cnt > 2)
						{
							pressure = 0;

							cnt = 0;
							pres = 0;
							return 1;
						}

					}
					return 0;
				}
				else
				{
					printf("touch_proc_event huts_read %s %d\r\n", strerror(errno),
					errno);
					exit(-1);
				}
			}
			if (data.type == EV_ABS)
			{
				//printf("data.code=(%#x %d) data.value=(%#x %d) data.type=(%#x %d) \r\n",data.code,data.code,data.value,data.value,data.type,data.type);
				cnt = 0;
				if (data.code == ABS_MT_TOUCH_MAJOR)
				{
					//printf("ABS_MT_TOUCH_MAJOR %d\r\n", data.value);
					//tssmp->pressure = data.value;
				}
				if (data.code == ABS_MT_TRACKING_ID)
				{
					//printf("ABS_MT_TRACKING_ID %d\r\n", data.value);
				}
				if (data.code == ABS_MT_POSITION_X)
				{
					//printf("ABS_MT_POSITION_X %d\r\n", data.value);
					x = data.value;
				}
				if (data.code == ABS_MT_POSITION_Y)
				{
					//printf("ABS_MT_POSITION_Y %d\r\n", data.value);
					y =  data.value;

					//printf("x=%d y=%d \r\n",x,y);
					pressure = 1000;
					pres = 1;
					//while(read(fd, &data, sizeof(data))>0);
					return 1;
				}
			}
			else
			{
				//printf("data.type == %d\r\n", data.type);
			}
		}
	}

	int fd;
};

huts_sample samp;

#define DEVMEM_GET_STATUS	_IOR('m', 4, unsigned int)
#define DEVMEM_SET_STATUS	_IOW('m', 5, unsigned int)
#define DEVMEM_GET_PHYSADDR	_IOR('m', 6, unsigned int)
#define DEVMEM_GET_VIRADDR	_IOR('m', 7, unsigned int)

#define IOCTL_LCD_ENABLE_INT	_IO('v', 28)
#define IOCTL_LCD_DISABLE_INT	_IO('v', 29)
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
int main(int argc, char * argv[])
{
	samp.init_touch_panel();
	int lcm_fd = open("/dev/fb0", O_RDWR);
	if (lcm_fd == -1)
	{
		printf("### Error: cannot open LCM device, returns %d\n", lcm_fd);
		exit(-1);
	}

	struct fb_var_screeninfo var;
	if (ioctl(lcm_fd, FBIOGET_VSCREENINFO, &var) == -1)
	{
		printf("### Error: ioctl FBIOGET_VSCREENINFO: %s\n", strerror(errno));
		exit(-1);
	}

	int u32Width = var.xres;
	int u32Height = var.yres;
	int lcm_dpp = var.bits_per_pixel;
	int SrcSize = u32Width * u32Height * (lcm_dpp / 8);

	int * pSrcBuffer = (int *)mmap(NULL, SrcSize, PROT_READ | PROT_WRITE, MAP_SHARED, lcm_fd, 0);
	if (pSrcBuffer == MAP_FAILED)
	{
		printf("### Error: failed to map LCM device to memory: %s\n", strerror(errno));
		exit(-1);
	}
	memset(pSrcBuffer,0,SrcSize);

	int x=100;
	int y=100;
//	*(pSrcBuffer + y*u32Width + x) = 0xff;
//	while(x<u32Width&&y<u32Height)
//	{
//		*(pSrcBuffer + y*u32Width + x) = 0xff;
//		x++;
//		y++;
//	}
	while (1)
	{
		int ret = samp.gather();
		printf("x=%d y=%d\r\n", samp.x, samp.y);
		*(pSrcBuffer + samp.y*u32Width + samp.x) = 0xff;
	}
}
