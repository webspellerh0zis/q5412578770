/* ***************************************************************************
 * LCUI_Work.c -- LCUI's other work
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
 * LCUI_Work.c -- LCUI ����������
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

#include <unistd.h>
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WORK_H 
#include LC_MISC_H
#include LC_ERROR_H
#include LC_INPUT_H
#include LC_WIDGET_H

/***************************** Func ***********************************/
void NULL_Func()
/* ���ܣ��պ����������κβ��� */
{
	return;
}

void FuncQueue_Init(LCUI_Queue *queue)
/* ���ܣ���ʼ������ָ����� */
{
	Queue_Init(queue, sizeof(LCUI_Func), NULL);
}

/****************************** Task **********************************/

void Send_Task_To_App(LCUI_Func *func_data)
/*
 * ���ܣ��������������ʹ����������ָ������
 * ˵����LCUI_Func�ṹ���еĳ�Ա���� id���������Ŀ������id
 */
{ 
	LCUI_App *app = Find_App(func_data->id);
	if(app == NULL) 
		return;
		
	Queue_Add(&app->task_queue, func_data);
}


int Have_Task(LCUI_App *app)
/* ���ܣ�����Ƿ������� */
{
	if(app == NULL) 
		return 0; 
	if(Queue_Get_Total(&app->task_queue) > 0)
		return 1; 
		
	return 0;
}

int Run_Task(LCUI_App *app)
/* ���ܣ�ִ������ */
{ 
	LCUI_Task *task;
	task = (LCUI_Task*)Queue_Get(&app->task_queue, 0);
	/* ���ú���ָ��ָ��ĺ����������ݲ��� */
	task->func(task->arg[0], task->arg[1]);
	
	return Queue_Delete(&app->task_queue, 0);
}


int AppTask_Custom_Add(int mode, LCUI_Func *func_data)
/*
 * ���ܣ�ʹ���Զ��巽ʽ��ӳ�������
 * �÷�ʾ����
 * �ں����ĸ�����������еĺ��������������ظ�ʱ�������
 * AppTask_Custom_Add(ADD_MODE_NOT_REPEAT | AND_ARG_F | AND_ARG_S, func_data);
 * ֻҪ�����Ͳ���1���ظ������
 * AppTask_Custom_Add(ADD_MODE_NOT_REPEAT | AND_ARG_F, func_data);
 * Ҫ�������ظ������
 * AppTask_Custom_Add(ADD_MODE_NOT_REPEAT, func_data);
 * ����µģ������Ƿ����ظ���
 * AppTask_Custom_Add(ADD_MODE_ADD_NEW, func_data);
 * ����ͬ�����򸲸ǣ�û��������
 * AppTask_Custom_Add(ADD_MODE_REPLACE, func_data);
 * */
{
	int total, i;
	/* �Ȼ�ȡ�������ݽṹ��ָ�� */
	LCUI_App *app = Get_Self_AppPointer();
	LCUI_FuncQueue *queue =& app->task_queue;
	
	LCUI_Func *temp = NULL;
	
	total = Queue_Get_Total(queue);
	if(mode != ADD_MODE_ADD_NEW)
	{/* ���ģʽ���ǡ�����µġ�ģʽ */
		//printf("mode: %d\n", mode);
		for (i = 0; i < total; ++i)
		{
			//printf("1\n");
			temp = Queue_Get(queue, i);
			if(temp->func == func_data->func)
			{/* �������ָ�����м�¼ */
			//printf("2\n");
				if(Check_Option(mode, ADD_MODE_NOT_REPEAT))
				{/* ���Ҫ����ǲ��ظ�ģʽ */ 
		//	printf("3\n");
					if(Check_Option(mode, AND_ARG_F))
					{/* ���Ҫ���ǵ�1�����������ظ� */
						//printf("ADD_MODE_NOT_REPEAT, AND_ARG_F\n");
						//printf("old:%p, new:%p\n", queue->queue[i].arg_f, arg_f);
						if(Check_Option(mode, AND_ARG_S))
						{/* ���Ҫ���ǵ�2������Ҳ�����ظ� */
							if(temp->arg[0] == func_data->arg[0] 
							&& temp->arg[1] == func_data->arg[1] 
							)/* ���ʵ���Ϻ����Լ�����1��2��һ�� */ 
								return -1; 
						}
						else
						{/* ����ֻ��Ҫ�����Լ���1����������ȫ���ظ� */
							if(temp->arg[0] == func_data->arg[0])  
								return -1; 
						}
					}
					else if(Check_Option(mode, AND_ARG_S))
					{/* �������ֻ��Ҫ���ǵ�2�����������ظ� */
						if(temp->arg[1] == func_data->arg[1] )  
							return -1; 
					}
					else /* ����ֻ��Ҫ������ͬ */ 
						return -1; 
				}
				else if(Check_Option(mode, ADD_MODE_REPLACE))
				{/* ���Ҫ������滻ģʽ */
					//printf("ADD_MODE_REPLACE\n");
					if(Check_Option(mode, AND_ARG_F))
					{/* ���Ҫ���ǵ�1��������ͬ */
						if(Check_Option(mode, AND_ARG_S))
						{/* ���Ҫ���ǵ�2������Ҳ��ͬ */
							if(temp->arg[0] == func_data->arg[0] 
							&& temp->arg[1] == func_data->arg[1]
							)
								break; 
						}
						else
						{/* ����ֻ��Ҫ�����Լ���1������ȫ����ͬ */
							if(temp->arg[0] == func_data->arg[0])
							{
						//		printf("ARG_F\n");
								break; 
							}
						}
					}
					else if(Check_Option(mode, AND_ARG_S))
					{/* �������ֻ��Ҫ���2������������ͬ */
						if(temp->arg[1] == func_data->arg[1]) 
							break; 
					}
					else /* ����ֻ��Ҫ������ͬ */ 
						break; 
				}
		//	printf("4\n");
			}
		}
		//printf("5\n");
		if(i == total)
			Queue_Add(queue, func_data); 
		else 
			Queue_Replace( queue, i, func_data ); 
	}
	else Queue_Add(queue, func_data); 
	//		printf("6\n");
	return 0;
}
/**************************** Task End ********************************/




/***************************** Event ***********************************/
static void Destroy_Event(void *arg)
/* ���ܣ������¼����� */
{
	LCUI_Event *event = (LCUI_Event*)arg;
	if(event != NULL) 
		Destroy_Queue(&event->func_data); 
}

void EventQueue_Init(LCUI_EventQueue * queue)
/* ���ܣ���ʼ���¼����� */
{
	/* 
	 * ����LCUI_Event�ṹ��ĳ�Ա���к������У������¼�ʱ��Ҫ�Ѹö������٣�������
	 * ҪDestroy_Event()��Ϊ�������� 
	 * */
	Queue_Init(queue, sizeof(LCUI_Event), Destroy_Event);
}


int Get_FuncData(LCUI_Func *p, void (*func) (), void *arg1, void *arg2)
/* 
 * ���ܣ�������ָ���Լ�����������ת����LCUI_Func���͵�ָ��
 * ˵�����˺����������ڴ�ռ䣬������ָ��ÿռ��ָ��
 *  */
{
	LCUI_App *app;
	app = Get_Self_AppPointer();
	
	if(NULL == app)
	{
		printf("Get_FuncData(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	}
	
	p->id = app->id;
	p->func = func;
	/* 
	 * ֻ�Ǳ�����ָ�������ָ�룬����ò����Ǿֲ����������������ĺ����˳��󣬸ñ���
	 * �Ŀռ���ܻ��޷����ʡ�
	 *  */
	p->arg[0] = arg1;	
	p->arg[1] = arg2;
	
	return 0;
}

LCUI_Event *Find_Event(LCUI_EventQueue *queue, int event_id)
/* ���ܣ������¼���ID����ȡָ����¼���ָ�� */
{
	LCUI_Event *event; 
	int i, total;  
	total = Queue_Get_Total(queue);
	if (total > 0)
	{
		for (i = 0; i < total; ++i)
		{
			event = (LCUI_Event*)Queue_Get(queue, i);
			if(event->id == event_id)
				return event;
		}
	}
	
	return NULL;
}

int EventQueue_Add(LCUI_EventQueue *queue, int event_id, LCUI_Func *func)
/* ���ܣ���¼�¼�����Ӧ�ص����������� */
{
	LCUI_Event *event;
	
	event = Find_Event(queue, event_id);
	if (NULL == event)
	{/* ���û�У������һ�����¼����� */ 
		int pos;
		LCUI_Event new_event;
		new_event.id = event_id;
		Queue_Init(&new_event.func_data, sizeof(LCUI_Func), NULL);
		pos = Queue_Add(queue, &new_event);/* ��������׷�������� */
		event = Queue_Get(queue, pos);	/* ��ȡָ��������Ա��ָ�� */
	}
	
	event->id = event_id; /* �����¼�ID */
	Queue_Add(&event->func_data, func);
	return 0;
}

int LCUI_MouseEvent_Connect (void (*func) (), int event_id)
/* 
 * ���ܣ�����������������¼������
 * ˵����������¼������󣬻��Ƚ��ѹ������¼��ĺ���ָ�뼰����¼���ָ��
 * ����������������У��ȴ���������ѭ���д���
 **/
{
	LCUI_Func func_data;
	if(0 != Get_FuncData(&func_data, func, NULL, NULL) ) 
		return -1;
	/* 
	 * ������ָ�뼰��һ����������������¼��������
	 * �У��ȵ���������¼�ʱ���Ὣ����״̬��Ϊ�ú�
	 * ���ĵڶ���������ת���������������� 
	 **/ 
	EventQueue_Add(&LCUI_Sys.mouse.event, event_id, &func_data); 
	return 0;
}


int LCUI_Key_Event_Connect (int key_value, void (*func) (), void *arg)
/* ���ܣ��������밴����ĳ���¼������ӣ�������������º󣬾ͻ����������� */
{
	LCUI_Func func_data;
	LCUI_App *app = Get_Self_AppPointer();
	if(NULL == app)
	{
		printf("LCUI_Key_Event_Connect(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	}
	
	Get_FuncData(&func_data, func, arg, NULL);
	return EventQueue_Add(&LCUI_Sys.key_event, key_value, &func_data);
}


int Processing_Event(LCUI_EventQueue *queue, int event_id)
/* 
 * ���ܣ�����ָ��ID���¼�
 * ˵�����������Ὣ�¼���������ָ��ID���¼������Ļص����� �����������������
 * */
{
	LCUI_Event *event;
	LCUI_Func *func;
	int total, i;
	event = Find_Event(queue, event_id);
	if(NULL == event)
		return -1;
		
	total = Queue_Get_Total(&event->func_data);
	for (i = 0; i < total; ++i)
	{
		func = Queue_Get(&event->func_data, i);
		/* ����������������� */ 
		AppTask_Custom_Add(ADD_MODE_ADD_NEW, func);
	}
	return 0;
}


/****************** �������϶�/����¼�����ش��� ************************/
static LCUI_Widget *click_widget = NULL, *overlay_widget = NULL;
static LCUI_Pos __offset_pos = {0, 0};  /* �������ʱ�����ƫ������ */ 
static LCUI_DragEvent drag_event;
int Widget_Drag_Event_Connect ( 
				LCUI_Widget *widget, 
				void (*func)(LCUI_Widget*, LCUI_DragEvent *)
)
/* 
 * ���ܣ����ص������벿�����϶��¼��������� 
 * ˵�����������Ӻ󣬵�������������϶����ͷţ�������ûص�����
 * */
{
	LCUI_DragEvent *p;
	LCUI_Func func_data;
	if(widget == NULL) return -1;
	p = &drag_event;
	if(0 != Get_FuncData(&func_data, func, (void*)widget, p))
		return -1;
		
	EventQueue_Add(&widget->event, EVENT_DRAG, &func_data);
	return 0;
}

int Widget_Clicked_Event_Connect (
			LCUI_Widget *widget,
			void (*func)(LCUI_Widget*, void *), 
			void *arg
)
/* 
 * ���ܣ����ص������벿�����϶��¼��������� 
 * ˵�����������Ӻ󣬲����Ὣ��λ����Ϊ�ڶ������������ص�����
 * */
{
	LCUI_Func func_data;
	if(widget == NULL) return -1;
	if(0 != Get_FuncData(&func_data, func, (void*)widget, arg))
		return -1;
		
	EventQueue_Add(&widget->event, EVENT_CLICKED, &func_data);
	return 0;
}

static LCUI_Widget *Widget_Find_Response_Status_Change(LCUI_Widget *widget)
/* 
 * ���ܣ���������Ӧ״̬�ı�Ĳ��� 
 * ˵�����˺������ڼ�鲿���Լ������ϼ����и���������һ������Ӧ״̬�ı�Ĳ�����ָ�뽫��
 * ��Ϊ�������ķ���ֵ��
 * */
{
	if(NULL == widget) 
		return NULL;
	if(widget->response_flag == 1)
		return widget;/* ���������Ӧ״̬�ı䣬�Ǿͷ��ظò�����ָ�� */
		
	if(widget->parent == NULL)
		return NULL; /* ���������Ϊ�գ��Ǿ�û�ҵ�������NULL */
	else /* ���������ĸ����������� */
		return Widget_Find_Response_Status_Change(widget->parent); 
}

static LCUI_Widget *Widget_Find_Response_Event(LCUI_Widget *widget, int event_id)
/* 
 * ���ܣ���������Ӧ�¼��Ĳ��� 
 * ˵�����˺������ڼ�鲿���Լ������ϼ����и���������һ������Ӧָ���¼��Ĳ���������ָ��
 * ������Ϊ�������ķ���ֵ
 * */
{ 
	LCUI_Event *event;
	if(NULL == widget) 
		return NULL;
		
	event = Find_Event(&widget->event, event_id);
	if(event != NULL)
		return widget;/* ���������Ӧ״̬�ı䣬�Ǿͷ��ظò�����ָ�� */
	 
	if(widget->parent == NULL)
		return NULL; /* ���������Ϊ�գ��Ǿ�û�ҵ�������NULL */
	else /* ���������ĸ����������� */
		return Widget_Find_Response_Event(widget->parent, event_id); 
}

static void Widget_Clicked(LCUI_MouseEvent *event)
/*
 * ���ܣ����ڴ���click�¼��������汻�����Ĳ�����ָ��
 * ˵������������������/�ͷ�ʱ����������������
 **/
{
	LCUI_Widget *widget; 
	//printf("Widget_Clicked() :start!!\n"); 
	if(event == NULL) return;
	
	widget = event->widget;  
	//printf("event widget: ");
	//print_widget_info(widget);

	widget = Widget_Find_Response_Event(widget, EVENT_CLICKED);
	if(NULL == widget)  
		widget = Widget_Find_Response_Event(event->widget, EVENT_DRAG);  
	//printf("widget :");
	//print_widget_info(widget);
	if( Mouse_LeftButton(event) == PRESSED )
	{/* ����������������� */ 
		/* widget����ָ������Ӧ״̬�仯�Ĳ�������ˣ���Ҫ���� */
		click_widget = widget; 
		if(widget != NULL)
		{
			//printf("1\n");
			/* ��ȫ�������ȥ���������꣬�õ�ƫ��λ�� */ 
			__offset_pos = Pos_Sub(
				event->global_pos, 
				Get_Widget_Global_Pos( widget )
			);
			/* �ó���������λ�� */
			drag_event.new_pos = Pos_Sub(event->global_pos, __offset_pos); 
			drag_event.first_click = 1; 
			drag_event.end_click = 0;
			/* ���������϶��¼� */
			Processing_Event(&widget->event, EVENT_DRAG); 
		}
			//printf("2\n");
		widget = Widget_Find_Response_Status_Change(widget); 
		if(widget != NULL)
		{
			//printf("3\n");
			if( event->widget->enabled == IS_TRUE
			&& widget->enabled == IS_TRUE )
			/* �����ǰ���ָ�븲�ǵ��Ĳ����ѱ����� */  
				Set_Widget_Status (widget, WIDGET_STATUS_CLICKING); 
			else 
				Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
		}
			//printf("4\n");
	}
	else if (Mouse_LeftButton (event) == FREE)
	{/* ����������������ͷ�״̬ */
	//printf("5\n");
		if(click_widget != NULL)
		{ 
			__offset_pos = Pos_Sub(
				event->global_pos, 
				Get_Widget_Global_Pos( click_widget )
			); 
			drag_event.new_pos = Pos_Sub(event->global_pos, __offset_pos); 
			drag_event.first_click = 0; 
			drag_event.end_click = 1; 
			Processing_Event(&click_widget->event, EVENT_DRAG);
			
			if(click_widget == widget)
			{/* ������ʱ�͵������ͬһ����ť�����ڽ��е� */
				/* 
				 * ����CLICKED�¼����������й������¼��Ļص�����������
				 * ������У�ʹ֮����ѭ����ִ�� 
				 * */  
				//printf("send clicked event\n"); 
		//printf("6\n");
				if(widget->enabled == IS_TRUE)
					Processing_Event(&widget->event, EVENT_CLICKED);
				
				widget = Widget_Find_Response_Status_Change(event->widget);
				if(NULL != widget){
				if(widget->enabled == IS_TRUE)
				{ 
					Set_Widget_Status (widget, WIDGET_STATUS_CLICKED);
					Set_Widget_Status (widget, WIDGET_STATUS_OVERLAY);
				} else 
					Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
				}
		//printf("7\n");
			}
			else
			{/* ���򣬽��ָ�֮ǰ���������״̬ */
		//printf("8\n");
				widget = Widget_Find_Response_Status_Change(click_widget);
				if(widget != NULL)
				{
					if(widget->enabled == IS_TRUE)
						Set_Widget_Status (widget, WIDGET_STATUS_NORMAL);
					else
						Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
				}
			} 
		}
		click_widget = NULL; /* ָ�븳ֵΪ�� */
	} 
	//printf("Widget_Clicked() :end!!\n");  
}

static void Tracking_Mouse_Move (LCUI_MouseEvent *event)
/* 
 * ���ܣ���������ƶ����������Ļ����¼�
 * ˵������ֻ�Ǹ�������¼���������״̬���л�
 * */
{
	//printf("Tracking_Mouse_Move(): start\n");
	LCUI_Widget *widget;
	/* ��ȡ��ǰ����α긲�ǵ��Ĳ�����ָ�� */
	widget = Get_Cursor_Overlay_Widget(); 
	//print_widget_info(widget);
	//print_widget_info(overlay_widget);
	if(widget != NULL)
	{	//printf("6 ");
		/* ��ȡ����Ӧ״̬�ı�Ĳ�����ָ�� */
		widget = Widget_Find_Response_Status_Change(widget); 
		if(widget != NULL)
		{/* �����Ч */
		//printf("1\n");
			if(overlay_widget != widget)
			{
		//printf("2\n");
				if (widget->enabled == IS_TRUE)
				{
		//printf("3\n");
		//printf("leftbutton status: %d\n", Mouse_LeftButton (event));
					if( click_widget == NULL )
					{
		//printf("leftbutton is free\n\n");
						Set_Widget_Status (widget, WIDGET_STATUS_OVERLAY);
					} 
				}
				else
					Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
				
		//printf("4\n");
				if (overlay_widget != NULL && click_widget == NULL )
				{/* ���֮ǰ�и��ǵ��Ĳ��� */
		//printf("5\n");
					if (overlay_widget->enabled == IS_TRUE) /* �����ť���������� */
						Set_Widget_Status (overlay_widget, WIDGET_STATUS_NORMAL);
					else
						Set_Widget_Status (overlay_widget, WIDGET_STATUS_DISABLE);
				}
				/* ���浱ǰ���ǵ��İ�ť����ָ�� */
				overlay_widget = widget;
				return;
			}
		}
	}
	//printf("6\n");
	if(overlay_widget != widget && click_widget == NULL)
	{/* ������ָ����֮ǰ�и��ǵ��Ĳ��� */
	//printf("2\n");
	//printf("7\n");
		if (overlay_widget->enabled == IS_TRUE)/* ����������ã��������ָ�����ͨ״̬ */
			Set_Widget_Status (overlay_widget, WIDGET_STATUS_NORMAL); 
		else /* ���򣬲���Ϊ�����õ�״̬ */
			Set_Widget_Status (overlay_widget, WIDGET_STATUS_DISABLE);
	}
	//printf("8\n");
	overlay_widget = widget;
	/* ���������϶��¼� */ 
	if(click_widget != NULL && Mouse_LeftButton (event) == PRESSED)
	{/* ���֮ǰ������������������������������ڰ���״̬ */ 
		drag_event.new_pos = Pos_Sub(event->global_pos, __offset_pos); 
		Processing_Event(&click_widget->event, EVENT_DRAG);
		drag_event.first_click = 0; 
		drag_event.end_click = 0; 
		/* ���������϶��¼� */
		Processing_Event(&click_widget->event, EVENT_DRAG);
	}
	//printf("Tracking_Mouse_Move(): end\n");
}


void Widget_Event_Init()
/* ���ܣ���ʼ�������¼����� */
{
	LCUI_MouseEvent_Connect(Tracking_Mouse_Move, MOUSE_EVENT_MOVE);
	LCUI_MouseEvent_Connect(Widget_Clicked, MOUSE_EVENT_CLICK);
}

/*************************** Event End *********************************/

