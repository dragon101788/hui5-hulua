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


void ParseXML_comment(const char * str,xmlproc * xml)
{
    const char * p = str;
    const char * q = strchr(str,'\n');
    char buf[256] = {0};
    strncpy(buf,p,q-p);
    if(strncmp(buf,VALID_XML_COMMENT,strlen(VALID_XML_COMMENT))==0)
    {
        HUMap mp;
        mp.m_key = trim(buf)+strlen(VALID_XML_COMMENT);
        mp.m_val = q+1;
        //printf("ParseXML_comment key=[%s] value=[%s]\n",mp.m_key.nstr(),mp->getvalue());
        ParseXMLFrom_Instan(mp.m_key.nstr(), mp, xml);
    }
}
