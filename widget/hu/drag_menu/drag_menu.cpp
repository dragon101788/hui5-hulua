#include "drag_menu.h"

//XMLinstan["drag_menu"] = Install_Element<drag_menu>;
static InstallXMLinstan install("drag_menu",Install_Element<drag_menu>);

hucall int init(HUMap & mp,void * data)
{
	printf("module init\r\n");
}
