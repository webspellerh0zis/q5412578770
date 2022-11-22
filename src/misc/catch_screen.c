/* ***************************************************************************
 * catch_screen.c -- screenshot support
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
 * catch_screen.c -- ��Ļ��ͼ֧��
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
	
	if( !LCUI_Active() ) {
		return;
	}
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

static int need_break = FALSE;
static pthread_t t = 0;
static LCUI_Rect target_area;
static void *catch()
/* �ڽ�ȡ����ʱ�������������������Ļ���� */
{
	LCUI_Graph graph;
	LCUI_Rect area;
	int i=0, tsec=0;
	time_t rawtime;
	struct tm * timeinfo;
	char filename[100];
	
	Graph_Init(&graph);
	area = target_area;
	
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
		
		Catch_Screen_Graph_By_FB( area, &graph );
		write_png( filename, &graph );
		usleep(35000);
	}
	LCUI_Thread_Exit(NULL);
}

int start_record_screen( LCUI_Rect area )
/* ¼����Ļָ����������� */
{
	if( t != 0 ) {
		return -1;
	}
	need_break = FALSE;
	target_area = area;
	return LCUI_Thread_Create(&t, NULL, catch, NULL );
}

int end_catch_screen()
/* ����¼�� */
{
	if( t == 0 ) {
		return -1;
	}
	need_break = TRUE;
	return LCUI_Thread_Join(t, NULL);
}
