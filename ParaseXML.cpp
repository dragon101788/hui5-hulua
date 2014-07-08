#include "ParaseXML.h"

#include <errno.h>
#include "tinyxml/tinyxml.h"
#include "xmlproc.h"

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

void ParaseUpdateXml2(TiXmlNode* pParent, HUMap & xmlmp)
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
			ParaseUpdateXml2(pchild, mp);
		}

		pchild = pchild->NextSibling();
	}

}

int ParseXMLElement2(hustr name, HUMap & xmlmp, xmlproc * xml);
void ParaseUpdateXml3(TiXmlNode* pParent, xmlproc * xml)
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

		const char * name = pchild->Value();
		HUMap mp(name);

		ParaseTinyAttribute2(pchild, mp);

		if (pchild->FirstChild() != NULL && pchild->FirstChild()->Type() == TiXmlNode::TINYXML_TEXT)
		{
			mp.m_val = pchild->FirstChild()->Value();
		}

		if (pchild->FirstChild() == NULL || pchild->FirstChild()->Type() == TiXmlNode::TINYXML_ELEMENT)
		{
			ParaseUpdateXml2(pchild, mp);
		}

		ParseXMLElement2(name, mp, xml);

		pchild = pchild->NextSibling();
	}

}

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

	ParaseUpdateXml3(root, xml);

}

