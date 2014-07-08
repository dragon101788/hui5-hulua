#ifndef __DL_PROC_H__
#define __DL_PROC_H__

#include "xmlproc.h"
#include <dlfcn.h>

class dlproc: public xmlproc
{
public:
	void *dp;
	char *error;
	typedef int (*onLoader_ft)(dlproc * dlproc);
	onLoader_ft onLoader;
	dlproc(const char * dlfile)
	{

		dp = dlopen(dlfile, RTLD_LAZY); /* 打开动态链接库 */
		if (dp == NULL) /* 若打开失败则退出 */
		{
			fputs(dlerror(), stderr);
			exit(1);
		}
	}
	void doLoader()
	{
		UnDoneProc();

		onLoader = (onLoader_ft) dlsym(dp, "onLoader");
		error = dlerror(); /* 检测错误 */
		if (error) /* 若出错则退出 */
		{
			printf("dlerror::%s\r\n", error);
			exit(1);
		}

		int ret = onLoader(this); /* 调用此共享函数 */

		DoneProc();
	}
	~dlproc()
	{
		printf("dlproc exit\r\n");
		dlclose(dp); /* 关闭共享库 */
		printf("dlproc exit OK\r\n");
	}
};
//
//dlproc * OpenDlProc(const char * name)
//{
//	typedef dlproc * (*init_fn)();
//	void *dp;
//	char *error;
//	init_fn init;
//
//	dlproc *
//	ret->dp = dp;
//	return ret;
//}

#endif //__DL_PROC_H__
