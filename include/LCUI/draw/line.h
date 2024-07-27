
#ifndef __LCUI_DRAW_LINE_H__
#define __LCUI_DRAW_LINE_H__
/* ����ˮƽ�� */
LCUI_EXPORT(void)
Graph_DrawHorizLine( LCUI_Graph *graph, LCUI_RGB color, int size, LCUI_Pos start, int end_x );

/* ���ƴ�ֱ�� */
LCUI_EXPORT(void)
Graph_DrawVertLine( LCUI_Graph *graph, LCUI_RGB color, int size, LCUI_Pos start, int end_y );

/* ���������� */
LCUI_EXPORT(void)
Graph_DrawBoxLine( LCUI_Graph *graph, LCUI_RGB color, int size, LCUI_Rect rect );
#endif