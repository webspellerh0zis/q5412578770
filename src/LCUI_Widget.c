/* ***************************************************************************
 * LCUI_Widget.c -- processing GUI widget 
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
 * LCUI_Widget.c -- ����GUI����
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
#include <unistd.h>
#include <math.h>
#include LC_LCUI_H 
#include LC_WIDGET_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_MEM_H
#include LC_FONT_H 
#include LC_ERROR_H 
#include LC_CURSOR_H
#include LC_INPUT_H


/***************************** Widget *********************************/

LCUI_Size Get_Widget_Size(LCUI_Widget *widget)
/* ���ܣ���ȡ�����ĳߴ� */
{
	return widget->size;
}

int Get_Widget_Height(LCUI_Widget *widget)
{
	return Get_Widget_Size(widget).h;
}

int Get_Widget_Width(LCUI_Widget *widget)
{
	return Get_Widget_Size(widget).w;
}

LCUI_Rect Get_Widget_Rect(LCUI_Widget *widget)
/* ���ܣ���ȡ���������� */
{
	return Rect(widget->pos.x, widget->pos.y, 
					widget->size.w, widget->size.h);
}

LCUI_Pos Get_Widget_Pos(LCUI_Widget *widget)
/* ���ܣ���ȡ������λ�� */
{
	return widget->pos;
}

void *Get_Widget_Private_Data(LCUI_Widget *widget)
/*
 * ���ܣ���ȡ������˽�����ݽṹ���ָ��
 **/
{
	return widget->private;
}

LCUI_Widget *Get_Widget_Parent(LCUI_Widget *widget)
/*
 * ���ܣ���ȡ�����ĸ�����
 **/
{
	return widget->parent;
}

void print_widget_info(LCUI_Widget *widget)
/* 
 * ���ܣ���ӡwidget����Ϣ
 * ˵�����ڵ���ʱ��Ҫ�õ���������ȷ��widget�Ƿ�������
 *  */
{
	if(widget != NULL)
	{
		printf("widget: %p, type: %s, visible: %d, pos: (%d,%d), size: (%d, %d)\n",
				widget, widget->type.string, widget->visible,
				widget->pos.x, widget->pos.y,
				widget->size.w, widget->size.h);  
	}
	else printf("NULL widget\n");
}


int Add_Widget_Refresh_Area (LCUI_Widget * widget, LCUI_Rect rect)
/*
 * ���ܣ���ָ���������ڲ��������趨��Ҫˢ�µ�����
 * */
{
	if(widget == NULL)
		return Add_Screen_Refresh_Area(rect);
		
	if (rect.width <= 0 || rect.height <= 0)
		return -1;
	
	/* ��������λ�ü��ߴ� */
	rect = Get_Valid_Area(Get_Widget_Size(widget), rect);
	
	/* ������������ */
	if(0 != RectQueue_Add (&widget->update_area, rect))
		return -1;
	
	if(widget->visible == IS_TRUE) 
		LCUI_Sys.shift_flag = IS_TRUE; 
		
	return 0;
}


void Set_Widget_Border_Style(LCUI_Widget *widget, LCUI_Border_Style style)
/* ���ܣ��趨�����ı߿��� */
{
	widget->border_style = style;
	Draw_Widget(widget);
}

void Split_Screen_Area_By_Widget(LCUI_Widget *widget)
/* 
 * ���ܣ����ݲ��������еĲ�������������������е�������зָ�
 * ˵�����ָ�������������rect_queue�У���ɾ�����ָ������
 *  */
{
	//static int count = 0;
	//int bak_count = count++;
	//printf("Split_Screen_Area_By_Widget(): enter, count = %d\n", bak_count); 
	int i,j, total; 
	LCUI_Pos pos;
	LCUI_Widget *child;
	LCUI_Rect rect, temp_rect;
	LCUI_Queue rq, *widget_list;
	
	RectQueue_Init(&rq);
	if(NULL == widget) 
		widget_list = &LCUI_Sys.widget_list; 
	else  
		widget_list = &widget->child;  
	
	total = Queue_Get_Total(widget_list);
	
	for(i=total-1; i>=0; --i)
	{/* �ӵ׵��������Ӳ��� */
		child = (LCUI_Widget*)Queue_Get(widget_list, i);
		if(child != NULL && child->visible == IS_TRUE)
		{/* ������Ӳ��� */ 
			/* �ݹ���ã������Ӳ��������򣬷ָ��Ѽ�¼������ */
			Split_Screen_Area_By_Widget ( child );
			
			for(j=0; RectQueue_Get(&rect, j, &LCUI_Sys.update_area); ++j)
			{/* ��ȡ��ǰ��¼������ */   
				/* ��ȡ�ò�����ʵ������ʾ�����򣬲���������ȫ������ */
				temp_rect = Get_Widget_Valid_Rect ( child ); 
				pos = Get_Widget_Global_Pos ( child );
				temp_rect.x += pos.x;
				temp_rect.y += pos.y; 
				if(!Rect_Valid(temp_rect)) 
					continue; 
					
				if( !Rect_Include_Rect(temp_rect, rect) 
				&& Rect_Is_Overlay(temp_rect, rect) )
				{ 
					/* ��Ȼ���ص������Ǿ�ɾ���������·ָ�����ľ�����ӽ�ȥ */
					Queue_Delete ( &LCUI_Sys.update_area, j );
					/* 
					 * ����������Ϊ���������Ѽ�¼��������Ϊǰ����
					 * ����ǰ��������зָ�
					 *  */
					Cut_Overlay_Rect(temp_rect, rect, &rq);
					while( RectQueue_Get(&rect, 0, &rq) )
					{
						Add_Screen_Refresh_Area(rect);			
						Queue_Delete(&rq, 0);
					}
					--j; 
				} 
			}
		}
	}
	//printf("Split_Screen_Area_By_Widget(): quit, count = %d\n", bak_count);
	Destroy_Queue(&rq);
	//count--;
}

void Response_Status_Change(LCUI_Widget *widget)
/* 
 * ���ܣ���ָ��������Ӧ����״̬�ĸı�
 * ˵������������ʱ��Ĭ���ǲ���Ӧ״̬�ı�ģ���Ϊÿ��״̬�ı�󣬶�Ҫ���ú����ػ沿����
 * �����һЩ�����Ƕ���ģ�û��Ҫ�ػ棬Ӱ��Ч�ʡ�������ò�������ť����������ƶ�����
 * ����ʱ�Լ��������ʱ������ı䰴ť��ͼ����ʽ���Ǿ���Ҫ�������������һ�¡�
 *  */
{
	widget->response_flag = 1;
}

void Shift_Widget_Refresh_Area(LCUI_Widget *widget)
/*
 * ���ܣ�ת�Ʋ�����rect���г�Ա����������
 **/
{
	//printf("Shift_Widget_Refresh_Area(): enter\n");
	int i, total;
	LCUI_Widget *child;
	LCUI_Rect rect;
	LCUI_Queue *widget_list;
	
	if(NULL == widget) 
		widget_list = &LCUI_Sys.widget_list; 
	else  
		widget_list = &widget->child; 
	
	total = Queue_Get_Total(widget_list);
	
	for(i=total-1; i>=0; --i)
	{/* �ӵ׵��������Ӳ��� */
		child = (LCUI_Widget*)Queue_Get(widget_list, i);
		if(child != NULL && child->visible == IS_TRUE)
		{/* ������Ӳ��� */
			/* �ݹ���ã�����¼������ת�����Ӳ����� */
			Shift_Widget_Refresh_Area(child);
			while( RectQueue_Get(&rect, 0, &child->update_area) )
			{
				rect.x += child->pos.x;
				rect.y += child->pos.y;
				//printf("[%d]\033[1;34mShift_Widget_Refresh_Area(x:%d, y:%d, w:%d, h:%d)\033[0m\n", child->update_area.total_num, rect.x, rect.y, rect.width, rect.height);
				/* ���Ӳ�����rect���г�Ա���Ƶ��Լ����� */
				
				Add_Widget_Refresh_Area(widget, rect); 
					
				/* ɾ���Ӳ���rect���г�Ա */
				Queue_Delete(&child->update_area, 0);
			}
		}
	} 
	//printf("Shift_Widget_Refresh_Area(): quit\n");
}



void Processing_Refresh_Area()
/*
 * ���ܣ������Ѽ�¼��ˢ������
 * ˵�����˺����Ὣ����������rect�����еĴ����������
 * ���յľֲ�ˢ�����������������Ļˢ����������У���
 * ��LCUI������
 **/
{
	//printf("Processing_Refresh_Area: enter\n");
	if ( LCUI_Sys.shift_flag == IS_TRUE )
	{	/* ���flag��־��ֵΪIS_TRUE */ 
		/* ת�Ʋ����ڼ�¼������������¼�� */ 
		Shift_Widget_Refresh_Area ( NULL );
		/* 
		 * ��λ��־������Ϊ�˱���ÿ�ν��뱾����ʱ��Ҫ����ݹ飬���Ը�
		 * �������ľ������ݽ����ƶ�����Ϊ�����˷�ʱ�䣬�����˳����Ч
		 * �� 
		 * */
		LCUI_Sys.shift_flag = IS_FALSE; 
	}
	/* �ָ�����ʹ֮�����κβ����������ص� */
	Split_Screen_Area_By_Widget(NULL);
	//printf("Processing_Refresh_Area: quit\n"); 
}


LCUI_Widget *Get_Parent_Widget(LCUI_Widget *widget, char *widget_type)
/*
 * ���ܣ���ȡ������ָ�����͵ĸ�������ָ��
 * ˵�������������ڲ�����ϵ������ͷ�����Ҹ�����ָ�룬���ж�����������Ƿ�Ϊ�ƶ�����
 * ����ֵ��û�з���Ҫ��ĸ��������ͷ���NULL�����򷵻ز���ָ��
 **/
{
	LCUI_Widget *temp;
	if(widget == NULL) return NULL; /* ������NULL���˳����� */
	temp = widget;
	while(temp->parent != NULL)
	{
		if(temp->parent != NULL
		  && Strcmp(&temp->parent->type, widget_type) == 0
		)/* ���ָ����Ч���������ͷ���Ҫ�� */
			return temp->parent; /* ���ز�����ָ�� */
		temp = temp->parent;/* ��ȡ�����ĸ�������ָ�� */
	}
	return NULL;
}

int LCUI_Destroy_App_Widgets(LCUI_ID app_id)
/* ���ܣ�����ָ��ID�ĳ�������в��� */
{
	int i, total;
	LCUI_Widget *temp;
	
	total = Queue_Get_Total(&LCUI_Sys.widget_list);
	for(i=0; i<total; i++)
	{
		temp = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list,i);
		if(temp->app_id == app_id)
		{
			/* 
			 * ��Queue_Delete()�����������еĲ����Ƴ�ʱ������ó�ʼ����������ʱָ
			 * ����Destroy_Widget()�������в���������ص�����
			 * */
			Queue_Delete(&LCUI_Sys.widget_list, i);
			/* ���»�ȡ�������� */
			total = Queue_Get_Total(&LCUI_Sys.widget_list);
			--i;/* ��ǰλ�õĲ����Ѿ��Ƴ�����λ���ɺ��沿��������ԣ�--i */
		}
	}
	return 0;
}


LCUI_String Get_Widget_Style(LCUI_Widget *widget)
/* ���ܣ���ȡ���������� */
{
	return widget->style;
}

LCUI_Border_Style Get_Widget_Border_Style(LCUI_Widget *widget)
/* ���ܣ���ȡ�����ı߿����� */
{
	return widget->border_style;
}

void Set_Widget_Style(LCUI_Widget *widget, char *style)
/* ���ܣ��趨�����ķ�� */
{
	Strcpy(&widget->style, style); 
}

LCUI_Widget *Get_Widget_By_Pos(LCUI_Widget *widget, LCUI_Pos pos)
/*
 * ���ܣ���ȡ�����а���ָ������ĵ���Ӳ���
 **/
{ 
	if(widget == NULL) 
		return NULL;
	int i, temp;
	LCUI_Widget *child;
	LCUI_Widget *w = widget; 
	for(i=0;;++i)
	{/* �Ӷ����ױ����Ӳ��� */
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if(NULL == child)
			break;
		if(child->visible == IS_TRUE)
		{ 
			temp = Rect_Inside_Point( pos, Get_Widget_Rect(child) );
			if(temp == 1)
			{/* �������㱻�����ڲ��������� */
				/* �ݹ���ã��ı�������� */
				w = Get_Widget_By_Pos(child, Pos_Sub(pos, child->pos));
				if(w == NULL)
					return widget;
				break;
			}
			else 
				continue;
		}
	}
	return w; 
}

LCUI_Widget *Get_Cursor_Overlay_Widget()
/*
 * ���ܣ���ȡ����굱ǰ���ǵĲ���
 **/
{ 
	int temp;
	int total,k; 
	LCUI_Widget *widget, *w = NULL;
	
	total = Queue_Get_Total(&LCUI_Sys.widget_list);
	for (k=0; k<total; ++k)
	{/* ������ף��������� */ 
		widget = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, k);
		if(widget == NULL) 
			break;
		if(widget->visible != IS_TRUE)
			continue;
			
		temp = Rect_Inside_Point(Get_Cursor_Pos(), Get_Widget_Rect(widget) );
		if(temp == 1)
		{/* �������㱻�����ڲ��������ڣ��Ǿͻ�ȡ�����а���ָ������ĵ���Ӳ��� */ 
			w = Get_Widget_By_Pos(widget, 
					Pos_Sub(Get_Cursor_Pos(), Get_Widget_Pos(widget)));
			if(w == NULL) 
				w = widget; 
			break;
		}
		else 
			continue; 
	} 
	return w;  
}



LCUI_Widget *Get_Focus_Widget()
/*
 * ���ܣ���ȡ��ǰ����״̬�µĲ���
 * ����ֵ��������ȡ�򷵻ز�����ָ�룬���򣬷���NULL
 **/
{
	#ifdef USE_JUNK___
	LCUI_App *app = Get_Self_AppPointer();
	LCUI_Widget *widget;
	widget = (LCUI_Widget*)Queue_Get(&app->widget_display, 0);
	if(widget != NULL)
	{
		while(widget->focus != NULL)
		{
			widget = widget->focus;
		}
	}
	else return NULL;
	return widget;
	#else 
	return NULL;
	#endif
}

int Widget_Is_Active(LCUI_Widget *widget)
/*
 * ���ܣ��жϲ����Ƿ�Ϊ�״̬
 **/
{
	if(widget->status != KILLED) return 1;
	return 0;
}

#define  _use_old_func_
#ifdef _use_old_func_
LCUI_Rect Get_Widget_Valid_Rect_By_New_Pos(LCUI_Widget *widget, LCUI_Pos offset)
/* 
 * ���ܣ���ȡ�ƶ�����λ�õĲ�������Ļ�ϵ���Ч��ʾ���� 
 * ˵����offset��ԭλ������λ�õĲ�
 * */
{
	LCUI_Rect Get_Widget_Valid_Rect(LCUI_Widget *widget);
	LCUI_Pos pos;
	int w, h, temp; 
	LCUI_Rect cut_rect;
	cut_rect.x = 0;
	cut_rect.y = 0;
	cut_rect.width = widget->size.w;
	cut_rect.height = widget->size.h;
	
	pos = widget->pos;
	pos.x += offset.x;
	pos.y += offset.y;
	
	if(widget->parent == NULL)
	{
		w = Get_Screen_Width();
		h = Get_Screen_Height();
	}
	else
	{
		w = widget->parent->size.w;
		h = widget->parent->size.h;
	}
	
	/* ��ȡ��ü������� */
	if(pos.x < 0) 
	{
		cut_rect.width += pos.x;
		cut_rect.x = 0 - pos.x; 
	}
	if(pos.x + widget->size.w > w)
		cut_rect.width -= (pos.x +  widget->size.w - w); 
	
	if(pos.y < 0)  
	{
		cut_rect.height += pos.y;
		cut_rect.y = 0 - pos.y; 
	}
	if(pos.y + widget->size.h > h)
		cut_rect.height -= (pos.y +  widget->size.h - h); 
	
	if(widget->parent != NULL)
	{
		LCUI_Rect rect;
		/* ��ȡ����������Ч��ʾ��Χ */
		rect = Get_Widget_Valid_Rect(widget->parent);
		/* �����������Ҫ�ü�����ô���Ӳ������������Ҳ��Ҫ���вü� */
		if(rect.x > 0)
		{/* ����ü������x���������0 */
			/* �ü�����Ͳ�����������ͬһ�����У�ֻҪ�ó�����������ص����򼴿� */
			temp = pos.x + cut_rect.x;
			if(temp < rect.x)
			{ /* ��������� x������+�ü����x������ С���� */
				temp = rect.x - pos.x;		/* �µĲü��������x������ */
				cut_rect.width -= (temp - cut_rect.x);/* �ı�ü�����Ŀ�� */
				cut_rect.x = temp;			/* �ı䲿���Ĳü������x���� */
			}
		}
		if(rect.y > 0)
		{
			temp = pos.y + cut_rect.y;
			if(pos.y < rect.y)
			{
				temp = rect.y - pos.y;
				cut_rect.height -= (temp - cut_rect.y);
				cut_rect.y = temp;
			}
		}
		if(rect.width < w)
		{/* ����������ü�����Ŀ��С�ڸ������Ŀ�� */
			temp = pos.x+cut_rect.x+cut_rect.width;
			if(temp > rect.x+rect.width) /* ��������ü�������߲����븸�����ü������ص� */
				cut_rect.width -= (temp-(rect.x+rect.width));
		}
		if(rect.height < h)
		{
			temp = pos.y+cut_rect.y+cut_rect.height;
			if(temp > rect.y+rect.height)
				cut_rect.height -= (temp-(rect.y+rect.height));
		}
	} 
	
	return cut_rect;
}

#else
LCUI_Rect Get_Widget_Valid_Rect_By_New_Pos(LCUI_Widget *widget, LCUI_Pos offset)
/* 
 * ���ܣ���ȡ�ƶ�����λ�õĲ�������Ļ�ϵ���Ч��ʾ���� 
 * ˵����offset��ԭλ������λ�õĲ���ص�������������Լ������򣬶������������е��������
 * 
 * */
{
	LCUI_Rect Get_Widget_Valid_Rect(LCUI_Widget *widget);
	LCUI_Pos pos;
	int w, h, temp; 
	LCUI_Rect cut_rect; 
	
	pos = widget->pos;
	pos.x += offset.x;
	pos.y += offset.y;
	
	if(widget->parent == NULL)
	{
		w = Get_Screen_Width();
		h = Get_Screen_Height();
	}
	else
	{
		w = widget->parent->size.w;
		h = widget->parent->size.h;
	}
	
	/* ��ȡ��ü������� */
	Get_Cut_Area(Size(w, h), Get_Widget_Rect(widget), &cut_rect);
	
	if(widget->parent != NULL)
	{
		LCUI_Rect rect;
		/* ��ȡ����������Ч��ʾ��Χ */
		rect = Get_Widget_Valid_Rect(widget->parent);
		/* �����������Ҫ�ü�����ô���Ӳ������������Ҳ��Ҫ���вü� */
		if(rect.x > 0)
		{/* ����ü������x���������0 */
			/* �ü�����Ͳ�����������ͬһ�����У�ֻҪ�ó�����������ص����򼴿� */
			temp = pos.x + cut_rect.x;
			if(temp < rect.x)
			{ /* ��������� x������+�ü����x������ С���� */
				temp = rect.x - pos.x;		/* �µĲü��������x������ */
				cut_rect.width -= (temp - cut_rect.x);/* �ı�ü�����Ŀ�� */
				cut_rect.x = temp;			/* �ı䲿���Ĳü������x���� */
			}
		}
		if(rect.y > 0)
		{
			temp = pos.y + cut_rect.y;
			if(pos.y < rect.y)
			{
				temp = rect.y - pos.y;
				cut_rect.height -= (temp - cut_rect.y);
				cut_rect.y = temp;
			}
		}
		if(rect.width < w)
		{/* ����������ü�����Ŀ��С�ڸ������Ŀ�� */
			temp = pos.x+cut_rect.x+cut_rect.width;
			if(temp > rect.x+rect.width) /* ��������ü�������߲����븸�����ü������ص� */
				cut_rect.width -= (temp-(rect.x+rect.width));
		}
		if(rect.height < h)
		{
			temp = pos.y+cut_rect.y+cut_rect.height;
			if(temp > rect.y+rect.height)
				cut_rect.height -= (temp-(rect.y+rect.height));
		}
	}
	else 
	{
		cut_rect.x += pos.x;
		cut_rect.y += pos.y; 
	}
	
	return cut_rect;
}
#endif

LCUI_Rect Get_Widget_Valid_Rect(LCUI_Widget *widget)
/* ���ܣ���ȡ��������Ļ��ʵ����ʾ������ */
{
	return Get_Widget_Valid_Rect_By_New_Pos(widget, Pos(0,0));
}

int Empty_Widget()
/* 
 * ���ܣ����ڼ�����Ĳ����б��Ƿ�Ϊ�� 
 * ����ֵ��
 *   1  ����Ĳ����б�Ϊ��
 *   0  ����Ĳ����б�Ϊ��
 * */
{
	if(Queue_Get_Total(&LCUI_Sys.widget_list) <= 0)
		return 1;
	return 0;
}

static void WidgetData_Init(LCUI_Queue *queue);

LCUI_Widget *Create_Widget(char *widget_type)
/* 
 * ���ܣ�����ָ�����͵Ĵ��ڲ���
 * ˵�������������Ĳ�����Ĭ����û�б���ͼʱ͸����
 * ����ֵ���ɹ��򲿼���ָ�룬ʧ���򷵻�NULL
 */
{
	int pos;
	LCUI_Widget *p, widget;
	LCUI_App *app;
	void (*func_init) (LCUI_Widget*);

	app = Get_Self_AppPointer(); 
	
	/* ��ʼ���ṹ���е����� */
	widget.auto_size		= IS_FALSE;
	widget.type_id			= 0;
	widget.status			= WIDGET_STATUS_NORMAL;
	widget.app_id			= app->id; 
	widget.parent			= NULL;
	widget.enabled			= IS_TRUE;
	widget.visible			= IS_FALSE; 
	widget.limit_pos			= IS_FALSE; 
	widget.pos				= Pos(0, 0); 
	widget.max_pos			= Pos(0, 0); 
	widget.min_pos			= Pos(0, 0); 
	widget.limit_size		= IS_FALSE; 
	widget.size				= Size(0, 0); 
	widget.max_size			= Size(0, 0); 
	widget.min_size			= Size(0, 0); 
	widget.align			= NONE; 
	widget.pos_type			= POS_TYPE_IN_SCREEN;
	widget.offset			= Pos(0, 0); 
	widget.back_color		= RGB(238,243,250);
	widget.fore_color		= RGB(0,0,0);
	widget.border_color		 = RGB(0,0,0);
	widget.border_style 	= NONE;
	widget.private			= NULL;
	widget.bg_mode			= BG_MODE_TRANSPARENT;
	widget.response_flag	= 0;
	
	widget.set_align = Set_Widget_Align;
	widget.set_alpha = Set_Widget_Alpha;
	widget.set_border = Set_Widget_Border;
	widget.show = Show_Widget;
	widget.hide = Hide_Widget;
	widget.resize = Resize_Widget;
	widget.move = Move_Widget;
	widget.enable = Enable_Widget;
	widget.disable = Disable_Widget;
	
	//widget.matrix		= NULL;
	/* ��ʼ���߿����� */
	Border_Init(&widget.border);
	/* ��ʼ������ͼ�����ݵĽṹ�� */
	Graph_Init(&widget.graph);
	Graph_Init(&widget.background_image);
	widget.graph.flag = HAVE_ALPHA;
	RectQueue_Init(&widget.update_area);/* ��ʼ��������¶��� */
	EventQueue_Init(&widget.event);/* ��ʼ���������¼����� */
	WidgetQueue_Init(&widget.child);/* ��ʼ���Ӳ������� */
	WidgetData_Init(&widget.data);/* ��ʼ�����ݸ��¶��� */
	String_Init(&widget.type);/* ��ʼ���ַ��� */
	String_Init(&widget.style);
	
	/* ��󣬽��ò���������������������� */
	pos = Queue_Add(&LCUI_Sys.widget_list, &widget);
	p = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, pos);
	
	if(widget_type != NULL)
	{
		/* ��֤���������Ƿ���Ч */
		if( !Check_WidgetType(widget_type))
		{
			puts(WIDGET_ERROR_TYPE_NOT_FOUND);
			return NULL;
		}
		/* ���沿������ */
		Strcpy(&p->type, widget_type);
		p->type_id = WidgetType_Get_ID(widget_type);	/* ��ȡ����ID */
		/* ��ȡ��ʼ������˽�нṹ�����ݵĺ���ָ�� */ 
		func_init = Get_WidgetFunc_By_ID(p->type_id, 
									FUNC_TYPE_INIT);
		/* ���г�ʼ�� */
		func_init( p ); 
	} 
	return p;
}


void Delete_Widget(LCUI_Widget *widget)
/* ���ܣ�ɾ��һ������ */
{
	int i, total;
	LCUI_Queue *p;
	LCUI_Widget *tmp;
	if(NULL == widget)
		return;
		
	if(widget->parent == NULL) 
		p = &LCUI_Sys.widget_list;
	else
		p = &widget->parent->child; 
	
	total = Queue_Get_Total(p);
	for(i=0; i<total; ++i)
	{
		tmp = (LCUI_Widget*)Queue_Get(p, i);
		if(tmp == widget)
		{
			Queue_Delete(p, i);
			break;
		}
	}	
}

LCUI_Pos Count_Widget_Pos(LCUI_Widget *widget)
/* ���ܣ��ۼƲ�����λ������ */
{
	LCUI_Pos pos;
	if(widget->parent == NULL) 
		return widget->pos; 
	pos = Count_Widget_Pos(widget->parent); 
	pos = Pos_Add(pos, widget->pos);
	return pos;
}

LCUI_Pos Get_Widget_Global_Pos(LCUI_Widget *widget)
/* ���ܣ���ȡ������ȫ������ */
{
	return Count_Widget_Pos(widget);
}

void Set_Widget_BG_Mode(LCUI_Widget *widget, LCUI_BG_Mode bg_mode)
/*
 * ���ܣ��ı䲿���ı���ģʽ
 * ˵��������ģʽ�����˲�����û�б���ͼ��ʱ����ʹ�ñ���ɫ��仹����ȫ͸����
 **/
{
	widget->bg_mode = bg_mode;
}

void Set_Widget_Align(LCUI_Widget *widget, LCUI_Align align, LCUI_Pos offset)
/* ���ܣ��趨�����Ķ��뷽ʽ�Լ�ƫ�ƾ��� */
{
	widget->align = align;
	widget->offset = offset;
	Update_Widget_Pos(widget);/* ����λ�� */
}

void Limit_Widget_Size(LCUI_Widget *widget, LCUI_Size min_size, LCUI_Size max_size)
/* ���ܣ����Ʋ����ĳߴ�䶯��Χ */
{
	if(min_size.w < 0)
		min_size.w = 0;
	if(min_size.h < 0)
		min_size.h = 0;
	if(max_size.w < 0)
		max_size.w = 0;
	if(max_size.h < 0)
		max_size.h = 0;
	if(max_size.w < min_size.w)
		max_size.w = min_size.w;
	if(max_size.h < min_size.h)
		max_size.h = min_size.h;
		
	widget->min_size = min_size;
	widget->max_size = max_size;
	widget->limit_size = IS_TRUE;
}

void Limit_Widget_Pos(LCUI_Widget *widget, LCUI_Pos min_pos, LCUI_Pos max_pos)
/* ���ܣ����Ʋ������ƶ���Χ */
{
	if(min_pos.x < 0)
		min_pos.x = 0;
	if(min_pos.y < 0)
		min_pos.y = 0;
	if(max_pos.x < 0)
		max_pos.x = 0;
	if(max_pos.y < 0)
		max_pos.y = 0;
	if(max_pos.x < min_pos.x)
		max_pos.x = min_pos.x;
	if(max_pos.y < min_pos.y)
		max_pos.y = min_pos.y;
		
	widget->min_pos = min_pos;
	widget->max_pos = max_pos;
	widget->limit_pos = IS_TRUE;
}

void Set_Widget_Border(LCUI_Widget *widget, LCUI_RGB color, LCUI_Border border)
/* ���ܣ��趨�����ı߿� */
{ 
	widget->border = border;
	widget->border_color = color;
	
	if(widget->border_style == BORDER_STYLE_NONE) 
		Set_Widget_Border_Style(widget, BORDER_STYLE_LINE_BORDER);
		
	Draw_Widget(widget); 
	Add_Widget_Refresh_Area(widget, Get_Widget_Rect(widget));
}


void Set_Widget_Backcolor(LCUI_Widget *widget, LCUI_RGB color)
/* 
* ���ܣ��趨�����ı���ɫ 
*/
{
	widget->back_color = color;
	Draw_Widget(widget);
	Refresh_Widget(widget);
}

int Set_Widget_Background_Image(LCUI_Widget *widget, LCUI_Graph *img, int flag)
/*
 * ���ܣ�Ϊ������䱳��ͼ��
 */
{
	if(NULL == img)
		Free_Graph(&widget->background_image);
	else
	{
		widget->background_image_layout = flag;
		/* ��䱳��ͼ�� */
		Copy_Graph(&widget->background_image, img);
	}
	Draw_Widget(widget); 
	return 0;
}

int Mix_Widget_FontBitmap(	LCUI_Widget *widget, 
							int start_x, int start_y, 
							LCUI_WString *contents, 
							int rows, int space, 
							int linegap, int flag	)
/* ���ܣ���ϲ����ڵ�����λͼ��ʹ�������ڲ�������ʾ */
{
	if(!Valid_Graph(&widget->graph)) 
		return -1; 
	if(rows > 0)
	{/* ���������������0 */
		int i, j ,x = start_x, y = start_y, height = 0;
		for(i = 0; i < rows; ++i)
		{
			for(j=0; j < contents[i].size; ++j)
			{
				if(contents[i].string[j].bitmap.height > 0)
				{
					height = contents[i].string[j].bitmap.height;
					break;
				}
			}
			if(height > 0) break;
		}
		for(i = 0; i < rows; ++i)
		{
			/* ����Ѿ����������ĳߴ磬�Ͳ���Ҫ��������λͼ�� */
			if(y > widget->size.h) 
				break; 
			for(j=0; j < contents[i].size; ++j)
			{
				if(x > widget->size.w)  /* ͬ�� */ 
					break; 
				/* ճ��ÿ���� */
				Mix_Fonts_Bitmap(
					&widget->graph,  x, y , 
					&contents[i].string[j].bitmap , 
					contents[i].string[j].color, flag );
					
				if(contents[i].update == IS_TRUE)
				{/* �����һ��������Ҫ���� */
					contents[i].update = IS_FALSE;
					Add_Widget_Refresh_Area(
						widget, 
						Rect(	start_x, y-1, 
								widget->size.w, 
								contents[i].string[0].bitmap.height+2)
					);
					/* ��΢���ӵ�����Χ������y-1��height+2 */
				}
				if( j < contents[i].size - 1) 
					/* �ۼ����ּ�� */
					x = x + space;  
				/* �ۼ����ֿ�� */
				x = x + contents[i].string[j].bitmap.width;
			}
			if(contents[i].size == 0)
				y += height;
			else
				y += contents[i].string[0].bitmap.height; 
			if(i > 0 && i < rows - 1) 
				/* �ۼ������о� */
				y += linegap; 
			x = start_x; /* x��λ */
			//printf("Mix_Widget_FontBitmap():rows:%d/%d, size:%d\n", i, rows, contents[i].size);
		}
	}
	return 0;
}

void Enable_Widget(LCUI_Widget *widget)
/* ���ܣ����ò��� */
{
	widget->enabled = IS_TRUE; 
	Set_Widget_Status(widget, WIDGET_STATUS_NORMAL);
}

void Disable_Widget(LCUI_Widget *widget)
/* ���ܣ����ò��� */
{
	widget->enabled = IS_FALSE; 
	Set_Widget_Status(widget, WIDGET_STATUS_DISABLE);
}

void Widget_Visible(LCUI_Widget *widget, int flag)
/* ���ܣ����岿���Ƿ�ɼ� */
{
	if(flag == IS_TRUE) 
		widget->visible = IS_TRUE; 
	else 
		widget->visible = IS_FALSE; 
}

void Set_Widget_Pos(LCUI_Widget *widget, LCUI_Pos pos)
/* 
 * ���ܣ��趨������λ�� 
 * ˵����ֻ�޸����꣬�����оֲ�ˢ��
 * */
{
	widget->pos = pos;
}

void Set_Widget_Alpha(LCUI_Widget *widget, unsigned char alpha)
/* ���ܣ��趨������͸���� */
{
	if(widget->graph.alpha != alpha)
	{
		widget->graph.alpha = alpha; 
		Refresh_Widget(widget);
	}
}

void Exec_Move_Widget(LCUI_Widget *widget, LCUI_Pos pos)
/*
 * ���ܣ�ִ���ƶ�����λ�õĲ���
 * ˵�������Ĳ���λ�ã�����Ӿֲ�ˢ������
 **/
{
	LCUI_Pos t;
	LCUI_Rect old_rect;
	if(IS_TRUE == widget->limit_pos)
	{/* ���ָ�������Ʋ������ƶ���Χ����ô�͵���λ�� */
		if(pos.x > widget->max_pos.x)
			pos.x = widget->max_pos.x;
		if(pos.y > widget->max_pos.y)
			pos.y = widget->max_pos.y;
		if(pos.x < widget->min_pos.x)
			pos.x = widget->min_pos.x;
		if(pos.y < widget->min_pos.y)
			pos.y = widget->min_pos.y;
	}
	t = widget->pos; 
	widget->pos = pos;
	if(widget->visible == IS_TRUE)
	{/* ����ò����ɼ� */
		/* �趨��Ҫˢ�µ����� */
		//old_rect = Rect(t.x, t.y, widget->size.w, widget->size.h); 
		//Add_Widget_Refresh_Area(widget->parent, old_rect);
		//Refresh_Widget(widget);
					
		LCUI_Rect a, b;
		old_rect = Rect(t.x, t.y, widget->size.w, widget->size.h);
		Refresh_Widget(widget);
		if(Rect_Is_Overlay(Get_Widget_Rect(widget), old_rect))
		{
			Get_Moved_Rect_Refresh_Area( pos.x, pos.y, old_rect, &a, &b );
			Add_Widget_Refresh_Area(widget->parent, a); 
			Add_Widget_Refresh_Area(widget->parent, b); 
		}
	}
}

void Exec_Hide_Widget(LCUI_Widget *widget)
/* ���ܣ�ִ�����ز����Ĳ��� */
{
	if(widget == NULL) 
		return;
	if(widget->visible == IS_FALSE)
		return;
	
	void ( *func_hide ) (LCUI_Widget*);
	
	/* ��ȡ���ز�����Ҫ���õĺ���ָ�룬������֮ */
	func_hide = Get_WidgetFunc_By_ID( widget->type_id,
					FUNC_TYPE_HIDE );
	func_hide(widget);
	
	Widget_Visible(widget, IS_FALSE); 
	Add_Widget_Refresh_Area(widget->parent, Get_Widget_Rect(widget)); 
}


void Exec_Show_Widget(LCUI_Widget *widget)
/* ���ܣ�ִ����ʾ���������� */
{ 
	void ( *func_show ) (LCUI_Widget*); 

	if(widget == NULL) 
		return;
		
	if(widget->visible == IS_TRUE)
		return;
		
	//Exec_Update_Widget(widget);/* ��ʾǰ����Ҫ�Բ���ͼ�����ݽ���һ�θ��� */
	Widget_Visible(widget, IS_TRUE); /* �����ɼ� */
	
	/* ���øò�������ʾʱ��Ҫ�õ��ĺ��� */
	func_show = Get_WidgetFunc_By_ID(widget->type_id, FUNC_TYPE_SHOW); 
	func_show(widget); 
	
	Refresh_Widget(widget); /* ˢ�²������������ͼ����ʾ */
}

void Auto_Resize_Widget(LCUI_Widget *widget)
/* ���ܣ��Զ�����������С������Ӧ�����ݴ�С */
{
	int i, total, temp;
	LCUI_Widget *child;
	LCUI_Queue point;
	LCUI_Size size;
	
	size.w = 0;
	size.h = 0;
	Queue_Init(&point, sizeof (LCUI_Pos), NULL);
	total = Queue_Get_Total(&widget->child);
	for(i=0; i<total; ++i)
	{/* ����ÿ���Ӳ��� */
		child = (LCUI_Widget *)Queue_Get(&widget->child, i);
		/* ��������Ӳ���������ε����ϽǶ����У�X���������� */
		temp = child->pos.x + child->size.w;
		if(temp > size.w)
			size.w = temp;
		/* ͬ�� */
		temp = child->pos.y + child->size.h;
		if(temp > size.h)
			size.h = temp;
	}
	size.w += 6;
	size.h += 6;
	//printf("Auto_Resize_Widget(): new size: %d,%d\n", size.w, size.h);
	//print_widget_info(widget);
	/* �ó��ʺϵĳߴ磬����֮ */
	Resize_Widget(widget, size);
}

void Exec_Resize_Widget(LCUI_Widget *widget, LCUI_Size size)
/* ���ܣ�ִ�иı䲿���ߴ�Ĳ��� */
{
	if(widget == NULL) 
		return;
		
	void ( *func_resize ) (LCUI_Widget*);
	
	if(widget->size.w == size.w && widget->size.h == size.h) 
		return;
	if(IS_TRUE == widget->limit_size)
	{/* ���ָ�������Ʋ����ĳߴ磬��ô�͵����ߴ� */
		if(size.w > widget->max_size.w)
			size.w = widget->max_size.w;
		if(size.h > widget->max_size.h)
			size.h = widget->max_size.h;
		if(size.w < widget->min_size.w)
			size.w = widget->min_size.w;
		if(size.h < widget->min_size.h)
			size.h = widget->min_size.h;
	}
	Add_Widget_Refresh_Area(widget->parent, Get_Widget_Rect(widget));
	widget->size = size;
	Malloc_Graph(&widget->graph, size.w, size.h);
	/* ��ȡ�ı䲿���ߴ�ʱ��Ҫ���õĺ��� */
	func_resize = Get_WidgetFunc_By_ID( widget->type_id, 
						FUNC_TYPE_RESIZE );
	func_resize(widget); 
	Refresh_Widget(widget); 
	Update_Child_Widget_Pos(widget);/* �����Ӳ�����λ�� */  
	
	if(widget->parent != NULL
	&& widget->parent->auto_size == IS_TRUE)
	/* �����Ҫ�����������ܹ��Զ�������С */
		Auto_Resize_Widget(widget->parent); 
}

void Enable_Widget_Auto_Size(LCUI_Widget *widget)
/* ���ܣ����ò����Զ��ߴ�������� */
{
	widget->auto_size = IS_TRUE;
}

void Disable_Widget_Auto_Size(LCUI_Widget *widget)
/* ���ܣ����ò����Զ��ߴ�������� */
{
	widget->auto_size = IS_FALSE;
}

void Exec_Refresh_Widget(LCUI_Widget *widget)
/* ���ܣ�ִ��ˢ����ʾָ����������������ͼ�εĲ��� */
{ 
	Add_Widget_Refresh_Area(widget, Rect(0, 0, widget->size.w, widget->size.h));
}

void Exec_Draw_Widget(LCUI_Widget *widget)
/* ���ܣ�ִ�в���ͼ�θ��²��� */
{ 
	if(widget == NULL) 
		return;
		 
	void ( *func_update ) (LCUI_Widget*); 
	
	if(Valid_Graph(&widget->background_image))
	{/* ����б���ͼ */
		/* alphaͨ���е�ÿ�����ص�͸��ֵΪ255������������ͼ�β�͸�� */
		Fill_Graph_Alpha(&widget->graph, 255);
		
		Fill_Background_Image(
				&widget->graph, 
				&widget->background_image, 
				widget->background_image_layout,
				widget->back_color
		); /* ��䱳��ɫ */
	}
	else 
	{/* ������ݱ���ģʽ������ */
		switch(widget->bg_mode)
		{
			case BG_MODE_FILL_BACKCOLOR: /* ��䱳��ɫ */
				Fill_Graph_Alpha(&widget->graph, 255);
				Fill_Color(&widget->graph, widget->back_color); 
				break;
				
			case BG_MODE_TRANSPARENT: /* ��ȫ͸�� */
				Fill_Graph_Alpha(&widget->graph, 0); 
				break;
			default:break;
		}
	}
	
	/* ��ȡ���� */
	func_update = Get_WidgetFunc_By_ID( widget->type_id, 
										FUNC_TYPE_UPDATE );
	func_update(widget);
	/* ���Ʊ߿��� */
	if(widget->border_style != BORDER_STYLE_NONE)
		Draw_Graph_Border(	&widget->graph, 
							widget->border_color, 
							widget->border); 
}



LCUI_Pos Widget_Align_Get_Pos(LCUI_Widget *widget)
/* ���ܣ����ݲ����Ĳ�������ȡ������λ�� */
{
	int width, height;
	LCUI_Pos pos;
	pos.x = pos.y = 0;
	/* ����λ�����ͣ�����ȡ�����ĳߴ� */ 
	switch(widget->pos_type)
	{
		case POS_TYPE_IN_SCREEN :
			width	= Get_Screen_Width();
			height	= Get_Screen_Height();
			break;
		case POS_TYPE_IN_WIDGET :
			width	= Get_Widget_Width(widget->parent);
			height	= Get_Widget_Height(widget->parent);
			break;
		default: return pos; 
	} 
	
	if(widget->align != ALIGN_NONE)
	{
		switch(widget->align)
		{
			case ALIGN_TOP_LEFT : /* �����ϽǶ��� */  
				break;
			case ALIGN_TOP_CENTER : /* �����м���� */ 
				pos.x = (width - widget->size.w) / 2; 
				break;
			case ALIGN_TOP_RIGHT : /* �����ϽǶ��� */ 
				pos.x = width - widget->size.w; 
				break;
			case ALIGN_MIDDLE_LEFT : /* ������ƫ����� */  
				pos.y = (height - widget->size.h) / 2;
				break;
			case ALIGN_MIDDLE_CENTER : /* ����������� */ 
				pos.x = (width - widget->size.w) / 2;
				pos.y = (height - widget->size.h) / 2;
				break;
			case ALIGN_MIDDLE_RIGHT : /* ������ƫ�ɶ��� */ 
				pos.x = width - widget->size.w;
				pos.y = (height - widget->size.h) / 2;
				break;
			case ALIGN_BOTTOM_LEFT : /* ��ײ�ƫ����� */  
				pos.y = height - widget->size.h;
				break;
			case ALIGN_BOTTOM_CENTER : /* ��ײ����ж��� */ 
				pos.x = (width - widget->size.w) / 2;
				pos.y = height - widget->size.h;
				break;
			case ALIGN_BOTTOM_RIGHT : /* ��ײ�ƫ�Ҷ��� */ 
				pos.x = width - widget->size.w;
				pos.y = height - widget->size.h;
				break; 
			default :  break; 
		}
		/* ����ƫ�ƾ��� */
		pos.x = pos.x + widget->offset.x;
		pos.y = pos.y + widget->offset.y; 
		return pos;
	}
	
	return widget->pos;
}

/* �������� */
static int Record_WidgetUpdate(LCUI_Widget *widget, void *data, int type);

void Move_Widget(LCUI_Widget *widget, LCUI_Pos new_pos)
/* 
 * ���ܣ��ƶ�����λ��
 * ˵������������Ĳ���ΪALIGN_NONE����ô���Ϳ����ƶ�����λ�ã������޷��ƶ�λ��
 * */
{
	if(widget == NULL) 
		return;
	/* ��¼�����ĸ������ݣ��ȴ����и��� */
	Record_WidgetUpdate(widget, &new_pos, DATATYPE_POS);
}

void Update_Widget_Pos(LCUI_Widget *widget)
/* ���ܣ����²�����λ�� */
{ 
	Record_WidgetUpdate(widget, NULL, DATATYPE_POS);
}

void Update_Child_Widget_Pos(LCUI_Widget *widget)
/* 
 * ���ܣ�����ָ���������Ӳ�����λ��
 * ˵��������Ϊ�Ӳ��������������ĳߴ�ı���еĲ����Ĳ��ֲ�ΪALIGN_NONE������Ҫ����
 * ����λ�á�
 * */
{
	LCUI_Widget *child;
	int i, total;
	total = Queue_Get_Total(&widget->child);
	for(i=0; i<total; ++i) 
	{
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		Update_Widget_Pos( child ); 
	}
}

void Offset_Widget_Pos(LCUI_Widget *widget, LCUI_Pos offset)
/* ���ܣ��Բ���ԭ�е�λ��Ϊ����������ָ����ƫ������ƫ��λ�� */
{
	Move_Widget( widget, Pos_Add(widget->pos, offset) ); 
}

void Move_Widget_To_Pos(LCUI_Widget *widget, LCUI_Pos des_pos, int speed)
/* 
 * ���ܣ���������ָ�����ٶ���ָ��λ���ƶ� 
 * ˵����des_pos��Ŀ��λ�ã�speed�Ǹò������ƶ��ٶȣ���λΪ������/��
 * */
{
	if(speed <= 0) return;
	
	int i, j;
	double w, h, l, n, x, y; 
	x = Get_Widget_Pos(widget).x;
	y = Get_Widget_Pos(widget).y;
	/* ������֮��ľ��� */
	w = des_pos.x-x;
	h = des_pos.y-y;
	l = sqrt(pow(w, 2) + pow(h, 2));
	
	n = l/speed;/* ���ƶ�����ʱ�� */
	n = n*100;	/* ���ƶ�����������100������Ϊÿ��0.01�ƶ�һ��λ�� */
	w = w/n;	/* ����ÿ���ƶ���x��y��������ƶ����� */
	h = h/n;
	j = (int)(n>(int)n?n+1:n);
	for(i=0; i<j; i++)
	{
		x += w;
		y += h;
		Move_Widget(widget, Pos(x, y));
		usleep(10000);/* ͣ��0.01�� */
	}
}

void Refresh_Widget(LCUI_Widget *widget)
/* ���ܣ�ˢ����ʾָ����������������ͼ�� */
{
	Record_WidgetUpdate(widget, NULL, DATATYPE_AREA);
}

void Resize_Widget(LCUI_Widget *widget, LCUI_Size new_size)
/* ���ܣ��ı䲿���ĳߴ� */
{
	if(widget == NULL) 
		return; 
		
	Record_WidgetUpdate(widget, &new_size, DATATYPE_SIZE);
}

void Draw_Widget(LCUI_Widget *widget)
/* ���ܣ����»��Ʋ��� */
{
	if(widget == NULL) 
		return; 
		
	Record_WidgetUpdate(widget, NULL, DATATYPE_GRAPH);
}


void Front_Widget(LCUI_Widget *widget)
/* ���ܣ���ָ����������ʾλ���ƶ�����ǰ�� */
{
	LCUI_Queue *queue;
	/* ��ȡָ����е�ָ�� */
	if(widget->parent == NULL)  
		queue = &LCUI_Sys.widget_list;
	else 
		queue = &widget->parent->child;
	
	/* ���ò����ƶ�������ǰ�� */
	WidgetQueue_Move(queue, 0, widget);
} 

void Show_Widget(LCUI_Widget *widget)
/* ���ܣ���ʾ���� */
{
	if(widget == NULL) 
		return;  
		
	Front_Widget(widget);/* �ı䲿��������λ�� */
	Record_WidgetUpdate(widget, NULL, DATATYPE_SHOW);
}

void Hide_Widget(LCUI_Widget *widget)
/* ���ܣ����ز��� */
{
	if(widget == NULL) 
		return; 
		
	Record_WidgetUpdate(widget, NULL, DATATYPE_HIDE); 
}

void Set_Widget_Status(LCUI_Widget *widget, int status)
/* ���ܣ��趨������״̬ */
{
	if(widget == NULL) 
		return; 
	Record_WidgetUpdate(widget, &status, DATATYPE_STATUS); 
}
/************************* Widget End *********************************/



/**************************** Widget Update ***************************/
typedef struct _WidgetData
{
	void	*data;	/* ��Ҫ���µ����� */
	int		type;	/* ������ݵ����� */
}
WidgetData;

static void Destroy_WidgetData(void *arg)
/* ���ܣ��ͷ�WidgetData�ṹ���е�ָ�� */
{
	WidgetData *wdata = (WidgetData*)arg;
	if(NULL == wdata) return;
	if(wdata->data != NULL)
		free(wdata->data);
}

static void WidgetData_Init(LCUI_Queue *queue)
/* ���ܣ���ʼ����¼�������ݸ��µĶ��� */
{
	Queue_Init(queue, sizeof(WidgetData), Destroy_WidgetData);
}

static int Find_WidgetData(LCUI_Widget *widget, const WidgetData *data)
/* ���ܣ���ѯָ�����������Ƿ��ڶ����� */
{
	WidgetData *temp;
	int i, total;
	total = Queue_Get_Total(&widget->data);	/* ��ȡ��Ա���� */
	for(i=0; i<total; ++i)
	{
		temp = (WidgetData*)Queue_Get(&widget->data, i);
		if(NULL == temp)
			break;
		if(temp->type == data->type) 
			return i; 
	}
	return -1;
}

static int Record_WidgetUpdate(LCUI_Widget *widget, void *data, int type)
/* 
 * ���ܣ���¼��Ҫ�������ݸ��µĲ���
 * ˵����������ָ���Լ�����µ�������������У����ݲ�������ʾ˳�������ж���
 * ����ֵ�����������򷵻�-1���������ز�С��0��ֵ
 *  */
{ 
	int pos, result = 0;
	size_t size = 0;
	WidgetData temp;
	/* �������ͣ�����֪ռ�ÿռ��С */
	switch(type)
	{
		case DATATYPE_POS	:	size = sizeof(LCUI_Pos);break;	/* λ�� */
		case DATATYPE_SIZE	:	size = sizeof(LCUI_Size);break;	/* �ߴ� */
		case DATATYPE_GRAPH	:	size = sizeof(LCUI_Graph);break;/* ͼ�� */
		case DATATYPE_STATUS:	size = sizeof(int);break; /* ״̬ */
		case DATATYPE_AREA:
		case DATATYPE_HIDE:
		case DATATYPE_SHOW:		break;
		default:				return -1;
	}
	
	temp.type = type;			/* �������� */
	if(data != NULL)
	{
		temp.data = malloc(size);	/* �����ڴ� */
		memcpy(temp.data, data, size);	/* �������� */ 
	}
	else temp.data = NULL;
	  
	pos = Find_WidgetData(widget, &temp);
	if(pos >= 0)	/* ����Ѿ����ڣ��͸��� */ 
		result = Queue_Replace(&widget->data, pos, &temp); 
	else	/* ����׷��������ĩβ */
		result = Queue_Add(&widget->data, &temp);
		
	return result;
}


int Processing_WidgetUpdate(LCUI_Widget *widget)
/* ���ܣ��������ĸ��� */
{
	LCUI_Widget *child;
	WidgetData *temp;
	LCUI_Pos pos;
	int i, total;
	/* ����������Ҫ���µ����� */
	//printf("Processing_WidgetUpdate(): enter\n");
	Queue_Lock(&widget->data);/* �������У������߳���ʱ���ܷ��� */
	while( ! Queue_Empty(&widget->data) )
	{
		temp = (WidgetData *)Queue_Get(&widget->data, 0);
		/* ���ݲ�ͬ�����������д��� */
		switch(temp->type)
		{
			case DATATYPE_SIZE	: /* �����ߴ���£������²�����λ�� */ 
				Exec_Resize_Widget(widget, *((LCUI_Size*)temp->data));
				Exec_Draw_Widget(widget);
				/* ��Ҫ����λ�ã����Բ���break */ 
			case DATATYPE_POS	:
				/*
				 * ���ڸ���λ�ÿ��ܻ��Ǹ��²����ߴ�ĸ��Ӳ�������Ҫ�ж�һ�¸�������
				 * �Ƿ�ΪDATATYPE_POS 
				 * */
				if( temp->type == DATATYPE_POS 
				 && widget->align == ALIGN_NONE 
				 && temp->data != NULL )
					pos = *((LCUI_Pos*)temp->data);
				else
					pos = Widget_Align_Get_Pos(widget); 
				
				Exec_Move_Widget(widget, pos);
				break;
			case DATATYPE_STATUS:
				widget->status = *(int*)temp->data;
				/* �ı䲿��״̬����Ҫ�����ػ棬���Բ���break */
			case DATATYPE_GRAPH	:
				Exec_Draw_Widget(widget); 
				break;
			case DATATYPE_HIDE:
				Exec_Hide_Widget(widget);
				break;
			case DATATYPE_SHOW: 
				Exec_Show_Widget(widget); 
				break;
			case DATATYPE_AREA:
				Exec_Refresh_Widget(widget);
				break;
			default: break;
		} 
		Queue_Delete(&widget->data, 0);/* �Ƴ��ó�Ա */
	}
	Queue_UnLock(&widget->data);
	total = Queue_Get_Total(&widget->child);  
	for(i=total-1; i>=0; --i)
	{
		/* ��β���׻�ȡ����ָ�� */
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);  
		if(child != NULL)
		{/* �ݹ���� */
			Processing_WidgetUpdate( child );
		}
	}
	//printf("Processing_WidgetUpdate(): quit\n");
	/* ���� */
	return 0;
}

void Processing_All_WidgetUpdate()
/* ���ܣ��������в����ĸ��� */
{
	LCUI_Widget *child;
	int i, total;
	total = Queue_Get_Total(&LCUI_Sys.widget_list); 
	//printf("Processing_All_WidgetUpdate():start\n");
	for(i=total-1; i>=0; --i)
	{
		/* ��β���׻�ȡ����ָ�� */
		child = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, i); 
		//printf("child: ");print_widget_info(child);
		if(child != NULL)
		{/* �ݹ���� */ 
			Processing_WidgetUpdate( child );
		}
	}
	//printf("Processing_All_WidgetUpdate():end\n");
}

/************************ Widget Update End ***************************/



/************************** Widget Library ****************************/

typedef struct _WidgetTypeData
{ 
	LCUI_String		type;		/* �������ͣ����ַ������� */
	LCUI_ID			type_id;	/* ����ID */
	LCUI_FuncQueue	func;		/* ������صĺ��� */
}WidgetTypeData;

int WidgetFunc_Add(
			char *type, 
			void (*widget_func)(LCUI_Widget*), 
			FuncType func_type
		)
/*
 * ���ܣ�Ϊָ�����͵Ĳ������������͵ĺ���
 * ����ֵ���������Ͳ����ڣ�����-1���������󷵻�-2
 **/
{
	WidgetTypeData *temp;
	LCUI_Func *temp_func, func_data;
	int total, i, found = 0;
	
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) 
	{
		printf("WidgetFunc_Add():"APP_ERROR_UNRECORDED_APP);
		exit(-1);
	}
	
	//printf("WidgetFunc_Add(): widget type: %s, func type: %d\n", type, func_type); 
	
	total = Queue_Get_Total(&app->widget_lib); 
		/* �������ݣ��ҵ���Ӧ��λ�� */
	for(i = 0; i < total; ++i)
	{
		temp = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(strcmp(temp->type.string, type) == 0)
		{ 
			total = Queue_Get_Total(&temp->func); 
			for(i=0; i<total; i++)
			{
				temp_func = Queue_Get(&temp->func, i);
				if(temp_func->id == func_type)
				{
					found = 1;
					break;
				}
			}
			
			Get_FuncData(&func_data, widget_func, NULL, NULL);
			func_data.id = func_type; /* ��������ID */
			if(found == 1) 
			{/* ����Ѿ����ڣ��͸��� */
				//printf("WidgetFunc_Add(): the function is already registered. repalce\n");
				Queue_Replace(&temp->func, i, &func_data); 
			}
			else
				Queue_Add(&temp->func, &func_data); 
			return 0;
		}
	}
	//printf("WidgetFunc_Add(): warning: the widget type was never recorded\n");
	return -1;
}

int WidgetType_Add(char *type)
/*
 * ���ܣ����һ���µĲ���������������
 * ����ֵ�������ӵ��²��������Ѵ��ڣ�����-1���ɹ��򷵻�0
 **/
{
	WidgetTypeData *wd, new_wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) 
	{
		printf("WidgetType_Add():"APP_ERROR_UNRECORDED_APP);
		exit(-1);
	}
	
	int total, i; 
	//printf("WidgetType_Add(): add widget type: %s\n", type);
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i)
	{
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, type) == 0)
		{ /* �������һ�� */
			//printf("WidgetType_Add(): the widget type is already registered\n");
			return -1;
		}
	} 
	
	/* ������Ҫ��ʼ�� */
	FuncQueue_Init(&new_wd.func);
	new_wd.type_id = rand()%100000; /* ���������Ϊ����ID */
	//printf("WidgetType_Add(): widget type id: %ld\n", new_wd.type_id); 
	String_Init(&new_wd.type);
	Strcpy(&new_wd.type, type);/* ���沿������ */
	Queue_Add(&app->widget_lib, &new_wd);/* ��������� */
	return 0;
}

static void Destroy_WidgetType(void *arg)
/* ���ܣ��Ƴ������������� */
{
	WidgetTypeData *wd = (WidgetTypeData*)arg;
	Destroy_Queue(&wd->func);
}

void WidgetLib_Init(LCUI_Queue *w_lib)
/* ���ܣ���ʼ�������� */
{
	Queue_Init(w_lib, sizeof(WidgetTypeData), Destroy_WidgetType);
}

int WidgetType_Delete(char *type)
/*
 * ���ܣ�ɾ��ָ���������͵��������
 **/
{
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	
	if(app == NULL) 
		return -2;
	
	int total,  i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i)
	{
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, type) == 0) /* �������һ�� */
			return Queue_Delete(&app->widget_lib, i);
	} 
	
	return -1;
}

void NULL_Widget_Func(LCUI_Widget *widget)
/*
 * ���ܣ��պ����������κβ���
 * ˵���������ȡָ���������͵ĺ���ָ��ʧ�ܣ���������������ĺ���ָ��
 **/
{
	
}

LCUI_ID WidgetType_Get_ID(char *widget_type)
/*
 * ���ܣ���ȡָ�����Ͳ���������ID
 **/
{ 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) return -2;
	
	int total, i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i)
	{
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, widget_type) == 0)
		{ /* �������һ�� */
			return wd->type_id;
		}
	}
	
	return -1;
}



int Get_Widget_Type_By_ID(LCUI_ID id, char *widget_type)
/*
 * ���ܣ���ȡָ������ID����������
 **/
{
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) return -2;
	
	int total, i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i)
	{
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(wd->type_id == id)
		{ /* �������һ�� */
			strcpy(widget_type, wd->type.string); 
			return 0;
		}
	}
	
	return -1;
}

void ( *Get_WidgetFunc_By_ID(LCUI_ID id, FuncType func_type) ) (LCUI_Widget*)
/*
 * ���ܣ���ȡָ����������ID�ĺ����ĺ���ָ��
 **/
{
	LCUI_Func *f = NULL; 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) 
		return NULL_Widget_Func;
	
	int total, i, found = 0; 
	//printf("Get_WidgetFunc_By_ID(): widget type id: %lu, func type: %d\n", id, func_type);
	total = Queue_Get_Total(&app->widget_lib); 
	for(i = 0; i < total; ++i)
	{
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(wd->type_id == id)
		{ /* �������һ�� */  
			total = Queue_Get_Total(&wd->func); 
			for(i=0; i<total; i++)
			{
				f = Queue_Get(&wd->func, i); 
				if(f->id == func_type)
				{
					found = 1;
					break;
				}
			}
			
			if(found == 1) /* ����Ѿ����� */
				return f->func; 
			else 
			{
				//printf("Get_WidgetFunc_By_ID(): warning: widget func not found!\n");
				return NULL_Widget_Func;
			}
		}
	}
	
	return NULL_Widget_Func;
}

void ( *Get_WidgetFunc(char *widget_type, FuncType func_type) ) (LCUI_Widget*)
/* ���ܣ���ȡָ�����Ͳ����ĺ����ĺ���ָ�� */
{
	LCUI_Func *f = NULL; 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL)
		return NULL_Widget_Func;
	
	int total, i, found = 0; 
	
	total = Queue_Get_Total(&app->widget_lib);
	//printf("Get_WidgetFunc(): widget type: %s, func type: %d\n", widget_type, func_type);
	for(i = 0; i < total; ++i)
	{
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, widget_type) == 0)
		{ /* �������һ�� */ 
			total = Queue_Get_Total(&wd->func);
			for(i=0; i<total; i++)
			{
				f = Queue_Get(&wd->func, i);
				if(f->id == func_type)
				{
					found = 1;
					break;
				}
			}
			
			if(found == 1) /* ����Ѿ����� */
				return f->func; 
			else 
			{
				//printf("Get_WidgetFunc(): warning: widget func not found!\n");
				return NULL_Widget_Func; 
			}
		}
	}
	
	return NULL_Widget_Func;
}

int Check_WidgetType(char *widget_type)
/*
 * ���ܣ����ָ�����������Ƿ���Ч
 **/
{ 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	
	if(app == NULL) 
		return 0;
	
	int total, i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i)
	{
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, widget_type) == 0)/* �������һ�� */ 
			return 1; 
	}
	
	return 0;
}


extern void Register_Window();
extern void Register_Label();
extern void Register_Button();
extern void Register_PictureBox();
extern void Register_ProgressBar();
extern void Register_Menu();
extern void Register_CheckBox();
extern void Register_RadioButton();

void Register_Default_Widget_Type()
/* ���ܣ�Ϊ����Ĳ��������Ĭ�ϵĲ������� */
{
	WidgetType_Add(NULL); /* ���һ��NULL���͵Ĳ��� */
	Register_Window();/* ע�ᴰ�ڲ��� */
	Register_Label();/* ע���ı���ǩ���� */
	Register_Button();/* ע�ᰴť���� */
	Register_PictureBox();/* ע��ͼƬ���Ӳ��� */
	Register_ProgressBar();
	Register_Menu();
	Register_CheckBox();
	Register_RadioButton();
}
/************************ Widget Library End **************************/


/*************************** Container ********************************/
void Widget_Container_Add(LCUI_Widget *container, LCUI_Widget *widget)
/* ���ܣ��������������Ϊ�����Ĳ����� */
{
	int pos;
	LCUI_Queue *old_queue, *new_queue;
	
	if(container == widget->parent) 
		return;
		
	if(container == NULL)
	{/* �������ΪNULL���Ǿʹ���������Ļ��Ϊ���� */
		new_queue = &LCUI_Sys.widget_list;
		/* �ı䲿����λ�����ͣ����ĵĻ����ó����Ĳ���λ�û������� */
		widget->pos_type = POS_TYPE_IN_SCREEN;
	}
	else
	{/* ������ָ������Ϊ���� */ 
		new_queue = &container->child;
		widget->pos_type = POS_TYPE_IN_WIDGET; 
	}
	
	if(widget->parent != NULL)
	/* ��������и��������Ǿ��ڸ��������Ӳ��������� */ 
		old_queue = &widget->parent->child; 
	else
	/* ����û�в�������ô��������ڴ���ʱ�ʹ�����ϵͳ���������� */
		old_queue = &LCUI_Sys.widget_list;
		
	/* �ı�ò�������������Ҫ������֮ǰ���������Ƴ� */
	pos = WidgetQueue_Get_Pos(old_queue, widget);
	if(pos >= 0)
		Queue_Delete_Pointer(old_queue, pos);
	
	widget->parent = container; /* ���游����ָ�� */ 
	Queue_Add_Pointer(new_queue, widget); /* ������������� */
	Update_Widget_Pos(widget);
}
/************************* Container End ******************************/

