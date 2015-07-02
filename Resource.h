#ifndef __HUI_RESOURCE_H__
#define __HUI_RESOURCE_H__

#include "codec.h"
#include "include/autoconf.h"

#if defined(CONFIG_RESOURCE_RUNTIME_DECODER)
#include "Resource_runtime_decoder.h"
typedef Resource_Runtime_Decoder Resource;

#elif defined(CONFIG_RESOURCE_RUNTIME_INDEX)
#include "Resource_runtime_index.h"
typedef Resource_Runtime_Index Resource;

#elif defined(CONFIG_RESOURCE_RUNTIME_FETCH)
#include "Resource_runtime_fetch.h"
typedef Resource_Runtime_Fetch Resource;

#else
#error "!!!dragon!!! Undefined resources management style"
#error "!!!dragon!!! --> plase menuconfig select [readonly resource manager]"
#endif //CONFIG_RESOURCE_RUNTIME

#endif //__HUI_RESOURCE_H__
