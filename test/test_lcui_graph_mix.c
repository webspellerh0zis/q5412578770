﻿//测试LCUI的图像混合速度

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_GRAPH_H
#include <time.h>

int main(int argc, char **argv)
{
	clock_t time, t;
	int i, w, h;
	LCUI_Graph bg, fg;
	
	Graph_Init(&bg);
	Graph_Init(&fg);
	bg.color_type = COLOR_TYPE_RGB;
	fg.color_type = COLOR_TYPE_RGBA;
	w = 1640;
	h = 1480;
	//为前景图和背景图分配内存
	Graph_Create(&bg, w, h);
	Graph_Create(&fg, w, h);
	printf("graph size: %dx%d\n", w, h);
	time = clock(); //开始计时
	for(i=0; i<=255; i+=5){
		t = clock(); 
		nobuff_printf("[%2d] mix graph, alpha = %3d, use time: ", i, i);
		Graph_FillAlpha(&fg, i);
		Graph_Mix(&bg, &fg, Pos(0,0));
		printf("%ldms\n", clock()-t);//结束计时，输出结果
	} 
	printf("total time: %ldms\n", clock()-time);
	return 0;
}

