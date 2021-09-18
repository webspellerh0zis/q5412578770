//测试LCUI的图像混合速度

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_MEM_H

int main()
{
	clock_t time, t;
	int i, w, h;
	LCUI_Graph bg, fg;
	
	Graph_Init(&bg);
	Graph_Init(&fg);
	bg.flag = NO_ALPHA;
	fg.flag = HAVE_ALPHA; 
	w = 1640;
	h = 1480;
	//为前景图和背景图分配内存
	Malloc_Graph(&bg, w, h);
	Malloc_Graph(&fg, w, h);
	printf("graph size: %dx%d\n", w, h);
	time = clock(); //开始计时
	for(i=0; i<=255; ++i){
		t = clock(); 
		nobuff_print("[%2d] mix graph, alpha = %3d, use time: ", i, i);
		Fill_Graph_Alpha(&fg, i);
		Mix_Graph(&bg, &fg, Pos(0,0));
		printf("%ldms\n", clock()-t);//结束计时，输出结果
	} 
	printf("total time: %ldms\n", clock()-time);
	return 0;
}

