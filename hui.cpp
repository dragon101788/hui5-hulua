#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <signal.h>
#include "ParseXML.h"
#include "hui.h"
#include "manager_touch.h"
#include "manager_timer.h"
#include "xmlproc.h"
#include "thread_timer.h"
#include "thread_touch.h"
#include "XMLInstal.h"
#include "Framebuffer.h"
#include "loaderDL.h"
#include "dlproc.h"
#include "luaproc.h"
#include "hui_hulua.h"

using namespace std;

#define DEFAULT_XMLFILE "hu.xml"

//void post_scfg(HUMap & xmlmp, xmlproc * xml)
//{
//	if (xmlmp.exist("cus"))
//	{
//		hustr cus = xmlmp["cus"]->getvalue();
//		g_xml_proc[cus]->PostScfg(xmlmp);
//	}
//	else
//	{
//		xml->PostScfg(xmlmp);
//	}
//}

void huErrExit(const char * str)
{
	printf("**************error exit*****************\r\n");
	puts(str);
	printf("*****************************************\r\n");

	dumpstack();
	go = 0;
	g_th_timer.cancel();
	printf("wait g_th_timer OK\r\n");
	g_th_touch.cancel();
	printf("wait g_th_touch OK\r\n");
	map<hustr, pXmlproc>::iterator it;
	for (it = g_xml_proc.begin(); it != g_xml_proc.end(); ++it)
	{
		it->second->cancel();
	}
	g_cur_xml->cancel();
	exit(-1);
}

void JumpToFile(const char * jump)
{

	//pXmlproc tmp = g_cur_xml;

	if (!g_cur_xml.isNULL())
	{
		g_cur_xml->UnForeProc();

	}

	hustr math = strrchr(jump,'.');
	if(math==".so")
	{
		g_cur_xml = new dlproc(jump);
		g_cur_xml->doLoader();
	}
	if(math==".lua")
        {
	        g_cur_xml = new luaproc(jump);
	        g_cur_xml->doLoader();

        }
	else if(math==".xml")
	{
                hustr snapfile("%s.png", jump);
                printf("snapfile=%s\n",snapfile.nstr());
                if (access_Image(snapfile))
                {
                    log_i("install snapfile = %s\r\n", snapfile.c_str());
                    fb.DumpToSnap(snapfile);
                }

                g_cur_xml = new xmlproc(jump);
                g_cur_xml->doLoader();
	}


	g_cur_xml->ForeProc();
}

//BLT * g_blt = NULL;

int go = 1;

static void my_sighdlr(int sig)
{
	printf("SIG = %d\r\n", sig);
	if (sig == SIGPIPE)
	{ // Ignore SIGPIPE.
		printf("SIG = %d SIGPIPE\r\n", sig);
		return;
	}
	if (sig == SIGSEGV)
	{ // Ignore SIGPIPE.
		huErrExit("Segmentation fault\r\n");
		return;
	}

	go = 0;
}

static int _Demo_RegisterSigint()
{
	//signal(SIGINT, my_sighdlr);
	signal(SIGHUP, my_sighdlr);
	//signal(SIGQUIT, my_sighdlr);
	signal(SIGILL, my_sighdlr);
	//signal(SIGABRT, my_sighdlr);
	signal(SIGFPE, my_sighdlr);
	//signal(SIGKILL, my_sighdlr);
	signal(SIGPIPE, my_sighdlr);
	//signal(SIGTERM, my_sighdlr);
	signal(SIGSEGV, my_sighdlr);

	return 0;
}

int InitSystem()
{
	if (_Demo_RegisterSigint())
	{
		huErrExit("### Register Sigint Failed\n");
	}

	return 0;
}

void ParseCUS(HUMap & xmlmp, xmlproc * xml)
{
	hustr cus = xmlmp["xmlfile"]->getvalue();
	//g_cur_xml = &g_xml_proc[cus];
	printf("$$$HU$$$ CUS xmlfile %s\r\n", cus.c_str());
	g_xml_proc[cus] = new xmlproc(cus);
	g_xml_proc[cus]->doLoader();
}

void Jump(const char * jump)
{
  if (g_cur_xml->filename != jump)
  {
          JumpToFile(jump);
  }
}

void ParseJump(HUMap & xmlmp, xmlproc * xml)
{
	hustr jump = xmlmp["xmlfile"]->getvalue();
	Jump(jump);
}


void ParseWidget(HUMap & xmlmp, xmlproc * xml)
{
	hustr dlfile = xmlmp["dlfile"]->getvalue();
	g_dl_map[dlfile].open(dlfile);

	HUMap arg;
	g_dl_map[dlfile].Func("init",arg,NULL);
}


void ParseSAVECUS(HUMap & xmlmp, xmlproc * xml)
{
	printf("ParseSAVECUS %s\r\n", xml->filename.c_str());
	g_xml_proc[xml->filename] = g_cur_xml;
}
void ParseEnv(HUMap & xmlmp, xmlproc * xml)
{
	HUMap::iterator it;
	for (it = xmlmp.begin(); it != xmlmp.end(); ++it)
	{
		setenv(it.key().c_str(), it.value().getvalue(), 1);
		printf("%s=%s\r\n", it.key().c_str(), getenv(it.value().c_str()));
		//debug[it->second.getkey()] = it->second->getvalue_int();
	}
}

void ParseControl(HUMap & xmlmp, xmlproc * xml)
{
	hustr event = xmlmp["event"]->getvalue();

	if (event == "del")
	{
		hustr name = xmlmp["name"]->getvalue();
//		xml->timer_manager::DelElement(name);
//		xml->touch_manager::DelElement(name);

		xml->element_manager::DelElement(name);
	}
//	else if (event == "proc")
//	{
//		int ptimer=0;
//		if (xmlmp.exist("ptimer"))
//		{
//			//xml->AddExec();
//			ptimer = xmlmp["ptimer"]->getvalue_int();
//		}
//		xml->AddExec(ptimer,xmlmp);
//	}
	else if (event == "sleep")
	{
		int msec = xmlmp["msec"]->getvalue_int();
		int sec = xmlmp["sec"]->getvalue_int();
		int time = (sec * 1000 + msec) * 1000;
		printf("$$HU$$ usleep %d\r\n", time);
		usleep(time);
	}
	else if (event == "xmlproc")
	{
		if (xmlmp.exist("done"))
		{
			int done = xmlmp["done"]->getvalue_int();
			if (done)
			{
				xml->DoneProc();
			}
			else
			{
				xml->UnDoneProc();
			}
		}
		if (xmlmp.exist("fore"))
		{
			int fore = xmlmp["fore"]->getvalue_int();
			if (fore)
			{
				xml->ForeProc();
			}
			else
			{
				xml->UnForeProc();
			}
		}
//		printf("%s done=%d fore=%d\r\n", xml->filename.c_str(), xml->done,
//				xml->fore);
	}
	else if (event == "snap")
	{
		printf("event = snap\r\n");
		hustr file = xmlmp["file"]->getvalue();
		int force = xmlmp["force"]->getvalue_int();
		if (file.empty())
		{
			file.format("%s.png", xml->filename.c_str());
			printf("save default snap\r\n");
		}
		else
		{
			printf("save snap to file %s\r\n", file.c_str());
		}
		if (force)
		{
			remove(file);
		}
		xml->ScheduleSaveSnap(file);
	}
//	else if (event == "cs")
//	{
//		hustr cont = xmlmp["cont"]->getvalue();
//		if (cont == "stop")
//		{
//			printf("stop\r\n");
//			g_cur_xml->CS_manager::Stop();
//		}
//		else if (cont == "start")
//		{
//			printf("start\r\n");
//			g_cur_xml->CS_manager::Start();
//		}
//		else if (cont == "clear")
//		{
//			printf("clear\r\n");
//			//g_th_timer.create();
//			g_cur_xml->m_cs.clear();
//		}
//		if (xmlmp.exist("post"))
//		{
//			hustr post = xmlmp["post"]->getvalue();
//			if (xmlmp.exist("cus"))
//			{
//				g_xml_proc[xmlmp["cus"]->getvalue()]->PostCS(post);
//			}
//			else
//			{
//				g_cur_xml->PostCS(post);
//			}
//
//		}
//
//	}
	else if (event == "timer")
	{
		hustr cont = xmlmp["cont"]->getvalue();
		if (cont == "stopall")
		{
			xml->StopAll();
		}
		else if (cont == "startall")
		{
			xml->StartAll();
		}
	}
	else if (event == "touch")
	{
		hustr cont = xmlmp["cont"]->getvalue();
		if (cont == "lockall")
		{
			xml->LockAll();
		}
		else if (cont == "unlockall")
		{
			xml->UnLockAll();
		}
	}
}

//XMLMap xmlmp;

void ParseInclude(HUMap & xmlmp, xmlproc * xml)
{

	const char * filename = xmlmp["xmlfile"]->getvalue();
	if (xmlmp.exist("cus"))
	{
		hustr cus = xmlmp["cus"]->getvalue();
		printf("include xmlfile=[%s] to %s\r\n",  xmlmp["xmlfile"]->getvalue(), cus.c_str());
		g_xml_proc[cus]->StartParseXML(filename);

	}
	else
	{
		printf("include xmlfile=[%s] to %s\r\n", xmlmp["xmlfile"]->getvalue(), xml->filename.c_str());
		xml->StartParseXML(filename);
	}

}

//void ParseCS(HUMap & xmlmp, xmlproc * xml)
//{
//	xml->CreateCS(xmlmp["name"]->getvalue(), xmlmp);
//}

//void Parse_gcfg(HUMap & xmlmp, xmlproc * xml)
//{
//	const char * name = xmlmp["name"]->getvalue();
//	info info;
//	element * ele = xml->GetElementByName(name);
//	if (ele != NULL)
//	{
//		ele->GetInfo(info);
//	}
//	//printf("snd msg:\r\n %s\r\n", info.nstr());
//	g_th_msg.msg.send_message(101, info);
//}

void Parse_Import(HUMap & xmlmp, xmlproc * xml)
{
        if(xmlmp.exist("lua"))
        {
            const char * name = xmlmp["lua"]->getvalue();
            printf("import lua %s\n",name);
            lua.dofile(name);
        }
}

void Parse_Luado(HUMap & xmlmp, xmlproc * xml)
{
        if(xmlmp.exist("fun"))
        {
            const char * name = xmlmp["fun"]->getvalue();
            printf("lua invok func %s\n",name);
            lua.dostring(name);
        }
}

void Parse_Lua(HUMap & xmlmp, xmlproc * xml)
{
        lua.dostring(xmlmp->getvalue());
}
//HUTMap<XMLinstan_tf> XMLinstan;
void init_xml_instan()
{
	XMLinstan["include"] = ParseInclude;
	XMLinstan["control"] = ParseControl;
	XMLinstan["jump"] = ParseJump;
	XMLinstan["widget"] = ParseWidget;
	XMLinstan["cus"] = ParseCUS;
	XMLinstan["savecus"] = ParseSAVECUS;
//	XMLinstan["scfg"] = post_scfg;
//	XMLinstan["gcfg"] = Parse_gcfg;
//	XMLinstan["cs"] = ParseCS;
	XMLinstan["env"] = ParseEnv;
	XMLinstan["import"] = Parse_Import;
	XMLinstan["luado"] = Parse_Luado;
	XMLinstan["lua"] = Parse_Lua;
}
int ParseXMLFrom_Instan(hustr name, HUMap & xmlmp, xmlproc * xml)
{
	log_i("$$$HU$$$ Parse [%s]\r\n", name.c_str());
	//xmlmp.display();
	XMLinstan_tf fun = XMLinstan[name];
	if (fun != NULL)
	{
		//xml->done = 0;
		fun(xmlmp, xml);
		//xml->done = 1;
	}
	else
	{
		log_w("$$$$$HU$$$$$$$warning :: can't find xml instan when Parse %s err\r\n", name.c_str());
	}
}

void ParaseTinyXmlFile(const char * file, xmlproc * xml);

void hui_exit(const char * cmd)
{
	go = 0;

	g_th_timer.wait();
	printf("wait g_th_timer OK\r\n");
	g_th_touch.wait();
	printf("wait g_th_touch OK\r\n");

	printf("cmd=%s\r\n", cmd);
	system(cmd);
}

//#include <sys/stat.h>
//#include <unistd.h>
//#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
//#include <string.h>
int image_write_to_snap(image * img, const char * rawpath);
void Dir(hustr dir, int l)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	printf("open %s\r\n", dir.c_str());
	if ((dp = opendir(dir)) == NULL)
	{
		fprintf(stderr, "cannot open directory: %s\n", dir.c_str());
		return;
	}
	while ((entry = readdir(dp)) != NULL)
	{

		if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
			continue;

		//printf("%*s%s/\n", depth, "", entry->d_name);

		hustr subfile = dir;
		subfile += "/";
		subfile += entry->d_name;
		lstat(subfile, &statbuf);
		if (S_ISDIR(statbuf.st_mode) && l > 0)
		{
			Dir(subfile, l - 1);
		}
		else
		{
			char * p = strrchr(entry->d_name, '.');
			if (p != NULL)
			{
				hustr type = p + 1;
				if (type == "png")
				{
					image tmp;
					hustr file("%s/%s", dir.c_str(), entry->d_name);
					tmp.SetResource(file);
					tmp.LoadResource();
					printf("image_write_to_snap %s\r\n", entry->d_name);
					image_write_to_snap(&tmp, file);
				}
			}

		}

		//printf("%*s%s\n", depth, "", entry->d_name);

	}
	closedir(dp);
}





int main(int argc, char *argv[])
{

	printf("%s\r\n", __TIME__);

	char * huipid = getenv("CURHUI");
	if (huipid != NULL)
	{
		int pid = strtoul(huipid, NULL, 10);
		printf("kill %d\r\n", pid);
		kill(pid, 9);
	}
	int pid = getpid();
	setenv("CURHUI", hustr("%d", pid), 1);

	init_xml_instan();
	if (InitSystem())
	{
		errexitf("$$$HU$$$ InitSystem error %s", strerror(errno));
	}


	g_var.init_arg(argc, argv);

	int snaplevel = g_var.getvar_int("snapcache");
	if (snaplevel)
	{
		Dir(".", snaplevel - 1);
		return 0;
	}
	
	

//	hustr snapfile("%s.png", xmlfile);
//	if (access(snapfile, F_OK) == 0)
//	{
//		fbf.BlackToSnap(snapfile);
//	}

	g_th_touch.init();
	g_th_timer.create();

//	g_cur_xml = new xmlproc(xmlfile);
//	//g_xml_proc[xmlfile] = g_cur_xml;
//	g_cur_xml->ParseXMLFile();
//	g_cur_xml->fore = 1;
        const char * xmlfile = g_var.getvar("xmlfile");
        if (xmlfile != NULL)
        {
            JumpToFile(xmlfile);
        }
        else
        {
            JumpToFile("hu.xml");
        }

        //lua_command(lua);

	printf("Press Ctrl-C to exit ...\n");
	g_th_timer.wait();
	printf("wait g_th_timer OK\r\n");
	g_th_touch.wait();
	printf("wait g_th_touch OK\r\n");

	printf("demo exit %d\r\n", go);

	return 0;
}

