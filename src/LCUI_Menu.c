/* ***************************************************************************
 * LCUI_Menu.c -- LCUI's Menu widget
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
 * LCUI_Menu.c -- LCUI �Ĳ˵�����
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
 
#include <LCUI_Build.h>

#include LC_LCUI_H 
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_WIDGET_H
#include LC_BUTTON_H 
#include LC_MENU_H
#include LC_RES_H
#include LC_MEM_H

static void Menu_Init(LCUI_Widget *widget)
/* ���ܣ���ʼ���˵������ݽṹ�� */
{
	/* ��ȡ˽�нṹ��ָ�룬�������ڴ� */
	LCUI_Menu *menu = Malloc_Widget_Private(widget, sizeof(LCUI_Menu));
	
	menu->parent_menu = NULL;
	menu->widget_link = NULL;
	menu->mini_width  = 50;
	WidgetQueue_Init(&menu->child_menu);
	Queue_Using_Pointer(&menu->child_menu);
	/* ���ñ߿�������Ϊ�����߿� */
	Set_Widget_Border_Style(widget, BORDER_STYLE_LINE_BORDER);
	Set_Widget_Border(widget, RGB(50,50,50), Border(1,1,1,1));
	Set_Widget_BG_Mode(widget, BG_MODE_FILL_BACKCOLOR);
	LCUI_MouseEvent_Connect(Auto_Hide_Menu, MOUSE_EVENT_MOVE);
}

static void Exec_Update_Menu(LCUI_Widget *widget)
/* ���ܣ����²˵���ͼ������ */
{
	LCUI_Widget *item;
	LCUI_Menu *menu;
	int i, max_width = 0, total, max_height = 0;
	
	total = Queue_Get_Total(&widget->child);
	for(i=total-1; i>=0; --i)
	{   /* ����˳�����в��� */
		item = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if(item->visible == IS_TRUE)
		{/* ����ò����ɼ� */
			/* �趨������λ�� */ 
			Move_Widget(item, Pos(0+widget->border.left, max_height+widget->border.top)); 
			/* �ó������ */
			max_width = (max_width < item->size.w ? item->size.w : max_width);
			/* �ۼ��ܸ߶� */
			max_height += item->size.h;
		}
	}
	menu = Get_Widget_Private_Data(widget);
	/* �����ǰ����ȵ����趨����Ϳ�� */
	if(max_width < menu->mini_width) 
		max_width = menu->mini_width;
	/* �ó�����Ⱥ󣬸ı�������Ϊѡ��Ĳ����ĳߴ� */
	for(i=total-1; i>=0; --i)
	{   /* ����˳�����в��� */
		item = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if(item == NULL) return;
		/* �ı�ߴ� */
		Resize_Widget(item, Size(max_width, item->size.h));
	}
	
	int w, h;
	w = widget->border.left + widget->border.right;
	h = widget->border.top + widget->border.bottom;
	/* ���Ĵ��ڵĳߴ磬����Ӧ�����ĳߴ� */ 
	Resize_Widget(widget, Size(max_width+w, max_height+h));
	Refresh_Widget(widget); /* ÿ�θ��£�����Ҫˢ������������ͼ����ʾ */
}

void Show_Menu(LCUI_Widget *src, LCUI_Widget *des_menu)
/* 
 * ���ܣ����˵���ʾ�ڲ���������
 * ˵�������������˵��Ĳ����Լ��˵�����ָ�룬�����ڸò�������ʾ�˵������
 * ���ñ�����ʱ��Ŀ��˵�����ʾ״̬����ô�������������
 */
{
	if(des_menu->visible == IS_TRUE)
	{/* ����˵�����ʾ״̬������֮��������أ��Ὣ��������ʾ���Ӳ˵����� */
		Hide_Menu(des_menu);
		return;
	}
	int height, width;
	LCUI_Pos src_pos, display_pos;  
	LCUI_Menu *menu = Get_Widget_Private_Data(des_menu);
	height = Get_Screen_Height();
	width = Get_Screen_Width();
	
	/* ͨ��������ȫ�������Լ��ߴ�,�ó��˵�����ʾλ�� */
	src_pos = Get_Widget_Global_Pos(src);
	display_pos.x = src_pos.x;
	display_pos.y = src_pos.y + src->size.h;
	
	/* ����˵�����ʾλ�ó�����Ļ��ʾ��Χ,�ͶԲ˵���λ�ý�����Ӧ���� */
	if(display_pos.x + des_menu->size.w > width) 
		display_pos.x = width - des_menu->size.w;
		
	if(display_pos.x < 0) 
		display_pos.x = 0;
		
	if(display_pos.y + des_menu->size.h > height) 
		display_pos.y = src_pos.y - des_menu->size.h;
		
	if(display_pos.y < 0) 
		display_pos.y = 0;
	
	menu->widget_link = src;/* ������֮�����Ĳ��� */
	Move_Widget(des_menu, display_pos);
	Show_Widget(des_menu);
}

void Side_Show_Menu(LCUI_Widget *src, void *arg)
/* 
 * ���ܣ����˵���ʾ�ڲ����Ĳ��
 * ˵�������������˵��Ĳ����Լ��˵�����ָ�룬�����ڸò����ұ���ʾ�˵�
 */
{
	LCUI_Widget *des_menu = (LCUI_Widget*)arg;
	if(des_menu == NULL) return;
	if(des_menu->visible == IS_TRUE)
	{/* ����˵�����ʾ״̬������֮��������أ��Ὣ��������ʾ���Ӳ˵����� */
		Hide_Menu(des_menu);
		return;
	}
	int height, width;
	LCUI_Pos src_pos, display_pos;  
	LCUI_Menu *menu = Get_Widget_Private_Data(des_menu);
	
	height = Get_Screen_Height();
	width = Get_Screen_Width();
	
	/* ͨ��������ȫ�������Լ��ߴ�,�ó��˵�����ʾλ�� */
	src_pos = Get_Widget_Global_Pos(src);
	display_pos.x = src_pos.x + src->size.w;
	display_pos.y = src_pos.y;
	
	/* ����˵�����ʾλ�ó�����Ļ��ʾ��Χ,�ͶԲ˵���λ�ý�����Ӧ���� */
	if(display_pos.x + des_menu->size.w > width) 
		display_pos.x = width - des_menu->size.w;
		
	if(display_pos.x < 0) 
		display_pos.x = 0;
		
	if(display_pos.y + des_menu->size.h > height) 
		display_pos.y = src_pos.y - des_menu->size.h;
		
	if(display_pos.y < 0) 
		display_pos.y = 0;
		
	menu->widget_link = src;/* ������֮�����Ĳ��� */
	Move_Widget(des_menu, display_pos);
	Show_Widget(des_menu);  
}


void Hide_Menu(LCUI_Widget *menu)
/*
 * ���ܣ�����ָ���˵��Լ���������ʾ���Ӳ˵�
 * ˵���������ز˵���ͬʱ������Ըò˵��������Ӳ˵���������
 * */
{
	if(menu == NULL) return;
	int i;
	LCUI_Menu *data = Get_Widget_Private_Data(menu);
	LCUI_Widget *widget;
	for(i=0; ;i++)
	{/* ��ȡ�Ӳ˵������еĲ���ָ�� */
		widget = (LCUI_Widget*)Queue_Get(&data->child_menu, i); 
		if(widget == NULL) break;/* �����ȡʧ�ܣ��˳�ѭ�� */
		Hide_Menu(widget);/* �ݹ���� */
	}
	Hide_Widget(menu);
}


LCUI_Widget *Get_Child_Menu(LCUI_Widget *des_menu, LCUI_Widget *item)
/* ���ܣ���ȡ�˵���ָ��ѡ����������Ӳ˵���ָ�� */
{
	int i;
	LCUI_Widget *widget;
	LCUI_Menu *menu = (LCUI_Menu*)Get_Widget_Private_Data(des_menu);
	LCUI_Menu *tmp_menu;
	for(i=0; ; ++i)
	{
		widget = (LCUI_Widget*)Queue_Get(&menu->child_menu, i);
		tmp_menu = (LCUI_Menu*)Get_Widget_Private_Data(widget);
		if(tmp_menu->widget_link == item)
		{/* �������˵���item���� */
			return widget;
		}
	}
	return NULL;
}

void Hide_Other_Menu(LCUI_Widget *des_menu, LCUI_Widget *item)
/* ���ܣ����س��˵��е�ѡ�����������ѡ�����������Ӳ˵� */
{
	if(des_menu == NULL) return;
	int i;
	LCUI_Widget *widget;
	LCUI_Menu *menu = (LCUI_Menu*)Get_Widget_Private_Data(des_menu);
	LCUI_Menu *tmp_menu;
	for(i=0; ; ++i)
	{
		/* ��ȡ�Ӳ˵�ָ�� */
		widget = (LCUI_Widget*)Queue_Get(&menu->child_menu, i); 
		if(widget == NULL) break;
		tmp_menu = (LCUI_Menu*)Get_Widget_Private_Data(widget);
		if(tmp_menu->widget_link != item)
		{/* �������˵����Ǻ�item������ */
			/* ����������������Ĳ˵� */
			Hide_Menu(widget);
		}
	}
}

static LCUI_Widget *save_menu = NULL;
void Auto_Hide_Menu(LCUI_MouseEvent *event)
/*
 * ���ܣ���������¼����Զ����ز˵�
 * ˵����������������¼�ʱ�����Զ��ж��Ƿ���Ҫ���ز˵�
 */
{
	LCUI_Widget *menu;
	if(event->widget != NULL)
	{/* �����ǰ���ָ���ڲ����� */
		/* ��ȡ�ò���������Ϊ�˵��ĸ���������Ϊ��ʱ������Ƕ�����͵Ĳ��� */
		menu = Get_Parent_Widget(event->widget, "menu");
		if(menu != NULL)
		{/* ����ò����ǲ˵���� */
			/* ��ʾ�뵱ǰ�����������Ӳ˵� */
			Show_Child_Menu(event->widget);
		}
		/* ���������˵�����Ӳ˵� */
		Hide_Other_Menu(menu, event->widget);
		save_menu = menu;
	}
}

int Show_Child_Menu(LCUI_Widget *item)
/* ���ܣ���ʾ��ָ���˵���������Ӳ˵� */
{
	/* ����ò˵���û�и��������߸��������ǲ˵���ֱ���˳����� */
	if(item->parent == NULL) return 0;
	if(Strcmp(&item->parent->type, "menu") != 0) 
	{
		LCUI_Widget *t;
		t = item;
		while(t->parent != NULL)
		{
			if(Strcmp(&t->parent->type, "menu") == 0)
			{
				item = t;
				break;
			}
			t = t->parent;
		}
		/* ���ж�һ�Σ�������������ǲ˵������˳����� */
		if(Strcmp(&item->parent->type, "menu") != 0) 
			return 0;
	}
	
	LCUI_Widget *child_menu;
	LCUI_Menu *menu, *tmp;
	int i;
	menu = (LCUI_Menu*)Get_Widget_Private_Data(item->parent); 
	for(i=0; ; i++)
	{/* ��ȡ�����Ӳ˵���ָ�� */
		child_menu = (LCUI_Widget*)Queue_Get(&menu->child_menu, i);
		if(child_menu == NULL) 
			break;
		else
		{
			/* ��ȡ�Ӳ˵���˽�����ݽṹ���ָ�� */
			tmp = (LCUI_Menu*)Get_Widget_Private_Data(child_menu); 
			if(item == tmp->widget_link)
			{/* �������˵��������Ӳ˵����ӵ��Ǹ����� */
				Side_Show_Menu(item, child_menu);/* ��ʾ����������Ĳ�� */
				break;
			}
		}
	}
	return 1;
}

LCUI_Widget *Create_Child_Menu(LCUI_Widget *parent_menu, LCUI_Widget *item)
/*
 * ���ܣ�Ϊ�˵������Ӳ˵�
 * ˵�����Ӳ˵������ĸ��˵����item����
 **/
{
	LCUI_Widget *child_menu;
	LCUI_Menu *menu, *pa_menu;
	child_menu = Create_Widget("menu");
	pa_menu = (LCUI_Menu*)Get_Widget_Private_Data(parent_menu);
	menu = (LCUI_Menu*)Get_Widget_Private_Data(child_menu);
	menu->parent_menu = parent_menu;/* ��¼�����˵� */
	menu->widget_link = item; /* ��¼���Ӳ˵��������� */
	Queue_Add(&pa_menu->child_menu, child_menu);/* ��������˵����Ӳ˵����� */
	Widget_Clicked_Event_Connect(item, Side_Show_Menu, child_menu);
	return child_menu; /* ���������Ӳ˵�������ָ�뷵�ظ���һ������ */
}

LCUI_Widget *Create_Menu_Item(LCUI_Widget *menu, const char *fmt, ...)
/*
 * ���ܣ�Ϊ�˵����ѡ��
 * ˵����Ҳ�����Բ˵�Ϊ��������������ӽ�ȥ�������Ϊ�˵��������ʾ�ı�
 */
{
	char text[LABEL_TEXT_MAX_SIZE];
    memset(text, 0, sizeof(text)); 
    
    va_list ap; 
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap); 
	
	/* �˵����е�ѡ����ð�ť��ʵ�� */
	LCUI_Widget *item = Create_Button_With_Text(text);
	if(item == NULL) return NULL;
	Set_Widget_Style(item, "menu_style");
	Widget_Container_Add(menu, item); /* ��item�������� */
	Show_Widget(item); /* ��ʾ֮ */
	return item;
}


void Register_Menu()
/* ���ܣ�ע�Ჿ������-�˵��������� */
{
	/* ��Ӽ����������� */
	WidgetType_Add("menu");
	
	/* Ϊ�������͹�����غ��� */
	WidgetFunc_Add("menu",	Menu_Init,			FUNC_TYPE_INIT);
	WidgetFunc_Add("menu",	Exec_Update_Menu,	FUNC_TYPE_UPDATE);
}
