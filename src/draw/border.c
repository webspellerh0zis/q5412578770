#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

#include <math.h>

void Border_Init( LCUI_Border *border )
/* ��ʼ���߿����� */
{
	border->top_width = 0;
	border->bottom_width = 0;
	border->left_width = 0;
	border->right_width = 0;
	border->top_style = BORDER_STYLE_NONE;
	border->bottom_style = BORDER_STYLE_NONE;
	border->left_style = BORDER_STYLE_NONE;
	border->right_style = BORDER_STYLE_NONE;
	border->top_color = RGB(0,0,0);
	border->bottom_color = RGB(0,0,0);
	border->left_color = RGB(0,0,0);
	border->right_color = RGB(0,0,0);
	border->top_left_radius = 0;
	border->top_right_radius = 0;
	border->bottom_left_radius = 0;
	border->bottom_right_radius = 0;
}

LCUI_Border Border( int width_px, BORDER_STYLE style, LCUI_RGB color )
/* �򵥵����ñ߿���ʽ������ȡ����ʽ���� */
{
	LCUI_Border border;
	border.top_width = width_px;
	border.bottom_width = width_px;
	border.left_width = width_px;
	border.right_width = width_px;
	border.top_style = style;
	border.bottom_style = style;
	border.left_style = style;
	border.right_style = style;
	border.top_color = color;
	border.bottom_color = color;
	border.left_color = color;
	border.right_color = color;
	border.top_left_radius = 0;
	border.top_right_radius = 0;
	border.bottom_left_radius = 0;
	border.bottom_right_radius = 0;
	return border; 
}

void Border_Radius( LCUI_Border *border, int radius )
/* ���ñ߿��Բ�ǰ뾶 */
{
	border->top_left_radius = radius;
	border->top_right_radius = radius;
	border->bottom_left_radius = radius;
	border->bottom_right_radius = radius;
}

int Graph_Draw_Border( LCUI_Graph *des, LCUI_Border border )
/* �򵥵�Ϊͼ�α�Ե���Ʊ߿� */
{
	if( !Graph_Valid(des) ) {
		return -1;
	}
	
	int x,y,count, k, w[2], h[2], start_x,start_y;
	
	w[0] = des->width - border.top_right_radius;
	h[0] = des->height - border.bottom_left_radius;
	w[1] = des->width - border.bottom_right_radius;
	h[1] = des->height - border.bottom_right_radius;
	
	Graph_Lock(des, 1);
	/* �������Ͻǵ�Բ�� */
	for(y=0; y<border.top_left_radius; ++y) {
		k = y * des->width;
		for(x=0; x<border.top_left_radius; ++x) {
			if( fabs( pow(x-border.top_left_radius, 2)
				+pow(y-border.top_left_radius, 2) )
			    -pow(border.top_left_radius, 2) > 1 ) {
				printf("(%d,%d), skip\n", y, x);
				continue;
			}
			printf("(%d,%d)\n", y, x);
			count = k + x;
			des->rgba[0][count] = border.top_color.red;
			des->rgba[1][count] = border.top_color.green;
			des->rgba[2][count] = border.top_color.blue;
			if(Graph_Have_Alpha(des)) {
				des->rgba[3][count] = 255;
			}
			break;
		}
	}
	
	/* �����ϱ߿� */
	for(y=0;y<border.top_width;++y) {
		k = y * des->width;
		for(x = border.top_left_radius; x < w[0]; ++x) {
			count = k + x;
			des->rgba[0][count] = border.top_color.red;
			des->rgba[1][count] = border.top_color.green;
			des->rgba[2][count] = border.top_color.blue;
			if(Graph_Have_Alpha(des)) {
				des->rgba[3][count] = 255;
			}
		}
	}
	start_y = des->height - border.bottom_width;
	/* �����±ߵ��� */
	for(y=0;y<border.bottom_width;++y) {
		k = (start_y+y) * des->width;
		for(x = border.bottom_left_radius; x < w[1]; ++x) {
			count = k + x;
			des->rgba[0][count] = border.bottom_color.red;
			des->rgba[1][count] = border.bottom_color.green;
			des->rgba[2][count] = border.bottom_color.blue;
			if(Graph_Have_Alpha(des)) {
				des->rgba[3][count] = 255;
			}
		}
	}
	/* ������ߵ��� */
	for(y=border.top_left_radius; y<h[0]; ++y) {
		k = y * des->width;
		for(x=0;x<border.left_width;++x) {
			count = k + x;
			des->rgba[0][count] = border.left_color.red;
			des->rgba[1][count] = border.left_color.green;
			des->rgba[2][count] = border.left_color.blue;
			if(Graph_Have_Alpha(des)) 
				des->rgba[3][count] = 255;
		}
	}
	/* �����ұߵ��� */
	start_x = des->width - border.right_width;
	for(y=border.top_right_radius; y<h[1]; ++y) {
		k = y * des->width + start_x;
		for(x=0;x<border.right_width;++x) {
			count = k + x;
			des->rgba[0][count] = border.right_color.red;
			des->rgba[1][count] = border.right_color.green;
			des->rgba[2][count] = border.right_color.blue;
			if(Graph_Have_Alpha(des)) {
				des->rgba[3][count] = 255;
			}
		}
	}
	/* �߿��߻������ */
	Graph_Unlock(des);
	return 0;
}
