﻿/*
 * scrollbar.c -- LCUI's scrollbar widget
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/cursor.h>
#include <LCUI/timer.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/scrollbar.h>
#include <LCUI/gui/css_parser.h>

#define EFFECT_FRAMES 80

/** 惯性滚动效果的相关数据 */
typedef struct InertialScrollingRec_ {
	int start_pos;			/**< 开始移动时的位置 */
	int end_pos;			/**< 结束移动时的位置 */
	int timer;			/**< 定时器 */
	int interval;			/**< 定时器的间隔时间 */
	double speed;			/**< 滚动速度 */
	double speed_delta;		/**< 速度差（加速度） */
	int64_t timestamp;		/**< 开始时间 */
	LCUI_BOOL is_running;		/**< 当前效果是否正在运行 */
} InertialScrollingRec, *InertialScrolling;

/** 滚动条的相关数据 */
typedef struct LCUI_ScrollBarRec_ {
	LCUI_Widget box;		/**< 容器 */
	LCUI_Widget layer;		/**< 滚动层 */
	LCUI_Widget slider;		/**< 滑块 */
	LCUI_BOOL is_dragging;		/**< 是否处于拖拽状态 */
	float slider_x, slider_y;	/**< 拖拽开始时的滑块位置 */
	int mouse_x, mouse_y;		/**< 拖拽开始时的鼠标坐标 */
	int touch_point_id;		/**< 触点的ID */
	int direction;			/**< 滚动条的方向（垂直或水平） */
	int scroll_step;		/**< 每次滚动的距离，主要针对使用鼠标滚轮触发的滚动 */
	int pos;			/**< 当前的位置 */
	int old_pos;			/**< 拖拽开始时的位置 */
	int distance;			/**< 滚动距离 */
	int64_t timestamp;		/**< 数据更新时间，主要针对触控拖动时的位置变化 */
	InertialScrollingRec effect;	/**< 用于实现惯性滚动效果的相关数据 */
} LCUI_ScrollBarRec, *LCUI_ScrollBar;

static struct LCUI_ScrollbarModule {
	int event_id;
	LCUI_WidgetPrototype prototype;
} self;

static const char *scrollbar_css = CodeToString(

scrollbar {
	top: 0;
	right: 0;
	width: 14px;
	height: 100%;
	position: absolute;
	background-color: #fafafa;
	border: 1px solid #eee;
}
scrollbar .slider {
	top: 0;
	left: 0;
	width: 14px;
	height: 14px;
	min-width: 14px;
	min-height: 14px;
	position: absolute;
	background-color: #aaa;
}
scrollbar .slider:hover {
	background-color: #bbb;
}
scrollbar .slider:active {
	background-color: #999;
}
.scrolllayer {
	top: 0;
	left: 0;
	position: relative;
}
.scrollbar-horizontal {
	right: auto;
	top: auto;
	bottom: 0;
	left: 0;
	width: 100%;
	height: 14px;
}

);

static void OnInertialScrolling( void *arg )
{
	int pos;
	double distance, time;
	LCUI_ScrollBar scrollbar;
	InertialScrolling effect;
	LCUI_Widget w = arg;
	
	scrollbar = Widget_GetData( w, self.prototype );
	effect = &scrollbar->effect;
	time = (double)LCUI_GetTimeDelta( effect->timestamp ) / 1000;
	distance = (effect->speed + 0.5 * effect->speed_delta * time) * time;
	pos = effect->end_pos + iround( distance );
	DEBUG_MSG( "distance: %g, pos: %d, speed_delta: %g, speed: %g\n",
		   distance, pos, effect->speed_delta,
		   effect->speed + effect->speed_delta * time );
	while( effect->is_running ) {
		double speed = effect->speed + effect->speed_delta * time;
		if( (effect->speed > 0 && speed <= 0) ||
		    (effect->speed < 0 && speed >= 0) ) {
			break;
		}
		ScrollBar_SetPosition( w, pos );
		return;
	}
	LCUITimer_Free( effect->timer );
	effect->is_running = FALSE;
	effect->timer = -1;
}

static void InitInertialScrolling( InertialScrolling effect )
{
	effect->timer = -1;
	effect->end_pos = 0;
	effect->start_pos = 0;
	effect->timestamp = 0;
	effect->speed = 0;
	effect->speed_delta = 320;
	effect->is_running = FALSE;
	effect->interval = 1000 / EFFECT_FRAMES;
}

static void UpdateInertialScrolling( InertialScrolling effect, int pos )
{
	effect->speed = 0;
	effect->is_running = FALSE;
	effect->start_pos = pos;
	effect->timestamp = LCUI_GetTime();
}

static void StartInertialScrolling( LCUI_Widget w )
{
	int distance;
	int64_t time_delta;
	LCUI_ScrollBar scrollbar;
	InertialScrolling effect;
	scrollbar = Widget_GetData( w, self.prototype );
	effect = &scrollbar->effect;
	effect->end_pos = scrollbar->pos;
	distance = effect->end_pos - effect->start_pos;
	time_delta = LCUI_GetTimeDelta( effect->timestamp );
	/* 根据距离计算当前移动速度 */
	if( time_delta > 0 ) {
		effect->speed = 1000.0 * distance / time_delta;
	} else {
		effect->speed = 0;
		return;
	}
	effect->speed_delta = -effect->speed;
	effect->timestamp = LCUI_GetTime();
	if( effect->is_running ) {
		return;
	}
	effect->is_running = TRUE;
	if( effect->timer > 0 ) {
		LCUITimer_Free( effect->timer );
	}
	effect->timer = LCUITimer_Set( effect->interval,
				       OnInertialScrolling, w, TRUE );
	DEBUG_MSG( "start_pos: %d, end_pos: %d\n",
		   effect->start_pos, effect->end_pos );
	DEBUG_MSG( "effect->speed: %g, distance: %d, time: %d\n",
		   effect->speed, distance, (int)time_delta );
}

static void Slider_OnMouseMove( LCUI_Widget slider, 
				LCUI_WidgetEvent e, void *arg )
{
	LCUI_Widget layer;
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar;
	float n, box_size, size, layer_pos, x, y;

	scrollbar = Widget_GetData( w, self.prototype );
	if( !scrollbar->is_dragging || !scrollbar->layer ) {
		return;
	}
	layer = scrollbar->layer;
	if( scrollbar->direction == SBD_HORIZONTAL ) {
		y = 0;
		x = scrollbar->slider_x;
		x += e->motion.x - scrollbar->mouse_x;
		if( scrollbar->box ) {
			box_size = scrollbar->box->box.content.width;
		} else {
			box_size = w->parent->box.content.width;
		}
		layer_pos = scrollbar->layer->box.outer.width - box_size;
		size = w->box.content.width - slider->width;
		if( x > size ) {
			x = size;
		} else if( x < 0 ) {
			x = 0;
		}
		n = 0.0;
		if( size > 0 ) {
			n = slider->x / size;
			if( n > 1.0 ) {
				n = 1;
			}
		}
		layer_pos = layer_pos * n;
		SetStyle( layer->custom_style, key_left, -layer_pos, px );
	} else {
		x = 0;
		y = scrollbar->slider_y;
		y += e->motion.y - scrollbar->mouse_y;
		if( scrollbar->box ) {
			box_size = scrollbar->box->box.content.height;
		} else {
			box_size = w->parent->box.content.height;
		}
		layer_pos = scrollbar->layer->box.outer.height - box_size;
		size = w->box.content.height - slider->height;
		if( y > size ) {
			y = size;
		} else if( y < 0 ) {
			y = 0;
		}
		n = 0.0;
		if( size > 0 ) {
			n = slider->y / size;
			if( n > 1.0 ) {
				n = 1;
			}
		}
		layer_pos = layer_pos * n;
		SetStyle( layer->custom_style, key_top, -layer_pos, px );
	}
	if( scrollbar->pos != iround( layer_pos ) ) {
		LCUI_WidgetEventRec e;
		e.type = self.event_id;
		e.cancel_bubble = TRUE;
		Widget_TriggerEvent( layer, &e, &layer_pos );
	}
	scrollbar->pos = iround( layer_pos );
	Widget_UpdateStyle( layer, FALSE );
	Widget_Move( slider, x, y );
}

static void Slider_OnMouseUp( LCUI_Widget slider, 
			      LCUI_WidgetEvent e, void *arg )
{
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = Widget_GetData( w, self.prototype );
	Widget_UnbindEvent( slider, "mousemove", Slider_OnMouseMove );
	Widget_UnbindEvent( slider, "mouseup", Slider_OnMouseUp );
	Widget_ReleaseMouseCapture( slider );
	scrollbar->is_dragging = FALSE;
}

static void Slider_OnMouseDown( LCUI_Widget slider, 
				LCUI_WidgetEvent e, void *arg )
{
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = Widget_GetData( w, self.prototype );
	if( scrollbar->is_dragging ) {
		return;
	}
	scrollbar->slider_x = slider->x;
	scrollbar->slider_y = slider->y;
	scrollbar->mouse_x = e->motion.x;
	scrollbar->mouse_y = e->motion.y;
	scrollbar->is_dragging = TRUE;
	Widget_SetMouseCapture( slider );
	Widget_BindEvent( slider, "mousemove", Slider_OnMouseMove, w, NULL );
	Widget_BindEvent( slider, "mouseup", Slider_OnMouseUp, w, NULL );
}

static void ScrollBar_OnInit( LCUI_Widget w )
{
	LCUI_Widget slider;
	LCUI_ScrollBar scrollbar;
	const size_t data_size = sizeof( LCUI_ScrollBarRec );

	slider = LCUIWidget_New( NULL );
	scrollbar = Widget_AddData( w, self.prototype, data_size );
	scrollbar->direction = SBD_VERTICAL;
	scrollbar->is_dragging = FALSE;
	scrollbar->scroll_step = 64;
	scrollbar->slider = slider;
	scrollbar->layer = NULL;
	scrollbar->box = NULL;
	scrollbar->old_pos = 0;
	scrollbar->pos = 0;
	scrollbar->touch_point_id = -1;
	InitInertialScrolling( &scrollbar->effect );
	Widget_BindEvent( slider, "mousedown", 
			  Slider_OnMouseDown, w, NULL );
	Widget_AddClass( slider, "slider" );
	Widget_Append( w, slider );
}

static void ScrollBar_UpdateSize( LCUI_Widget w )
{
	float n = 1.0, size, box_size;
	LCUI_ScrollBar scrollbar = Widget_GetData( w, self.prototype );
	LCUI_Widget slider = scrollbar->slider;
	if( !scrollbar->box ) {
		return;
	}
	if( scrollbar->direction == SBD_HORIZONTAL ) {
		if( scrollbar->layer ) {
			size = scrollbar->layer->box.outer.width;
		} else {
			size = 0;
		}
		box_size = scrollbar->box->width;
		if( size > box_size && box_size > 0 ) {
			n = box_size / size;
		}
		SetStyle( slider->custom_style, key_width, n, scale );
	} else {
		if( scrollbar->layer ) {
			size = scrollbar->layer->box.outer.height;
		} else {
			size = 0;
		}
		box_size = scrollbar->box->height;
		if( size > box_size && box_size > 0 ) {
			n = box_size / size;
		}
		SetStyle( slider->custom_style, key_height, n, scale );
	}
	ScrollBar_SetPosition( w, scrollbar->pos );
	Widget_UpdateStyle( slider, FALSE );
	if( n < 1.0 ) {
		Widget_Show( w );
	} else {
		Widget_Hide( w );
	}
}

static void ScrollLayer_OnWheel( LCUI_Widget layer,
				 LCUI_WidgetEvent e, void *arg )
{
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = Widget_GetData( w, self.prototype );
	int pos = ScrollBar_GetPosition( w );
	if( e->wheel.delta > 0 ) {
		pos -= scrollbar->scroll_step;
	} else {
		pos += scrollbar->scroll_step;
	}
	ScrollBar_SetPosition( w, pos );
}

/** 滚动层的触屏事件响应 */
static void ScrollLayer_OnTouch( LCUI_Widget layer,
				 LCUI_WidgetEvent e, void *arg )
{
	uint_t time_delta;
	int i, pos, distance;
	LCUI_TouchPoint point;
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar;
	if( e->touch.n_points < 1 ) {
		return;
	}
	scrollbar = Widget_GetData( w, self.prototype );
	if( scrollbar->touch_point_id == -1 ) {
		point = &e->touch.points[0];
		/* 如果这个触点的状态不是 TOUCHDOWN，则说明是上次触控拖拽操
		 * 作时的多余触点，直接忽略这次触控事件 */
		if( point->state != WET_TOUCHDOWN ) {
			return;
		}
		scrollbar->touch_point_id = point->id;
	} else {
		for( point = NULL, i = 0; i < e->touch.n_points; ++i ) {
			point = &e->touch.points[i];
			if( point->id == scrollbar->touch_point_id ) {
				break;
			}
		}
		if( !point ) {
			return;
		}
	}
	switch( point->state ) {
	case WET_TOUCHDOWN:
		scrollbar->distance = 0;
		scrollbar->effect.speed = 0;
		scrollbar->effect.is_running = FALSE;
		scrollbar->old_pos = scrollbar->pos;
		if( scrollbar->is_dragging ) {
			return;
		}
		Widget_SetTouchCapture( layer, point->id );
		scrollbar->mouse_x = point->x;
		scrollbar->mouse_y = point->y;
		break;
	case WET_TOUCHUP:
		Widget_ReleaseTouchCapture( layer, -1 );
		time_delta = (uint_t)LCUI_GetTimeDelta( scrollbar->timestamp );
		if( scrollbar->is_dragging && time_delta < 50 ) {
			StartInertialScrolling( w );
		}
		scrollbar->touch_point_id = -1;
		scrollbar->is_dragging = FALSE;
		Widget_BlockEvent( layer, FALSE );
		break;
	case WET_TOUCHMOVE:
		pos = scrollbar->old_pos;
		if( scrollbar->direction == SBD_HORIZONTAL ) {
			pos -= point->x - scrollbar->mouse_x;
		} else {
			pos -= point->y - scrollbar->mouse_y;
		}
		if( pos == scrollbar->pos ) {
			break;
		}
		if( !scrollbar->is_dragging ) {
			scrollbar->is_dragging = TRUE;
			LCUIWidget_ClearEventTarget( NULL );
			Widget_BlockEvent( layer, TRUE );
		}
		distance = pos - scrollbar->pos;
		if( (scrollbar->distance > 0) != (distance > 0) ||
			scrollbar->distance == 0 ) {
			UpdateInertialScrolling( &scrollbar->effect,
						 scrollbar->pos );
		}
		scrollbar->distance = distance;
		scrollbar->timestamp = LCUI_GetTime();
		ScrollBar_SetPosition( w, pos );
	default: break;
	}
}

static void ScrollBar_OnUpdateSize( LCUI_Widget box, 
				    LCUI_WidgetEvent e, void *arg )
{
	ScrollBar_UpdateSize( e->data );
}

static void ScrollBar_OnSetPosition( LCUI_Widget box, 
				     LCUI_WidgetEvent e, void *arg )
{
	int *pos = arg;
	ScrollBar_SetPosition( e->data, *pos );
	e->cancel_bubble = TRUE;
}

void ScrollBar_BindBox( LCUI_Widget w, LCUI_Widget box )
{
	LCUI_ScrollBar scrollbar = Widget_GetData( w, self.prototype );
	if( scrollbar->box ) {
		Widget_UnbindEvent( scrollbar->box, "resize",
				    ScrollBar_OnUpdateSize );
	}
	scrollbar->box = box;
	Widget_BindEvent( box, "resize", ScrollBar_OnUpdateSize, w, NULL );
	Widget_BindEvent( box, "setscroll", ScrollBar_OnSetPosition, w, NULL );
	ScrollBar_UpdateSize( w );
}

void ScrollBar_BindLayer( LCUI_Widget w, LCUI_Widget layer )
{
	LCUI_ScrollBar scrollbar = Widget_GetData( w, self.prototype );
	if( scrollbar->layer ) {
		Widget_UnbindEvent( scrollbar->layer, "resize",
				    ScrollBar_OnUpdateSize );
	}
	scrollbar->layer = layer;
	Widget_BindEvent( layer, "resize", ScrollBar_OnUpdateSize, w, NULL );
	Widget_BindEvent( layer, "mousewheel", ScrollLayer_OnWheel, w, NULL );
	Widget_BindEvent( layer, "touch", ScrollLayer_OnTouch, w, NULL );
	ScrollBar_UpdateSize( w );
}

int ScrollBar_GetPosition( LCUI_Widget w )
{
	LCUI_ScrollBar scrollbar = Widget_GetData( w, self.prototype );
	return scrollbar->pos;
}

void ScrollBar_SetPosition( LCUI_Widget w, int pos )
{
	float new_pos, box_size, size, slider_pos;
	LCUI_ScrollBar scrollbar = Widget_GetData( w, self.prototype );
	LCUI_Widget slider = scrollbar->slider;
	LCUI_Widget layer = scrollbar->layer;
	LCUI_WidgetEvent e;

	if( !layer ) {
		return;
	}
	new_pos = 1.0f * pos;
	memset( &e, 0, sizeof( e ) );
	if( scrollbar->direction == SBD_HORIZONTAL ) {
		size = scrollbar->layer->box.outer.width;
		if( scrollbar->box ) {
			box_size = scrollbar->box->box.content.width;
		} else {
			box_size = w->parent->box.content.width;
		}
		if( new_pos + box_size > size ) {
			new_pos = size - box_size;
		}
		if( new_pos < 0.0f ) {
			new_pos = 0.0f;
		}
		slider_pos = w->box.content.width - slider->width;
		slider_pos = slider_pos * new_pos / (size - box_size);
		Widget_SetStyle( slider, key_left, slider_pos, px );
		Widget_SetStyle( layer, key_left, -new_pos, px );
	} else {
		size = scrollbar->layer->box.outer.height;
		if( scrollbar->box ) {
			box_size = scrollbar->box->box.content.height;
		} else {
			box_size = w->parent->box.content.height;
		}
		if( new_pos + box_size > size ) {
			new_pos = size - box_size;
		}
		if( new_pos < 0 ) {
			new_pos = 0;
		}
		slider_pos = w->box.content.height - slider->height;
		if( size == box_size ) {
			slider_pos = 0;
		} else {
			slider_pos = slider_pos * new_pos / (size - box_size);
		}
		Widget_SetStyle( slider, key_top, slider_pos, px );
		Widget_SetStyle( layer, key_top, -new_pos, px );
	}
	pos = iround( new_pos );
	if( scrollbar->pos != pos ) {
		LCUI_WidgetEventRec e;
		e.type = self.event_id;
		e.cancel_bubble = TRUE;
		Widget_TriggerEvent( layer, &e, &new_pos );
	}
	scrollbar->pos = pos;
	Widget_UpdateStyle( slider, FALSE );
	Widget_UpdateStyle( layer, FALSE );
}

void ScrollBar_SetDirection( LCUI_Widget w, int direction )
{
	LCUI_ScrollBar scrollbar = Widget_GetData( w, self.prototype );
	if( direction == SBD_HORIZONTAL ) {
		Widget_RemoveClass( w, "scrollbar-horizontal" );
	} else {
		Widget_AddClass( w, "scrollbar-horizontal" );
	}
	scrollbar->direction = direction;
}

static void ScrollBar_OnSetAttr( LCUI_Widget w,
				 const char *name, const char *value )
{
	LCUI_Widget target;
	if( strcmp( name, "data-parent" ) == 0 ) {
		target = LCUIWidget_GetById( value );
		if( target ) {
			ScrollBar_BindBox( w, target );
		}
	} else if( strcmp( name, "data-layer" ) == 0 ) {
		target = LCUIWidget_GetById( value );
		if( target ) {
			ScrollBar_BindLayer( w, target );
		}
	}
}

void LCUIWidget_AddTScrollBar( void )
{
	int setscroll_event_id;
	self.prototype = LCUIWidget_NewPrototype( "scrollbar", NULL );
	self.prototype->init = ScrollBar_OnInit;
	self.prototype->setattr = ScrollBar_OnSetAttr;
	self.event_id = LCUIWidget_AllocEventId();
	setscroll_event_id = LCUIWidget_AllocEventId();
	LCUIWidget_SetEventName( self.event_id, "scroll" );
	LCUIWidget_SetEventName( setscroll_event_id, "setscroll" );
	LCUI_LoadCSSString( scrollbar_css, __FILE__ );
}
