#include "ParseXML.h"

#include <errno.h>
#include "xmlproc.h"


#include<libxml/parser.h>
#include<libxml/tree.h>
int ParseXMLFrom_Instan(hustr name, HUMap & xmlmp, xmlproc * xml);

//#define DEBUG_PARSE_XML
#ifdef DEBUG_PARSE_XML
#define prase_debug(a,...)    fprintf(stderr,a,##__VA_ARGS__)
#else
#define prase_debug(a,...)
#endif

void ParseUpdateXml(xmlNodePtr node,HUMap & xmlmp)
{
  for(node=node->children;node;node=node->next)
  {
      if(xmlIsBlankNode(node))
      {
        prase_debug("%s",xmlNodeGetContent(node));
      }
      else
      {
        if(XML_TEXT_NODE == node->type)
        {
            prase_debug("[%s:%s]\n",node->parent->name,node->content);
            xmlmp.getfather()[(char *)node->parent->name] = (char *)node->content;
        }
        else if(XML_CDATA_SECTION_NODE == node->type)
        {
            prase_debug("%s:CDATA:%s\n",node->parent->name,node->content);
            xmlmp.getfather()[(char *)node->parent->name] = (char *)node->content;

        }
        else if(XML_COMMENT_NODE == node->type)
        {
            prase_debug("%s:COMMENT:%s\n",node->parent->name,node->content);
        }
        else if(XML_ELEMENT_NODE == node->type)
        {
            prase_debug("%s",(char *)node->name);
            HUMap &mp = xmlmp.CreateMultiLast((char *)node->name);

            //const xmlChar *name,*value;
            for(xmlAttrPtr attr = node->properties;attr;attr = attr->next)
            {
//              name=attr->name;
//              value=attr->children->content;
              mp[(char *)attr->name] = (char *)attr->children->content;
              prase_debug("[%s:%s]",(char*)attr->name,(char*)attr->children->content);   //get value, CDATA is not parse and don't take into value
            }
            ParseUpdateXml(node,mp);

            prase_debug("\n");
        }
        else
        {
          prase_debug("[%s] %d is blank %d\n",node->name,node->type,xmlIsBlankNode(node));
        }
      }
  }
}
int ParseXmlString(const char * buf,unsigned long filesize,HUMap & mp)
{
    xmlDocPtr doc;
    xmlNodePtr root,node;

    doc=xmlParseMemory(buf,filesize);    //parse xml in memory
    if(doc==NULL){
            printf("doc == null\n");
            return -1;
    }
    root=xmlDocGetRootElement(doc);
    for(node=root->children;node;node=node->next){
            if(xmlStrcasecmp(node->name,BAD_CAST"content")==0)
                    break;
    }
    node=root;
    if(node==NULL){
            printf("no node = content\n");
            return -1;
    }

    ParseUpdateXml(node,mp);
    xmlFreeDoc(doc);
}
int ParseXmlFile(const char *path,HUMap & mp)
{
        FILE *file = NULL;
        char *content = NULL;
        unsigned long filesize = 0;
        if((file=fopen(path,"r"))==NULL)
        {
                perror("openf file error");
        }
        fseek(file,0,SEEK_END);
        filesize=ftell(file);
        rewind(file);
        content=(char *)malloc(filesize+1);
        memset(content,0,filesize+1);
        fread(content,1,filesize,file);

        prase_debug("content:\n%s\n",content);

        ParseXmlString(content,filesize,mp);

        fclose(file);
        free(content);
}
