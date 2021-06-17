/* ***************************************************************************
 * LCUI_Main.c -- LCUI's kernel
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
 * LCUI_Main.c -- LCUI �ĺ���
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
#include LC_QUEUE_H
#include LC_GRAPHICS_H
#include LC_RES_H
#include LC_CURSOR_H 
#include LC_INPUT_H
#include LC_MISC_H 
#include LC_MEM_H
#include LC_ERROR_H
#include LC_FONT_H
#include LC_THREAD_H 
#include LC_WIDGET_H

#include <linux/fb.h>
#include <sys/mman.h>
#include <stdio.h>  
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

LCUI_System LCUI_Sys; 

/***************************** Core ***********************************/
static void Processing_Screen_Update()
/* ���ܣ�������Ļ���ݸ��� */
{
	LCUI_Rect rect;
	LCUI_Graph graph;  
	Graph_Init(&graph);
	/* ��ס���У������̲߳��ܷ��� */
	Queue_Lock(&LCUI_Sys.update_area);
	//printf("Processing_Screen_Update(): start\n"); 
	while(LCUI_Active())
	{
		if ( RectQueue_Get(&rect, 0, &LCUI_Sys.update_area) )
		{/* ����Ӷ����л�ȡ���ݳɹ� */
			//printf("RectQueue_Get(): %d,%d,%d,%d\n", rect.x, rect.y, rect.width, rect.height);
			/* ��ȡ�ڴ��ж�Ӧ�����ͼ������ */ 
			//printf("Processing_Screen_Update(): get\n"); 
			Get_Screen_Real_Graph (rect, &graph);
			//printf("Processing_Screen_Update(): end\n");
			/* д����֡���壬����Ļ��ʾͼ�� */ 
			Write_Graph_To_FB (&graph, Pos(rect.x, rect.y));
			Free_Graph(&graph); 
			//printf("Processing_Screen_Update(): queue delete start\n"); 
			//printf("queue mode: %d\n", LCUI_Sys.update_area.mode);
			Queue_Delete (&LCUI_Sys.update_area, 0);/* �Ƴ������еĳ�Ա */ 
			//printf("Processing_Screen_Update(): queue delete end\n");
		}
		else break;
	}
	/* �������� */
	Queue_UnLock(&LCUI_Sys.update_area);
	
	//printf("Processing_Screen_Update(): end\n"); 
	Free_Graph(&graph);
}

#ifdef need_test
static int auto_flag = 0;
static void *autoquit()
/* �ڳ�ʱ�󣬻��Զ���ֹ�������ڵ��� */
{
	LCUI_ID time = 0;
	while(time <5000000)
	{
		if(auto_flag == 0)
		{
			usleep(10000);
			time += 10000;
		}
		else 
		{
			auto_flag = 0;
			time = 0;
		}
	}
	exit(0);
}
#endif

static void *LCUI_Core ()
/* ���ܣ�����LCUI�ĺ��Ĵ����� */
{
	//pthread_t t;
	//LCUI_Thread_Create(&t, NULL, autoquit, NULL);
	while(LCUI_Active())
	{
		//printf("core start\n");
		Processing_All_WidgetUpdate();/* �������в������� */ 
		usleep(5000);/* ͣ��һ��ʱ�䣬�ó��������� */
		
		Processing_Refresh_Area(); /* ������Ҫˢ�µ����� */  
		Processing_Screen_Update();/* ������Ļ���� */  
		//auto_flag = 1;
		//printf("core end\n"); 
	}
	
	pthread_exit(NULL);
}

static int Core_Start()
/* ���ܣ������ں� */
{
	LCUI_Sys.status = ACTIVE;	/* �ı�LCUI��״̬ */
	return pthread_create( &LCUI_Sys.core_thread, NULL, LCUI_Core, NULL );
}

static int Core_End()
/* ���ܣ������ں� */
{
	LCUI_Sys.status = KILLED;	/* �ı�LCUI��״̬ */
	return pthread_join( LCUI_Sys.core_thread, NULL );
}
/**************************** Core End *********************************/

/************************* App Manage *********************************/
LCUI_App *Find_App(LCUI_ID id)
/* ���ܣ����ݳ����ID����ȡָ��������ݽṹ��ָ�� */
{
	LCUI_App *app; 
	int i, total;  
	total = Queue_Get_Total(&LCUI_Sys.app_list);
	if (total > 0)
	{ /* ���������������0 */
		for (i = 0; i < total; ++i)
		{
			app = (LCUI_App*)Queue_Get(&LCUI_Sys.app_list, i);
			if(app->id == id)
				return app;
		}
	}
	
	return NULL;
}

LCUI_App* Get_Self_AppPointer()
/* ���ܣ���ȡ�����ָ�� */
{
	Thread_TreeNode *ttn;
	LCUI_ID id;
	
	id = pthread_self(); /* ��ȡ���߳�ID */  
	if(id == LCUI_Sys.core_thread
	|| id == LCUI_Sys.key_thread
	|| id == LCUI_Sys.mouse.thread
	|| id == LCUI_Sys.ts.thread)
	{/* �����ں˼������߳�IDû�б���¼��ֻ��ֱ�ӷ���LCUI��������߳�ID�� */
		return Find_App(LCUI_Sys.self_id);
	}
	/* ��ȡ���̵߳�ID */
	ttn = Search_Thread_Tree(&LCUI_Sys.thread_tree, id);
	/* 
	 * ������������ֱ������ָ��ΪNULL����Ϊ���߳���û
	 * �и��߳̽��ָ��ģ�������߳�ID���ڸ��߳����
	 * ���߳�ID������ 
	 * */
	while(ttn->parent != NULL)
	{ 
		ttn = ttn->parent; 
		if(ttn != NULL && ttn->parent == NULL) 
			break;
	}
	
	return Find_App(ttn->tid);
}


void LCUI_App_Init(LCUI_App *app)
/* ���ܣ���ʼ���������ݽṹ�� */
{
	app->stop_loop = IS_FALSE;
	app->id = 0;
	FuncQueue_Init(&app->task_queue); 
	//EventQueue_Init(&app->key_event);
	WidgetLib_Init(&app->widget_lib);
}


static void LCUI_Quit ()
/*
 * ���ܣ��˳�LCUI
 * ˵������û���κ�LCUI����ʱ��LCUI����ñ��������ָ�����LCUIǰ���ֳ���
 * */
{
	int err = 0; 
	
	LCUI_Sys.status = KILLED;	/* ״̬��־��ΪKILLED */
	Free_LCUI_Font ();			/* �ͷ�LCUI��Ĭ����������ռ�õ��ڴ���Դ */
	Core_End();/* �ȴ�Core�߳��˳� */
	Destroy_Queue(&LCUI_Sys.key_event);/* ���������¼����ݶ��� */ 
	Disable_Mouse_Input();			/* ����������� */
	Disable_TouchScreen_Input();	/* ���ô���֧�� */ 
	Disable_Key_Input();			/* ���ð������� */
	/* �ָ���Ļ��ʼ���� */ 
	Write_Graph_To_FB (&LCUI_Sys.screen.buff, Pos(0, 0));	
	/* ���֡�������ڴ��е�ӳ�� */
	err = munmap (LCUI_Sys.screen.fb_mem, LCUI_Sys.screen.smem_len);
	if (err != 0) perror ("munmap()");
	close (LCUI_Sys.screen.fb_dev_fd); 
	exit (err);
}


static int LCUI_AppList_Delete (LCUI_ID app_id)
/* ���ܣ��ر�һ��LCUI���� */
{
	int pos = -1;
	LCUI_App *app; 
	int i, total;  
	total = Queue_Get_Total(&LCUI_Sys.app_list);
	if (total > 0)
	{ /* ���������������0�� ���ҳ�����Ϣ���ڶ��е�λ�� */
		for (i = 0; i < total; ++i)
		{
			app = (LCUI_App*)Queue_Get(&LCUI_Sys.app_list, i);
			if(app->id == app_id)
			{
				pos = i;
				break;
			}
		}
		if(pos < 0) return -1;
	}
	else return -1;
	/* �ӳ�����ʾ˳�������ɾ���������ID */
	Queue_Delete (&LCUI_Sys.app_list, pos);
	
	if (Queue_Empty(&LCUI_Sys.app_list)) /* ��������б�Ϊ�� */  
		LCUI_Quit (); /* �˳�LCUI */ 
	return 0;
}

static void LCUI_Destroy_App(LCUI_App *app)
/* ���ܣ����ٳ��������Ϣ */
{
	if(app == NULL) return;
	
	LCUI_App_Thread_Cancel(app->id); /* �����������������߳� */
	LCUI_Destroy_App_Widgets(app->id); /* ���������������в��� */
}


static void LCUI_AppList_Init()
/* ���ܣ���ʼ���������ݱ� */
{
	Queue_Init(&LCUI_Sys.app_list, sizeof(LCUI_App), LCUI_Destroy_App);
}


static int LCUI_AppList_Add ()
/* 
 * ���ܣ�����һ��LCUI����
 * ˵�����˺����Ὣ������Ϣ����������б�
 * ����ֵ���ɹ��򷵻س����ID��ʧ���򷵻�-1
 **/
{
	LCUI_App app;
	LCUI_ID id = pthread_self();
	/* ��ʼ���������ݽṹ�� */
	LCUI_App_Init (&app);
	app.id	= id;	/* ����ID */ 
	Queue_Add(&LCUI_Sys.app_list, &app);/* ��������� */
	return 0;
}

int App_Quit()
/* ���ܣ��˳����� */
{
	LCUI_App *app;
	app = Get_Self_AppPointer();
	if(NULL == app)
	{
		printf("App_Quit(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	} 
	
	return LCUI_AppList_Delete(app->id); 
}

void Main_Loop_Quit()
/* ���ܣ��ó����˳���ѭ�� */
{ 
	LCUI_App *app = Get_Self_AppPointer();
	if(NULL == app)
	{
		printf("Main_Loop_Quit(): "APP_ERROR_UNRECORDED_APP);
		return;
	}
	app->stop_loop = IS_TRUE; 
}
/*********************** App Manage End ******************************/

static void Print_LCUI_Copyright_Text()
/* ���ܣ���ӡLCUI����Ϣ */
{
	printf(
	"============| LCUI v0.12.4 |============\n"
	"Copyright (C) 2012 Liu Chao.\n"
	"Licensed under GPLv2.\n"
	"Report bugs to <lc-soft@live.cn>.\n"
	"Project Homepage: www.lcui.org.\n"
	"========================================\n"
	);
}

int Get_Screen_Width ()
/*
 * ���ܣ���ȡ��Ļ���
 * ����ֵ����Ļ�Ŀ�ȣ���λΪ���أ�������ʹ��LCUI_Init()������ʹ�ã�������Ч
 * */
{
	if (LCUI_Sys.init == IS_FALSE) 
		return 0; 
	else 
		return LCUI_Sys.screen.size.w; 
}

int Get_Screen_Height ()
/*
 * ���ܣ���ȡ��Ļ�߶�
 * ����ֵ����Ļ�ĸ߶ȣ���λΪ���أ�������ʹ��LCUI_Init()������ʹ�ã�������Ч
 * */
{
	if (LCUI_Sys.init == IS_FALSE) 
		return 0; 
	else 
		return LCUI_Sys.screen.size.h; 
}

LCUI_Size Get_Screen_Size ()
/* ���ܣ���ȡ��Ļ�ߴ� */
{
	return LCUI_Sys.screen.size; 
}

void Fill_Pixel(LCUI_Pos pos, LCUI_RGB color)
/* ���ܣ����ָ��λ�õ����ص����ɫ */
{
	unsigned char *dest;
	int k;
	k = (pos.y * LCUI_Sys.screen.size.w + pos.x) << 2; 

	dest = LCUI_Sys.screen.fb_mem;		/* ָ��֡���� */
	dest[k] = color.blue;
	dest[k + 1] = color.green;
	dest[k + 2] = color.red; 
}

void Write_Graph_To_FB (LCUI_Graph * src, LCUI_Pos pos)
/* 
 * ���ܣ�д��ͼ��������֡���棬�Ӷ�����Ļ����ʾͼ�� 
 * ˵�����˺�������Ҫ���룬�ο���mgaveiw 0.1.4 ��mga_vfb.c�ļ��е�
 * write_to_fb������Դ����.
 * */
{
	int bits;
	unsigned char *dest;
    struct fb_cmap kolor; 
	unsigned int x, y, n, k, count;
	unsigned int temp1, temp2, temp3, i; 
	LCUI_Rect cut_rect;
	LCUI_Graph temp, *pic;

	if (!Valid_Graph (src)) return;
	
	dest = LCUI_Sys.screen.fb_mem;		/* ָ��֡���� */
	pic = src; 
	Graph_Init (&temp);
	
	if ( Get_Cut_Area ( 
			Get_Screen_Size(), 
			Rect ( pos.x, pos.y, src->width, src->height ), 
			&cut_rect
		) )
	{/* �����Ҫ�ü�ͼ�� */
		if(!Rect_Valid(cut_rect))
			return;
			
		pos.x += cut_rect.x;
		pos.y += cut_rect.y;
		Cut_Graph (pic, cut_rect, &temp);
		pic = &temp;
	}
	
	Using_Graph (pic, 0);
	/* ��ȡ��ʾ����λ�� */
	bits = Get_Screen_Bits(); 
	switch(bits)
	{
		case 32:/* 32λ������RGB��ռ8λ��ʣ�µ�8λ����alpha����4���ֽ� */ 
		for (n=0,y = 0; y < pic->height; ++y)
		{
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n)
			{
				count = k + x;//count = 4 * (k + x);/* �������������ص������ */
				count = count << 2; 
				/* ����֡����(FrameBuffer)����ɫ������BGR��ͼƬ������RGB����Ҫ�ı�һ��д��˳�� */
				dest[count] = pic->rgba[2][n];
				dest[count + 1] = pic->rgba[1][n];
				dest[count + 2] = pic->rgba[0][n]; 
			}
		}
		break;
		case 24:/* 24λ��RGB��ռ8λ��Ҳ���ǹ�3���ֽ� */ 
		for (n=0, y = 0; y < pic->height; ++y)
		{
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n)
			{
				count = k + x;//count = 3 * (k + x); 
				count = (count << 1) + count;
				dest[count] = pic->rgba[2][n];
				dest[count + 1] = pic->rgba[1][n];
				dest[count + 2] = pic->rgba[0][n];
			}
		}
		break;
		case 16:/* 16λ��rgb�ֱ�ռ5λ��6λ��5λ��Ҳ����RGB565 */
		/*
		 * GB565��ɫģʽ, һ������ռ�����ֽ�, ����:
		 * ���ֽڵ�ǰ5λ������ʾB(BLUE)
		 * ���ֽڵĺ���λ+���ֽڵ�ǰ��λ������ʾG(Green)
		 * ���ֽڵĺ�5λ������ʾR(RED)
		 * */  
		for (n=0, y = 0; y < pic->height; ++y)
		{
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n)
			{
				count = (k + x) << 1;//count = 2 * (k + x);
				temp1 = pic->rgba[0][n];
				temp2 = pic->rgba[2][n];
				temp3 = pic->rgba[1][n];
				dest[count] = ((temp3 & 0x1c)<<3)+((temp2 & 0xf8)>>3);
				dest[count+1] = ((temp1 & 0xf8))+((temp3 & 0xe0)>>5);
			}
		}
		break;
		case 8: /* 8λ��ռ1���ֽ� */
		kolor.start = 0;
		kolor.len = 255; 
		kolor.red = calloc(256, sizeof(__u16));
		kolor.green = calloc(256, sizeof(__u16));
		kolor.blue = calloc(256, sizeof(__u16));
		kolor.transp = 0; 
		
		for (i=0;i<256;i++) 
		{
			kolor.red[i]=0;
			kolor.green[i]=0;
			kolor.blue[i]=0;
		}
		
		for (n=0, y = 0; y < pic->height; ++y)
		{
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n)
			{
				count = k + x;
				
				temp1 = pic->rgba[0][n]*0.92;
				temp2 = pic->rgba[1][n]*0.92;
				temp3 = pic->rgba[2][n]*0.92; 
				
				i = ((temp1 & 0xc0))
						+((temp2 & 0xf0)>>2)
						+((temp3 & 0xc0)>>6);
						
				kolor.red[i] = temp1*256;
				kolor.green[i] = temp2*256;
				kolor.blue[i] = temp3*256;
				dest[count] = (((temp1 & 0xc0))
								+((temp2 & 0xf0)>>2)
								+((temp3 & 0xc0)>>6)); 
			}
		}
		
		ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOPUTCMAP, &kolor); 
		free(kolor.red);
		free(kolor.green);
		free(kolor.blue);
		break;
		default: break;
    }
    
	End_Use_Graph (pic);
	Free_Graph (&temp);
}

static void Get_Widget_Real_Graph(LCUI_Widget *widget, LCUI_Rect rect, LCUI_Graph *graph)
/*
 * ���ܣ���ȡָ�������в�����ʵ��ͼ��
 * ˵������ȡͼ��ǰ����Ҫָ������ͼ��֮����������ϳɣ���ȡ��ͼ����ָ����������ʾ��ʵ��ͼ��
 **/
{
	//printf("Get_Widget_Real_Graph(): enter\n");
	//print_widget_info(widget);
	if(widget->visible == IS_FALSE) 
		return;
		
	int x, y, i;
	LCUI_Widget *child;
	/* ��ȡ��������Ļ�ڵ����� */
	LCUI_Pos pos = Get_Widget_Global_Pos(widget);
	/* ���㲿��ͼ��ճ���������е�λ�� */
	x = pos.x - rect.x;
	y = pos.y - rect.y;
	//printf("global pos: %d, %d\n", pos.x, pos.y);
	//printf("update area: %d, %d, %d, %d\n", rect.x, rect.y, rect.width, rect.height);
	/*
	 * ���ڸ��������Ӳ������������Ӳ�������ʾ��Χ���ڸ������ڣ���
	 * Ҫ����������ε���������Ӳ�����ͼ�γ���������Χ����ü���
	 * ������ͼ�β�ճ��������ͼ�С�
	 **/
	 if(widget->parent != NULL)
	 {/* ����и����� */ 
		LCUI_Rect cut_rect;
		/* ��ȡ�ò�������Ч��ʾ���� */
		cut_rect = Get_Widget_Valid_Rect(widget);
		/* ���ϲü�������� */
		x += cut_rect.x;
		y += cut_rect.y;
		/* �ü��������ϳ�������ͼ */
		Cut_And_Overlay_Graph(&widget->graph, cut_rect, Pos(x, y), graph);
	}
	else  /* �Ƚ��������ϳ�������ͼ�� */
		Mix_Graph (graph, &widget->graph, Pos(x, y));
	//printf("widget overlay pos: %d, %d\n", x, y);
	int total;
	LCUI_Rect tmp;
	
	total = Queue_Get_Total(&widget->child); 
	/* ò��ֻ��Ҫx��y������ߴ翿����ͼ����Ϣ���ɵõ� */
	for(i=total-1; i>=0; --i)
	{/* �ӵ׵��������Ӳ��� */
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		//printf("get child widget: %p, type: %s\n", child, child->type.string);
		if(child != NULL)
		{
			if(child->visible == IS_TRUE)
			{/* ����пɼ����Ӳ��� */
				tmp = Get_Widget_Rect(child);
				tmp.x += pos.x;
				tmp.y += pos.y;
				//printf("tmp: %d, %d, %d, %d\n", tmp.x, tmp.y, tmp.width, tmp.height);
				//printf("rect: %d, %d, %d, %d\n", rect.x, rect.y, rect.width, rect.height);
				/* �����������ص������Ӳ����ϳ�������ͼ�� */
				if (Rect_Is_Overlay(tmp, rect)) 
				{
					//printf("overlay\n");
					Get_Widget_Real_Graph(child, rect, graph);  /* �ݹ���� */
				}
				//else printf("not bverlay\n");
			}
		}
		else {
			//printf("break\n");
			continue;
		}
	}
	//printf("Get_Widget_Real_Graph(): quit\n");
}

void Catch_Screen_Graph_By_Cache(LCUI_Rect area, LCUI_Graph *out)
/* 
 * ���ܣ�ͨ���ڴ��е�ͼ�����ݣ���������Ļ����ʾ��ͼ��
 * ˵����Ч�ʽϵͣ���Ϊ��Ҫ���кϳɡ�
 *  */
{
	int k; 
	LCUI_Widget *widget;
	LCUI_Rect w_rect;
	LCUI_Pos pos;
	/* �����������Ƿ���Ч */
	if (area.x < 0) return; 
	if (area.y < 0) return; 
	if (area.x + area.width > Get_Screen_Width ())
		return;
	if (area.y + area.height > Get_Screen_Height ())
		return;
		
	if (area.width <= 0 || area.height <= 0)
		return;
		
	int total; 
	/* ��ȡ������ı���ͼ�� */
	out->flag = NO_ALPHA;
	Cut_Graph (&LCUI_Sys.screen.buff, area, out);
	total = Queue_Get_Total(&LCUI_Sys.widget_list);
	
	/* �ȴӵ׵��ϱ�������������������ص���ͼ���ͼ�ν��л�ϣ��õ����յ�ͼ�� */
	for (k = total - 1; k >= 0; --k)
	{	/* ����ײ�Ĳ�����ʼ */ 
		widget = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, k);
		w_rect = Get_Widget_Rect(widget);
		
		if (Rect_Is_Overlay(w_rect, area))/* ����ص� */
			Get_Widget_Real_Graph(widget, area, out);
	} 
	
	if (LCUI_Sys.cursor.visible == IS_TRUE)
	{						/* ����α�ɼ� */
		/* ���������Ƿ����α��ͼ�������ص� */ 
		if (Rect_Is_Overlay( area, Get_Cursor_Rect() ))
		{					/* ����ص� */ 
			pos = Pos_Sub(Get_Cursor_Pos(), Pos(area.x, area.y));
			/* ��ͼ�κϳ� */
			Mix_Graph (out, &LCUI_Sys.cursor.graph, pos);
		}
	}

	return;
}

void Catch_Screen_Graph_By_FB (LCUI_Rect area, LCUI_Graph *out)
/* 
 * ���ܣ�ֱ�Ӷ�ȡ֡�����е�ͼ������
 * ˵����Ч�ʽϸߣ��������ͼ���п��ܻ������⡣
 * */
{
	LCUI_Rect cut_rect;
	unsigned char *dest;
	dest = LCUI_Sys.screen.fb_mem;		/* ָ��֡���� */
	int x, y, n, k, count;
	
	if ( Get_Cut_Area ( Get_Screen_Size(), area,&cut_rect ) )
	{/* �����Ҫ�ü�ͼ�� */
		if(!Rect_Valid(cut_rect))
			return;
			
		area.x += cut_rect.x;
		area.y += cut_rect.y;
		area.width = cut_rect.width;
		area.height = cut_rect.height;
	}
	
	Malloc_Graph(out, area.width, area.height);
	Using_Graph (out, 1); 
	n = 0;
	for (y = 0; y < area.height; ++y)
	{
		k = (area.y + y) * LCUI_Sys.screen.size.w + area.x;
		for (x = 0; x < area.width; ++x)
		{
			count = k + x;
			count = count << 2;  
			out->rgba[2][n] = dest[count];
			out->rgba[1][n] = dest[count + 1];
			out->rgba[0][n] = dest[count + 2];
			++n;
		}
	}
	End_Use_Graph (out);
}


int Add_Screen_Refresh_Area (LCUI_Rect rect)
/* ���ܣ���������Ļ�������Ҫˢ�µ����� */
{
	if (rect.width <= 0 || rect.height <= 0)
		return -1; 
		
	rect = Get_Valid_Area(Get_Screen_Size(), rect);
	
	return RectQueue_Add (&LCUI_Sys.update_area, rect);
}


#define __MAX_TEXT_LEN 1024
void nobuff_print(const char *fmt, ...)
/* ���ܣ���ӡ��ִ�еĲ�������Ϣ */
{
	char text[__MAX_TEXT_LEN];
    memset(text, 0, sizeof(text)); 
    
    va_list ap;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap); 
	
	printf("%s ", text); 
	fflush(stdout);	/* ������������ */
}


static void Mouse_Init(void)
/* ���ܣ���ʼ��������� */
{
	LCUI_Sys.mouse.fd = 0;  
	LCUI_Sys.mouse.status = REMOVE; /* ���Ϊ�Ƴ�״̬ */
	LCUI_Sys.mouse.move_speed = 1; /* �ƶ�����Ϊ1 */
	EventQueue_Init(&LCUI_Sys.mouse.event);/* ��ʼ������¼���Ϣ���� */
}


static void Cursor_Init()
/* ���ܣ���ʼ���α����� */
{
	LCUI_Graph pic;
	Graph_Init(&pic);
	Load_Graph_Default_Cursor(&pic);/* �����Դ����α��ͼ������ */ 
	Set_Cursors_Graph(&pic); 
}
 

static void LCUI_IO_Init()
/* ���ܣ���ʼ������������� */
{
	int result;

	/* ����Ƿ�֧����� */
	nobuff_print("checking mouse support...");
	result = Check_Mouse_Support();
	if(result == 0)
	{
		printf("yes\n");
		/* ����������봦�� */
		nobuff_print("enable mouse input..."); 
		result = Enable_Mouse_Input();
		if(result == 0) printf("success\n");
		else printf("fail\n");
	}
	else printf("no\n");
	
	LCUI_Sys.ts.status = REMOVE;
	LCUI_Sys.ts.thread = 0;
	LCUI_Sys.ts.td = NULL; 
	/* ���ô������봦�� */
	printf("enable touchscreen input processing\n"); 
	Enable_TouchScreen_Input();  
	
	Enable_Key_Input();
}

static void print_screeninfo(
			struct fb_var_screeninfo fb_vinfo,
			struct fb_fix_screeninfo fb_fix
)
/* ���ܣ���ӡ��Ļ��ص���Ϣ */
{
	char visual[256], type[256];
	
	switch(fb_fix.type)
	{
		case FB_TYPE_PACKED_PIXELS:	strcpy(type, "packed pixels");break;
		case FB_TYPE_PLANES:		strcpy(type, "non interleaved planes");break;
		case FB_TYPE_INTERLEAVED_PLANES:	strcpy(type, "interleaved planes");break;
		case FB_TYPE_TEXT:					strcpy(type, "text/attributes");break;
		case FB_TYPE_VGA_PLANES:			strcpy(type, "EGA/VGA planes");break;
		default: strcpy(type, "unkown");break;
	}
	
	switch(fb_fix.visual)
	{
		case FB_VISUAL_MONO01:  strcpy(visual, "Monochr. 1=Black 0=White");break;
		case FB_VISUAL_MONO10:  strcpy(visual, "Monochr. 1=White 0=Black");break;
		case FB_VISUAL_TRUECOLOR:  strcpy(visual, "true color");break;
		case FB_VISUAL_PSEUDOCOLOR:  strcpy(visual, "pseudo color (like atari)");break;
		case FB_VISUAL_DIRECTCOLOR:  strcpy(visual, "direct color");break;
		case FB_VISUAL_STATIC_PSEUDOCOLOR:  strcpy(visual, "pseudo color readonly");break;
		default: strcpy(type, "unkown");break;
	}
	printf(
		"============== screen info =============\n" 
		"FB mem start  : 0x%08lX\n"
		"FB mem length : %d\n"
		"FB type       : %s\n"
		"FB visual     : %s\n"
		"accel         : %d\n"
		"geometry      : %d %d %d %d %d\n"
		"timings       : %d %d %d %d %d %d\n"
		"rgba          : %d/%d, %d/%d, %d/%d, %d/%d\n"
		"========================================\n",
		fb_fix.smem_start, fb_fix.smem_len,
		type, visual,
		fb_fix.accel,
		fb_vinfo.xres, fb_vinfo.yres, 
		fb_vinfo.xres_virtual, fb_vinfo.yres_virtual,  
		fb_vinfo.bits_per_pixel,
		fb_vinfo.upper_margin, fb_vinfo.lower_margin,
		fb_vinfo.left_margin, fb_vinfo.right_margin, 
		fb_vinfo.hsync_len, fb_vinfo.vsync_len,
		fb_vinfo.red.length, fb_vinfo.red.offset,
		fb_vinfo.green.length, fb_vinfo.green.offset,
		fb_vinfo.blue.length, fb_vinfo.blue.offset,
		fb_vinfo. transp.length, fb_vinfo. transp.offset
	);
}

static int Screen_Init()
/* ���ܣ���ʼ����Ļ */
{

	__u16 rr[256],gg[256],bb[256];
	struct fb_var_screeninfo fb_vinfo;
	struct fb_fix_screeninfo fb_fix;
	struct fb_cmap oldcmap = {0,256,rr,gg,bb} ;

	nobuff_print("open video output device...");
	LCUI_Sys.screen.fb_dev_name = FB_DEV;
	if ((LCUI_Sys.screen.fb_dev_fd = 
		open(LCUI_Sys.screen.fb_dev_name, O_RDWR)) == -1) 
	{/* �����ͼ������豸���� */
		printf("fail\n");
		perror("error");
		exit(-1);
	}
	else printf("success\n");
	/* ��ȡ��Ļ�����Ϣ */
	ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGET_VSCREENINFO, &fb_vinfo);
	ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGET_FSCREENINFO, &fb_fix);
	
	print_screeninfo(fb_vinfo, fb_fix);
	
	LCUI_Sys.screen.bits = fb_vinfo.bits_per_pixel;
	if (fb_vinfo.bits_per_pixel==8) 
		ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGETCMAP, &oldcmap); 
	
	nobuff_print("mapping framebuffer...");
	LCUI_Sys.screen.smem_len = fb_fix.smem_len;/* �����ڴ�ռ��С */
	/* ӳ��֡�������ڴ�ռ� */
	LCUI_Sys.screen.fb_mem = mmap(NULL,fb_fix.smem_len,
							PROT_READ|PROT_WRITE,MAP_SHARED,
							LCUI_Sys.screen.fb_dev_fd,0);
							
	if((void *)-1 == LCUI_Sys.screen.fb_mem)
	{/* ���ʧ�� */
		printf("fail\n");
		perror(strerror(errno));
		exit(-1);
	}
	else printf("success\n");
	
	Graph_Init(&LCUI_Sys.screen.buff); /* ��ʼ��ͼ������ */
	
	LCUI_Sys.screen.buff.flag = NO_ALPHA;/* ��alphaͨ�� */
	LCUI_Sys.screen.buff.type = TYPE_BMP;/* bmpλͼ */
	LCUI_Sys.screen.size.w = fb_vinfo.xres; /* ������Ļ�ߴ� */
	LCUI_Sys.screen.size.h = fb_vinfo.yres; 
	LCUI_Sys.screen.buff.width = fb_vinfo.xres; 
	LCUI_Sys.screen.buff.height = fb_vinfo.yres;
	return 0;
}

int Get_Screen_Bits()
/* ���ܣ���ȡ��Ļ�е�ÿ�����صı�ʾ���õ�λ�� */
{
	return LCUI_Sys.screen.bits;
}

LCUI_Pos Get_Screen_Center_Point()
/* ���ܣ���ȡ��Ļ���ĵ������ */
{
	return Pos(LCUI_Sys.screen.size.w/2.0, LCUI_Sys.screen.size.h/2.0);
}

int LCUI_Active()
/* ���ܣ����LCUI�Ƿ� */
{
	if(LCUI_Sys.status == ACTIVE)
		return 1;
	
	return 0;
}
//extern int debug_mark;
int LCUI_Init(int argc, char *argv[])
/* 
 * ���ܣ����ڶ�LCUI���г�ʼ������ 
 * ˵����ÿ��ʹ��LCUIʵ��ͼ�ν���ĳ��򣬶���Ҫ�ȵ��ô˺�������LCUI�ĳ�ʼ��
 * */
{
	int temp;
	if(LCUI_Sys.init != IS_TRUE)
	{/* ���LCUIû�г�ʼ���� */ 
		srand(time(NULL));/* �����������Ҫ�õ���ֻ����һ�μ��� */
		LCUI_Sys.init = IS_TRUE;
		Print_LCUI_Copyright_Text();
		
		Thread_TreeNode_Init (&LCUI_Sys.thread_tree);	/* ��ʼ�����߳̽�� */
		LCUI_Sys.thread_tree.tid = pthread_self();	/* ��ǰ�߳�ID��Ϊ����� */
		LCUI_Sys.self_id = pthread_self();		/* �����߳�ID */
		/* �趨������ʱ�� */
		LCUI_Sys.max_app_idle_time = MAX_APP_IDLE_TIME;
		LCUI_Sys.max_lcui_idle_time = MAX_LCUI_IDLE_TIME;
		
		EventQueue_Init(&LCUI_Sys.key_event);	/* ��ʼ�������¼����� */
		LCUI_Font_Init (&LCUI_Sys.default_font);/* ��ʼ��Ĭ�ϵ��������� */
		LCUI_AppList_Init (&LCUI_Sys.app_list); /* ��ʼ��LCUI�������� */
		RectQueue_Init (&LCUI_Sys.update_area);	/* ��ʼ����Ļ������¶��� */ 
		WidgetQueue_Init (&LCUI_Sys.widget_list); /* ��ʼ���������� */
		/* ��ʼ�����ڴ����Ѱ��µļ��ļ�ֵ���� */
		Queue_Init(&LCUI_Sys.press_key, sizeof(int), NULL);
		
		/* ��¼������Ϣ */
		temp = LCUI_AppList_Add();
		if(temp != 0)  
			exit(-1);
		
		Core_Start();	/* LCUI�ĺ��Ŀ�ʼ���� */ 
		Screen_Init();	/* ��ʼ����Ļͼ��������� */
		Mouse_Init();	/* ��ʼ����� */
		Cursor_Init();	/* ��ʼ������α� */
		LCUI_IO_Init();	/* ��ʼ����������豸 */ 
		Widget_Event_Init(); /* ��ʼ�������¼����� */
		/* ���浱ǰ��Ļ���ݣ��Ա��˳�LCUI��ԭ */
		Get_Screen_Graph(&LCUI_Sys.screen.buff); 
		//debug_mark = 1;
		/* ����α���� */
		Set_Cursor_Pos(Get_Screen_Center_Point());  
		Show_Cursor();	/* ��ʾ����α� */ 
	}
	else
	{
		temp = LCUI_AppList_Add();
		if(temp != 0) 
			exit(-1);
	}
	/* ע��Ĭ�ϲ������� */
	Register_Default_Widget_Type(); 
	return 0;
}

void *catch()
/* �ڽ�ȡ����ʱ�������������������Ļ���� */
{ 
	LCUI_Graph graph;
	int i=0, tsec=0;
	time_t rawtime;
	struct tm * timeinfo;
	char filename[100];
	Graph_Init(&graph);
	while(1)
	{
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		if(tsec != timeinfo->tm_sec)
			i=0;
			
		sprintf(filename, "%4d-%02d-%02d-%02d-%02d-%02d-%02d.png",
			timeinfo->tm_year+1900, timeinfo->tm_mon+1, 
			timeinfo->tm_mday, timeinfo->tm_hour, 
			timeinfo->tm_min, timeinfo->tm_sec, i++
		);
		tsec = timeinfo->tm_sec;
		
		Catch_Screen_Graph_By_Cache(Rect((Get_Screen_Width()-320)/2.0, 
				(Get_Screen_Height()-240)/2.0, 320, 240), &graph);
		write_png_file(filename, &graph);
		usleep(35000);
	}
}

int Need_Main_Loop(LCUI_App *app)
/* ���ܣ������ѭ���Ƿ���Ҫ�������� */
{
	if(app->stop_loop == IS_TRUE)
		return 0;
	return 1;
}

int LCUI_Main ()
/* 
 * ���ܣ�LCUI�������ѭ��
 * ˵����ÿ��LCUI������Ҫ���������˺������ó���ִ��LCUI���������
 *  */
{
	LCUI_App *app;
	LCUI_ID idle_time = 1500;
	LCUI_Graph graph;
	Graph_Init(&graph);
	//�����̣߳����ڽ�ͼ��
	//pthread_t t;
	//LCUI_Thread_Create(&t, NULL, catch, NULL);
	
	app = Get_Self_AppPointer();
	if(app == NULL)
	{
		printf("LCUI_Main(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	}
	
	while (Need_Main_Loop(app))
	{/* ѭ�������ǳ�����Ҫ�ر� */ 
	
		if(Empty_Widget()) /* û�в������Ͳ���Ҫѭ�� */
			break;
		
		if(Have_Task(app)) /* �������Ҫִ�е����� */
		{
			idle_time = 1500;
			Run_Task(app); 
		}
		else
		{/* ������ͣһ��ʱ�� */
			usleep (idle_time);
			idle_time += 1500;	/* ÿ��ѭ���Ŀ���ʱ��Խ��Խ�� */
			if (idle_time >= LCUI_Sys.max_app_idle_time)
				idle_time = LCUI_Sys.max_app_idle_time;
		}
	}
	
	return App_Quit ();	/* ֱ�ӹرճ��򣬲��ͷ���Դ */ 
}

int Get_LCUI_Version(char *out)
/* ���ܣ���ȡLCUI�İ汾 */
{
	return sprintf(out, "%s", LCUI_VERSION);
}
