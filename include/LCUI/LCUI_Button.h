/* ***************************************************************************
 * LCUI_Button.h -- LCUI��s Button widget
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
 * LCUI_Button.h -- LCUI �İ�ť����
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

#ifndef __LCUI_BUTTON_H__
#define __LCUI_BUTTON_H__
typedef struct _LCUI_Button LCUI_Button;

/***********************��ť**************************/
struct _LCUI_Button
{
	LCUI_Graph		image;			/* �ڰ�ť����ʾ��ͼ�� */
	
	/* �����ΪCustom_Style(�Զ���)ʱ�������ݰ�ť��״̬ʹ����Щͼ�� */
	LCUI_Graph		btn_normal;		/* ��ͨ״̬ */
	LCUI_Graph		btn_over;		/* ����긲�� */
	LCUI_Graph		btn_down;		/* ����״̬ */
	LCUI_Graph		btn_focus;		/* ����״̬ */
	LCUI_Graph		btn_disable;	/* �����õ�״̬ */
	
	LCUI_Align		image_align;	/* �ڰ�ť����ʾ��ͼ��Ķ��뷽ʽ */
	
	int				text_image_relation; /* ��ť��ͼ�����ı������λ�� */
	
	LCUI_Widget		*label;	/* Ƕ�׵��ı���ǩ */
};
/****************************************************/

LCUI_Widget *Get_Button_Label(LCUI_Widget *widget);
/* ���ܣ���ȡǶ���ڰ�ť�������label���� */ 

void Custom_Button_Style(	LCUI_Widget *widget, LCUI_Graph *normal, 
							LCUI_Graph *over, LCUI_Graph *down, 
							LCUI_Graph *focus, LCUI_Graph *disable);
/* ���ܣ��Զ��尴ť�ڸ���״̬����ʾ��λͼ */ 

void Set_Button_Text(LCUI_Widget *widget, const char *fmt, ...);
/* ���ܣ��趨��ť������ʾ���ı����� */ 

LCUI_Widget *Create_Button_With_Text(const char *fmt, ...);
/* ���ܣ�����һ�����ı����ݵİ�ť */ 
#endif
