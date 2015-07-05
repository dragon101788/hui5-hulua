#ifndef __HUI_RESOURCE_H__
#define __HUI_RESOURCE_H__

#include "codec.h"
#include "include/autoconf.h"
#include "ttf_font.h"

#if defined(CONFIG_RESOURCE_RUNTIME_DECODER)
#include "Resource_runtime_decoder.h"
typedef Resource_Runtime_Decoder Resource;

#elif defined(CONFIG_RESOURCE_RUNTIME_INDEX)
#include "Resource_runtime_index.h"
typedef Resource_Runtime_Index Resource;

#elif defined(CONFIG_RESOURCE_RUNTIME_FETCH)
#include "Resource_runtime_fetch.h"
typedef Resource_Runtime_Fetch Resource;

#else
#error "!!!dragon!!! Undefined resources management style"
#error "!!!dragon!!! --> plase menuconfig select [readonly resource manager]"
#endif //CONFIG_RESOURCE_RUNTIME

class ResourceContainer
{
public:
	humap<Resource> res;
	void SetRes(const char * name, int id, const char * path)
	{
		if (res[name][id]->path != path || res[name][id]->isNULL())
		{
			//printf("SetRes %d %s\r\n",id,path);
			res[name][id]->SetResource(path);
		}
	}
	void SetRes(const char * name, const char * path)
	{
		if (res[name]->path != path || res[name]->isNULL())
		{
			//printf("SetRes %d %s\r\n",id,path);
			res[name]->SetResource(path);
		}
	}
	image * GetRes(const char * name)
	{
		res[name].value().LoadResource();
		return &res[name].value();
	}
	image * GetRes(const char * name, int id)
	{
		res[name][id].value().LoadResource();
		return &res[name][id].value();
	}
	void ParseModifRes(HUMap &m_mp)
	{
		for (int i = 0; i < m_mp.count("res"); ++i)
		{
			HUMap  xmlmp = m_mp["res"][i];
			hustr meth = xmlmp["meth"]->getvalue();
			const char * name = xmlmp["name"]->getvalue();
//			if(strlen(name)==0)
//			  errexitf("ModifRes %s unknown name\n");
			int id = xmlmp["id"]->getvalue_int();
			int src_x = xmlmp["src_x"]->getvalue_int();
			int src_y = xmlmp["src_y"]->getvalue_int();
			int cp_width = xmlmp["cp_width"]->getvalue_int();
			int cp_height = xmlmp["cp_height"]->getvalue_int();
			int dst_x = xmlmp["dst_x"]->getvalue_int();
			int dst_y = xmlmp["dst_y"]->getvalue_int();

			res[name][id]->LoadResource();
			if (cp_width + cp_height == 0)
			{
				cp_width = res[name][id]->GetImageWidth();
				cp_height = res[name][id]->GetImageHeight();
			}
			if (meth == "render")
			{
				image file;
				file.Load(xmlmp["file"]->getvalue());

				log_d(
						"$$$HU$$$ XML Draw Render %s to %s res=%d %d %d %d %d %d %d\r\n",
						file.path.c_str(), GetName(), id, src_x, src_y, cp_width,
						cp_height, dst_x, dst_y);

				if (!res[name][id]->isNULL())
				{

					//ele->image::Render(&file, src_x, src_y, cp_width, cp_height, dst_x, dst_y);

					res[name][id]->RenderFrom(&file, src_x, src_y, cp_width, cp_height, dst_x,
							dst_y);
					//ele->Render();

				}
				else
				{
					errexitf("%s res %d no init\r\n", name, id);
				}
			}
			else if (meth == "text")
			{
#ifdef CONFIG_USING_FONT
					int red = xmlmp["red"]->getvalue_int();
					int green = xmlmp["green"]->getvalue_int();
					int blue = xmlmp["blue"]->getvalue_int();
					int color = (red & 0xff) << 16 | (green & 0xff) << 8 | blue & 0xff;
					//bkcolor = m_mp["bkcolor"]->getvalue_int();
					hustr font = xmlmp["font"]->getvalue();
					hustr txt = xmlmp["txt"]->getvalue();
					int style = (unsigned char) m_mp["style"]->getvalue_int();
					int size = xmlmp["size"]->getvalue_int();
					/**
					 * padding_left:左边留白距离
					 * padding_top:顶部留白距离
					 * align_center：是否中心对齐
					 */
					int align_center=0;
					int padding_left=xmlmp["padding_left"]->getvalue_int();
					int padding_top=xmlmp["padding_top"]->getvalue_int();
					if (xmlmp.exist("align_center"))
					align_center=xmlmp["align_center"]->getvalue_int();

					text tmpttf;
					tmpttf.m_font = &font_mp[font];
					//log_d("get font_mp %s %x %x\r\n",);
					tmpttf.fontHeight = size;
					tmpttf.color = color;
					tmpttf.style = style;


				if (!res[name][id]->isNULL())
				{

					image & img = res[name][id].value();
					tmpttf.SetBuffer(cp_width, cp_height);
					if (align_center)
					{
						padding_left += (img.GetImageWidth() - dst_x) / 2
								- (txt.length()) * size / 4;
						padding_left > 0 ? padding_left : 0;
						padding_top += (img.GetImageHeight() - dst_y - size) / 2;
						padding_top > 0 ? padding_top : 0;
					}

					tmpttf.drawText((char *) txt.c_str(), txt.length(),
							padding_left, padding_top);

					//tmpttf.drawText((char *) txt.c_str(), txt.length());

					log_d("ParseModifRes text=%s [%s] <%x %x>\r\n",txt.c_str(),font.nstr() ,tmpttf.m_font->face,tmpttf.m_font->ft_Lib);

					res[name][id]->RenderFrom(&tmpttf, src_x, src_y, cp_width,
							cp_height, dst_x, dst_y);
					//ele->Render();

				}
#endif
			}

		}
		m_mp.erase("res");
	}
};


#endif //__HUI_RESOURCE_H__
