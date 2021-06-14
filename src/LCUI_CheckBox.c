/* ***************************************************************************
 * LCUI_CheckBox.c -- LCUI's CheckBox widget
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
 * LCUI_CheckBox.c -- LCUI �ĸ�ѡ�򲿼�
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
#include LC_CHECKBOX_H
#include LC_GRAPHICS_H
#include LC_RES_H 
#include LC_MISC_H 
#include LC_INPUT_H 
#include LC_MEM_H 

void Set_CheckBox_On(LCUI_Widget *widget)
/* ���ܣ��趨��ѡ��Ϊѡ��״̬ */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
								Get_Widget_Private_Data(widget); 
	
	check_box->on = IS_TRUE;
	Draw_Widget(widget);
}

void Set_CheckBox_Off(LCUI_Widget *widget)
/* ���ܣ��趨��ѡ��Ϊδѡ��״̬ */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
								Get_Widget_Private_Data(widget); 
	
	check_box->on = IS_FALSE;
	Draw_Widget(widget);
}

int Get_CheckBox_Status(LCUI_Widget *widget)
/* ���ܣ���ȡ��ѡ���״̬ */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
								Get_Widget_Private_Data(widget); 
	
	return check_box->on;
}

int CheckBox_Is_On(LCUI_Widget *widget)
/* ���ܣ���⸴ѡ���Ƿ�ѡ�� */
{
	if(IS_TRUE == Get_CheckBox_Status(widget))
		return 1;
	
	return 0;
}

int CheckBox_Is_Off(LCUI_Widget *widget)
/* ���ܣ���⸴ѡ���Ƿ�δѡ�� */
{
	if(IS_TRUE == Get_CheckBox_Status(widget))
		return 0;
	
	return 1;
}

void Switch_CheckBox_Status(LCUI_Widget *widget, void *arg)
/* 
 * ���ܣ��л���ѡ���״̬
 * ˵�������״̬��ָ���Ǵ���û�򹴵�����״̬
 *  */
{ 
	if(CheckBox_Is_On(widget))
		Set_CheckBox_Off(widget);
	else
		Set_CheckBox_On(widget); 
}

void CheckBox_Set_ImgBox_Size(LCUI_Widget *widget, LCUI_Size size)
/* ���ܣ��趨��ѡ���е�ͼ���ĳߴ� */
{
	if(size.w <= 0 && size.h <= 0)
		return;
		
	LCUI_Widget *imgbox = Get_CheckBox_ImgBox(widget);
	Resize_Widget(imgbox, size);
	/* ����û�в��ֺ��ӣ������Զ�����������ļ������ʱ��������� */
	Set_Widget_Align(imgbox->parent, ALIGN_MIDDLE_LEFT, Pos(size.w, 0));
}

static void CheckBox_Init(LCUI_Widget *widget)
/* ���ܣ���ʼ����ѡ�򲿼������� */
{
	LCUI_Widget *container[2];
	LCUI_CheckBox *check_box = (LCUI_CheckBox*)
				Malloc_Widget_Private(widget, sizeof(LCUI_CheckBox));
	
	check_box->on = IS_FALSE;
	/* ��ʼ��ͼ������ */ 
	Graph_Init(&check_box->img_off_disable);
	Graph_Init(&check_box->img_off_normal);
	Graph_Init(&check_box->img_off_focus);
	Graph_Init(&check_box->img_off_down);
	Graph_Init(&check_box->img_off_over);
	Graph_Init(&check_box->img_on_disable);
	Graph_Init(&check_box->img_on_normal);
	Graph_Init(&check_box->img_on_focus);
	Graph_Init(&check_box->img_on_down);
	Graph_Init(&check_box->img_on_over);
	/* ��������Ĳ��� */
	check_box->label = Create_Widget("label");
	check_box->imgbox = Create_Widget("picture_box");  
	/* �����������������ڵ�����������������λ�� */
	container[0] = Create_Widget(NULL);
	container[1] = Create_Widget(NULL);
	
	/* ������Щ�������Զ��ߴ�����Ĺ��� */
	Enable_Widget_Auto_Size(widget);
	Enable_Widget_Auto_Size(container[0]);
	Enable_Widget_Auto_Size(container[1]);
	
	/* ����ť������Ϊlabel���������� */
	Widget_Container_Add(container[0], check_box->imgbox);
	Widget_Container_Add(container[1], check_box->label);
	Widget_Container_Add(widget, container[0]);
	Widget_Container_Add(widget, container[1]);
	
	/* �����ߴ� */
	Resize_Widget(check_box->imgbox, Size(20, 20));
	//Resize_Widget(container[0], Size(18,18));
	//Resize_Widget(widget, Size(18,18));
	/* �������� */
	Set_Widget_Align(container[0], ALIGN_MIDDLE_LEFT, Pos(0,0));
	Set_Widget_Align(container[1], ALIGN_MIDDLE_LEFT, Pos(22,0));
	Set_Widget_Align(check_box->imgbox, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Set_Widget_Align(check_box->label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	/* ����ͼ���ĳߴ�ģʽΪ���� */
	Set_PictureBox_Size_Mode(check_box->imgbox, SIZE_MODE_STRETCH);
	
	/* ��ʾ֮ */
	Show_Widget(check_box->label);
	Show_Widget(check_box->imgbox);
	Show_Widget(container[0]);
	Show_Widget(container[1]);
	/* ��������������¼� */
	Widget_Clicked_Event_Connect(widget, Switch_CheckBox_Status, NULL);
	/* ��Ӧ״̬�ı� */
	Response_Status_Change(widget);
}

static void Destroy_CheckBox(LCUI_Widget *widget)
/* ���ܣ��ͷŸ�ѡ�򲿼�ռ�õ���Դ */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
								Get_Widget_Private_Data(widget); 
	/* �ͷ�ͼ������ռ�õ��ڴ���Դ */ 
	Free_Graph(&check_box->img_off_disable);
	Free_Graph(&check_box->img_off_normal);
	Free_Graph(&check_box->img_off_focus);
	Free_Graph(&check_box->img_off_down);
	Free_Graph(&check_box->img_off_over);
	Free_Graph(&check_box->img_on_disable);
	Free_Graph(&check_box->img_on_normal);
	Free_Graph(&check_box->img_on_focus);
	Free_Graph(&check_box->img_on_down);
	Free_Graph(&check_box->img_on_over);
}

static void Exec_Update_CheckBox(LCUI_Widget *widget)
/* ���ܣ����¸�ѡ���ͼ������ */
{
	LCUI_Graph *p;
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
								Get_Widget_Private_Data(widget); 
								
	if(Strcmp(&widget->style, "custom") == 0)
	{/* ���Ϊ�Զ������Ǿ�ʹ���û�ָ����ͼ�Σ�����ɲο���ť�����Ĵ����� */ 
		if(widget->enabled == IS_FALSE) 
			widget->status = WIDGET_STATUS_DISABLE;
			
		switch(widget->status)
		{
		case WIDGET_STATUS_NORMAL:
			if(check_box->on == IS_TRUE)
				p = &check_box->img_on_normal;
			else
				p = &check_box->img_off_normal;
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_OVERLAY :
			if(check_box->on == IS_TRUE)
				p = &check_box->img_on_over;
			else
				p = &check_box->img_off_over;
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKING : 
			if(check_box->on == IS_TRUE)
				p = &check_box->img_on_down;
			else
				p = &check_box->img_off_down;
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKED : 
			break;
		case WIDGET_STATUS_FOCUS : 
			break;
		case WIDGET_STATUS_DISABLE :
			if(check_box->on == IS_TRUE)
				p = &check_box->img_on_disable;
			else
				p = &check_box->img_off_disable;
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p); 
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
		p = Get_PictureBox_Graph(check_box->imgbox);
		Free_Graph(p);
		
		/* ���ڱ��������˳���ʹ�þֲ����������ͼ�����ݻ���Ч����ˣ������ڴ�ռ������� */
		p = (LCUI_Graph*)calloc(1,sizeof(LCUI_Graph));
		
		switch(widget->status)
		{ 
		case WIDGET_STATUS_NORMAL:
			if(check_box->on == IS_TRUE)
				Load_Graph_Default_CheckBox_On_Normal(p);
			else
				Load_Graph_Default_CheckBox_Off_Normal(p);
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_OVERLAY :
			if(check_box->on == IS_TRUE)
				Load_Graph_Default_CheckBox_On_Selected(p);
			else
				Load_Graph_Default_CheckBox_Off_Selected(p);
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKING : 
			if(check_box->on == IS_TRUE)
				Load_Graph_Default_CheckBox_On_Selected(p);
			else
				Load_Graph_Default_CheckBox_Off_Selected(p);
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKED : 
			break;
		case WIDGET_STATUS_FOCUS : 
			break;
		case WIDGET_STATUS_DISABLE :
			if(check_box->on == IS_TRUE)
				Load_Graph_Default_CheckBox_On_Disabled(p);
			else
				Load_Graph_Default_CheckBox_Off_Disabled(p);
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
			default : break;
		}
	} 
}

LCUI_Widget *Get_CheckBox_Label(LCUI_Widget *widget)
/* ���ܣ���ȡ��ѡ�򲿼��е�label������ָ�� */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
								Get_Widget_Private_Data(widget); 
	if(NULL == check_box)
		return NULL;
		
	return check_box->label;
}

LCUI_Widget *Get_CheckBox_ImgBox(LCUI_Widget *widget)
/* ���ܣ���ȡ��ѡ�򲿼��е�PictureBox������ָ�� */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
								Get_Widget_Private_Data(widget); 
	if(NULL == check_box)
		return NULL;
		
	return check_box->imgbox;
}

void Set_CheckBox_Text(LCUI_Widget *widget, const char *fmt, ...)
/* ���ܣ��趨�븴ѡ�򲿼��������ı����� */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Widget *label = Get_CheckBox_Label(widget); 
	
    memset(text, 0, sizeof(text)); 
    
    va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE-1, fmt, ap);
	va_end(ap);

	Set_Label_Text(label, text); 
}

LCUI_Widget *Create_CheckBox_With_Text(const char *fmt, ...)
/* ���ܣ�����һ�����ı����ݵĸ�ѡ�� */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Widget *widget = Create_Widget("check_box");
	
    memset(text, 0, sizeof(text)); 
    
    va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE-1, fmt, ap);
	va_end(ap); 
	
	Set_CheckBox_Text(widget, text);
	return widget;
}


void Register_CheckBox()
/*
 * ���ܣ�ע�Ჿ������-������������
 **/
{
	/* ��Ӽ����������� */
	WidgetType_Add("check_box"); 
	
	/* Ϊ�������͹�����غ��� */ 
	WidgetFunc_Add("check_box",	CheckBox_Init, FUNC_TYPE_INIT);
	WidgetFunc_Add("check_box", Exec_Update_CheckBox, FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("check_box", Destroy_CheckBox, FUNC_TYPE_DESTROY);
}
