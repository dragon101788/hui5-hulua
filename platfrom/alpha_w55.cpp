#include "../codec.h"
#include "alpha_w55.h"

BLT g_blt;

void Render_img_to_img(image * dst, image * src, int src_x, int src_y, int cp_width, int cp_height, int dst_x, int dst_y)
{
	if (dst->SrcGPUAddr() == 0 || src->SrcGPUAddr() == 0)
	{
		errexitf("warning::Image source point is NULL dst=%#x src=%#x\r\n", dst->SrcGPUAddr(), src->SrcGPUAddr());
	}

	dst->lock();
	src->lock();

//	ProcArea(dst, src, src_x, src_y, cp_width, cp_height, dst_x, dst_y);

	S_DRVBLT_BLIT_OP s_sblitop =
	{ 0 };

	s_sblitop.dest.u32FrameBufAddr = dst->SrcGPUAddr();
	s_sblitop.dest.i32XOffset = 0;
	s_sblitop.dest.i32YOffset = 0;
	s_sblitop.dest.i32Stride = dst->u32Stride;
	s_sblitop.dest.i16Width = dst->u32Width;
	s_sblitop.dest.i16Height = dst->u32Height;

	s_sblitop.src.pSARGB8 = (S_DRVBLT_ARGB8 *) src->pSrcBuffer;
	s_sblitop.src.u32SrcImageAddr = src->SrcGPUAddr() + (src_x * 4) + (src_y * src->u32Width * 4);
	s_sblitop.src.i32Stride = src->u32Stride;
	s_sblitop.src.i32XOffset = ((0 - dst_x) << 16);
	s_sblitop.src.i32YOffset = ((0 - dst_y) << 16);
	s_sblitop.src.i16Width = cp_width;
	s_sblitop.src.i16Height = cp_height;

	S_DRVBLT_BLIT_TRANSFORMATION stransformation;
	stransformation.colorMultiplier.i16Blue = 255;
	stransformation.colorMultiplier.i16Green = 255;
	stransformation.colorMultiplier.i16Red = 255;
	//stransformation.colorMultiplier.i16Alpha = 255;
	stransformation.colorMultiplier.i16Alpha = 255*src->transp/100;

	stransformation.colorOffset.i16Blue = 0;
	stransformation.colorOffset.i16Green = 0;
	stransformation.colorOffset.i16Red = 0;
	stransformation.colorOffset.i16Alpha = 0;
	stransformation.matrix.a = 0x00010000;
	stransformation.matrix.b = 0x00000000;
	stransformation.matrix.c = 0x00000000;
	stransformation.matrix.d = 0x00010000;
	stransformation.srcFormat = eDRVBLT_SRC_ARGB8888;
	stransformation.destFormat = eDRVBLT_DEST_ARGB8888;
	//stransformation.flags = eDRVBLT_HASTRANSPARENCY;
	stransformation.flags = 0;
	stransformation.flags |= eDRVBLT_HASTRANSPARENCY;
	stransformation.flags |= eDRVBLT_HASCOLORTRANSFORM;
	stransformation.flags |= eDRVBLT_HASALPHAONLY;
	//stransformation.fillStyle = (E_DRVBLT_FILL_STYLE) (eDRVBLT_NOTSMOOTH | eDRVBLT_CLIP);
	stransformation.fillStyle = (E_DRVBLT_FILL_STYLE) (eDRVBLT_CLIP | eDRVBLT_NOTSMOOTH | eDRVBLT_REPEAT_TEXTURE);
	stransformation.userData = NULL;

	s_sblitop.transformation = &stransformation;

	if (g_blt.blt_start_blit(&s_sblitop))
	{	// Configure blit operation and then trigger.
		huErrExit("Configure blit operation and then trigger.");
	}

	dst->unlock();
	src->unlock();

}
