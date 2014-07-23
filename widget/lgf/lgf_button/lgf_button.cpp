#include "lgf_button.h"


//XMLinstan["button"] = Install_Element<button>;
//static InstallXMLinstan install("lgf_button",Install_Element<lgf_button>);

InstallXMLinstanWidget("lgf_button",lgf_button);

hucall int init(HUMap & mp,void * data)
{
	printf("module init\r\n");
}
