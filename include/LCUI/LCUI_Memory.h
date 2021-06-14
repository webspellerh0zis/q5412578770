/* ***************************************************************************
 * LCUI_Memory.h -- Some of the data type of memory management
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
 * LCUI_Memory.h -- һЩ�������͵��ڴ����
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

#ifndef __LCUI_MEMORY_H__ /* ���û�ж��� __LCUI_MEMORY_H__ �� */
#define __LCUI_MEMORY_H__  /* ���� __LCUI_MEMORY_H__ �� */

void Using_Graph(LCUI_Graph *pic, int mode);
/* ���ܣ���ָ��ģʽʹ��ͼ������
 * ˵��������mode��ֵΪ0ʱ���ԡ�����ģʽʹ�����ݣ�����ֵʱ���ԡ�д"ģʽʹ������ */ 

void End_Use_Graph(LCUI_Graph *pic);
/* ���ܣ�����ͼ�����ݵ�ʹ�� */ 

unsigned char** Get_Malloc(int width,int height,int flag);
/* ���ܣ�Ϊͼ�����������ڴ�ռ䣬����ʼ�����ڴ�ռ�Ϊ�� */ 
 
void Free_Graph(LCUI_Graph *pic);
/* ���ܣ��ͷ�LCUI_Graph�ڵ�ͼ������ռ�õ��ڴ���Դ */ 

void *Malloc_Widget_Private(LCUI_Widget *widget, size_t size);
/* ���ܣ�Ϊ����˽�нṹ��ָ������ڴ� */ 

int Malloc_Graph(LCUI_Graph *pic, int width, int height);
/* ���ܣ�Ϊͼ�����ݷ����ڴ���Դ */ 

void Copy_Graph(LCUI_Graph *des, LCUI_Graph *src);
/* 
 * ���ܣ�����ͼ������
 * ˵������src�����ݿ�����des 
 * */ 

void Free_String(LCUI_String *in);
/* ���ܣ��ͷ�String�ṹ���е�ָ��ռ�õ��ڴ�ռ� */ 

void Free_Bitmap(LCUI_Bitmap *bitmap);
/* ���ܣ��ͷŵ�ɫλͼռ�õ��ڴ���Դ������ʼ�� */ 

void Free_WChar_T(LCUI_WChar_T *ch);
/* ���ܣ��ͷ�LCUI_Wchar_T�����е�ָ�����ռ�õ��ڴ� */ 

void Free_WString(LCUI_WString *str);
/* ���ܣ��ͷ�LCUI_WString�ͽṹ���е�ָ�����ռ�õ��ڴ� */ 

void Malloc_Bitmap(LCUI_Bitmap *bitmap, int width, int height);
/* ���ܣ�ΪBitmap�ڵ����ݷ����ڴ���Դ������ʼ�� */ 

void Realloc_Bitmap(LCUI_Bitmap *bitmap, int width, int height);
/* ���ܣ�����λͼ�ĳߴ� */ 

void Free_Font(LCUI_Font *in);
/* ���ܣ��ͷ�Font�ṹ������ռ�õ��ڴ���Դ */ 

void Free_LCUI_Font();
/* ���ܣ��ͷ�LCUIĬ�ϵ�Font�ṹ������ռ�õ��ڴ���Դ */ 
#endif  /*  __LCUI_MEMORY_H__ */

