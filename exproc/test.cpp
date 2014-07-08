#include <stdio.h>
#include <stdlib.h>
#include "dlproc.h"
#include "control/static_image.h"
#include "control/button.h"



extern "C" int onLoader(dlproc * dlproc)
{
	printf("testproc doLoader\r\n");
	element * te;
	te = new static_image;
	te->m_mp["name"] = "bk";
	te->m_mp["width"] = "480";
	te->m_mp["height"] = "272";
	te->m_mp["x"] = "0";
	te->m_mp["y"] = "0";
	te->m_mp["lay"] = "0";
	te->m_mp["node"] = "main/bk.png";
	te->xml_mgr = dlproc;
	te->mgr = dlproc;
	te->FlushConfig();

	te = new button;
	te->m_mp["name"] = "doujiang";
	te->m_mp["width"] = "147";
	te->m_mp["height"] = "130";
	te->m_mp["x"] = "17";
	te->m_mp["y"] = "51";
	te->m_mp["run"] = "./xzfl-dj.sh";
	te->m_mp["up"] = "main/doujiang1.png";
	te->m_mp["dn"] = "main/doujiang1.png";
	te->xml_mgr = dlproc;
	te->mgr = dlproc;
	te->FlushConfig();

	te = new button;
	te->m_mp["name"] = "wuguying";
	te->m_mp["width"] = "147";
	te->m_mp["height"] = "130";
	te->m_mp["x"] = "167";
	te->m_mp["y"] = "51";
	te->m_mp["run"] = "./xzfl-wgy.sh";
	te->m_mp["up"] = "main/wuguying1.png";
	te->m_mp["dn"] = "main/wuguying2.png";
	te->xml_mgr = dlproc;
	te->mgr = dlproc;
	te->FlushConfig();

	te = new button;
	te->m_mp["name"] = "shipu";
	te->m_mp["width"] = "147";
	te->m_mp["height"] = "130";
	te->m_mp["x"] = "317";
	te->m_mp["y"] = "51";
	te->m_mp["run"] = "./szsp.sh";
	te->m_mp["up"] = "main/shipu1.png";
	te->m_mp["dn"] = "main/shipu2.png";
	te->xml_mgr = dlproc;
	te->mgr = dlproc;
	te->FlushConfig();

	te = new button;
	te->m_mp["name"] = "jiyinshui";
	te->m_mp["width"] = "120";
	te->m_mp["height"] = "90";
	te->m_mp["x"] = "0";
	te->m_mp["y"] = "182";
	te->m_mp["run"] = "./jys.sh";
	te->m_mp["up"] = "main/jiyinshui1.png";
	te->m_mp["dn"] = "main/jiyinshui2.png";
	te->xml_mgr = dlproc;
	te->mgr = dlproc;
	te->FlushConfig();

	te = new button;
	te->m_mp["name"] = "qingxi";
	te->m_mp["width"] = "120";
	te->m_mp["height"] = "90";
	te->m_mp["x"] = "120";
	te->m_mp["y"] = "182";
	te->m_mp["run"] = "./zdqx.sh";
	te->m_mp["up"] = "main/qingxi1.png";
	te->m_mp["dn"] = "main/qingxi2.png";
	te->xml_mgr = dlproc;
	te->mgr = dlproc;
	te->FlushConfig();

	te = new button;
	te->m_mp["name"] = "yuyue";
	te->m_mp["width"] = "120";
	te->m_mp["height"] = "90";
	te->m_mp["x"] = "240";
	te->m_mp["y"] = "182";
	te->m_mp["run"] = "./yyxzsp.sh";
	te->m_mp["up"] = "main/yuyue1.png";
	te->m_mp["dn"] = "main/yuyue2.png";
	te->xml_mgr = dlproc;
	te->mgr = dlproc;
	te->FlushConfig();

	te = new button;
	te->m_mp["name"] = "shezhi";
	te->m_mp["width"] = "120";
	te->m_mp["height"] = "90";
	te->m_mp["x"] = "360";
	te->m_mp["y"] = "182";
	te->m_mp["run"] = "./sz1.sh";
	te->m_mp["up"] = "main/shezhi1.png";
	te->m_mp["dn"] = "main/shezhi2.png";
	te->xml_mgr = dlproc;
	te->mgr = dlproc;
	te->FlushConfig();
}
