/* ***************************************************************************
 * LCUI_Memory.c -- Some of the data type of memory management
 * 
 * Copyright (C) 2012 by
 * Liu Chao
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
 * LCUI_Memory.c -- һЩ�������͵��ڴ����
 *
 * ��Ȩ���� (C) 2012 ������ 
 * ����
 * 
 * ����ļ���LCUI��Ŀ��һ���֣�����ֻ���Ը���GPLv2���Э����ʹ�á����ĺͷ�����
 *
 * (GPLv2 �� GNUͨ�ù������֤�ڶ��� ��Ӣ����д)
 * 
 * ����ʹ�á��޸Ļ򷢲����ļ����������Ѿ��Ķ�����ȫ���ͽ���������Э�顣
 * 
 * LCUI ��Ŀ�ǻ���ʹ��Ŀ�Ķ�����ɢ���ģ��������κε������Σ�����û�������Ի���
 * ����;���������������������GPLv2���Э�顣
 *
 * ��Ӧ���յ������ڱ��ļ���GPLv2���Э��ĸ�������ͨ����LICENSE.TXT�ļ��У����
 * û�У���鿴��<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPHICS_H
#include LC_FONT_H
#include LC_MISC_H
#include LC_WIDGET_H 

#include <unistd.h>

void Using_Graph(LCUI_Graph *pic, int mode)
/* ���ܣ���ָ��ģʽʹ��ͼ������
 * ˵��������mode��ֵΪ0ʱ���ԡ�����ģʽʹ�����ݣ�����ֵʱ���ԡ�дģʽʹ�����ݡ� */
{
	if(mode == 0) thread_rwlock_rdlock(&pic->lock);
	else thread_rwlock_wrlock(&pic->lock);
}

void End_Use_Graph(LCUI_Graph *pic)
/* ���ܣ�����ͼ�����ݵ�ʹ�� */
{
	thread_rwlock_unlock(&pic->lock);
}

unsigned char** Get_Malloc(int width,int height,int flag)
/* ���ܣ�Ϊͼ�����������ڴ�ռ䣬����ʼ�����ڴ�ռ�Ϊ�� */
{
	unsigned int size;
	unsigned char** out_buff;
	size = sizeof(unsigned char)*width*height;
	if(flag == HAVE_ALPHA)
		out_buff = (unsigned char**)malloc(sizeof(unsigned char*)*4);
	else
		out_buff = (unsigned char**)malloc(sizeof(unsigned char*)*3);
		out_buff[0] = (unsigned char*)malloc(size);
		out_buff[1] = (unsigned char*)malloc(size);
		out_buff[2] = (unsigned char*)malloc(size);
		if(flag == HAVE_ALPHA)
			out_buff[3] = (unsigned char*)calloc(1, size);
			
		if(!out_buff || !out_buff[0] || !out_buff[1] 
		|| !out_buff[2] || (flag == HAVE_ALPHA && !out_buff[3]))
			return NULL; 
	return out_buff;
}
 
void Free_Graph(LCUI_Graph *pic)
/* ���ܣ��ͷ�LCUI_Graph�ڵ�ͼ������ռ�õ��ڴ���Դ */
{ 
	if(Valid_Graph(pic))
	{
		Using_Graph(pic, 1);
		free(pic->rgba[0]);
		free(pic->rgba[1]);
		free(pic->rgba[2]);
		if(pic->flag == HAVE_ALPHA) 
			free(pic->rgba[3]);
		free(pic->rgba);
		pic->rgba = NULL;
		pic->malloc = IS_FALSE;
		pic->width = 0;
		pic->height = 0;
		End_Use_Graph(pic); 
	}
}

void *Malloc_Widget_Private(LCUI_Widget *widget, size_t size)
/* ���ܣ�Ϊ����˽�нṹ��ָ������ڴ� */
{
	widget->private = malloc(size);
	return widget->private;
}


int Malloc_Graph(LCUI_Graph *pic, int width, int height)
/* ���ܣ�Ϊͼ�����ݷ����ڴ���Դ */
{
	if(Valid_Graph(pic)) 
		Free_Graph(pic); 
	
	if(width > 10000 || height > 10000)
	{
		printf("error: can not allocate too much memory!\n");
		exit(-1);
	}
	
	if(height < 0 || width < 0) 
	{
		Free_Graph(pic);
		return -1; 
	}
	 
	Using_Graph(pic, 1);
	pic->rgba  = Get_Malloc(width, height, pic->flag); 
	if(NULL == pic->rgba)
	{
		pic->width  = 0;
		pic->height = 0;
		End_Use_Graph(pic);
		pic->malloc = IS_FALSE;
		return -1;
	}
	
	pic->width  = width;
	pic->height = height;
	pic->malloc = IS_TRUE;
	End_Use_Graph(pic); 
	return 0;
}

void Copy_Graph(LCUI_Graph *des, LCUI_Graph *src)
/* 
 * ���ܣ�����ͼ������
 * ˵������src�����ݿ�����des 
 * */
{
	int size;
	des->flag = src->flag;       /* �Ƿ���Ҫ͸���� */
	if(Valid_Graph(src))
	{
		if(Valid_Graph(des)) Free_Graph(des);
		if(src->flag == HAVE_ALPHA) des->flag = HAVE_ALPHA;
		else des->flag = NO_ALPHA;
		Malloc_Graph(des, src->width, src->height);/* ���·��� */
		/* ����ͼ������ */
		Using_Graph(des, 1);
		Using_Graph(src, 0);
		size = sizeof(unsigned char)*src->width*src->height;
		memcpy(des->rgba[0], src->rgba[0], size);
		memcpy(des->rgba[1], src->rgba[1], size);
		memcpy(des->rgba[2], src->rgba[2], size);
		if(src->flag == HAVE_ALPHA)
		memcpy(des->rgba[3], src->rgba[3], size);
		des->type = src->type;       /* �洢ͼƬ���� */
		des->bit_depth = src->bit_depth;  /* λ�� */
		des->alpha = src->alpha;      /* ͸���� */
		End_Use_Graph(des);
		End_Use_Graph(src);
	}
}

void Free_String(LCUI_String *in)
/* ���ܣ��ͷ�String�ṹ���е�ָ��ռ�õ��ڴ�ռ� */
{
	if(in->size > 0) 
		free(in->string); 
	in->string = NULL;
}

void Free_Bitmap(LCUI_Bitmap *bitmap)
/* ���ܣ��ͷŵ�ɫλͼռ�õ��ڴ���Դ������ʼ�� */
{
	int y;
	if(Valid_Bitmap(bitmap)) 
	{
		for(y = 0; y < bitmap->height; ++y) 
			free(bitmap->data[y]); 
		free(bitmap->data);
		bitmap->alpha    = 255;/* ����λͼĬ�ϲ�͸�� */
		bitmap->width    = 0;
		bitmap->height   = 0;
		bitmap->malloc   = IS_FALSE;/* ����λͼû�з����ڴ� */
		bitmap->data     = NULL;
	}
}

void Free_WChar_T(LCUI_WChar_T *ch)
/* ���ܣ��ͷ�LCUI_Wchar_T�����е�ָ�����ռ�õ��ڴ� */
{
	Free_Bitmap(&ch->bitmap);
}

void Free_WString(LCUI_WString *str)
/* ���ܣ��ͷ�LCUI_WString�ͽṹ���е�ָ�����ռ�õ��ڴ� */
{
	int i;
	if(str != NULL)
	{
		if(str->size > 0 && str->string != NULL)
		{
			for(i = 0; i < str->size; ++i) 
				Free_WChar_T(&str->string[i]); 
			free(str->string);
			str->string = NULL;
		}
	}
}



void Malloc_Bitmap(LCUI_Bitmap *bitmap, int width, int height)
/* ���ܣ�ΪBitmap�ڵ����ݷ����ڴ���Դ������ʼ�� */
{
	int i;
	if(width * height > 0)
	{
		if(Valid_Bitmap(bitmap)) Free_Bitmap(bitmap);
		bitmap->width = width;
		bitmap->height = height;
		bitmap->data = (unsigned char**)malloc(
				bitmap->height*sizeof(unsigned char*));
		for(i = 0; i < bitmap->height; ++i)
		{   /* Ϊ����ͼ��ÿһ�з����ڴ� */
			bitmap->data[i] = (unsigned char*)calloc(1,
							sizeof(unsigned char) * bitmap->width); 
		}
		bitmap->alpha    = 255; /* ����Ĭ�ϲ�͸�� */
		bitmap->malloc = IS_TRUE;
	}
}

void Realloc_Bitmap(LCUI_Bitmap *bitmap, int width, int height)
/* ���ܣ�����λͼ�ĳߴ� */
{
	int i, j;
	if(width >= bitmap->width && height >= bitmap->height)
	{/* ����³ߴ����ԭ���ĳߴ� */
		bitmap->data = (unsigned char**)realloc(
						bitmap->data, height*sizeof(unsigned char*));
		for(i = 0; i < height; ++i)
		{   /* Ϊ����ͼ��ÿһ�������ڴ� */
			bitmap->data[i] = (unsigned char*)realloc(
					bitmap->data[i], sizeof(unsigned char) * width); 
			/* �������Ĳ������� */
			if(i < bitmap->height)  
				for(j = bitmap->width; j < width; ++j)
					bitmap->data[i][j] = 0; 
			else  
				memset(bitmap->data[i], 0 , bitmap->width); 
		}
	}
}

void Free_Font(LCUI_Font *in)
/* ���ܣ��ͷ�Font�ṹ������ռ�õ��ڴ���Դ */
{
	Free_String(&in->font_file);
	Free_String(&in->family_name);
	Free_String(&in->style_name);
	if(in->status == ACTIVE)
	{ 
		in->status = KILLED;
		if(in->type == CUSTOM)
		{ 
			FT_Done_Face(in->ft_face);
			FT_Done_FreeType(in->ft_lib);
		} 
		in->ft_lib = NULL;
		in->ft_face = NULL;
	} 
}

void Free_LCUI_Font()
/* ���ܣ��ͷ�LCUIĬ�ϵ�Font�ṹ������ռ�õ��ڴ���Դ */
{ 
	Free_String(&LCUI_Sys.default_font.font_file);
	Free_String(&LCUI_Sys.default_font.family_name);
	Free_String(&LCUI_Sys.default_font.style_name);
	if(LCUI_Sys.default_font.status == ACTIVE)
	{
		LCUI_Sys.default_font.status = KILLED;
		FT_Done_Face(LCUI_Sys.default_font.ft_face);
		FT_Done_FreeType(LCUI_Sys.default_font.ft_lib); 
		LCUI_Sys.default_font.ft_lib = NULL;
		LCUI_Sys.default_font.ft_face = NULL;
	}
}




