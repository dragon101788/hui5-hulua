#include "main.h"


//static InstallXMLinstan install("dragframe",Install_Element<dragframe>);
static InstallELEinstan<dragframe> install("dragframe");

hucall int init(HUMap & mp,void * data)
{
	printf("module init\r\n");
}
