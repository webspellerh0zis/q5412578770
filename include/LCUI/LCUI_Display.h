/* ****************************************************************************
 * LCUI_Display.h -- APIs of graphics display.
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
 * LCUI_Build.h -- ��ͼ����ʾ��ص�APIs
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

#ifndef __LCUI_DISPLAY_H__
#define __LCUI_DISPLAY_H__

LCUI_BEGIN_HEADER

int Graph_Display (LCUI_Graph * src, LCUI_Pos pos);
/* ���ܣ���ʾͼ�� */

int Get_Screen_Width ();
/*
 * ���ܣ���ȡ��Ļ���
 * ����ֵ����Ļ�Ŀ�ȣ���λΪ���أ�������ʹ��LCUI_Init()������ʹ�ã�������Ч
 * */ 

int Get_Screen_Height ();
/*
 * ���ܣ���ȡ��Ļ�߶�
 * ����ֵ����Ļ�ĸ߶ȣ���λΪ���أ�������ʹ��LCUI_Init()������ʹ�ã�������Ч
 * */ 

LCUI_Size Get_Screen_Size ();
/* ���ܣ���ȡ��Ļ�ߴ� */ 

void Fill_Pixel(LCUI_Pos pos, LCUI_RGB color);
/* ���ܣ����ָ��λ�õ����ص����ɫ */ 

int Get_Screen_Graph(LCUI_Graph *out);
/* 
 * ���ܣ���ȡ��Ļ����ʾ��ͼ��
 * ˵�����Զ������ڴ��ָ�룬�������ݿ�����ָ����ڴ� 
 * */ 

int Add_Screen_Refresh_Area (LCUI_Rect rect);
/* ���ܣ���������Ļ�������Ҫˢ�µ����� */ 

int Get_Screen_Bits();
/* ���ܣ���ȡ��Ļ�е�ÿ�����صı�ʾ���õ�λ�� */ 

LCUI_Pos Get_Screen_Center_Point();
/* ���ܣ���ȡ��Ļ���ĵ������ */ 

int Widget_Layer_Is_Opaque(LCUI_Widget *widget);
/* ���ܣ��жϲ���ͼ���Ƿ�͸�� */ 

int Widget_Layer_Not_Visible(LCUI_Widget *widget);
/* ���ܣ���ⲿ��ͼ���Ƿ���ȫ͸�� */ 

void Get_Overlay_Widget(LCUI_Rect rect, LCUI_Widget *widget, LCUI_Queue *queue);
/* 
 * ���ܣ���ȡ��ָ�������ص��Ĳ��� 
 * ˵�����õ��Ķ��У������еĲ�������˳��Ϊ����-���� == ��-����
 * */

int Get_Screen_Real_Graph (LCUI_Rect rect, LCUI_Graph * graph);
/* 
 * ���ܣ���ȡ��Ļ��ָ��������ʵ��Ҫ��ʾ��ͼ�� 
 * ˵����ָ��������������벿�����򲻲����ص���
 * */ 

int Enable_Graph_Display();
/* ���ܣ�����ͼ����� */ 

int Disable_Graph_Display();
/* ���ܣ�����ͼ����� */ 

LCUI_END_HEADER

#endif
