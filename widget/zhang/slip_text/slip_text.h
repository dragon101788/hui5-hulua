#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "XMLInstal.h"
#include "layer.h"
#include "thread_msg.h"
#include "ttf_font.h"
#include "stdlib.h"
int getFileSize(char * strFileName)   
{  
    FILE * fp = fopen(strFileName, "r");  
    fseek(fp, 0L, SEEK_END);  
    int size = ftell(fp);  
    fclose(fp);  
    return size;  
}  
typedef struct page    //页链表
{
	image img;
	struct page *next;
	struct page *prior;
}page,*page_list;

int init_list(page_list &L,int w,int h)   //初始化单链表
{
	L=new page;//(page_list)malloc(sizeof(page));   //表头附加结点
	if(!L)
	{
	printf("L malloc mem fialed!!!!!!\n");
	 exit(-2);
	}
	L->img.SetBuffer(w,h);
	L->next=NULL;
	L->prior=NULL;
	return 1;                 
}//初始化了一个一个节点的双向链表
page_list list_find_last(page_list L)
{
page_list last=L;
while(last->next!=NULL)
{
last=last->next;
}
return last;
}

page_list list_find_head(page_list L)
{
page_list head=L;
while(head->prior!=NULL)
{
head=head->prior;
}
return head;
}

int list_how_far_last(page_list L)
{
page_list last=L;
int num=0;
while(last->next!=NULL)
{
num++;
last=last->next;
}
return num;
}

int list_how_far_head(page_list L)
{
page_list head=L;
int num=0;
while(head->prior!=NULL)
{
num++;
//printf("list_how_far_head:num=%d!!!!\n",num);
head=head->prior;
}
return num;
}

void destroy_list(page_list &L)
{
page_list i,head,last;
head=list_find_head(L);
last=list_find_last(L);
for(i=head;i!=last->next;i=i->next)
{
//       free(i->img.pSrcBuffer);
       delete i;//free(head);//删除原来的头        
}
}


int list_add_next_delete_prior(page_list &L,int w,int h)  //前面删一个，后面加一个，往下滑动
{
	int i=0,j=0;
	page_list last,head;
	last=L;//当前的最后一个
	while(last->next!=NULL)
	{
		i++;
		last=last->next;
	}
	head=L;//当前的第一个
	while(head->prior!=NULL)
	{
		j++;
		head=head->prior;
	}

	page_list newpage=new page;//(page_list)malloc(sizeof(page));
	newpage->img.SetBuffer(w,h);
	last->next=newpage;
	newpage->prior=last;
	newpage->next=NULL;
//	printf("i=%d;j=%d!!!\n",i,j);
	if(j>=3)
		{
		head->next->prior=NULL;
	//	free(head->img.pSrcBuffer);
	//	printf("delete head!!!!\r\n");
		delete head;//free(head);//删除原来的头 	
		}
	return 0;
}

int list_add_prior_delete_next(page_list &L,int w,int h)
{
	int i=0,j=0;
	page_list last,head;//当前的最后一个
	last=L;//当前的最后一个
	while(last->next!=NULL)
	{
		i++;
		last=last->next;
	}
	head=L;//当前的第一个
	while(head->prior!=NULL)
	{
		j++;
		head=head->prior;
	}

	//printf("list_add_prior_delete_next: i=%d;j=%d!!!\n",i,j);
	page_list newpage=new page;//(page_list)malloc(sizeof(page));
        newpage->img.SetBuffer(w,h);
	head->prior=newpage;
	newpage->next=head;
	newpage->prior=NULL;
	if(i>=4)   
		{  
		last->prior->next=NULL;
	//	free(last->img.pSrcBuffer); 
	//	printf("delete last!!!\n");     
		delete last;//free(last);//删除原来的尾     
		}
	return 0;
}


class slip_text: public touch_element, public element, public timer_element
{
public:
	slip_text()
	{
		sx = 0;
		sy = 0;
        	 font=NULL;     //保存路径
        	txt=NULL;      //i
        	style=0;
        	size=0;
        	lenth=0;
        	lenth_once=0;//一页缓存最多缓存字数
		lenth_final=0;
        	page=1;
        	const_page=1;
		cache_up =0;
		cache_down =0;
		flag_set_timer=0;
	}
	~slip_text()
	{
		fclose(txt_file);
		free(buffer);
		free(WStr);
		destroy_list(mylist);
	//	delete mylist;
		
	}
	void doTouchDown()
	{
		Flush();
	}

	void doTouchUp()
	{
		sx -= move_x();
		sy -= move_y();
	
		if (sy<0)
		{
			if(page==1)
			sy = 0;
		}
		       
		else  if(sy>0)
		 {
			if(page==const_page)
			sy=0;
			else if(sy>=height)//向下换页
			{
				if(page==const_page-1)
					sy=height;
			}		
	
		}
		if(flag_set_timer)//在抬手处处理定时任务目的在于解决滑动卡顿
		{
			TimerSet(0);
			flag_set_timer=0;
		}
	//	printf("sy=%d!!\n",sy);
		Flush();
	}

	void doDelete()
	{
		xml_mgr->DelTouchElement(this);
	}
	int doTimer(int tm)
	{	
	//	if(const_page!=1)	
	//	{
	//	printf("doTimer cache_up=%d cache_down=%d!!!!!!!!!!!!!!!!!!!!\n",cache_up,cache_down);		
		if(cache_down)//当sy大于height时，准备缓存下一页
		{     
			if(page<const_page-1) 
			{
				ttf.cleanBuf(); 
				if(print_num<txt_len)
				{
                		print_num+=ttf.DrawText_unicode((wchar_t *) WStr+word_num_mp[page+2], lenth_once);
				word_num_mp[page+3]=print_num;
				if(print_num>=txt_len)
				const_page=page+2;
			     	list_add_next_delete_prior(mylist,width,height);
				page_list last=list_find_last(mylist);
                       		// memcpy(last->img.pSrcBuffer,ttf.pSrcBuffer,SrcSize);
				 last->img.AreaCopy(&ttf,0,0);
				
				cache_down=0;
				}
			}
			else
				cache_down=0;
		}

		else if(cache_up)//当sy小于height时，准备缓存上一页
		{
			if(page!=1)
			{			
				ttf.cleanBuf();	
                		ttf.DrawText_unicode((wchar_t *) WStr+word_num_mp[page-1], lenth_once);
				print_num=word_num_mp[page+4];
                                list_add_prior_delete_next(mylist,width,height);
                                page_list head=list_find_head(mylist);
                	        //memcpy(head->img.pSrcBuffer,ttf.pSrcBuffer,SrcSize);
				head->img.AreaCopy(&ttf,0,0);
				cache_up=0;
			}
			else
			cache_up=0;
		}

		TimerStop();
	}
	void doTouchActive()
	{
		
	

	}
	void doRender()
	{
		int py = sy - move_y();
		printf("sy=%d,py=%d!!\n",sy,py);
		if (py<0)
		{
			if(page==1)
			py = 0;
			else  //向上换页 
			{
				py+=height;
				sy+=height;
				mylist=mylist->prior;
			//	{
				page--;
				printf("page--,page=%d\n!!!!!!! ",page);
				if(list_how_far_head(mylist)==0)//下面只剩一个缓存页面
				{
					while(cache_up) printf("wait for cache_up clean!!!!!!1\n");//等待上一次的缓存完成
					cache_up=1;
					//printf("start cache up\n!!!!!\n");
					flag_set_timer=1;
				//	TimerSet(0);
				}
			        	
			}
		}
		       
		else  if(py>0)
		 {
			if(page==const_page)
			py=0;
			else if(py>=height)//向下换页
			{
			//	page++;
				if(page==const_page-1)
				py=height;
				else
				{
					page++;
					py-=height;
					sy-=height;
					mylist=mylist->next;
					printf("page++,page=%d\n!!!!!!! ",page);
					if(list_how_far_last(mylist)==1)//下面只剩一个缓存页面
					{
						while(cache_down) printf("wait for cache_down clean!!!!!!!\n");
						cache_down=1;
						//printf("start cache_down!!!!!!!!\n");
						//TimerSet(0);
						flag_set_timer=1;
					}
				}
					
			}		
	
		}
		printf("page=%d,const_page=%d,print_num=%d,txt_len=%d,sy=%d,py=%d!!\n",page,const_page,print_num,txt_len,sy,py);
			
			image::Render(&mylist->img, sx - move_x(), py, width, height-py, 0, 0);//page start from 0
			if(const_page>1)
			image::Render(&mylist->next->img, sx - move_x(), 0, width,py, 0, height-py);
	
	}

	void doFlushConfig()
	{
		PraseElement();
		//printf("x=%d y=%d w=%d h=%d\r\n", x, y, width, height);
		x_lock = 1;
                int green = m_mp["green"]->getvalue_int();
                int blue = m_mp["blue"]->getvalue_int();
                int red = m_mp["red"]->getvalue_int();
                color = (red & 0xff) << 16 | (green & 0xff) << 8 | blue & 0xff;
                //bkcolor = m_mp["bkcolor"]->getvalue_int();
                font = m_mp["font"]->getvalue();
                txt_name = m_mp["txt"]->getvalue();
		txt_len = getFileSize((char*)txt_name.c_str());
		if((txt_file=fopen(txt_name.c_str(),"r+"))==NULL)	
		{	
		printf("Cannot open output file.\n");
		    }
		printf("txt_len=%d!!!!!!!!!!!!!!!!\n",txt_len);
		buffer=(char *)malloc(txt_len);//记得free()
		//txt=buffer;
		fread((char *)buffer,1,txt_len,txt_file);
		printf("fread end!!!");
                style = (unsigned char) m_mp["style"]->getvalue_int();
                size = m_mp["size"]->getvalue_int();
		int col_num=height/(size*1.2);
		//height=size*1.2*col_num; //对height进行调节
                ttf.m_font = &font_mp[font];
		unsigned int buf_len = (txt_len + 1) * sizeof(wchar_t) * 2; //当utf8显示字母时有最大占用
		WStr = (wchar_t *) malloc(buf_len); //用来存放unicode
        	if (WStr == NULL)
        	{
	                huErrExit("DrawText:: malloc failed!\n");
        	}
        	memset(WStr, 0, buf_len);
	        txt_len = ttf.m_font->convert("wchar_t", "UTF-8", (char *) buffer, txt_len, (char *) WStr, buf_len);
       		txt_len /= 4; //byte to wchar_t
                ttf.fontHeight = size;
                ttf.color = color;
                ttf.style = style;
		lenth_once =width/size*2*(height/(size*1.2));// 粗略，需要调整 english
		if(txt_len<lenth_once)
			lenth_once=txt_len;
		printf("txt_len=%d,  lenth_once=%d!!!!!!!!!!!!!!!\n",txt_len,lenth_once);
		const_page=65000;//txt_len/lenth_once+((txt_len%lenth_once)? 1:0);
		lenth_final=(txt_len%lenth_once)?txt_len%lenth_once:lenth_once;
		printf("const_page=%d!!!!!!!!!!\n",const_page);
		init_list(mylist,width,height);//初始化一个有三个元素的链表 
		printf("list init passed !!!!!\n");
                ttf.SetBuffer(width, height);
//                ttf.DrawText("UTF-8", (char *) buffer, lenth_once);
		word_num_mp[1]=print_num;
                print_num+=ttf.DrawText_unicode((wchar_t *) WStr+print_num, lenth_once);
		word_num_mp[2]=print_num;
		if(print_num>=txt_len)
		const_page=1;
		//memcpy(mylist->img.pSrcBuffer,ttf.pSrcBuffer,ttf.SrcSize);
		mylist->img.AreaCopy(&ttf,0,0);
		if(const_page>1)
		{
			list_add_next_delete_prior(mylist,width,height);
                //	ttf.SetBuffer(width, height);
			ttf.cleanBuf();	
		//	ttf.DrawText("UTF-8", (char *) buffer+lenth_once, lenth_once);
                	print_num+=ttf.DrawText_unicode((wchar_t *) WStr+print_num, lenth_once);
			word_num_mp[3]=print_num;//第3页开始字数
			if(print_num>=txt_len)
				const_page=2;
			printf("11111111113\n");
			//memcpy(mylist->next->img.pSrcBuffer,ttf.pSrcBuffer,ttf.SrcSize);
			mylist->next->img.AreaCopy(&ttf,0,0);
		}
		if(const_page>2)
		{
			list_add_next_delete_prior(mylist,width,height);         	
			ttf.cleanBuf();	
		//	ttf.SetBuffer(width, height);
		//	ttf.DrawText("UTF-8", (char *) buffer+lenth_once*2, lenth_once);

               		 print_num+=ttf.DrawText_unicode((wchar_t *) WStr+print_num, lenth_once);
			word_num_mp[4]=print_num;
			if(print_num>=txt_len)
				const_page=3;
			printf("11111111113\n");
			//memcpy(mylist->next->next->img.pSrcBuffer,ttf.pSrcBuffer,ttf.SrcSize);
			mylist->next->next->img.AreaCopy(&ttf,0,0);
		}

		printf("11111111114\n");



		touch_init_area(x, y, width, height);
		xml_mgr->AddEleArea(this);
		xml_mgr->AddTimerElement( this);
        
		Flush();
	}
	int sx;
	int sy;
	text ttf;
        map<hustr, text>::iterator font_it;
        unsigned int color;
        //unsigned int bkcolor;
        hustr font;     //保存路径
        hustr txt;      //i
        char * buffer;      //i
	wchar_t * WStr;
	hustr txt_name;
	hustr txt_once; //一次缓存的文本
        //wchar_t *mytxt;
        unsigned char style;
        int size;
        //int id;
        int lenth;
	int lenth_once;//一页缓存最多缓存字数
	int lenth_final; //最后一页缓存字数
        int page;
	int const_page;
	int cache_up; 
	int cache_down;
	FILE *txt_file;
	int txt_len;
        page_list mylist;
	unsigned long print_num;
	int flag_set_timer;
	map<int,unsigned long> word_num_mp;	
};

#endif //__STATIC_IMAGE_H__
