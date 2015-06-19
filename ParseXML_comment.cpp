#include "ParseXML_comment.h"



char * trim(char * ptr)
{
    int start,end,i;
    if (ptr)
    {
        for(start=0; isspace(ptr[start]); start++)
            ;
        for(end=strlen(ptr)-1; isspace(ptr[end]); end--)
            ;
        for(i=start; i<=end; i++)
            ptr[i-start]=ptr[i];
        ptr[end-start+1]=0;
        return (ptr);
    }
    else
        return NULL;
}


void ParseXML_comment(const char * str,HUMap & xmlmp)
{
    printf("ParseXML_comment\n[%s]\n",str);
    if(str[0]=='#')
    {
          const char * p = str+1;
          const char * q = strchr(str,'\n');
          HUMap mp;
          char name[64]={0};
          strncpy(name,p,q-p);
          xmlmp[trim(name)] = q+1;
    }

}
