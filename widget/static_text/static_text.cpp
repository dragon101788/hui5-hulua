#include "static_text.h"




//XMLinstan["static_text"] = Install_Element<static_text>;
//static InstallXMLinstan install1("static_text",Install_Element<static_text>);
static InstallELEinstan<static_text> install("static_text");

hucall int init(HUMap & mp,void * data)
{
	printf("module init\r\n");
}
