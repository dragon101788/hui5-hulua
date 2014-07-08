#include "button.h"


//XMLinstan["button"] = Install_Element<button>;
static InstallXMLinstan install("button",Install_Element<button>);


hucall int init(HUMap & mp,void * data)
{
	printf("module init\r\n");
}
