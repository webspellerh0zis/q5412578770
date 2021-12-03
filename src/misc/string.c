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

void String_Init(LCUI_String *in)
/* ���ܣ���ʼ��String�ṹ���е����� */
{
	in->size = 0;
	in->string = NULL;
}

void Strcpy (LCUI_String * des, const char *src)
/* ���ܣ������ַ�����String�ṹ�������� */
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

int Strcmp(LCUI_String *str1, const char *str2)
/* ���ܣ��Ա�str1��str2 */
{
	if (str1 != NULL && str1->size > 0 && str2 != NULL) 
		return strcmp(str1->string, str2); 
	else return -1;
}

int LCUI_Strcmp(LCUI_String *str1, LCUI_String *str2)
/* LCUI_String �ַ����Ա� */
{
	if( str1->size > 0 && str2->size > 0 ) {
		return strcmp(str1->string, str2->string);
	}
	return 0;
}

int LCUI_Strcpy(LCUI_String *str1, LCUI_String *str2)
/* LCUI_String �ַ������� */
{
	if( str1->size <=0 || str2->size <= 0 ) {
		return -1;
	}
	if(str1->size > 0) {
		if( str2->size > str1->size ) { 
			free( str1->string );
			str1->string = (char*)calloc(str2->size+1, sizeof(char));
		}
	}
	strcpy( str1->string, str2->string );
	str1->size = str2->size;
	return 0;
}

void String_Free(LCUI_String *in) 
{
	if(in->size > 0) {
		free(in->string); 
	}
	in->string = NULL;
}

void WChar_T_Free(LCUI_WChar_T *ch) 
{
	FontBMP_Free(&ch->bitmap);
}

void WString_Free(LCUI_WString *str) 
{
	int i;
	if(str != NULL) {
		if(str->size > 0 && str->string != NULL) {
			for(i = 0; i < str->size; ++i) {
				WChar_T_Free(&str->string[i]); 
			}
			free(str->string);
			str->string = NULL;
		}
	}
}
