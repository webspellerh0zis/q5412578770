﻿/* ***************************************************************************
 * widget_base.c -- the widget base operation set.
 *
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
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
 * widget_base.c -- 部件的基本操作集。
 *
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
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
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/button.h>
#include <LCUI/gui/widget/sidebar.h>

static struct LCUIWidgetModule {
	LCUI_Widget root;	/** 获取根级部件 */
	LCUI_RBTree ids;	/** 各种部件的ID索引 */
} LCUIWidget;

LCUI_Widget LCUIWidget_GetRoot(void)
{
	return LCUIWidget.root;
}

int Widget_Append( LCUI_Widget parent, LCUI_Widget w )
{
	LinkedListNode *node, *snode;
	DEBUG_MSG("parent: %p, w: %p\n", parent, w);
	if( !parent || !w || parent == w->parent ) {
		return -1;
	}
	if( parent == w ) {
		return -2;
	}
	node = Widget_GetNode( w );
	snode = Widget_GetShowNode( w );
	if( w->parent ) {
		LinkedList_Unlink( &w->parent->children, node );
		LinkedList_Unlink( &w->parent->children_show, snode );
		Widget_PostSurfaceEvent( w, WET_REMOVE );
	}
	w->parent = parent;
	LinkedList_AppendNode( &parent->children, node );
	LinkedList_AppendNode( &parent->children_show, snode );
	Widget_FlushZIndex( w );
	Widget_PostSurfaceEvent( w, WET_ADD );
	Widget_AddTaskToSpread( w, WTT_REFRESH_STYLE );
	Widget_UpdateTaskStatus( w );
	Widget_AddTask( parent, WTT_LAYOUT );
	DEBUG_MSG("tip\n");
	return 0;
}

int Widget_Unwrap( LCUI_Widget *widget )
{
	LCUI_Widget self, child;
	LinkedList *list, *list_show;
	LinkedListNode *target, *node, *prev, *snode;

	self = *widget;
	if( !self->parent ) {
		return -1;
	}
	list = &self->parent->children;
	list_show = &self->parent->children_show;
	node = Widget_GetNode( self );
	target = node->prev;
	LinkedList_ForEach( node, &self->children ) {
		prev = node->prev;
		child = node->data;
		snode = Widget_GetShowNode( child );
		LinkedList_Unlink( &self->children, node );
		LinkedList_Unlink( &self->children_show, snode );
		child->parent = self->parent;
		LinkedList_Link( list, target, node );
		LinkedList_AppendNode( list_show, snode );
		Widget_FlushZIndex( child );
		Widget_AddTaskToSpread( child, WTT_REFRESH_STYLE );
		Widget_UpdateTaskStatus( child );
		node = prev;
	}
	Widget_Destroy( widget );
	return 0;
}

/** 构造函数 */
static void Widget_Init( LCUI_Widget widget )
{
	memset( widget, 0, sizeof(struct LCUI_WidgetRec_));
	widget->custom_style = StyleSheet();
	widget->cached_style = StyleSheet();
	widget->style = StyleSheet();
	widget->computed_style.visible = TRUE;
	widget->computed_style.position = SV_STATIC;
	widget->computed_style.display = SV_BLOCK;
	widget->computed_style.pointer_events = SV_AUTO;
	widget->inherited_style = StyleSheet();
	widget->computed_style.width.type = SVT_AUTO;
	widget->computed_style.height.type = SVT_AUTO;
	widget->computed_style.box_sizing = SV_CONTENT_BOX;
	widget->computed_style.opacity = 1.0;
	widget->computed_style.margin.top.type = SVT_PX;
	widget->computed_style.margin.right.type = SVT_PX;
	widget->computed_style.margin.bottom.type = SVT_PX;
	widget->computed_style.margin.left.type = SVT_PX;
	widget->computed_style.padding.top.type = SVT_PX;
	widget->computed_style.padding.right.type = SVT_PX;
	widget->computed_style.padding.bottom.type = SVT_PX;
	widget->computed_style.padding.left.type = SVT_PX;
	widget->event = LCUIEventBox_Create();
	widget->event = LCUIEventBox_Create();
	Widget_InitTaskBox( widget );
	Background_Init( &widget->computed_style.background );
	BoxShadow_Init( &widget->computed_style.shadow );
	Border_Init( &widget->computed_style.border );
	LinkedList_Init( &widget->children );
	LinkedList_Init( &widget->children_show );
	LinkedList_Init( &widget->dirty_rects );
	Graph_Init( &widget->graph );
	LCUIMutex_Init( &widget->mutex );
}

LCUI_Widget LCUIWidget_New( const char *type_name )
{
	LCUI_WidgetClass *wc;
	LinkedListNode *node;
	LCUI_Widget widget = malloc( sizeof(struct LCUI_WidgetRec_) +
				     sizeof(LinkedListNode)*2 );
	Widget_Init( widget );
	node = Widget_GetNode( widget );
	node->data = widget;
	node->next = node->prev = NULL;
	node = Widget_GetShowNode( widget );
	node->data = widget;
	node->next = node->prev = NULL;
	if( type_name ) {
		widget->type = strdup( type_name );
		wc = LCUIWidget_GetClass( type_name );
		if( wc ) {
			wc->methods.init( widget );
		}
	}
	Widget_AddTask( widget, WTT_REFRESH_STYLE );
	return widget;
}

static void Widget_OnDestroy( void *arg )
{
	LCUI_Widget widget = (LCUI_Widget)arg;
	Widget_SetId( widget, NULL );
	Widget_DestroyTaskBox( widget );
	LCUIEventBox_Destroy( widget->event );
	widget->event = NULL;
	LinkedList_ClearData( &widget->children, Widget_OnDestroy );
	LinkedList_ClearData( &widget->children_show, NULL );
	LinkedList_Clear( &widget->dirty_rects, free );
	Widget_PostSurfaceEvent( widget, WET_REMOVE );
	Widget_AddTask( widget->parent, WTT_LAYOUT );
	free( widget );
}

void Widget_ExecDestroy( LCUI_Widget *widget )
{
	LCUI_Widget w = *widget;
	if( w->parent ) {
		LinkedListNode *node = Widget_GetNode( w );
		LinkedList_Unlink( &w->parent->children, node );
		node = Widget_GetShowNode( w );
		LinkedList_Unlink( &w->parent->children_show, node );
	}
	Widget_OnDestroy( w );
	*widget = NULL;
}

void Widget_Destroy( LCUI_Widget *w )
{
	LCUI_Widget root = *w;
	while( root->parent ) {
		root = root->parent;
	}
	if( root == LCUIWidget.root ) {
		Widget_AddTask( *w, WTT_DESTROY );
		*w = NULL;
	} else {
		Widget_ExecDestroy( w );
	}
}

LCUI_Widget Widget_At( LCUI_Widget widget, int x, int y )
{
	LCUI_BOOL is_hit;
	LinkedListNode *node;
	LCUI_Widget target = widget, c = NULL;
	do {
		is_hit = FALSE;
		LinkedList_ForEach( node, &target->children_show ) {
			c = node->data;
			if( !c->computed_style.visible ) {
				continue;
			}
			if( LCUIRect_HasPoint(&c->box.border, x, y) ) {
				target = c;
				x -= c->box.content.x;
				y -= c->box.content.y;
				is_hit = TRUE;
				break;
			 }
		}
	} while( is_hit );
	return (target == widget) ? NULL:target;
}

LCUI_Widget LCUIWidget_GetById( const char *idstr )
{
	if( !idstr ) {
		return NULL;
	}
	return RBTree_CustomGetData( &LCUIWidget.ids, idstr );
}

int Widget_Top( LCUI_Widget w )
{
	DEBUG_MSG("tip\n");
	return Widget_Append( LCUIWidget.root, w );
}

void Widget_SetTitleW( LCUI_Widget w, const wchar_t *title )
{
	int len;
	wchar_t *new_title, *old_title;

	len = wcslen(title) + 1;
	new_title = (wchar_t*)malloc(sizeof(wchar_t)*len);
	if( !new_title ) {
		return;
	}
	wcsncpy( new_title, title, len );
	old_title = w->title;
	w->title = new_title;
	if( old_title ) {
		free( old_title );
	}
	Widget_AddTask( w, WTT_TITLE );
}

int Widget_SetId( LCUI_Widget w, const char *idstr )
{
	if( w->id ) {
		RBTree_CustomErase( &LCUIWidget.ids, idstr );
		free( w->id );
		w->id = NULL;
	}
	if( !idstr ) {
		return -1;
	}
	w->id = strdup( idstr );
	if( RBTree_CustomInsert( &LCUIWidget.ids, idstr, w ) ) {
		return 0;
	}
	return -2;
}

/** 计算边框样式 */
static void ComputeBorderStyle( LCUI_StyleSheet ss, LCUI_Border *b )
{
	LCUI_Style *style;
	int key = key_border_start + 1;
	for( ; key < key_border_end; ++key ) {
		style = &ss->sheet[key];
		if( !style->is_valid ) {
			continue;
		}
		switch( key ) {
		case key_border_color:
			b->top.color = style->color;
			b->right.color = style->color;
			b->bottom.color = style->color;
			b->left.color = style->color;
			break;
		case key_border_style:
			b->top.style = style->value;
			b->right.style = style->value;
			b->bottom.style = style->value;
			b->left.style = style->value;
			break;
		case key_border_width:
			b->top.width = style->value;
			b->right.width = style->value;
			b->bottom.width = style->value;
			b->left.width = style->value;
			break;
		case key_border_top_color:
			b->top.color = style->color;
			break;
		case key_border_right_color:
			b->right.color = style->color;
			break;
		case key_border_bottom_color:
			b->bottom.color = style->color;
			break;
		case key_border_left_color:
			b->left.color = style->color;
			break;
		case key_border_top_width:
			b->top.width = style->value;
			break;
		case key_border_right_width:
			b->right.width = style->value;
			break;
		case key_border_bottom_width:
			b->bottom.width = style->value;
			break;
		case key_border_left_width:
			b->left.width = style->value;
			break;
		case key_border_top_style:
			b->top.style = style->value;
			break;
		case key_border_right_style:
			b->right.style = style->value;
			break;
		case key_border_bottom_style:
			b->bottom.style = style->value;
			break;
		case key_border_left_style:
			b->left.style = style->value;
			break;
		default: break;
		}
	}
}

void Widget_FlushBorder( LCUI_Widget w )
{
	LCUI_Rect rect;
	LCUI_Border ob, *nb;
	ob = w->computed_style.border;
	ComputeBorderStyle( w->style, &w->computed_style.border );
	nb = &w->computed_style.border;
	/* 如果边框变化并未导致图层尺寸变化的话，则只重绘边框 */
	if( ob.top.width != nb->top.width || 
	    ob.right.width != nb->right.width ||
	    ob.bottom.width != nb->bottom.width ||
	    ob.left.width != nb->left.width ) {
		Widget_AddTask( w, WTT_RESIZE );
		Widget_AddTask( w, WTT_POSITION );
		return;
	}
	rect.x = rect.y = 0;
	rect.width = w->box.border.width;
	rect.width -= max( ob.top_right_radius, ob.right.width );
	rect.height = max( ob.top_left_radius, ob.top.width );
	/* 上 */
	Widget_InvalidateArea( w, &rect, SV_BORDER_BOX );
	rect.x = w->box.border.w;
	rect.width = max( ob.top_right_radius, ob.right.width );
	rect.x -= rect.width;
	rect.height = w->box.border.height;
	rect.height -= max( ob.bottom_right_radius, ob.bottom.width );
	/* 右 */
	Widget_InvalidateArea( w, &rect, SV_BORDER_BOX );
	rect.x = max( ob.bottom_left_radius, ob.left.width );
	rect.y = w->box.border.height;
	rect.width = w->box.border.width;
	rect.width -= rect.x;
	rect.height = max( ob.bottom_right_radius, ob.bottom.width );
	rect.y -= rect.height;
	/* 下 */
	Widget_InvalidateArea( w, &rect, SV_BORDER_BOX );
	rect.width = rect.x;
	rect.x = 0;
	rect.y = max( ob.top_left_radius, ob.left.width );
	rect.height = w->box.border.height;
	rect.height -= rect.y;
	/* 左 */
	Widget_InvalidateArea( w, &rect, SV_BORDER_BOX );
}

/** 计算矩形阴影样式 */
static void ComputeBoxShadowStyle( LCUI_StyleSheet ss, LCUI_BoxShadow *bsd )
{
	LCUI_Style *style;
	int key = key_box_shadow_start + 1;
	for( ; key < key_box_shadow_end; ++key ) {
		style = &ss->sheet[key];
		if( !style->is_valid ) {
			continue;
		}
		switch( key ) {
		case key_box_shadow_x: bsd->x = style->value; break;
		case key_box_shadow_y: bsd->y = style->value; break;
		case key_box_shadow_spread: bsd->spread = style->value; break;
		case key_box_shadow_blur: bsd->blur = style->value; break;
		case key_box_shadow_color: bsd->color = style->color; break;
		default: break;
		}
	}
}

void Widget_FlushBoxShadow( LCUI_Widget w )
{
	LCUI_BoxShadow bs = w->computed_style.shadow;
	ComputeBoxShadowStyle( w->style, &w->computed_style.shadow );
	/* 如果阴影变化并未导致图层尺寸变化，则只重绘阴影 */
	if( bs.x == w->computed_style.shadow.x && 
	    bs.y == w->computed_style.shadow.y && 
	    bs.spread == w->computed_style.shadow.spread ) {
		LCUI_Rect rects[4];
		LCUIRect_CutFourRect( &w->box.border,
				      &w->box.graph, rects );
		Widget_InvalidateArea( w, &rects[0], SV_GRAPH_BOX );
		Widget_InvalidateArea( w, &rects[1], SV_GRAPH_BOX );
		Widget_InvalidateArea( w, &rects[2], SV_GRAPH_BOX );
		Widget_InvalidateArea( w, &rects[3], SV_GRAPH_BOX );
		return;
	}
	Widget_AddTask( w, WTT_RESIZE );
	Widget_AddTask( w, WTT_POSITION );
}

void Widget_FlushVisibility( LCUI_Widget w )
{
	LCUI_Style *s;
	LCUI_BOOL visible = TRUE;
	s = &w->cached_style->sheet[key_visible];
	if( w->computed_style.display == SV_NONE
	    || (s->is_valid && !s->value) ) {
		visible = FALSE;
	}
	s = &w->style->sheet[key_display];
	if( w->style->sheet[key_display].is_valid ) {
		w->computed_style.display = s->style;
	} else {
		w->computed_style.display = SV_BLOCK;
	}
	w->computed_style.visible = TRUE;
	s = &w->style->sheet[key_visible];
	if( w->computed_style.display == SV_NONE || 
	    (s->is_valid && !s->value) ) {
		w->computed_style.visible = FALSE;
	}
	if( w->computed_style.visible == visible ) {
		return;
	}
	visible = w->computed_style.visible;
	if( w->parent ) {
		Widget_InvalidateArea( w, NULL, SV_GRAPH_BOX );
		if( (w->computed_style.display == SV_BLOCK ||
		      w->computed_style.display == SV_INLINE_BLOCK)
		    && w->computed_style.position == SV_STATIC
		    && w->computed_style.float_mode == SV_NONE ) {
			Widget_AddTask( w->parent, WTT_LAYOUT );
		}
	}
	DEBUG_MSG( "visible: %s\n", visible ? "TRUE" : "FALSE" );
	Widget_PostSurfaceEvent( w, visible ? WET_SHOW : WET_HIDE );
}

int Widget_GetIndex( LCUI_Widget w )
{
	int index = 0;
	LinkedListNode *node;
	if( !w->parent ) {
		return 0;
	}
	node = Widget_GetNode( w );
	node = node->prev;
	while( node->prev ) {
		node = node->prev;
		++index;
	}
	return index;
}

void Widget_FlushZIndex( LCUI_Widget w )
{
	LinkedList *list;
	int index = Widget_GetIndex( w );
	LCUI_Style *s = &w->style->sheet[key_z_index];
	LinkedListNode *cnode, *csnode, *snode = Widget_GetShowNode( w );
	if( s->is_valid && s->type == SVT_VALUE ) {
		w->computed_style.z_index = s->value;
	} else {
		w->computed_style.z_index = 0;
	}
	if( !w->parent ) {
		return;
	}
	list = &w->parent->children_show;
	LinkedList_Unlink( list, snode );
	LinkedList_ForEach( cnode, list ) {
		LCUI_Widget child = cnode->data;
		LCUI_WidgetStyle *ccs = &child->computed_style;
		csnode = Widget_GetShowNode( child );
		if( ccs->position == w->computed_style.position ) {
			if( w->computed_style.position == SV_STATIC ||
			    w->computed_style.z_index < ccs->z_index ) {
				continue;
			}
			if( index < Widget_GetIndex( child ) ) {
				continue;
			}
		} else if( w->computed_style.position == SV_STATIC ) {
			continue;
		} else if( w->computed_style.z_index < ccs->z_index ) {
			continue;
		} else if( w->computed_style.z_index == ccs->z_index ) {
			if( index < Widget_GetIndex( child ) ) {
				continue;
			}
		}
		LinkedList_Link( list, csnode->prev, snode );
		break;
	}
	if( !cnode ) {
		LinkedList_AppendNode( list, snode );
	}
	if( w->computed_style.position != SV_STATIC ) {
		Widget_AddTask( w, WTT_REFRESH );
	}
}

static int ComputePositionOffset( LCUI_Widget w, int key )
{
	LCUI_Style *s = &w->style->sheet[key];
	switch( s->type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		}
		return w->parent->box.content.width * s->scale;
	case SVT_PX: return s->px;
	case SVT_NONE:
	case SVT_AUTO:
	default: break;
	}
	return 0;
}

static int ComputeStyleOption( LCUI_Widget w, int key, int default_value )
{
	if( !w->style->sheet[key].is_valid ) {
		return default_value;
	}
	if( w->style->sheet[key].type != SVT_STYLE ) {
		return default_value;
	}
	return w->style->sheet[key].style;
}

void Widget_FlushPosition( LCUI_Widget w )
{
	LCUI_Rect rect = w->box.graph;
	int position = ComputeStyleOption( w, key_position, SV_STATIC );
	w->computed_style.left = ComputePositionOffset( w, key_left );
	w->computed_style.top = ComputePositionOffset( w, key_top );
	w->computed_style.right = ComputePositionOffset( w, key_right );
	w->computed_style.bottom = ComputePositionOffset( w, key_bottom );
	if( w->parent && w->computed_style.position != position ) {
		Widget_AddTask( w->parent, WTT_LAYOUT );
	}
	w->computed_style.position = position;
	if( position == SV_ABSOLUTE ) {
		w->x = 0;
		w->y = 0;
		if( w->style->sheet[key_left].is_valid ) {
			w->x = w->computed_style.left;
		} else if( w->style->sheet[key_right].is_valid ) {
			if( w->parent ) {
				w->x = w->parent->box.content.width;
				w->x -= w->width;
			}
			w->x -= w->computed_style.right;
		}
		if( w->style->sheet[key_top].is_valid ) {
			w->y = w->computed_style.top;
		} else if( w->style->sheet[key_bottom].is_valid ) {
			if( w->parent ) {
				w->y = w->parent->box.content.height;
				w->y -= w->height;
			}
			w->y -= w->computed_style.bottom;
		}
	}
	/* 以x、y为基础 */
	w->box.border.x = w->x;
	w->box.border.y = w->y;
	w->box.content.x = w->x;
	w->box.content.y = w->y;
	w->box.outer.x = w->x;
	w->box.outer.y = w->y;
	w->box.graph.x = w->x;
	w->box.graph.y = w->y;
	/* 计算各个框的坐标 */
	w->box.content.x += w->computed_style.border.left.width + w->padding.left;
	w->box.content.y += w->computed_style.border.top.width + w->padding.top;
	w->box.outer.x -= w->margin.left;
	w->box.outer.y -= w->margin.top;
	w->box.graph.x -= BoxShadow_GetBoxX(&w->computed_style.shadow);
	w->box.graph.y -= BoxShadow_GetBoxY(&w->computed_style.shadow);
	if( w->parent ) {
		DEBUG_MSG("new-rect: %d,%d,%d,%d\n", w->box.graph.x, w->box.graph.y, w->box.graph.w, w->box.graph.h);
		DEBUG_MSG("old-rect: %d,%d,%d,%d\n", rect.x, rect.y, rect.w, rect.h);
		/* 标记移动前后的区域 */
		Widget_InvalidateArea( w->parent, &w->box.graph, SV_CONTENT_BOX );
		Widget_InvalidateArea( w->parent, &rect, SV_CONTENT_BOX );
	}
	/* 检测是否为顶级部件并做相应处理 */
	Widget_PostSurfaceEvent( w, WET_MOVE );
}

/** 更新位图尺寸 */
static void Widget_UpdateGraphBox( LCUI_Widget w )
{
	LCUI_Rect *rb = &w->box.border;
	LCUI_Rect *rg = &w->box.graph;
	rg->x = w->x - BoxShadow_GetBoxX( &w->computed_style.shadow );
	rg->y = w->y - BoxShadow_GetBoxY( &w->computed_style.shadow );
	rg->width = BoxShadow_GetWidth( &w->computed_style.shadow, rb->width );
	rg->height = BoxShadow_GetHeight( &w->computed_style.shadow, rb->height );
	/* 如果有会产生透明效果的样式 */
	if( w->computed_style.border.bottom_left_radius > 0
	 || w->computed_style.border.bottom_right_radius > 0
	 || w->computed_style.border.top_left_radius > 0
	 || w->computed_style.border.top_right_radius > 0
	 || w->computed_style.background.color.alpha < 255
	 || w->computed_style.shadow.blur > 0 ) {
		w->graph.color_type = COLOR_TYPE_ARGB;
	} else {
		w->graph.color_type = COLOR_TYPE_RGB;
	}
	Graph_Create( &w->graph, rg->width, rg->height );
}

/** 计算合适的内容框大小 */
static void Widget_ComputeContentSize( LCUI_Widget w, int *width, int *height )
{
	int n;
	LCUI_Widget child;
	LinkedListNode *node;

	*width = *height = 0;
	LinkedList_ForEach( node, &w->children_show ) {
		child = node->data;
		if( !child->computed_style.visible ) {
			continue;
		}
		n = child->box.outer.x + child->box.outer.width;
		if( n > *width ) {
			*width = n;
		}
		n = child->box.outer.y + child->box.outer.height;
		if( n > *height ) {
			*height = n;
		}
	}
}

/** 计算尺寸 */
static void Widget_ComputeSize( LCUI_Widget w )
{
	switch( w->computed_style.width.type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		 }
		w->width = w->parent->box.content.width;
		w->width *= w->computed_style.width.scale;
		break;
	case SVT_PX:
		w->width = w->computed_style.width.px;
		break;
	case SVT_NONE:
	case SVT_AUTO:
	default:
		w->width = 0;
		break;
	}
	switch( w->computed_style.height.type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		 }
		w->height = w->parent->box.content.height;
		w->height *= w->computed_style.height.scale;
		break;
	case SVT_PX:
		w->height = w->computed_style.height.px;
		break;
	case SVT_NONE:
	case SVT_AUTO:
	default:
		w->height = 0;
		break;
	}
	if( w->computed_style.height.type == SVT_AUTO
	 || w->computed_style.width.type == SVT_AUTO ) {
		LCUI_WidgetClass *wc;
		int width, height;

		wc = LCUIWidget_GetClass( w->type );
		if( wc && wc->methods.autosize ) {
			wc->methods.autosize( w, &width, &height );
		} else {
			Widget_ComputeContentSize( w, &width, &height );
		}
		if( w->computed_style.width.type == SVT_AUTO ) {
			w->width = width;
		}
		if( w->computed_style.height.type == SVT_AUTO ) {
			w->height = height;
		}
	}
	w->box.border.width = w->width;
	w->box.border.height = w->height;
	w->box.content.width = w->width;
	w->box.content.height = w->height;
	/* 如果是以边框盒作为尺寸调整对象，则需根据边框盒计算内容框尺寸 */
	if( w->computed_style.box_sizing == SV_BORDER_BOX ) {
		/* 名字太长了，都放一行里代码会太长，只好分解成多行了 */
		w->box.content.width -= w->computed_style.border.left.width;
		w->box.content.width -= w->computed_style.border.right.width;
		w->box.content.width -= w->padding.left;
		w->box.content.width -= w->padding.right;
		w->box.content.height -= w->computed_style.border.top.width;
		w->box.content.height -= w->computed_style.border.bottom.width;
		w->box.content.height -= w->padding.top;
		w->box.content.height -= w->padding.bottom;
	} else {
		/* 否则是以内容框作为尺寸调整对象，需计算边框盒的尺寸 */
		w->box.border.width += w->computed_style.border.left.width;
		w->box.border.width += w->computed_style.border.right.width;
		w->box.border.width += w->padding.left;
		w->box.border.width += w->padding.right;
		w->box.border.height += w->computed_style.border.top.width;
		w->box.border.height += w->computed_style.border.bottom.width;
		w->box.border.height += w->padding.top;
		w->box.border.height += w->padding.bottom;
	}
	/* 先暂时用边框盒区域作为外部区域，等加入外边距设置后再改 */
	w->box.outer = w->box.border;
}

static void Widget_SendResizeEvent( LCUI_Widget w )
{
	LinkedListNode *node;
	LCUI_Widget child;
	LCUI_WidgetEvent e;
	e.type_name = "resize";
	e.type = WET_RESIZE;
	e.cancel_bubble = TRUE;
	e.target = w;
	e.data = NULL;
	Widget_SendEvent( w, &e, NULL );
	Widget_AddTask( w, WTT_REFRESH );
	Widget_PostSurfaceEvent( w, WET_RESIZE );
	LinkedList_ForEach( node, &w->children ) {
		child = node->data;
		if( child->computed_style.width.type == SVT_SCALE || 
		    child->computed_style.height.type == SVT_SCALE ) {
			Widget_AddTask( child, WTT_RESIZE );
		}
	}
}

void Widget_FlushSize( LCUI_Widget w )
{
	LCUI_Rect rect;
	int i, box_sizing;
	struct { 
		LCUI_Style *sval;
		int *ival;
		int key;
	} pd_map[4] = {
		{ &w->computed_style.padding.top, &w->padding.top, key_padding_top },
		{ &w->computed_style.padding.right, &w->padding.right, key_padding_right },
		{ &w->computed_style.padding.bottom, &w->padding.bottom, key_padding_bottom },
		{ &w->computed_style.padding.left, &w->padding.left, key_padding_left }
	};
	rect = w->box.graph;
	/* 从样式表中获取尺寸 */
	w->computed_style.width = w->style->sheet[key_width];
	w->computed_style.height = w->style->sheet[key_height];
	/* 内边距的单位暂时都用 px  */
	for( i=0; i<4; ++i ) {
		LCUI_Style *s = &w->style->sheet[pd_map[i].key];
		if( !s->is_valid || s->type != SVT_PX ) {
			pd_map[i].sval->type = SVT_PX;
			pd_map[i].sval->px = 0;
			*pd_map[i].ival = 0;
			continue;
		}
		*pd_map[i].sval = *s;
		*pd_map[i].ival = s->px;
	}
	box_sizing = ComputeStyleOption( w, key_box_sizing, SV_CONTENT_BOX );
	w->computed_style.box_sizing = box_sizing;
	Widget_ComputeSize( w );
	Widget_UpdateGraphBox( w );
	/* 若尺寸无变化则不继续处理 */
	if( rect.width == w->box.graph.width &&
	    rect.height == w->box.graph.height ) {
		return;
	}
	/* 若在变化前后的宽高中至少有一个为 0，则不继续处理 */
	if( (w->box.graph.width <= 0 || w->box.graph.height <= 0) &&
	    (rect.width <= 0 || rect.height <= 0) ) {
		return;
	}
	if( w->computed_style.width.type != SVT_AUTO ) {
		Widget_AddTask( w, WTT_LAYOUT );
	}
	if( w->parent ) {
		Widget_InvalidateArea( w->parent, &rect, SV_CONTENT_BOX );
		rect.width = w->box.graph.width;
		rect.height = w->box.graph.height;
		Widget_InvalidateArea( w->parent, &rect, SV_CONTENT_BOX );	
		if( w->parent->computed_style.width.type == SVT_AUTO
		    || w->parent->computed_style.height.type == SVT_AUTO ) {
			Widget_AddTask( w->parent, WTT_RESIZE );
		}
		if( w->computed_style.display != SV_NONE
		    && w->computed_style.position == SV_STATIC ) {
			Widget_AddTask( w->parent, WTT_LAYOUT );
		}
	}
	Widget_SendResizeEvent( w );
}

void Widget_FlushProps( LCUI_Widget w )
{
	int prop = ComputeStyleOption( w, key_pointer_events, SV_AUTO );
	w->computed_style.pointer_events = prop;
}

/** 计算内边距 */
void Widget_ComputePadding( LCUI_Widget w )
{
	int i;
	double result;
	struct {
		LCUI_Style *value;
		int *buffer;
		int size;
	} map[4] = {
		{
			&w->computed_style.padding.top,
			&w->padding.top,
			w->parent->box.content.height
		}, {
			&w->computed_style.padding.right,
			&w->padding.right,
			w->parent->box.content.width
		}, {
			&w->computed_style.padding.bottom,
			&w->padding.bottom,
			w->parent->box.content.height
		}, {
			&w->computed_style.padding.left,
			&w->padding.left,
			w->parent->box.content.width
		}
	};
	for( i=0; i<4; ++i ) {
		switch( map[i].value->type ) {
		case SVT_SCALE:
			if( !w->parent ) {
				break;
			}
			result = map[i].size * map[i].value->scale / 100;
			*map[i].buffer = (int)result;
			break;
		case SVT_PX:
			*map[i].buffer = map[i].value->px;
			break;
		case SVT_NONE:
		case SVT_AUTO:
		default:
			// ...
			break;
		}
	}
}

/** 计算内边距 */
void Widget_ComputeMargin( LCUI_Widget w )
{
	int i;
	double result;
	struct {
		LCUI_Style *value;
		int *buffer;
		int size;
	} map[4] = {
		{
			&w->computed_style.margin.top,
			&w->margin.top,
			w->parent->box.content.height
		}, {
			&w->computed_style.margin.right,
			&w->margin.right,
			w->parent->box.content.width
		}, {
			&w->computed_style.margin.bottom,
			&w->margin.bottom,
			w->parent->box.content.height
		}, {
			&w->computed_style.margin.left,
			&w->margin.left,
			w->parent->box.content.width
		}
	};
	for( i=0; i<4; ++i ) {
		switch( map[i].value->type ) {
		case SVT_SCALE:
			if( !w->parent ) {
				break;
			}
			result = map[i].size * map[i].value->scale / 100;
			*map[i].buffer = (int)result;
			break;
		case SVT_PX:
			*map[i].buffer = map[i].value->px;
			break;
		case SVT_NONE:
		case SVT_AUTO:
		default:
			// ...
			break;
		}
	}
}

/** 设置内边距 */
void Widget_SetPadding( LCUI_Widget w, int top, int right, int bottom, int left )
{
	SetStyle( w->custom_style, key_padding_top, top, px );
	SetStyle( w->custom_style, key_padding_right, right, px );
	SetStyle( w->custom_style, key_padding_bottom, bottom, px );
	SetStyle( w->custom_style, key_padding_left, left, px );
}

/** 设置外边距 */
void Widget_SetMargin( LCUI_Widget w, int top, int right, int bottom, int left )
{
	SetStyle( w->custom_style, key_margin_top, top, px );
	SetStyle( w->custom_style, key_margin_right, right, px );
	SetStyle( w->custom_style, key_margin_bottom, bottom, px );
	SetStyle( w->custom_style, key_margin_left, left, px );
	Widget_Update( w, FALSE );
}

void Widget_Move( LCUI_Widget w, int left, int top )
{
	SetStyle( w->custom_style, key_top, top, px );
	SetStyle( w->custom_style, key_left, left, px );
	DEBUG_MSG("top = %d, left = %d\n", top, left);
	Widget_Update( w, FALSE );
}

void Widget_Resize( LCUI_Widget w, int width, int height )
{
	SetStyle( w->custom_style, key_width, width, px );
	SetStyle( w->custom_style, key_height, height, px );
	Widget_Update( w, FALSE );
}

void Widget_Show( LCUI_Widget w )
{
	SetStyle( w->custom_style, key_visible, TRUE, int );
	Widget_Update( w, FALSE );
}

void Widget_Hide( LCUI_Widget w )
{
	SetStyle( w->custom_style, key_visible, FALSE, int );
	Widget_Update( w, FALSE );
}

void Widget_SetBackgroundColor( LCUI_Widget w, LCUI_Color color )
{
	w->computed_style.background.color = color;
}

void Widget_Lock( LCUI_Widget w )
{
	LCUIMutex_Lock( &w->mutex );
}

void Widget_Unlock( LCUI_Widget w )
{
	LCUIMutex_Unlock( &w->mutex );
}

static int StrList_AddOne( char ***strlist, const char *str )
{
	int i = 0;
	char **newlist;

	if( !*strlist ) {
		newlist = (char**)malloc( sizeof(char*) * 2 );
		goto check_done;
	}
	for( i = 0; (*strlist)[i]; ++i ) {
		if( strcmp((*strlist)[i], str) == 0 ) {
			return 0;
		}
	}
	newlist = (char**)realloc( *strlist, (i+2)*sizeof(char*) );
check_done:
	if( !newlist ) {
		return 0;
	}
	newlist[i] = strdup(str);
	newlist[i+1] = NULL;
	*strlist = newlist;
	return 1;
}

int StrList_Add( char ***strlist, const char *str )
{
	char buff[256];
	int count = 0, i, head;

	for( head = 0, i = 0; str[i]; ++i ) {
		if( str[i] != ' ' ) {
			continue;
		}
		if( i - 1 > head ) {
			strncpy( buff, &str[head], i - head );
			buff[i - head] = 0;
			count += StrList_AddOne( strlist, buff );
		}
		head = i + 1;
	}
	if( i - 1 > head ) {
		strncpy( buff, &str[head], i - head );
		buff[i - head] = 0;
		count += StrList_AddOne( strlist, buff );
	}
	return count;
}

LCUI_BOOL StrList_Has( char **strlist, const char *str )
{
	int i;
	if( !strlist ) {
		return FALSE;
	}
	for( i = 0; strlist[i]; ++i ) {
		if( strcmp(strlist[i], str) == 0 ) {
			return TRUE;
		}
	}
	return FALSE;
}

static int StrList_RemoveOne( char ***strlist, const char *str )
{
	int i, pos, len;
	char **newlist;

	if( !*strlist ) {
		return 0;
	}
	for( pos = 0,i = 0; (*strlist)[i] != NULL; ++i ) {
		if( strcmp((*strlist)[i], str) == 0 ) {
			pos = i;
		}
	}
	if( pos == 0 && i < 2 ) {
		free( *strlist );
		*strlist = NULL;
		return 1;
	}
	len = i - 1;
	newlist = (char**)malloc( i * sizeof(char*) );
	for( i = 0; i < pos; ++i ) {
		newlist[i] = (*strlist)[i];
	}
	for( i = pos; i < len; ++i ) {
		newlist[i] = (*strlist)[i+1];
	}
	newlist[i] = NULL;
	free( (*strlist)[pos] );
	free( *strlist );
	*strlist = newlist;
	return 1;
}

int StrList_Remove( char ***strlist, const char *str )
{
	char buff[256];
	int count = 0, i, head;

	for( head = 0, i = 0; str[i]; ++i ) {
		if( str[i] != ' ' ) {
			continue;
		}
		if( i - 1 > head ) {
			strncpy( buff, &str[head], i - head );
			buff[i - head] = 0;
			count += StrList_RemoveOne( strlist, buff );
		}
		head = i + 1;
	}
	if( i - 1 > head ) {
		strncpy( buff, &str[head], i - head );
		buff[i - head] = 0;
		count += StrList_RemoveOne( strlist, buff );
	}
	return count;
}

/** 为部件添加一个类 */
int Widget_AddClass( LCUI_Widget w, const char *class_name )
{
	if( StrList_Add(&w->classes, class_name) <= 0 ) {
		return 0;
	}
	// 标记需要更新该部件及子级部件的样式表
	Widget_AddTaskToSpread( w, WTT_UPDATE_STYLE );
	return 1;
}

/** 判断部件是否包含指定的类 */
LCUI_BOOL Widget_HasClass( LCUI_Widget w, const char *class_name )
{
	return StrList_Has( w->classes, class_name );
}

/** 从部件中移除一个类 */
int Widget_RemoveClass( LCUI_Widget w, const char *class_name )
{
	if( StrList_Remove( &w->classes, class_name ) <= 0 ) {
		return 0;
	}
	Widget_AddTask( w, WTT_REFRESH_STYLE );
	Widget_AddTaskToSpread( w, WTT_REFRESH_STYLE );
	return 1;
}

/** 为部件添加一个状态 */
int Widget_AddStatus( LCUI_Widget w, const char *status_name )
{
	if( StrList_Add( &w->pseudo_classes, status_name ) <= 0 ) {
		return 0;
	}
	Widget_AddTask( w, WTT_REFRESH_STYLE );
	Widget_AddTaskToSpread( w,  WTT_REFRESH_STYLE );
	return 1;
}

/** 判断部件是否包含指定的状态 */
LCUI_BOOL Widget_HasStatus( LCUI_Widget w, const char *status_name )
{
	return StrList_Has( w->pseudo_classes, status_name );
}

/** 从部件中移除一个状态 */
int Widget_RemoveStatus( LCUI_Widget w, const char *status_name )
{
	if( StrList_Remove( &w->pseudo_classes, status_name ) != 1 ) {
		return 0;
	}
	Widget_AddTask( w, WTT_REFRESH_STYLE );
	Widget_AddTaskToSpread( w,  WTT_REFRESH_STYLE );
	return 1;
}

/** 更新子部件的布局 */
void Widget_UpdateLayout( LCUI_Widget w )
{
	struct {
		int x, y;
		int line_height;
		LCUI_Widget prev;
		int prev_display;
		int max_width;
	} ctx = { 0 };
	LCUI_Widget child;
	LinkedListNode *node;

	ctx.max_width = 256;
	for( child = w; child; child = child->parent ) {
		switch( child->computed_style.width.type ) {
		case SVT_PX:
		case SVT_SCALE:
			ctx.max_width = child->box.content.width;
			break;
		case SVT_AUTO:
		default:continue;
		}
		break;
	}
	LinkedList_ForEach( node, &w->children ) {
		child = node->data;
		if( child->computed_style.position != SV_STATIC ) {
			continue;
		}
		switch( child->computed_style.display ) {
		case SV_NONE: continue;
		case SV_BLOCK:
			child->x = ctx.x;
			child->y = ctx.y;
			ctx.x = 0;
			ctx.line_height = 0;
			ctx.y += child->box.outer.height;
			break;
		case SV_INLINE_BLOCK:
			child->x = ctx.x;
			child->y = ctx.y;
			if( child->box.outer.height > ctx.line_height ) {
				ctx.line_height = child->box.outer.height;
			}
			if( ctx.prev && ctx.prev_display != SV_INLINE_BLOCK ) {
				ctx.x = 0;
				ctx.y += ctx.line_height;
				break;
			}
			ctx.x += child->box.outer.width;
			break;
		default: continue;
		}
		Widget_FlushPosition( child );
		ctx.prev = child;
		ctx.prev_display = child->computed_style.display;
	}
	if( w->computed_style.height.type == SVT_AUTO
	 || w->computed_style.width.type == SVT_AUTO ) {
		Widget_AddTask( w, WTT_RESIZE );
	}
}

static void _LCUIWidget_PrintTree( LCUI_Widget w, int depth, const char *prefix )
{
	int i, len;
	LCUI_Widget child;
	LinkedListNode *node;
	char str[16], child_prefix[512], classes[512];

	len = strlen(prefix);
	strcpy( child_prefix, prefix );
	LinkedList_ForEach( node, &w->children ) {
		if( node == w->children.tail.prev ) {
			strcpy( str, "└" );
			strcpy( &child_prefix[len], "    " );
		} else {
			strcpy( str, "├" );
			strcpy( &child_prefix[len], "│  " );
		} 
		strcat( str, "─" );
		child = node->data;
		if( child->children.length == 0 ) {
			strcat( str, "─" );
		} else {
			strcat( str, "┬" );
		}
		classes[0] = 0;
		if( child->classes ) {
			for( i = 0; child->classes[i]; ++i ) {
				strcat( classes, child->classes[i] );
				strcat( classes, " ");
			}
		} else {
			strcpy( classes, "(null)" );
		}
		printf("%s%s type: %s, class: %s, xy:(%d,%d), size:(%d,%d), visible: %s\n", 
			prefix, str, child->type, classes, child->x, child->y, 
			child->width, child->height, 
			child->computed_style.visible ? "true":"false");

		_LCUIWidget_PrintTree( child, depth+1, child_prefix );
	}
}

void Widget_PrintTree( LCUI_Widget w )
{
	w = w ? w : LCUIWidget.root;
	printf("%s, xy:(%d,%d), size:(%d,%d), visible: %s\n", 
		w->type, w->x, w->y, w->width, w->height,
		w->computed_style.visible ? "true":"false");
	_LCUIWidget_PrintTree( w, 0, "  " );
}

static int CompareWidgetId( void *data, const void *keydata )
{
	return strcmp(((LCUI_Widget)data)->id, (const char*)keydata);
}

void LCUI_InitWidget(void)
{
	LCUIWidget_InitTask();
	LCUIWidget_InitEvent();
	LCUIWidget_InitLibrary();
	LCUIWidget_InitStyle();
	LCUIWidget_AddTextView();
	LCUIWidget_AddButton();
	LCUIWidget_AddSideBar();
	RBTree_Init( &LCUIWidget.ids );
	RBTree_SetDataNeedFree( &LCUIWidget.ids, FALSE );
	RBTree_OnJudge( &LCUIWidget.ids, CompareWidgetId );
	LCUIWidget.root = LCUIWidget_New("root");
	Widget_SetTitleW( LCUIWidget.root, L"LCUI Display" );
}

void LCUI_ExitWidget(void)
{

}
