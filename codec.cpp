#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <linux/fb.h>
#include <getopt.h>
#include <termios.h>
#include <png.h>

#include "codec.h"
#include "hulib.h"

int
image_write_to_snap(image * img, const char * path);
int
image_read_from_snap(image * img, const char * path);

int access_Image(const char * filename)
{
	if (access(filename, F_OK) == 0 || access(hustr("cache/%s.snap", filename), F_OK) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int codec_to_Image(image * enode, const char * filename)
{
	if (filename == NULL)
	{
		errexitf("codec_to_Image path is unvalid [%s]\r\n", filename);
	}
	enode->lock();

	if (image_read_from_snap(enode, filename) == 0)
	{
		debug("codec_to_Image %s load snap\r\n", filename);
	}
	else
	{
		debug("codec_to_Image %s load file\r\n", filename);

		const char * type = strrchr(filename, '.') + 1;
		if (strcasecmp(type, "png") == 0)
		{
			//debug_timer();
			//printf("$$$HU$$$ JPG\r\n");

			if (pngCodec_to_image(enode, filename))
			{
				errexitf("pngCodec_to_image %s", filename);
			}
			//debug_timer(filename);
		}
		//	else if (strcasecmp(type, "bmp") == 0)
		//	{
		//		//printf("$$$HU$$$ BMP\r\n");
		//		if (bmpCodec_to_image(enode, filename))
		//		{
		//			errexitf("bmpCodec_to_image %s", filename);
		//		}
		//	}
		//	else if(strcasecmp(strrchr(filename,'.')+1,"jpg")==0)
		//	{
		//		//printf("$$$HU$$$ JPG\r\n");
		//		if (jpegCodec_to_image(enode,filename))
		//		{
		//			exit(1);
		//		}
		//	}
		else
		{
			errexitf("unknow file [%s] type %s\r\n", filename, type);
		}
	}

	enode->unlock();

	return 0;
}

/******************************ͼƬ���*********************************/
//typedef struct _pic_data pic_data;
//struct _pic_data
//{
// int width, height; /* �ߴ� */
// int bit_depth;  /* λ�� */
// int flag;   /* һ����־����ʾ�Ƿ���alphaͨ�� */
//
// unsigned char **rgba; /* ͼƬ���� */
//};
/**********************************************************************/
#define PNG_BYTES_TO_CHECK 4
#define HAVE_ALPHA 1
#define NO_ALPHA 0

int pngCodec_to_image(image * sobj, const char *filepath)
{
	FILE *pic_fp;
	pic_fp = fopen(filepath, "rb");
	if (pic_fp == NULL)
	{
		errexitf("open %s %s \r\n", filepath, strerror(errno));
	}

	png_structp png_ptr;
	png_infop info_ptr;
	char buf[PNG_BYTES_TO_CHECK];
	int temp;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info_ptr = png_create_info_struct(png_ptr);

	setjmp(png_jmpbuf(png_ptr));

	temp = fread(buf, 1, PNG_BYTES_TO_CHECK, pic_fp);
	temp = png_sig_cmp((png_byte*) buf, (png_size_t) 0, PNG_BYTES_TO_CHECK);

	if (temp != 0)
		return 1;

	rewind(pic_fp);
	png_init_io(png_ptr, pic_fp);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	int color_type, channels, bit_depth;

	/*��ȡ��ȣ��߶ȣ�λ���ɫ����*/
	channels = png_get_channels(png_ptr, info_ptr); /*��ȡͨ����*/
	bit_depth = png_get_bit_depth(png_ptr, info_ptr); /* ��ȡλ�� */
	color_type = png_get_color_type(png_ptr, info_ptr); /*��ɫ����*/

	//printf("channels=%d bit_depth=%d color_type=%d\r\n",channels,bit_depth,color_type);
	int i, j;
	int pos = 0;
	/* row_pointers��߾���rgba��� */
	png_bytep* row_pointers;
	row_pointers = png_get_rows(png_ptr, info_ptr);
	int w = png_get_image_width(png_ptr, info_ptr);
	int h = png_get_image_height(png_ptr, info_ptr);
	sobj->SetBuffer(w, h);

	if (channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		struct PNG_ARGB
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		}__attribute__ ((packed));
		PNG_ARGB ** pix = (PNG_ARGB **) row_pointers;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{

				IMG_PIX * dst_pix = sobj->GetPix(x, y);
				dst_pix->u8Red = pix[y][x].r;
				dst_pix->u8Green = pix[y][x].g;
				dst_pix->u8Blue = pix[y][x].b;
				dst_pix->u8Alpha = pix[y][x].a;

			}
		}
	}
	else if (channels == 3 || color_type == PNG_COLOR_TYPE_RGB)
	{
		struct PNG_RGB
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
		}__attribute__ ((packed));
		PNG_RGB ** pix = (PNG_RGB **) row_pointers;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				IMG_PIX * dst_pix = sobj->GetPix(x, y);
				dst_pix->u8Red = pix[y][x].r;
				dst_pix->u8Green = pix[y][x].g;
				dst_pix->u8Blue = pix[y][x].b;
				dst_pix->u8Alpha = 255;

			}
		}

	}
	else
		return 1;

	fclose(pic_fp);
	/* �������ռ�õ��ڴ� */
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	return 0;
}

/* 将puc_data结构中的数据写入至png文件 */
int pngEndec_to_image(const char *file_name, image * graph)
{
	printf("write_png_file save to %s\r\n", file_name);
	int j, i, temp, pos;
	png_byte color_type;

	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep * row_pointers;
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
	{
		errexitf("[write_png_file] File %s could not be opened for writing", file_name);
	}

	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
	{
		huErrExit("[write_png_file] png_create_write_struct failed");
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		huErrExit("[write_png_file] png_create_info_struct failed");
	}
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		huErrExit("[write_png_file] Error during init_io");
	}
	png_init_io(png_ptr, fp);

	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		huErrExit("[write_png_file] Error during writing header");
	}
	/* 判断要写入至文件的图片数据是否有透明度，来选择色彩类型 */
	color_type = PNG_COLOR_TYPE_RGB_ALPHA;

	png_set_IHDR(png_ptr, info_ptr, graph->GetImageWidth(), graph->GetImageHeight(), 8, color_type, PNG_INTERLACE_NONE,
	PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		huErrExit("[write_png_file] Error during writing bytes");
	}
	temp = (4 * graph->GetImageWidth());

	pos = 0;
	row_pointers = (png_bytep*) malloc(graph->GetImageHeight() * sizeof(png_bytep));
	for (i = 0; i < graph->GetImageHeight(); i++)
	{
		row_pointers[i] = (png_bytep) malloc(sizeof(unsigned char) * temp);
		for (j = 0; j < temp; j += 4)
		{
			IMG_PIX * tmp = graph->GetPix(pos);
			row_pointers[i][j] = tmp->u8Red; // red
			row_pointers[i][j + 1] = tmp->u8Green; // green
			row_pointers[i][j + 2] = tmp->u8Blue; // blue
			row_pointers[i][j + 3] = tmp->u8Alpha; // alpha

//			row_pointers[i][j] = ((IMG_PIX *) graph->pSrcBuffer + pos)->u8Red; // red
//			row_pointers[i][j + 1] = ((IMG_PIX *) graph->pSrcBuffer + pos)->u8Green; // green
//			row_pointers[i][j + 2] = ((IMG_PIX *) graph->pSrcBuffer + pos)->u8Blue; // blue
//			row_pointers[i][j + 3] = ((IMG_PIX *) graph->pSrcBuffer + pos)->u8Alpha; // alpha
			++pos;
		}
	}
	png_write_image(png_ptr, row_pointers);

	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		huErrExit("[write_png_file] Error during end of write");
	}
	png_write_end(png_ptr, NULL);

	/* cleanup heap allocation */
	for (j = 0; j < graph->GetImageHeight(); j++)
		free(row_pointers[j]);
	free(row_pointers);

	fclose(fp);
	return 0;
}

int image_write_to_snap(image * img, const char * rawpath)
{
	hustr path("cache/%s.snap", rawpath);
	restart: FILE * fp = fopen(path, "w+");
	if (fp == NULL)
	{
		if (errno == ENOENT)
		{
			char str[strlen(path) + 1];
			strcpy(str, path);
			char*p = strrchr(str, '/');
			*p = 0;
			system(hustr("mkdir -p %s", str));
			goto restart;
		}
		else
		{
			errexitf("image_write_to_snap open %s:%d %s\r\n", path.c_str(), errno, strerror(errno));
		}

	}
	fseek(fp, SEEK_SET, 0);
	fwrite(&img->SrcSize, sizeof(unsigned long), 1, fp);
	fwrite(&img->u32Width, sizeof(unsigned int), 1, fp);
	fwrite(&img->u32Height, sizeof(unsigned int), 1, fp);
	fwrite(&img->u32Stride, sizeof(unsigned int), 1, fp);

	unsigned int reserved[17];
	fwrite(&reserved, sizeof(unsigned int), 17, fp);
	fwrite(img->pSrcBuffer, 1, img->SrcSize, fp);

	fclose(fp);
	return 0;
}

int image_read_from_snap(image * img, const char * rawpath)
{
	hustr path("cache/%s.snap", rawpath);

	if (access(path, F_OK) != 0)
	{
		return -1;
	}
	FILE * fp = fopen(path, "r");
	if (fp == NULL)
	{
		errexitf("image_read_from_snap open %s:%d %s\r\n", path.c_str(), errno, strerror(errno));
	}
	fseek(fp, SEEK_SET, 0);
	fread(&img->SrcSize, sizeof(unsigned long), 1, fp);
	fread(&img->u32Width, sizeof(unsigned int), 1, fp);
	fread(&img->u32Height, sizeof(unsigned int), 1, fp);
	fread(&img->u32Stride, sizeof(unsigned int), 1, fp);
	unsigned int reserved[17];
	fread(&reserved, sizeof(unsigned int), 17, fp);
	img->SetBuffer(img->GetImageWidth(), img->GetImageHeight());
	fread(img->pSrcBuffer, 1, img->SrcSize, fp);

	fclose(fp);
	return 0;
}
//int jpegCodec_to_image( image * sobj,const char *filename)
//{
//	//TEST_DECODE_DOWNSCALE
//	unsigned long BufferSize, bufferCount,readSize;
//	jpeg_param_t jpeg_param;
//	jpeg_info_t *jpeginfo;
//	unsigned char *pJpegBuffer = NULL, *pSRCbuffer = NULL, *pJpegSWaddr = NULL;
//	int jpg_codec_fd;
//	__u32 jpeg_buffer_size;
//	int enc_reserved_size;
//	int ret = 0;
//	int i,len, jpeginfo_size;
//	int width,height, parser;
//	FILE *fp;
//
//	for(i=0;i<100;i++)
//	{
//		jpg_codec_fd = open("/dev/video1", O_RDWR);
//		if (jpg_codec_fd < 0)
//		{
//			continue;
//		}
//		break;
//	}
//	if (jpg_codec_fd < 0)
//	{
//		printf("JPEG Engine is busy\n");
//		exit(-1);
//	}
//	/* allocate memory for JPEG engine */
//	ioctl(jpg_codec_fd, JPEG_GET_JPEG_BUFFER, (__u32)&jpeg_buffer_size);
//
//	//printf("\tjpeg engine memory buffer: 0x%X\n",jpeg_buffer_size);
//
//	pJpegBuffer = (unsigned char *)mmap(NULL, jpeg_buffer_size, PROT_READ|PROT_WRITE, MAP_SHARED, jpg_codec_fd, 0);
//
//	if(pJpegBuffer == MAP_FAILED)
//	{
//		printf("JPEG Map Failed!\n");
//		exit(-1);
//	}
//	else
//	{
//	//	printf("\tGet memory from jpeg engine: 0x%X\n",jpeg_buffer_size);
//	}
//
//	memset((void*)&jpeg_param, 0, sizeof(jpeg_param_t));
//	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
//	jpeginfo = (jpeg_info_t *)malloc(jpeginfo_size);
//
//	int DecOPWbuffersize;
//
//	jpeg_param.encode = 0;			/* Decode Operation */
//	jpeg_param.src_bufsize = jpeg_buffer_size/2;	/* Src buffer size (Bitstream buffer size for JPEG engine) */
//	jpeg_param.dst_bufsize = jpeg_buffer_size/2; /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
//	jpeg_param.decInWait_buffer_size = 0;	/* Decode input Wait buffer size (Decode input wait function disable when								   decInWait_buffer_size is 0) */
//	jpeg_param.decopw_en = 0;
//	jpeg_param.windec_en = 0;
//
//
//	//jpeg_param.dec_stride = xres; /* Enable stride function */
//
//
//	/* Total buffer size for JPEG engine */
//
//	BufferSize = (jpeg_param.src_bufsize + jpeg_param.dst_bufsize);
//
//	if(BufferSize > jpeg_buffer_size)
//	{
//		printf("JPEG Engine Buffer isn't enough\n");
//		exit(-1);
//	}
//
//	/* Clear buffer */
//
//	//memset(pJpegBuffer, 0x77, BufferSize);
//
//	/* Open JPEG file */
//	fp = fopen(filename, "r");
//	if(fp == NULL)
//	{
//	    printf("open %s error : %s\n", filename,strerror(errno));
//	    exit(-1);
//	}
//
//	pSRCbuffer = pJpegBuffer;
//	bufferCount = 0;
//	parser = 0;
//	//printf("JPEG Header Parser:\n");
//	/* Read Bitstream to JPEG engine src buffer */
//	while(!feof(fp))
//	{
//		fd_set writefds;
//		struct timeval tv;
//		int result;
//		tv.tv_sec		= 0;
//		tv.tv_usec		= 0;
//		FD_ZERO( &writefds );
//		FD_SET( jpg_codec_fd , &writefds );
//		tv.tv_sec		= 0;
//		tv.tv_usec		= 0;
//
//		select( jpg_codec_fd + 1 , NULL , &writefds , NULL, &tv );
//		if( FD_ISSET( jpg_codec_fd, &writefds ))
//		{
//			readSize = fread(pSRCbuffer, 1, 4096 , fp);
//			pSRCbuffer += readSize;
//			bufferCount += readSize;
//		}
//		if(!parser)
//		{
//			result = ParsingJPEG(pJpegBuffer, bufferCount, &width, &height);
//			if(!result)
//			{
//				//printf("\t->Width %d, Height %d\n", width,height);
//				parser = 1;
//			}
//			else{}
//				//printf("\t->Can't get image siz in %5d byte bistream\n", bufferCount);
//		}
//
//		if( bufferCount > jpeg_param.src_bufsize)
//		{
//			printf("Bitstream size is larger than src buffer, %d!!\n",bufferCount);
//			return 0;
//		}
//	}
//	printf("Bitstream is %d Bytes\n",bufferCount);
//
//	if(bufferCount % 4)
//		bufferCount = (bufferCount & ~0x3) + 4;
//
//
//	printf("Set Src Buffer is %d Bytes\n",bufferCount);
//
//	jpeg_param.src_bufsize = bufferCount;	/* Src buffer size (Bitstream buffer size for JPEG engine) */
//	jpeg_param.dst_bufsize = BufferSize - bufferCount;	/* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
//
//
//
//	jpeg_param.buffersize = 0;		/* only for continuous shot */
//		jpeg_param.buffercount = 1;
//
//	/* Set decode output format: RGB555/RGB565/RGB888/YUV422/PLANAR_YUV */
//	jpeg_param.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_RGB565;
//
//
//
//	/* Set operation property to JPEG engine */
//	if((ioctl(jpg_codec_fd, JPEG_S_PARAM, &jpeg_param)) < 0)
//	{
//		fprintf(stderr,"set jpeg param failed:%d\n",errno);
//		ret = -1;
//		goto out;
//	}
//
//
//	/* Trigger JPEG engine */
//	if((ioctl(jpg_codec_fd, JPEG_TRIGGER, NULL)) < 0)
//	{
//		fprintf(stderr,"trigger jpeg failed:%d\n",errno);
//		ret = -1;
//		goto out;
//	}
//
//	/* Get JPEG decode information */
//	len = read(jpg_codec_fd, jpeginfo, jpeginfo_size);
//
//	if(len<0) {
//		fprintf(stderr, "read data error errno=%d\n", errno);
//		ret = -1;
//		goto out;
//	}
//	//printf("JPEG: Width %d, Height %d\n",jpeginfo->width, jpeginfo->height);
//
//	if(jpeginfo->state == JPEG_DECODED_IMAGE)
//	{
//
//		sobj->SrcSize=jpeginfo->image_size[0];
//
//		//sobj->srcBG_fd = -1;
//		sobj->pSrcBuffer = NULL;
//		sobj->srcFormat = eDRVBLT_SRC_RGB565;
//		int err = posix_memalign(&sobj->pSrcBuffer, 4, sobj->SrcSize);
//		if (err)
//		{
//				errexitf("posix_memalign failed: %s\n", strerror(err));
//		}
//
//
//		memcpy(sobj->pSrcBuffer,(char*)(pJpegBuffer + jpeg_param.src_bufsize), sobj->SrcSize);
//		sobj->u32Stride=jpeginfo->width<<1;
//		sobj->get_width()=jpeginfo->width;
//		sobj->get_height()=jpeginfo->height;
//
//		printf("jpeg codec filename=%s %d %d\r\n",filename,sobj->get_width(),sobj->get_height());
//
//	}
//	else if(jpeginfo->state == JPEG_DECODE_ERROR)
//		printf("Decode Error\n");
//	else if(jpeginfo->state == JPEG_MEM_SHORTAGE)
//		printf("Memory Shortage\n");
//	else if(jpeginfo->state == JPEG_DECODE_PARAM_ERROR)
//		printf("Decode Parameter Error\n");
//
//
//out:
//	if(pJpegBuffer)
//		munmap(pJpegBuffer, jpeg_buffer_size);
//
//	close(jpg_codec_fd);
//	fclose(fp);
//	free(jpeginfo);
//	return 0;
//
//}

void ProcArea(image * dst_img, image * rsc_img, int & src_x, int & src_y, int & cp_width, int & cp_height, int & dst_x, int & dst_y)
{
	if (cp_width == 0 || cp_height == 0)
	{
		return;
	}
	if (cp_width < 0 || cp_height < 0)
	{
		huErrExit("AreaCopy cp_windth or cp_height <0\r\n");
	}
	//printf("$$$HU$$$ src_x=%d\r\n", src_x);

	//***************************************
	//判断是否复制的坐标小于0，并且修正宽度
//	if (dst_x < 0)
//        {
//            cp_width = dst_img->GetImageWidth() + dst_x;
//               // dst_x = 0;
//        }
//        if (dst_y < 0)
//        {
//            cp_height = dst_img->GetImageHeight() + dst_y;
//                //dst_y = 0;
//        }

        //*************************************
	if (src_x < 0)
	{
	        //cp_width += src_x;
		dst_x -= src_x;
		src_x = 0;
	}
	if (src_y < 0)
	{
	        //cp_height += src_y;
		dst_y -= src_y;
		src_y = 0;
	}

	if (cp_height > dst_img->GetImageHeight())
        {
                //printf("AreaCopyDH dst_y=%d cp_height=%d dst_img.height=%d\n",dst_y , cp_height , dst_img->GetImageHeight());
                cp_height = dst_img->GetImageHeight();
        }
        if (cp_width > dst_img->GetImageWidth())
        {
                //printf("AreaCopyDW dst_x=%d cp_width=%d dst_img.width=%d\n",dst_x , cp_width , dst_img->GetImageWidth());
                cp_width = dst_img->GetImageWidth();
        }

	//*************************************
	if (src_y + cp_height > rsc_img->GetImageHeight())
	{
	        //printf("AreaCopySH src_y=%d cp_height=%d rsc_img->get_height()=%d\r\n", src_y, cp_height, rsc_img->GetImageHeight());
		cp_height = rsc_img->GetImageHeight() - src_y;
		if (cp_height <= 0)
		{
			return;
		}
	}
	if (src_x + cp_width > rsc_img->GetImageWidth())
	{
	        //printf("AreaCopySW src_x=%d cp_width=%d rsc_img->get_width()=%d\r\n", src_x, cp_width, rsc_img->GetImageWidth());
		cp_width = rsc_img->GetImageWidth() - src_x;
		if (cp_width <= 0)
		{
			return;
		}
	}

	//*************************************

}

void AreaCopy(image * dst_img, image * src_img, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
{
	int x;
	int y;
	dst_img->lock();
	src_img->lock();
	//printf("$$$HU$$$ AreaCopy1 src_x=%d src_y=%d cp_width=%d cp_height=%d dst_x=%d dst_y=%d\r\n", src_x, src_y, cp_width, cp_height, dst_x, dst_y);
	ProcArea(dst_img, src_img, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
	//printf("$$$HU$$$ AreaCopy2 src_x=%d src_y=%d cp_width=%d cp_height=%d dst_x=%d dst_y=%d row_size=%d\r\n", src_x, src_y, cp_width, cp_height, dst_x, dst_y,cp_width * sizeof(IMG_PIX));
	//printf("$$$HU$$$ AreaCopy3 src[%d:%d] dst[%d:%d]\n",src_img->GetImageWidth(),src_img->GetImageHeight(),dst_img->GetImageWidth(),dst_img->GetImageHeight());
	//printf("$$$HU$$$ AreaCopy4 0x%x 0x%x\n",dst_img->pSrcBuffer,src_img->pSrcBuffer);

	for (y=0; y < cp_height; y++)
	{
//	        printf("y=%d\n",y);
//                for(x=0;x<cp_width;x++)
//                {
//                    printf("x=%d\n",x);
//                    IMG_PIX * dst_pix = ((IMG_PIX *) dst_img->pSrcBuffer + (y + dst_y) * dst_img->u32Width + (x + dst_x));
//                    IMG_PIX * src_pix = ((IMG_PIX *) src_img->pSrcBuffer + (y + src_y) * src_img->u32Width + (x + src_x));
//                    if(src_pix->u8Alpha)
//                      *dst_pix = *src_pix;
//                }

		memcpy((unsigned int *) dst_img->pSrcBuffer + (y + dst_y) * dst_img->u32Width + dst_x,
				(unsigned int *) src_img->pSrcBuffer + (y + src_y) * src_img->u32Width + src_x,
				(cp_width * sizeof(IMG_PIX)));
	}

	//printf("OK\n");
	dst_img->unlock();
	src_img->unlock();
}

//void Render_img_to_img(image * dst,image * img,int x,int y)
//{
////	if(g_blt==NULL)
////	{
////		return;
////	}
//
//	img->LoadResource();
//
//	if(dst->SrcGPUAddr()== 0 ||img->SrcGPUAddr() == 0)
//	{
//		errexitf("warning::Image source point is NULL dst=%#x src=%#x\r\n",dst->SrcGPUAddr(),img->SrcGPUAddr());
//	}
//
//	S_DRVBLT_BLIT_OP				s_sblitop={0};
//
//	s_sblitop.dest.u32FrameBufAddr = dst->SrcGPUAddr();
//	s_sblitop.dest.i32XOffset = 0;
//	s_sblitop.dest.i32YOffset = 0;
//	s_sblitop.dest.i32Stride = dst->u32Stride;
//	s_sblitop.dest.i16Width = dst->get_width();
//	s_sblitop.dest.i16Height = dst->get_height();
//
//	s_sblitop.src.pSARGB8 = (IMG_PIX *)img->pSrcBuffer;
//	s_sblitop.src.u32SrcImageAddr = img->SrcGPUAddr();
//	s_sblitop.src.i32Stride = img->u32Stride;
//	s_sblitop.src.i32XOffset = ((0-x) << 16);
//	s_sblitop.src.i32YOffset = ((0-y) << 16);
//	s_sblitop.src.i16Width = img->get_width();
//	s_sblitop.src.i16Height = img->get_height();
//
//	img->stransformation.destFormat = dst->destFormat;
//	img->stransformation.srcFormat = img->srcFormat;
//
//	s_sblitop.transformation = &img->stransformation;
//
//	if (g_blt.blt_start_blit(&s_sblitop))
//	{	// Configure blit operation and then trigger.
//		errexit("Configure blit operation and then trigger.");
//	}
//
//	if (g_blt.blt_flush())
//	{	// Wailt for blit done.
//		errexit("Wailt for blit done.");
//	}
//}

//void Render_img_to_buf(void * buf, image * img, int width, int height)
//{
//	image dst;
//	dst.u32Width = width;
//	dst.u32Height = height;
//	dst.pSrcBuffer = buf;
//	dst.u32Stride = width * 2;
//
//	Render_img_to_img(&dst, img, 0, 0, img->get_width(), img->get_height(), 0, 0);
//}

//int fill_image(image * img, int a, int r, int g, int b)
//{
//
//	S_DRVBLT_FILL_OP s_sfillop;
//	s_sfillop.color.u8Blue = b;
//	s_sfillop.color.u8Green = g;
//	s_sfillop.color.u8Red = r;
//	s_sfillop.color.u8Alpha = a;
//	s_sfillop.blend = true;
//	s_sfillop.u32FrameBufAddr = img->SrcGPUAddr();
//	s_sfillop.rowBytes = img->u32Stride;
//	s_sfillop.format = img->destFormat;
//	s_sfillop.rect.i16Xmin = 0;
//	s_sfillop.rect.i16Xmax = img->get_width();
//	s_sfillop.rect.i16Ymin = 0;
//	s_sfillop.rect.i16Ymax = img->get_height();
//
//	if ((ioctl(g_blt.blt_fd, BLT_IOCSFILL, &s_sfillop)) == -1)
//	{
//		errexitf("set FILL parameter failed: %s\n", strerror(errno));
//	}
//
//	if ((ioctl(g_blt.blt_fd, BLT_IOCTRIGGER, NULL)) == -1)
//	{
//		errexitf("trigger BLT failed: %s\n", strerror(errno));
//	}
//	return 0;
//}
//
