/* ***************************************************************************
 * LCUI_Work.h -- LCUI's other work
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
 * LCUI_Work.h -- LCUI ����������
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

#ifndef __LCUI_WORK_H__
#define __LCUI_WORK_H__

#define RELATIVE_POS	1 /* ������� */
#define NORMAL_POS		0 /*��ͨ���� */

typedef LCUI_Queue LCUI_EventQueue;
typedef LCUI_Queue LCUI_FuncQueue;
typedef LCUI_Queue LCUI_TaskQueue;

typedef struct _LCUI_Event		LCUI_Event;
typedef struct _LCUI_Func		LCUI_Func;
typedef struct _LCUI_Func		LCUI_Task;
typedef struct _LCUI_DragEvent	LCUI_DragEvent;

/************ �����¼� ************/
typedef enum _WidgetEvent_ID
{
	EVENT_DRAG,		/* �������϶��¼� */
	EVENT_CLICKED	/* �����ĵ���¼� */
}WidgetEvent_ID;
/*********************************/

/*************** �����϶��¼� ********************/
struct _LCUI_DragEvent
{
	LCUI_Pos new_pos;	/* ��λ�� */
	int first_click;	/* ��־���Ƿ�Ϊ�״ε�� */ 
	int end_click;		/* ��־���Ƿ��Ѿ������϶� */
};
/**********************************************/

/************************* ���� *****************************/
struct _LCUI_Func
{
	/* 
	 * ����ID����������Ҫ��ID��ʶ�������ͣ������������������
	 * ����Ҳ��Ҫ��ID����ʶĿ�����ID 
	 * */
	int id;	
	
	void (*func)();  /* ����ָ�� */
	
	/* ���²�������ô�����ص�����������Ҫ������δ����¼��� */  
	void *arg[2];   /* ������������������ */
};
/************************************************************/

/*************************** �¼� ******************************/
struct _LCUI_Event
{
	int			id;			/* ��¼�¼�ID */
	LCUI_Queue	func_data;	/* ��¼�������ĺ��������Ĳ��� */
};
/**************************************************************/

/***************************** Func ***********************************/
void NULL_Func();
/* ���ܣ��պ����������κβ��� */ 

void FuncQueue_Init(LCUI_Queue *queue);
/* ���ܣ���ʼ������ָ����� */ 

/****************************** Task **********************************/

void Send_Task_To_App(LCUI_Func *func_data);
/*
 * ���ܣ��������������ʹ����������ָ������
 * ˵����LCUI_Func�ṹ���еĳ�Ա���� id���������Ŀ������id
 */ 

int Have_Task(LCUI_App *app);
/* ���ܣ�����Ƿ������� */ 

int Run_Task(LCUI_App *app);
/* ���ܣ�ִ������ */ 

int AppTask_Custom_Add(int mode, LCUI_Func *func_data);
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
/**************************** Task End ********************************/


/***************************** Event ***********************************/ 
void EventQueue_Init(LCUI_EventQueue * queue);
/* ���ܣ���ʼ���¼����� */ 

int Get_FuncData(LCUI_Func *p, void (*func) (), void *arg1, void *arg2);
/* 
 * ���ܣ�������ָ���Լ�����������ת����LCUI_Func���͵�ָ��
 * ˵�����˺����������ڴ�ռ䣬������ָ��ÿռ��ָ��
 * */ 

LCUI_Event *Find_Event(LCUI_EventQueue *queue, int event_id);
/* ���ܣ������¼���ID����ȡָ����¼���ָ�� */ 

int EventQueue_Add(LCUI_EventQueue *queue, int event_id, LCUI_Func *func);
/* ���ܣ���¼�¼�����Ӧ�ص����������� */ 

int LCUI_MouseEvent_Connect (void (*func) (), int event_id);
/* 
 * ���ܣ�����������������¼������
 * ˵����������¼������󣬻��Ƚ��ѹ������¼��ĺ���ָ�뼰����¼���ָ��
 * ����������������У��ȴ���������ѭ���д���
 **/ 

int LCUI_Key_Event_Connect (int key_value, void (*func) (), void *arg);
/* ���ܣ��������밴����ĳ���¼������ӣ�������������º󣬾ͻ����������� */ 

int Processing_Event(LCUI_EventQueue *queue, int event_id);
/* 
 * ���ܣ�����ָ��ID���¼�
 * ˵�����������Ὣ�¼���������ָ��ID���¼������Ļص����� �����������������
 * */ 

/********************* �������϶�/����¼� ******************************/
int Widget_Drag_Event_Connect ( 
				LCUI_Widget *widget, 
				void (*func)(LCUI_Widget*, LCUI_DragEvent *)
);
/* 
 * ���ܣ����ص������벿�����϶��¼��������� 
 * ˵�����������Ӻ󣬵�������������϶����ͷţ�������ûص�����
 * */ 

int Widget_Clicked_Event_Connect (
			LCUI_Widget *widget,
			void (*func)(LCUI_Widget*, void *), 
			void *arg
);
/* 
 * ���ܣ����ص������벿�����϶��¼��������� 
 * ˵�����������Ӻ󣬲����Ὣ��λ����Ϊ�ڶ������������ص�����
 * */ 

void Widget_Event_Init();
/* ���ܣ���ʼ�������¼����� */ 

/*************************** Event End *********************************/


#endif

