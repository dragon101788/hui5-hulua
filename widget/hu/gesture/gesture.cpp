#include "gesture.h"

//XMLinstan["gesture"] = Install_Element<gesture>;
static InstallXMLinstan install("gesture",Install_Element<gesture>);

hucall int init(HUMap & mp,void * data)
{
	printf("module init\r\n");
}
