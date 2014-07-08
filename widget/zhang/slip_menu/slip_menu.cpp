#include "slip_menu.h"


hucall int init(HUMap & mp,void * data)
{
	printf("module init\r\n");
}

//	XMLinstan["Radio_buttons"] = Install_Element<Radio_buttons>;
static InstallXMLinstan install("slip_menu",Install_Element<slip_menu>);

