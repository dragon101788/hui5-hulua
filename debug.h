#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "autoconf.h"

#ifdef CONFIG_DEBUG
#define debug(a,...)	fprintf(stderr,a,##__VA_ARGS__)
#else
#define debug(a,...)
#endif

#ifdef CONFIG_LOG_D
#define log_d(a,...)    fprintf(stderr,a,##__VA_ARGS__)
#else
#define log_d(a,...)
#endif


#ifdef CONFIG_LOG_I
#define log_i(a,...)    fprintf(stderr,a,##__VA_ARGS__)
#else
#define log_i(a,...)
#endif


#ifdef CONFIG_LOG_W
#define log_w(a,...)    fprintf(stderr,a,##__VA_ARGS__)
#else
#define log_w(a,...)
#endif

#ifdef CONFIG_LOG_E
#define log_e(a,...)    fprintf(stderr,a,##__VA_ARGS__)
#else
#define log_e(a,...)
#endif

#endif //__DEBUG_H__
