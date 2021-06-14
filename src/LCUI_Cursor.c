/* ***************************************************************************
 * LCUI_Cursor.c -- control mouse cursor
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
 * LCUI_Cursor.c -- ���������
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
#include LC_GRAPHICS_H
#include LC_CURSOR_H
#include LC_MISC_H
#include LC_MEM_H

void Refresh_Cursor ()
/* ���ܣ�ˢ������α�����Ļ����ʾ��ͼ�� */
{
	Add_Screen_Refresh_Area ( Get_Cursor_Rect() );
}



void Show_Cursor ()
/* ���ܣ���ʾ����α� */
{
	LCUI_Sys.cursor.visible = IS_TRUE;	/* ��ʶ�α�Ϊ�ɼ� */
	Refresh_Cursor ();			/* ˢ���α����ʾ */
}



void Hide_Cursor ()
/* ���ܣ���������α� */
{
	LCUI_Sys.cursor.visible = IS_FALSE;
	Refresh_Cursor ();
}


LCUI_Rect Get_Cursor_Rect()
/* ���ܣ���ȡ����α������Χ */
{
	LCUI_Rect rect;
	rect.x = LCUI_Sys.cursor.pos.x;
	rect.y = LCUI_Sys.cursor.pos.y;
	rect.width = LCUI_Sys.cursor.graph.width;
	rect.height = LCUI_Sys.cursor.graph.height;
	return rect;
}

void Set_Cursor_Pos (LCUI_Pos pos)
/* ���ܣ��趨�α��λ�� */
{ 
	LCUI_Queue rt;
	LCUI_Rect old;
	RectQueue_Init(&rt);
	old = Get_Cursor_Rect();
 	LCUI_Sys.cursor.pos = pos; 
	Add_Screen_Refresh_Area ( old ); /* ˢ���α�ԭ���������е�ͼ�� */
	Refresh_Cursor ();			/* ˢ���α����ʾ */ 
}

int Set_Cursors_Graph (LCUI_Graph * graph)
/* 
 * ���ܣ��趨�α��ͼ��
 * ����ֵ���趨�ɹ�����0��ʧ���򷵻�-1
*/
{
	if (Valid_Graph (graph))
	{
		Copy_Graph (&LCUI_Sys.cursor.graph, graph);
		Refresh_Cursor();
		return 0;
	}
	return -1;
}

LCUI_Pos Get_Cursor_Pos ()
/* ���ܣ���ȡ���ָ���λ�� */
{
	return LCUI_Sys.cursor.pos; 
}
