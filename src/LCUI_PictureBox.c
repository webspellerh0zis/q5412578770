/* ***************************************************************************
 * LCUI_PictureBox.c -- LCUI's PictureBox widget
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
 * LCUI_PictureBox.c -- LCUI ��ͼ��򲿼�
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
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_WIDGET_H
#include LC_PICBOX_H
#include LC_MEM_H
static void Destroy_PictureBox(LCUI_Widget* widget)
/* ���ܣ��ͷ�ͼƬ����ռ�õ��ڴ���Դ */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	Free_Graph(&pic_box->buff_graph); 
	Free_Graph(&pic_box->error_image);
	Free_Graph(&pic_box->initial_image); 
}

static void PictureBox_Init(LCUI_Widget *widget)
/* ���ܣ���ʼ��ͼƬ���� */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
			Malloc_Widget_Private(widget, sizeof(LCUI_PictureBox));;
	
	Rect_Init(&pic_box->read_box);
	pic_box->image		= NULL; 
	pic_box->scale		= 1.0;
	pic_box->size_mode	= SIZE_MODE_CENTER; 
	Graph_Init(&pic_box->buff_graph); 
	Graph_Init(&pic_box->error_image);
	Graph_Init(&pic_box->initial_image); 
}

LCUI_Rect Get_PictureBox_View_Area(LCUI_Widget *widget)
/* ���ܣ���ȡͼƬ���ӵ�ͼƬ��ʾ������ */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	return pic_box->read_box;
}

LCUI_Pos Get_PictureBox_View_Area_Pos(LCUI_Widget *widget)
/* ���ܣ���ȡ��ȡͼƬ���ӵ�ͼƬ��ʾ����������� */
{
	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(widget);
	return Pos(rect.x, rect.y);
}

static void Exec_Update_PictureBox(LCUI_Widget *widget)
/* ���ܣ�����PictureBox���� */
{
	LCUI_PictureBox *pic_box;
	LCUI_Graph graph, *p;
	int width, height; 
	LCUI_Pos pos;
	pos.x = 0;
	pos.y = 0; 
	Graph_Init(&graph); 
	pic_box  = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	/************************
	printf("Exec_Update_PictureBox start\n");
	Print_Graph_Info(pic_box->image);
	printf("pic_box:%p, parent:%s, visible:%d, pos:%d,%d, size:%d,%d\n", 
		widget,widget->parent->type.string, widget->parent->visible,widget->pos.x,
		widget->pos.y, widget->size.w, widget->size.h);
	* ***********************/
	if(Valid_Graph(pic_box->image))
	{/* �����ͼƬ. ����ͼƬ��Ч */ 
		switch(pic_box->size_mode)
		{
		case SIZE_MODE_ZOOM:
		/* �ü�ͼ�� */ 
			if(pic_box->scale == 1.00) 
				p = pic_box->image; 
			else
				p = &pic_box->buff_graph;
				
			width = pic_box->read_box.width;
			height = pic_box->read_box.height;
			pos.x = (widget->size.w - width)/2.0;
			pos.y = (widget->size.h - height)/2.0;
			if(!Valid_Graph(&widget->background_image)) 
			/* ���û�б���ͼ�� */ 
				/* ��ȡ��ֱ�Ӹ���ͼ�� */
				Cut_And_Replace_Graph(p, 
					Rect(pic_box->read_box.x, pic_box->read_box.y, width, height), 
					pos, &widget->graph); 
			else  
				/* ��ȡ����alphaͨ������ͼ�� */
				Cut_And_Overlay_Graph(p, 
					Rect(pic_box->read_box.x, pic_box->read_box.y, width, height), 
					pos, &widget->graph);  
			break;
			 
		case SIZE_MODE_NORMAL:/* ����ģʽ */
			width = pic_box->read_box.width; 
			height = pic_box->read_box.height;  
			if(!Valid_Graph(&widget->background_image)) 
			/* ���û�б���ͼ�� */ 
				/* ��ȡ��ֱ�Ӹ���ͼ�� */
				Cut_And_Replace_Graph(pic_box->image, 
					Rect(pic_box->read_box.x, pic_box->read_box.y, width, height), 
					pos, &widget->graph); 
			else 
				/* ��ȡ����alphaͨ������ͼ�� */
				Cut_And_Overlay_Graph(pic_box->image, 
					Rect(pic_box->read_box.x, pic_box->read_box.y, width, height), 
					pos, &widget->graph);
			break;
			
		case SIZE_MODE_STRETCH:/* ����ģʽ */ 
			/* ��ʼ����ͼƬ */
			Zoom_Graph( pic_box->image, &graph, CUSTOM, widget->size ); 
			if(!Valid_Graph(&widget->background_image)) 
			/* ���û�б���ͼ�� */ 
				Replace_Graph(&widget->graph, &graph, pos);
			else
				Mix_Graph(&widget->graph, &graph, pos);
			Free_Graph(&graph);
			break;
			
		case SIZE_MODE_TILE:/* ƽ��ģʽ */ 
			Tile_Graph( pic_box->image, &graph, widget->size.w, widget->size.h); 
			if(!Valid_Graph(&widget->background_image)) 
			/* ���û�б���ͼ�� */ 
				Replace_Graph(&widget->graph, &graph, pos); 
			else  
				Mix_Graph(&widget->graph, &graph, pos); 
			Free_Graph(&graph);
			break;
			
		case SIZE_MODE_CENTER:
			/* �ж�ͼ��ĳߴ��Ƿ�С��ͼƬ���ӵĳߴ磬����������λ�� */
			if(pic_box->image->width < widget->size.w)
			{
				pic_box->read_box.x = 0;
				pic_box->read_box.width = pic_box->image->width;
				pos.x = (widget->size.w - pic_box->image->width)/2 + 0.5;
			}
			if(pic_box->image->height < widget->size.h)
			{
				pos.y = (widget->size.h - pic_box->image->height)/2 + 0.5;
				pic_box->read_box.y = 0;
				pic_box->read_box.height = pic_box->image->height;
			}
			if(pic_box->read_box.y + pic_box->read_box.height >= pic_box->image->height) 
			/* �����ȡ����ĳߴ����ͼƬ�ߴ� */
				pic_box->read_box.y = pic_box->image->height - pic_box->read_box.height;
			if(pic_box->read_box.x + pic_box->read_box.width >= pic_box->image->width) 
				pic_box->read_box.x = pic_box->image->width - pic_box->read_box.width;
				
			if(!Valid_Graph(&widget->background_image)) 
			/* ���û�б���ͼ�� */ 
				/* ��ȡ��ֱ�Ӹ���ͼ�� */
				Cut_And_Replace_Graph(pic_box->image, 
					Rect(pic_box->read_box.x, pic_box->read_box.y, 
					pic_box->read_box.width, pic_box->read_box.height),
					pos, &widget->graph);
			else 
				/* ��ȡ����alphaͨ������ͼ�� */
				Cut_And_Overlay_Graph(pic_box->image, 
					Rect(pic_box->read_box.x, pic_box->read_box.y, 
					pic_box->read_box.width, pic_box->read_box.height),
					pos, &widget->graph); 
			break;
			default : break;
		} 
	}
	//printf("Exec_Update_PictureBox end\n");
	Refresh_Widget(widget); 
}

float Get_PictureBox_Zoom_Scale(LCUI_Widget *widget)
/* ���ܣ���ȡͼƬ���ӵ����ű��� */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	return pic_box->scale;
}



void Set_PictureBox_Image_From_Graph(LCUI_Widget *widget, LCUI_Graph *image)
/* ���ܣ����һ��ͼƬ������ͼƬ���� */
{ 
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	
	float scale_x,scale_y;
	int i;
	LCUI_Graph *graph = image; 
	/* ͼƬ�����ˣ����ͷŻ���ͼ�� */
	Free_Graph(&pic_box->buff_graph);
	
	for(i = 0;i < 2; ++i)
	{
		if(Valid_Graph(graph))
		{/* ���image��Ч */
			/* ����ͼ������ָ�� */ 
			pic_box->image = graph;
			/* ��ȡ�ķ�����Ϊ����ͼƬ���� */
			pic_box->read_box.x = 0;
			pic_box->read_box.y = 0;
			pic_box->read_box.width = graph->width;
			pic_box->read_box.height = graph->height;
			pic_box->read_box.center_x = 0.5;
			pic_box->read_box.center_y = 0.5;
			pic_box->scale = 1; /* ���ű���Ϊ100% */
			switch(pic_box->size_mode)
			{
			case SIZE_MODE_ZOOM:
				scale_x = (float)widget->size.w / pic_box->read_box.width;
				scale_y = (float)widget->size.h / pic_box->read_box.height;
				if(scale_x < scale_y) pic_box->scale = scale_x;
				else pic_box->scale = scale_y;
				break;
			case SIZE_MODE_NORMAL:/* ����ģʽ */
				break;
			case SIZE_MODE_STRETCH:/* ����ģʽ */
				break;
			case SIZE_MODE_TILE:/* ƽ��ģʽ */
				break;
			case SIZE_MODE_CENTER:
				/* �ж�ͼ��ĳߴ��Ƿ񳬳�ͼƬ���ӵĳߴ� */
				if(pic_box->image->width >= widget->size.w)
				{
					pic_box->read_box.x = (pic_box->image->width - widget->size.w)/2;
					pic_box->read_box.width = widget->size.w;
				}
				if(pic_box->image->height >= widget->size.h)
				{
					pic_box->read_box.y = (pic_box->image->height - widget->size.h)/2;
					pic_box->read_box.height = widget->size.h;
				}
				break;
				default : break;
			}
			break;
		}
		else if(pic_box->image_status == IMAGE_STATUS_LOADING) 
			/* ʹ�ö�Ӧ��ͼƬ */
			graph = &pic_box->initial_image; 
		else
		{
			/* ʹ�ö�Ӧ��ͼƬ */
			graph = &pic_box->error_image;
			pic_box->image_status = IMAGE_STATUS_FAIL;
		}
	}
	Draw_Widget(widget);
	Refresh_Widget(widget); 
}

int Set_PictureBox_Image_From_File(LCUI_Widget *widget, char *image_file)
/* ���ܣ����һ��ͼƬ�ļ�����������ͼƬ���� */
{
	int value;
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	LCUI_Graph *graph = (LCUI_Graph*)calloc(1, sizeof(LCUI_Graph));
	pic_box->image_status = IMAGE_STATUS_LOADING; /* ͼƬ״̬Ϊ�������� */
	Set_PictureBox_Image_From_Graph(widget, NULL);
	value = Load_Image(image_file, graph);/* ����ͼƬ�ļ� */
	if(value != 0)
	{
		/* ����ʧ�� */
		pic_box->image_status = IMAGE_STATUS_FAIL;
		Set_PictureBox_Image_From_Graph(widget, NULL);
	}
	else 
	{
		/* ����ɹ� */
		pic_box->image_status = IMAGE_STATUS_SUCCESS;
		Set_PictureBox_Image_From_Graph(widget, graph);
	}
	return value;
}



int Set_PictureBox_ErrorImage(LCUI_Widget *widget, LCUI_Graph *pic)
/* ���ܣ��趨������ͼ��ʧ��ʱ��ʾ��ͼ�� */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
								Get_Widget_Private_Data(widget);
	
	if(Valid_Graph(pic))
	{
		Copy_Graph(&pic_box->error_image, pic);
		return 0;
	}
	return -1;
}

int Set_PictureBox_InitImage(LCUI_Widget *widget, LCUI_Graph *pic)
/* ���ܣ��趨���ڼ�����һͼ��ʱ��ʾ��ͼ�� */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	
	if(Valid_Graph(pic))
	{
		Copy_Graph(&pic_box->initial_image, pic);
		return 0;
	}
	return -1;
}

void Set_PictureBox_Size_Mode(LCUI_Widget *widget, int mode)
/* ���ܣ��趨ͼƬ���ӵ�ͼ����ʾģʽ */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	
	if(pic_box->size_mode != mode)
	{
		float scale_x,scale_y;
		pic_box->size_mode = mode;
		switch(mode)
		{
		case SIZE_MODE_ZOOM:
			scale_x = (float)widget->size.w / pic_box->image->width;
			scale_y = (float)widget->size.h / pic_box->image->height;
			if(scale_x < scale_y) pic_box->scale = scale_x;
			else pic_box->scale = scale_y;
			/*
			 * pic_box->read_box.x = 0;
			 * pic_box->read_box.y = 0;
			 * pic_box->read_box.width = pic_box->image->width;
			 * pic_box->read_box.height = pic_box->image->height;
			 * */
			pic_box->read_box.center_x = 0.5;
			pic_box->read_box.center_y = 0.5;
			Zoom_PictureBox_View_Area(widget, pic_box->scale); 
			break;
		default: break;
		}
		Draw_Widget(widget);
		Refresh_Widget(widget); 
	} 
}


void Resize_PictureBox_View_Area(LCUI_Widget *widget, int width, int height)
/* ���ܣ��趨PictureBox������ͼƬ��ʾ����Ĵ�С */
{
	LCUI_Pos start, center_pos;
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
								Get_Widget_Private_Data(widget);
	
	if(width <= 0 || height <= 0)
		return;
	
	if(!Valid_Graph(pic_box->image))
		return;
	/* �����ĵ�λ��ת�������� */
	center_pos.x = pic_box->read_box.center_x * pic_box->scale * pic_box->image->width;
	center_pos.x = pic_box->read_box.center_y * pic_box->scale * pic_box->image->height;
	/* �����������ݣ�ʹ֮Ϊ��Ч���� */
	start.x = center_pos.x - width/2.0;
	start.y = center_pos.y - height/2.0;
	if(start.x < 0) start.x = 0;
	if(start.y < 0) start.y = 0;
	if(start.x + width > pic_box->image->width)
	start.x = pic_box->image->width - width;
	if(start.y + height > pic_box->image->height)
	start.y = pic_box->image->height - height;
	if(start.x < 0) 
	{
		start.x = 0;
		width = pic_box->image->width;
	}
	if(start.y < 0) 
	{
		start.y = 0;
		height = pic_box->image->height;
	}
	pic_box->read_box.x = start.x;
	pic_box->read_box.y = start.y;
	pic_box->read_box.width = width;
	pic_box->read_box.height = height;
	/* ����ͼƬ�����ڵ�ͼ�� */
	Draw_Widget(widget);
	Refresh_Widget(widget); 
}

LCUI_Graph *Get_PictureBox_Graph(LCUI_Widget *widget)
/* ���ܣ���ȡPictureBox�����ڵ�ͼ��ָ�� */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	return pic_box->image;
}

int Move_PictureBox_View_Area(LCUI_Widget *widget, LCUI_Pos des_pos)
/* ���ܣ��ƶ�ͼƬ�����ڵ�ͼƬ����ʾ�����λ�� */
{
	LCUI_Graph *p;
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
								Get_Widget_Private_Data(widget);
	
	if(!Valid_Graph(pic_box->image)) 
		return -1;
	
	LCUI_Size size;
		
	if(pic_box->scale == 1.00 || !Valid_Graph(&pic_box->buff_graph))
		p = pic_box->image;
	else
		p = &pic_box->buff_graph;
		
	size.w = pic_box->read_box.width;
	size.h = pic_box->read_box.height;
	/* �����������ݣ�ʹ֮Ϊ��Ч���� */
	if(des_pos.x < 0) des_pos.x = 0;
	if(des_pos.y < 0) des_pos.y = 0;
	if(des_pos.x + size.w > p->width)
	des_pos.x = p->width - size.w;
	if(des_pos.y + size.h > p->height)
	des_pos.y = p->height - size.h;
	
	if(des_pos.x == pic_box->read_box.x 
	&& des_pos.y == pic_box->read_box.y)
		return 0; 
		
	/* ����ͼƬ�����ڵ�ͼ�� */
	pic_box->read_box.x = des_pos.x;
	pic_box->read_box.y = des_pos.y;
	/* ���¼������ĵ��λ�� */ 
	pic_box->read_box.center_x = (des_pos.x + size.w/2.0)/p->width;
	pic_box->read_box.center_y = (des_pos.y + size.h/2.0)/p->height;
	
	Draw_Widget(widget);
	//
	//printf("read box: %d,%d,%d,%d\n", 
	//pic_box->read_box.x, pic_box->read_box.y, 
	//pic_box->read_box.width, pic_box->read_box.height);
	return 0;
}


int Zoom_PictureBox_View_Area(LCUI_Widget *widget, float scale)
/* ���ܣ�����PictureBox������ͼƬ������� */
{
	LCUI_Graph *p;
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
						Get_Widget_Private_Data(widget);
	LCUI_Pos center_pos;
	if(!Valid_Graph(pic_box->image)) return -1;
	
	float width = 0,height = 0;
	/* ��Ч��ΧΪ2%~2000% */
	if(scale < 0.02) scale = 0.02;
	if(scale > 20) scale = 20;
	pic_box->size_mode = SIZE_MODE_ZOOM; /* ��Ϊ����ģʽ */
	pic_box->scale = scale;
	
	/* ���ź��ͼ��ߴ� */
	width = scale * pic_box->image->width;
	height = scale * pic_box->image->height;
	
	if(scale == 1.00) /* ԭ100%�����Ͳ���Ҫ�����ˣ�ֱ�����ֳɵ� */
	{
		Free_Graph(&pic_box->buff_graph);
		p = pic_box->image;
	}
	else /* ����ͼ�� */
	{
		p = &pic_box->buff_graph;
		Zoom_Graph(pic_box->image, &pic_box->buff_graph, DEFAULT, Size(width, height));
	}
		
	if(width <= widget->size.w)
	{/* ������ź��ͼ��Ŀ�Ȼ��ǲ����ڲ����Ŀ�� */
		pic_box->read_box.width = width;
		pic_box->read_box.x = 0;
		pic_box->read_box.center_x = 0.5; 
	}
	else
	{/* ���򣬵�����ȡ���� */
		pic_box->read_box.width = widget->size.w;
		center_pos.x = pic_box->read_box.center_x * width;
		pic_box->read_box.x = center_pos.x - pic_box->read_box.width/2.0;
		if(pic_box->read_box.x < 0)
		{/* ���С��0������Ҫ�������ĵ�λ�� */
			center_pos.x = pic_box->read_box.width/2.0 + pic_box->read_box.x;
			pic_box->read_box.center_x = center_pos.x / width;
			pic_box->read_box.x = 0;
		}
		if(pic_box->read_box.x + pic_box->read_box.width
		 > p->width)
		{
			pic_box->read_box.x = pic_box->buff_graph.width - pic_box->read_box.width; 
			center_pos.x = p->width/2.0 + pic_box->read_box.x;
			pic_box->read_box.center_x = center_pos.x / width; 
		}
	} 
	if(height <= widget->size.h)
	{/* ������ź��ͼ��Ŀ�Ȼ��ǲ����ڲ����Ŀ�� */
		pic_box->read_box.height = height;
		pic_box->read_box.y = 0;
		pic_box->read_box.center_y = 0.5; 
	}
	else
	{/* ���򣬵�����ȡ���� */
		pic_box->read_box.height = widget->size.h;
		center_pos.y = pic_box->read_box.center_y * height;
		pic_box->read_box.y = center_pos.y - pic_box->read_box.height/2.0;
		if(pic_box->read_box.y < 0)
		{/* ���С��0������Ҫ�������ĵ�λ�� */
			center_pos.y = p->height/2.0 + pic_box->read_box.y;
			pic_box->read_box.center_y = center_pos.y / height;
			pic_box->read_box.y = 0;
		}
		if(pic_box->read_box.y + pic_box->read_box.height
		 > p->height)
		{
			pic_box->read_box.y = p->height - pic_box->read_box.height; 
			center_pos.y = pic_box->read_box.height/2.0 + pic_box->read_box.y;
			pic_box->read_box.center_y = center_pos.y / height; 
		}
	}
	
	Draw_Widget(widget);
	Refresh_Widget(widget);
	return 0;
}

static void Exec_Resize_PictureBox(LCUI_Widget *widget)
/* ���ܣ��ı�PictureBox�����ĳߴ� */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	if(widget->size.w * widget->size.h > 0)
	{
		switch(pic_box->size_mode)
		{
		case SIZE_MODE_ZOOM: 
			/* ���¸ı�ͼ���������ĳߴ磬����ߴ�ָ���Ǵ�Դͼ���н�ȡ����ͼ��ĳߴ� */
			Resize_PictureBox_View_Area(
				widget, 
				widget->size.w / pic_box->scale, 
				widget->size.h / pic_box->scale );
			/* �����ڵ����ű��������������ź��ͼ�� */
			Zoom_PictureBox_View_Area(widget, pic_box->scale);
			break;
		case SIZE_MODE_NORMAL:/* ����ģʽ */
			break;
		case SIZE_MODE_STRETCH:/* ����ģʽ */
			break;
		case SIZE_MODE_TILE:/* ƽ��ģʽ */
			break;
		case SIZE_MODE_CENTER: /* ����ģʽ */ 
			Resize_PictureBox_View_Area(widget, widget->size.w, widget->size.h);
			break;
			default : break;
		}
	}
	Refresh_Widget(widget); 
}

void Register_PictureBox()
/* ���ܣ�ע�Ჿ������-��ť�������� */
{
	/* ��Ӳ������� */
	WidgetType_Add("picture_box");
	
	/* Ϊ�������͹�����غ��� */
	WidgetFunc_Add("picture_box",	PictureBox_Init,		FUNC_TYPE_INIT);
	WidgetFunc_Add("picture_box",	Exec_Update_PictureBox,	FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("picture_box",	Exec_Resize_PictureBox,	FUNC_TYPE_RESIZE); 
	WidgetFunc_Add("picture_box",	Destroy_PictureBox,	FUNC_TYPE_DESTROY); 
}

