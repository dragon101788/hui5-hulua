#ifndef __THREAD_MSG_H__
#define __THREAD_MSG_H__

extern int go;

#include "xmlproc.h"
#include "HuMsg.h"

void ParseUpdateXml3(TiXmlNode* pParent, xmlproc * xml);
int ParseXMLElement(HUMap & xmlmp);
void GetInfo(const char * name, info & info);

class msg_thread: public thread
{
public:
	Msg msg;
	//thread_debug tdbg;
	int PostMsg(const char * str)
	{
		msg.send_message(100, str);
	}
	int run()
	{
		msg.init_message();

		while (go)
		{
			hu_msgbuf qbuf;
			msg.read_message(100, &qbuf);
			TiXmlDocument xml;
			xml.Parse(qbuf.mtext);
			ParseUpdateXml3(&xml, g_cur_xml);
		}
		printf("msg thread exit\r\n");
	}
};

extern msg_thread g_th_msg;
//class ExeMsg
//{
//	hustr snd;
//	int cnt;
//public:
//	operator const char *()
//	{
//		return snd.c_str();
//	}
//	ExeMsg()
//	{
//		cnt = 0;
//	}
//	int start()
//	{
//		if (cnt != 0)
//		{
//			printf("ExeMsg::start %s\r\n", snd.nstr());
//			g_th_msg.PostMsg(snd);
//		}
//		return cnt;
//	}
//	void add(const char * key, const char * value)
//	{
//		//printf("ExeMsg add %s %s\r\n",key,value);
//		snd += key;
//		snd += "=\"";
//		snd += value;
//		snd += "\" ";
//		cnt++;
//	}
//	void add(HUMap & mp, const char * str)
//	{
//		if (mp.exist(str))
//		{
//			add(str, mp[str].getvalue());
//		}
//	}
//	void parase(HUMap & mp)
//	{
//		snd = "<control event=proc ";
////		HUMap::iterator it;
////		for (it = mp.begin(); it != mp.end(); ++it)
////		{
////			add(it->second.getkey(), it->second.getvalue());
////		}
//		add(mp, "run");
//		add(mp, "run_bg");
//		add(mp, "jump");
//		add(mp, "cs");
//		snd += "/>";
//		debug("executable::parase %s\r\n", snd.nstr());
//	}
//};

#endif //__THREAD_MSG_H__