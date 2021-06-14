/* ***************************************************************************
 * LCUI_CheckBox.h -- LCUI's CheckBox widget
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
 * LCUI_CheckBox.h -- LCUI �ĸ�ѡ�򲿼�
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

#ifndef __LCUI_CHECKBOX_H__
#define __LCUI_CHECKBOX_H__
typedef struct _LCUI_CheckBox LCUI_CheckBox;

/*********************** ��ѡ�� **************************/
struct _LCUI_CheckBox
{
	int	on; /* һ����־��ָ���Ƿ�ѡ�� */
	/* �����ΪCustom_Style(�Զ���)ʱ�������ݰ�ť��״̬ʹ����Щͼ�� */
	/* δ�򹴵�״̬ */
	LCUI_Graph		img_off_normal;		/* ��ͨ״̬ */
	LCUI_Graph		img_off_over;		/* ����긲�� */
	LCUI_Graph		img_off_down;		/* ����״̬ */
	LCUI_Graph		img_off_focus;		/* ����״̬ */
	LCUI_Graph		img_off_disable;	/* �����õ�״̬ */
	/* ���Ϲ���״̬ */
	LCUI_Graph		img_on_normal;		/* ��ͨ״̬ */
	LCUI_Graph		img_on_over;		/* ����긲�� */
	LCUI_Graph		img_on_down;		/* ����״̬ */
	LCUI_Graph		img_on_focus;		/* ����״̬ */
	LCUI_Graph		img_on_disable;		/* �����õ�״̬ */
	
	LCUI_Widget	*label;	/* Ƕ�׵��ı���ǩ */
	LCUI_Widget	*imgbox;/* ������ʾͼ�� */
};
/****************************************************/

int Get_CheckBox_Status(LCUI_Widget *widget);
/* ���ܣ���ȡ��ѡ���״̬ */ 

int CheckBox_Is_On(LCUI_Widget *widget);
/* ���ܣ���⸴ѡ���Ƿ�ѡ�� */ 

int CheckBox_Is_Off(LCUI_Widget *widget);
/* ���ܣ���⸴ѡ���Ƿ�δѡ�� */ 

void CheckBox_Set_ImgBox_Size(LCUI_Widget *widget, LCUI_Size size);
/* ���ܣ��趨��ѡ���е�ͼ���ĳߴ� */ 

LCUI_Widget *Get_CheckBox_Label(LCUI_Widget *widget);
/* ���ܣ���ȡ��ѡ�򲿼��е�label������ָ�� */ 

LCUI_Widget *Get_CheckBox_ImgBox(LCUI_Widget *widget);
/* ���ܣ���ȡ��ѡ�򲿼��е�PictureBox������ָ�� */ 

void Set_CheckBox_Text(LCUI_Widget *widget, const char *fmt, ...);
/* ���ܣ��趨�븴ѡ�򲿼��������ı����� */ 

LCUI_Widget *Create_CheckBox_With_Text(const char *fmt, ...);
/* ���ܣ�����һ�����ı����ݵĸ�ѡ�� */ 

#endif
