#include "cartoon.h"

//XMLinstan["cartoon"] = Install_Element<cartoon>;
//static InstallXMLinstan install("cartoon",Install_Element<cartoon>);
static InstallELEinstan<cartoon> install("cartoon");

hucall int init(HUMap & mp,void * data)
{
	printf("module init\r\n");
}
