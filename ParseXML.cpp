#include "ParseXML.h"

#include <errno.h>
#include "tinyxml/tinyxml.h"
#include "xmlproc.h"
#include "ParseXML_comment.h"

void ParaseTinyAttribute2(TiXmlNode* pchild, HUMap & xmlmp)
{
	TiXmlAttribute* attr = pchild->ToElement()->FirstAttribute();
	if (attr)
	{
		TiXmlNode* node = pchild;
		while (node)
		{
			while (attr)
			{

				xmlmp[attr->Name()] = attr->Value();
				attr = attr->Next();
			}

			node = node->NextSiblingElement();
		}

	}
}

void ParseUpdateXml(TiXmlNode* pParent, HUMap & xmlmp)
{
	if (pParent == NULL)
	{
		return;
	}

	TiXmlNode* pchild = pParent->FirstChild();

	while (pchild)
	{
		int t = pchild->Type();

		if (t == TiXmlNode::TINYXML_COMMENT)
		{
			pchild = pchild->NextSibling();
			continue;
		}

		HUMap &mp = xmlmp.CreateMultiLast(pchild->Value());

		ParaseTinyAttribute2(pchild, mp);

		if (pchild->FirstChild() != NULL && pchild->FirstChild()->Type() == TiXmlNode::TINYXML_TEXT)
		{
			mp.m_val = pchild->FirstChild()->Value();
		}

		if (pchild->FirstChild() == NULL || pchild->FirstChild()->Type() == TiXmlNode::TINYXML_ELEMENT)
		{
			ParseUpdateXml(pchild, mp);
		}

		pchild = pchild->NextSibling();
	}

}

int ParseXMLFrom_Instan(hustr name, HUMap & xmlmp, xmlproc * xml);


void ParaseTinyXmlFile(const char * file, xmlproc * xml)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(file))
	{
		errexitf("ParaseTinyXmlFile LoadFile %s %s \r\n		TiXmlDocument error :%s\r\n", xml->filename.c_str(), strerror(errno),
				doc.ErrorDesc());
	}

	TiXmlElement * root = doc.RootElement();

	if (!root)
	{
		return;
	}

	HUMap mp;
	ParseUpdateXml(root, mp);
	//mp.display();
	HUMap::OrderList lst;
	lst.accept(mp);
	for(HUMap::OrderList::iterator it = lst.begin();it!=lst.end();++it)
        {
	    hustr name = (*it).m_key;
	    ParseXMLFrom_Instan(name, *it, xml);
        }
}

