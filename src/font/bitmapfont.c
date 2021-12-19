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
#include "config.h"
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_FONT_H
#include LC_MEM_H
#include LC_ERROR_H
#include LC_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#ifdef USE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#endif

extern uchar_t const *in_core_font_8x8();

int FontBMP_Valid(LCUI_FontBMP *bitmap)
/*
 * ���ܣ����λͼ�����Ƿ���Ч
 * ����ֵ����Ч����1����Ч����0
 */
{
	if(NULL != bitmap && bitmap->width > 0 && bitmap->rows > 0) {
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
	printf("width:%d, rows:%d\n", bitmap->width, bitmap->rows);
}

void FontBMP_Init(LCUI_FontBMP *bitmap)
/* ��ʼ������λͼ */
{
	bitmap->rows = 0;
	bitmap->width = 0;
	bitmap->top = 0;
	bitmap->left = 0; 
	bitmap->num_grays = 0;
	bitmap->pixel_mode = 0;
	bitmap->buffer = NULL; 
}

void FontBMP_Free(LCUI_FontBMP *bitmap)
/* �ͷ�����λͼռ�õ���Դ */
{
	if(FontBMP_Valid(bitmap)) {
		free(bitmap->buffer);
		FontBMP_Init(bitmap);
	}
}

int FontBMP_Create(LCUI_FontBMP *bitmap, int width, int rows)
/* ���ܣ���������λͼ */
{ 
	size_t size;
	if(width < 0 || rows < 0) {
		FontBMP_Free(bitmap);
		return -1;
	}
	if(FontBMP_Valid(bitmap)) {
		FontBMP_Free(bitmap);
	}
	bitmap->width = width;
	bitmap->rows = rows;
	size = width*rows*sizeof(uchar_t);
	bitmap->buffer = (uchar_t*)malloc( size );
	if( bitmap->buffer == NULL ) {
		return -2;
	}
	return 0;
}

void Get_Default_FontBMP(unsigned short code, LCUI_FontBMP *out_bitmap)
/* ���ܣ������ַ����룬��ȡϵͳ�Դ�������λͼ */
{
	int i,j, start, m;
	uchar_t const *p;
	
	p = in_core_font_8x8();
	FontBMP_Create(out_bitmap, 8, 8);/* Ϊλͼ�����ڴ棬8x8�ĳߴ� */
	if(code < 256) {
		if(code == ' ') {
			memset(out_bitmap->buffer, 0, sizeof(uchar_t)*64); 
		} else {
			start = code * 8;
			for (i=start;i<start+8;++i) {
				m = (i-start) * 8 + 7;
				/* ����ֵת����һ��λͼ */
				for (j=0;j<8;++j,--m) {
					out_bitmap->buffer[m] 
					 = (p[i]&(1<<j))?255:0;  
				}
			}
		}
	} else {
		uchar_t null_bmp[] = {
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
			m = i*8;
			for (j=0;j<8;j++,++m) {
				out_bitmap->buffer[m] = (null_bmp[m]>0)?255:0; 
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
#ifdef USE_FREETYPE
	font->load_flags = FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT; 
	font->render_mode = FT_RENDER_MODE_MONO;
#else
	font->load_flags = 0;
	font->render_mode = 0;
#endif
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
#ifdef USE_FREETYPE
			FT_Done_Face(in->ft_face);
			FT_Done_FreeType(in->ft_lib);
#endif
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
#ifdef USE_FREETYPE
		FT_Done_Face(LCUI_Sys.default_font.ft_face);
		FT_Done_FreeType(LCUI_Sys.default_font.ft_lib); 
#endif
		LCUI_Sys.default_font.ft_lib = NULL;
		LCUI_Sys.default_font.ft_face = NULL;
	}
}


int Show_FontBMP(LCUI_FontBMP *fontbmp)
/* ���ܣ�����Ļ��ӡ��0��1��ʾ����λͼ */
{
	int x,y,m;
	for(y = 0;y < fontbmp->rows; ++y){
		m = y*fontbmp->width;
		for(x = 0; x < fontbmp->width; ++x,++m){
			if(fontbmp->buffer[m] > 0) {
				printf("1");
			} else {
				printf("0");
			}
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

int FontBMP_Mix( LCUI_Graph	*graph, LCUI_Pos	des_pos,
		LCUI_FontBMP	*bitmap, LCUI_RGB	color,
		int flag )
/* ���ܣ�������λͼ�����뱳��ͼ�λ�� */
{
	LCUI_Graph *des;
	LCUI_Rect des_rect, cut;
	uint_t total, m, n, y;
	
	des_rect = Get_Graph_Valid_Rect( graph );
	des = Get_Quote_Graph( graph ); 
	
	if( !FontBMP_Valid( bitmap ) || !Graph_Valid( graph ) ) {
		return -1;
	}
	if(des_pos.x > des->width || des_pos.y > des->height) {
		return -2;
	}
	
	if( Get_Cut_Area( Size( des_rect.width, des_rect.height ),
		Rect( des_pos.x, des_pos.y, bitmap->width, bitmap->rows ),
		&cut
	)) {
		des_pos.x += cut.x;
		des_pos.y += cut.y;
	}
	
	Graph_Lock( des, 1 );
	/* ��ʼ��ȡͼƬ�е�ͼ�����鲢д�봰�� */
	if( flag == GRAPH_MIX_FLAG_OVERLAY ) {
		for (y = 0; y < cut.height; ++y) {
			m = (cut.y + y) * bitmap->width + cut.x;
			n = (des_pos.y + y + des_rect.y) * des->width + des_pos.x + des_rect.x;
			total = n + cut.width;
			for (; n < total; ++n,++m) { 
				des->rgba[0][n] = ALPHA_BLENDING(color.red, des->rgba[0][n], bitmap->buffer[m]);
				des->rgba[1][n] = ALPHA_BLENDING(color.green, des->rgba[1][n], bitmap->buffer[m]);
				des->rgba[2][n] = ALPHA_BLENDING(color.blue, des->rgba[2][n], bitmap->buffer[m]);
			}
		}
	} else {
		for (y = 0; y < cut.height; ++y) {
			m = (cut.y + y) * bitmap->width + cut.x;
			n = (des_pos.y + y + des_rect.y) * des->width + des_pos.x + des_rect.x;
			total = n + cut.width;
			for (; n < total; ++n,++m) { 
				if(bitmap->buffer[m] != 0) {
					des->rgba[0][n] = color.red;
					des->rgba[1][n] = color.green;
					des->rgba[2][n] = color.blue;
					des->rgba[3][n] = bitmap->buffer[m];
				}
			}
		} 
	}
	Graph_Unlock( des );
	return 0;
}

int Open_Fontfile(LCUI_Font *font_data, char *fontfile)
/* ���ܣ��������ļ���������������LCUI_Font�ṹ���� */
{
#ifdef USE_FREETYPE
	int type;
	FT_Library    library;
	FT_Face       face;
	FT_Error      face_error = 0, lib_error = 0;
	
	type = font_data->type;
	if(font_data->status == ACTIVE) {
		if(fontfile == NULL 
		|| Strcmp(&font_data->font_file, fontfile) == 0) {
			return 0;
		}
		else if( Strcmp(&font_data->font_file, 
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
#else
	printf("warning: not font engine support!\n");
	return -1;
#endif
}

#ifdef USE_FREETYPE
static int Convert_FT_BitmapGlyph(LCUI_FontBMP *des, const FT_BitmapGlyph src)
{
	static size_t size;
	
	des->top = src->top;
	des->left = src->left;
	des->rows = src->bitmap.rows;
	des->width = src->bitmap.width;
	des->pixel_mode = src->bitmap.pixel_mode;
	des->num_grays = src->bitmap.num_grays;
	
	size = des->rows * des->width * sizeof(uchar_t);
	des->buffer = malloc( size );
	if( des->buffer == NULL ) {
		return -1;
	}
	memcpy( des->buffer, src->bitmap.buffer, size );
	
	DEBUG_MSG("des->top: %d, des->left: %d, des->rows: %d\n", des->top, des->left, des->rows);
	return size;
}
#endif

int Get_FontBMP(LCUI_Font *font_data, wchar_t ch, LCUI_FontBMP *out_bitmap)
/*
 * ���ܣ���ȡ����wchar_t���ַ�������λͼ����
 * ˵����LCUI_Font�ṹ���д������ѱ��򿪵������ļ������face����ľ������������ļ��Ѿ���
 * �ɹ���һ�Σ��˺��������ٴδ������ļ���
 */
{
#ifdef USE_FREETYPE
	size_t size;
	BOOL have_space = IS_FALSE;
	
	FT_Face         p_FT_Face = NULL;   /* face����ľ�� */ 
	FT_BitmapGlyph  bitmap_glyph;
	FT_Glyph        glyph; 
	FT_Error        error;
	
	if(font_data != NULL) {
	 /* ���LCUI_Font�ṹ���е�������Ϣ��Ч���ʹ򿪽ṹ���е�ָ���������ļ�����
	  * �������ļ���face����ľ���������ṹ���С�
	  * ��Ȼ�����LCUI_Font�ṹ������Ч�������ļ���face����ľ������ֱ�ӷ���0��
	  */
		if(font_data->ft_face == NULL 
		 || font_data->ft_lib == NULL ) { 
			error = Open_Fontfile( font_data, 
					font_data->font_file.string);
			if(error) {
				Get_Default_FontBMP( ch, out_bitmap );
				return 1;
			}
		}
		p_FT_Face = font_data->ft_face; 
	} else {
		Get_Default_FontBMP( ch, out_bitmap );
		return -1;
	}
	
	FT_Select_Charmap( p_FT_Face, FT_ENCODING_UNICODE ); /* �趨ΪUNICODE��Ĭ�ϵ�Ҳ�� */
	FT_Set_Pixel_Sizes( p_FT_Face, 0, font_data->size ); /* �趨����ߴ� */ 
	
	if( ch == ' ' ) {
		ch = 'a';
		have_space = IS_TRUE;
	}
	/* �������ֻ�Ǽ򵥵ص���FT_Get_Char_Index��FT_Load_Glyph */
	error = FT_Load_Char( p_FT_Face, ch, font_data->load_flags);
	if(error) {
		return error; 
	}
	
	/* �Ӳ������ȡһ������ͼ�� 
	 * ��ע�⣬������FT_Glyph���������FT_Done_Glyph�ɶ�ʹ�� */
	error = FT_Get_Glyph( p_FT_Face->glyph, &glyph );
	if(error) {
		return error; 
	}
	if ( glyph->format != FT_GLYPH_FORMAT_BITMAP ) {
		error = FT_Glyph_To_Bitmap(&glyph, font_data->render_mode, 0 ,1);
		if(error) {
			return error;
		}
	}
	bitmap_glyph = (FT_BitmapGlyph)glyph;
	size = Convert_FT_BitmapGlyph (out_bitmap, bitmap_glyph);
	if( have_space ) {
		memset( out_bitmap->buffer, 0, size );
	}
	FT_Done_Glyph(glyph);
	return 0;
#else
	Get_Default_FontBMP( ch, out_bitmap );
	return -1;
#endif
}


