/* ***************************************************************************
 * rect.c -- Rectangle area handling
 * 
 * Copyright (C) 2012-2014 by
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
 * rect.c -- 矩形区域处理
 *
 * 版权所有 (C) 2012-2014 归属于
 * 刘超
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/
 
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H

#define LCUIRect_IsIncludeRect(a,b)	\
	b->x >= a->x && b->x + b->w <= a->x + a->w \
	&& b->y >= a->y && b->y + b->h <= a->y + a->h

/* 将数值转换成LCUI_Rect型结构体 */
LCUI_API LCUI_Rect Rect( int x, int y, int w, int h )
{
	LCUI_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

/** 根据容器尺寸，获取指定区域中需要裁剪的区域 */
LCUI_API void LCUIRect_GetCutArea( LCUI_Size box_size, LCUI_Rect rect,
							LCUI_Rect *cut )
{
	cut->x = 0;
	cut->y = 0;
	cut->width = rect.width;
	cut->height = rect.height;
	/* 获取需裁剪的区域 */
	if(rect.x < 0) {
		cut->width += rect.x;
		cut->x = 0 - rect.x;
	}
	if(rect.x + rect.width > box_size.w) {
		cut->width -= (rect.x + rect.width - box_size.w); 
	}
	
	if(rect.y < 0) {
		cut->height += rect.y;
		cut->y = 0 - rect.y; 
	}
	if(rect.y + rect.height > box_size.h) {
		cut->height -= (rect.y + rect.height - box_size.h); 
	}
}

/** 根据容器尺寸，获取指定区域的有效显示区域 */
LCUI_API LCUI_Rect LCUIRect_ValidateArea( LCUI_Size box_size, LCUI_Rect rect )
{
	if (rect.x < 0) {
		rect.width += rect.x;
		rect.x = 0;
	}
	if (rect.y < 0) {
		rect.height += rect.y;
		rect.y = 0;
	}
	
	if (rect.x + rect.width > box_size.w) {
		if(rect.x < box_size.w) {
			rect.width = box_size.w - rect.x; 
		} else {
			rect.width = 0;
		}
	}
	if (rect.y + rect.height > box_size.h) {
		if(rect.y < box_size.h) {
			rect.height = box_size.h - rect.y; 
		} else {
			rect.height = 0;
		}
	}
	return rect;
}

/** 检测矩形是否遮盖另一个矩形 */
LCUI_API LCUI_BOOL LCUIRect_IsCoverRect( LCUI_Rect rect1, LCUI_Rect rect2 )
{
	if( rect1.x > rect2.x ) {
		if( rect2.x + rect2.w <= rect1.x ) {
			return FALSE;
		}
	} else {
		if( rect1.x + rect1.w <= rect2.x ) {
			return FALSE;
		}
	}
	if( rect1.y > rect2.y ) {
		if( rect2.y + rect2.h <= rect1.y ) {
			return FALSE;
		}
	} else {
		if( rect1.y + rect1.h <= rect2.y ) {
			return FALSE;
		}
	}
	return TRUE;
}

/** 
 * 获取两个矩形中的重叠矩形
 * @returns 如果两个矩形重叠，则返回TRUE，否则返回FALSE
 */
static LCUI_BOOL 
LCUIRect_GetOverlayRect( LCUI_Rect *a, LCUI_Rect *b, LCUI_Rect *out )
{
	if( a->x > b->x ) {
		out->x = a->x;
		out->w = b->x + b->w - a->x;
	} else {
		out->x = b->x;
		out->w = a->x + a->w - b->x;
	}
	if( a->y > b->y ) {
		out->y = a->y;
		out->h = b->y + b->h - a->y;
	} else {
		out->y = b->y;
		out->h = a->y + a->h - b->y;
	}
	if( out->w <= 0 || out->h <= 0 ) {
		return FALSE;
	}
	return TRUE;
}

static void LCUIRect_MergeRect( LCUI_Rect *big, LCUI_Rect *a, LCUI_Rect *b )
{
	if( a->x < b->x ) {
		big->x = a->x;
	} else {
		big->x = b->x;
	}
	if( a->x + a->w < b->x + b->w ) {
		big->w = b->x + b->w - big->x;
	} else {
		big->w = a->x + a->w - big->x;
	}if( a->y < b->y ) {
		big->y = a->y;
	} else {
		big->y = b->y;
	}
	if( a->y + a->h < b->y + b->h ) {
		big->h = b->y + b->h - big->y;
	} else {
		big->h = a->y + a->h - big->y;
	}
}

/** 
 * 根据重叠矩形 rect1，将矩形 rect2 分割成四个矩形
 * 分割方法如下：
 * ┏━━┳━━━━━━┓
 * ┃    ┃     3      ┃
 * ┃ 0  ┣━━━┳━━┃
 * ┃    ┃rect1 ┃    ┃
 * ┃    ┃      ┃ 2  ┃
 * ┣━━┻━━━┫    ┃
 * ┃     1      ┃    ┃
 * ┗━━━━━━┻━━┛
 *
 * rect2 必须被 rect1 完全包含
 */
static void LCUIRect_CutFourRect( LCUI_Rect *rect1, LCUI_Rect *rect2, 
				LCUI_Rect rects[4] )
{
	rects[0].x = rect2->x;
	rects[0].y = rect2->y;
	rects[0].w = rect1->x - rect2->x;
	rects[0].h = rect1->y + rect1->h - rect2->y;

	rects[1].x = rect2->x;
	rects[1].y = rects[0].y + rects[0].h;
	rects[1].w = rect1->x + rect1->w - rect2->x;
	rects[1].h = rect2->y + rect2->h - rects[1].y;

	rects[2].x = rect1->x + rect1->w;
	rects[2].y = rect1->y;
	rects[2].w = rect2->x + rect2->w - rects[2].x;
	rects[2].h = rect2->y + rect2->h - rects[2].y;

	rects[3].x = rect1->x;
	rects[3].y = rect2->y;
	rects[3].w = rect2->x + rect2->w - rects[3].x;
	rects[3].h = rect1->y - rects[3].y;
}

/** 
 * 根据重叠矩形 rect1，将矩形 rect2 分割成两个矩形
 * rect1必须在 rect2 内，且与rect2有一个顶点相同 
 */
static int LCUIRect_CutTwoRect( LCUI_Rect *rect1, LCUI_Rect *rect2, 
				LCUI_Rect rects[2] )
{
	/* 两个相交矩形，除去重叠区域，最多可分割出两个区域。
		* 根据与重叠矩形的坐标，判断新矩形在重叠矩形的哪个位置 */
	if( rect2->x == rect1->x ) {
		/* 右下方 */
		if( rect2->y == rect1->y ) {
			rects[0].x = rect2->x + rect1->w;
			rects[0].y = rect1->y;
			rects[0].w = rect2->x + rect2->w - rects[0].x;
			rects[0].h = rect1->h;
			rects[1].x = rect2->x;
			rects[1].y = rect1->h;
			rects[1].w = rect2->w;
			rects[1].h = rect2->y + rect2->h - rects[0].y;
			return 0;
		}
		/* 右上方 */
		rects[0].x = rect2->x;
		rects[0].y = rect2->y;
		rects[0].w = rect2->w;
		rects[0].h = rect2->h - rect1->h;
		rects[1].x = rect2->x + rect1->w;
		rects[1].w = rect2->w - rect1->w;
		rects[1].y = rect1->y;
		rects[1].h = rect1->h;
		return 1;
	}
	/* 左下方 */
	if( rect2->y == rect1->y ) {
		rects[0].x = rect2->x;
		rects[0].y = rect2->y;
		rects[0].w = rect2->w - rect1->w;
		rects[0].h = rect1->h;
		rects[1].x = rect2->x;
		rects[1].y = rect2->y + rect1->h;
		rects[1].w = rect2->w - rect1->w;
		rects[1].h = rect2->h - rect1->h;
		return 2;
	}
	/* 左上方 */
	rects[0].x = rect2->x;
	rects[0].y = rect2->y;
	rects[0].w = rect2->w;
	rects[0].h = rect2->h - rect1->h;
	rects[1].x = rect2->x;
	rects[1].y = rect2->y + rect1->h;
	rects[1].w = rect2->w - rect1->w;
	rects[1].h = rect1->h;
	return 3;
}

/** 初始化脏矩形记录 */
LCUI_API void DirtyRectList_Init( LCUI_DirtyRectList *list )
{
	LinkedList_Init( list, sizeof(LCUI_Rect) );
	LinkedList_SetDataMemReuse( list, TRUE );
	LinkedList_SetDataNeedFree( list, TRUE );
}

/** 销毁脏矩形记录 */
LCUI_API void DirtyRectList_Destroy( LCUI_DirtyRectList *list )
{
	LinkedList_Destroy( list );
}

/** 添加一个脏矩形记录 */
LCUI_API int DirtyRectList_Add( LCUI_DirtyRectList *list, LCUI_Rect *rect )
{
	int ret;
	LCUI_Rect *p_rect, o_rect, tmp_rect[2];

	if( rect->w <= 0 || rect->h <= 0 ) {
		return -1;
	}
	/* 定位至链表表头 */
	LinkedList_Goto( list, 0 );
	DEBUG_MSG("list: %p, total: %d, rect(%d,%d,%d,%d)\n", list, LinkedList_GetTotal(list), rect->x, rect->y, rect->w, rect->h);
	while( !LinkedList_IsAtEnd(list) ) {
		p_rect = (LCUI_Rect*)LinkedList_Get( list );
		DEBUG_MSG("p_rect(%d,%d,%d,%d)\n", 
			p_rect->x, p_rect->y, p_rect->w, p_rect->h);
		/* 如果被现有的矩形包含 */
		if( LCUIRect_IsIncludeRect( p_rect, rect ) ) {
			DEBUG_MSG("not add\n");
			return -2;
		}
		/* 如果包含现有的矩形 */
		if( LCUIRect_IsIncludeRect( rect, p_rect ) ) {
			DEBUG_MSG("delete\n");
			LinkedList_Delete( list );
			continue;
		}
		/* 如果与现有的矩形不重叠 */
		if( !LCUIRect_GetOverlayRect( rect, p_rect, &o_rect ) ) {
			DEBUG_MSG("skip\n");
			LinkedList_ToNext( list );
			continue;
		}
		DEBUG_MSG("overlay rect(%d,%d,%d,%d)\n",
			o_rect.x, o_rect.y, o_rect.w, o_rect.h);
		/* 如果两个矩形相距很近，则直接合并 */
		if( rect->w - o_rect.w < 20 && rect->h - o_rect.h < 20
		|| p_rect->w - o_rect.w < 20 && p_rect->h - o_rect.h < 20
		|| p_rect->x == rect->x && p_rect->w == rect->w
		|| p_rect->y == rect->y && p_rect->h == rect->h ) {
			LCUIRect_MergeRect( &o_rect, p_rect, rect );
			DEBUG_MSG("merge rect, rect(%d,%d,%d,%d)\n",
				o_rect.x, o_rect.y, o_rect.w, o_rect.h);
			/* 删除当前矩形 */
			LinkedList_Delete( list );
			/* 添加合并后的矩形 */
			return DirtyRectList_Add( list, &o_rect );
		}
		LCUIRect_CutTwoRect( &o_rect, rect, tmp_rect );
		DEBUG_MSG("cut rect, (%d,%d,%d,%d), (%d,%d,%d,%d), child: (%d,%d,%d,%d), (%d,%d,%d,%d)\n",
			o_rect.x, o_rect.y, o_rect.w, o_rect.h,
			rect->x, rect->y, rect->w, rect->h,
			tmp_rect[0].x, tmp_rect[0].y, tmp_rect[0].w, tmp_rect[0].h,
			tmp_rect[1].x, tmp_rect[1].y, tmp_rect[1].w, tmp_rect[1].h);
		ret = DirtyRectList_Add( list, &tmp_rect[0] );
		ret |= DirtyRectList_Add( list, &tmp_rect[1] );
		return ret;
	}
	DEBUG_MSG("add\n");
	/* 验证通过，则添加进去 */
	LinkedList_AddDataCopy( list, rect );
	return 0;
}

/** 删除脏矩形 */
LCUI_API int DirtyRectList_Delete( LCUI_DirtyRectList *list, LCUI_Rect *rect )
{
	int i;
	LCUI_Rect *p_rect, o_rect, tmp_rect[4];

	if( rect->w <= 0 || rect->h <= 0 ) {
		return -1;
	}
	DEBUG_MSG("list: %p, total: %d, rect(%d,%d,%d,%d)\n", list, LinkedList_GetTotal(list), rect->x, rect->y, rect->w, rect->h);
	/* 定位至链表表头 */
	LinkedList_Goto( list, 0 );
	while( !LinkedList_IsAtEnd(list) ) {
		p_rect = (LCUI_Rect*)LinkedList_Get( list );
		DEBUG_MSG("[%d] p_rect(%d,%d,%d,%d)\n", list->current_node_pos, p_rect->x, p_rect->y, p_rect->w, p_rect->h);
		/* 如果包含现有的矩形 */
		if( LCUIRect_IsIncludeRect( rect, p_rect ) ) {
			DEBUG_MSG("delete\n");
			LinkedList_Delete( list );
			continue;
		}
		/* 如果被现有的矩形包含，则分割现有矩形 */
		if( LCUIRect_IsIncludeRect( p_rect, rect ) ) {
			DEBUG_MSG("cut 4\n");
			LCUIRect_CutFourRect( rect, p_rect, tmp_rect );
			LinkedList_Delete( list );
			for( i=0; i<4; ++i ) {
				if( tmp_rect[i].w <= 0
				|| tmp_rect[i].h <= 0 ) {
					continue;
				}
				LinkedList_InsertCopy( list, &tmp_rect[0] );
			}
			/* 
			 * 既然现有矩形包含了这个矩形，那么不用继续遍历了，
			 * 因为不会有其它矩形会与这个矩形相交，直接退出即可。 
			 */
			return 0;
		}
		/* 如果与现有的矩形不重叠 */
		if( !LCUIRect_GetOverlayRect( rect, p_rect, &o_rect ) ) {
			DEBUG_MSG("skip\n");
			LinkedList_ToNext( list );
			continue;
		}
		DEBUG_MSG("cut 2\n");
		LCUIRect_CutTwoRect( &o_rect, p_rect, tmp_rect );
		LinkedList_Delete( list );
		LinkedList_InsertCopy( list, &tmp_rect[0] );
		LinkedList_InsertCopy( list, &tmp_rect[1] );
		/* 跳过这两个新加入的矩形 */
		LinkedList_ToNext( list );
		LinkedList_ToNext( list );
	}
	DEBUG_MSG("quit\n");
	return 1;
}
