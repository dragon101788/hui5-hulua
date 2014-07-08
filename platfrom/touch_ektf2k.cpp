#include "touch_ektf2k.h"
#include "manager_touch.h"

static int fd;
int TouchInit()
{
	if ((fd = open("/dev/input/event0", O_RDONLY)) < 0)
	{
		return 1;
	}
	return 0;
}
int GetTouchSample(touch_sample * samp)
{
	static int cnt = 0;

	while (1)
	{
		struct input_event data;
		int ret = read(fd, &data, sizeof(data));
		if (ret < 0)
		{
			if (errno == EAGAIN)
			{
				return 0;
			}
			else
			{
				errexitf("touch_proc_event huts_read %s %d\r\n", strerror(errno), errno);
			}
		}
		if (data.type == EV_ABS)
		{
			//printf("data.code=(%#x %d) data.value=(%#x %d) data.type=(%#x %d) \r\n",data.code,data.code,data.value,data.value,data.type,data.type);

			if (data.code == ABS_MT_TOUCH_MAJOR)
			{
				//printf("ABS_MT_TOUCH_MAJOR %d\r\n", data.value);
				//tssmp->pressure = data.value;

			}
			else if (data.code == ABS_MT_TRACKING_ID)
			{
				//printf("ABS_MT_TRACKING_ID %d\r\n", data.value);
			}
			else if (data.code == ABS_MT_POSITION_X)
			{
				//printf("ABS_MT_POSITION_X %d\r\n", data.value);
				samp->x = data.value;
			}
			else if (data.code == ABS_MT_POSITION_Y)
			{
				//printf("ABS_MT_POSITION_Y %d\r\n", data.value);
				samp->y = data.value;

				//printf("x=%d y=%d \r\n",x,y);

			}
			else
			{
				//printf("EV_ABS data.code=%d data.value=%d\r\n", data.code, data.value);
			}
			cnt++;
		}
		else if (data.type == EV_SYN)
		{

			if (data.code == SYN_MT_REPORT)
			{
				samp->pressure = 1000;
				//printf("x=%d y=%d down\r\n", x, y);
				return 1;
			}
			else
			{
				//printf("EV_SYN data.code=%d data.value=%d\r\n", data.code, data.value);
				//					cnt++;
				if (cnt > 3)
				{
					cnt = 0;
					//return 1;

				}
				else if (cnt == 0)
				{
					samp->pressure = 0;
					//printf("x=%d y=%d up\r\n", x, y);
					return 1;
				}
			}

			samp->pressure = 1000;
			//pres = 1;
			//while(read(fd, &data, sizeof(data))>0);
			//return 1;
		}
		else
		{
			//printf("data.type == %d\r\n", data.type);
		}
	}
}
