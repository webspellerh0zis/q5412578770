/* ***************************************************************************
 * in_core_font.c -- basic in-core font-engine.
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
 * in_core_font.c -- �����������������棬�����ڴӳ����ڲ���������λͼ
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
#include <LCUI/LCUI.h>
#include <LCUI/font.h>

enum in_core_font_type {
	FONT_INCONSOLATA
};

static int InCoreFont_Open( const char *filepath, LCUI_FontFace **out_face )
{
	int *code;
	LCUI_FontFace *my_face;

	code = (int*)malloc( sizeof( int ) );
	if( strcmp( filepath, "in-core.inconsolata" ) == 0 ) {
		my_face = (LCUI_FontFace*)malloc(sizeof(LCUI_FontFace));
		*code = FONT_INCONSOLATA;
		my_face->data = code;
		strcpy( my_face->family_name, "inconsolata" );
		strcpy( my_face->style_name, "regular" );
		*out_face = my_face;
		return 0;
	}
	return -1;
}

static void InCoreFont_Close( void *face )
{
	free(face);
}

static int InCoreFont_Render( LCUI_FontBitmap *bmp, wchar_t ch, 
			      int pixel_size, LCUI_FontFace *face )
{
	int *code = (int*)face->data;
	switch( *code ) {
	case FONT_INCONSOLATA:
	default:
		return FontInconsolata_GetBitmap( bmp, ch, pixel_size );
	}
	return -1;
}

int LCUIFont_InitInCoreFont( LCUI_FontEngine *engine )
{
	int *code;
	LCUI_FontFace* face;
	
	strcpy( engine->name, "in-core" );
	/* ����� LCUI �����������Ϣ */
	face = (LCUI_FontFace*)malloc(sizeof(LCUI_FontFace));
	strcpy( face->style_name, "regular" );
	strcpy( face->family_name, "inconsolata" );
	code = (int*)malloc(sizeof(int));
	*code = FONT_INCONSOLATA;
	face->data = code;
	FontLIB_AddFontInfo( face, "in-core.inconsolata" );

	engine->render = InCoreFont_Render;
	engine->open = InCoreFont_Open;
	engine->close = InCoreFont_Close;
	return 0;
}

int LCUIFont_ExitInCoreFont( void )
{
	return 0;
}
