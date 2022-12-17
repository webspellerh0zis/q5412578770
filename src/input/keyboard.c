/* ***************************************************************************
 * keyboard.c -- keyboard support
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
 * keyboard.c -- ����֧��
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

#include <termios.h>
#include <unistd.h>
#include <sys/fcntl.h>


static struct termios tm;//, tm_old; 
static int fd = STDIN_FILENO;

int Set_Raw(int t)
{
	if (t > 0) {
		if(tcgetattr(fd, &tm) < 0) {
			return -1; 
		} 
		/* �����ն�Ϊ�޻����޻���ģʽ */
		tm.c_lflag &= ~(ICANON|ECHO);
		tm.c_cc[VMIN] = 1;
		tm.c_cc[VTIME] = 0;
		if(tcsetattr(fd, TCSANOW, &tm) < 0) {
			return -1; 
		}
		printf("\033[?25l");/* ���ع�� */
	} else {
		tm.c_lflag |= ICANON;
		tm.c_lflag |= ECHO;
		tm.c_cc[VMIN] = 1;
		tm.c_cc[VTIME] = 0;
		if(tcsetattr(fd,TCSANOW,&tm)<0) {
			return -1;
		}
		printf("\033[?25h"); /* ��ʾ��� */ 
	}
	return 0;
}

int Check_Key(void)  
/* 
 * ���ܣ�����Ƿ��а������� 
 * ����ֵ��
 *   1   �а�������
 *   2   �ް�������
 * */
{
	struct termios oldt;//, newt;  
	int ch;  
	int oldf;  
	tcgetattr(STDIN_FILENO, &oldt);  
	//newt = oldt;  
	//newt.c_lflag &= ~(ICANON | ECHO);  
	//tcsetattr(STDIN_FILENO, TCSANOW, &newt);  
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);  
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);  
	ch = getchar();  /* ��ȡһ���ַ� */
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  
	fcntl(STDIN_FILENO, F_SETFL, oldf);  
	if(ch != EOF)  
	{/* ����ַ���Ч�����ַ��Ż����뻺������ */
		ungetc(ch, stdin);  
		return 1;  
	} 
	return 0;
}


int Get_Key(void)
/* ���ܣ���ȡ�����µİ����ļ�ֵ */
{ 
	int k,c;
	static int input, count = 0;
	++count;
	k = fgetc(stdin); /* ��ȡ���뻺���е��ַ� */
	input += k; 
	if(Check_Key()) {/* ��������ַ��ڻ����� */
		Get_Key(); /* �ݹ���� */
	}
	c = input;
	--count;
	if(count == 0) {/* �ݹ���������0 */
		input = 0;
	}
	if(c == 3) {
		exit(1);
	}
	return c; 
}

int Find_Pressed_Key(int key)
/*
 * ���ܣ����ָ����ֵ�İ����Ƿ��ڰ���״̬
 * ����ֵ��
 *   1 ����
 *   0 ������
 **/
{
	int t;
	int i, total;
	
	total = Queue_Get_Total(&LCUI_Sys.press_key);
	for(i=0; i<total; ++i) {
		t = *(int*)Queue_Get(&LCUI_Sys.press_key, i);
		if(t == key) {
			return 1;
		}
	}
	return 0;
}


void Send_Keyboard_Event( LCUI_Key key )
{
	LCUI_Key *key_data;
	LCUI_Func *func;
	int total, i;
	
	total = Queue_Get_Total( &LCUI_Sys.key_event );
	if( total <= 0 ) {
		return;
	}
	key_data = (LCUI_Key*)malloc( sizeof(LCUI_Key) );
	memcpy( key_data, &key, sizeof(LCUI_Key) );
	for (i = 0; i < total; ++i) {
		func = Queue_Get( &LCUI_Sys.key_event, i );
		/* Ϊ�ڶ��������������ڴ棬��Ҫ�ڵ�����ص������������� */
		func->arg[0] = key_data;
		func->destroy_arg[0] = TRUE;
		/* ����������������� */
		Send_Task_To_App( func );
	} 
}


static BOOL proc_keyboard()
{
	LCUI_Key key;
	LCUI_Rect area;
	LCUI_Graph graph;
	
	Graph_Init(&graph);
	area.width = 320;
	area.height = 240;
	area.x = (Get_Screen_Width()-area.width)/2;
	area.y = (Get_Screen_Height()-area.height)/2;
	if ( !Check_Key ()) { /* ���û�а������� */ 
		return FALSE;
	}
	key.code = Get_Key (); 
	#define __NEED_CATCHSCREEN__
	#ifdef __NEED_CATCHSCREEN__
	if(key.code == 'c')
	{//������c���󣬿��Խ��н�ͼ��ֻ��ȡָ�������ͼ��
		time_t rawtime;
		struct tm * timeinfo;
		char filename[100];
		time ( &rawtime );
		timeinfo = localtime ( &rawtime ); /* ��ȡϵͳ��ǰʱ�� */ 
		sprintf(filename, "%4d-%02d-%02d-%02d-%02d-%02d.png",
			timeinfo->tm_year+1900, timeinfo->tm_mon+1, 
			timeinfo->tm_mday, timeinfo->tm_hour, 
			timeinfo->tm_min, timeinfo->tm_sec
		);
		Catch_Screen_Graph_By_FB( area, &graph );
		write_png(filename, &graph);
	}
	else if(key.code == 'r') {
		/* �������r������¼��ָ�������ͼ�� */
		start_record_screen( area );
	}
	#endif
	/* ��������й����İ����¼� */
	Send_Keyboard_Event( key );
	Graph_Free(&graph);
	return TRUE;
}

BOOL Enable_Key_Input()
/* ���ܣ����ð������봦�� */
{
	Set_Raw(1);/* �����ն����� */
	return TRUE;
}

BOOL Disable_Key_Input()
/* ���ܣ������������봦�� */
{
	Set_Raw(0);/* �ָ��ն����� */
	return TRUE;
}

void Keyboard_Init()
{
	LCUI_Dev_Add( Enable_Key_Input, proc_keyboard, Disable_Key_Input);
}
/*************************** Key End **********************************/
