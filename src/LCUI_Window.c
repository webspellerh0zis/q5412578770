/* ***************************************************************************
 * LCUI_Window.c -- LCUI's window widget
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
 * LCUI_Window.c -- LCUI �Ĵ��ڲ���
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
#include LC_WIDGET_H
#include LC_BUTTON_H
#include LC_MEM_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_WINDOW_H
#include LC_MISC_H 
#include LC_GRAPHICS_H
#include LC_RES_H
#include LC_INPUT_H
#include LC_FONT_H
#include LC_ERROR_H

LCUI_Widget *Get_Window_TitleBar(LCUI_Widget *window)
/* ���ܣ���ȡ���ڱ�������ָ�� */
{ 
	LCUI_Window *win_p = (LCUI_Window *)window->private; 
	return win_p->titlebar;
}

LCUI_Widget *Get_Window_Client_Area(LCUI_Widget *window)
/* ���ܣ���ȡ���ڿͻ�����ָ�� */
{
	LCUI_Window *win_p = (LCUI_Window *)window->private;
	return win_p->client_area;	
}

static void Move_Window(LCUI_Widget *titlebar, LCUI_DragEvent *event)
/* ���ܣ���������ƶ��¼� */
{
	LCUI_Pos pos;
	LCUI_Widget *window; 
	if(event->first_click == 0 )
	{
		window = titlebar->parent;
		pos = event->new_pos;
		if(window != NULL)
		{
			/* ��ȥ�ڴ����е��������, �ó�����λ�� */
			pos = Pos_Sub(pos, Get_Widget_Pos(titlebar));
			/* �ƶ����ڵ�λ�� */
			Move_Widget(window, pos);
		}
	}
}

void Set_Window_Title_Icon(LCUI_Widget *window, LCUI_Graph *icon)
/* ���ܣ��Զ���ָ�����ڵı�����ͼ�� */
{
	LCUI_Graph *image;
	LCUI_Widget *title_widget = Get_Window_TitleBar(window);
	LCUI_TitleBar *title_data = (LCUI_TitleBar *)
							Get_Widget_Private_Data(title_widget);
	image = Get_PictureBox_Graph(title_data->icon_box);
	Free_Graph(image);/* �ͷ��ڴ� */
	if(icon != NULL)
	{
		Set_PictureBox_Image_From_Graph(title_data->icon_box, icon);
		Set_Widget_Align(title_data->icon_box, ALIGN_MIDDLE_LEFT, Pos(3,0));
		Set_Widget_Align(title_data->label, ALIGN_MIDDLE_LEFT, Pos(23,0));
	}
}

static void Window_TitleBar_Init(LCUI_Widget *titlebar)
/* ���ܣ���ʼ�����ڱ����� */
{
	LCUI_Graph img;
	LCUI_TitleBar *t;
	Graph_Init(&img);
	t = Malloc_Widget_Private(titlebar, sizeof(LCUI_TitleBar));
	t->icon_box = Create_Widget("picture_box");
	t->label = Create_Widget("label");
	Widget_Container_Add(titlebar, t->icon_box);
	Widget_Container_Add(titlebar, t->label);
	Resize_Widget(t->icon_box, Size(18,18));
	Set_PictureBox_Size_Mode(t->icon_box, SIZE_MODE_CENTER);
	Show_Widget(t->icon_box);
	Show_Widget(t->label);
	Set_Widget_Align(t->icon_box, ALIGN_MIDDLE_LEFT, Pos(0,0));
	Set_Widget_Align(t->label, ALIGN_MIDDLE_LEFT, Pos(0,0));
	Load_Graph_Default_TitleBar_BG(&img);
	Set_Widget_Background_Image(titlebar, &img, LAYOUT_STRETCH);
}

LCUI_Size Get_Window_Client_Size(LCUI_Widget *win_p)
/* ���ܣ���ȡ���ڵĿͻ����ĳߴ� */
{
	LCUI_Widget *client_area	= Get_Window_Client_Area(win_p);
	return client_area->size;
}

void Window_Widget_Auto_Size(LCUI_Widget *win_p)
/* ���ܣ��ڴ��ڳߴ�ı�ʱ�Զ��ı�������Ϳͻ����ĳߴ� */
{
	int x, y, width, height;
	LCUI_Widget *titlebar		= Get_Window_TitleBar(win_p);
	LCUI_Widget *client_area	= Get_Window_Client_Area(win_p);
	/* ����ͬ�ķ�������� */
	switch(Get_Widget_Border_Style(win_p))
	{
		case BORDER_STYLE_NONE:  /* û�б߿� */
			/* �ȼ�������ͳߴ� */
			x = win_p->border.left;
			y = win_p->border.top;
			width = win_p->size.w - x - win_p->border.right;
			height = win_p->size.h - y - win_p->border.bottom;
			
			Move_Widget( client_area, Pos(x, y) );/* ����λ�� */
			Resize_Widget( client_area, Size(width, height) );/* ������С */
			Hide_Widget( titlebar );/* ���ر����� */
			Show_Widget( client_area );/* �ͻ�����Ҫ��ʾ */
			break;
			
		case BORDER_STYLE_LINE_BORDER: /* �����߿� */
			Move_Widget( client_area, Pos(0, 0) );
			Resize_Widget( client_area, Size(win_p->size.w, win_p->size.h) );
			Hide_Widget( titlebar);
			Show_Widget( client_area);
			break;
			
		case BORDER_STYLE_STANDARD: /* ��׼�߿� */
		/* 
		 * ˵���������û����ĳߴ��λ�õĵ�������Ҫȷ���������ĳߴ磬��ˣ�
		 * ����Exec_Resize_Widget()�������̵����������ߴ磬����ǵ���
		 * Resize_Widget()��������ô��ȷ���ĳߴ��������Ϊ�������ĳ�
		 * �绹����ǰ�ĳߴ硣
		 * */ 
			Set_Widget_Border(win_p, RGB(50,50,50), Border(1,1,1,1));
			/* �ȼ�������ͳߴ� */
			x = win_p->border.left;
			y = win_p->border.top;
			width = win_p->size.w - x - win_p->border.right;
			height = win_p->size.h - y - win_p->border.bottom;
			
			Move_Widget(titlebar, Pos(x, y) );
			Exec_Resize_Widget(titlebar, Size(width, 25));
			
			Move_Widget(client_area, Pos(x, y + titlebar->size.h));
			Resize_Widget(client_area, Size(width, height - titlebar->size.h));
			/* �������Ϳͻ�������Ҫ��ʾ */
			Show_Widget(titlebar); 
			Show_Widget(client_area); 
			break;
			
		default:
			//
			break;
	} 
}

static void Exec_Update_Window(LCUI_Widget *win_p)
/* ���ܣ����´���ͼ������ */
{
	LCUI_Widget *titlebar = Get_Window_TitleBar(win_p);
	LCUI_Widget *client_area = Get_Window_Client_Area(win_p);
	/* ���Ĵ��ڱ߿���ʱ�����������һ�����ã���ΪҪ���ݷ�����������ڵ����� */
	Window_Widget_Auto_Size(win_p);
	/* ��������Ĳ��� */
	Draw_Widget(titlebar);
	Draw_Widget(client_area);
}

LCUI_Widget *Get_Parent_Window(LCUI_Widget *widget)
/* ���ܣ���ȡָ���������ڵĴ��� */
{
	if(widget == NULL) return NULL;
	if(widget->parent == NULL) return NULL;
	if(strcmp(widget->parent->type.string, "window") == 0)
		return widget->parent;
	
	return Get_Parent_Window(widget->parent);
}


static void Quit_Parent_Window(LCUI_Widget *btn, void *arg)
/* ���ܣ��˳�����btn���ڵĴ��� */
{
	//printf("Quit_Parent_Window start\n");
	Main_Loop_Quit();
	//printf("Quit_Parent_Window end\n");
	//return;
	//LCUI_Widget *win_p = Get_Parent_Window(btn);
	//if(win_p == NULL) 
	//	puts(QUIT_PARENT_WINDOW_ERROR);
	//Delete_Widget(win_p);
}

static void Destroy_Window(LCUI_Widget *win_p)
/*
 * ���ܣ��ͷ�window����ռ�õ��ڴ���Դ
 * ˵������������������
 **/
{
	//����û��ָ�����������ڴ棬��˲���Ҫ�ͷ�ָ�����
}

static void Window_Init(LCUI_Widget *win_p)
/*
 * ���ܣ���ʼ������
 * ˵���������ڹ��캯��
 **/
{
	LCUI_Widget *titlebar;
	LCUI_Widget *client_area;
	LCUI_Widget *btn_close;
	LCUI_Window *win;
	win = (LCUI_Window*)Malloc_Widget_Private(win_p, sizeof(LCUI_Window));
	win->hide_style	 = NONE;
	win->show_style	 = NONE;
	win->count		 = 0;
	win->init_align = ALIGN_MIDDLE_CENTER;
	
	/* ����һ������������ */
	titlebar = Create_Widget("titlebar");
	/* �ٴ���һ���ͻ������� */
	client_area = Create_Widget(NULL); 
	btn_close = Create_Widget("button"); 

	static LCUI_Graph btn_highlight, btn_normal, btn_down; 
	Graph_Init(&btn_down);
	Graph_Init(&btn_highlight);
	Graph_Init(&btn_normal);
	/* ����Ĭ��ͼ�� */
	Load_Graph_Default_TitleBar_CloseBox_Normal(&btn_normal);
	Load_Graph_Default_TitleBar_CloseBox_Down(&btn_down);
	Load_Graph_Default_TitleBar_CloseBox_HighLight(&btn_highlight);
	/* ��ʾ�����Ͻ� */
	Set_Widget_Align(btn_close, ALIGN_TOP_RIGHT, Pos(0, -2)); 
	/* ���ߴ�ĳɺ�ͼƬһ�� */
	Resize_Widget(btn_close, Size(btn_normal.width, btn_normal.height));
	Custom_Button_Style(btn_close, 
			&btn_normal, &btn_highlight, &btn_down, NULL, NULL);
	/* ������ť�ĵ���¼�������ť������󣬵���Quit_Window���� */
	Widget_Clicked_Event_Connect(btn_close, Quit_Parent_Window, NULL);
	Free_Graph(&btn_highlight);
	Free_Graph(&btn_down);
	Free_Graph(&btn_normal);
	
	win->client_area = client_area;
	win->titlebar = titlebar;
	win->btn_close = btn_close;
	/* û�б���ͼ����䱳��ɫ */
	Set_Widget_BG_Mode(win_p, BG_MODE_FILL_BACKCOLOR);
	Set_Widget_Border_Style(win_p, BORDER_STYLE_STANDARD); 
	
	/* ���������� */
	Widget_Container_Add(titlebar, btn_close);
	Widget_Container_Add(win_p, titlebar);
	Widget_Container_Add(win_p, client_area);
	Resize_Widget(win_p, Size(50, 50));
	Show_Widget(btn_close);
	
	Widget_Drag_Event_Connect(titlebar, Move_Window); 
}

static void Show_Window(LCUI_Widget *win_p)
/* ���ܣ��ڴ�����ʾʱ��������ش��� */
{
	int w, h;
	LCUI_Pos pos;
	LCUI_Window *win = (LCUI_Window*)Get_Widget_Private_Data(win_p);
	pos.x = 0;
	pos.y = 0;
	win->count++;
	if(win->count == 1)
	{/* ����ǵ�һ����ʾ */
		if(win_p->parent == NULL && win_p->pos_type == POS_TYPE_IN_SCREEN)
		{
			w = Get_Screen_Width();
			h = Get_Screen_Height();
		}
		else 
		{
			w = Get_Widget_Width(win_p->parent);
			h = Get_Widget_Height(win_p->parent);
		}
		
		switch(win->init_align)
		{/* ���ڵ�λ�� */
		case ALIGN_TOP_LEFT : 
			break;
		case ALIGN_TOP_CENTER :
			pos.x = (w - Get_Widget_Width(win_p))/2;
			break;
		case ALIGN_TOP_RIGHT :
			pos.x = w - Get_Widget_Width(win_p);
			break;
		case ALIGN_MIDDLE_LEFT :
			pos.y = (h - Get_Widget_Height(win_p))/2;
			break;
		case ALIGN_MIDDLE_CENTER :
			pos.y = (h - Get_Widget_Height(win_p))/2;
			pos.x = (w - Get_Widget_Width(win_p))/2; 
			break;
		case ALIGN_MIDDLE_RIGHT :
			pos.x = w - Get_Widget_Width(win_p);
			pos.y = (h - Get_Widget_Height(win_p))/2;
			break;
		case ALIGN_BOTTOM_LEFT :
			pos.y = h - Get_Widget_Height(win_p);
			pos.x = 0;
		case ALIGN_BOTTOM_CENTER :
			pos.y = h - Get_Widget_Height(win_p);
			pos.x = (w - Get_Widget_Width(win_p))/2;
		case ALIGN_BOTTOM_RIGHT :
			pos.y = h - Get_Widget_Height(win_p);
			pos.x = w - Get_Widget_Width(win_p);
		default : 
			break;
		}
		//printf("window pos: %d,%d\n",x, y);
		Set_Widget_Pos(win_p, pos);
	}
	//�д���չ 
}

static void Hide_Window(LCUI_Widget *win_p)
/* ���ܣ������ش���ʱʹ���Ӿ���Ч */
{
	//�д���չ
}

static void Resize_Window(LCUI_Widget *win_p)
/* ���ܣ��ڸı䴰�ڳߴ�ʱʹ���Ӿ���Ч */
{
	//�д���չ
	
	/* Ϊ�����Ͳ��������غ������������������ڴ��ڳߴ�ı�ʱҲ�ı��Լ��ĳߴ� */ 
	Window_Widget_Auto_Size(win_p); 
}

void Set_Window_Title_Text(LCUI_Widget *win_p, char *text)
/* ���ܣ�Ϊ�������ñ������� */
{
	LCUI_Widget *titlebar = Get_Window_TitleBar(win_p);
	LCUI_TitleBar *title = Get_Widget_Private_Data(titlebar);
	Set_Label_Text(title->label, text);
}


void Window_Client_Area_Add(LCUI_Widget *window, LCUI_Widget *widget)
/* ���ܣ���������������ڿͻ��� */
{
	LCUI_Widget *w = Get_Window_Client_Area(window);
	Widget_Container_Add(w, widget);
}

void Window_TitleBar_Add(LCUI_Widget *window, LCUI_Widget *widget)
/* ���ܣ���������������ڱ����� */
{
	LCUI_Widget *w = Get_Window_TitleBar(window);
	Widget_Container_Add(w, widget);
}

void Register_Window()
/* ���ܣ�ע�Ჿ������-������������ */
{
	/* ��Ӽ����������� */
	WidgetType_Add("window");
	WidgetType_Add("titlebar");
	
	/* Ϊ�������͹�����غ��� */ 
	WidgetFunc_Add("titlebar",	Window_TitleBar_Init,	FUNC_TYPE_INIT);
	WidgetFunc_Add("window",	Window_Init,			FUNC_TYPE_INIT);
	WidgetFunc_Add("window",	Exec_Update_Window,		FUNC_TYPE_UPDATE);
	WidgetFunc_Add("window",	Resize_Window,			FUNC_TYPE_RESIZE);
	WidgetFunc_Add("window",	Show_Window,			FUNC_TYPE_SHOW);
	WidgetFunc_Add("window",	Hide_Window,			FUNC_TYPE_HIDE);
	WidgetFunc_Add("window",	Destroy_Window,			FUNC_TYPE_DESTROY);
}

