/* ***************************************************************************
 * LCUI_Button.c -- LCUI��s Button widget
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
 * LCUI_Button.c -- LCUI �İ�ť����
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
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_GRAPHICS_H
#include LC_MISC_H
#include LC_INPUT_H 
#include LC_MEM_H 

static void Exec_Update_Button(LCUI_Widget *widget)
/* ���ܣ����°�ť��ͼ������ */
{
	LCUI_RGB color;
	LCUI_Button *button = (LCUI_Button *)Get_Widget_Private_Data(widget);
	/* ���ݰ�ť�Ĳ�ͬ��������� */
	if(Strcmp(&widget->style, "custom") == 0)
	{
		int no_bitmap = 0;
		if(widget->enabled == IS_FALSE) 
			widget->status = WIDGET_STATUS_DISABLE;
		/* �жϰ�ť��״̬����ѡ����Ӧ�ı���ɫ */
		switch(widget->status)
		{
		case WIDGET_STATUS_NORMAL:
			if(Valid_Graph(&button->btn_normal))
			{/* �����Ϊ��ť��normal״̬�趨��Ч���Զ���ͼ�� */
				/* ����ͼ�� */ 
				Zoom_Graph(&button->btn_normal, &widget->graph, 
						CUSTOM, widget->size);
				no_bitmap = 0;/* �����λͼ */
			}
			else
			{
				no_bitmap = 1;/* �����λͼ */
				/* ��������ɫ�����������ɫѡ�����鿴������ɫ��RGBֵ */
				color = RGB(100, 150, 255); 
			}
			break;
		case WIDGET_STATUS_OVERLAY :
			if(Valid_Graph(&button->btn_over))
			{ 
				Zoom_Graph(&button->btn_over, &widget->graph, 
					CUSTOM, widget->size);
				no_bitmap = 0;
			}
			else
			{
				no_bitmap = 1;
				color = RGB(50, 180, 240); /* ǳ��ɫ */
			}
			break;
		case WIDGET_STATUS_CLICKING :
			if(Valid_Graph(&button->btn_down))
			{
				/* ����ͼ�� */
				Zoom_Graph(&button->btn_down, &widget->graph, 
					CUSTOM, widget->size);
				no_bitmap = 0;
			}
			else
			{
				no_bitmap = 1;
				color = RGB(255, 50, 50); /* ��ɫ */
			}
			break;
		case WIDGET_STATUS_CLICKED :
			if(Valid_Graph(&button->btn_down))
			{
				/* ����ͼ�� */
				Zoom_Graph(&button->btn_down, &widget->graph, 
					CUSTOM, widget->size);
				no_bitmap = 0;
			}
			else
			{
				no_bitmap = 1;
				color = RGB(220, 220, 220); /* ��ɫ */
			} 
			break;
		case WIDGET_STATUS_FOCUS :
			if(Valid_Graph(&button->btn_focus))
			{
				/* ����ͼ�� */
				Zoom_Graph(&button->btn_focus, &widget->graph, 
					CUSTOM, widget->size );
				no_bitmap = 0;
			}
			else
			{
				no_bitmap = 1;
				color = RGB(50, 50, 255); /* ��ɫ */
			} 
			break;
		case WIDGET_STATUS_DISABLE :
			if(Valid_Graph(&button->btn_disable))
			{
				/* ����ͼ�� */
				Zoom_Graph(&button->btn_disable, &widget->graph, 
					CUSTOM, widget->size );
				no_bitmap = 0;
			}
			else
			{
				no_bitmap = 1;
				color = RGB(190, 190, 190); /* ��ɫ */
			} 
			break;
			default : break;
		}
		if(no_bitmap == 1)
		{/* ���û��λͼ */
			/* alphaͨ����ֵ��Ϊ255����͸�� */
			Fill_Graph_Alpha(&widget->graph, 255);
			/* Ϊ������䱳��ͼ */
			Fill_Background_Image( &widget->graph, 
					&widget->background_image, 0, color);
			/* Ȼ����ư�ť�߿򣬺�ɫ�� */
			Draw_Graph_Border(&widget->graph, RGB(0,0,0), 
									Border(1, 1, 1, 1));
		}
	}
	else if(Strcmp(&widget->style, "menu_style") == 0)
	{/* �˵�Ĭ��ʹ�õİ�ť��� */ 
		switch(widget->status)
		{
		case WIDGET_STATUS_NORMAL :
			Free_Graph(&widget->background_image);
			Fill_Graph_Alpha(&widget->graph, 0);  
			break;
		case WIDGET_STATUS_OVERLAY :
			color = RGB(80, 180, 240);
			Fill_Color(&widget->graph, color);
			Draw_Graph_Border(&widget->graph, RGB(50,50,255), 
									Border(1, 1, 1, 1));
			Fill_Graph_Alpha(&widget->graph, 255);
			break;
		case WIDGET_STATUS_CLICKING :
			color = RGB(80, 170, 255);
			Fill_Color(&widget->graph, color);
			Draw_Graph_Border(&widget->graph, RGB(50,50,255), 
									Border(1, 1, 1, 1));
			Fill_Graph_Alpha(&widget->graph, 255);
			break;
		case WIDGET_STATUS_CLICKED :
			color = RGB(80, 170, 255);
			Fill_Color(&widget->graph, color);
			Draw_Graph_Border(&widget->graph, RGB(50,50,255), 
									Border(1, 1, 1, 1));
			Fill_Graph_Alpha(&widget->graph, 255);
			break;
		case WIDGET_STATUS_FOCUS :
			color = RGB(50, 50, 255);
			Fill_Graph_Alpha(&widget->graph, 255);
			break;
		case WIDGET_STATUS_DISABLE :
			Free_Graph(&widget->background_image);
			Fill_Graph_Alpha(&widget->graph, 0); 
			break;
			default :
			break;
		}
	}
	else
	{/* �����ť�ķ��Ϊȱʡ */ 
		Strcpy(&widget->style, "default");
		switch(widget->status)
		{ 
		case WIDGET_STATUS_NORMAL :
			color = RGB(30, 145, 255); 
			break;
		case WIDGET_STATUS_OVERLAY :
			color = RGB(50, 180, 240);  
			break;
		case WIDGET_STATUS_CLICKING :
			color = RGB(255, 50, 50); 
			break;
		case WIDGET_STATUS_CLICKED :
			color = RGB(220, 220, 220); 
			break;
		case WIDGET_STATUS_FOCUS :
			color = RGB(50, 50, 255);
			break;
		case WIDGET_STATUS_DISABLE :
			color = RGB(190, 190, 190); 
			break;
			default :
			break;
		} 
		Fill_Graph_Alpha(&widget->graph, 255);
		Fill_Background_Image(&widget->graph, 
					&widget->background_image, 0, color); 
		Draw_Graph_Border(&widget->graph, RGB(0,0,0), 
							Border(1, 1, 1, 1));
	}
	/* ��ťÿ�θ��¶���Ҫ����������ť�����ڵ�ͼ�� */ 
	Refresh_Widget(widget); 
}


static void Button_Init(LCUI_Widget *widget)
/* ���ܣ���ʼ����ť���������� */
{
	LCUI_Button *button = (LCUI_Button*)
				Malloc_Widget_Private(widget, sizeof(LCUI_Button));
	 
	/* ��ʼ��ͼ������ */
	Graph_Init(&button->image); 
	Graph_Init(&button->btn_disable);
	Graph_Init(&button->btn_normal);
	Graph_Init(&button->btn_focus);
	Graph_Init(&button->btn_down);
	Graph_Init(&button->btn_over);
	
	button->image_align			= ALIGN_MIDDLE_CENTER;	/* ͼ������������� */
	button->text_image_relation	= Overlay;				/* ���ָ�����ͼƬ�� */ 
	button->label				= Create_Widget("label");/* ����label���� */ 
	
	Response_Status_Change(widget); /* ��Ӧ״̬�ı� */ 
	/* ����ť������Ϊlabel���������� */
	Widget_Container_Add(widget, button->label);
	/* label����������ʾ */
	Set_Widget_Align(button->label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Show_Widget(button->label); /* ��ʾlabel���� */
	Enable_Widget_Auto_Size(widget);/* �����Զ��ߴ����������Ӧ���� */
}


static void Destroy_Button(LCUI_Widget *widget)
{
	LCUI_Button *button = (LCUI_Button*)Get_Widget_Private_Data(widget);
	/* �ͷ�ͼ������ռ�õ��ڴ� */
	Free_Graph(&button->image); 
	Free_Graph(&button->btn_disable);
	Free_Graph(&button->btn_normal);
	Free_Graph(&button->btn_focus);
	Free_Graph(&button->btn_down);
	Free_Graph(&button->btn_over);
}


LCUI_Widget *Get_Button_Label(LCUI_Widget *widget)
/* ���ܣ���ȡǶ���ڰ�ť�������label���� */
{
	LCUI_Button *button = (LCUI_Button*)Get_Widget_Private_Data(widget);
	return button->label;
}

void Custom_Button_Style(	LCUI_Widget *widget, LCUI_Graph *normal, 
							LCUI_Graph *over, LCUI_Graph *down, 
							LCUI_Graph *focus, LCUI_Graph *disable)
/* ���ܣ��Զ��尴ť�ڸ���״̬����ʾ��λͼ */
{
	LCUI_Button *button = (LCUI_Button*)Get_Widget_Private_Data(widget);
	/* ���ͼ����Ч���Ϳ��� */
	if(Valid_Graph(normal)) Copy_Graph(&button->btn_normal, normal);
	if(Valid_Graph(over)) Copy_Graph(&button->btn_over, over);
	if(Valid_Graph(down)) Copy_Graph(&button->btn_down, down);
	if(Valid_Graph(focus)) Copy_Graph(&button->btn_focus, focus);
	if(Valid_Graph(disable)) Copy_Graph(&button->btn_disable, disable);
	/* �趨Ϊ�Զ����� */
	Set_Widget_Style(widget, "custom");
	Draw_Widget(widget); /* ���»��Ʋ��� */
}

void Set_Button_Text(LCUI_Widget *widget, const char *fmt, ...)
/* ���ܣ��趨��ť������ʾ���ı����� */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Button *button = (LCUI_Button*)Get_Widget_Private_Data(widget);
	LCUI_Widget *label = button->label;  
	
    memset(text, 0, sizeof(text));
    /* �����ǿɱ��������vsnprintf�������ݲ������ַ���������text�� */
    va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE-1, fmt, ap);
	va_end(ap);
	/* �趨������ʾ���ı� */
	Set_Label_Text(label, text);
}


LCUI_Widget *Create_Button_With_Text(const char *fmt, ...)
/* ���ܣ�����һ�����ı����ݵİ�ť */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Widget *widget = Create_Widget("button");
	
    memset(text, 0, sizeof(text)); 
    
    va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE-1, fmt, ap);
	va_end(ap); 
	
	Set_Button_Text(widget, text);
	return widget;
}


void Register_Button()
/*���ܣ�����ť��������ע���������� */
{
	/* ��Ӽ����������� */
	WidgetType_Add("button");
	
	/* Ϊ�������͹�����غ��� */
	WidgetFunc_Add("button", Button_Init,			FUNC_TYPE_INIT);
	WidgetFunc_Add("button", Exec_Update_Button,	FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("button", Destroy_Button,		FUNC_TYPE_DESTROY); 
}




