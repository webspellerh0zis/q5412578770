/* ***************************************************************************
 * device.c -- The input device processing module
 * 
 * Copyright (C) 2013 by
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
 * device.c -- 输入设备的处理模块
 *
 * 版权所有 (C) 2013 归属于
 * 刘超
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/
#include <LCUI_Build.h>
#include LC_LCUI_H

/*----------------------------- Device -------------------------------*/
/* 设备列表初始化 */
static void 
dev_list_init( LCUI_Queue *dev_list )
{
	Queue_Init( dev_list, sizeof(dev_func_data), NULL );
}

/* 
 * 功能：注册设备
 * 说明：为指定设备添加处理函数
 * */
LCUI_EXPORT(int)
LCUIDevice_Add(	LCUI_BOOL (*init_func)(void), 
		LCUI_BOOL (*proc_func)(void), 
		LCUI_BOOL (*destroy_func)(void) )
{
	dev_func_data data;
	
	if( init_func ) {
		init_func();
	}
	data.init_func = init_func;
	data.proc_func = proc_func;
	data.destroy_func = destroy_func;
	if( 0<= Queue_Add( &LCUI_Sys.dev_list, &data ) ) {
		return 0;
	}
	return -1;
}

/* 处理列表中的设备的数据 */
static void
proc_dev_list ( void *arg )
{
	LCUI_Queue *dev_list;
	dev_func_data *data_ptr;
	int total, i, result, sleep_time = 1;
	
	dev_list = (LCUI_Queue *)arg;
	while( LCUI_Active() ) {
		result = 0;
		total = Queue_Get_Total( dev_list );
		for(i=0; i<total; ++i) {
			data_ptr = Queue_Get( dev_list, i );
			if( !data_ptr || !data_ptr->proc_func ) {
				continue;
			}
			result += data_ptr->proc_func();
		}
		if( result > 0 ) {
			sleep_time = 1;
		} else {
			LCUI_MSleep( sleep_time );
			if( sleep_time < 100 ) {
				sleep_time += 1;
			}
		}
	}
	LCUIThread_Exit(NULL);
}

/* 初始化设备处理模块 */
LCUI_EXPORT(int)
LCUIModule_Device_Init(void)
{
	dev_list_init( &LCUI_Sys.dev_list );
	return _LCUIThread_Create( &LCUI_Sys.dev_thread,
			proc_dev_list, &LCUI_Sys.dev_list );
}

/* 停用设备处理模块 */
LCUI_EXPORT(void)
LCUIModule_Device_End(void)
{
	int total, i;
	dev_func_data *data_ptr;
	LCUI_Queue *dev_list;
	
	dev_list = &LCUI_Sys.dev_list; 
	total = Queue_Get_Total( dev_list );
	for(i=0; i<total; ++i) {
		data_ptr = Queue_Get( dev_list, i );
		if( !data_ptr || !data_ptr->destroy_func ) {
			continue;
		}
		data_ptr->destroy_func();
	}
}
/*--------------------------- End Device -----------------------------*/
