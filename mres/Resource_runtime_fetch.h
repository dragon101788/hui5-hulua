#ifndef __HUI_RESOURCE_RUNTIME_FETCH_H__
#define __HUI_RESOURCE_RUNTIME_FETCH_H__



class Resource_Runtime_Fetch :public image
{
public:
        hustr path;
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
                        debug("warning LoadResource empty path\r\n");
                        return -1;
                }
                return codec_to_Image(this, path.nstr());//装载图片
        }
        void ReSetResource(const char * filepath)
        {
                if (path != filepath)
                {
                        path = filepath;
                        codec_to_Image(this, path.c_str());
                }
        }
        //释放空间并不改变其他内容
        virtual void Free()
        {
                if (pSrcBuffer != NULL)
                {
                        debug("destory image pSrcBuffer [%s] %dx%d\r\n", path.c_str(), u32Width, u32Height);
                        free(pSrcBuffer);
                        pSrcBuffer = NULL;
                }
        }
};



#endif //__HUI_RESOURCE_RUNTIME_FETCH_H__
