/* ***************************************************************************
 * touchscreen.c -- touchscreen support
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
 * touchscreen.c -- ����֧��
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
#include LC_CURSOR_H
#include LC_WIDGET_H
#include LC_DISPLAY_H
#include LC_INPUT_H

static void * Handle_TouchScreen_Input ()
/* ���ܣ����������� */
{
	char *tsdevice;
	struct ts_sample samp;
	int button, x, y, ret;
	LCUI_MouseEvent event;
	
	char str[100];
	while (LCUI_Active()) {
		if (LCUI_Sys.ts.status != INSIDE) {
			tsdevice = getenv("TSLIB_TSDEVICE");
			if( tsdevice != NULL ) {
				LCUI_Sys.ts.td = ts_open(tsdevice, 0);
			} else {
				tsdevice = TS_DEV;
			}
			LCUI_Sys.ts.td = ts_open (tsdevice, 0);
			if (!LCUI_Sys.ts.td) { 
				sprintf (str, "ts_open: %s", tsdevice);
				perror (str);
				LCUI_Sys.ts.status = REMOVE;
				break;
			}

			if (ts_config (LCUI_Sys.ts.td)) {
				perror ("ts_config");
				LCUI_Sys.ts.status = REMOVE;
				break;
			}
			LCUI_Sys.ts.status = INSIDE;
		}

		/* ��ʼ��ȡ��������������� */ 
		ret = ts_read (LCUI_Sys.ts.td, &samp, 1); 
		if (ret < 0) {
			perror ("ts_read");
			continue;
		}

		if (ret != 1) {
			continue;
		}
		x = samp.x;
		y = samp.y;
		
		if (x > Get_Screen_Width ()) {
			x = Get_Screen_Width ();
		}
		if (y > Get_Screen_Height ()) {
			y = Get_Screen_Height ();
		}
		if (x < 0) {
			x = 0;
		}
		if (y < 0) {
			y = 0;
		}
		/* �趨�α�λ�� */ 
		Set_Cursor_Pos (Pos(x, y));
		
		event.global_pos.x = x;
		event.global_pos.y = y;
		/* ��ȡ��ǰ���ָ�븲�ǵ��Ĳ�����ָ�� */
		event.widget = Get_Cursor_Overlay_Widget();
		/* ����и��ǵ��Ĳ���������Ҫ�������ָ���벿����������� */
		if(event.widget != NULL) {
			event.pos.x = x - Get_Widget_Global_Pos(event.widget).x;
			event.pos.y = y - Get_Widget_Global_Pos(event.widget).y;
		} else {/* ���򣬺�ȫ������һ�� */
			event.pos.x = x;
			event.pos.y = y;
		}
		if (samp.pressure > 0) {
			button = 1; 
		} else {
			button = 0; 
		}
			/* ��������¼� */
		Handle_Mouse_Event(button, &event); 
		//printf("%ld.%06ld: %6d %6d %6d\n", samp.tv.tv_sec, samp.tv.tv_usec, samp.x, samp.y, samp.pressure);
	}
	if(LCUI_Sys.ts.status == INSIDE) {
		ts_close(LCUI_Sys.ts.td); 
	}
	LCUI_Sys.ts.status = REMOVE;
	thread_exit (NULL);
}

int Enable_TouchScreen_Input()
/* ���ܣ����ô������봦�� */
{ 
	if(LCUI_Sys.ts.status == REMOVE) {
		return  thread_create ( &LCUI_Sys.ts.thread, NULL, 
					Handle_TouchScreen_Input, NULL ); 
	}
	return 0;
}

int Disable_TouchScreen_Input()
/* ���ܣ������������봦�� */
{
	if(LCUI_Sys.ts.status == INSIDE) {
		return thread_cancel ( LCUI_Sys.ts.thread ); 
	}
	return 0;
}

