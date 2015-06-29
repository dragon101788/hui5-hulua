#include "HumapXML.h"

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

void HumapFromXmlLoopDec(xmlNodePtr node,HUMap & xmlmp)
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
            xmlmp.m_val = (char *)node->content;
        }
        else if(XML_CDATA_SECTION_NODE == node->type)
        {
            prase_debug("%s:CDATA:%s\n",node->parent->name,node->content);
            xmlmp.m_val += (char *)node->content;

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
            HumapFromXmlLoopDec(node,mp);

            prase_debug("\n");
        }
        else
        {
          prase_debug("[%s] %d is blank %d\n",node->name,node->type,xmlIsBlankNode(node));
        }
      }
  }
}

void HumapToXmlLoopEnc(xmlNodePtr node,HUMap & mp)
{

    HUMap::OrderList lst(mp);
    for(HUMap::OrderList::iterator it = lst.begin();it!=lst.end();++it)
    {
      const char * name = (*it).MapName();
      const char * value = (*it).MapValue();
      HUMap * father = (*it).m_father;
      if(strchr(value,'&'))
      {//CDATA
          xmlNodePtr node1 = xmlNewNode(NULL, BAD_CAST (name));
          xmlNodePtr cdata = xmlNewCDataBlock(NULL,BAD_CAST(value),strlen(value));
          xmlAddChild(node1, cdata);
          xmlAddChild(node, node1);
      }
      else if(strlen(value)<1)
      {//node
          xmlNodePtr node1 = xmlNewNode(NULL, BAD_CAST (name));
          HumapToXmlLoopEnc(node1, (*it));
          xmlAddChild(node, node1);
      }
      else if(strlen(value)<30&&father->count(name)==1)
      {//ATTTIBUTE
          xmlNewProp(node, BAD_CAST(name), BAD_CAST(value));
      }
      else
      {//TEXT
          xmlNewChild(node, NULL, BAD_CAST(name),BAD_CAST(value));
      }


    }
}
int HumapToXmlString(hustr & outstr,HUMap & mp)
{
    //mp.display();
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL, node = NULL;
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST(mp.MapName().nstr()));
    xmlDocSetRootElement(doc, root_node);

    HumapToXmlLoopEnc(root_node,mp);

    xmlChar *outbuf;
    int outlen;
    xmlDocDumpFormatMemoryEnc(doc, &outbuf, &outlen, "UTF-8", 1);
    outstr = (const char *)outbuf;
    xmlFree(outbuf);


    xmlFreeDoc(doc);

    xmlCleanupParser();

    xmlMemoryDump();

    return(0);

}

int HumapToXmlFile(const char * path,HUMap & mp)
{
    //mp.display();
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL, node = NULL;
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST(mp.MapName().nstr()));
    xmlDocSetRootElement(doc, root_node);

    HumapToXmlLoopEnc(root_node,mp);

    xmlSaveFormatFileEnc(path, doc, "UTF-8", 1);

    xmlFreeDoc(doc);

    xmlCleanupParser();

    xmlMemoryDump();

    return(0);

}
int HumapFromXmlString(const char * buf,unsigned long filesize,HUMap & mp)
{
    xmlDocPtr doc;
    xmlNodePtr node;

    doc=xmlParseMemory(buf,filesize);    //parse xml in memory
    if(doc==NULL){
            printf("doc == null\n");
            return -1;
    }
    node=xmlDocGetRootElement(doc);
    if(node==NULL){
            printf("no node = content\n");
            return -1;
    }
    mp.MapName() = (char *)node->name;
    HumapFromXmlLoopDec(node,mp);
    xmlFreeDoc(doc);
}

int HumapFromXmlFile(const char *path,HUMap & mp)
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

        HumapFromXmlString(content,filesize,mp);
        HumapToXmlFile(hustr("out/%s",path),mp);

        fclose(file);
        free(content);
}
