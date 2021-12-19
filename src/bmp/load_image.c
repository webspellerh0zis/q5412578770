
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_GRAPH_H 
#include LC_DRAW_H

#include <stdio.h>
#include <stdlib.h>

static int detect_image(const char *filepath, LCUI_Graph *out)
/* ���ܣ����ͼƬ��ʽ��������ͼƬ */
{
	int result = 1;
	if (result == 1) {
		result = load_png(filepath, out);  
	}
	if (result == 1) {
		result = load_jpeg(filepath, out);  
	}
	if (result == 1) {
		result = load_bmp(filepath, out); 
	}
	return result;
}

int Load_Image(const char *filepath, LCUI_Graph *out)
/* 
 * ���ܣ�����ָ��ͼƬ�ļ���ͼ������
 * ˵������ͼƬ�ļ������������ڴ棬�򿪵�ͼƬ�ļ�Խ��ռ�õ��ڴ�Ҳ��Խ�� 
 * */
{
	FILE *fp;
	int result = 0;   /* �������Ϊ0 */
	
	Graph_Init(out); 
	out->have_alpha = IS_FALSE;
	/*fp��ȫ�ֱ����������������õ���*/
	if ((fp = fopen(filepath,"r")) == NULL) {
		perror(filepath);
		result = OPEN_ERROR; 
	} else {
		fgetc(fp);
		if (!ferror (fp)) {/*r���û����*/
			fseek(fp,0,SEEK_END);
			if (ftell(fp)>4) {
				fclose(fp);
				result = detect_image(filepath, out); 
			} else {
				result = SHORT_FILE;//�ļ���С 
				fclose(fp);
			}
		}
	}
	return result;   /* ���ش������ */
}

