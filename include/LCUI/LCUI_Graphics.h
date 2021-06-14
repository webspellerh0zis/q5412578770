/* ***************************************************************************
 * LCUI_Graphics.h -- graphics handle
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
 * LCUI_Graphics.h -- ͼ�δ���
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
#ifndef __LCUI_GRAPHICS_H__
#define __LCUI_GRAPHICS_H__
LCUI_RGB
RGB (unsigned char red, unsigned char green,
	 unsigned char blue);
/* ���ܣ���������ɫֵת����LCUI_RGB������ */

int Get_Graph_Type(LCUI_Graph *pic);
/* ����ͼ�ε����� */ 

LCUI_Size Get_Graph_Size(LCUI_Graph *pic);
/* ���ܣ���ȡͼ�εĳߴ� */ 

int Graph_Is_PNG(LCUI_Graph *pic);
/* 
 * ���ܣ��ж�ͼ���Ƿ�Ϊpng
 * ����ֵ�����򷵻�1�����򷵻�0
 * */ 

int Graph_Is_BMP(LCUI_Graph *pic);
/* 
 * ���ܣ��ж�ͼ���Ƿ�Ϊbmp
 * ����ֵ�����򷵻�1�����򷵻�0
 * */ 

int Graph_Is_JPG(LCUI_Graph *pic);
/* 
 * ���ܣ��ж�ͼ���Ƿ�Ϊjpg 
 * ����ֵ�����򷵻�1�����򷵻�0
 * */ 

int Graph_Have_Alpha(LCUI_Graph *pic);
/* 
 * ���ܣ��ж�ͼ���Ƿ���д�alphaͨ�� 
 * ����ֵ�����򷵻�1�����򷵻�0
 * */ 

int Valid_Graph(LCUI_Graph *pic);
/*
 * ���ܣ����ͼ�������Ƿ���Ч
 * ����ֵ����Ч����1����Ч����0
 */ 

int Valid_Bitmap(LCUI_Bitmap *bitmap);
/*
 * ���ܣ����λͼ�����Ƿ���Ч
 * ����ֵ����Ч����1����Ч����0
 */ 

void Print_Bitmap_Info(LCUI_Bitmap *bitmap);
/* ���ܣ���ӡλͼ����Ϣ */ 

void Print_Graph_Info(LCUI_Graph *pic);
/* ���ܣ���ӡͼ�����Ϣ */ 

void Bitmap_Init(LCUI_Bitmap *in);
/* ���ܣ���ʼ��LCUI_Bitmap�ṹ������ */ 

void Graph_Init(LCUI_Graph *pic);
/* ��ʼ��ͼƬ���ݽṹ�� */ 

void Zoom_Graph(LCUI_Graph *in, LCUI_Graph *out, int flag, LCUI_Size size);
/* ���ܣ�����ָ��ģʽ����ͼ��������� */ 

int Load_Image(char *filepath, LCUI_Graph *out);
/* 
 * ���ܣ�����ָ��ͼƬ�ļ���ͼ������
 * ˵������ͼƬ�ļ������������ڴ棬�򿪵�ͼƬ�ļ�Խ��ռ�õ��ڴ�Ҳ��Խ�� 
 * */ 

int Cut_Graph(LCUI_Graph *src, LCUI_Rect rect, LCUI_Graph *out);
/* 
 * ���ܣ���Դͼ���н�ȡ��ָ�������ͼ�� 
 * ˵�����ü�����ͼ�����ݣ�ȫ��͸���Ⱥ�Դͼ��һ��
 * */ 

int Graph_Flip_Horizontal(LCUI_Graph *src, LCUI_Graph *out);
/* ���ܣ���ͼ�����ˮƽ��ת */ 

int Cut_And_Overlay_Graph(LCUI_Graph *src, LCUI_Rect cut, LCUI_Pos put, LCUI_Graph *des);
/*
 * ���ܣ���Դͼ���н�ȡ��һ��ͼ�β�����alphaͨ������������ͼ��
 * ˵������������ǽ�ȡ�͸���һ����еģ��ڽ�ȡ��ͬʱ����ͼ�θ�����Ŀ��ͼ���ϣ�Ч�ʱȷֲ����ú�����һ�㡣
 */ 

int Cut_And_Replace_Graph(LCUI_Graph *src, LCUI_Rect cut, LCUI_Pos put, LCUI_Graph *des);
/*
 * ���ܣ���Դͼ���н�ȡ��һ��ͼ�β�����������ͼ��
 * ˵����������������ͬ������ǽ�ȡ�ͺϳ�һ����еģ��ڽ�ȡ��ͬʱ����ͼ�θ�����Ŀ��ͼ���ϣ�Ч�ʱȷֲ����ú�����һ�㡣
 */ 

int Get_Screen_Graph(LCUI_Graph *out);
/* 
 * ���ܣ���ȡ��Ļ����ʾ��ͼ��
 * ˵�����Զ������ڴ��ָ�룬�������ݿ�����ָ����ڴ� 
 * */ 

void Fill_Color(LCUI_Graph *pic, LCUI_RGB color);
/* ���ܣ�Ϊ�����ͼ�������ɫ */ 

void Tile_Graph(LCUI_Graph *src, LCUI_Graph *out, int width, int height);
/* 
 * ���ܣ�ƽ��ͼ��
 * ����˵����
 * src ԭʼͼ��
 * out �����ͼ��
 * width ���Ŀ��
 * height ���ĸ߶�
 **/ 

int Mix_Graph(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos);
/* 
 * ���ܣ���ǰ��ͼ�뱳��ͼ��ϵ���
 * ˵����back_graph�Ǳ���ͼ��fore_graphǰ��ͼ�񣬻�Ϻ�����ݱ����ڱ���ͼ��
 */ 

int Replace_Graph(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos);
/* 
 * ���ܣ��滻ԭͼ�ж�Ӧλ�õ������е�ͼ��
 * ˵������Overlay_Graph������ͬ��back_graph�е�ͼ�����ݻᱻfore_graph�е�ͼ�����ݸ���
 */ 

int Align_Image(LCUI_Graph *graph, LCUI_Graph *image, int flag);
/* ���ܣ���ͼƬ��ָ�����뷽ʽճ��������ͼ�� */ 

int Fill_Background_Image(LCUI_Graph *graph, LCUI_Graph *bg, int flag, LCUI_RGB color);
/* ���ܣ�Ϊָ��ͼ����䱳��ͼ�� */ 

int Fill_Graph_Alpha(LCUI_Graph *src, unsigned char alpha);
/* ���ܣ����ͼ�ε�alphaͨ��������ֵ */ 

int Draw_Graph_Border(LCUI_Graph *src,LCUI_RGB color, LCUI_Border border);
/* ���ܣ�Ϊͼ�α�Ե���ƾ��α߿� */ 

void Get_Overlay_Widget(LCUI_Rect rect, LCUI_Widget *widget, LCUI_Queue *queue);
/* ���ܣ���ȡ��ָ�������ص��Ĳ��� */ 

LCUI_RGBA Get_Graph_Pixel(LCUI_Graph *graph, LCUI_Pos pos);
/* ���ܣ���ȡͼ����ָ����������ص����ɫ */ 

int Graph_Is_Opaque(LCUI_Graph *graph);
/* 
 * ���ܣ����ͼ���Ƿ�Ϊ��͸�� 
 * ˵������ȫ͸���򷵻�-1����͸���򷵻�1����͸��Ч���򷵻�0
 * */ 
int write_png_file(char *file_name , LCUI_Graph *graph);
/* ���ܣ���LCUI_Graph�ṹ�е�����д����png�ļ� */

int Widget_Layer_Is_Opaque(LCUI_Widget *widget);
/* ���ܣ��жϲ���ͼ���Ƿ�͸�� */ 

int Widget_Layer_Not_Visible(LCUI_Widget *widget);
/* ���ܣ���ⲿ��ͼ���Ƿ���ȫ͸�� */ 

int Get_Screen_Real_Graph (LCUI_Rect rect, LCUI_Graph * graph);
/* 
 * ���ܣ���ȡ��Ļ��ָ��������ʵ��Ҫ��ʾ��ͼ�� 
 * ˵����ָ��������������벿�����򲻲����ص���
 * */ 

LCUI_RGBA RGBA_Mix(LCUI_RGBA back, LCUI_RGBA fore);
/* ���ܣ�����������ص����ɫ */ 

int Rotate_Graph(LCUI_Graph *src, int rotate_angle, LCUI_Graph *des);
/* 
 * ���ܣ���תͼ��
 * ˵����ָ����ת���ĵ������Լ���ת�Ƕȣ����ɵõ���ת���ͼ��
 * ��Դ����ο��ԣ�http://read.pudn.com/downloads154/sourcecode/graph/684994/%E5%9B%BE%E5%83%8F%E6%97%8B%E8%BD%AC/%E5%9B%BE%E5%83%8F%E6%97%8B%E8%BD%AC/%E6%BA%90%E4%BB%A3%E7%A0%81/MyDIPView.cpp__.htm
 * �㷨�д��Ż����ơ�
 */ 
#endif /* __LCUI_GRAPHICS_H__ */

