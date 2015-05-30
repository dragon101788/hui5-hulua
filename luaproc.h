#ifndef __LUA_PROC_H__
#define __LUA_PROC_H__

#include "xmlproc.h"
#include "hui_hulua.h"

class luaproc: public xmlproc
{
public:
        luaproc(const char * dlfile)
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

                lua.register_func("register_Instance",register_Instance);
                int result = lua.call<int>("main");

                DoneProc();
        }
        ~luaproc()
        {
                printf("dlproc exit\r\n");
                dlclose(dp); /* 关闭共享库 */
                printf("dlproc exit OK\r\n");
        }
};
//
//dlproc * OpenDlProc(const char * name)
//{
//      typedef dlproc * (*init_fn)();
//      void *dp;
//      char *error;
//      init_fn init;
//
//      dlproc *
//      ret->dp = dp;
//      return ret;
//}

#endif //__LUA_PROC_H__
