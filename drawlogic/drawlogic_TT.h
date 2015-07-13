#ifndef __DTAWLOGIC_BQF_H__
#define __DTAWLOGIC_BQF_H__

#include "HumapXML.h"
#include "codec.h"
#include "schedule.h"
#include <deque>
#include <set>
#include "Resource.h"
#include "element_base.h"
#include "drawlogic.h"
using namespace std;

class schedule_draw;
class element_manager;
class xmlproc;

#define luacmd_is(name) (strncmp(cmd,name,strlen(name))==0&&(cmd = cmd+strlen(name)))

class drawlogic_TT;
typedef drawlogic_TT element;
class drawlogic_TT: virtual public element_base,public schedule_ele,public ResourceContainer,virtual public Mutex
{
public:
#define PARENT_LAY  99
	template<typename T>
	static void lua_instal(lua_State* L)
	{
		  hulua::class_def<T>(L, "flush", &element::Flush);
		  hulua::class_def<T>(L, "move", &element::move);
	}

	void move(int x,int y)
	{
		cleanLastPos();
		SetX(x);
		SetY(y);
		initstack();
		Flush();
	}

	//HUMap m_mp;
	virtual void doFlushConfig(HUMap &m_mp) = 0;
	virtual void doRender() = 0;
	virtual void doDelete()
	{
		printf("warning element bash OnDelete\r\n");
	}

	int procArea(int &s_ofx,int &s_ofy,int &d_ofx,int &d_ofy, element *ele);
	int  procArea(image * dst_img, image * rsc_img, int & src_x, int & src_y, int & cp_width, int & cp_height, int & dst_x, int & dst_y,int dst_end_x,int dst_end_y);
	void renderLayersInOneEle(int s_ofx,int s_ofy,int d_ofx,int d_ofy, element *ele,int &drawCnt);
	void renderLayers();
	void cleanArea();
	void copyLayer(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y);
	void renderLayer(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y);
	void renderOut();



	void Delete()
	{
		doDelete();

		revocation();
		SetHide(1);
		Render();
	//	m_mgr->elem.erase(Get);


	}
	void onSchedule()
	{
		log_d("$$$HU$$$ Render_layer::[%s]\r\n", GetName());
		Render();
		log_d("$$$HU$$$ Render_layer::[%s] OK\r\n", GetName());
	}
	drawlogic_TT():hide_lay(-1),m_parent(NULL),m_proc(NULL),m_mgr(NULL)
	{
	        m_flag |= ELEMENT_FLAG_DRAWLOGIC;
		//log_d("new element m_parent=%x\n",m_parent);
	}

	virtual ~drawlogic_TT()
	{
		log_i("$$$HU$$$ distroy element %s\r\n", GetName());
	//	backstack();
	}
	void Flush();
	void revocation();

	void Render();
	void Back(){
		cleanLastPos();
	}
	void cleanLastPos();
	void FlushConfig(HUMap &m_mp);



	virtual void doEleLuaCommand(const char * cmd) {};

	void doLuaCommand(const char * cmd)
	{
            if(luacmd_is("hide"))
            {
                printf("hide [%s]\n",cmd);
                if(strstr(cmd,"on"))
                {
                    SetHide(1);
                    Flush();
                }
                else if(strstr(cmd,"off"))
                {
                    SetHide(0);
                    Flush();
                }
            }
            else if(luacmd_is("move"))
            {
                HUMap mp;
                StrToHUMap(cmd,mp);
                int x = mp["x"]->getvalue_int();
                int y = mp["y"]->getvalue_int();
                sscanf(cmd,"x=%d y=%d",&x,&y);
                printf("x=%d y=%d\n",x,y);
            }
            else
            {
                doEleLuaCommand(cmd);
            }
	}

	void PraseElement(HUMap &m_mp)
	{

		log_i("$$$HU$$$ ElementPrase [%s] x=%d y=%d width=%d height=%d hide=%d\r\n",
				GetName(),GetX(), GetY(), GetWidth(), GetHeight(), GetHeight());

		if(m_mp.exist("element_manager"))
                {
                    m_mgr = (element_manager *)m_mp["element_manager"].value().m_data;
                }
                if(m_mp.exist("parent"))
                {
                    element * parent = (element *)m_mp["parent"].value().m_data;
                    log_d("%s sub element\n",parent->GetName());
                    m_parent = parent;
                }

                //printf("%s SetBuffer width=%d height=%d\r\n", name.c_str(), width, height);
         //       SetBuffer(GetWidth(), GetHeight());

		initstack();
	}

	class Cmpare
	{
	public:
		bool operator()(const element * st1, const element * st2) const
		{
			return (st1->GetLay()) < (st2->GetLay());
		}
	};
	void initstack();
		void addLayers(element * ele)
	{
		list<element *>::iterator it;
		for (it = layers.begin(); it != layers.end(); ++it)
		{
			if (*it == ele)
			{
				break;
			}
		}
		if (it == layers.end())
		{
			//log_i("$$$HU$$$ [%s] add [%s] ET\r\n",name.c_str(),ele->name.c_str());
			layers.push_back(ele);
			layers.sort(Cmpare());
		}
	}

	void delLayers(element * ele)
	{
		layers.remove(ele);
	}

	void setOutImage(image * out){
		if(out->isNULL()){
			out->SetBuffer(GetWidth(),GetHeight());
		}
		render_res[PARENT_LAY].setRes(out, 0, 0,out->GetImageWidth(),out->GetImageHeight(),  0, 0);
	}

	void setDefaultOutImage(){
		if(top_image.isNULL()){
			top_image.SetBuffer(GetWidth(),GetHeight());
		}
		render_res[PARENT_LAY].setRes(&top_image, 0, 0,top_image.GetImageWidth(),top_image.GetImageHeight(),  0, 0);
	}


	image * getOutImage(){
		if(render_res[PARENT_LAY].img==NULL){
			setDefaultOutImage();
		}
		return render_res[PARENT_LAY].img;
	}


	/*
	 * layer:将图片绘制到本元素的第几层
	 */
	void RenderToSelf(image * src_img, int dst_x, int dst_y,int layer){
		render_res[layer].setRes( src_img, 0, 0,GetWidth(), GetHeight(),  dst_x, dst_y);
	}

	 void RenderToSelf(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y,int layer)
	{
		 render_res[layer].setRes( src_img, src_x, src_y,cp_width, cp_height,  dst_x, dst_y);
	}
	class LayerRes
	{
	public:
		int dst_x;
		int dst_y;
		int offset_x;
		int offset_y;
		int cp_width;
		int cp_height;
		image *img;
		LayerRes():img(NULL),offset_x(0),offset_y(0),dst_x(0),dst_y(0){

		}



		void setRes(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y){
			img=src_img;
			offset_x=src_x;
			offset_y=src_y;
			this->cp_width=cp_width;
			this->cp_height=cp_height;
			this->dst_x=dst_x;
			this->dst_y=dst_y;
		}


		/******************
		 * 图像平移
		 */

		inline void  translate(int dx, int dy){
			dst_x=dx;
			dst_y=dy;
		}
	};

	bool hasParent()const{
		return m_parent!=NULL;
	}

	map<int, LayerRes> render_res;//由原来的单一指针改为指针map，可以在doRender里面同时绘制几层图
	list<element *> layers;     //底顶合一队列
	element * m_parent;
	xmlproc * m_proc;
	int hide_lay;//向对此元素此层隐藏
	element_manager * m_mgr;
	image top_image;//元素的最高一层，专门用来绘制子空间
};

//后续将绘图元素剥离出来
class element_manager
{
public:
	virtual void AddElement(const char * name, element * ele)
	{

		if (GetElementByName(name) == NULL)
		{
			elem[name] = ele;
		}
		else
		{
			printf("Repeat to add element %s\r\n", name);
		}

	}
	void ClearElement()
	{
		elem.clear();
	}
	void DelElement(const char * name)
	{
		element * ele = GetElementByName(name);
		if (ele == NULL)
		{
			printf("can't del layer element %s\r\n", name);
		}
		else
		{
			ele->Delete();
			elem.erase(name);
		}
	}
	element * GetElementByName(const char * name)
	{
		map<hustr, SmartPtr<element> >::iterator it;
		it = elem.find(name);
		if (it != elem.end())
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}
	typedef map<hustr, SmartPtr<element> >::iterator iterator;
	iterator begin()
	{
		return elem.begin();
	}
	iterator end()
	{
		return elem.end();
	}
	element_manager()
	{
	}
	virtual ~element_manager()
	{
	}
	map<hustr, SmartPtr<element> > elem; //ʹ������ָ��

};

#endif //__DTAWLOGIC_BQF_H__
