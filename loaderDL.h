#ifndef __LOAD_DL_H__
#define __LOAD_DL_H__

#include "xmlproc.h"
#include <dlfcn.h>

class DL
{
private:
	void *dp;
	typedef int (*dl_ft)(HUMap & mp,void * dat);

public:
	DL()
	{
		dp = NULL;
	}
	int open(const char * dlfile)
	{

		dp = dlopen(dlfile, RTLD_LAZY); /* 打开动态链接库 */
		if (dp == NULL) /* 若打开失败则退出 */
		{
			fputs(dlerror(), stderr);
			exit(1);
		}
	}
	void Func(const char * name,HUMap & mp,void * dat)
	{
		char *error;
		dl_ft fun = (dl_ft) dlsym(dp, name);
		error = dlerror(); /* 检测错误 */
		if (error) /* 若出错则退出 */
		{
			printf("dlerror::%s\r\n", error);
			exit(1);
		}
		int ret = fun(mp,dat); /* 调用此共享函数 */
	}
	~DL()
	{
		if(dp!=NULL)
			dlclose(dp); /* 关闭共享库 */
	}
};
extern map<hustr,DL> g_dl_map;

#endif //__DL_PROC_H__
