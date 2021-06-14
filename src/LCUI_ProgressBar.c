/* ***************************************************************************
 * LCUI_ProgressBar.c -- LCUI's ProgressBar widget
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
 * LCUI_ProgressBar.c -- LCUI �Ľ���������
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
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_WIDGET_H
#include LC_PROGBAR_H
#include LC_PICBOX_H
#include LC_THREAD_H
#include LC_RES_H
#include LC_MEM_H
#include <unistd.h>
#include <pthread.h>

int Draw_Empty_Slot(LCUI_Graph *graph, int width, int height)
/* ���ܣ����ƽ������Ŀղ� */
{
	if(width < 4 || height < 4) return -1;
	graph->flag = HAVE_ALPHA;
	Malloc_Graph(graph, width, height); /* �����ڴ� */
	Fill_Color(graph, RGB(250,250,250));/* ����ɫ */
	Fill_Graph_Alpha(graph, 255);
	/* �ĸ����ϵ�һ�����ص���ȫ͸�� */
	graph->rgba[3][0] = 0;
	graph->rgba[3][width-1] = 0;
	graph->rgba[3][width*(height-1)] = 0;
	graph->rgba[3][width*height-1] = 0;
	
	int i, n;
	unsigned char c;
	/* ������ߺ��ұߵ������� */
	for(i=0; i<height; ++i)
	{
		n = i*width; 
		c = 160 +(30-(30.0/height)*i);
		graph->rgba[0][n] = c;
		graph->rgba[1][n] = c;
		graph->rgba[2][n] = c;
		graph->rgba[0][n+1] = 230;
		graph->rgba[1][n+1] = 230;
		graph->rgba[2][n+1] = 230; 
		
		n = n+width-1;
		graph->rgba[0][n] = c;
		graph->rgba[1][n] = c;
		graph->rgba[2][n] = c;
		graph->rgba[0][n-1] = 230;
		graph->rgba[1][n-1] = 230;
		graph->rgba[2][n-1] = 230;
	}
	
	/* ���ƶ��˵����� */
	memset(graph->rgba[0], 180, width);
	memset(graph->rgba[1], 180, width);
	memset(graph->rgba[2], 180, width);
	
	memset(graph->rgba[0]+width+1, 240, width-2); 
	memset(graph->rgba[1]+width+1, 240, width-2); 
	memset(graph->rgba[2]+width+1, 240, width-2); 
	
	/* ���Ƶ׶˵����� */
	n = width*(height-1);
	memset(graph->rgba[0]+n, 140, width);
	memset(graph->rgba[1]+n, 140, width);
	memset(graph->rgba[2]+n, 140, width);
	
	n = width*(height-2);
	memset(graph->rgba[0]+n+1, 225, width-2); 
	memset(graph->rgba[1]+n+1, 225, width-2); 
	memset(graph->rgba[2]+n+1, 225, width-2); 
	return 0;
}

LCUI_Widget *Get_ProgressBar_Flash_Img_Widget(LCUI_Widget *widget)
/* ���ܣ���ȡ������������ͼ�����ڵ�PictureBox���� */
{
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_Private_Data(widget);
	return pb->img_pic_box;
}

static void Destroy_ProgressBar(LCUI_Widget *widget)
/* ���ܣ��ͷŽ���������ռ�õ��ڴ���Դ */
{
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_Private_Data(widget);
	Free_Graph(&pb->fore_graph);
	Free_Graph(&pb->flash_image);
}

static void ProgressBar_Init(LCUI_Widget *widget)
/* ���ܣ���ʼ�������������ݽṹ�� */
{
	LCUI_ProgressBar *pb= (LCUI_ProgressBar*)
									Malloc_Widget_Private
									(widget, sizeof(LCUI_ProgressBar));
	pb->thread = 0;
	Graph_Init(&pb->fore_graph); 
	Graph_Init(&pb->flash_image);
	pb->img_move_speed = 200;
	pb->sleep_time = 100000;
	pb->max_value = 1;
	pb->value = 0;
	
	LCUI_Widget *f_pb, *img_pb;
	f_pb = Create_Widget("picture_box"); 
	img_pb = Create_Widget("picture_box");
	/* ���Լ�Ϊ����������Щ�����Ž�ȥ */
	Widget_Container_Add(f_pb, img_pb);
	Widget_Container_Add(widget, f_pb); 
	/* û�б���ͼʱ����䱳��ɫ */
	Set_Widget_BG_Mode(widget, BG_MODE_FILL_BACKCOLOR);
	Set_Widget_Border(widget, RGB(50,50,50), Border(1,1,1,1));
	
	Set_Widget_Style(widget, "classic"); /* ������Ϊ������ */
	
	Set_PictureBox_Size_Mode(f_pb, SIZE_MODE_STRETCH); 
	
	Show_Widget(f_pb); 
	Show_Widget(img_pb);
	
	pb->fore_pic_box = f_pb; 
	pb->img_pic_box = img_pb;
}

static void Exec_Update_ProgressBar(LCUI_Widget *widget)
/* ���ܣ����½�������ͼ�� */
{ 
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_Private_Data(widget);
	if(Strcmp(&widget->style, "dynamic") == 0)
	{
		/* ���ƿղ� */
		Draw_Empty_Slot(&widget->graph, widget->size.w, widget->size.h);
		Set_Widget_Border_Style(widget, BORDER_STYLE_NONE);
		/* ��������ͼ�� */
		if(!Valid_Graph(&pb->fore_graph)) 
			Load_Graph_ProgressBar_Fore(&pb->fore_graph);  
			
		if(!Valid_Graph(&pb->flash_image)) 
			Load_Graph_ProgressBar_Img(&pb->flash_image);
		
		Resize_Widget(pb->img_pic_box, 
				Size(pb->flash_image.width, pb->flash_image.height)); 
		/* ��ͼƬ������ʾ���ͼ�� */
		Set_PictureBox_Image_From_Graph(pb->img_pic_box, &pb->flash_image);
	}
	else 
	{
		Strcpy(&widget->style, "classic");
		if(!Valid_Graph(&pb->fore_graph)) 
			Malloc_Graph(&pb->fore_graph, 10, widget->size.h); 
		Fill_Color(&pb->fore_graph, RGB(80,80,200));
		Fill_Graph_Alpha(&pb->fore_graph, 255);
	}
	
	/* ��ͼƬ������ʾ���ͼ�� */
	Set_PictureBox_Image_From_Graph(pb->fore_pic_box, &pb->fore_graph); 
	
	int width, height;
	/* ����������ĳ��� */ 
	width = (widget->size.w - widget->border.left - widget->border.right) 
			* pb->value / pb->max_value + 0.5;
	height = widget->size.h - widget->border.top - widget->border.bottom;
	
	Move_Widget(pb->fore_pic_box, Pos(widget->border.left, widget->border.top));
	/* �ı�������ĳߴ� */
	Resize_Widget(pb->fore_pic_box, 
		Size(width, height));
}


void Set_ProgressBar_Max_Value(LCUI_Widget *widget, int max_value)
/* ���ܣ��趨���������ֵ */
{
	LCUI_ProgressBar *pb = Get_Widget_Private_Data(widget);
	pb->max_value = max_value; 
	Draw_Widget(widget); 
}

int Get_ProgressBar_Max_Value(LCUI_Widget *widget)
/* ���ܣ���ȡ���������ֵ */
{
	LCUI_ProgressBar *pb = Get_Widget_Private_Data(widget);
	return pb->max_value;
}

void Set_ProgressBar_Value(LCUI_Widget *widget, int value)
/* ���ܣ��趨��������ǰֵ */
{
	LCUI_ProgressBar *pb = Get_Widget_Private_Data(widget);
	pb->value = value; 
	Draw_Widget(widget);
}

int Get_ProgressBar_Value(LCUI_Widget *widget)
/* ���ܣ���ȡ��������ǰֵ */
{
	LCUI_ProgressBar *pb = Get_Widget_Private_Data(widget);
	return pb->value;
}

static void *Move_Flash_Img(void *arg)
/* ���ܣ��ƶ��������������λ�� */
{
	int x=0;
	LCUI_Widget *widget = (LCUI_Widget*)arg;
	LCUI_Widget *flash = Get_ProgressBar_Flash_Img_Widget(widget);
	LCUI_ProgressBar *pb = Get_Widget_Private_Data(widget);
	while(1)
	{
		for(x=(0-flash->size.w); 
			x<=flash->parent->size.w; 
			x+=(pb->img_move_speed/20.0+0.5))
		{
			Move_Widget(flash, Pos(x , flash->pos.y));
			usleep(50000);
		}
		usleep(pb->sleep_time);
	}
	LCUI_Thread_Exit(NULL);
}

static void Show_ProgressBar(LCUI_Widget *widget)
/* ���ܣ�����ʾ������ʱ�������������� */
{
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_Private_Data(widget);
	if(Strcmp(&widget->style, "dynamic") == 0)
	{
		if(pb->thread == 0)
		{
			Show_Widget(pb->img_pic_box);
			LCUI_Thread_Create(&pb->thread, NULL, Move_Flash_Img, (void*)widget);
		}
	}
	else 
	{
		Hide_Widget(pb->img_pic_box);
		if(pb->thread != 0)
		{/* ��������߳�ID��Ϊ0���ͳ����߳� */
			LCUI_Thread_Cancel(pb->thread);
			pb->thread = 0;
		}
	}
}

void Register_ProgressBar()
/* ���ܣ�ע�Ჿ������-�������������� */
{
	/* ��Ӳ������� */
	WidgetType_Add("progress_bar");
	
	/* Ϊ�������͹�����غ��� */
	WidgetFunc_Add("progress_bar",	ProgressBar_Init,		FUNC_TYPE_INIT);
	WidgetFunc_Add("progress_bar",	Exec_Update_ProgressBar,FUNC_TYPE_UPDATE);
	WidgetFunc_Add("progress_bar",	Show_ProgressBar,		FUNC_TYPE_SHOW); 
	WidgetFunc_Add("progress_bar",	Destroy_ProgressBar,	FUNC_TYPE_DESTROY); 
}

