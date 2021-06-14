/* ***************************************************************************
 * LCUI_Misc.h -- no specific categories of miscellaneous function
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
 * LCUI_Misc.h -- û�о������������
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
 

#ifndef __LCUI_MISC_H__
#define __LCUI_MISC_H__
void String_Init(LCUI_String *in);
/* ���ܣ���ʼ��String�ṹ���е����� */ 

void Rect_Init(LCUI_Rect *rect);
/* ���ܣ���ʼ��������������� */ 

void Border_Init(LCUI_Border *in);
/* ���ܣ���ʼ���߿� */ 

void Strcpy (LCUI_String * des, char *src);
/* ���ܣ������ַ�����String�ṹ�������� */ 

int Strcmp(LCUI_String *str1, char *str2);
/* ���ܣ��Ա�str1��str2 */ 

void Get_Moved_Rect_Refresh_Area (int new_x, int new_y, LCUI_Rect rect,
							 LCUI_Rect * rect_a, LCUI_Rect * rect_b);
/* ���ܣ���ȡһ�������ƶ�����Ҫˢ�µĲ�������Ҳ����A��B�������� */ 

int Rect_Cross_Overlay(LCUI_Rect a, LCUI_Rect b);
/* 
 * ���ܣ�������������Ƿ��ʮ�ּ�ʽ���� 
 * ����ֵ��
 *  1 a����b��
 * -1 a�⣬b��
 *  0 ������
 **/  

int Check_Option(int result, int option);
/* ���ܣ����result�Ƿ����option */ 

int Get_Cut_Area(LCUI_Size container, LCUI_Rect rect, LCUI_Rect *cut);
/* 
 * ���ܣ���ȡ��ü�������
 * ˵����ָ�������ߴ�������е�����λ�ü��ߴ磬���ɵõ�����������Ҫ���вü�����
 *  */ 

LCUI_Rect Get_Valid_Area(LCUI_Size container, LCUI_Rect rect);
/* 
 * ���ܣ���ȡָ�������������е���Ч��ʾ���� 
 * ˵����ָ�������������С����ָ�������е�����λ�ü���С�����ܵõ�������ʵ������ʾ
 * ���ĸ�����Χ��
 * */ 

int
Rect_Include_Rect (LCUI_Rect a, LCUI_Rect b);
/*
 * ���ܣ�������������У�A�����Ƿ����B����
 * ����ֵ�������������ڰ��й�ϵ����1�����򷵻�0��
 * */ 

LCUI_Pos Pos(int x, int y);
/* ���ܣ�ת����LCUI_Pos���� */ 

LCUI_Size Size(int w, int h);
/* ���ܣ�ת����LCUI_Size���� */ 

int Size_Cmp(LCUI_Size a, LCUI_Size b);
/* 
 * ���ܣ��Ա������ߴ�
 * ˵����a����b������1�� b����a������-1������򷵻�0
 * */ 

int Cut_Overlay_Rect (	LCUI_Rect old, LCUI_Rect new, 
						LCUI_Queue *rq	);
/*
 * ���ܣ������ص����ֵ��������Σ����зָ���õ��ָ��ľ���
 * ˵������Ҫ���ھֲ�����ˢ�����ӵ���ˢ�µ������п��ܻ�������ӵ������ص���Ϊ����
 * �ظ�ˢ��ͬһ��������Ҫ�����ʱ�Ծ��ν��зָ�õ���ȫ�ص��Ͳ��ص��ľ��Ρ�
 * ����˵����
 * old �� �Ѵ��ڵľ�������
 * new �� ��Ҫ��ӵľ�������
 * rq  �� ָ����εĶ��е�ָ��
 * ע�⣡���ݲ���ʱ������ߵ�old��newλ�á�
 **/ 

int Get_Overlay_Rect(LCUI_Rect a, LCUI_Rect b, LCUI_Rect *out);
/* ���ܣ���ȡ�������ص����ֵľ��� */ 

LCUI_Rect Rect (int x, int y, int width, int height);
/* ���ܣ�����ֵת����LCUI_Rect�ͽṹ�� */ 

LCUI_Border Border(int top, int bottom, int left, int right);

LCUI_Pos Pos_Add(LCUI_Pos a, LCUI_Pos b);
/* ���ܣ�������LCUI_Pos���ͱ����ĺ� */ 

int Pos_Cmp(LCUI_Pos a, LCUI_Pos b);
/* ���ܣ��Ա����������Ƿ�һ�� */ 

LCUI_Pos Pos_Sub(LCUI_Pos a, LCUI_Pos b);
/* ���ܣ�������LCUI_Pos���ͱ����Ĳ� */ 

int Rect_Inside_Point (LCUI_Pos pos, LCUI_Rect rect);
/* ���ܣ����һ�����Ƿ񱻾��ΰ��� */ 

int
Check_Rect_Overlap (int ax, int ay, int aw, int ah, int bx, int by, int bw,
					int bh);
/*
* ���ܣ�������������Ƿ��ص�
* ����˵����
* ax ������A�����Ͻ��������x�������
* ay ������A�����Ͻ��������y�������
* aw ������A�Ŀ��
* ah ������A�ĸ߶�
* bx ������B�����Ͻ��������x�������
* by ������B�����Ͻ��������y�������
* bw ������B�Ŀ��
* bh ������B�ĸ߶�
* ����ֵ�������ص�����0���ص��򷵻�1��
*/ 

int
Rect_Is_Overlay (LCUI_Rect a, LCUI_Rect b);
/* ���ܣ�������������Ƿ��ص� */ 

int Rect_Equal (LCUI_Rect a, LCUI_Rect b);
/* ���ܣ��ж����������Ƿ���� */ 

int Rect_Valid(LCUI_Rect r);
/* ���ܣ��жϾ����Ƿ���Ч */ 
#endif
