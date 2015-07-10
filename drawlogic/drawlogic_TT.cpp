#include "drawlogic_TT.h"
#include "xmlproc.h"
#include "ttf_font.h"
#include "include/autoconf.h"
#include "alpha.h"

void areaCopy_no_procArea(image * dst_img, image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
{
	//int x;
	int y;

	if (dst_img->SrcGPUAddr() == 0 || src_img->SrcGPUAddr() == 0)
		{
		//	log_w("warning::Image source point is NULL dst=%#x src=%#x\r\n", dst_img->srcGPUAddr(), src_img->srcGPUAddr());
			return;
		}

//	if(ProcArea(dst_img, src_img, src_x, src_y, cp_width, cp_height, dst_x, dst_y)){
//		return ;
//	}
	dst_img->lock();
	src_img->lock();
	int line_byte=cp_width<<2;

//	unsigned int dst_step= dst_img->u32Width;
//	unsigned int src_step= src_img->u32Width;
	unsigned int * dst_start=(unsigned int *)dst_img->GetAddr() +  dst_y * dst_img->GetImageWidth() + dst_x;
	unsigned int * src_start=(unsigned int *)src_img->GetAddr() +  src_y * src_img->GetImageWidth() + src_x;
	unsigned int dst_offset=0;
	unsigned int src_offset=0;
	for (y = 0; y < cp_height; y++)
	{
		memcpy( dst_start+dst_offset,
				 src_start+src_offset, line_byte);
		dst_offset+=dst_img->GetImageWidth();
		src_offset+=src_img->GetImageWidth();
	}

	dst_img->unlock();
	src_img->unlock();
}



void element::Flush()
{
	if (m_proc != NULL)
	{
		lock();
		m_proc->que.addele(this);
		unlock();
	}
	else
	{
		errexitf("$$$$HU$$$$ Flush element %s manager is NULL\r\n",
				GetName());
	}
}
void element::revocation()
{
	if (m_proc != NULL)
	{
		lock();
		m_proc->que.delele(this);
		unlock();
	}
	else
	{
		errexitf("$$$$HU$$$$ revocation element %s manager is NULL\r\n",
				GetName());
	}
}

int crossAlgorithm(element * r1, element * r2)
{
	int nMaxLeft = 0;
	int nMaxTop = 0;
	int nMinRight = 0;
	int nMinBottom = 0;

	//计算两矩形可能的相交矩形的边界
	nMaxLeft = r1->GetX() >= r2->GetX() ? r1->GetX() : r2->GetX();
	nMaxTop = r1->GetY() >= r2->GetY() ? r1->GetY() : r2->GetY();
	nMinRight =
			(r1->GetX() + r1->GetImageWidth()) <= (r2->GetX() + r2->GetImageWidth()) ?
					(r1->GetX() + r1->GetImageWidth()) : (r2->GetX() + r2->GetImageWidth());
	nMinBottom =
			(r1->GetY() + r1->GetImageHeight()) <= (r2->GetY() + r2->GetImageHeight()) ?
					(r1->GetY() + r1->GetImageHeight()) : (r2->GetY() + r2->GetImageHeight());
	// 判断是否相交
	//printf("crossAlgorithm %d %d %d %d\r\n",nMaxLeft , nMinRight , nMaxTop , nMinBottom);
	if (nMaxLeft >= nMinRight || nMaxTop >= nMinBottom){

		return 0;
	}
	else
	{
		return 1;
	}
}


void element::initstack()
{
	element_manager::iterator it;
	for (it = m_mgr->begin(); it != m_mgr->end(); ++it)
	{
		element * ele = it->second;
//		if(ele->parent==parent)
//		{  //只有当在同一父亲下，或父亲都为0，才相互作用
			if (crossAlgorithm(ele, this))
			{
				if(ele->GetLay() != GetLay()){
					addLayers(ele);   //区域内有重叠元素，并且层小于自己则加入队列
					ele->addLayers(this); //将自己加入队列
				}
			}
		//}
	}
	addLayers(this);  //自己也要加在自己的队列中
}




void element::renderLayersInOneEle(int s_ofx,int s_ofy,int d_ofx,int d_ofy, element *ele,int &drawCnt){
	map<int,LayerRes>::iterator itp;
	LayerRes *lay_res;
	for (itp = ele->render_res.begin();itp != ele->render_res.end(); itp++)
	{
		lay_res=&itp->second;
		//log_i("itp->first=%d\n",itp->first);
		int final_s_off_x=s_ofx+lay_res->offset_x;
		int final_s_off_y=s_ofy+lay_res->offset_y;
		int final_d_off_x=d_ofx+lay_res->dst_x;
		int final_d_off_y=d_ofy+lay_res->dst_y;
		//log_i("+++++final_s_off_y=%d\n",final_s_off_y);
		if(lay_res->dst_x>=ele->GetImageWidth()){
			int reduce=(lay_res->dst_x/ele->GetImageWidth())*ele->GetImageWidth();
			final_s_off_x-=reduce;
			final_d_off_x-=reduce;
		}
		if(lay_res->dst_y>=ele->GetImageHeight()){
			int reduce=(lay_res->dst_y/ele->GetImageHeight())*ele->GetImageHeight();
			final_s_off_y-=reduce;
			final_d_off_y-=reduce;
		}
	//	log_i("+++++final_s_off_y=%d\n",final_s_off_y);
	//	if(itp == ele->render_res.begin())
		if(drawCnt==0)
			copyLayer(lay_res->img, final_s_off_x, final_s_off_y, lay_res->cp_width,lay_res->cp_height,final_d_off_x, final_d_off_y);
		else
			renderLayer(lay_res->img, final_s_off_x, final_s_off_y, lay_res->cp_width,lay_res->cp_height,final_d_off_x, final_d_off_y);
		drawCnt++;
	}
//	if(ele->isParent()){
//		int final_s_off_x=s_ofx+ele->scroll_x;
//		int final_s_off_y=s_ofy+ele->scroll_y;
//		if(drawCnt==0){
//			copyLayer(&ele->top_image,  final_s_off_x, final_s_off_y, render_width,GetImageHeight(), d_ofx, d_ofy);
//		}else
//			renderLayer(&ele->top_image, final_s_off_x, final_s_off_y, render_width,render_height, d_ofx, d_ofy);
//		drawCnt++;
//	}

}
void element::renderLayers()
{
	//log_i("iiiiiiiiiiiiiiii %s renderLayers in \n",name.c_str());
	int cnt=0;//绘制了几次
	if (!layers.empty()) //如果队列不为空，则进行绘制
	{
		list<element *>::iterator it;
		element * ele;
		int s_ofx ; //源x
		int d_ofx ; //目标x
		int s_ofy ; //源x
		int d_ofy ; //目标x
		for (it = layers.begin(); it != layers.end(); ++it)
		{
			ele = *it;
/*********
* 如果ele->hide=1；显然无需绘制它
* 如果ele->lay==hide_lay；即此元素所在层正好为本元素屏蔽掉的层，那么也不显示它
*/
			if (ele->GetHide() == 0&&ele->GetLay()!=hide_lay)
			{
				//log_i("$$$HU$$$ RenderEB %s <-- %s\r\n", name.c_str(), ele->name.c_str());
				 s_ofx = 0; //源偏移x
				 d_ofx = 0; //目标偏移x
				 if(procArea(s_ofx,s_ofy,d_ofx,d_ofy,ele))
					 continue;
				 renderLayersInOneEle( s_ofx, s_ofy,d_ofx,d_ofy,ele,cnt);
			}
		}
		if((cnt==0)&&GetHide()){ //没有队列，为了清除原状态。否则会显示上一次的状态
			cleanArea();
		}
	}
}

void element::Render()
{
	//log_i("~~~%s RenderOut in~~~\n",name.c_str());
	lock();
//	cfgPartRender();
	//log_i("~~~%s render_offset_x=%d,render_offset_y=%d,render_width=%d,render_height=%d~~~\n",name.c_str(),render_offset_x,render_offset_y,render_width,render_height);

	if (GetHide() == 0)
	{
		doRender();
	}
	else
	{
		//render_res.clear();
	//	log_i("Render %s hide\r\n", name.c_str());
	}
	//log_i("~~~%s RenderOut before renderLayers~~~\n",name.c_str());
	renderLayers();  //如果自己隐藏的话，此函数是不会绘制自己的。
	//log_i("~~~%s RenderOut after renderLayers~~~\n",name.c_str());
//	if(parent!=NULL){
//		parent->addDraw(x+render_offset_x,y+render_offset_y,render_width,render_height);
//		if(!parent->isParent()){
//			parent->tobeParent(name,this);
//		}
//		if(render_cached)
//			parent->flush();
//		else
//			parent->renderOut();
//
//	}else{

	m_proc->addDraw();
//	}
	//log_i("~~~%s RenderOut leave~~~\n",name.c_str());
	unlock();
}


void element::cleanLastPos()
{
	lock();
//	render_offset_x=0;
//	render_offset_y=0;
//	render_width=width;
//	render_height=height;
 //  log_i("%s cleanLastPos !!\n",name.c_str());
   int temp_hide=GetHide();
   SetHide(1);
	renderLayers();  //如果自己隐藏的话，此函数是不会绘制自己的。
	SetHide(temp_hide);
//	if(parent!=NULL){
//		parent->addDraw(x,y,render_width,render_height);
//		if(!parent->isParent()){
//			parent->tobeParent(name,this);
//		}
//		parent->cleanLastPos_by_parent();
//	}
	unlock();
}

void element::copyLayer(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
{
	int _dst_x=dst_x+GetX();
	int _dst_y=dst_y+GetY();
	//log_i("####in copyLayer####\n");
//	if(hasParent()){
//
//		if(procArea(&parent->top_image, src_img, src_x, src_y, cp_width, cp_height, _dst_x, _dst_y,x+width,y+height))
//			 return;
//	//	log_i("####%s src_x=%d, src_y=%d, cp_width=%d, cp_height=%d, _dst_x=%d, _dst_y=%d####\n",name.c_str(),src_x, src_y, cp_width, cp_height, _dst_x, _dst_y);
//		::areaCopy_no_procArea(&parent->top_image ,src_img,src_x, src_y, cp_width, cp_height,_dst_x,_dst_y);
//		//::Copy_img_to_img(&parent->top_image ,src_img,src_x, src_y, cp_width, cp_height,_dst_x,_dst_y);
//	}else{
	    if(procArea(&m_proc->out, src_img, src_x, src_y, cp_width, cp_height, _dst_x, _dst_y,GetX()+GetWidth(),GetY()+GetHeight()))
			 return;
	//	log_i("####%s src_x=%d, src_y=%d, cp_width=%d, cp_height=%d, _dst_x=%d, _dst_y=%d####\n",name.c_str(),src_x, src_y, cp_width, cp_height, _dst_x, _dst_y);
		::areaCopy_no_procArea(&m_proc->out, src_img,src_x, src_y, cp_width, cp_height,_dst_x,_dst_y);
		//::Copy_img_to_img(&m_mgr->out, src_img,src_x, src_y, cp_width, cp_height,_dst_x,_dst_y);
//	}
}

void element::renderLayer(image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
{
//	src_img->LoadResource();
	int _dst_x=dst_x+GetX();
	int _dst_y=dst_y+GetY();
	//log_i("####in renderLayer####\n");
//	if(hasParent()){
//		if(procArea(&parent->top_image, src_img, src_x, src_y, cp_width, cp_height, _dst_x, _dst_y,x+width,y+height))
//			return;
//	//	log_i("####%s src_x=%d, src_y=%d, cp_width=%d, cp_height=%d, _dst_x=%d, _dst_y=%d####\n",name.c_str(),src_x, src_y, cp_width, cp_height, _dst_x, _dst_y);
//		render_img_to_img(&parent->top_image, src_img, src_x, src_y, cp_width, cp_height, _dst_x, _dst_y);
//
//	}else{
		if(procArea(&m_proc->out, src_img, src_x, src_y, cp_width, cp_height, _dst_x, _dst_y,GetX()+GetWidth(),GetY()+GetHeight()))
			return;
	//	log_i("####%s src_x=%d, src_y=%d, cp_width=%d, cp_height=%d, _dst_x=%d, _dst_y=%d####\n",name.c_str(),src_x, src_y, cp_width, cp_height, _dst_x, _dst_y);
		::Render_img_to_img(&m_proc->out, src_img, src_x, src_y, cp_width, cp_height, _dst_x, _dst_y);
//	}
}
int element::procArea(int &s_ofx,int &s_ofy,int &d_ofx,int &d_ofy, element *ele){
if (ele->GetX() < GetX())
	{
		s_ofx =GetX()- ele->GetX();
		if(s_ofx>ele->GetWidth())
			return 1;
		//d_ofx = render_offset_x;
	}
	else if (ele->GetX() > GetX())
	{
		s_ofx = 0;
		d_ofx = ele->GetX() - GetX();
		if(d_ofx>GetImageWidth())
			return 1;
	}

	 s_ofy = 0; //源x
	 d_ofy = 0; //目标x
	if (ele->GetY() < GetY())
	{
		s_ofy = GetY() - ele->GetY();
		if(s_ofy>ele->GetHeight())
			return 1;
		//d_ofy = 0;
	}
	else if (ele->GetY() > GetY())
	{
		s_ofy = 0;
		d_ofy = ele->GetY() -GetY() ;
		if(d_ofy>GetImageHeight())
			return 1;

	}
	return 0;
}
int  element::procArea(image * dst_img, image * rsc_img, int & src_x, int & src_y, int & cp_width, int & cp_height, int & dst_x, int & dst_y,int dst_end_x,int dst_end_y)
{
	if (cp_width <= 0 || cp_height <= 0)
	{
		return 1;
	}

	if (src_x < 0)
	{
		dst_x -= src_x;
		src_x = 0;
	}
	if (src_y < 0)
	{
		dst_y -= src_y;
		src_y = 0;
	}
	if (dst_x < GetX())
	{
		src_x +=GetX()- dst_x;
		dst_x = GetX();
	}
	if (dst_y <GetX())
	{
		src_y += GetY()-dst_y;
		dst_y = GetY();
	}

	if (src_y + cp_height > rsc_img->GetImageHeight())
	{
		//printf("areaCopy src_y=%d cp_height=%d rsc_img->get_height()=%d\r\n", src_y, cp_height, rsc_img->get_height());
		cp_height = rsc_img->GetImageHeight() - src_y;
		if (cp_height <= 0)
		{
			return 1;
		}
	}
	if (src_x + cp_width > rsc_img->GetImageWidth())
	{
		//printf("areaCopy src_x=%d cp_width=%d rsc_img->get_width()=%d\r\n", src_x, cp_width, rsc_img->get_width());
		cp_width = rsc_img->GetImageWidth() - src_x;
		if (cp_width <= 0)
		{
			return 1;
		}
	}
	if(dst_end_x>dst_img->GetImageWidth()){
		dst_end_x=dst_img->GetImageWidth();
	}
	if(dst_end_y>dst_img->GetImageHeight()){
		dst_end_y=dst_img->GetImageHeight();
	}
	if (dst_y + cp_height > dst_end_y)
	{
		cp_height = dst_end_y - dst_y;
		if (cp_height <= 0)
		{
			return 1;
		}
	}
	if (dst_x + cp_width > dst_end_x)
	{
		cp_width = dst_end_x - dst_x;
		if (cp_width <= 0)
		{
			return 1;
		}
	}
	return 0;
}



void element::cleanArea()
{
	//log_i("--%s cleanArea render_offset_x=%d,render_offset_y=%d,GetImageWidth()=%d,GetImageHeight()=%d---\n"
	//		,name.c_str(),render_offset_x,render_offset_y,GetImageWidth(),GetImageHeight());
//	if (hasParent()){
//		unsigned long *src_offset=(unsigned long *)parent->top_image.pSrcBuffer+(y+render_offset_y)*parent->top_image.u32Width+(x+render_offset_x);
//		int cp_size=render_width<<2;
//		parent->top_image.lock();
//		for(int i=0;i<render_height;i++){
//
////			if(src_offset+cp_size>parent->top_image.SrcSize){
////				break;
////			}
//			memset((void*)src_offset, 0,cp_size);
//			src_offset+=parent->top_image.u32Width;
//		}
//		parent->top_image.unlock();
//	}else{
		unsigned long  *src_offset=(unsigned long *)m_proc->out.GetAddr()+GetY()*m_proc->out.GetImageWidth()+GetX();
		int cp_size=GetImageWidth()<<2;
		m_proc->out.lock();
		for(int i=0;i<GetImageHeight();i++){
			memset((void*)src_offset, 0, cp_size);
			src_offset+=m_proc->out.GetImageWidth();
		}
		m_proc->out.unlock();
	//}
}


void element::FlushConfig(HUMap &mp)
{
	lock();
	PraseElement(mp);
	doFlushConfig(mp);
	ParseModifRes(mp);
	Flush();

	m_mgr->AddElement(GetName(), this);
	unlock();
}



//image * RollBack::dst = NULL;
//image * RollBack::src = NULL;
//map<hustr, SmartPtr<element> > layer::elem; //ʹ������ָ��
