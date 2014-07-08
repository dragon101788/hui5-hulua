#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "autoconf.h"

#ifdef CONFIG_DEBUG
#define debug(a,...)	fprintf(stderr,a,##__VA_ARGS__)
#else
#define debug(a,...)
#endif

#endif //__DEBUG_H__
