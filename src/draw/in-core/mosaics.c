/* ***************************************************************************
 * mosaics.c -- mosaics graphic
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
 * mosaics.c -- ������ͼ��
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
#include LC_MEM_H

#include <stdio.h>
#include <string.h>

int Load_Graph_Mosaics(LCUI_Graph *pic)
/* ���ܣ�����������ͼ�� */
{
	unsigned char color[]={
		78,78,78,78,78,78,78,78,121,121,121,121,121,121,121,121,
		78,78,78,78,78,78,78,78,121,121,121,121,121,121,121,121,
		78,78,78,78,78,78,78,78,121,121,121,121,121,121,121,121,
		78,78,78,78,78,78,78,78,121,121,121,121,121,121,121,121,
		78,78,78,78,78,78,78,78,121,121,121,121,121,121,121,121,
		78,78,78,78,78,78,78,78,121,121,121,121,121,121,121,121,
		78,78,78,78,78,78,78,78,121,121,121,121,121,121,121,121,
		78,78,78,78,78,78,78,78,121,121,121,121,121,121,121,121,
		121,121,121,121,121,121,121,121,78,78,78,78,78,78,78,78,
		121,121,121,121,121,121,121,121,78,78,78,78,78,78,78,78,
		121,121,121,121,121,121,121,121,78,78,78,78,78,78,78,78,
		121,121,121,121,121,121,121,121,78,78,78,78,78,78,78,78,
		121,121,121,121,121,121,121,121,78,78,78,78,78,78,78,78,
		121,121,121,121,121,121,121,121,78,78,78,78,78,78,78,78,
		121,121,121,121,121,121,121,121,78,78,78,78,78,78,78,78,
		121,121,121,121,121,121,121,121,78,78,78,78,78,78,78,78,
	};
	int value;
	if(Graph_Valid(pic))
	{
		Graph_Free(pic);
	}
	Graph_Init(pic);
	pic->have_alpha = IS_FALSE;
	pic->type = TYPE_PNG;
	pic->alpha = 255;
	value = Graph_Create(pic,16,16);
	if(value == 0){
		memcpy(pic->rgba[0],color,sizeof(color));
		memcpy(pic->rgba[1],color,sizeof(color));
		memcpy(pic->rgba[2],color,sizeof(color));
	}
	return value;
}
