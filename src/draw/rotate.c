#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

#include <math.h>

static long max(long a, long b)
{
	return a > b ? a:b;
}

static double radian(int angle) 
{
	return angle*3.1415926/180;
}

int Graph_Rotate(LCUI_Graph *src, int rotate_angle, LCUI_Graph *des)
/* 
 * ���ܣ���תͼ��
 * ˵����ָ����ת���ĵ������Լ���ת�Ƕȣ����ɵõ���ת���ͼ��
 * ��Դ����ο��Ի�������ش���
 * �㷨�д��Ż����ơ�
 */
{
	if(!Graph_Valid(src)) {
		return -1;
	}
	// Դͼ��Ŀ�Ⱥ͸߶�
	int	width, height;
	// ��ת��ͼ��Ŀ�Ⱥ͸߶�   
	int	new_width,new_height; 
	// ��ת�Ƕȣ����ȣ�   
	float   fRotateAngle; 
	// ��ת�Ƕȵ����Һ�����   
	float   fSina, fCosa; 
	// Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩   
	float   fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩   
	float   fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// �����м䳣��   
	float   f1,f2; 
	// ��ȡͼ���"���"��4�ı�����   
	width = src->width; 
	// ��ȡͼ��ĸ߶�   
	height = src->height;   
	   
	// ����ת�ǶȴӶ�ת��������   
	fRotateAngle = (float) radian(rotate_angle); 
	// ������ת�Ƕȵ�����   
	fSina = (float) sin((double)fRotateAngle); 
	// ������ת�Ƕȵ�����   
	fCosa = (float) cos((double)fRotateAngle);   
	
	// ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩   
	fSrcX1 = (float) (- (width  - 1) / 2);   
	fSrcY1 = (float) (  (height - 1) / 2);   
	fSrcX2 = (float) (  (width  - 1) / 2);   
	fSrcY2 = (float) (  (height - 1) / 2);   
	fSrcX3 = (float) (- (width  - 1) / 2);   
	fSrcY3 = (float) (- (height - 1) / 2);   
	fSrcX4 = (float) (  (width  - 1) / 2);   
	fSrcY4 = (float) (- (height - 1) / 2);   
	   
	// ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩   
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;   
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;   
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;   
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;   
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;   
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;   
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;   
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;   
	   
	// ������ת���ͼ��ʵ�ʿ��   
	new_width  = (long) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);   
	// ������ת���ͼ��߶�   
	new_height = (long) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);   
	   
	// �������������������Ժ�ÿ�ζ�������   
	f1 = (float) (-0.5*(new_width-1)*fCosa-0.5*(new_height-1)*fSina+0.5*(width-1)); 
	f2 = (float) (0.5*(new_width-1)*fSina-0.5*(new_height-1)*fCosa+0.5*(height-1));   
	   
	if(Graph_Valid(des)) {
		Graph_Free(des);/* �Ƚ�����ڴ��ͷ� */
	}
	des->have_alpha = src->have_alpha;
	// �����ڴ棬�����µ�ͼ��
	if(Graph_Create(des, new_width, new_height) != 0) {
		return -1;
	}

	long m, n, z;
	long src_x, src_y, des_x, des_y;
	// ���ͼ��ÿ�н��в���
	Graph_Lock(src, 0);
	Graph_Lock(des, 1);
	for(des_y = 0; des_y < new_height; ++des_y) {
		m = new_width * des_y;
		// ���ͼ��ÿ�н��в���   
		for(des_x = 0; des_x < new_width; ++des_x) {
			n = m + des_x;
			// �����������Դͼ�е�����   
			src_y = (long) (-((float) des_x) * fSina + ((float) des_y) * fCosa + f2 + 0.5);   
			src_x = (long) ( ((float) des_x) * fCosa + ((float) des_y) * fSina + f1 + 0.5);   
			   
			// �ж��Ƿ���Դͼ��Χ��   
			if( (src_x >= 0) && (src_x < width) 
			&& (src_y >= 0) && (src_y < height)) {
				// ָ��ԴDIB��i0�У���j0�����ص�ָ��
				z = width * src_y + src_x;
				des->rgba[0][n] = src->rgba[0][z];
				des->rgba[1][n] = src->rgba[1][z];
				des->rgba[2][n] = src->rgba[2][z];
				if(Graph_Have_Alpha(des)) {
					des->rgba[3][n] = src->rgba[3][z];
				}
			} else {
				// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255   
				des->rgba[0][n] = 255;
				des->rgba[1][n] = 255;
				des->rgba[2][n] = 255;
				if(Graph_Have_Alpha(des)) {
					des->rgba[3][n] = 0;
				}
			}
		}
	}
	Graph_Unlock(src);
	Graph_Unlock(des);
	return 0;
}
