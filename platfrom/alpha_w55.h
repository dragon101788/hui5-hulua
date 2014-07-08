#ifndef __BLT_H__
#define __BLT_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <linux/fb.h>
#include <getopt.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <list>
using namespace std;

#include "../hulib.h"
#include "w55fa92_blt.h"

class BLT: public Mutex
{
public:
	BLT()
	{
		lock();
		for (int i = 0; i < 10; i++)
		{
			blt_fd = open("/dev/blt", O_RDWR);
			if (blt_fd == -1)
			{
				if (errno == EBUSY)
				{
					usleep(100);
				}
				else
				{
					errexitf("Open BLT device failed: %s\n", strerror(errno));

				}
			}
			else
			{
				break;
			}
		}

		if (ioctl(blt_fd, BLT_IOCTMMU, 1) == -1)
		{
			printf("### Error: failed to configure BLT device to MMU mode: %s\n", strerror(errno));
			exit(-1);
		}
		unlock();
	}
	~BLT()
	{
		if (blt_fd != -1)
		{
			close(blt_fd);
		}

	}
	int blt_set_colkey(unsigned short u16RGB565ColorKey)
	{
		if ((ioctl(blt_fd, BLT_IOCTRGB565COLORKEY, u16RGB565ColorKey)) < 0)
		{
			fprintf(stderr, "set RGB565 color key parameter failed: %s\n", strerror(errno));

			exit(-1);
		}

		return 0;
	}

	int blt_flush()
	{
		lock();
		while (ioctl(blt_fd, BLT_IOCFLUSH) == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}

			errexitf("ioctl BLT_IOCFLUSH failed: %s\n", strerror(errno));
		}
		unlock();
	}
	int blt_start_blit(S_DRVBLT_BLIT_OP *blitop)
	{

		lock();

		if ((ioctl(blt_fd, BLT_IOCSBLIT, blitop)) != 0)
		{
			errexitf("set BLIT parameter failed: %s\n", strerror(errno));
		}

		if ((ioctl(blt_fd, BLT_IOCTRIGGER, NULL)) != 0)
		{
			errexitf("trigger BLT failed: %s\n", strerror(errno));
		}

		while (ioctl(blt_fd, BLT_IOCFLUSH) == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}

			errexitf("ioctl BLT_IOCFLUSH failed: %s\n", strerror(errno));
		}
		unlock();
		return 0;
	}

	int blt_fd;
};

extern BLT g_blt;

#endif //__BLT_H__
