#ifndef __XML_PORG_H__
#define __XML_PORG_H__

#include "hulib.h"
#include "drawlogic.h"
#include "manager_touch.h"
#include "manager_timer.h"
#include "Framebuffer.h"
#include "HumapXML.h"

class xmlproc;


typedef SmartPtr<xmlproc> pXmlproc;
extern pXmlproc g_cur_xml;
extern int go;
int ParseXMLFrom_Instan(hustr name, HUMap & xmlmp, xmlproc * xml);
void hui_exit(const char * cmd);
void JumpToFile(const char * jump, const char * snap);

extern DebugTimer fps;


//
//typedef HUTimer<HuExec> ProcTimer;
//extern ProcTimer g_exec;

class xmlproc : public element_manager,
		public timer_manager,
		public touch_manager,
		public schedule_draw,
		public thread,
//		public ProcTimer::HUTimerContainer,
		virtual public Mutex
{
private:
	class xmlout: public image
	{
	public:
		void Draw(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
		{
			lock();
			AreaCopyFrom(src_img, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
			unlock();
		}
		void RenderToFramebuffer(framebuffer * fb)
		{
			lock();
			fb->RenderImageToFrameBuffer(this);
			unlock();
		}
	};
	xmlout out;

	class save_snap: public schedule_ele
	{
	public:
		xmlproc * m_xml;
		hustr name;
		save_snap(xmlproc * xml, const char * path)
		{
			m_xml = xml;
			name = path;
		}
		~save_snap()
		{
			printf("~snap\r\n");
		}
		int SaveSnap(const char * file)
		{
			if (!access_Image(file))
			{
				printf("%s no exist , save Snap !!!\r\n", file);
				m_xml->out.Save(file);
				return 1;

			}
			else
			{
				printf("%s exist , no save Snap !!!\r\n", file);
				return 0;
			}
		}
		void onSchedule()
		{
			SaveSnap(name);
			delete this;
		}
	};
	int fore; //前台
	int done; //完成解析
	int m_exit; //线程退出

	int isDraw; //有改变图像
public:
	void Draw(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
	{
		//lock();
		out.Draw(src_img, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
		isDraw++;
		//unlock();
	}

	hustr filename;

	void ForeProc(); //将页面切换前台
	void UnForeProc(); //将页面切换后台
	void DoneProc(); //强制完成
	void UnDoneProc(); //强制未完成

	static int timerfun_run(int tm, hustr cmd)
	{
		system(cmd);
	}

	int ScheduleSaveSnap(const char * file)
	{
		que.addele(new save_snap(this, file));
	}
	int run()
	{
		while (go && m_exit)
		{
			int ret = ScheduleProc();
			ProcDraw();
			FPSWaitFPS(25);
		}
	}
	void ProcDraw()
	{
		if (fore == 0 || isDraw == 0)
			return;

		lock();
		if (isDraw != 0 && fore == 1 && done == 1)
		{
			//printf("%s RenderFromBuffer\r\n",filename.c_str());
			out.RenderToFramebuffer(&fb);

//#ifdef CONFIG_DEBUG_FPS
			  fps.debug_timer("<fps>");
//#endif
			isDraw = 0;
		}
		unlock();
	}
	void ProcTouch(touch_sample * samp)
	{
		if (fore == 0)
			return;

		lock();
		if (fore == 1 && done == 1)
		{
			touch_proc_event(samp);
		}
		else
		{
			printf("ProcTouch [%s] Not ready to touch fore=%d done=%d\r\n", filename.c_str(), fore, done);
		}
		unlock();
	}
	void ProcTimer()
	{
		if (fore == 0)
			return;

		lock();
		if (fore == 1 && done == 1)
		{
			UpdateTimer();
		}
		else
		{
			//printf("ProcTimer %s Not ready to timer fore=%d done=%d\r\n", filename.c_str(), fore, done);
		}
		unlock();
	}
	virtual void doLoader()
	{
                if (filename.empty())
                {
                        huErrExit("can't init filename\r\n");
                }
		StartParseXML_file(filename);
	}
	void StartParseXML_file(const char * file);
	void StartParseXML_str(const char * file);
//	void ParseXMLElementFile(const char * file)
//	{
//	    ParseXMLFile(file, this);
//	}

	virtual ~xmlproc()
	{
		printf("~xmlproc %s\r\n", filename.c_str());
		Destroy();
	}

	int init();

	xmlproc()
	{
		init();
	}
	xmlproc(const char * file)
	{
		init();
		filename = file;

	}

	void Destroy()
	{

		debug("---------------%s----------------\r\n", filename.c_str());
		//system("ps|grep hui|sed -n \'1 p\'");

		UnForeProc();
		UnDoneProc();

		m_exit = 0;

		lock();
		printf("Destroy wait thread \r\n");
		wait();

		debug("Destroy wait timer_manager::ClearElement \r\n");
		timer_manager::ClearElement();

		debug("Destroy wait touch_manager::ClearElement \r\n");
		touch_manager::ClearElement();

		debug("Destroy wait element_manager::ClearElement \r\n");
		element_manager::ClearElement();
		//system("ps|grep hui|sed -n \'1 p\'");
		out.destroy();
		unlock();
		debug("---------------%s----------------OK\r\n", filename.c_str());

	}
//	int LoadSnap(const char * file)
//	{
//		if (access(file, F_OK) == 0)
//		{
//			printf("%s exist Load Snap !!!\r\n", file);
//			out.SetResource(file);
//			out.LoadResource();
//			return 1;
//		}
//		else
//		{
//			return 0;
//		}
//
//	}
//	int SaveSnap(const char * file)
//	{
//		if (access(file, F_OK) != 0)
//		{
//			printf("%s no exist , save Snap !!!\r\n", file);
//			out.SaveResource(file);
//			return 1;
//		}
//		else
//		{
//			printf("%s exist , no save Snap !!!\r\n", file);
//			return 0;
//		}
//	}
};

#endif //__XML_PORG_H__
