#ifndef __TOUCH_OTTA2001A_H__
#define __TOUCH_OTTA2001A_H__

class ott2001a_sample: public touch_panel_sample_base
{
public:
	int init_touch_panel()
	{
		if ((fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK)) < 0)
		{
			errexitf("Error open %s\n\n", "/dev/input/event0");
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
					errexitf("touch_proc_event huts_read %s %d\r\n",
							strerror(errno), errno);
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
					y = 272 - data.value;

					//printf("x=%d y=%d \r\n",x,y);
					pressure = 1000;
					pres = 1;
					//while(read(fd, &data, sizeof(data))>0);
					return 1;
				}
			}
			else if (data.type == EV_SYN)
			{

			}
			else
			{
				printf("data.type == %d\r\n", data.type);
			}
		}
	}

	int fd;
};


#endif //__TOUCH_OTTA2001A_H__
