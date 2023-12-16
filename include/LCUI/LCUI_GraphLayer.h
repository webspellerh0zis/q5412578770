
#ifndef __LCUI_GRAPHLAYER_H__
#define __LCUI_GRAPHLAYER_H__

LCUI_BEGIN_HEADER

typedef struct _LCUI_GraphLayer LCUI_GraphLayer;

struct _LCUI_GraphLayer
{
	LCUI_BOOL		visible;	/* 图层是否可见 */
	LCUI_BOOL		inherit_alpha;	/* 是否继承父图层的透明度 */
	int		z_index;	/* 图层的堆叠顺序，值越大，图层显示位置越靠前 */
	LCUI_Pos	pos;		/* 图层的xy轴坐标 */
	
	LCUI_GraphLayer	*parent;	/* 该图层的容器图层 */
	LCUI_Queue		child;		/* 该图层中内的子图层记录 */
	LCUI_Graph		graph;		/* 图层像素数据 */
};

/* 将子图层从父图层中的子图层队列中移除 */
int GraphLayer_DeleteChild( LCUI_GraphLayer *child_glayer );

/* 释放图层占用的内存资源 */
void GraphLayer_Free( LCUI_GraphLayer *glayer );

/* 创建新的图层 */
LCUI_GraphLayer *
GraphLayer_New( void );

/* 添加子图层至容器图层中 */
int GraphLayer_AddChild(	LCUI_GraphLayer *des_ctnr,
				LCUI_GraphLayer *glayer );

/* 移动子图层至新的容器图层中 */
int GraphLayer_MoveChild(	LCUI_GraphLayer *new_ctnr, 
				LCUI_GraphLayer *glayer );

/* 获取图层矩形 */
LCUI_Rect GraphLayer_GetRect( LCUI_GraphLayer *glayer );

/* 获取图层尺寸 */
LCUI_Size GraphLayer_GetSize( LCUI_GraphLayer *glayer );

/* 获取图层的全局透明度 */
uchar_t GraphLayer_GetAlpha( LCUI_GraphLayer *glayer );

/* 图层是否继承父图层的透明度 */
void GraphLayer_InerntAlpha( LCUI_GraphLayer *glayer, LCUI_BOOL flag );

/* 设定图层的XY轴坐标 */
int GraphLayer_SetPos( LCUI_GraphLayer *glayer, int x, int y );

/* 设定图层的全局透明度 */
void GraphLayer_SetAlpha( LCUI_GraphLayer *glayer, uchar_t alpha );

/* 设定图层的Z轴坐标，调用此函数后，需要调用GraphLayer_Sort函数对图层列表进行排序 */
int GraphLayer_SetZIndex( LCUI_GraphLayer *glayer, int z_index );

/* 根据子图层的z-index值，对目标图层的子图层进行排序 */
int GraphLayer_Sort( LCUI_GraphLayer *glayer );

/* 调整图层的大小 */
int GraphLayer_Resize( LCUI_GraphLayer *glayer, int w, int h );

/* 获取指定图层中指定坐标上存在的子图层 */
LCUI_GraphLayer *
GraphLayer_ChildAt( LCUI_GraphLayer *ctnr, int x, int y );

/* 获取指向图层自身图形数据的指针 */
LCUI_Graph *GraphLayer_GetSelfGraph(	LCUI_GraphLayer *glayer );

/* 获取指定根图层中的子图层的有效区域 */
LCUI_Rect GraphLayer_GetValidRect(
	LCUI_GraphLayer *root_glayer, LCUI_GraphLayer *glayer );

/* 指定根容器图层，获取当前子图层相对于根容器图层的全局坐标 */
LCUI_Pos GraphLayer_GetGlobalPos( 
	LCUI_GraphLayer *root_glayer, LCUI_GraphLayer *glayer );

/* 获取与图层中指定区域内层叠的子图层 */
int GraphLayer_GetLayers(
	LCUI_GraphLayer *glayer, 
	LCUI_Rect rect, LCUI_Queue *queue );

/* 获取该图层和子图层混合后的图形数据 */
int GraphLayer_GetGraph(	LCUI_GraphLayer *ctnr, 
				LCUI_Graph *graph_buff,
				LCUI_Rect rect );

/* 将指定图层显示在同等z-index值图层的前端 */
int GraphLayer_Front( LCUI_GraphLayer *glayer );

/* 显示图层 */
int GraphLayer_Show( LCUI_GraphLayer *glayer );

/* 隐藏图层 */
int GraphLayer_Hide( LCUI_GraphLayer *glayer );

LCUI_END_HEADER

#endif
