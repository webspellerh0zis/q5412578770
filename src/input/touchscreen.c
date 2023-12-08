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

#ifdef USE_TSLIB
#include <tslib.h> 
#endif

/*********** ������� **************/
typedef struct _LCUI_TS
{
	void *td;
	int state;
}
LCUI_TS;
/**********************************/

static LCUI_TS ts_data;

static BOOL proc_touchscreen()
{
#ifdef USE_TSLIB
	struct ts_sample samp;
	int button, ret;
	static LCUI_Pos pos;
	
	if (ts_data.state != INSIDE) {
		return FALSE;
	}

	/* ��ʼ��ȡ��������������� */ 
	ret = ts_read (ts_data.td, &samp, 1); 
	if (ret < 0) {
		perror ("ts_read");
		return FALSE;
	}

	if (ret != 1) {
		return FALSE;
	}
	pos.x = samp.x;
	pos.y = samp.y;
	
	if (pos.x > LCUIScreen_GetWidth ()) {
		pos.x = LCUIScreen_GetWidth ();
	}
	if (pos.y > LCUIScreen_GetHeight ()) {
		pos.y = LCUIScreen_GetHeight ();
	}
	pos.x = pos.x<0 ? 0:pos.x; 
	pos.y = pos.y<0 ? 0:pos.y; 
	button = samp.pressure > 0 ? 1:0;
	LCUI_PushMouseEvent( pos, button );
	//printf("%ld.%06ld: %6d %6d %6d\n", samp.tv.tv_sec, samp.tv.tv_usec, samp.x, samp.y, samp.pressure); 
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL Enable_TouchScreen_Input()
/* ���ܣ����ô������봦�� */
{ 
#ifdef USE_TSLIB
	char *tsdevice;
	char str[256];
	if (ts_data.state != INSIDE) {
		tsdevice = getenv("TSLIB_TSDEVICE");
		if( tsdevice ) {
			ts_data.td = ts_open(tsdevice, 0);
		} else {
			tsdevice = TS_DEV;
		}
		ts_data.td = ts_open (tsdevice, 0);
		if (!ts_data.td) {
			sprintf (str, "ts_open: %s", tsdevice);
			perror (str);
			ts_data.state = REMOVE;
			return FALSE;
		}

		if (ts_config (ts_data.td)) {
			perror ("ts_config");
			ts_data.state = REMOVE;
			return FALSE;
		}
		ts_data.state = INSIDE;
	}
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL Disable_TouchScreen_Input()
/* ���ܣ������������봦�� */
{
#ifdef USE_TSLIB
	if(ts_data.state == INSIDE) {
		ts_close(ts_data.td);
		ts_data.state = REMOVE;
		return TRUE;
	}
	return FALSE;
#else
	return FALSE;
#endif
}

void *Get_TouchScreen()
/* ��ȡ�����豸�ļ���� */
{
	return ts_data.td;
}

/* ��ʼ����������ģ�� */
void LCUIModule_TouchScreen_Init( void )
{
	ts_data.td = NULL;
	ts_data.state = REMOVE;
	LCUI_Dev_Add( Enable_TouchScreen_Input, proc_touchscreen,
			Disable_TouchScreen_Input );
}
