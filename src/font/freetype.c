/* ***************************************************************************
 * freetype.c -- The FreeType font-engine support module.
 *
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
 * freetype.c -- FreeType ���������֧��ģ�顣
 *
 * ��Ȩ���� (C) 2015 ������ ���� <lc-soft@live.cn>
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
#ifdef LCUI_FONT_ENGINE_FREETYPE
#include <LCUI/LCUI.h>
#include <LCUI/font.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H

#define LCUI_FONT_RENDER_MODE	FT_RENDER_MODE_NORMAL
#define LCUI_FONT_LOAD_FALGS	(FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT)

static struct {
	FT_Library library;
} freetype;

static int FreeType_Open( const char *filepath, LCUI_FontFace **out_face )
{
	int err;
	FT_Face ft_face;
	LCUI_FontFace *my_face;

	err = FT_New_Face( freetype.library, filepath, 0, &ft_face );
	if( err ) {
		*out_face = NULL;
		return err;
	}
	my_face = (LCUI_FontFace*)malloc(sizeof(LCUI_FontFace));
	FT_Select_Charmap( ft_face, FT_ENCODING_UNICODE );
	strcpy( my_face->family_name, ft_face->family_name );
	strcpy( my_face->style_name, ft_face->style_name );
	my_face->data = ft_face;
	*out_face = my_face;
	return 0;
}

static void FreeType_Close( void *face )
{
	FT_Done_Face( face );
}

/** ת�� FT_GlyphSlot ��������Ϊ LCUI_FontBMP */
static size_t Convert_FTGlyph( LCUI_FontBMP *bmp, FT_GlyphSlot slot, int mode )
{
	int error;
	size_t size;
	FT_BitmapGlyph bitmap_glyph;
	FT_Glyph  glyph;

	/* �����β�����ȡһ������ͼ��
	 * ��ע�⣬������FT_Glyph���������FT_Done_Glyph�ɶ�ʹ�� */
	error = FT_Get_Glyph( slot, &glyph );
	if(error) {
		return -1;
	}
	/*---------------------- ��ӡ������Ϣ --------------------------
	printf(" width= %ld,  met->height= %ld\n"
	"horiBearingX = %ld, horiBearingY = %ld, horiAdvance = %ld\n"
	"vertBearingX = %ld, vertBearingY = %ld,  vertAdvance = %ld\n",
	slot->metrics.width>>6, slot->metrics.height>>6,
	slot->metrics.horiBearingX>>6, slot->metrics.horiBearingY>>6,
	slot->metrics.horiAdvance>>6, slot->metrics.vertBearingX>>6,
	slot->metrics.vertBearingY>>6, slot->metrics.vertAdvance>>6 );
	------------------------------------------------------------*/
	if ( glyph->format != FT_GLYPH_FORMAT_BITMAP ) {
		error = FT_Glyph_To_Bitmap(&glyph, mode, 0 ,1);
		if(error) {
			return -1;
		}
	}
	bitmap_glyph = (FT_BitmapGlyph)glyph;
	/*
	 * FT_Glyph_Metrics�ṹ���б������ζ�����ͨ��face->glyph->metrics��
	 * �����ʣ��ɵõ����εĿ��ߡ���߽�ࡢ�ϱ߽�ࡢˮƽ���ȵȡ�
	 * ע�⣺��Ϊ�������е����嶼������ֱ��������FT_HAS_VERTICALΪ��ʱ��
	 * vertBearingX��vertBearingY��vertAdvance��ֵ�ǲ��ɿ��ģ�Ŀǰ�ݲ�����
	 * ������Ĵ���
	 * */
	bmp->top = bitmap_glyph->top;
	bmp->left = slot->metrics.horiBearingX>>6;
	bmp->rows = bitmap_glyph->bitmap.rows;
	bmp->width = bitmap_glyph->bitmap.width;
	bmp->advance.x = slot->metrics.horiAdvance>>6;	/* ˮƽ��� */
	bmp->advance.y = slot->metrics.vertAdvance>>6;	/* ��ֱ��� */
	/* �����ڴ棬���ڱ�������λͼ */
	size = bmp->rows * bmp->width * sizeof(uchar_t);
	bmp->buffer = (uchar_t*)malloc( size );
	if( !bmp->buffer ) {
		FT_Done_Glyph(glyph);
		return -1;
	}

	switch( bitmap_glyph->bitmap.pixel_mode ) {
	    /* 8λ�Ҷ�λͼ��ֱ�ӿ��� */
	    case FT_PIXEL_MODE_GRAY:
		memcpy( bmp->buffer, bitmap_glyph->bitmap.buffer, size );
		break;
	    /* ��ɫ����ͼ����Ҫת�� */
	    case FT_PIXEL_MODE_MONO: {
		FT_Bitmap bitmap;
		FT_Int x, y;
		uchar_t *bit_ptr, *byte_ptr;

		FT_Bitmap_New( &bitmap );
		/* ת��λͼbitmap_glyph->bitmap��bitmap��1������ռ1���ֽ� */
		FT_Bitmap_Convert( freetype.library, &bitmap_glyph->bitmap, &bitmap, 1 );
		bit_ptr = bitmap.buffer;
		byte_ptr = bmp->buffer;
		for( y=0; y<bmp->rows; ++y ) {
			for( x=0; x<bmp->width; ++x ) {
				*byte_ptr = *bit_ptr ? 255:0;
				++byte_ptr, ++bit_ptr;
			}
		}
		FT_Bitmap_Done( freetype.library, &bitmap );
		break;
	    }
	    /* ��������ģʽ��λͼ����ʱ��ֱ�����255������Ҫʱ������ */
	    case FT_PIXEL_MODE_BGRA:
	    default:
		memset( bmp->buffer, 255, size );
		break;
	}
	FT_Done_Glyph( glyph );
	return size;
}

static int FreeType_Render( LCUI_FontBMP *bmp, wchar_t ch, 
			    int pixel_size, LCUI_FontFace *face )
{
	int error;
	size_t size;
	LCUI_BOOL has_space = FALSE;
	FT_Face ft_face = (FT_Face)face->data;

	/* �趨����ߴ� */
	FT_Set_Pixel_Sizes( ft_face, 0, pixel_size );
	/* ����ǿո� */
	if( ch == ' ' ) {
		ch = 'a';
		has_space = TRUE;
	}
	/* ������ֵ��������� */
	error = FT_Load_Char( ft_face, ch, LCUI_FONT_LOAD_FALGS );
	if(error) {
		return error;
	}
	size = Convert_FTGlyph( bmp, ft_face->glyph, LCUI_FONT_RENDER_MODE );
	/* ����ǿո���λͼ������� */
	if( has_space ) {
		memset( bmp->buffer, 0, size );
	}
	return 0;
}

int LCUIFont_InitFreeType( LCUI_FontEngine *engine )
{
	if( FT_Init_FreeType(&freetype.library) ) {
		return -1;
	}
	strcpy( engine->name, "FreeType" );
	engine->render = FreeType_Render;
	engine->open = FreeType_Open;
	engine->close = FreeType_Close;
	return 0;
}

int LCUIFont_ExitFreeType( void )
{
	FT_Done_FreeType( freetype.library );
	return 0;
}

#endif
