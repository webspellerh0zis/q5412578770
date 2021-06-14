/* ***************************************************************************
 * LCUI_RadioButton.h -- LCUI��s RadioButton widget
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
 * LCUI_RadioButton.h -- LCUI �ĵ�ѡ�򲿼�
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

#ifndef __LCUI_RADIOBTN_H__
#define __LCUI_RADIOBTN_H__
typedef struct _LCUI_RadioButton LCUI_RadioButton;

/*********************** ��ѡ�� **************************/
struct _LCUI_RadioButton
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
	
	LCUI_Queue		*mutex;	/* ��¼����ĵ�ѡ�򲿼���ָ�� */
};
/****************************************************/
void RadioButton_Create_Mutex(LCUI_Widget *a, LCUI_Widget *b);
/* ���ܣ�Ϊ������ѡ���������ϵ */

void RadioButton_Delete_Mutex(LCUI_Widget *widget);
/* ���ܣ�����ѡ��ӻ����ϵ�����Ƴ� */

void Set_RadioButton_On(LCUI_Widget *widget);
/* ���ܣ��趨��ѡ��Ϊѡ��״̬ */ 

void Set_RadioButton_Off(LCUI_Widget *widget);
/* ���ܣ��趨��ѡ��Ϊδѡ��״̬ */ 

int Get_RadioButton_Status(LCUI_Widget *widget);
/* ���ܣ���ȡ��ѡ���״̬ */ 

int RadioButton_Is_On(LCUI_Widget *widget);
/* ���ܣ���ⵥѡ���Ƿ�ѡ�� */ 

int RadioButton_Is_Off(LCUI_Widget *widget);
/* ���ܣ���ⵥѡ���Ƿ�δѡ�� */ 

void Switch_RadioButton_Status(LCUI_Widget *widget, void *arg);
/* 
 * ���ܣ��л���ѡ���״̬
 * ˵�������״̬��ָ���Ǵ���û�򹴵�����״̬
 *  */ 

LCUI_Widget *Get_RadioButton_Label(LCUI_Widget *widget);
/* ���ܣ���ȡ��ѡ�򲿼��е�label������ָ�� */ 

LCUI_Widget *Get_RadioButton_ImgBox(LCUI_Widget *widget);
/* ���ܣ���ȡ��ѡ�򲿼��е�PictureBox������ָ�� */

void Set_RadioButton_Text(LCUI_Widget *widget, const char *fmt, ...);
/* ���ܣ��趨�뵥ѡ�򲿼��������ı����� */ 

LCUI_Widget *Create_RadioButton_With_Text(const char *fmt, ...);
/* ���ܣ�����һ�����ı����ݵĵ�ѡ�� */ 
#endif
