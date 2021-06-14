/* ***************************************************************************
 * LCUI_Label.h -- LCUI's Label widget
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
 * LCUI_Label.h -- LCUI ���ı���ǩ����
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
#ifndef __LCUI_LABEL_H__
#define __LCUI_LABEL_H__
typedef struct _LCUI_Label			LCUI_Label;
/**************************** lable���� ********************************/
struct _LCUI_Label
{
	int				auto_size;           /* �Զ�������С(IS_TURE/IS_FALSE) */

	LCUI_Graph		*image;              /* �ڲ�������ʾ��ͼ�� */
	LCUI_Align		image_align;         /* ��ʾ��ͼ��Ķ��뷽ʽ */ 
	LCUI_Font		font;                /* ������������Ϣ */
	LCUI_String		text;                /* ��ؼ��������ı��������ԭʼ�ַ��� */
	LCUI_WString	*contents;           /* ���ݣ���¼��ÿ���ֵ���Ϣ�Լ�λͼ */
	int				rows;                /* ���ݵ����� */
	LCUI_Align		text_align;          /* �ı��Ķ��뷽ʽ */
};/* ��������ʾ����ʱ����Ϣ����˵�������� */
/******************************* END ************************************/

void Set_Label_Image(LCUI_Widget *widget, LCUI_Graph *img, LCUI_Align align);
/* 
 * ���ܣ�����label�����ı���ͼ��
 * ����˵����
 * widget ����Ҫ���в����Ĳ���
 * img    ����Ҫ��Ϊ����ͼ��ͼ������
 * align  ������ͼ��Ĳ���
 * */

int Get_Label_Row_Len(LCUI_Widget *widget, int row);
/*
 * ���ܣ���ȡlabel������ָ���е��ַ�������
 * ����˵����
 * widget ����Ҫ���в����Ĳ���
 * row   : �ڼ���
 * ����ֵ��ʧ���򷵻�-2���ɹ����س��ȣ����Ͳ�������-1
 * */ 

void Set_Label_Text(LCUI_Widget *widget, const char *fmt, ...);
/* ���ܣ��趨���ǩ�������ı����� */

int Set_Label_Font(LCUI_Widget *widget, int font_size, char *font_file);
/*
 * ���ܣ�Ϊ��ǩ�����趨�����С���������͡�������ɫ
 * ����˵����
 * widget    ����Ҫ���в����Ĳ���
 * font_file �������ļ���λ�ã����Ϊ����·��
 * color     ���������ɫ��Ҳ����������ʾ����ɫ
 * */ 

void Set_Label_Font_Default_Color(LCUI_Widget *widget, LCUI_RGB color);
/*
 * ���ܣ�����label�����������ı���������ɫ��������ɫΪRGB��ɫ�Ļ��ɫ
 * ����˵����
 * label ����Ҫ���в�����label����
 * color : ��ɫ
 * */ 
#endif
