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
			}
			else tsdevice = TS_DEV;
			LCUI_Sys.ts.td = ts_open (tsdevice, 0);
			if (!LCUI_Sys.ts.td) { 
				sprintf (str, "ts_open: %s", tsdevice);
				perror (str);
				LCUI_Sys.ts.status = REMOVE;
				break;
			}

			if (ts_config (LCUI_Sys.ts.td))
			{
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

		if (ret != 1) continue;

		x = samp.x;
		y = samp.y;
		
		if (x > Get_Screen_Width ())
			x = Get_Screen_Width ();
		if (y > Get_Screen_Height ())
			y = Get_Screen_Height ();
		if (x < 0) x = 0;
		if (y < 0) y = 0;
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
		if (samp.pressure > 0)  button = 1; 
		else  button = 0; 
			/* ��������¼� */
		Handle_Mouse_Event(button, &event); 
		//printf("%ld.%06ld: %6d %6d %6d\n", samp.tv.tv_sec, samp.tv.tv_usec, samp.x, samp.y, samp.pressure);
	}
	if(LCUI_Sys.ts.status == INSIDE) {
		ts_close(LCUI_Sys.ts.td); 
	}
	LCUI_Sys.ts.status = REMOVE;
	pthread_exit (NULL);
}

int Enable_TouchScreen_Input()
/* ���ܣ�����������봦�� */
{
	/* ����һ���̣߳�����ˢ��ʾ���ָ�� */
	if(LCUI_Sys.ts.status == REMOVE) {
		return  pthread_create ( &LCUI_Sys.ts.thread, NULL, 
					Handle_TouchScreen_Input, NULL ); 
	}
	return 0;
}

int Disable_TouchScreen_Input()
/* ���ܣ�����������봦�� */
{
	if(LCUI_Sys.ts.status == INSIDE) {
		return pthread_cancel ( LCUI_Sys.ts.thread );/* ����LCUI���߳� */ 
	}
	return 0;
}

