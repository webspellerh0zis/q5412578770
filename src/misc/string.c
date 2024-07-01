/* ***************************************************************************
 * string.c -- The string operation set.
 * 
 * Copyright (C) 2013 by
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
 * string.c -- �ַ���������
 *
 * ��Ȩ���� (C) 2013 ������
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
#include LC_FONT_H

/* ����ĸת���ɴ�д */
static char uppercase( char ch )
{
	if( 'a' <= ch && ch <= 'z' ) {
		return ch - 32;
	}
	return ch;
}

/* �����ִ�Сд���Ա������ַ��� */
LCUI_EXPORT(int)
lcui_strcasecmp( const char *str1, const char *str2 )
{
	const char *p1, *p2;
	for(p1=str1, p2=str2; *p1!=0 || *p2!=0; ++p1, ++p2) {
		if(uppercase(*p1) == uppercase(*p2)) {
			continue;
		}
		return *p1-*p2;
	}
	if( *p1 != 0 ) {
		return 1;
	} else if( *p2 != 0 ) {
		return -1;
	}
	return 0;
}

/* ��ʼ���ַ��� */
LCUI_EXPORT(void)
LCUIString_Init( LCUI_String *in )
{
	in->length = 0;
	in->string = NULL;
}

/* ��ʼ�����ַ��� */
LCUI_EXPORT(void)
LCUIWString_Init( LCUI_WString *in )
{
	in->length = 0;
	in->string = NULL;
}

/* ����Դ�ַ�����Ŀ���ַ����� */
LCUI_EXPORT(void)
_LCUIString_Copy( LCUI_String * des, const char *src )
{
	if(des == NULL || src == NULL) {
		return;
	}
	if (des->length != 0) {
		free (des->string);
	}
	des->length = strlen(src);
	des->string = calloc(des->length+1, sizeof(char));
	strcpy (des->string, src);
}

/* ���ַ������� */
LCUI_EXPORT(void)
_LCUIWString_Copy( LCUI_WString *des, const wchar_t *src )
{
	if(des == NULL || src == NULL) {
		return;
	}
	if (des->length != 0) {
		free (des->string);
	}
	des->length = wcslen(src);
	des->string = calloc(des->length+1, sizeof(wchar_t));
	wcscpy( des->string, src );
}

LCUI_EXPORT(void)
LCUIWString_Copy( LCUI_WString *des_str, LCUI_WString *src_str )
{
	_LCUIWString_Copy( des_str, src_str->string );
}

/* �ַ����Ա� */
LCUI_EXPORT(int)
_LCUIString_Cmp( LCUI_String *str1, const char *str2 )
{
	if( str1 && str1->length > 0 && str2 ) {
		return strcmp(str1->string, str2); 
	}
	return -1;
}

LCUI_EXPORT(int)
LCUIString_Cmp( LCUI_String *str1, LCUI_String *str2 )
{
	if( !str2 ) {
		return -1;
	}
	return _LCUIString_Cmp( str1, str2->string );
}

/* ���ַ����Ա� */
LCUI_EXPORT(int)
_LCUIWString_Cmp( LCUI_WString *str1, const wchar_t *str2 )
{
	if( str1->length > 0 && str2 ) {
		return wcscmp( str1->string, str2 );
	}
	return 0;
}

LCUI_EXPORT(int)
LCUIWString_Cmp( LCUI_WString *str1, LCUI_WString *str2 )
{
	if( !str2 ) {
		return -1;
	}
	return _LCUIWString_Cmp( str1, str2->string );
}

/* �ַ������� */
LCUI_EXPORT(int)
LCUIString_Copy( LCUI_String *str1, LCUI_String *str2 )
{
	if( str2->length <= 0 ) {
		return -1;
	}
	if(str1->length > 0) {
		free( str1->string );
	}
	str1->string = (char*)calloc(str2->length+1, sizeof(char));
	strcpy( str1->string, str2->string );
	str1->length = str2->length;
	return 0;
}

LCUI_EXPORT(void)
LCUIString_Free( LCUI_String *in )
{
	if(in->length > 0) {
		free(in->string); 
	}
	in->string = NULL;
}

/* �ͷſ��ַ�ռ�õ���Դ */
LCUI_EXPORT(void)
LCUIWchar_Free( LCUI_WChar *ch )
{
	ch->bitmap = NULL;
}

/* �ͷſ��ַ���ռ�õ���Դ */
LCUI_EXPORT(void)
LCUIWString_Free( LCUI_WString *str )
{
	if(!str || str->length <= 0 || !str->string) {
		return;
	}
	free(str->string);
	str->string = NULL;
}
