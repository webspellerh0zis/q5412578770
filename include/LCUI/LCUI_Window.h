/* ***************************************************************************
 * LCUI_Window.h -- LCUI's window widget
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
 * LCUI_Window.h -- LCUI �Ĵ��ڲ���
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


#ifndef __LCUI_WINDOW_H_
#define __LCUI_WINDOW_H_ 
/* ��ʹ��typedefΪ�ṹ�崴��ͬ���֣�֮���ٶ���ṹ�� */
typedef struct _LCUI_TitleBar		LCUI_TitleBar;
typedef struct _LCUI_Window			LCUI_Window;

/******************* ���ڱ����� ***********************/
struct _LCUI_TitleBar
{
	LCUI_Widget *icon_box;		/* ͼ�� */
	LCUI_Widget *label;		/* ����������ʾ���ı� */
}; /* �洢���ڵı��������ݵĽṹ�� */
/****************************************************/


/************************ �������� **************************/
struct _LCUI_Window
{
	LCUI_Widget *titlebar;		/* ������ */
	LCUI_Widget *client_area;	/* �ͻ��� */
	LCUI_Widget *btn_close;	/* �رհ�ť */
	int hide_style;			/* ��������ʱʹ�õ���Ч */
	int show_style;			/* ������ʾʱʹ�õ���Ч */
	int resize_style;		/* ���ڸı�ߴ�ʱʹ�õ���Ч */
	int count;			/* ��ʾ�������� */
	
	LCUI_Align	init_align;		/* ���ڳ�ʼ��ʾλ�� */
};
/***********************************************************/

LCUI_Widget *Get_Window_TitleBar(LCUI_Widget *window);
/* ���ܣ���ȡ���ڱ�������ָ�� */ 

LCUI_Widget *Get_Window_Client_Area(LCUI_Widget *window);
/* ���ܣ���ȡ���ڿͻ�����ָ�� */ 

void Set_Window_Title_Icon(LCUI_Widget *window, LCUI_Graph *icon);
/* ���ܣ��Զ���ָ�����ڵı�����ͼ�� */ 

LCUI_Size Get_Window_Client_Size(LCUI_Widget *win_p);
/* ���ܣ���ȡ���ڵĿͻ����ĳߴ� */ 

void Window_Widget_Auto_Size(LCUI_Widget *win_p);
/* ���ܣ��ڴ��ڳߴ�ı�ʱ�Զ��ı�������Ϳͻ����ĳߴ� */ 

LCUI_Widget *Get_Parent_Window(LCUI_Widget *widget);
/* ���ܣ���ȡָ���������ڵĴ��� */ 

void Set_Window_Title_Text(LCUI_Widget *win_p, char *text);
/* ���ܣ�Ϊ�������ñ������� */ 

void Window_Client_Area_Add(LCUI_Widget *window, LCUI_Widget *widget);
/* ���ܣ���������������ڿͻ��� */ 

void Window_TitleBar_Add(LCUI_Widget *window, LCUI_Widget *widget);
/* ���ܣ���������������ڱ����� */ 
#endif /* __LCUI_WINDOW_H__ */
