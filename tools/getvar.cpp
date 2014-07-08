#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[])
{
	if (argc != 3)
		return -1;

	if(strlen(argv[1])<1)
	{
		return 0;
	}
	char fnd[24] = { 0 };

	sprintf(fnd,"%s=",argv[2]);

	char * p = strstr(argv[1], fnd) + strlen(fnd);
	if(p==NULL)
		return -1;
	char * q = strchr(p,'=');
	if(q==NULL)
	{
		printf("%s", p);
		return 0;
	}
	else
	{
		while(*q!=' ')
		{
			q--;
		}
		int n = (int)(q-p);
		char put[24]={0};
		strncpy(put,p,n);
		printf("%s",put);
		return 0;
	}
	return -1;
//	for(int i=0;i<argc;i++)
//	{
//		printf("%d %s\r\n",i,argv[i]);
//	}
}
