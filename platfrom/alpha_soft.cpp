#include "../codec.h"


#define MAX_ALPHA 255
#define MIN_ALPHA 0

void Render_img_to_img(image * dst, image * src, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
{
//	ProcArea(dst, src, src_x, src_y, cp_width, cp_height, dst_x, dst_y);
	if (dst->SrcGPUAddr() == 0 || src->SrcGPUAddr() == 0)
	{
		errexitf("warning::Image source point is NULL dst=%#x src=%#x\r\n", dst->SrcGPUAddr(), src->SrcGPUAddr());
	}

	int x;
	int y;
	for (y = 0; y < cp_height; y++)
	{
		for (x = 0; x < cp_width; x++)
		{
			IMG_PIX * dst_pix = ((IMG_PIX *) dst->pSrcBuffer + (y + dst_y) * dst->u32Width + x + dst_x);
			IMG_PIX * src_pix = ((IMG_PIX *) src->pSrcBuffer + (y + src_y) * src->u32Width + x + src_x);
			int alpha = src_pix->u8Alpha*src->transp/100;
			dst_pix->u8Red =  (dst_pix->u8Red * (MAX_ALPHA - alpha) + src_pix->u8Red * alpha) / MAX_ALPHA;
			dst_pix->u8Green = (dst_pix->u8Green * (MAX_ALPHA - alpha) + src_pix->u8Green * alpha) / MAX_ALPHA;
			dst_pix->u8Blue =  (dst_pix->u8Blue * (MAX_ALPHA - alpha) + src_pix->u8Blue * alpha) / MAX_ALPHA;
			dst_pix->u8Alpha = (dst_pix->u8Alpha>src_pix->u8Alpha?dst_pix->u8Alpha:src_pix->u8Alpha);
			//((S_DRVBLT_ARGB8 *) dst->pSrcBuffer + (dst->y + dst_y) * dst->u32Width + dst->x + dst_x)->u8Alpha = 255;
		}
	}
}
