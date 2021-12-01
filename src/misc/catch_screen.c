#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_DISPLAY_H

#include <unistd.h>

void Catch_Screen_Graph_By_Cache(LCUI_Rect area, LCUI_Graph *out)
/* 
 * ���ܣ�ͨ���ڴ��е�ͼ�����ݣ���������Ļ����ʾ��ͼ��
 * ˵����Ч�ʽϵͣ���Ϊ��Ҫ���кϳɡ�
 *  */
{
	Get_Screen_Real_Graph(area, out);
}

void Catch_Screen_Graph_By_FB (LCUI_Rect area, LCUI_Graph *out)
/* 
 * ���ܣ�ֱ�Ӷ�ȡ֡�����е�ͼ������
 * ˵����Ч�ʽϸߣ��������ͼ���п��ܻ������⡣
 * */
{
	LCUI_Rect cut_rect;
	unsigned char *dest;
	dest = LCUI_Sys.screen.fb_mem;	/* ָ��֡���� */
	int x, y, n, k, count;
	
	/* �����Ҫ�ü�ͼ�� */
	if ( Get_Cut_Area ( Get_Screen_Size(), area,&cut_rect ) ){
		if(!Rect_Valid(cut_rect)) {
			return;
		}
			
		area.x += cut_rect.x;
		area.y += cut_rect.y;
		area.width = cut_rect.width;
		area.height = cut_rect.height;
	}
	
	Graph_Create(out, area.width, area.height);
	Graph_Lock (out, 1); 
	/* ֻ����������32λ��ʾ������ʾ��ͼ�Σ��д����� */
	for (n=0,y=0; y < area.height; ++y) {
		k = (area.y + y) * LCUI_Sys.screen.size.w + area.x;
		for (x = 0; x < area.width; ++x) {
			count = k + x;
			count = count << 2;  
			out->rgba[2][n] = dest[count];
			out->rgba[1][n] = dest[count + 1];
			out->rgba[0][n] = dest[count + 2];
			++n;
		}
	}
	Graph_Unlock (out);
}

static int need_break = IS_FALSE;
static pthread_t t;
static void *catch(void *arg)
/* �ڽ�ȡ����ʱ�������������������Ļ���� */
{
	LCUI_Graph graph;
	LCUI_Rect *area;
	int i=0, tsec=0;
	time_t rawtime;
	struct tm * timeinfo;
	char filename[100];
	
	Graph_Init(&graph);
	area = (LCUI_Rect*)arg;
	
	while(!need_break) {
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
		
		Catch_Screen_Graph_By_FB( *area, &graph );
		write_png( filename, &graph );
		usleep(35000);
	}
	LCUI_Thread_Exit(NULL);
}

int start_catch_screen()
{
	need_break = IS_FALSE;
	return LCUI_Thread_Create(&t, NULL, catch, NULL);
}

int end_catch_screen()
{
	need_break = IS_TRUE;
	return LCUI_Thread_Join(t, NULL);
}
