/* ***************************************************************************
 * LCUI_Queue.h -- basic queue processing
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
 * LCUI_Queue.h -- �����Ķ��д���
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
#ifndef __LCUI_QUEUE_H__
#define __LCUI_QUEUE_H__
#include LC_THREAD_H
/********* ���е�ʹ��ģʽ *******/
#define QUEUE_MODE_FREE		0
#define QUEUE_MODE_READ		1
#define QUEUE_MODE_WRITE	1<<1
#define QUEUE_MODE_LOCK		1<<2
/*****************************/

#define MAX_TID		50

typedef struct _LCUI_Queue LCUI_Queue;

struct _LCUI_Queue
{ 
	thread_rwlock lock;/* ��д�� */
	int member_type;	/* ��Ա���� */ 
	
	void **queue;		/* ��¼���г�Ա */
	size_t element_size;	/* ��Ա��ռ�õ��ڴ�ռ��С����λΪ�ֽ� */
	int total_num;		/* ��¼���г�Ա���� */
	int max_num;		/* �������� */
	void (*destroy_func) (void*); /* ���������������ٶ��г�Աʱ����������������� */
};
 
/************************ LCUI_Queue **********************************/
int Queue_Lock (LCUI_Queue *queue);
/* ���ܣ�����һ�����У�ʹֻ֮�ܱ�һ���߳�ʹ�� */ 

int Queue_UnLock (LCUI_Queue *queue);
/* ���ܣ��⿪������ */ 

int Queue_Using (LCUI_Queue * queue, int mode) ;
/* 
 * ���ܣ��趨���е�״̬Ϊ��ʹ�á� 
 * ˵��������mode��Ҫ��QUEUE_MODE_READ��QUEUE_MODE_WRITE������֮һ��
 * QUEUE_MODE_READ ��ʾ������ģʽ�������Ƕ������Ժ������ԡ�����ģʽ���ʱ����е��̹߳���
 * QUEUE_MODE_WRITE ��ʾ��д��ģʽ��дֻ����һ���߳�д�������̼߳Ȳ��ܶ�Ҳ����д��
 * */ 

int Queue_End_Use (LCUI_Queue * queue) ;
/* ���ܣ�����������ݵĶ���Ϊ����״̬ */ 

void Queue_Init (LCUI_Queue * queue, size_t element_size, void (*func) ()) ;
/* ���ܣ���ʼ������ */ 

void Queue_Using_Pointer(LCUI_Queue * queue);
/* 
 * ���ܣ��趨���г�Ա����Ϊָ�� 
 * ˵�����������ֻ�����ڴ��ָ�룬���Ҳ�ϣ���������ٺ�ָ��ָ����ڴ�ռ�Ҳ���ͷţ���
 * ʹ�øú������á�
 * */ 

int Queue_Get_Total(LCUI_Queue * queue);
/* ˵������ȡ���е�ǰ�ܳ�Ա���� */ 

int Queue_Swap(LCUI_Queue * queue, int pos_a, int pos_b);
/* ���ܣ�����������ָ��λ��������Ա��ֵ */ 

int Queue_Delete_By_Flag (LCUI_Queue * queue, int pos, int flag) ;
/* 
 * ���ܣ��Ӷ�����ɾ��һ����Ա�����������ж���
 * ˵��������ʽ��flag��ֵ����ͬ 
 * ����ֵ�����������棨1���������ؼ٣�0��
 * */ 

int Queue_Delete (LCUI_Queue * queue, int pos);
/* ���ܣ��Ӷ�����ɾ��һ����Ա�����ͷŸó�Առ�õ��ڴ���Դ */ 

int Queue_Delete_Pointer (LCUI_Queue * queue, int pos) ;
/* ���ܣ��Ӷ�����ɾ��һ����Աָ�룬���Ը�ָ��ָ����ڴ�����ͷ� */ 

void Destroy_Queue(LCUI_Queue * queue) ;
/* ���ܣ��ͷŶ���ռ�õ��ڴ���Դ */ 

void * Queue_Get (LCUI_Queue * queue, int pos);
/* 
 * ���ܣ��Ӷ����л�ȡָ��λ�õĳ�Ա 
 * ˵�����ɹ�����ָ��ó�Ա��ָ�룬ʧ�ܷ���NULL
 * ע�⣺����Է��ص�ָ�����free����
 * */ 

int Queue_Insert(const void *data, int pos, LCUI_Queue * queue);
/* ���ܣ��������ָ��λ�ò����Ա */ 

int Queue_Move(LCUI_Queue *queue, int des_pos, int src_pos);
/* ���ܣ���������ָ��λ�õĳ�Ա�ƶ���Ŀ��λ�� */ 

int Queue_Replace(LCUI_Queue * queue, int pos, const void *data);
/* ���ܣ����Ƕ�����ָ��λ�õĳ�Ա */ 

int Queue_Add_By_Flag(LCUI_Queue * queue, void *data, int flag);
/* 
 * ���ܣ����µĳ�Ա��������� 
 * ˵�����Ƿ�Ϊ�³�Ա���·����ڴ�ռ䣬�ɲ���flag��ֵ����
 * ����ֵ�������򷵻��ڶ����е�λ�ã������򷵻ط�0ֵ
 * */ 

int Queue_Add(LCUI_Queue * queue, void *data) ;
/* 
 * ���ܣ����µĳ�Ա��������� 
 * ˵�����������ֻ�ǵ�������ӳ�Ա��������и���Ĺ��ܣ���Ҫ�Լ�ʵ��
 * */ 

int Queue_Add_Pointer(LCUI_Queue * queue, void *data);
/* 
 * ���ܣ����µĳ�Ա��������� 
 * ˵������Queue_Add������ͬ���ú���ֻ���޸�ָ��λ�õĳ�Աָ��ָ��ĵ�ַ����Ҫ��
 * �벿�����еĴ����ϣ��еĲ�����Ҫ��һ������ת�Ƶ���һ�������ϣ������·����ڴ�ռ䣬
 * ֱ��ʹ��ԭ�����ڴ��ַ������Ϊ�˱��ⲿ��ת�����ڶ��к󣬲���ָ����Ч�����⡣
 * */ 
 
int Queue_Empty(LCUI_Queue *queue);
/* ���ܣ��������Ƿ�Ϊ�� */ 

/************************ LCUI_Queue End ******************************/

/************************** WidgetQueue ********************************/ 

void WidgetQueue_Init(LCUI_Queue *queue);
/* ���ܣ���ʼ���������� */ 

int WidgetQueue_Get_Pos(LCUI_Queue *queue, LCUI_Widget *widget);
/* ���ܣ��Ӳ��������л�ȡָ������������λ�� */ 

int WidgetQueue_Move(LCUI_Queue *queue, int pos, LCUI_Widget *widget);
/* 
 * ���ܣ��ƶ�ָ�������ڶ����е�λ��
 * ����ֵ���ɹ��򷵻ز�������λ�ã�ʧ�ܷ���-1
 * */ 
/************************ WidgetQueue End ******************************/


/************************* RectQueue **********************************/
void RectQueue_Init(LCUI_Queue *queue);
/* ���ܣ���ʼ������������ݵĶ��� */ 

int RectQueue_Get( LCUI_Rect * rect, int pos, LCUI_Queue * queue);
/* ���ܣ��Ӷ���ָ��λ���л�ȡһ���������� */ 

void Queue_Copy(LCUI_Queue *des, LCUI_Queue *src);
/* ���ܣ���Դ�������ȫ����Ա����׷����Ŀ������� */ 
 
int RectQueue_Add (LCUI_Queue * queue, LCUI_Rect rect) ;
/* ���ܣ�����������׷�������� */ 
/************************* RectQueue end *******************************/

#endif


