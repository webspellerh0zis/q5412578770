#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DISPLAY_H 

#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h> 
#include <sys/ioctl.h> 

int Graph_Display (LCUI_Graph * src, LCUI_Pos pos)
/* 
 * ���ܣ���ʾͼ�� 
 * ˵�����˺���ʹ��֡���壨FrameBuffer������ͼ�����
 * *ע����Ҫ����ο���mgaveiw��mga_vfb.c�ļ��е�write_to_fb����.
 * */
{
	int bits;
	unsigned char *dest;
	struct fb_cmap kolor; 
	unsigned int x, y, n, k, count;
	unsigned int temp1, temp2, temp3, i; 
	LCUI_Rect cut_rect;
	LCUI_Graph temp, *pic;

	if (!Graph_Valid (src)) {
		return -1;
	}
	/* ָ��֡���� */
	dest = LCUI_Sys.screen.fb_mem;		
	pic = src; 
	Graph_Init (&temp);
	
	if ( Get_Cut_Area ( Get_Screen_Size(), 
			Rect ( pos.x, pos.y, src->width, src->height ), 
			&cut_rect
		) ) {/* �����Ҫ�ü�ͼ�� */
		if(!Rect_Valid(cut_rect)) {
			return -2;
		}
		pos.x += cut_rect.x;
		pos.y += cut_rect.y;
		Graph_Cut (pic, cut_rect, &temp);
		pic = &temp;
	}
	
	Graph_Lock (pic, 0);
	/* ��ȡ��ʾ����λ�� */
	bits = Get_Screen_Bits(); 
	switch(bits) {
	    case 32:/* 32λ������RGB��ռ8λ��ʣ�µ�8λ����alpha����4���ֽ� */ 
		for (n=0,y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;//count = 4 * (k + x);/* �������������ص������ */
				count = count << 2; 
				/* ����֡����(FrameBuffer)����ɫ������BGR��ͼƬ������RGB����Ҫ�ı�һ��д��˳�� */
				dest[count] = pic->rgba[2][n];
				dest[count + 1] = pic->rgba[1][n];
				dest[count + 2] = pic->rgba[0][n]; 
			}
		}
		break;
	    case 24:/* 24λ��RGB��ռ8λ��Ҳ���ǹ�3���ֽ� */ 
		for (n=0, y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;//count = 3 * (k + x); 
				count = (count << 1) + count;
				dest[count] = pic->rgba[2][n];
				dest[count + 1] = pic->rgba[1][n];
				dest[count + 2] = pic->rgba[0][n];
			}
		}
		break;
	    case 16:/* 16λ��rgb�ֱ�ռ5λ��6λ��5λ��Ҳ����RGB565 */
		/*
		 * GB565��ɫģʽ, һ������ռ�����ֽ�, ����:
		 * ���ֽڵ�ǰ5λ������ʾB(BLUE)
		 * ���ֽڵĺ���λ+���ֽڵ�ǰ��λ������ʾG(Green)
		 * ���ֽڵĺ�5λ������ʾR(RED)
		 * */  
		for (n=0, y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = (k + x) << 1;//count = 2 * (k + x);
				temp1 = pic->rgba[0][n];
				temp2 = pic->rgba[2][n];
				temp3 = pic->rgba[1][n];
				dest[count] = ((temp3 & 0x1c)<<3)+((temp2 & 0xf8)>>3);
				dest[count+1] = ((temp1 & 0xf8))+((temp3 & 0xe0)>>5);
			}
		}
		break;
	    case 8: /* 8λ��ռ1���ֽ� */
		kolor.start = 0;
		kolor.len = 255; 
		kolor.red = calloc(256, sizeof(__u16));
		kolor.green = calloc(256, sizeof(__u16));
		kolor.blue = calloc(256, sizeof(__u16));
		kolor.transp = 0; 
		
		for (i=0;i<256;i++) {
			kolor.red[i]=0;
			kolor.green[i]=0;
			kolor.blue[i]=0;
		}
		
		for (n=0, y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;
				
				temp1 = pic->rgba[0][n]*0.92;
				temp2 = pic->rgba[1][n]*0.92;
				temp3 = pic->rgba[2][n]*0.92; 
				
				i = ((temp1 & 0xc0))
					+((temp2 & 0xf0)>>2)
					+((temp3 & 0xc0)>>6);
						
				kolor.red[i] = temp1*256;
				kolor.green[i] = temp2*256;
				kolor.blue[i] = temp3*256;
				dest[count] = (((temp1 & 0xc0))
						+((temp2 & 0xf0)>>2)
						+((temp3 & 0xc0)>>6)); 
			}
		}
		
		ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOPUTCMAP, &kolor); 
		free(kolor.red);
		free(kolor.green);
		free(kolor.blue);
		break;
	    default: break;
	}
	Graph_Unlock (pic);
	Graph_Free (&temp);
	return -1;
}
