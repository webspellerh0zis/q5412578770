/* ***************************************************************************
 * bitmapfont.c -- The Bitmap Font operation set.
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
 * bitmapfont.c -- λͼ����Ĳ�����
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
 
//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_FONT_H
#include LC_MEM_H
#include LC_ERROR_H
#include LC_GRAPH_H

#include <iconv.h>

#include FT_GLYPH_H

extern uchar_t const *in_core_font_8x8();

int FontBMP_Valid(LCUI_FontBMP *bitmap)
/*
 * ���ܣ����λͼ�����Ƿ���Ч
 * ����ֵ����Ч����1����Ч����0
 */
{
	if(NULL != bitmap && bitmap->width * bitmap->height > 0) {
		return 1; 
	}
	return 0;
}

void Print_FontBMP_Info(LCUI_FontBMP *bitmap)
/* ���ܣ���ӡλͼ����Ϣ */
{
	printf("address:%p\n",bitmap);
	if(bitmap == NULL) {
		return;
	}
	printf("width:%d,height:%d,alpha:%u\n", 
	bitmap->width, bitmap->height, bitmap->alpha);
}

void FontBMP_Init(LCUI_FontBMP *bitmap)
/* ��ʼ������λͼ */
{
	bitmap->width = 0;
	bitmap->height = 0;
	bitmap->data = NULL;
	bitmap->alpha = 255;
}

void FontBMP_Free(LCUI_FontBMP *bitmap)
/* �ͷ�����λͼռ�õ���Դ */
{
	if(FontBMP_Valid(bitmap)) {
		free(bitmap->data);
		FontBMP_Init(bitmap);
	}
}

void FontBMP_Create(LCUI_FontBMP *bitmap, int width, int height)
/* ���ܣ�ΪBitmap�ڵ����ݷ����ڴ���Դ������ʼ�� */
{
	int i;
	if(width < 0 || height < 0) {
		return;
	}
	if(FontBMP_Valid(bitmap)) {
		FontBMP_Free(bitmap);
	}
	bitmap->width = width;
	bitmap->height = height;
	bitmap->data = (uchar_t**)malloc(bitmap->height*sizeof(uchar_t*));
	/* Ϊ����ͼ��ÿһ�з����ڴ� */
	for(i = 0; i < bitmap->height; ++i) { 
		bitmap->data[i] = (uchar_t*)calloc(1, 
				sizeof(uchar_t) * bitmap->width); 
	}
	bitmap->alpha  = 255; /* ����λͼĬ�ϲ�͸�� */
}

void FontBMP_Resize(LCUI_FontBMP *bitmap, int width, int height)
/* ���ܣ�����λͼ�ĳߴ� */
{
	int i, j;
	/* ����³ߴ粻����ԭ���ĳߴ� */
	if(width < bitmap->width || height < bitmap->height) {
		return;
	} 
	bitmap->data = (uchar_t**)realloc( bitmap->data, 
				height*sizeof(uchar_t*) );
	for(i = 0; i < height; ++i) {   /* Ϊ����ͼ��ÿһ�������ڴ� */
		bitmap->data[i] = (uchar_t*)realloc( bitmap->data[i], 
					sizeof(uchar_t) * width); 
		/* �������Ĳ������� */
		if(i < bitmap->height) {
			for(j = bitmap->width; j < width; ++j) {
				bitmap->data[i][j] = 0; 
			}
		} else {
			memset(bitmap->data[i], 0 , bitmap->width); 
		}
	} 
}

void Get_Default_FontBMP(unsigned short code, LCUI_FontBMP *out_bitmap)
/* ���ܣ������ַ����룬��ȡϵͳ�Դ�������λͼ */
{
	int i,j, start;
	uchar_t const *p;
	
	p = in_core_font_8x8();
	FontBMP_Create(out_bitmap, 9, 9);/* Ϊλͼ�����ڴ棬9x9�ĳߴ� */
	if(code < 256) {
		start = code * 8;
		for (i=start;i<start+8;++i) {
			for (j=0;j<8;++j) {/* ����ֵת����һ��λͼ */
				out_bitmap->data[i-start][7-j] = 
						(p[i]&(1<<j))?255:0;  
			}
		}
	} else {
		uchar_t error_bitmap[] = {
			1,1,1,1,1,1,1,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,1,1,1,1,1,1,1 
		};
		for (i=0;i<8;i++) {
			for (j=0;j<8;j++) {
				out_bitmap->data[i][j]= 
					(error_bitmap[i*8+j] > 0) ?255:0; 
			}
		}
	}
}

/* Ĭ�ϵ������ļ�·�� */
static char default_font[1024] = LCUI_DEFAULT_FONTFILE;

void Set_Default_Font(char *fontfile)
/* 
 * ���ܣ��趨Ĭ�ϵ������ļ�·��
 * ˵������Ҫ��LCUI��ʼ��ǰʹ�ã���ΪLCUI��ʼ��ʱ���Ĭ�ϵ������ļ�
 *  */
{
	if(fontfile == NULL) {
		strcpy(default_font, "");
	} else {
		strcpy(default_font, fontfile);
	}
}

void LCUI_Font_Init(LCUI_Font *font)
/* 
 * ���ܣ���ʼ��LCUI��Font�ṹ������ 
 * ˵������������LCUI��ʼ��ʱ���ã�LCUI_Font�ṹ���м�¼��������ص�����
 * */
{ 
	char *p;
	printf("loading fontfile...\n");/* �޻����ӡ���� */
	font->type = DEFAULT;
	font->size = 12;
	font->fore_color.red = 0;
	font->fore_color.green = 0;
	font->fore_color.blue = 0;
	String_Init(&font->font_file);
	String_Init(&font->family_name);
	String_Init(&font->style_name);
	font->space = 1;
	font->linegap = 0;
	font->status = KILLED;
	//font->load_flags = FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT;
	//font->load_flags = FT_LOAD_RENDER | FT_LOAD_MONOCHROME;
	font->load_flags = FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT; 
	font->render_mode = FT_RENDER_MODE_MONO;
	font->ft_lib = NULL;
	font->ft_face = NULL;
	/* ����ڻ��������ж����������ļ�·�����Ǿ�ʹ���� */
	p = getenv("LCUI_FONTFILE");
	if(p != NULL) {
		strcpy(default_font, p); 
	}
	/* ��Ĭ�������ļ� */
	Open_Fontfile(&LCUI_Sys.default_font, default_font); 
}

void Font_Init(LCUI_Font *in)
/* 
 * ���ܣ���ʼ��Font�ṹ������
 * ˵����Ĭ���Ǽ̳�ϵͳ����������
 * */
{
	in->type = DEFAULT;   /* ��������ΪLCUIĬ�ϵ� */
	in->size = 12;
	in->fore_color.red = 0;
	in->fore_color.green = 0;
	in->fore_color.blue = 0;
	String_Init(&in->font_file);
	String_Init(&in->family_name);
	String_Init(&in->style_name);
	if(LCUI_Sys.default_font.status == ACTIVE) {
		in->status = ACTIVE;
		LCUI_Strcpy(&in->family_name, &LCUI_Sys.default_font.family_name);
		LCUI_Strcpy(&in->style_name, &LCUI_Sys.default_font.style_name);
		LCUI_Strcpy(&in->font_file, &LCUI_Sys.default_font.font_file);
		in->ft_lib = LCUI_Sys.default_font.ft_lib;
		in->ft_face = LCUI_Sys.default_font.ft_face;
	} else {
		in->status = KILLED;
		in->ft_lib = NULL;
		in->ft_face = NULL;
	}
	in->space = 1;
	in->linegap = 0;
	in->load_flags = LCUI_Sys.default_font.load_flags;
	in->render_mode = LCUI_Sys.default_font.render_mode;
}

void Font_Free(LCUI_Font *in)
/* ���ܣ��ͷ�Font�ṹ������ռ�õ��ڴ���Դ */
{
	String_Free(&in->font_file);
	String_Free(&in->family_name);
	String_Free(&in->style_name);
	if(in->status == ACTIVE) { 
		in->status = KILLED;
		if(in->type == CUSTOM) { 
			FT_Done_Face(in->ft_face);
			FT_Done_FreeType(in->ft_lib);
		}
		in->ft_lib = NULL;
		in->ft_face = NULL;
	}
}

void LCUI_Font_Free()
/* ���ܣ��ͷ�LCUIĬ�ϵ�Font�ṹ������ռ�õ��ڴ���Դ */
{ 
	String_Free(&LCUI_Sys.default_font.font_file);
	String_Free(&LCUI_Sys.default_font.family_name);
	String_Free(&LCUI_Sys.default_font.style_name);
	if(LCUI_Sys.default_font.status == ACTIVE) {
		LCUI_Sys.default_font.status = KILLED;
		FT_Done_Face(LCUI_Sys.default_font.ft_face);
		FT_Done_FreeType(LCUI_Sys.default_font.ft_lib); 
		LCUI_Sys.default_font.ft_lib = NULL;
		LCUI_Sys.default_font.ft_face = NULL;
	}
}


int Show_FontBMP(LCUI_FontBMP *in_fonts)
/* ���ܣ�����Ļ��ӡ��0��1��ʾ����λͼ */
{
	int x,y;
	for(y = 0;y < in_fonts->height; ++y){
		for(x = 0; x < in_fonts->width; ++x){
			if(in_fonts->data[y*in_fonts->width + x] > 0)
				printf("1");
			else printf("0");
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

int FontBMP_Mix(
		LCUI_Graph	*back_graph,  /* ����ͼ�� */
		int		start_x,
		int		start_y,
		LCUI_FontBMP	*in_fonts,  /* ���������λͼ���� */
		LCUI_RGB	color,       /* �������ɫ */ 
		int		flag
)
/* ���ܣ�������λͼ�����뱳��ͼ�λ�� */
{
	if(!FontBMP_Valid(in_fonts) || !Graph_Valid(back_graph)) {
		return -1;
	}
	
	float k;
	int count = 0, m;
	uchar_t j;
	LCUI_Rect read, write;/* ���ڼ�¼����ķ�Χ������ */
	int x = 0, y = 0, end_x, end_y;//���½ǵ�����
	int box_width, box_height, width, height;
	
	Rect_Init(&read);
	Rect_Init(&write);
	
	/* ����ͼƬ�ĳߴ� */
	width = in_fonts->width;
	height = in_fonts->height;
	read.width = width;
	read.height = height;
	/* ��ȡ����������ĳߴ� */
	box_width = back_graph->width;
	box_height = back_graph->height;
	/* �õ�ͼ�����½ǵ������ */
	end_x = start_x + width;
	end_y = start_y + height;

	/* ���ͼƬ�ߴ糬�������޶��ĳߴ磬����ͼƬ��ʾ������ */
	if(start_x < 0) {/* ��������x�������С��0 */
		read.x = 0 - start_x; /* �ı��ȡ������������x������� */
		read.width = read.width - read.x;
		start_x = 0;
	}
	if(end_x > box_width) {/* �����������ʾ���� */
		read.width = read.width - (end_x - box_width);
		end_x = box_width;
	}
	if(start_y < 0) {
		read.y = 0 - start_y;
		read.height = read.height - read.y;
		start_y = 0;
	}
	if(end_y > box_height) {
		read.height = read.height - (end_y - box_height);
		end_y = box_height;
	}
	
	k = in_fonts->alpha / 255.0;
	/* ��ȡ�������������������Χ */
	write.x = start_x;
	write.y = start_y;
	/* ��ʼ��ȡͼƬ�е�ͼ�����鲢д�봰�� */
	switch(flag) {
		case GRAPH_MIX_FLAG_OVERLAY:
			for (y = 0; y < read.height; ++y) {
				m = (write.y + y) * back_graph->width + write.x; 
				for (x = 0; x < read.width; ++x) {
					count = m + x;/* ��������������ڵĸ�������� */
					j = in_fonts->data[read.y + y][read.x + x] * k;
					back_graph->rgba[0][count] = (color.red * j + back_graph->rgba[0][count] * (255 - j)) /255;
					back_graph->rgba[1][count] = (color.green * j + back_graph->rgba[1][count] * (255 - j)) /255;
					back_graph->rgba[2][count] = (color.blue * j + back_graph->rgba[2][count] * (255 - j)) /255;
				}
			}
			break;
		case GRAPH_MIX_FLAG_REPLACE:
			for (y = 0; y < read.height; ++y) {
				m = (write.y + y) * back_graph->width + write.x; 
				for (x = 0; x < read.width; ++x) {
					count = m + x;/* ��������������ڵĸ�������� */
					j = in_fonts->data[read.y + y][read.x + x] * k;
					if(j != 0) {
						back_graph->rgba[0][count] = color.red;
						back_graph->rgba[1][count] = color.green;
						back_graph->rgba[2][count] = color.blue;
						back_graph->rgba[3][count] = j;
					}
				}
			}
			break;
		default : flag = GRAPH_MIX_FLAG_REPLACE;
	}
	return 0;
}

int Open_Fontfile(LCUI_Font *font_data, char *fontfile)
/* ���ܣ��������ļ���������������LCUI_Font�ṹ���� */
{
	int type;
	FT_Library    library;
	FT_Face       face;
	FT_Error      face_error = 0, lib_error = 0;
	
	type = font_data->type;
	if(font_data->status == ACTIVE) {
		if(fontfile == NULL 
		|| Strcmp(&font_data->font_file, fontfile) == 0) {
			return 0;
		} else if( Strcmp(&font_data->font_file, 
				LCUI_Sys.default_font.font_file.string)) {
			type = CUSTOM;
		}
	}
	else if(fontfile == NULL) {
		return -1;
	}
	lib_error = FT_Init_FreeType( & library);  /* ��ʼ��FreeType�� */
	if (lib_error) { /* ����ʼ����ʱ������һ������ */
		printf("open fontfile: "FT_INIT_ERROR);
		return - 1 ;
	}
	
	face_error = FT_New_Face( library, fontfile , 0 , & face );
	if(face_error) {
		FT_Done_FreeType(library);
		if ( face_error == FT_Err_Unknown_File_Format )
			printf("open fontfile: "FT_UNKNOWN_FILE_FORMAT); /* δ֪�ļ���ʽ */ 
		else 
			printf("open fontfile: "FT_OPEN_FILE_ERROR);/* �򿪴��� */
		perror(fontfile);
		return -1;
	}
	printf(
		"=============== font info ==============\n" 
		"family name: %s\n"
		"style name : %s\n"
		"========================================\n" ,
		face->family_name,
		face->style_name
	);
	Font_Free(font_data);
	/* ����������Ϣ */
	Strcpy(&font_data->family_name, face->family_name);
	Strcpy(&font_data->style_name, face->style_name);
	Strcpy(&font_data->font_file, fontfile);
	font_data->type = type;
	font_data->status = ACTIVE; 
	font_data->ft_lib = library;
	font_data->ft_face = face;
	return 0;
}

int Get_FontBMP(LCUI_Font *font_data, wchar_t ch, LCUI_FontBMP *out_bitmap)
/*
 * ���ܣ���ȡ����wchar_t���ַ���λͼ
 * ˵����LCUI_Font�ṹ���д������ѱ��򿪵������ļ������face����ľ������������ļ��Ѿ���
 * �ɹ���һ�Σ��˺��������ٴδ������ļ���
 */
{ 
	FT_Face         p_FT_Face = NULL;   /* face����ľ�� */
	FT_Bitmap       bitmap;         
	FT_BitmapGlyph  bitmap_glyph;    
	FT_Glyph        glyph;
	FT_GlyphSlot    slot;               /* ���β۵ľ�� */
	FT_Error        error = 0;
	int i , j = 0, value = 0;
	int k, start_y = 0, ch_height = 0, ch_width = 0;
	
	DEBUG_MSG("Get_FontBMP():char: %d\n", ch);
	if(font_data != NULL) {
	 /* ���LCUI_Font�ṹ���е�������Ϣ��Ч���ʹ򿪽ṹ���е�ָ���������ļ�����
	  * �������ļ���face����ľ���������ṹ���С�
	  * ��Ȼ�����LCUI_Font�ṹ���������ļ���face����ľ������ֱ�ӷ���0��
	  */
		if(font_data->ft_face == NULL 
		|| font_data->ft_lib == NULL ) { 
			error = Open_Fontfile(font_data, 
					font_data->font_file.string);
			if(error) {
				value = -1; 
			}
		}
		p_FT_Face = font_data->ft_face; 
	} else {
		return -1;
	}
	
	/* ���ܻ�ȡ�����ļ�����ʹ�����õ�8x8�������� */
	if(value != 0) {
		if(ch == ' ') { /* ����ǿո� */
			/* �Զ���ĺ��������ڻ�ȡ8x8��������λͼ */
			Get_Default_FontBMP('a', out_bitmap);
			for (i=0;i<8;i++)  
			for (j=0;j<8;j++)  
				out_bitmap->data[i][j]= 0; 
		} else { 
			FontBMP_Create(out_bitmap, 8, 8);
			Get_Default_FontBMP(ch, out_bitmap);
		}
		return 0;
	}
	/* ����������������ļ� */
	FT_Select_Charmap(p_FT_Face, FT_ENCODING_UNICODE);   /* �趨ΪUNICODE��Ĭ�ϵ�Ҳ�� */
	FT_Set_Pixel_Sizes(p_FT_Face, 0, font_data->size);   /* �趨�����С */
	slot = p_FT_Face->glyph;
	if(ch == ' ') { /* ����пո�,���Ŀ�Ⱦ�����ĸa�Ŀ��Ϊ׼ */
		error = FT_Load_Char( p_FT_Face, 'a', font_data->load_flags); 
		if(error) {
			return error; 
		}
		
		error = FT_Get_Glyph(p_FT_Face -> glyph, &glyph);
		if(error) {
			return error; 
		}
		
		FontBMP_Free(out_bitmap);
		/* ����ͼ�εĸ߶ȣ�����߶�Ҫ��������ĸ߶ȣ�������+3 */
		/* 256���Ҷ�����ת����λͼ */
		FT_Glyph_To_Bitmap(&glyph, font_data->render_mode, 0 ,1);
		/* FT_RENDER_MODE_NORMAL ����Ĭ����Ⱦģʽ������Ӧ��8λ�����λͼ */
		bitmap_glyph = (FT_BitmapGlyph)glyph;
		bitmap       = bitmap_glyph -> bitmap;
		/* �������ڵı���ͼ�ĳߴ���Ҫ��һ�� */
		out_bitmap->height = font_data->size + 3;
		FontBMP_Create(out_bitmap, bitmap.width, out_bitmap->height);
		/* �ͷ�����ռ�õ��ڴ� */
		FT_Done_Glyph(glyph);
		glyph = NULL; 
		return 0;
	}
	/* �������ֻ�Ǽ򵥵ص���FT_Get_Char_Index��FT_Load_Glyph */
	error = FT_Load_Char( p_FT_Face, ch, font_data->load_flags);
	if(error) {
		return error; 
	}
	
	/* �Ӳ������ȡһ������ͼ�� 
	 * ��ע�⣬������FT_Glyph���������FT_Done_Glyph�ɶ�ʹ�� */
	error = FT_Get_Glyph(p_FT_Face->glyph, &glyph);
	if(error) {
		return error; 
	}
	
	int bg_height;
	FontBMP_Free(out_bitmap);
	/* ����ͼ�εĸ߶ȣ�����߶�Ҫ��������ĸ߶ȣ�������+4 */
	bg_height = font_data->size + 4; 
	/* 256���Ҷ�����ת����λͼ */
	FT_Glyph_To_Bitmap(&glyph, font_data->render_mode, 0 ,1);
	/* FT_RENDER_MODE_NORMAL ��Ĭ����Ⱦģʽ������Ӧ��8λ�����λͼ�� */
	bitmap_glyph = (FT_BitmapGlyph)glyph;
	bitmap       = bitmap_glyph -> bitmap;
	k = 0;
	/* ��ȡ����y������ */
	start_y = font_data->size - slot->bitmap_top; 
	ch_width = bitmap.width;
	/* ��������λͼ�ڱ���ͼ�еķ�Χ */
	if(start_y < 0) {
		start_y = 0; 
	}
	if(bitmap.rows > bg_height) {
		ch_height = font_data->size; 
	} else {
		ch_height = bitmap.rows; 
	}
		
	if(ch_height + start_y > bg_height) {
		ch_height = bg_height - start_y; 
	}
	/* �����ڴ�ռ�,����������⣬�ᵼ��FT_Done_Glyph�������ֶδ��� */
	FontBMP_Create(out_bitmap, ch_width, bg_height);
	/* ��ʼ������λͼ��������ͼ���� */
	for(i = 0; i < ch_height; ++i) 
	for(j = 0;j < ch_width; ++j) {
		switch (bitmap.pixel_mode) {
		    case FT_PIXEL_MODE_GRAY:
			/* һ��8λλͼ��һ��������ʾ���������ͼ��ÿ��������һ���ֽڴ洢 */
			out_bitmap->data[start_y + i][j] = bitmap.buffer[k];
			break;
		    case FT_PIXEL_MODE_MONO: 
			/* һ����ɫλͼ,ÿ��bit��Ӧһ����,�Ǻڼ��� */
			out_bitmap->data[start_y + i][j] = bitmap.buffer[k]?255:0;
			break;
		}
		++k;
	} 
	/* �ͷ�����ռ�õ��ڴ� */
	FT_Done_Glyph(glyph);
	glyph = NULL; 
	
	//Print_FontBMP_Info(out_bitmap);
	return 0;
}


