#ifndef __HUI_H__
#define __HUI_H__


void huErrExit(const char * str);
#define huErrExitf(a,...)       huErrExit(hustr(a,##__VA_ARGS__))

#endif //__HUI_H__
