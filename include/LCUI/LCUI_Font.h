/* ***************************************************************************
 * LCUI_Font.h -- Font Handling
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
 * LCUI_Font.h -- ���崦��
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
#ifndef __LCUI_FONTS_H__
#define __LCUI_FONTS_H__
void Get_Default_Font_Bitmap(unsigned short code, LCUI_Bitmap *out_bitmap);
/* ���ܣ������ַ����룬��ȡϵͳ�Դ�������λͼ */

unsigned short Get_Unicode(char *in_gb2312);

void Set_Default_Font(char *fontfile);
/* 
 * ���ܣ��趨Ĭ�ϵ������ļ�·��
 * ˵������Ҫ��LCUI��ʼ��ǰʹ�ã���ΪLCUI��ʼ��ʱ���Ĭ�ϵ������ļ�
 *  */ 

void LCUI_Font_Init(LCUI_Font *font);
/* 
 * ���ܣ���ʼ��LCUI��Font�ṹ������ 
 * ˵������������LCUI��ʼ��ʱ���ã�LCUI_Font�ṹ���м�¼��������ص�����
 * */ 

void Font_Init(LCUI_Font *in);
/* 
 * ���ܣ���ʼ��Font�ṹ������
 * ˵����Ĭ���Ǽ̳�ϵͳ����������
 *  */ 

int Show_Font_Bitmap(LCUI_Bitmap *in_fonts);
/* ���ܣ�����Ļ��ӡ��0��1��ʾ����λͼ */ 

int Mix_Fonts_Bitmap(
		LCUI_Graph		*back_graph,  /* ����ͼ�� */
		int				start_x,
		int				start_y,
		LCUI_Bitmap	*in_fonts,  /* ���������λͼ���� */
		LCUI_RGB		color,       /* �������ɫ */ 
		int				flag
);
/* ���ܣ�������λͼ�����뱳��ͼ�λ�� */ 

int Char_To_Wchar_T(char *in_text, wchar_t **unicode_text);
/*
 * ���ܣ���char���ַ���ת����wchar_t�ַ���
 * ����˵����
 * in_text      �������char���ַ���
 * unicode_text �������wchar_t���ַ���
 * ����ֵ��������wchar_t���ַ����ĳ��ȣ����򷵻�-1
 * */ 

int String_To_List(char *text , LCUI_WString **out_list);
/*
 * ���ܣ���ԭʼ���ַ����ָ�ɶ�ά����,�ַ�����ΪUnicode
 * ����˵����
 * in_text  �� �����char���ַ���
 * list     �� ����Ķ�άLCUI_Wchar_Tָ��
 * ����ֵ���ַ���������
 * */ 

int Open_Fontfile(LCUI_Font *font_data, char *fontfile);
/* ���ܣ��������ļ���������������LCUI_Font�ṹ���� */ 

int Get_WChar_Bitmap(LCUI_Font *font_data, wchar_t ch, LCUI_Bitmap *out_bitmap);
/*
 * ���ܣ���ȡ����wchar_t���ַ���λͼ
 * ˵����LCUI_Font�ṹ���д������ѱ��򿪵������ļ������face����ľ������������ļ��Ѿ���
 * �ɹ���һ�Σ��˺��������ٴδ������ļ���
 */ 

int Count_Contents_Size(
	LCUI_WString *contents, 
	int *out_width, int *out_height, 
	int rows, int space, int linegap);
/*
 * ���ܣ������ı�λͼ�ĳߴ�
 * ����˵����
 * contents�� ��Ҫ�����Ŀ��
 * out_width   �� ����Ŀ��
 * out_height  �� ����ĸ߶�
 * rows    :  �ı���������
 * space   :  ÿ����֮��ļ��
 * linegap :  �о�
 * */ 
#endif /* __LCUI_FONTS_H__ */
