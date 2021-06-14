/* ***************************************************************************
 * LCUI_RadioButton.c -- LCUI��s RadioButton widget
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
 * LCUI_RadioButton.c -- LCUI �ĵ�ѡ�򲿼�
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
#include LC_PICBOX_H
#include LC_RADIOBTN_H
#include LC_GRAPHICS_H
#include LC_RES_H 
#include LC_MISC_H 
#include LC_INPUT_H 
#include LC_MEM_H 

static LCUI_Queue	mutex_lib;
static int mutex_lib_init = 0; /* ��־���Ƿ��ʼ���� */

void RadioButton_Delete_Mutex(LCUI_Widget *widget)
/* ���ܣ�����ѡ��ӻ����ϵ�����Ƴ� */
{
	int i, total;
	LCUI_Widget *tmp;
	LCUI_RadioButton *rb;
	
	rb = (LCUI_RadioButton *)Get_Widget_Private_Data(widget);
	total = Queue_Get_Total(rb->mutex);
	for(i=0; i<total; ++i)
	{
		tmp = (LCUI_Widget*)Queue_Get(rb->mutex, i);
		if(tmp == widget)
		{
			Queue_Delete_Pointer(rb->mutex, i);
			break;
		}
	}
}

void RadioButton_Create_Mutex(LCUI_Widget *a, LCUI_Widget *b)
/* ���ܣ�Ϊ������ѡ���������ϵ */
{
	int pos;
	LCUI_Queue *p, queue;
	LCUI_RadioButton *rb_a, *rb_b;
	if(mutex_lib_init == 0)
	{
		Queue_Init(&mutex_lib, sizeof(LCUI_Queue), Destroy_Queue);
		mutex_lib_init = 1;
	}
	
	rb_a = (LCUI_RadioButton *)Get_Widget_Private_Data(a);
	rb_b = (LCUI_RadioButton *)Get_Widget_Private_Data(b);
	
	if(rb_a->mutex == NULL)
	{
		if(rb_b->mutex == NULL)
		{
			Queue_Init(&queue, sizeof(LCUI_Widget*), NULL);
			/* ���Ӷ�������������У�����ȡλ�� */
			pos = Queue_Add(&mutex_lib, &queue);
			/* �Ӷ����л�ȡָ���Ӷ��е�ָ�� */
			p = (LCUI_Queue*)Queue_Get(&mutex_lib, pos);
			/* ���ָ�������� */
			Queue_Add_Pointer(p, a);
			Queue_Add_Pointer(p, b);
			/* ����ָ���ϵ���е�ָ�� */
			rb_a->mutex = p;
			rb_b->mutex = p;
		}
		else
		{
			Queue_Add_Pointer(rb_b->mutex, a);
			rb_a->mutex = rb_b->mutex;
		}
	}
	else
	{
		if(rb_b->mutex == NULL) 
		{
			Queue_Add_Pointer(rb_a->mutex, b);
			rb_b->mutex = rb_a->mutex;
		}
		else 
		{/* ���������������������л����ϵ����Ҫ�����ǲ𿪣������½��������ϵ */
			RadioButton_Delete_Mutex(a);
			RadioButton_Delete_Mutex(b);
			RadioButton_Create_Mutex(a, b);
		}
	}
}


void Set_RadioButton_On(LCUI_Widget *widget)
/* ���ܣ��趨��ѡ��Ϊѡ��״̬ */
{
	LCUI_RadioButton *radio_button;
	LCUI_Widget *other;
	int i, total;
	radio_button = (LCUI_RadioButton *)
								Get_Widget_Private_Data(widget); 
	if(NULL != radio_button->mutex)
	{/* ��������������л����ϵ���ͽ�������ѡ�򲿼���״̬��Ϊ��δѡ�С�״̬ */
		total = Queue_Get_Total(radio_button->mutex);
		for(i=0; i<total; ++i)
		{
			other = (LCUI_Widget*)Queue_Get(radio_button->mutex, i);
			Set_RadioButton_Off(other);
		}
	}
	radio_button->on = IS_TRUE;
	Draw_Widget(widget);
}

void Set_RadioButton_Off(LCUI_Widget *widget)
/* ���ܣ��趨��ѡ��Ϊδѡ��״̬ */
{
	LCUI_RadioButton *radio_button = (LCUI_RadioButton *)
								Get_Widget_Private_Data(widget); 
	
	radio_button->on = IS_FALSE;
	Draw_Widget(widget);
}

int Get_RadioButton_Status(LCUI_Widget *widget)
/* ���ܣ���ȡ��ѡ���״̬ */
{
	LCUI_RadioButton *radio_button = (LCUI_RadioButton *)
								Get_Widget_Private_Data(widget); 
	
	return radio_button->on;
}

int RadioButton_Is_On(LCUI_Widget *widget)
/* ���ܣ���ⵥѡ���Ƿ�ѡ�� */
{
	if(IS_TRUE == Get_RadioButton_Status(widget))
		return 1;
	
	return 0;
}

int RadioButton_Is_Off(LCUI_Widget *widget)
/* ���ܣ���ⵥѡ���Ƿ�δѡ�� */
{
	if(IS_TRUE == Get_RadioButton_Status(widget))
		return 0;
	
	return 1;
}

void Switch_RadioButton_Status(LCUI_Widget *widget, void *arg)
/* 
 * ���ܣ��л���ѡ���״̬
 * ˵�������״̬��ָ���Ǵ���û�򹴵�����״̬
 *  */
{ 
	if(RadioButton_Is_Off(widget))
		Set_RadioButton_On(widget); 
}

void RadioButton_Set_ImgBox_Size(LCUI_Widget *widget, LCUI_Size size)
/* ���ܣ��趨��ѡ���е�ͼ���ĳߴ� */
{
	if(size.w <= 0 && size.h <= 0)
		return;
		
	LCUI_Widget *imgbox = Get_RadioButton_ImgBox(widget);
	Resize_Widget(imgbox, size);
	/* ����û�в��ֺ��ӣ������Զ�����������ļ������ʱ��������� */
	Set_Widget_Align(imgbox->parent, ALIGN_MIDDLE_LEFT, Pos(size.w, 0));
}

static void RadioButton_Init(LCUI_Widget *widget)
/* ���ܣ���ʼ����ѡ�򲿼������� */
{
	LCUI_Widget *container[2];
	LCUI_RadioButton *radio_button = (LCUI_RadioButton*)
				Malloc_Widget_Private(widget, sizeof(LCUI_RadioButton));
	
	radio_button->on = IS_FALSE;
	/* ��ʼ��ͼƬ���� */ 
	Graph_Init(&radio_button->img_off_disable);
	Graph_Init(&radio_button->img_off_normal);
	Graph_Init(&radio_button->img_off_focus);
	Graph_Init(&radio_button->img_off_down);
	Graph_Init(&radio_button->img_off_over);
	Graph_Init(&radio_button->img_on_disable);
	Graph_Init(&radio_button->img_on_normal);
	Graph_Init(&radio_button->img_on_focus);
	Graph_Init(&radio_button->img_on_down);
	Graph_Init(&radio_button->img_on_over);
	
	radio_button->mutex = NULL;
	
	radio_button->label = Create_Widget("label");/* ����label���� */
	radio_button->imgbox = Create_Widget("picture_box"); /* ����ͼ��򲿼� */
	/* �����������������ڵ�����������������λ�� */
	container[0] = Create_Widget(NULL);
	container[1] = Create_Widget(NULL);
	
	/* ������Щ�������Զ��ߴ�����Ĺ��� */
	Enable_Widget_Auto_Size(widget);
	Enable_Widget_Auto_Size(container[0]);
	Enable_Widget_Auto_Size(container[1]);
	
	/* ����ť������Ϊlabel���������� */
	Widget_Container_Add(container[0], radio_button->imgbox);
	Widget_Container_Add(container[1], radio_button->label);
	Widget_Container_Add(widget, container[0]);
	Widget_Container_Add(widget, container[1]);
	
	/* �����ߴ� */
	Resize_Widget(radio_button->imgbox, Size(20, 20));
	/* �������� */
	Set_Widget_Align(container[0], ALIGN_MIDDLE_LEFT, Pos(0,0));
	Set_Widget_Align(container[1], ALIGN_MIDDLE_LEFT, Pos(22,0));
	Set_Widget_Align(radio_button->imgbox, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Set_Widget_Align(radio_button->label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	
	Set_PictureBox_Size_Mode(radio_button->imgbox, SIZE_MODE_STRETCH);
	
	/* ��ʾ֮ */
	Show_Widget(radio_button->label);
	Show_Widget(radio_button->imgbox);
	Show_Widget(container[0]);
	Show_Widget(container[1]);
	
	Widget_Clicked_Event_Connect(widget, Switch_RadioButton_Status, NULL);
	Response_Status_Change(widget);
}


static void Exec_Update_RadioButton(LCUI_Widget *widget)
/* ���ܣ����µ�ѡ���ͼ������ */
{
	LCUI_Graph *p;
	LCUI_RadioButton *radio_button = (LCUI_RadioButton *)
								Get_Widget_Private_Data(widget); 
								
	if(Strcmp(&widget->style, "custom") == 0)
	{/* ���Ϊ�Զ������Ǿ�ʹ���û�ָ����ͼ�� */
		//printf("custom\n"); 
		if(widget->enabled == IS_FALSE) 
			widget->status = WIDGET_STATUS_DISABLE;
			
		switch(widget->status)
		{/* �жϰ�ť��״̬����ѡ����Ӧ�ı���ɫ */
		case WIDGET_STATUS_NORMAL:
			if(radio_button->on == IS_TRUE)
				p = &radio_button->img_on_normal;
			else
				p = &radio_button->img_off_normal;
			
			Set_PictureBox_Image_From_Graph(radio_button->imgbox, p);
			break;
		case WIDGET_STATUS_OVERLAY :
			if(radio_button->on == IS_TRUE)
				p = &radio_button->img_on_over;
			else
				p = &radio_button->img_off_over;
			
			Set_PictureBox_Image_From_Graph(radio_button->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKING : 
			if(radio_button->on == IS_TRUE)
				p = &radio_button->img_on_down;
			else
				p = &radio_button->img_off_down;
			
			Set_PictureBox_Image_From_Graph(radio_button->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKED : 
			break;
		case WIDGET_STATUS_FOCUS : 
			break;
		case WIDGET_STATUS_DISABLE :
			if(radio_button->on == IS_TRUE)
				p = &radio_button->img_on_disable;
			else
				p = &radio_button->img_off_disable;
			
			Set_PictureBox_Image_From_Graph(radio_button->imgbox, p); 
			break;
			default :
			break;
		} 
	}
	else
	{/* �����ť�ķ��Ϊȱʡ */
		Strcpy(&widget->style, "default");
		if(widget->enabled == IS_FALSE) 
			widget->status = WIDGET_STATUS_DISABLE;
		
		/* ���ͷ�PictureBox�����б����ͼ�����ݵ�ָ�� */
		p = Get_PictureBox_Graph(radio_button->imgbox);
		Free_Graph(p);
		
		/* ���ڱ��������˳���ʹ�þֲ����������ͼ�����ݻ���Ч����ˣ������ڴ�ռ������� */
		p = (LCUI_Graph*)calloc(1,sizeof(LCUI_Graph));
		
		switch(widget->status)
		{/* �жϰ�ť��״̬����ѡ����Ӧ�ı���ɫ */
		case WIDGET_STATUS_NORMAL:
			if(radio_button->on == IS_TRUE)
				Load_Graph_Default_RadioButton_On_Normal(p);
			else
				Load_Graph_Default_RadioButton_Off_Normal(p);
			Set_PictureBox_Image_From_Graph(radio_button->imgbox, p);
			break;
		case WIDGET_STATUS_OVERLAY :
			if(radio_button->on == IS_TRUE)
				Load_Graph_Default_RadioButton_On_Selected(p);
			else
				Load_Graph_Default_RadioButton_Off_Selected(p);
			
			Set_PictureBox_Image_From_Graph(radio_button->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKING : 
			if(radio_button->on == IS_TRUE)
				Load_Graph_Default_RadioButton_On_Pressed(p);
			else
				Load_Graph_Default_RadioButton_Off_Pressed(p);
			
			Set_PictureBox_Image_From_Graph(radio_button->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKED : 
			break;
		case WIDGET_STATUS_FOCUS : 
			break;
		case WIDGET_STATUS_DISABLE :
			break;
			default :
			break;
		}
	} 
}

LCUI_Widget *Get_RadioButton_Label(LCUI_Widget *widget)
/* ���ܣ���ȡ��ѡ�򲿼��е�label������ָ�� */
{
	LCUI_RadioButton *radio_button = (LCUI_RadioButton *)
								Get_Widget_Private_Data(widget); 
	if(NULL == radio_button)
		return NULL;
		
	return radio_button->label;
}

LCUI_Widget *Get_RadioButton_ImgBox(LCUI_Widget *widget)
/* ���ܣ���ȡ��ѡ�򲿼��е�PictureBox������ָ�� */
{
	LCUI_RadioButton *radio_button = (LCUI_RadioButton *)
								Get_Widget_Private_Data(widget); 
	if(NULL == radio_button)
		return NULL;
		
	return radio_button->imgbox;
}

void Set_RadioButton_Text(LCUI_Widget *widget, const char *fmt, ...)
/* ���ܣ��趨�뵥ѡ�򲿼��������ı����� */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Widget *label = Get_RadioButton_Label(widget); 
	
    memset(text, 0, sizeof(text)); 
    
    va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE, fmt, ap);
	va_end(ap);

	Set_Label_Text(label, text); 
}

LCUI_Widget *Create_RadioButton_With_Text(const char *fmt, ...)
/* ���ܣ�����һ�����ı����ݵĵ�ѡ�� */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Widget *widget = Create_Widget("radio_button");
	
    memset(text, 0, sizeof(text)); 
    
    va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE, fmt, ap);
	va_end(ap); 
	
	Set_RadioButton_Text(widget, text);
	return widget;
}


void Register_RadioButton()
/*
 * ���ܣ�ע�Ჿ������-������������
 **/
{
	/* ��Ӽ����������� */
	WidgetType_Add("radio_button"); 
	
	/* Ϊ�������͹�����غ��� */ 
	WidgetFunc_Add("radio_button",	RadioButton_Init, FUNC_TYPE_INIT);
	WidgetFunc_Add("radio_button", Exec_Update_RadioButton, FUNC_TYPE_UPDATE); 
}
