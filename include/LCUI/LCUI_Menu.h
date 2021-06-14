/* ***************************************************************************
 * LCUI_Menu.h -- LCUI's Menu widget��it is not yet perfect
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
 * LCUI_Menu.h -- LCUI �Ĳ˵�����,��δ����
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

/* 
 * ˵����
 * ��Ҫ�Ľ��ĵط��ܶ࣬������ָ���뿪�˵����˵��е�ѡ����Ҳ����ʧ���ȵ��ò���֧�ֽ�
 * ��״̬��ʱ�򣬲˵��д��ڽ���״̬�Ĳ����������ʾ����ʹ���ָ���ƿ��˵����˵��еĽ���
 * �����ǲ����ģ�����״̬���Ǳ����ŵġ�
 * */
 
#ifndef __LCUI_MENU_H__
#define __LCUI_MENU_H__

typedef struct _LCUI_Menu	LCUI_Menu;

struct _LCUI_Menu
{
	int mini_width;		/* �˵�����Ϳ�� */
	LCUI_Widget *widget_link; /* ��¼��ò˵����ӵĲ��� */
	LCUI_Widget *parent_menu;	/* ��¼�����˵�����ָ�� */
	LCUI_Queue child_menu;/* ��¼�Ӳ˵����� */
};


void Show_Menu(LCUI_Widget *src, LCUI_Widget *des_menu);
/* 
 * ���ܣ����˵���ʾ�ڲ���������
 * ˵�������������˵��Ĳ����Լ��˵�����ָ�룬�����ڸò�������ʾ�˵�
 */
 
void Side_Show_Menu(LCUI_Widget *src, void *arg);
/* 
 * ���ܣ����˵���ʾ�ڲ����Ĳ��
 * ˵�������������˵��Ĳ����Լ��˵�����ָ�룬�����ڸò����ұ���ʾ�˵�
 */
 

int Show_Child_Menu(LCUI_Widget *item);
/*
 * ���ܣ���ʾ��ָ���˵���������Ӳ˵�
 * */

void Auto_Hide_Menu(LCUI_MouseEvent *event);
/*
 * ���ܣ���������¼����Զ����ز˵�
 * ˵����������������¼�ʱ�����Զ��ж��Ƿ���Ҫ���ز˵�
 */
 
LCUI_Widget *Create_Child_Menu(LCUI_Widget *parent_menu, LCUI_Widget *item);
/*
 * ���ܣ�Ϊ�˵������Ӳ˵�
 * ˵�����Ӳ˵������ĸ��˵����item����
 **/

LCUI_Widget *Create_Menu_Item(LCUI_Widget *menu, const char *fmt, ...);
/*
 * ���ܣ�Ϊ�˵����ѡ��
 * ˵����Ҳ�����Բ˵�Ϊ��������������ӽ�ȥ�������Ϊ�˵��������ʾ�ı�
 */

void Hide_Menu(LCUI_Widget *menu);
/*
 * ���ܣ�����ָ���˵��Լ���������ʾ���Ӳ˵�
 * ˵���������ز˵���ͬʱ������Ըò˵��������Ӳ˵���������
 * */
#endif
