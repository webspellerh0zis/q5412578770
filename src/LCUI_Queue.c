/* ***************************************************************************
 * LCUI_Queue.c -- basic queue processing
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
 * LCUI_Queue.c -- �����Ķ��д���
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
#include LC_LCUI_H
#include LC_MISC_H
#include LC_MEM_H
#include LC_WIDGET_H
#include LC_THREAD_H
#include LC_ERROR_H

/************************ LCUI_Queue **********************************/
int Queue_Lock (LCUI_Queue *queue)
/* ���ܣ�����һ�����У�ʹֻ֮�ܱ�һ���߳�ʹ�� */
{ 
	return thread_mutex_lock(&queue->lock);
}

int Queue_UnLock (LCUI_Queue *queue)
/* ���ܣ��⿪������ */
{
	return thread_mutex_unlock(&queue->lock);
}

int Queue_Using (LCUI_Queue * queue, int mode) 
/* 
 * ���ܣ��趨���е�״̬Ϊ��ʹ�á� 
 * ˵��������mode��Ҫ��QUEUE_MODE_READ��QUEUE_MODE_WRITE������֮һ��
 * QUEUE_MODE_READ ��ʾ������ģʽ�������Ƕ������Ժ������ԡ�����ģʽ���ʱ����е��̹߳���
 * QUEUE_MODE_WRITE ��ʾ��д��ģʽ��дֻ����һ���߳�д�������̼߳Ȳ��ܶ�Ҳ����д��
 * */
{ 
	if(mode == QUEUE_MODE_READ) 
		return thread_rwlock_rdlock(&queue->lock); 
	else 
		return thread_rwlock_wrlock(&queue->lock);  
}

int Queue_End_Use (LCUI_Queue * queue) 
/* ���ܣ�����������ݵĶ���Ϊ����״̬ */
{
	return thread_rwlock_unlock(&queue->lock); 
}


void Queue_Init (LCUI_Queue * queue, size_t element_size, void (*func) ()) 
/* ���ܣ���ʼ������ */
{
	thread_rwlock_init(&queue->lock);
	queue->member_type  = 0;   
	queue->queue		= NULL;
	queue->total_num	= 0;
	queue->max_num		= 0;
	queue->element_size	= element_size;
	queue->destroy_func	= func; 
}

void Queue_Using_Pointer(LCUI_Queue * queue)
/* 
 * ���ܣ��趨���г�Ա����Ϊָ�� 
 * ˵�����������ֻ�����ڴ��ָ�룬���Ҳ�ϣ���������ٺ�ָ��ָ����ڴ�ռ�Ҳ���ͷţ���
 * ʹ�øú������á�
 * */
{
	queue->member_type = 1;
}

int Queue_Get_Total(LCUI_Queue * queue)
/* ˵������ȡ���е�ǰ�ܳ�Ա���� */
{
	return queue->total_num;
}

int Queue_Swap(LCUI_Queue * queue, int pos_a, int pos_b)
/* ���ܣ�����������ָ��λ��������Ա��ֵ */
{
	void *temp;
	if (pos_a < queue->total_num 
		&& pos_b < queue->total_num 
		&& queue->total_num > 0)
	{
		temp = queue->queue[pos_a];
		queue->queue[pos_a] = queue->queue[pos_b];
		queue->queue[pos_b] = temp;
		return 0;
	}
	return -1;
}

int Queue_Delete_By_Flag (LCUI_Queue * queue, int pos, int flag) 
/* 
 * ���ܣ��Ӷ�����ɾ��һ����Ա�����������ж���
 * ˵��������ʽ��flag��ֵ����ͬ 
 * ����ֵ�����������棨1���������ؼ٣�0��
 * */
{
	int i, value = 0;
	void *bak;
	Queue_Using (queue, QUEUE_MODE_WRITE);
	if (pos >=0 && pos < queue->total_num && queue->total_num > 0)
	{
		if(flag == 1)
		{/* ���Ҫɾ�����Ƕ��г�Ա�Ļ� */
			/* �ͷŸ�λ�õĳ�Առ�õ��ڴ�ռ� */
			if(NULL != queue->destroy_func)  /* ����������������͵������������� */
				queue->destroy_func(queue->queue[pos]); 
			/* ����Ҫ�ͷ��ڴ棬ֻ���ڵ���Destroy_Queue����ʱ��ȫ���ͷ� */
			//free(queue->queue[pos]); 
		}
		bak = queue->queue[pos];/* ���ݵ�ַ */
		for (i = pos; i < queue->total_num - 1; ++i)
		/* �ƶ����и�����Աλ�ã���ֻ�ǽ���ָ���ֵ������Ҫɾ���ĳ�Ա��������ĩβ */
			queue->queue[i] = queue->queue[i + 1]; 
			
		if(flag == 1) 
		{/* ��ɾ���Ķ��г�Ա���ڴ��ַ�ƶ�������ĩβ */
			queue->queue[i] = bak;
			memset(queue->queue[i], 0, queue->element_size);
		}
		else queue->queue[i] = NULL;
		/* 
		 * �����ʹ�ñ�����ת�ƶ��г�Ա����һ�����У��ö��г�Ա������ͬһ���ڴ�ռ䣬
		 * ֻ��������¼�ó�Ա���ڴ��ַ�Ķ��в�ͬ�����ֲ�����������������Դ�����б���
		 * �ó�Ա�ĵ�ַ����ΪԴ���п��ܻᱻ���٣�����ʱҲ��free��������ÿ����Ա����
		 * Ŀ�����δ�����٣�������ʹ��֮ǰת�ƹ����ĳ�Ա������������
		 *  */
		/* ���ٳ�Ա���� */
		--queue->total_num;
		value = 1;
	} 
	Queue_End_Use (queue);
	return value;
}

int Queue_Delete (LCUI_Queue * queue, int pos)
/* ���ܣ��Ӷ�����ɾ��һ����Ա�����ͷŸó�Առ�õ��ڴ���Դ */
{
	return Queue_Delete_By_Flag(queue, pos, 1);
}

int Queue_Delete_Pointer (LCUI_Queue * queue, int pos) 
/* ���ܣ��Ӷ�����ɾ��һ����Աָ�룬���Ը�ָ��ָ����ڴ�����ͷ� */
{
	return Queue_Delete_By_Flag(queue, pos, 0);
}


void Destroy_Queue(LCUI_Queue * queue) 
/*
 * ���ܣ��ͷŶ���ռ�õ��ڴ���Դ 
 */
{ 
	int i;
	if(queue->member_type == 0)
	{/* �����Ա����ͨ���ͣ��ͷŶ��г�Առ�õ��ڴ�ռ� */
		while(Queue_Delete(queue, 0));/* ��ն��г�Ա */ 
		for(i=0; i<queue->max_num; i++)
			free(queue->queue[i]); 
	}
	free (queue->queue);/* �ͷŶ�άָ��ռ�õĵ��ڴ�ռ� */ 
	
	queue->queue		= NULL;
	queue->total_num	= 0;
	queue->max_num		= 0;
	thread_rwlock_destroy(&queue->lock);
}


void * Queue_Get (LCUI_Queue * queue, int pos)
/* 
 * ���ܣ��Ӷ����л�ȡָ��λ�õĳ�Ա 
 * ˵�����ɹ�����ָ��ó�Ա��ָ�룬ʧ�ܷ���NULL
 * ע�⣺����Է��ص�ָ�����free����
 * */
{
	void  *data = NULL;
	Queue_Using (queue, QUEUE_MODE_READ);
	if (queue->total_num > 0 && pos < queue->total_num) 
		data = queue->queue[pos]; 
	Queue_End_Use (queue);
	return data;
}

int Queue_Insert(const void *data, int pos, LCUI_Queue * queue)
/* ���ܣ��������ָ��λ�ò����Ա */
{
	int i, total;
	size_t size;
	total = Queue_Get_Total(queue);
	if(pos > total) 
		return -1; 
	
	Queue_Using(queue, QUEUE_MODE_WRITE);
	/* û�еĻ�������Ҫ��������� */
	++queue->total_num;
	if(queue->total_num > queue->max_num)
	{/* ��������ǰ�������ڵ�ǰ���������������ڴ������ݵ��ڴ�ռ� */
		size =  sizeof(void*) * queue->total_num;
		queue->queue = (void**)realloc( queue->queue, size );
		if(queue->queue == NULL)
		{/* ������·����ڴ�ʧ���� */
			Queue_End_Use(queue);
			printf("Queue_Insert(): "ERROR_MALLOC_ERROR);
			exit(-1);
		}
		/* Ϊ��Ա�����ڴ�ռ� */
		queue->queue[total] = malloc(queue->element_size);
		queue->max_num = queue->total_num;
	}
	/* ƽ�ƶ����еĳ�Ա */
	for(i = queue->total_num - 1; i > pos; --i)
		memcpy(queue->queue[i], queue->queue[i-1], queue->element_size);
	/* ��ֵ���������� */
	memcpy(queue->queue[pos], data, queue->element_size);
	
	Queue_End_Use(queue);
	return 0;
}

int Queue_Move(LCUI_Queue *queue, int des_pos, int src_pos)
/* ���ܣ���������ָ��λ�õĳ�Ա�ƶ���Ŀ��λ�� */
{
	void *temp;
	int i, total;
	
	total = Queue_Get_Total(queue);
	if(des_pos < 0 || des_pos > total 
	|| src_pos < 0 || src_pos > total )
		return -1;
	
	Queue_Using(queue, QUEUE_MODE_WRITE);
	temp = queue->queue[src_pos];
	if (src_pos > des_pos)
	{	/* �����λ����ԭλ�õ�ǰ�棬����λ��֮��ĳ�Ա�����ƶ� */
		for (i = src_pos; i > des_pos; --i) 
			queue->queue[i] = queue->queue[i - 1];  
	}
	else if (src_pos < des_pos)
	{	/* �����λ����ԭλ�õĺ��棬����λ��֮��ĳ�Ա�����ƶ� */
		for (i = src_pos; i < des_pos; ++i) 
			queue->queue[i] = queue->queue[i + 1];  
	} 
	
	queue->queue[des_pos] = temp;
	Queue_End_Use(queue);
	return 0;
}


int Queue_Replace(LCUI_Queue * queue, int pos, const void *data)
/* ���ܣ����Ƕ�����ָ��λ�õĳ�Ա */
{
	int total;
	total = Queue_Get_Total(queue);
	if(pos >= total)	/* ����������з�Χ */
		return -1;
		
	Queue_Using(queue, QUEUE_MODE_WRITE);
	
	/* 
	 * ���ǵ����г�Ա��ʱ���ǽṹ�壬���ҽṹ���Ա�п��ܻ���ָ�룬Ϊ�˱������ظ����ǲ�
	 * ��ָ������ͷŶ����µ��ڴ��������Ҫ�ȵ������������Ըó�Ա�������٣���Ϊ��������
	 * һ���Խṹ���е�ָ������ͷţ�֮���ٸ����³�Ա���������ó�Ա���ڴ�ռ䡣
	 *  */
	if(NULL != queue->destroy_func) 
		queue->destroy_func(queue->queue[pos]); 
		
	memcpy(queue->queue[pos], data, queue->element_size);
	Queue_End_Use(queue);
	return 0;
}

int Queue_Add_By_Flag(LCUI_Queue * queue, void *data, int flag)
/* 
 * ���ܣ����µĳ�Ա��������� 
 * ˵�����Ƿ�Ϊ�³�Ա���·����ڴ�ռ䣬�ɲ���flag��ֵ����
 * ����ֵ�������򷵻��ڶ����е�λ�ã������򷵻ط�0ֵ
 * */
{
	int pos;
	
	Queue_Using(queue, QUEUE_MODE_WRITE);
	
	pos = queue->total_num;
	queue->total_num += 1;					/* ����+1 */
	if(queue->total_num > queue->max_num)
	{/* �����ǰ��������֮ǰ�������� */
		queue->max_num = queue->total_num;
		/* �����������1��˵��֮ǰ�Ѿ�malloc����ֱ��realloc�����ڴ� */
		if (queue->total_num > 1 && queue->queue != NULL) 
			queue->queue =	(void **)
								realloc(
									queue->queue, sizeof(void*) * 
									queue->total_num ); 
		else /* ������Ҫ��malloc */
			queue->queue = (void **) malloc (sizeof(void*)); 
		
		if(NULL == queue->queue)
		{
			printf("Queue_Add_By_Flag(): "ERROR_MALLOC_ERROR);
			Queue_End_Use(queue);
			exit(-1);
		}
		/* Ϊ��λ�õĳ�Ա�����ڴ�ռ� */
		if (flag == 1)
			queue->queue[pos] = malloc(queue->element_size);
	}
	else if(flag == 1)
	{
		/* 
		 * ת�ƶ��г�Ա�ĵ�ַ����һ�����к�Դ�����еĶ���ָ��ḳΪNULL��������Ҫ
		 * ���·����ڴ档
		 *  */ 
		if(queue->queue[pos] == NULL)
			queue->queue[pos] = malloc(queue->element_size);
	}
	/* 
	 * total_num����1����������max_num����ô�������ֳɵ��ڴ�ռ���ã�ֱ��
	 * ����memcpy�����������ݽ�ȥ���ɡ���ΪQueue_Delete�����������ͷų�Ա
	 * ռ�õ��ڴ�ռ䣬���Ҳֻ�ǽ���Ա���ָ��ָ����ڴ�ռ��ͷš�
	 *  */
	
	if(flag == 1)  
		/* ֱ���õȺŸ�ֵ�Ļ������������о�����Ϣ����memcpy�������� */
		memcpy(queue->queue[pos], data, queue->element_size);  
	else
		queue->queue[pos] = data;
		
	Queue_End_Use (queue);
	return pos;
}

int Queue_Add(LCUI_Queue * queue, void *data) 
/* 
 * ���ܣ����µĳ�Ա��������� 
 * ˵�����������ֻ�ǵ�������ӳ�Ա��������и���Ĺ��ܣ���Ҫ�Լ�ʵ��
 * */
{
	return Queue_Add_By_Flag(queue, data, 1); 
}


int Queue_Add_Pointer(LCUI_Queue * queue, void *data)
/* 
 * ���ܣ����µĳ�Ա��������� 
 * ˵������Queue_Add������ͬ���ú���ֻ���޸�ָ��λ�õĳ�Աָ��ָ��ĵ�ַ����Ҫ��
 * �벿�����еĴ����ϣ��еĲ�����Ҫ��һ������ת�Ƶ���һ�������ϣ������·����ڴ�ռ䣬
 * ֱ��ʹ��ԭ�����ڴ��ַ������Ϊ�˱��ⲿ��ת�����ڶ��к󣬲���ָ����Ч�����⡣
 * */
{
	return Queue_Add_By_Flag(queue, data, 0); 
}

int Queue_Empty(LCUI_Queue *queue)
/* ���ܣ��������Ƿ�Ϊ�� */
{
	if(queue->total_num > 0)
		return 0;
		
	return 1;
}

//#define _NEED_TEST_QUEUE_
#ifdef _NEED_TEST_QUEUE_
/*
 * ����������main���������ڶԱ��ļ��ڵĺ������в��ԣ������ѡ������һ��main����������
 * �����У��������
 * */


/* 
 * ���ܣ�����ͨ�ö��еĳ�Աָ����ɾ����
 * ˵�����ȴӶ���1�л�ȡָ��λ�õĳ�Աָ�룬֮��ɾ����λ�õĳ�Աָ�룬�ѳ�Աָ�������
 * ����2�С�
 *  */
int main()
{
	int i;
	char *p, ch, str[11];
	LCUI_Queue q1, q2;
	/* ��ʼ�� */
	Queue_Init(&q1, sizeof(char), NULL);
	Queue_Init(&q2, sizeof(char), NULL);
	/* ���0��9���ַ������� */
	for(i=0; i<10; i++)
	{
		ch = '0' + i;
		Queue_Add(&q1, &ch);
	}
	/* ��ȡÿ����Ա�������������� */
	for(i=0; i<10; i++) 
	{	/* �Ƚ�void��ָ��ת����char��ָ�룬֮��ȡ��ָ��ָ��ı������� */
		str[i] = *( (char*)Queue_Get(&q1, i) );  
	}
	str[i] = 0;
	
	printf("q1, string:%s\n", str);
	p = (char*)Queue_Get(&q1, 5);
	printf("char: %c\n", *p);
	Queue_Delete_Pointer(&q1, 5); 
	for(i=0; i<9; i++)
		str[i] = *( (char*)Queue_Get(&q1, i) );
	str[i] = 0;
	printf("q1, string:%s\n", str);
	Queue_Add_Pointer(&q2, p);
	for(i=0; i<Queue_Get_Total(&q2); i++)
		str[i] = *( (char*)Queue_Get(&q2, i) );
	str[i] = 0;
	printf("q2, string:%s\n", str);
	
	Destroy_Queue(&q1);
	Destroy_Queue(&q2);
	return 0;
}

/* 
 * ���ܣ�����ͨ�ö��еĻ�������
 * ˵�����˺����ǽ���0��-��9�����ַ�����ͨ�ö����У�֮����ȡ���������ַ����У�����ӡ�ַ���
 *  */
int main()
{
	int i;
	char ch, str[11];
	LCUI_Queue bq;
	/* ��ʼ�� */
	Queue_Init(&bq, sizeof(char), NULL);
	/* ���0��9���ַ������� */
	for(i=0; i<10; i++)
	{
		ch = '0' + i;
		Queue_Add(&bq, &ch);
	}
	/* ��ȡÿ����Ա�������������� */
	for(i=0; i<10; i++) 
	{	/* �Ƚ�void��ָ��ת����char��ָ�룬֮��ȡ��ָ��ָ��ı������� */
		str[i] = *( (char*)Queue_Get(&bq, i) );  
	}
	str[i] = 0;
	
	printf("string:%s\n", str);
	
	Destroy_Queue(&bq);
	return 0;
}
#endif


/************************ LCUI_Queue End ******************************/

/************************** WidgetQueue ********************************/

static void Destroy_Widget(LCUI_Widget *widget)
/*
 * ���ܣ�����һ������
 * ˵�����������������Ӳ�������������������
 * */
{
	widget->parent = NULL;
	widget->focus = NULL;
	
	/* �ͷ��ַ��� */
	Free_String(&widget->type);
	Free_String(&widget->style);
	
	/* �ͷŴ���ͼ�����ݵĽṹ�� */
	Free_Graph(&widget->graph);
	Free_Graph(&widget->background_image);
	
	/* ���ٲ����Ķ��� */
	Destroy_Queue(&widget->child);
	Destroy_Queue(&widget->event);
	Destroy_Queue(&widget->data);
	Destroy_Queue(&widget->update_area);
	
	widget->focus = NULL;
	widget->visible = IS_FALSE;
	widget->enabled = IS_TRUE;
	
	void (*func)(LCUI_Widget*);
	func = Get_WidgetFunc_By_ID(widget->type_id, 
						FUNC_TYPE_DESTROY);
	func(widget); /* ����֮���ͷ�privateָ��ָ����ڴ�ռ� */
	free(widget->private);/* �ͷ����ָ�򲿼�˽�����ݽṹ���ָ�� */
}

void WidgetQueue_Init(LCUI_Queue *queue)
/* ���ܣ���ʼ���������� */
{
	Queue_Init(queue, sizeof(LCUI_Widget), Destroy_Widget);
}

int WidgetQueue_Get_Pos(LCUI_Queue *queue, LCUI_Widget *widget)
/* ���ܣ��Ӳ��������л�ȡָ������������λ�� */
{
	LCUI_Widget *temp;
	int i, result = -1, total; 
	
	total = Queue_Get_Total(queue); 
	for(i = 0; i < total; ++i)
	{
		temp = Queue_Get(queue, i);
		if(temp == widget)
		{ 
			result = i; 
			break;
		}
	} 
	return result;
}


int WidgetQueue_Move(LCUI_Queue *queue, int pos, LCUI_Widget *widget)
/* 
 * ���ܣ��ƶ�ָ�������ڶ����е�λ��
 * ����ֵ���ɹ��򷵻ز�������λ�ã�ʧ�ܷ���-1
 * */
{
	LCUI_Widget *temp;
	int total, i, j, des_pos;
	total = Queue_Get_Total(queue);
	for(i=0; i<total; ++i)
	{
		temp = (LCUI_Widget*)Queue_Get(queue, i);
		if(temp == widget)
		{/* ����ҵ����� */
			j = i;
			for(i=0; i<j; ++i)
			{/* ��ͷ����ǰλ�ñ������� */
				temp = (LCUI_Widget*)Queue_Get(queue, i);
				
				if( temp->lock_display == IS_TRUE )
				{/* �����λ�õĲ���������λ�� */
					if(widget->lock_display == IS_TRUE)
					{/* ���Ŀ�겿��������λ�ã��ǾͿ����ƶ�����ǰ�� */
						des_pos = 0; 
						Queue_Using(queue, QUEUE_MODE_WRITE);
						for (i=j; i > des_pos; --i) 
							queue->queue[i] = queue->queue[i - 1]; 
							
						queue->queue[des_pos] = widget; 
						Queue_End_Use(queue);
						break;
					}
				}
				else
				{/* ���򣬸�λ�õĲ���û����λ�� */
					des_pos = i;
					Queue_Using(queue, QUEUE_MODE_WRITE);
					for (i=j; i > des_pos; --i) 
						queue->queue[i] = queue->queue[i - 1];  
						
					queue->queue[des_pos] = widget; 
					Queue_End_Use(queue);
					break;
				}
			} 
		}
	}
	return -1;
}
/************************ WidgetQueue End ******************************/


/************************* RectQueue **********************************/
void RectQueue_Init(LCUI_Queue *queue)
/* ���ܣ���ʼ������������ݵĶ��� */
{
	/* ����LCUI_Rect�ṹ���еĳ�Աû��ָ�룬��ˣ�����Ҫ�ͷ�ָ��ָ����ڴ棬Ҳ�Ͳ���Ҫ�������� */
	Queue_Init(queue, sizeof(LCUI_Rect), NULL);
}

int RectQueue_Get( LCUI_Rect * rect, int pos, LCUI_Queue * queue)
/* ���ܣ��Ӷ���ָ��λ���л�ȡһ���������� */
{
	void *temp;
	temp = Queue_Get(queue, pos);
	if(NULL == temp)
		return 0;
	
	memcpy(rect, temp, sizeof(LCUI_Rect));
	return 1;
}

void Queue_Copy(LCUI_Queue *des, LCUI_Queue *src)
/* ���ܣ���Դ�������ȫ����Ա����׷����Ŀ������� */
{
	LCUI_Rect *rect;
	int i, total;
	total = Queue_Get_Total(src);
	for(i=0; i<total; ++i)
	{
		rect = (LCUI_Rect *)Queue_Get(src, i);/* ��ȡԴ������ĳ�Ա */
		//printf("[%d] rect: %d,%d, %d,%d\n", i, rect->x, rect->y, rect->width, rect->height);
		Queue_Add(des, rect); /* �����Ŀ������� */
	}
}
 
int RectQueue_Add (LCUI_Queue * queue, LCUI_Rect rect) 
/* ���ܣ�����������׷�������� */
{ 
	int i, flag = 0;
	LCUI_Rect t_rect; 
	LCUI_Queue rect_buff;
	
	//if(debug_mark)
	//	printf("New : [%d,%d] %d,%d\n", rect.x, rect.y, rect.width, rect.height);
	
	if(!Rect_Valid(rect))
	{
		//printf("not valid\n");
		return -1;
	}
	
	RectQueue_Init(&rect_buff);
	
	for (i = 0; i < queue->total_num; ++i)
	{
		if(RectQueue_Get(&t_rect, i, queue))
		{
			//if(debug_mark)
			//  printf("temp : [%d,%d] %d,%d\n", t_rect.x, t_rect.y, t_rect.width, t_rect.height);
			
			if (!Rect_Valid(t_rect)) 
				/* ɾ������������ݣ���Ϊ������Ч�� */
				Queue_Delete (queue, i); 
			else if (Rect_Include_Rect (rect, t_rect)) 
				/* ɾ������������ݣ���Ϊ���Ѿ��������ľ���������� */
				Queue_Delete (queue, i); 
			else if (Rect_Include_Rect (t_rect, rect))
			{/* ��������ľ����������Ѵ��ڵľ����������ڰ�����ϵ */
				//if(debug_mark) 
				//  printf("Include 2\n");
				  
				flag = 1;
				break;
			}
			else if(Rect_Equal(rect, t_rect))
			{/* ��ȵľͲ���Ҫ�� */
				//if(debug_mark)
				//  printf("Equal || not valid\n");
				
				flag = 1;
				break;
			}
			else if(Rect_Is_Overlay(rect, t_rect))
			{/* ��������ľ���������еľ����ص� */ 
				/* �����η�������ɸ����ص��ľ��Σ�֮��������ӽ�ȥ */
				//printf("Rect_Is_Overlay(rect, t_rect)\n");
				Cut_Overlay_Rect(t_rect, rect, &rect_buff);
				//debug_mark = 1;
				Queue_Copy(queue, &rect_buff);
				//debug_mark = 0;
				flag = 1;
				break;
			}
		}
		else break;
	}
	
	Destroy_Queue(&rect_buff);
	if (flag == 0)
	/* û�еĻ�������Ҫ��������� */ 
		return Queue_Add(queue, &rect);
	/* ���ٶ��� */
	//if(debug_mark) 
	//	printf("done\n");
	return -1;
}
/************************* RectQueue end *******************************/

