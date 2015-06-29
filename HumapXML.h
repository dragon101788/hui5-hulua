#ifndef __PARASE_XML_H__
#define __PARASE_XML_H__

#include "hulib.h"

int HumapToXmlString(hustr & outstr,HUMap & mp);
int HumapToXmlFile(const char * path,HUMap & mp);
int HumapFromXmlFile(const char *path,HUMap & mp);
int HumapFromXmlString(const char * buf,unsigned long filesize,HUMap & mp);

#endif //__PARASE_XML_H__