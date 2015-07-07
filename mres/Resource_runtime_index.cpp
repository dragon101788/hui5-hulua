#include "Resource_runtime_index.h"
#include "hulib.h"

ResourceRuntimeIndexManager resmgr;


int ResourceRuntimeIndexManager::IndexResource(const char * path,Resource_Runtime_Index * img)
{
	if(m_index.find(path)==m_index.end())
	{////未找到资源 解码并且写入文件 加入map
		return -1;
	}
	img->pSrcBuffer = m_resbuf + m_index[path]["offset"]->getvalue_int();
	img->u32Width = m_index[path]["width"]->getvalue_int();
	img->u32Height = m_index[path]["height"]->getvalue_int();
	img->u32Stride = m_index[path]["stride"]->getvalue_int();
	printf("IndexResource %s 0x%x %d %d %d\n",path,img->pSrcBuffer,img->u32Width,img->u32Height,img->u32Stride);
}
