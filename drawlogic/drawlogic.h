#ifndef __DRAWLOGIC_H__
#define __DRAWLOGIC_H__


#if defined(CONFIG_DRAWLOGIC_BFQ)
#include "drawlogic_BFQ.h"
class drawlogic_BQF;
typedef drawlogic_BQF element;


#else
#error "!!!dragon!!! Undefined draw logic style"
#error "!!!dragon!!! --> plase menuconfig select [draw logic]"
#endif //CONFIG_DRAWLOGIC










#endif //__DRAWLOGIC_H__
