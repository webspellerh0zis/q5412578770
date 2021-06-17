/* ***************************************************************************
 * LCUI_PictureBox.h -- LCUI's PictureBox widget
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
 * LCUI_PictureBox.h -- LCUI ��ͼ��򲿼�
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

#ifndef __LCUI_PICTURE_BOX_H_
#define __LCUI_PICTURE_BOX_H_

typedef struct _LCUI_PictureBox	LCUI_PictureBox;
/******************* ͼ��״̬ *******************/
#define IMAGE_STATUS_LOADING   1
#define IMAGE_STATUS_FAIL     -1
#define IMAGE_STATUS_SUCCESS   0
/**********************************************/


/**************************** ͼƬ���� *********************************/
struct _LCUI_PictureBox
{
	LCUI_Graph		*image;			/* ��PictureBox����ʾ��ͼ�� */
	LCUI_Rect		read_box;		/* ��ʾͼ�������Χ */ 
	LCUI_Graph		buff_graph;		/* ���ڴ������ź��ͼ�� */
	float			scale;			/* ���ű��� */
	int				size_mode;		/* ����PictureBox����δ���ͼ��λ�úͲ�����С */
	
	LCUI_Graph		error_image;	/* ������ͼ��ʧ��ʱ��ʾ��ͼ�� */
	LCUI_Graph		initial_image;	/* ���ڼ�����һͼ��ʱ��ʾ��ͼ�� */ 
	
	int		image_status;			/* ͼƬ״̬(LOADING / FAIL / SUCCESS) */
};
/****************************** END ***********************************/

LCUI_Rect Get_PictureBox_View_Area(LCUI_Widget *widget);
/* ���ܣ���ȡͼƬ���ӵ�ͼƬ��ʾ������ */ 

LCUI_Pos Get_PictureBox_View_Area_Pos(LCUI_Widget *widget);
/* ���ܣ���ȡ��ȡͼƬ���ӵ�ͼƬ��ʾ����������� */

float Get_PictureBox_Zoom_Scale(LCUI_Widget *widget);
/* ���ܣ���ȡͼƬ���ӵ����ű��� */ 

void Set_PictureBox_Image_From_Graph(LCUI_Widget *widget, LCUI_Graph *image);
/* ���ܣ����һ��ͼƬ������ͼƬ���� */ 

int Set_PictureBox_Image_From_File(LCUI_Widget *widget, char *image_file);
/* ���ܣ����һ��ͼƬ�ļ�����������ͼƬ���� */ 

int Set_PictureBox_ErrorImage(LCUI_Widget *widget, LCUI_Graph *pic);
/* ���ܣ��趨������ͼ��ʧ��ʱ��ʾ��ͼ�� */ 

int Set_PictureBox_InitImage(LCUI_Widget *widget, LCUI_Graph *pic);
/* ���ܣ��趨���ڼ�����һͼ��ʱ��ʾ��ͼ�� */ 

void Set_PictureBox_Size_Mode(LCUI_Widget *widget, int mode);
/* ���ܣ��趨ͼƬ���ӵ�ͼ����ʾģʽ */ 

void Resize_PictureBox_View_Area(LCUI_Widget *widget, int width, int height);
/* ���ܣ��趨PictureBox������ͼƬ��ʾ����Ĵ�С */ 

LCUI_Graph *Get_PictureBox_Graph(LCUI_Widget *widget);
/* ���ܣ���ȡPictureBox�����ڵ�ͼ��ָ�� */ 

int Move_PictureBox_View_Area(LCUI_Widget *widget, LCUI_Pos des_pos);
/* ���ܣ��ƶ�ͼƬ�����ڵ�ͼƬ����ʾ�����λ�� */ 

int Zoom_PictureBox_View_Area(LCUI_Widget *widget, float scale);
/* ���ܣ�����PictureBox������ͼƬ������� */ 
#endif

