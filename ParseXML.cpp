#include "ParseXML.h"

#include <errno.h>
//#include "tinyxml/tinyxml.h"
#include "xmlproc.h"
//#include "ParseXML_comment.h"
//
//void ParaseTinyAttribute2(TiXmlNode* pchild, HUMap & xmlmp)
//{
//        if(pchild->ToElement() != 0)
//        {
//              TiXmlAttribute* attr = pchild->ToElement()->FirstAttribute();
//              if (attr)
//              {
//                      TiXmlNode* node = pchild;
//                      while (node)
//                      {
//                              while (attr)
//                              {
//
//                                      xmlmp[attr->Name()] = attr->Value();
//                                      attr = attr->Next();
//                              }
//
//                              node = node->NextSiblingElement();
//                      }
//
//              }
//        }
//
//}
//
//void ParseUpdateXml(TiXmlNode* pParent, HUMap & xmlmp)
//{
//	if (pParent == NULL)
//	{
//		return;
//	}
//
//	TiXmlNode* pchild = pParent->FirstChild();
//
//	while (pchild)
//	{
//		int t = pchild->Type();
//
////		if (t == TiXmlNode::TINYXML_COMMENT)
////		{
////		        ParseXML_comment(pchild->Value(),xmlmp);
////			pchild = pchild->NextSibling();
////			continue;
////		}
//
//		HUMap &mp = xmlmp.CreateMultiLast(pchild->Value());
//		ParaseTinyAttribute2(pchild, mp);
//
//		if (pchild->FirstChild() != NULL)
//                {
//
//
//                      if (pchild->FirstChild()->Type() == TiXmlNode::TINYXML_TEXT)
//                      {
//                            mp.m_val = pchild->FirstChild()->Value();
//                      }
//                      else if (pchild->FirstChild()->Type() == TiXmlNode::TINYXML_COMMENT)
//                      {
//                            ParseXML_comment(pchild->Value(),xmlmp);
//                            pchild = pchild->NextSibling();
//                            continue;
//                      }
//                      else if(pchild->FirstChild()->Type() == TiXmlNode::TINYXML_ELEMENT)
//                      {
//                            ParseUpdateXml(pchild, mp);
//                      }
//                      else
//                      {
//                            printf("$$$dragon$$$ incognizance type=%d\n",pchild->FirstChild()->Type());
//                      }
//                }
//		else
//		{
//		     //printf("[%s] %d is null\n",pchild->Value(),t);
//		}
//
//
//		pchild = pchild->NextSibling();
//	}
//
//}
//
//int ParseXMLFrom_Instan(hustr name, HUMap & xmlmp, xmlproc * xml);
//
//int parse_xml_file(const char *path,xmlproc * xml);
//void ParaseTinyXmlFile(const char * file, xmlproc * xml)
//{
//	TiXmlDocument doc;
//	if (!doc.LoadFile(file))
//	{
//		errexitf("ParaseTinyXmlFile LoadFile %s %s \r\n		TiXmlDocument error :%s\r\n", xml->filename.c_str(), strerror(errno),
//				doc.ErrorDesc());
//	}
//
//	TiXmlElement * root = doc.RootElement();
//
//	if (!root)
//	{
//		return;
//	}
//
//	HUMap mp;
//	ParseUpdateXml(root, mp);
//	mp.display();
//	printf("-----------------------\n");
//	parse_xml_file(file, xml);
//
//	HUMap::OrderList lst;
//	lst.accept(mp);
//	for(HUMap::OrderList::iterator it = lst.begin();it!=lst.end();++it)
//        {
//	    hustr name = (*it).m_key;
//	    ParseXMLFrom_Instan(name, *it, xml);
//        }
//}

#include<libxml/parser.h>
#include<libxml/tree.h>
int ParseXMLFrom_Instan(hustr name, HUMap & xmlmp, xmlproc * xml);
void ParseUpdateXml(xmlNodePtr node,HUMap & xmlmp)
{
  //printf("$$$print_tree");
  char value[256];
  for(node=node->children;node;node=node->next)
  {
      if(xmlIsBlankNode(node))
      {
        //printf("%s",xmlNodeGetContent(node));
      }
      else
      {
        if(XML_TEXT_NODE == node->type)
        {
            //printf("[%s:%s]\n",node->parent->name,node->content);
            (*xmlmp.m_father)[(char *)node->parent->name] = (char *)node->content;
        }
        else if(XML_CDATA_SECTION_NODE == node->type)
        {
            //printf("%s:CDATA:%s\n",node->parent->name,node->content);
            (*xmlmp.m_father)[(char *)node->parent->name] = (char *)node->content;
        }
        else if(XML_COMMENT_NODE == node->type)
        {
            //printf("%s:COMMENT:%s\n",node->parent->name,node->content);
        }
        else if(XML_ELEMENT_NODE == node->type)
        {
            //printf("%s",(char *)node->name);
            HUMap &mp = xmlmp.CreateMultiLast((char *)node->name);

            //const xmlChar *name,*value;
            xmlAttrPtr attr = node->properties;
            for(;attr;attr = attr->next)
            {
//              name=attr->name;
//              value=attr->children->content;
              mp[(char *)attr->name] = (char *)attr->children->content;
              //printf("[%s:%s]",(char*)name,(char*)value);   //get value, CDATA is not parse and don't take into value
            }
            ParseUpdateXml(node,mp);

            //printf("\n");
        }
        else
        {
          //printf("[%s] %d is blank %d\n",node->name,node->type,xmlIsBlankNode(node));
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

        ParseXmlString(content,filesize,mp);

        fclose(file);
        free(content);
        //printf("content:\n%s\n",content);
}
//int main1(int argc,char * argv[]){
//        char *content;
//        unsigned long filesize;
//        FILE *file;
//        if((file=fopen(argv[1],"r"))==NULL){
//                perror("openf file error");
//        }
//        fseek(file,0,SEEK_END);
//        filesize=ftell(file);
//        rewind(file);
//        content=(char *)malloc(filesize+1);
//        memset(content,0,filesize+1);
//        fread(content,1,filesize,file);
//        fclose(file);
//        printf("content:\n%s\n",content);
//        if(parse_xml_file(content,filesize)<0){
//                perror("parse xml failed");
//        }
//        return 0;
//}
