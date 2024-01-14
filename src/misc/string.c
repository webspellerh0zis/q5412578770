/* ***************************************************************************
 * string.c -- The string operation set.
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
 * string.c -- �ַ���������
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
	in->size = 0;
	in->string = NULL;
}

/* ����Դ�ַ�����Ŀ���ַ����� */
LCUI_EXPORT(void)
_LCUIString_Copy( LCUI_String * des, const char *src )
{
	if(des == NULL) {
		return;
	}
	if (src != NULL) {
		if (des->size != 0) {
			free (des->string);
		}
		des->size = sizeof (char) * (strlen (src) + 1);
		des->string = calloc (1, des->size);
		strcpy (des->string, src);
	} else {
		des->size = sizeof (char) * (2);
		des->string = calloc (1, des->size);
	}
}

/* LCUI_String �ַ����Ա� */
LCUI_EXPORT(int)
_LCUIString_Cmp( LCUI_String *str1, const char *str2 )
{
	if (str1 != NULL && str1->size > 0 && str2 != NULL) {
		return strcmp(str1->string, str2); 
	}
	return -1;
}

/* LCUI_String �ַ����Ա� */
LCUI_EXPORT(int)
LCUIString_Cmp( LCUI_String *str1, LCUI_String *str2 )
{
	if( str1->size > 0 && str2->size > 0 ) {
		return strcmp(str1->string, str2->string);
	}
	return 0;
}

/* ����Դ�ַ�����Ŀ���ַ����� */
LCUI_EXPORT(int)
LCUIString_Copy( LCUI_String *str1, LCUI_String *str2 )
{
	if( str2->size <= 0 ) {
		return -1;
	}
	if(str1->size > 0) {
		free( str1->string );
	}
	str1->string = (char*)calloc(str2->size+1, sizeof(char));
	strcpy( str1->string, str2->string );
	str1->size = str2->size;
	return 0;
}

LCUI_EXPORT(void)
LCUIString_Free( LCUI_String *in )
{
	if(in->size > 0) {
		free(in->string); 
	}
	in->string = NULL;
}

/* �ͷſ��ַ�ռ�õ���Դ */
LCUI_EXPORT(void)
LCUIWchar_Free( LCUI_WChar_T *ch )
{
	ch->bitmap = NULL;
}

/* �ͷſ��ַ���ռ�õ���Դ */
LCUI_EXPORT(void)
LCUIWString_Free( LCUI_WString *str )
{
	int i;
	if(!str || str->size <= 0 || !str->string) {
		return;
	}
	for(i = 0; i < str->size; ++i) {
		LCUIWchar_Free(&str->string[i]); 
	}
	free(str->string);
	str->string = NULL;
}
