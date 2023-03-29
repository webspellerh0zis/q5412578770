#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

int Graph_Draw_HLBorder( LCUI_Graph *des, LCUI_BorderData *border )
/* Ϊͼ�λ���Բ�Ǳ߿����е�HL�Ǹ߼���Hight Level����Ӣ����д */
{
	if( !Graph_Valid(des) ) {
		return -1;
	}
	
	// code...
	
	return 0;
}

int Graph_Draw_Border( LCUI_Graph *des, LCUI_RGB color, LCUI_Border border )
/* �򵥵�Ϊͼ�α�Ե���ƾ��α߿� */
{
	if( !Graph_Valid(des) ) {
		return -1;
	}
	
	int x,y,count, k, start_x,start_y;
	
	Graph_Lock(des, 1);
	/* �����ϱߵ��� */
	for(y=0;y<border.top;++y) {
		k = y * des->width;
		for(x = 0; x < des->width; ++x) {
			count = k + x;
			des->rgba[0][count] = color.red;
			des->rgba[1][count] = color.green;
			des->rgba[2][count] = color.blue;
			if(Graph_Have_Alpha(des)) 
				des->rgba[3][count] = 255;
		}
	}
	start_y = des->height - border.bottom;
	/* �����±ߵ��� */
	for(y=0;y<border.bottom;++y) {
		k = (start_y+y) * des->width;
		for(x=0;x<des->width;++x) {
			count = k + x;
			des->rgba[0][count] = color.red;
			des->rgba[1][count] = color.green;
			des->rgba[2][count] = color.blue;
			if(Graph_Have_Alpha(des)) 
				des->rgba[3][count] = 255;
		}
	}
	/* ������ߵ��� */
	for(y=0;y<des->height;++y) {
		k = y * des->width;
		for(x=0;x<border.left;++x) {
			count = k + x;
			des->rgba[0][count] = color.red;
			des->rgba[1][count] = color.green;
			des->rgba[2][count] = color.blue;
			if(Graph_Have_Alpha(des)) 
				des->rgba[3][count] = 255;
		}
	}
	/* �����ұߵ��� */
	start_x = des->width - border.right;
	for(y=0;y<des->height;++y) {
		k = y * des->width + start_x;
		for(x=0;x<border.right;++x) {
			count = k + x;
			des->rgba[0][count] = color.red;
			des->rgba[1][count] = color.green;
			des->rgba[2][count] = color.blue;
			if(Graph_Have_Alpha(des)) 
				des->rgba[3][count] = 255;
		}
	}
	/* �߿��߻������ */
	Graph_Unlock(des);
	return 0;
}
