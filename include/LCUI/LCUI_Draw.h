/* ****************************************************************************
 * LCUI_Draw.h -- The graphics draw module of LCUI.
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
 * LCUI_Draw.h -- LCUI��ͼ�λ���ģ��
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

#ifndef __LCUI_DRAW_H__
#define __LCUI_DRAW_H__

LCUI_BEGIN_HEADER

#include LC_DRAW_BORDER_H

int Graph_Rotate(LCUI_Graph *src, int rotate_angle, LCUI_Graph *des);
/* 
 * ���ܣ���תͼ��
 * ˵����ָ����ת���ĵ������Լ���ת�Ƕȣ����ɵõ���ת���ͼ��
 * ��Դ����ο��Ի�������ش���
 * �㷨�д��Ż����ơ�
 */

int load_bmp(const char *filepath, LCUI_Graph *out);
/* �򿪲�����BMPͼƬ�ļ��ڵ�ͼ������ */

int load_jpeg(const char *filepath, LCUI_Graph *out);
/* ���ܣ����벢����jpgͼƬ */

int load_png(const char *filepath, LCUI_Graph *out);
/* ����PNGͼƬ�е�ͼ������ */

int write_png(const char *file_name, LCUI_Graph *graph);
/* ��ͼ������д����png�ļ� */

int Load_Image(const char *filepath, LCUI_Graph *out);
/* 
 * ���ܣ�����ָ��ͼƬ�ļ���ͼ������
 * ˵������ͼƬ�ļ������������ڴ棬�򿪵�ͼƬ�ļ�Խ��ռ�õ��ڴ�Ҳ��Խ�� 
 * */

int GaussianSmooth( LCUI_Graph *src, LCUI_Graph *des, double sigma );
/* ��ͼ����и�˹ģ������ */ 

int Graph_Smooth( LCUI_Graph *src, LCUI_Graph *des, double sigma );
/* ��ͼ�����ģ������ */
int Draw_Empty_Slot(LCUI_Graph *graph, int width, int height);
/* ���ܣ����ƽ������Ŀղ� */ 

LCUI_END_HEADER

#endif
