#ifndef __HUI_RESOURCE_RUNTIME_INDEX_H__
#define __HUI_RESOURCE_RUNTIME_INDEX_H__

#include <sys/mman.h>

#define RESOURCE_NAME "hui.res"
//#define RESOURCE_DEFSIZE (1024*1024*16)   //16M

#define RESOURCE_PATH_MAX_SIZE		256
#define RESOURCE_RESERVED			17
typedef unsigned long long offs_t;

#include "codec.h"
#include "HumapXML.h"

class Resource_Runtime_Index;
class ResourceRuntimeIndexManager
{
	int m_fd;
	char * m_resbuf;
	size_t m_size;
	HUMap m_mp;
	map<hustr,HUMap::iterator> m_index;
public:
	ResourceRuntimeIndexManager()
	{
		m_resbuf = NULL;
		m_fd = -1;


		HumapFromXmlFile("./hu.res.xml",m_mp);

		for(HUMap::iterator it = m_mp.begin();it!=m_mp.end();++it)
		{
				HUMap & mp = it;
				const char * name = mp["name"]->getvalue();
				m_index[name] = it;
				//printf("%s %s\n",it.key().nstr(),it.value().nstr());
		}

		m_fd = open(RESOURCE_NAME, O_RDONLY|O_CREAT,0666);
		if (m_fd == -1)
		{
			errexitf("### Error: cannot init ResourceRuntimeIndexManager, returns %d\n", m_fd);
		}
		lseek(m_fd, 0 ,SEEK_END);
		int size = lseek(m_fd, 0, SEEK_CUR);
		if(size != 0)
		{
			m_resbuf = (char *)mmap(NULL, size, PROT_READ , MAP_SHARED, m_fd, 0);
			if (m_resbuf == MAP_FAILED)
			{
				errexitf("### Error: failed to map ResourceRuntimeIndexManager to memory %d: %s\n",size ,strerror(errno));
			}
			m_size = size;
			printf("ResourceRuntimeIndexManager mmap 0x%p %ld\n",m_resbuf,m_size);
		}
	}
	~ResourceRuntimeIndexManager()
	{
		if (m_resbuf != NULL && m_resbuf != MAP_FAILED)
		{
			munmap(m_resbuf, m_size);
			m_resbuf = NULL;
		}
		if(m_fd > 0)
		{
			close(m_fd);
			m_fd = -1;
		}
		m_resbuf = NULL;
		m_fd = -1;
	}

	int IndexResource(const char * path,Resource_Runtime_Index * img);

};

extern ResourceRuntimeIndexManager resmgr;

class Resource_Runtime_Index :public image
{
private:
		void ReSetResource(const char * filepath)
		{
			if (path != filepath)
			{
				path = filepath;
				codec_to_Image(this, path.c_str());
			}
		}
public:
        hustr path;
        friend class ResourceRuntimeIndexManager;

        virtual void * alloc_memory(size_t size)
		{
        	printf("$$$HU$$$ Resource_Runtime_Index alloc_memory\n");
		}
		virtual void free_memory(void * buf)
		{
			printf("$$$HU$$$ Resource_Runtime_Index free_memory\n");
		}

        void SetResource(const char * filepath)
        {
                //printf("SetResource %s\r\n",filepath);
                if (path != filepath)
                {
                        destroy();
                        path = filepath;
                }
        }
        int LoadResource()
        {
                if (pSrcBuffer != NULL)
                {
                        return 0;
                }
                if (path.empty())
                {
                        errexitf("warning LoadResource empty path\r\n");
                }
                resmgr.IndexResource(path,this);
                return 0;//codec_to_Image(this, path.nstr());//装载图片
        }

        //释放空间并不改变其他内容
        virtual void Free()
        {
                if (pSrcBuffer != NULL)
                {
                        debug("destory image pSrcBuffer [%s] %dx%d\r\n", path.c_str(), u32Width, u32Height);
                        free_memory(pSrcBuffer);
                        pSrcBuffer = NULL;
                }
        }
};



#endif //__HUI_RESOURCE_RUNTIME_INDEX_H__
