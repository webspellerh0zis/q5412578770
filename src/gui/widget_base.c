/* ***************************************************************************
 * widget_base.c -- the widget base operation set.
 *
 * Copyright (C) 2012-2013 by
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
 * widget_base.c -- �����Ļ�����������
 *
 * ��Ȩ���� (C) 2012-2013 ������
 * ����
 *
 * ����ļ���LCUI��Ŀ��һ���֣�����ֻ���Ը���GPLv2���Э����ʹ�á����ĺͷ�����
 *
 * (GPLv2 �� GNUͨ�ù������֤�ڶ��� ��Ӣ����д)
 *
 * ����ʹ�á��޸Ļ򷢲����ļ����������Ѿ��Ķ�����ȫ���ͽ���������Э�顣
 *
 * LCUI ��Ŀ�ǻ���ʹ��Ŀ�Ķ�����ɢ���ģ��������κε������Σ�����û�������Ի���
 * ����;���������������������GPLv2���Э�顣
 *
 * ��Ӧ���յ������ڱ��ļ���GPLv2���Э��ĸ�������ͨ����LICENSE.TXT�ļ��У����
 * û�У���鿴��<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_GRAPH_H
#include LC_DISPLAY_H
#include LC_DRAW_H
#include LC_FONT_H
#include LC_ERROR_H
#include LC_CURSOR_H
#include LC_INPUT_H

#include <math.h>
#include <limits.h>

/* �������� */
static int
Record_WidgetUpdate(LCUI_Widget *widget, void *data, DATATYPE type, int flag);


/*************************** Container ********************************/
/* �������������Ϊ�����Ĳ����� */
LCUI_API int
Widget_Container_Add( LCUI_Widget *ctnr, LCUI_Widget *widget )
{
	int pos;
	LCUI_GraphLayer *ctnr_glayer;
	LCUI_Queue *old_queue, *new_queue;

	//_DEBUG_MSG("ctnr widget: %p, widget: %p\n", ctnr, widget);
	if( !widget || ctnr == widget->parent ) {
		return -1;
	}
	/* �������ΪNULL���Ǿʹ���������Ļ��Ϊ���� */
	if( !ctnr ) {
		new_queue = &LCUI_Sys.widget_list;
		ctnr_glayer = LCUI_Sys.root_glayer;
	} else {/* ������ָ������Ϊ���� */
		new_queue = &ctnr->child;
		ctnr_glayer = ctnr->client_glayer;
	}

	/* ��������и��������Ǿ��ڸ��������Ӳ��������� */
	if( widget->parent ) {
		old_queue = &widget->parent->child;
	} else {/* ����û�в�������ô��������ڴ���ʱ�ʹ�����ϵͳ���������� */
		old_queue = &LCUI_Sys.widget_list;
	}
	/* �������ѻ�ù����㣬��λ֮ǰ�����еĽ��� */
	if( Widget_GetFocus( widget ) ) {
		Reset_Focus( widget->parent );
	}

	/* �ı�ò�������������Ҫ������֮ǰ���������Ƴ� */
	pos = Queue_Find( old_queue, widget );
	if(pos >= 0) {
		Queue_DeletePointer(old_queue, pos);
	}

	widget->parent = ctnr; /* ���游����ָ�� */
	Queue_AddPointer( new_queue, widget ); /* ������������� */
	/* ������ͼ���ƶ����µĸ�ͼ���� */
	GraphLayer_MoveChild( ctnr_glayer, widget->main_glayer );
	Widget_UpdatePos( widget );
	return 0;
}

LCUI_API int
_Widget_GetContainerWidth(LCUI_Widget *widget)
/* ͨ������ó�ָ�������������Ŀ�ȣ���λΪ���� */
{
	int width;
	widget = widget->parent;
	if( !widget ) {
		return LCUIScreen_GetWidth();
	}
	if( widget->w.which_one == 0 ) {
		width = widget->w.px;
	} else {
		width = _Widget_GetContainerWidth( widget );
		width *= widget->w.scale;
	}
	width -= widget->padding.left;
	width -= widget->padding.right;
	return width;
}

LCUI_API int
_Widget_GetContainerHeight(LCUI_Widget *widget)
/* ͨ������ó�ָ�������������ĸ߶ȣ���λΪ���� */
{
	int height;
	widget = widget->parent;
	if( !widget ) {
		return LCUIScreen_GetHeight();
	}
	if( widget->h.which_one == 0 ) {
		height = widget->h.px;
	} else {
		height = _Widget_GetContainerHeight( widget );
		height *= widget->h.scale;
	}
	height -= widget->padding.top;
	height -= widget->padding.bottom;
	return height;
}

/* ��ȡ�����Ŀ�� */
LCUI_API int
Widget_GetContainerWidth( LCUI_Widget *widget )
{
	int width;
	if( !widget ) {
		return LCUIScreen_GetWidth();
	}
	width = _Widget_GetWidth( widget );
	width -= (widget->padding.left + widget->padding.right);
	return width;
}

/* ��ȡ�����ĸ߶� */
LCUI_API int
Widget_GetContainerHeight( LCUI_Widget *widget )
{
	int height;
	if( !widget ) {
		return LCUIScreen_GetHeight();
	}
	height = _Widget_GetHeight( widget );
	height -= (widget->padding.top + widget->padding.bottom);
	return height;
}

/* ��ȡ�����ĳߴ� */
LCUI_API LCUI_Size
Widget_GetContainerSize( LCUI_Widget *widget )
{
	LCUI_Size size;
	size.w = Widget_GetContainerWidth( widget );
	size.h = Widget_GetContainerHeight( widget );
	return size;
}
/************************* Container End ******************************/

/***************************** Widget *********************************/

/* ��ȡ��������ͼ��ָ�� */
LCUI_API LCUI_GraphLayer *
Widget_GetMainGraphLayer( LCUI_Widget *widget )
{
	if( widget == NULL ) {
		return LCUI_Sys.root_glayer;
	}
	return widget->main_glayer;
}

/* ��ȡ�����Ŀͻ���ͼ��ָ�� */
LCUI_API LCUI_GraphLayer *
Widget_GetClientGraphLayer( LCUI_Widget *widget )
{
	if( widget == NULL ) {
		return LCUI_Sys.root_glayer;
	}
	return widget->client_glayer;
}

/* ��ȡ�������Ӳ������� */
LCUI_API LCUI_Queue*
Widget_GetChildList( LCUI_Widget *widget )
{
	if( widget == NULL ) {
		return &LCUI_Sys.widget_list;
	}
	return &widget->child;
}

/* ��ȡ�����ľ���������� */
LCUI_API LCUI_RectQueue*
Widget_GetInvalidAreaQueue( LCUI_Widget *widget )
{
	if( widget == NULL ) {
		return LCUIScreen_GetInvalidAreaQueue();
	}
	return &widget->invalid_area;
}

/*-------------------------- Widget Pos ------------------------------*/
LCUI_API int
_Widget_GetX( LCUI_Widget *widget )
/* ͨ������ó�������x������ */
{
	if(widget->x.which_one == 0) {
		return widget->x.px;
	}
	return widget->x.scale * _Widget_GetContainerWidth( widget );
}
LCUI_API int
_Widget_GetY( LCUI_Widget *widget )
/* ͨ������ó�������y������ */
{
	if(widget->x.which_one == 0) {
		return widget->y.px;
	}
	return widget->y.scale * _Widget_GetContainerHeight( widget );
}

static int
_get_x_overlay_widget( LCUI_Queue *list, LCUI_Rect area, LCUI_Queue *out_data )
/* ��ȡ��x����ָ���������ص��Ĳ��� */
{
	int i, n, len;
	LCUI_Widget *wptr;

	if( Queue_GetTotal( out_data ) > 0 ) {
		Queue_Destroy( out_data );
	}
	len = Queue_GetTotal( list );
	for(i=0,n=0; i<len; ++i) {
		wptr = Queue_Get( list, i );
		/* �����ǰ����������area ��x�����ص�������������� */
		if( wptr->pos.x < area.x+area.width
		 && wptr->pos.x >= area.x ) {
			Queue_AddPointer( out_data, wptr );
			++n;
		}
	}
	return n;
}

static LCUI_Widget*
_get_max_y_widget( LCUI_Queue *widget_list )
/* ��ȡ�ױ�y���������Ĳ��� */
{
	int total, i, y, max_y;
	LCUI_Widget *widget, *tmp;

	total = Queue_GetTotal( widget_list );
	widget = Queue_Get( widget_list, 0 );

	for(max_y=0,i=0; i<total; ++i) {
		tmp = Queue_Get( widget_list, i );
		y = tmp->y.px + tmp->size.h;
		if(y > max_y) {
			max_y = y;
			widget = tmp;
		}
	}
	return widget;
}

static void
_move_widget( LCUI_Widget *widget, LCUI_Pos new_pos )
{
	static LCUI_Pos tmp_pos;
	if( widget->pos_type == POS_TYPE_RELATIVE ) {
		/* �����RELATIVE��λ���ͣ���Ҫ����ƫ������ */
		tmp_pos = Pos_Add(new_pos, widget->offset);
		Widget_Move( widget, tmp_pos );
	} else {
		Widget_Move( widget, new_pos );
	}
}

static void
Widget_UpdateChildStaticPos( LCUI_Widget *widget )
/* ����ʹ��static��λ���͵��Ӳ��� */
{
	//_DEBUG_MSG("enter\n");
	int i, x, y, total, j, n;
	LCUI_Queue *queue;
	LCUI_Size container_size;
	LCUI_Pos new_pos;
	LCUI_Widget *wptr, *tmp, *tmp2;
	LCUI_Queue *old_row, *cur_row, *tmp_row, q1, q2, widget_list;
	LCUI_Rect area;

	new_pos = Pos(0,0);
	old_row = &q1;
	cur_row = &q2;
	Queue_Init( old_row, sizeof(LCUI_Widget*), NULL );
	Queue_Init( cur_row, sizeof(LCUI_Widget*), NULL );
	Queue_Init( &widget_list, sizeof(LCUI_Widget*), NULL );
	Queue_UsingPointer( &widget_list );
	Queue_UsingPointer( old_row );
	Queue_UsingPointer( cur_row );

	if( !widget ) {
		queue = &LCUI_Sys.widget_list;
	} else {
		queue = &widget->child;
	}
	container_size = Widget_GetContainerSize( widget );
	//_DEBUG_MSG("container size: %d,%d\n", container_size.w, container_size.h);
	total = Queue_GetTotal( queue );
	//_DEBUG_MSG("queue total: %d\n", total);
	for(i=total-1,y=0,x=0; i>=0; --i) {
		wptr = Queue_Get( queue, i );
		/* ���˵���λ���Ͳ���static��relative�Ĳ��� */
		if(wptr->pos_type != POS_TYPE_STATIC
		&& wptr->pos_type != POS_TYPE_RELATIVE ) {
			continue;
		}
		//_DEBUG_MSG("widget: %p\n", wptr);
		if( new_pos.x == 0 && wptr->size.w > container_size.w ) {
			new_pos.x = x = 0;
			Queue_AddPointer( cur_row, wptr );
			//_DEBUG_MSG("width > container width, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
			_move_widget( wptr, new_pos );
			/* ����y������ */
			new_pos.y += wptr->size.h;
		}
		//_DEBUG_MSG("0, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
		if( y == 0 ) {/* ������ڵ�һ�У���ֱ�Ӽ�¼���� */
			//_DEBUG_MSG("1, %d + %d > %d\n", new_pos.x, wptr->size.w, container_size.w);
			if(new_pos.x + wptr->size.w > container_size.w) {
				/* �������������Χ���Ϳ�ʼ��һ�м�¼ */
				new_pos.x = x = 0;
				Queue_Destroy( old_row );
				tmp_row = old_row;
				old_row = cur_row;
				cur_row = tmp_row;
				tmp = Queue_Get( old_row, x );
				if( tmp ) {
					new_pos.y = tmp->y.px + tmp->size.h;
				}
				++y; ++i;
				/* ���´�ѭ�����ٴδ���ò��� */
				//_DEBUG_MSG("1.1, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
				continue;
			}

			Queue_AddPointer( cur_row, wptr );
			//_DEBUG_MSG("1.2, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
			_move_widget( wptr, new_pos );
			/* ����x������ */
			new_pos.x += wptr->size.w;
			continue;
		}
		//_DEBUG_MSG("2,%d + %d > %d\n", new_pos.x, wptr->size.w, container_size.w);
		/* �����ǰ�������鳬��������Χ��y++����ʼ����һ�м�¼����ָ�롣*/
		if(new_pos.x + wptr->size.w > container_size.w) {
			new_pos.x = x = 0;
			Queue_Destroy( old_row );
			tmp_row = old_row;
			old_row = cur_row;
			cur_row = tmp_row;
			tmp = Queue_Get( old_row, x );
			if( tmp ) {
				new_pos.y = tmp->pos.y + tmp->size.h;
			}
			++y;
		}
		//_DEBUG_MSG("2, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
		/* ��������λ�õĲ��������� */
		area = Rect( new_pos.x, new_pos.y, wptr->size.w, wptr->size.h );
		/* �������һ�м�¼,��ȡ���漸���뵱ǰ������x�����ص��Ĳ����б� */
		n = _get_x_overlay_widget( cur_row, area, &widget_list );
		//printf("n: %d\n", n);
		if( n <= 0 ) { /* �����һ��û����֮�ص��Ĳ��� */
			Queue_AddPointer( cur_row, wptr );
			//_DEBUG_MSG("3,[%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
			_move_widget( wptr, new_pos );
			new_pos.x = wptr->x.px + wptr->size.w;
			continue;
		}
		/* ��ȡ�ױ�y���������Ĳ��� */
		tmp = _get_max_y_widget( &widget_list );
		/* �����һ���еĲ�������ĵױ߳�����ǰ�����һ�� */
		if( tmp->y.px+tmp->size.h > (wptr->y.px+wptr->size.h)/2.0 ) {
			/* ���ò�����ߵĲ���Ҳһͬ��¼����ǰ�� */
			for(j=0; j<n; ++j) {
				tmp2 = Queue_Get( &widget_list, j );
				Queue_AddPointer( cur_row, tmp2 );
				if(tmp2 == tmp) {
					break;
				}
			}
			new_pos.x = tmp->x.px + tmp->size.w;
			continue;
		}
		new_pos.y = tmp->y.px+tmp->size.h;
		Queue_AddPointer( cur_row, wptr );
		//_DEBUG_MSG("4,[%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
		_move_widget( wptr, new_pos );
		new_pos.x = tmp->x.px + wptr->size.w;
	}
	//_DEBUG_MSG("quit\n");
}

LCUI_API LCUI_Pos
_Widget_GetPos(LCUI_Widget *widget)
/* ���ܣ�ͨ������ó�������λ�ã���λΪ���� */
{
	LCUI_Pos pos;
	LCUI_Size size;
	pos.x = pos.y = 0;

	switch(widget->pos_type) {
	    case POS_TYPE_STATIC:
	    case POS_TYPE_RELATIVE:
		pos.x = _Widget_GetX( widget );
		pos.y = _Widget_GetY( widget );
		break;

	    default:
		if(widget->align == ALIGN_NONE) {
			pos.x = _Widget_GetX( widget );
			pos.y = _Widget_GetY( widget );
			break;
		}
		size = Widget_GetContainerSize( widget->parent );
		pos = Align_Get_Pos(size, Widget_GetSize(widget), widget->align);
		/* ����ƫ�ƾ��� */
		pos = Pos_Add(pos, widget->offset);
		break;
	}
	return pos;
}

LCUI_API LCUI_Pos
Widget_GetPos(LCUI_Widget *widget)
/*
 * ���ܣ���ȡ�������������������λ��
 * ˵������λ��������������������Ͻǵ㣬���������������ڱ߾ࡣ
 *  */
{
	return widget->pos;
}

LCUI_API LCUI_Pos
Widget_GetRelPos(LCUI_Widget *widget)
/*
 * ���ܣ���ȡ������������������������λ��
 * ˵�������������������򲢲�һ����������������������Ϊ���������С�ܵ�
 * �����������ڱ߾��Ӱ�졣
 *  */
{
	return Pos(widget->x.px, widget->x.px);
}

LCUI_API LCUI_Pos
Widget_ToRelPos(LCUI_Widget *widget, LCUI_Pos global_pos)
/*
 * ���ܣ�ȫ������ת�����������
 * ˵���������ȫ�����꣬�����ݴ���Ĳ���ָ�룬ת��������ڸò���������
 *  */
{
	if( widget == NULL || widget->parent == NULL) {
		return global_pos;
	}
	//widget = widget->parent;
	while( widget ) {
		global_pos.x -= widget->padding.left;
		global_pos.y -= widget->padding.top;
		global_pos.x -= widget->pos.x;
		global_pos.y -= widget->pos.y;
		widget = widget->parent;
	}
	return global_pos;
}

LCUI_API int
_Widget_GetMaxX( LCUI_Widget *widget )
{
	if(widget->max_x.which_one == 0) {
		return widget->max_x.px;
	}
	return widget->max_x.scale * _Widget_GetContainerWidth( widget );
}

LCUI_API int
_Widget_GetMaxY( LCUI_Widget *widget )
{
	if(widget->max_y.which_one == 0) {
		return widget->max_y.px;
	}
	return widget->max_y.scale * _Widget_GetContainerHeight( widget );
}

LCUI_API LCUI_Pos
_Widget_GetMaxPos( LCUI_Widget *widget )
{
	LCUI_Pos pos;
	pos.x = _Widget_GetMaxX( widget );
	pos.y = _Widget_GetMaxY( widget );
	return pos;
}

LCUI_API LCUI_Pos
Widget_GetMaxPos(LCUI_Widget *widget)
{
	return widget->max_pos;
}

LCUI_API int
_Widget_GetMinX( LCUI_Widget *widget )
{
	if(widget->min_x.which_one == 0) {
		return widget->min_x.px;
	}
	return widget->min_x.scale * _Widget_GetContainerWidth( widget );
}

LCUI_API int
_Widget_GetMinY( LCUI_Widget *widget )
{
	if(widget->min_y.which_one == 0) {
		return widget->min_y.px;
	}
	return widget->min_y.scale * _Widget_GetContainerHeight( widget );
}

LCUI_API LCUI_Pos
_Widget_GetMinPos( LCUI_Widget *widget )
{
	LCUI_Pos pos;
	pos.x = _Widget_GetMinX( widget );
	pos.y = _Widget_GetMinY( widget );
	return pos;
}

LCUI_API LCUI_Pos
Widget_GetMinPos(LCUI_Widget *widget)
{
	return widget->min_pos;
}
/*------------------------ END Widget Pos -----------------------------*/

/*------------------------- Widget Size -------------------------------*/
LCUI_API int
_Widget_GetMaxWidth( LCUI_Widget *widget )
{
	if(widget->max_w.which_one == 0) {
		return widget->max_w.px;
	}
	return widget->max_w.scale * _Widget_GetContainerWidth( widget );
}

LCUI_API int
_Widget_GetMaxHeight( LCUI_Widget *widget )
{
	if(widget->max_h.which_one == 0) {
		return widget->max_h.px;
	}
	return widget->max_h.scale * _Widget_GetContainerHeight( widget );
}

LCUI_API LCUI_Size
_Widget_GetMaxSize( LCUI_Widget *widget )
{
	LCUI_Size size;
	size.w = _Widget_GetMaxWidth( widget );
	size.h = _Widget_GetMaxHeight( widget );
	return size;
}

LCUI_API LCUI_Size
Widget_GetMaxSize( LCUI_Widget *widget )
{
	return widget->max_size;
}

LCUI_API int
_Widget_GetMinWidth( LCUI_Widget *widget )
{
	if(widget->min_w.which_one == 0) {
		return widget->min_w.px;
	}
	return widget->min_w.scale * _Widget_GetContainerWidth( widget );
}

LCUI_API int
_Widget_GetMinHeight( LCUI_Widget *widget )
{
	if(widget->min_h.which_one == 0) {
		return widget->min_h.px;
	}
	return widget->min_h.scale * _Widget_GetContainerHeight( widget );
}

LCUI_API int
_Widget_GetHeight(LCUI_Widget *widget)
{
	if(widget->h.which_one == 0) {
		return widget->h.px;
	}
	return widget->h.scale * _Widget_GetContainerHeight( widget );
}

LCUI_API int
_Widget_GetWidth(LCUI_Widget *widget)
{
	if(widget->w.which_one == 0) {
		return widget->w.px;
	}
	return widget->w.scale * _Widget_GetContainerWidth( widget );
}

LCUI_API int
Widget_GetHeight(LCUI_Widget *widget)
{
	return widget->size.h;
}

LCUI_API int
Widget_GetWidth(LCUI_Widget *widget)
{
	return widget->size.w;
}

LCUI_API LCUI_Size
_Widget_GetMinSize( LCUI_Widget *widget )
{
	LCUI_Size size;
	size.w = _Widget_GetMinWidth( widget );
	size.h = _Widget_GetMinHeight( widget );
	return size;
}

LCUI_API LCUI_Size
Widget_GetMinSize(LCUI_Widget *widget)
{
	return widget->min_size;
}

LCUI_API LCUI_Size
Widget_GetSize(LCUI_Widget *widget)
/* ���ܣ���ȡ�����ĳߴ� */
{
	return widget->size;
}

LCUI_API LCUI_Size
_Widget_GetSize(LCUI_Widget *widget)
/* ���ܣ�ͨ�������ȡ�����ĳߴ� */
{
	LCUI_Size size;
	size.w = _Widget_GetWidth( widget );
	size.h = _Widget_GetHeight( widget );
	return size;
}

LCUI_API LCUI_Rect
Widget_GetRect(LCUI_Widget *widget)
/* ���ܣ���ȡ���������� */
{
	return Rect(widget->pos.x, widget->pos.y,
			widget->size.w, widget->size.h);
}

/*------------------------ END Widget Size ----------------------------*/

LCUI_API void*
Widget_GetPrivData(LCUI_Widget *widget)
/* ���ܣ���ȡ������˽�����ݽṹ���ָ�� */
{
	if( !widget ) {
		return NULL;
	}
	return widget->private_data;
}

LCUI_API LCUI_Widget*
Get_Widget_Parent(LCUI_Widget *widget)
/* ���ܣ���ȡ�����ĸ����� */
{
	return widget->parent;
}

LCUI_API int
Widget_PrintChildList( LCUI_Widget *widget )
{
	int i, n;
	LCUI_Queue *child_list;
	LCUI_Widget *child;
	child_list = Widget_GetChildList( widget );
	if(child_list == NULL) {
		return -1;
	}
	n = Queue_GetTotal( child_list );
	for(i=0; i<n; ++i) {
		child = (LCUI_Widget*)Queue_Get( child_list, i );
		if( child == NULL ) {
			continue;
		}
		printf("[%d] widget: %p, type: %s, z-index: %d, pos: (%d,%d), size: (%d, %d)\n",
			i, child, child->type_name.string, child->main_glayer->z_index,
			child->pos.x, child->pos.y,
			child->size.w, child->size.h);
	}
	return 0;
}

LCUI_API void
print_widget_info(LCUI_Widget *widget)
/*
 * ���ܣ���ӡwidget����Ϣ
 * ˵�����ڵ���ʱ��Ҫ�õ���������ȷ��widget�Ƿ�������
 *  */
{
	int i, n;
	LCUI_Widget *child;
	LCUI_Queue *child_list;
	if( widget ) {
		if( widget->parent ) {
			child_list = &widget->child;
		} else {
			child_list = &LCUI_Sys.widget_list;
		}
		n = Queue_GetTotal( child_list );
		for(i=0; i<n; ++i) {
			child = Queue_Get( child_list, i );
			if( child == widget ) {
				break;
			}
		}
		printf("widget: %p, type: %s, visible: %d, show pos: %d, pos: (%d,%d), size: (%d, %d)\n",
			widget, widget->type_name.string, widget->visible,
			i, widget->pos.x, widget->pos.y,
			widget->size.w, widget->size.h);
	} else {
		printf("NULL widget\n");
	}
}

/* ��ָ���������ڲ��������趨��Ҫˢ�µ����� */
LCUI_API int
Widget_InvalidArea ( LCUI_Widget *widget, LCUI_Rect rect )
{
	if( !widget ) {
		return LCUIScreen_InvalidArea( rect );
	}
	if (rect.width <= 0 || rect.height <= 0) {
		return -1;
	}
	/* �����ڱ߾� */
	rect.x += widget->padding.left;
	rect.y += widget->padding.top;
	/* ���ݲ������������ĳߴ磬��������λ�ü��ߴ� */
	rect = LCUIRect_ValidArea( Widget_GetSize(widget), rect );
	if( widget->visible ) {
		LCUI_Sys.need_sync_area = TRUE;
	}
	//if( widget->type_name.string && strcmp(widget->type_name.string, "text_box") == 0 ) {
	//	_DEBUG_MSG("add rect: %d,%d,%d,%d\n",
	//		rect.x, rect.y, rect.width, rect.height );
	//}

	/* ������������ */
	RectQueue_AddToValid( &widget->invalid_area, rect );
	return 0;
}

/*
 * global_pos: ��ǰ������ȫ������
 * valid_area: ��ǰ������������Ч��ʾ����
 * */
static void
__Widget_SyncInvalidArea(	LCUI_Widget *widget,
				LCUI_Pos global_pos,
				LCUI_Rect valid_area )
{
	int n;
	LCUI_Pos point;
	LCUI_Queue *widget_list;
	LCUI_Widget *child;
	LCUI_Rect rect;
	LCUI_RectQueue *rect_queue;

	rect_queue = Widget_GetInvalidAreaQueue( widget );
	point.x = valid_area.x + valid_area.width;
	point.y = valid_area.y + valid_area.height;
	RectQueue_Switch( rect_queue );
	while( RectQueue_GetFromCurrent( rect_queue, &rect ) ) {
		/* ���ϵ�ǰ������ȫ������ */
		rect.x += global_pos.x;
		rect.y += global_pos.y;
		/* ���ݵ�ǰ��Ч���򣬶�ȡ����������е��� */
		if( rect.x < valid_area.x ) {
			rect.width -= (valid_area.x - rect.x);
			rect.x = valid_area.x;
		}
		if( rect.y < valid_area.y ) {
			rect.height -= (valid_area.y - rect.y);
			rect.y = valid_area.y;
		}
		if( rect.x + rect.width > point.x ) {
			rect.width = point.x - rect.x;
		}
		if( rect.y + rect.height > point.y ) {
			rect.height = point.y - rect.y;
		}
		/* �������Ļ��Ч������� */
		LCUIScreen_InvalidArea( rect );
	}
	/* ���ݵ�ǰ���������򣬼�����Ч��ʾ���� */
	if( global_pos.x > valid_area.x ) {
		valid_area.width -= (global_pos.x - valid_area.x);
		valid_area.x = global_pos.x;
	}
	if( global_pos.y > valid_area.y ) {
		valid_area.height -= (global_pos.y - valid_area.y);
		valid_area.y = global_pos.y;
	}
	if( global_pos.x + widget->size.w < point.x ) {
		valid_area.width = global_pos.x + widget->size.w - valid_area.x;
	}
	if( global_pos.y + widget->size.h < point.y ) {
		valid_area.height = global_pos.y + widget->size.h - valid_area.y;
	}
	widget_list = Widget_GetChildList( widget );
	n = Queue_GetTotal( widget_list );
	while(n--) {
		child = (LCUI_Widget*)Queue_Get( widget_list, n );
		if( !child || !child->visible ) {
			continue;
		}
		point.x = global_pos.x + child->pos.x;
		point.y = global_pos.y + child->pos.y;
		point.x += widget->padding.left;
		point.y += widget->padding.top;
		__Widget_SyncInvalidArea( child, point, valid_area );
	}
}

/* �����пɼ���������Ч���� ͬ���� ��Ļ��Ч��������� */
LCUI_API void
Widget_SyncInvalidArea( void )
{
	int n;
	LCUI_Widget *child;
	LCUI_Rect valid_area;
	LCUI_Queue *widget_list;

	widget_list = Widget_GetChildList( NULL );
	valid_area.x = valid_area.y = 0;
	valid_area.width = LCUIScreen_GetWidth();
	valid_area.height = LCUIScreen_GetHeight();
	n = Queue_GetTotal( widget_list );
	while(n--) {
		child = (LCUI_Widget*)Queue_Get( widget_list, n );
		if( !child || !child->visible ) {
			continue;
		}
		__Widget_SyncInvalidArea( child, child->pos, valid_area );
	}
}


/*
 * ���ܣ���ָ��������Ӧ����״̬�ĸı�
 * ˵������������ʱ��Ĭ���ǲ���Ӧ״̬�ı�ģ���Ϊÿ��״̬�ı�󣬶�Ҫ���ú����ػ沿����
 * �����һЩ�����Ƕ���ģ�û��Ҫ�ػ棬Ӱ��Ч�ʡ�������ò�������ť����������ƶ�����
 * ����ʱ�Լ��������ʱ������ı䰴ť��ͼ����ʽ���Ǿ���Ҫ�������������һ�¡�
 * �÷���
 * Widget_SetValidState( widget, WIDGET_STATE_NORMAL );
 * Widget_SetValidState( widget, WIDGET_STATE_OVERLAY | WIDGET_STATE_ACTIVE );
 * */
LCUI_API void
Widget_SetValidState( LCUI_Widget *widget, int state )
{
	widget->valid_state = state;
}

/*
 * ���ܣ���ȡ������ָ�����͵ĸ�������ָ��
 * ˵�������������ڲ�����ϵ������ͷ�����Ҹ�����ָ�룬���ж�����������Ƿ�Ϊ�ƶ�����
 * ����ֵ��û�з���Ҫ��ĸ��������ͷ���NULL�����򷵻ز���ָ��
 **/
LCUI_API LCUI_Widget*
Widget_GetParent(LCUI_Widget *widget, char *widget_type)
{
	LCUI_Widget *temp;
	if( !widget ) {
		return NULL; /* ������NULL���˳����� */
	}
	temp = widget;
	while( temp->parent ) {
		if(temp->parent
		  && _LCUIString_Cmp(&temp->parent->type_name, widget_type) == 0
		) {/* ���ָ����Ч���������ͷ���Ҫ�� */
			return temp->parent; /* ���ز�����ָ�� */
		}
		temp = temp->parent;/* ��ȡ�����ĸ�������ָ�� */
	}
	return NULL;
}

/* ��ȡָ��ID���Ӳ��� */
LCUI_API LCUI_Widget*
Widget_GetChildByID( LCUI_Widget *widget, LCUI_ID id )
{
	int i, n;
	LCUI_Widget *child;
	LCUI_Queue *child_list;

	if( widget ) {
		child_list = &widget->child;
	} else {
		child_list = &LCUI_Sys.widget_list;
	}
	n = Queue_GetTotal( child_list );
	for(i=0; i<n; ++i) {
		child = Queue_Get( child_list, i );
		if( !child ) {
			continue;
		}
		if( child->self_id == id ) {
			return child;
		}
	}
	return NULL;
}

/* ����ָ��ID�ĳ�������в��� */
LCUI_API void
LCUIApp_DestroyAllWidgets( LCUI_ID app_id )
{
	int i, total;
	LCUI_Widget *temp;

	total = Queue_GetTotal(&LCUI_Sys.widget_list);
	for(i=0; i<total; i++) {
		temp = Queue_Get(&LCUI_Sys.widget_list,i);
		if(temp->app_id != app_id) {
			continue;
		}
		/*
		 * ��Queue_Delete()�����������еĲ����Ƴ�ʱ������ó�ʼ����������ʱָ
		 * ����Destroy_Widget()�������в���������ص�����
		 * */
		Queue_Delete(&LCUI_Sys.widget_list, i);
		/* ���»�ȡ�������� */
		total = Queue_GetTotal(&LCUI_Sys.widget_list);
		--i;/* ��ǰλ�õĲ����Ѿ��Ƴ�����λ���ɺ��沿��������ԣ�--i */
	}
}


/* ���ָ�������Ƿ��ڽ���״̬ */
LCUI_API LCUI_BOOL
Widget_GetFocus( LCUI_Widget *widget )
{
	LCUI_Widget *tmp;

	if( !widget ) {
		return FALSE;
	}
	tmp = Get_FocusWidget( widget->parent );
	if( tmp == widget ) {
		return TRUE;
	}
	return FALSE;
}

/* ���ָ�������Ƿ���ȫ�ֽ��㣬Ҳ���Ǹò����Լ��ϼ����и������Ƿ񶼵õ��˽��� */
LCUI_API LCUI_BOOL
Widget_GetGlobalFocus( LCUI_Widget *widget )
{
	if( Widget_GetFocus(widget) ) {
		return Widget_GetFocus( widget->parent );
	}
	return FALSE;
}

/* ��ȡ�����ķ������ */
LCUI_API LCUI_String
Widget_GetStyleName( LCUI_Widget *widget )
{
	return widget->style_name;
}

/* �趨�����ķ������ */
LCUI_API void
Widget_SetStyleName( LCUI_Widget *widget, const char *style_name )
{
	_LCUIString_Copy(&widget->style_name, style_name);
	Widget_Draw( widget );
}

/* �趨�����ķ��ID */
LCUI_API void
Widget_SetStyleID( LCUI_Widget *widget, int style_id )
{
	widget->style_id = style_id;
	Widget_Draw( widget );
}

static LCUI_Widget *
__Widget_At( LCUI_Widget *ctnr, LCUI_Pos pos )
{
	int i, total, temp;
	LCUI_Widget *child, *widget;
	LCUI_Queue *widget_list;
	LCUI_Pos tmp_pos;
	LCUI_RGBA pixel;
	LCUI_Graph *graph;

	if( ctnr ) {
		widget_list = &ctnr->child;
		/* �ж� ��������Ӧ����ͼ���е����ص��͸���� �Ƿ����Ҫ��
		 * ���͸����С��/��С��clickable_area_alpha��ֵ����ô������
		 * �ò�����
		 *  */
		graph = Widget_GetSelfGraph( ctnr );
		if( Graph_GetPixel( graph, pos, &pixel )) {
			//printf("mode: %d, pixel alpha: %d, alpha: %d\n",
			//widget->clickable_mode, pixel.alpha, widget->clickable_area_alpha );
			if( (ctnr->clickable_mode == 0
			 && pixel.alpha < ctnr->clickable_area_alpha )
			 || (ctnr->clickable_mode == 1
			 && pixel.alpha >= ctnr->clickable_area_alpha ) ) {
				//printf("Ignore widget\n");
				return NULL;
			}
		}/* else {
			printf("get graph pixel error\n");
		} */
		/* ��ȥ�ڱ߾� */
		pos.x -= ctnr->padding.left;
		pos.y -= ctnr->padding.top;
	} else {
		widget_list = &LCUI_Sys.widget_list;
	}
	widget = ctnr;
	total = Queue_GetTotal( widget_list );
	for(i=0; i<total; ++i) {/* �Ӷ����ױ����Ӳ��� */
		child = Queue_Get( widget_list, i );
		if( !child || !child->visible ) {
			continue;
		}
		temp = LCUIRect_IncludePoint( pos, Widget_GetRect(child) );
		/* ��������û�������ڲ��������� */
		if( !temp ) {
			continue;
		}
		/* �ı�������� */
		tmp_pos.x = pos.x - child->pos.x;
		tmp_pos.y = pos.y - child->pos.y;
		widget = __Widget_At( child, tmp_pos );
		if( widget == NULL ) {
			widget = ctnr;
		}
		break;
	}
	return widget;
}

/* ��ȡָ�������ϵ��Ӳ��������򷵻��Ӳ���ָ�룬���򷵻�NULL */
LCUI_API LCUI_Widget*
Widget_At( LCUI_Widget *ctnr, LCUI_Pos pos )
{
	LCUI_Widget *widget;
	widget = __Widget_At( ctnr, pos );
	if( widget == ctnr ) {
		return NULL;
	}
	return widget;

}

LCUI_API int
Widget_IsActive(LCUI_Widget *widget)
/* ���ܣ��жϲ����Ƿ�Ϊ�״̬ */
{
	if(widget->state != KILLED) {
		return 1;
	}
	return 0;
}

LCUI_API int
Empty_Widget(void)
/*
 * ���ܣ����ڼ�����Ĳ����б��Ƿ�Ϊ��
 * ����ֵ��
 *   1  ����Ĳ����б�Ϊ��
 *   0  ����Ĳ����б�Ϊ��
 * */
{
	if(Queue_GetTotal(&LCUI_Sys.widget_list) <= 0) {
		return 1;
	}
	return 0;
}

/* ���ܣ�Ϊ����˽�нṹ��ָ������ڴ� */
LCUI_API void*
WidgetPrivData_New( LCUI_Widget *widget, size_t size )
{
	widget->private_data = malloc(size);
	return widget->private_data;
}

static void Widget_BackgroundInit( LCUI_Widget *widget )
{
	Graph_Init( &widget->background.image );
	widget->background.color = RGB(255,255,255);
	widget->background.transparent = TRUE;
	widget->background.layout = LAYOUT_NONE;
}


static void
Destroy_Widget( void *arg )
/*
 * ���ܣ�����һ������
 * ˵�����������������Ӳ�������������������
 * */
{
	LCUI_Widget *widget;
	widget = (LCUI_Widget *)arg;
	widget->parent = NULL;

	/* �ͷ��ַ��� */
	LCUIString_Free(&widget->type_name);
	LCUIString_Free(&widget->style_name);
	GraphLayer_Free( widget->main_glayer );
	GraphLayer_Free( widget->client_glayer );

	/* ���ٲ����Ķ��� */
	Queue_Destroy(&widget->child);
	Queue_Destroy(&widget->event);
	Queue_Destroy(&widget->update_buff);
	RectQueue_Destroy(&widget->invalid_area);

	widget->visible = FALSE;
	widget->enabled = TRUE;
	/* ���ûص��������ٲ���˽������ */
	WidgetFunc_Call( widget, FUNC_TYPE_DESTROY );
	free( widget->private_data );
}

/* ��ʼ���������� */
LCUI_API void
WidgetQueue_Init(LCUI_Queue *queue)
{
	Queue_Init(queue, sizeof(LCUI_Widget), Destroy_Widget);
}


/*
 * ���ܣ�����ָ�����͵Ĳ���
 * ����ֵ���ɹ��򲿼���ָ�룬ʧ���򷵻�NULL
 */
LCUI_API LCUI_Widget*
Widget_New( const char *widget_type )
{
	LCUI_Widget *widget;
	LCUI_App *app;

	if( !LCUI_Active() ) {
		return NULL;
	}
	widget = (LCUI_Widget *)malloc( sizeof(LCUI_Widget) );
	if( !widget ) {
		char str[256];
		sprintf( str, "%s ()", __FUNCTION__ );
		perror(str);
		return NULL;
	}
	app = LCUIApp_GetSelf();
	/*--------------- ��ʼ�������������Լ����� ------------------*/
	widget->auto_size		= FALSE;
	widget->auto_size_mode		= AUTOSIZE_MODE_GROW_AND_SHRINK;
	widget->type_id			= 0;
	widget->modal			= FALSE;
	widget->state			= WIDGET_STATE_NORMAL;
	widget->self_id			= 0;
	widget->app_id			= app->id;
	widget->parent			= NULL;
	widget->enabled			= TRUE;
	widget->visible			= FALSE;
	widget->focus			= TRUE;
	widget->focus_widget		= NULL;
	widget->pos			= Pos(0, 0);
	widget->size			= Size(0, 0);
	widget->min_size		= Size(0, 0);
	widget->max_size		= Size(INT_MAX, INT_MAX);
	widget->align			= ALIGN_NONE;
	widget->dock			= DOCK_TYPE_NONE;
	widget->offset			= Pos(0, 0);
	widget->pos_type		= POS_TYPE_ABSOLUTE;
	widget->color			= RGB(0,0,0);
	widget->private_data		= NULL;
	widget->valid_state		= 0;
	widget->clickable_mode		= 0;
	widget->clickable_area_alpha	= 0;
	/*------------------------- END --------------------------*/

	/*--------------- ��ʼ�������ĸ������� ------------------*/
	PX_P_t_init( &widget->x );
	PX_P_t_init( &widget->y );
	PX_P_t_init( &widget->max_x );
	PX_P_t_init( &widget->max_y );
	PX_P_t_init( &widget->min_x );
	PX_P_t_init( &widget->min_y );
	widget->max_x.px = INT_MAX;
	widget->max_y.px = INT_MAX;
	widget->min_x.px = INT_MIN;
	widget->min_y.px = INT_MIN;
	PX_P_t_init( &widget->w );
	PX_P_t_init( &widget->h );
	PX_P_t_init( &widget->max_w );
	PX_P_t_init( &widget->max_h );
	PX_P_t_init( &widget->min_w );
	PX_P_t_init( &widget->min_h );
	/*---------------------- END -----------------------*/

	/*------------- ����ָ���ʼ�� ------------------*/
	widget->set_align = Widget_SetAlign;
	widget->set_alpha = Widget_SetAlpha;
	widget->set_border = Widget_SetBorder;
	widget->show = Widget_Show;
	widget->hide = Widget_Hide;
	widget->resize = Widget_Resize;
	widget->move = Widget_Move;
	widget->enable = Widget_Enable;
	widget->disable = Widget_Disable;
	/*----------------- END -----------------------*/

	/* ��ʼ���߿����� */
	Border_Init( &widget->border );
	Padding_Init( &widget->padding );
	Margin_Init( &widget->margin );

	Widget_BackgroundInit( widget );

	/* ��������ͼ�� */
	widget->main_glayer = GraphLayer_New();
	widget->client_glayer = GraphLayer_New();
	/* �ͻ���ͼ����Ϊ��ͼ�����ͼ�� */
	GraphLayer_AddChild( widget->main_glayer, widget->client_glayer );
	/* ��ͼ����Ϊ��ͼ�����ͼ�� */
	GraphLayer_AddChild( LCUI_Sys.root_glayer, widget->main_glayer );
	/* �̳���ͼ���͸���� */
	GraphLayer_InerntAlpha( widget->client_glayer, TRUE );
	/* �趨ͼ������ */
	widget->main_glayer->graph.have_alpha = TRUE;
	widget->client_glayer->graph.have_alpha = TRUE;
	//widget->client_glayer->graph.is_opaque = FALSE;
	//widget->client_glayer->graph.not_visible = TRUE;
	/* ��ʾͼ�� */
	GraphLayer_Show( widget->client_glayer );

	RectQueue_Init( &widget->invalid_area ); /* ��ʼ����Ч�����¼ */
	EventSlots_Init( &widget->event );	/* ��ʼ���������¼����ݶ��� */
	WidgetQueue_Init( &widget->child );	/* ��ʼ���Ӳ����� */
	WidgetUpdateBuff_Init( &widget->update_buff );	/* ��ʼ�����ݸ��¶��� */
	LCUIString_Init( &widget->type_name );
	LCUIString_Init( &widget->style_name );

	/* ��󣬽��ò���������������������� */
	Queue_AddPointer( &LCUI_Sys.widget_list, widget );
	if( !widget_type ) {
		return widget;
	}
	/* ��֤���������Ƿ���Ч */
	if( !WidgetType_Valid(widget_type) ) {
		puts(WIDGET_ERROR_TYPE_NOT_FOUND);
		return NULL;
	}
	/* ���沿������ */
	_LCUIString_Copy( &widget->type_name, widget_type );
	widget->type_id = WidgetType_GetID( widget_type );
	/* ���ò����Ļص��������Բ���˽�����ݽ��г�ʼ�� */
	WidgetFunc_Call( widget, FUNC_TYPE_INIT );
	return widget;
}

/* ���ٲ��� */
static void Widget_ExecDestroy( LCUI_Widget *widget )
{
	int i, total;
	LCUI_Queue *child_list;
	LCUI_Widget *tmp;

	if( !widget ) {
		return;
	}
	if( !widget->parent ) {
		child_list = &LCUI_Sys.widget_list;
	} else {
		child_list = &widget->parent->child;
	}
	Queue_Lock( child_list );
	total = Queue_GetTotal(child_list);
	for(i=0; i<total; ++i) {
		tmp = Queue_Get(child_list, i);
		if(tmp == widget) {
			Queue_Delete(child_list, i);
			break;
		}
	}
	Queue_Unlock( child_list );
}

/* �ۼƲ�����λ������ */
static LCUI_Pos Widget_CountPos( LCUI_Widget *widget )
{
	LCUI_Pos pos;

	if( !widget->parent ) {
		return widget->pos;
	}
	pos = Widget_CountPos(widget->parent);
	pos.x += widget->parent->padding.left;
	pos.y += widget->parent->padding.top;
	pos = Pos_Add(pos, widget->pos);
	return pos;
}

/* ��ȡ������ȫ������ */
LCUI_API LCUI_Pos
Widget_GetGlobalPos(LCUI_Widget *widget)
{
	return Widget_CountPos(widget);
}

LCUI_API void
Set_Widget_ClickableAlpha( LCUI_Widget *widget, uchar_t alpha, int mode )
/* �趨�����ɱ�����������͸���� */
{
	if( mode == 0 ) {
		widget->clickable_mode = 0;
	} else {
		widget->clickable_mode = 1;
	}
	//printf("set, mode: %d, alpha: %d\n", widget->clickable_mode, alpha);
	widget->clickable_area_alpha = alpha;
}

LCUI_API void
Widget_SetAlign(LCUI_Widget *widget, ALIGN_TYPE align, LCUI_Pos offset)
/* ���ܣ��趨�����Ķ��뷽ʽ�Լ�ƫ�ƾ��� */
{
	if( !widget ) {
		return;
	}
	widget->align = align;
	widget->offset = offset;
	Widget_UpdatePos(widget);/* ����λ�� */
}

LCUI_API int
Widget_SetMaxSize( LCUI_Widget *widget, char *width, char *height )
/*
 * ���ܣ��趨���������ߴ�
 * ˵������ֵΪ0ʱ�������ĳߴ粻�����ƣ��÷�ʾ���ɲο�Set_Widget_Size()����
 * */
{
	int n;
	n = get_PX_P_t( width, &widget->max_w );
	n += get_PX_P_t( width, &widget->max_h );
	return n;
}

LCUI_API int
Widget_SetMinSize( LCUI_Widget *widget, char *width, char *height )
/*
 * ���ܣ��趨��������С�ߴ�
 * ˵�����÷�ʾ���ɲο�Set_Widget_Size()����
 * */
{
	int n;
	n = get_PX_P_t( width, &widget->min_w );
	n += get_PX_P_t( width, &widget->min_h );
	return n;
}

LCUI_API void
Widget_LimitSize(LCUI_Widget *widget, LCUI_Size min_size, LCUI_Size max_size)
/* ���ܣ����Ʋ����ĳߴ�䶯��Χ */
{
	if(min_size.w < 0) {
		min_size.w = 0;
	}
	if(min_size.h < 0) {
		min_size.h = 0;
	}
	if(max_size.w < 0) {
		max_size.w = 0;
	}
	if(max_size.h < 0) {
		max_size.h = 0;
	}
	if(max_size.w < min_size.w) {
		max_size.w = min_size.w;
	}
	if(max_size.h < min_size.h) {
		max_size.h = min_size.h;
	}

	widget->min_w.px = min_size.w;
	widget->min_h.px = min_size.h;
	widget->min_w.which_one = 0;
	widget->min_h.which_one = 0;
	widget->max_w.px = max_size.w;
	widget->max_h.px = max_size.h;
	widget->max_w.which_one = 0;
	widget->max_h.which_one = 0;
	widget->min_size = min_size;
	widget->max_size = max_size;
}

LCUI_API void
Widget_LimitPos(LCUI_Widget *widget, LCUI_Pos min_pos, LCUI_Pos max_pos)
/* ���ܣ����Ʋ������ƶ���Χ */
{
	if(min_pos.x < 0) {
		min_pos.x = 0;
	}
	if(min_pos.y < 0) {
		min_pos.y = 0;
	}
	if(max_pos.x < 0) {
		max_pos.x = 0;
	}
	if(max_pos.y < 0) {
		max_pos.y = 0;
	}
	if(max_pos.x < min_pos.x) {
		max_pos.x = min_pos.x;
	}
	if(max_pos.y < min_pos.y) {
		max_pos.y = min_pos.y;
	}

	widget->min_x.px = min_pos.x;
	widget->min_y.px = min_pos.y;
	widget->min_x.which_one = 0;
	widget->min_x.which_one = 0;
	widget->max_x.px = max_pos.x;
	widget->max_y.px = max_pos.y;
	widget->max_x.which_one = 0;
	widget->max_y.which_one = 0;
	Widget_UpdatePos( widget );
}

LCUI_API void
_Limit_Widget_Pos( LCUI_Widget *widget, char *x_str, char*y_str )
{

}

LCUI_API void
_Limit_Widget_Size( LCUI_Widget *widget, char *w_str, char*h_str )
{

}

/* �趨�����ı߿� */
LCUI_API void
Widget_SetBorder( LCUI_Widget *widget, LCUI_Border border )
{
	widget->border = border;
	Widget_Draw( widget );
	Widget_InvalidArea( widget->parent, Widget_GetRect(widget) );
}

/* �趨�����߿���ĸ��ǵ�Բ�ǰ뾶 */
LCUI_API void
Widget_SetBorderRadius( LCUI_Widget *widget, unsigned int radius )
{
	Border_Radius( &widget->border, radius );
	Widget_Draw( widget );
	Widget_InvalidArea( widget->parent, Widget_GetRect(widget) );
}

/* �趨�����ı���ͼ�� */
LCUI_API void
Widget_SetBackgroundImage( LCUI_Widget *widget, LCUI_Graph *img )
{
	if(!widget) {
		return;
	}
	if( !Graph_IsValid(img) ) {
		Graph_Init( &widget->background.image );
		return;
	}
	widget->background.image = *img;
}

/* �趨����ͼ�Ĳ��� */
LCUI_API void
Widget_SetBackgroundLayout( LCUI_Widget *widget, LAYOUT_TYPE layout )
{
	if(!widget) {
		return;
	}
	widget->background.layout = layout;
}

/* �趨�����ı�����ɫ */
LCUI_API void
Widget_SetBackgroundColor( LCUI_Widget *widget, LCUI_RGB color )
{
	if(!widget) {
		return;
	}
	widget->background.color = color;
}

/* �趨���������Ƿ�͸�� */
LCUI_API void
Widget_SetBackgroundTransparent( LCUI_Widget *widget, LCUI_BOOL flag )
{
	if(!widget) {
		return;
	}
	widget->background.transparent = flag;
}

LCUI_API void
Widget_Enable(LCUI_Widget *widget)
/* ���ܣ����ò��� */
{
	widget->enabled = TRUE;
	Widget_SetState(widget, WIDGET_STATE_NORMAL);
}

LCUI_API void
Widget_Disable(LCUI_Widget *widget)
/* ���ܣ����ò��� */
{
	widget->enabled = FALSE;
	Widget_SetState( widget, WIDGET_STATE_DISABLE );
}

/* ָ�������Ƿ�ɼ� */
LCUI_API void
Widget_Visible( LCUI_Widget *widget, LCUI_BOOL flag )
{
	widget->visible = flag;
	if( flag ) {
		GraphLayer_Show( widget->main_glayer );
	} else {
		GraphLayer_Hide( widget->main_glayer );
	}
}

LCUI_API void
Widget_SetPos(LCUI_Widget *widget, LCUI_Pos pos)
/*
 * ���ܣ��趨������λ��
 * ˵����ֻ�޸����꣬�����оֲ�ˢ��
 * */
{
	widget->pos = pos;
}

LCUI_API void
Widget_SetPadding( LCUI_Widget *widget, LCUI_Padding padding )
/* ���ò������ڱ߾� */
{
	LCUI_Size size;
	widget->padding = padding;
	/* �����趨���ڱ߾࣬����clientͼ��ĳߴ缰λ�� */
	GraphLayer_SetPos( widget->client_glayer,
			widget->padding.left, widget->padding.top );
	size = widget->size;
	size.w -= (widget->padding.left + widget->padding.right);
	size.h -= (widget->padding.top + widget->padding.bottom);
	GraphLayer_Resize( widget->client_glayer, size.w, size.h );
	/* �����Ӳ�����λ�� */
	Widget_UpdateChildPos( widget );
}

LCUI_API void
Widget_SetMargin( LCUI_Widget *widget, LCUI_Margin margin )
/* ���ò�������߾� */
{
	widget->margin = margin;
	Widget_UpdateChildPos( widget->parent );
}

LCUI_API void
Widget_SetPosType( LCUI_Widget *widget, POS_TYPE pos_type )
/* �趨�����Ķ�λ���� */
{
	widget->pos_type = pos_type;
	Record_WidgetUpdate( widget, NULL, DATATYPE_POS_TYPE, 0 );
}

/* ���ò����Ķѵ�˳�� */
LCUI_API int
Widget_SetZIndex( LCUI_Widget *widget, int z_index )
{
	LCUI_GraphLayer *glayer;
	glayer = Widget_GetMainGraphLayer(widget);
	if( glayer == NULL ) {
		return -1;
	}
	/* ����ģ̬�������ģ̬������z_indexֵ */
	if( !widget->modal ) {
		if( z_index > 9999 ) {
			z_index = 9999;
		}
	} else {
		if( z_index < 10000 ) {
			z_index = 10000;
		}
	}
	GraphLayer_SetZIndex( glayer, z_index );
	Record_WidgetUpdate( widget->parent, NULL, DATATYPE_SORT, 0 );
	return 0;
}

LCUI_API void
Widget_SetAlpha(LCUI_Widget *widget, unsigned char alpha)
/* ���ܣ��趨������͸���� */
{
	if( GraphLayer_GetAlpha( widget->main_glayer ) != alpha) {
		GraphLayer_SetAlpha( widget->main_glayer, alpha );
		Widget_Refresh( widget );
	}
}

LCUI_API void
Widget_ExecMove( LCUI_Widget *widget, LCUI_Pos pos )
/*
 * ���ܣ�ִ���ƶ�����λ�õĲ���
 * ˵�������Ĳ���λ�ã�����Ӿֲ�ˢ������
 **/
{
	LCUI_Rect old_rect, new_rect;
	LCUI_Pos max_pos, min_pos;

	if( !widget ) {
		return;
	}
	max_pos = Widget_GetMaxPos( widget );
	min_pos = Widget_GetMinPos( widget );

	/* �������Ƶ��ƶ���Χ������λ�� */
	if(pos.x > max_pos.x) {
		pos.x = max_pos.x;
	}
	if(pos.y > max_pos.y) {
		pos.y = max_pos.y;
	}
	if(pos.x < min_pos.x) {
		pos.x = min_pos.x;
	}
	if(pos.y < min_pos.y) {
		pos.y = min_pos.y;
	}
	if( pos.x == widget->pos.x
	 && pos.y == widget->pos.y ) {
		return;
	}
	/* ���ͼ������ʾ�ģ�����λ�ñ䶯���Ǿ���Ҫ�����Ч���� */
	if( widget->visible ) {
		old_rect = Widget_GetRect( widget );
		//_DEBUG_MSG("old:%d,%d,%d,%d\n",
		// rect.x, rect.y, rect.width, rect.height);
		widget->pos = pos;
		new_rect.x = pos.x;
		new_rect.y = pos.y;
		new_rect.width = old_rect.width;
		new_rect.height = old_rect.height;
		//_DEBUG_MSG("new:%d,%d,%d,%d\n",
		// rect.x, rect.y, rect.width, rect.height);
		Widget_InvalidArea( widget->parent, new_rect );
		Widget_InvalidArea( widget->parent, old_rect );
	} else {
		/* ����ֱ�Ӹ����� */
		widget->pos = pos;
	}
	GraphLayer_SetPos( widget->main_glayer, pos.x, pos.y );
}

LCUI_API void
Widget_ExecHide(LCUI_Widget *widget)
/* ���ܣ�ִ�����ز����Ĳ��� */
{
	if( !widget || !widget->visible ) {
		return;
	}

	/* ��ȡ���ز�����Ҫ���õĺ���ָ�룬������֮ */
	WidgetFunc_Call( widget, FUNC_TYPE_HIDE );
	Widget_Visible( widget, FALSE );
	Widget_InvalidArea( widget->parent, Widget_GetRect(widget) );
}

/* Ϊ�������Ӳ����������� */
static void
Widget_ExecSortChild( LCUI_Widget *widget )
{
	int i, j, total;
	LCUI_Widget *child_a, *child_b;
	LCUI_Queue *child_list;

	child_list = Widget_GetChildList( widget );
	Queue_Lock( child_list );
	total = Queue_GetTotal( child_list );
	/* �Ƚ�ģ̬�����ƶ�������ǰ�� */
	for(j=i=0; i<total; ++i) {
		child_a = (LCUI_Widget*)Queue_Get( child_list, i );
		if( !child_a || !child_a->modal ) {
			continue;
		}
		/* ����ģ̬������z-indexֵ */
		Widget_SetZIndex( child_a, 0 );
		Queue_Move( child_list, 0, i );
		++j; /* j����ͳ��ģ̬���������� */
	}
	/* ����ǰ��j��ģ̬�������ӵ�j��������ʼ */
	for(i=j; i<total; ++i) {
		child_a = (LCUI_Widget*)Queue_Get( child_list, i );
		if( !child_a ) {
			continue;
		}
		for(j=i+1; j<total; ++j) {
			child_b = (LCUI_Widget*)Queue_Get( child_list, j );
			if( !child_b ) {
				continue;
			}
			if( child_b->main_glayer->z_index
			 > child_a->main_glayer->z_index ) {
				Queue_Swap( child_list, j, i );
			}
		}
	}
	Queue_Unlock( child_list );
	GraphLayer_Sort( Widget_GetClientGraphLayer(widget) );
}

/* ��������ʾ��ͬ��z-indexֵ�Ĳ�����ǰ�� */
LCUI_API int
Widget_Front( LCUI_Widget *widget )
{
	int i, src_pos, des_pos, total;
	LCUI_Widget *child;
	LCUI_Queue *child_list;

	if( widget == NULL ) {
		return -1;
	}
	child_list = Widget_GetChildList( widget->parent );
	Queue_Lock( child_list );
	total = Queue_GetTotal( child_list );
	/* ���ڶ������ҵ��Լ����Լ�z-indexֵС�ڻ�������ĵ�һ������ */
	for(i=0,src_pos=des_pos=-1; i<total; ++i) {
		child = (LCUI_Widget*)Queue_Get( child_list, i );
		if( !child ) {
			continue;
		}
		if( child == widget ) {
			src_pos = i;
			continue;
		}
		if( des_pos == -1 ) {
			if( child->main_glayer->z_index
			  <= widget->main_glayer->z_index ) {
				des_pos = i;
			}
		} else {
			if( src_pos != -1 ) {
				break;
			}
		}
	}
	/* û���ҵ����˳� */
	if( des_pos == -1 || src_pos == -1 ) {
		Queue_Unlock( child_list );
		return -1;
	}
	if( src_pos+1 == des_pos ) {
		Queue_Unlock( child_list );
		return 1;
	}
	/* �ҵ��Ļ����ƶ�λ�� */
	Queue_Move( child_list, des_pos, src_pos );
	Queue_Unlock( child_list );
	return GraphLayer_Front( Widget_GetMainGraphLayer(widget) );
}

LCUI_API void
Widget_ExecShow(LCUI_Widget *widget)
/* ���ܣ�ִ����ʾ���������� */
{
	if( !widget || widget->visible ) {
		return;
	}
	/* ���øò�������ʾʱ��Ҫ�õ��ĺ��� */
	WidgetFunc_Call(widget, FUNC_TYPE_SHOW);
	Widget_Visible( widget, TRUE ); /* �����ɼ� */
	if( widget->focus ) {
		//Set_Focus( widget );	/* ��������ò��� */
	}
	Widget_Front(widget); /* �ı䲿��������λ�� */
	Widget_Refresh( widget ); /* ˢ�²������������ͼ����ʾ */
}

/* �Զ�����������С������Ӧ�����ݴ�С */
LCUI_API void
Widget_AutoResize(LCUI_Widget *widget)
{
	int i, total, temp;
	LCUI_Widget *child;
	LCUI_Queue point;
	LCUI_Size size;

	size.w = 0;
	size.h = 0;
	Queue_Init(&point, sizeof (LCUI_Pos), NULL);
	total = Queue_GetTotal(&widget->child);
	for(i=0; i<total; ++i) {/* ����ÿ���Ӳ��� */
		child = Queue_Get(&widget->child, i);
		if( !child ) {
			continue;
		}
		/* ��������Ӳ���������ε����ϽǶ����У�X���������� */
		temp = child->pos.x + child->size.w;
		if(temp > size.w) {
			size.w = temp;
		}
		/* ͬ�� */
		temp = child->pos.y + child->size.h;
		if(temp > size.h) {
			size.h = temp;
		}
	}
	//size.w += 6;
	//size.h += 6;
	//printf("Widget_AutoResize(): %p, autosize: %d, new size: %d,%d\n",
	//widget, widget->auto_size, size.w, size.h);
	//print_widget_info(widget);
	/* �ó��ʺϵĳߴ磬����֮ */
	Widget_Resize(widget, size);
}

LCUI_API void
Widget_ExecResize(LCUI_Widget *widget, LCUI_Size size)
/* ���ܣ�ִ�иı䲿���ߴ�Ĳ��� */
{
	LCUI_WidgetEvent event;

	if( !widget ) {
		return;
	}

	if( widget->size.w == size.w && widget->size.h == size.h ) {
		return;
	}

	/* ��¼�¼����� */
	event.type = EVENT_RESIZE;
	event.resize.new_size = size;
	event.resize.old_size = widget->size;

	if( widget->visible ) {
		LCUI_Rect rect;
		rect = Widget_GetRect( widget );
		Widget_InvalidArea( widget->parent, rect );
		widget->size = size;
		rect.width = size.w;
		rect.height = size.h;
		Widget_InvalidArea( widget->parent, rect );
	} else {
		widget->size = size;
	}
	//_DEBUG_MSG("size: %d, %d\n", size.w, size.h);
	GraphLayer_Resize( widget->main_glayer, size.w, size.h );
	/* �����ͻ���ͼ��ߴ� */
	size.w -= (widget->padding.left + widget->padding.right);
	size.h -= (widget->padding.top + widget->padding.bottom);
	//_DEBUG_MSG("size: %d, %d\n", size.w, size.h);
	GraphLayer_Resize( widget->client_glayer, size.w, size.h );

	WidgetFunc_Call( widget, FUNC_TYPE_RESIZE );
	//Widget_Refresh( widget );

	Widget_UpdateChildSize( widget ); /* �����Ӳ�����λ�ü��ߴ� */
	if( widget->parent && widget->parent->auto_size ) {
		/* �����Ҫ�����������ܹ��Զ�������С */
		Widget_AutoResize( widget->parent );
	}
	Widget_DispatchEvent( widget, &event ); /* ��������RESIZE�¼� */
}

/* ���û���ò������Զ��ߴ�������� */
LCUI_API void
Widget_SetAutoSize(	LCUI_Widget *widget,
			LCUI_BOOL flag, AUTOSIZE_MODE mode )
{
	widget->auto_size = flag;
	widget->auto_size_mode = mode;
}

LCUI_API void
Widget_ExecRefresh(LCUI_Widget *widget)
/* ���ܣ�ִ��ˢ����ʾָ����������������ͼ�εĲ��� */
{
	//_DEBUG_MSG("refresh widget: %d,%d,%d,%d\n",
	//	Widget_GetRect(widget).x, Widget_GetRect(widget).y,
	//	Widget_GetRect(widget).width, Widget_GetRect(widget).height );
	Widget_InvalidArea( widget->parent, Widget_GetRect(widget) );
}

LCUI_API void
Widget_ExecUpdate(LCUI_Widget *widget)
/* ���ܣ�ִ�в����ĸ��²��� */
{
	WidgetFunc_Call( widget, FUNC_TYPE_UPDATE );
}

LCUI_API void
Widget_ExecDrawBackground( LCUI_Widget *widget )
{
	int fill_mode;
	LCUI_Graph *graph;
	LCUI_Background *bg;

	graph = Widget_GetSelfGraph( widget );
	bg = &widget->background;
	/* �������͸������ʹ�ø���ģʽ������ͼ���Ƶ������� */
	if( bg->transparent ) {
		fill_mode = GRAPH_MIX_FLAG_REPLACE;
	} else { /* ����ʹ�õ���ģʽ */
		fill_mode = GRAPH_MIX_FLAG_OVERLAY;
	}
	fill_mode |= bg->layout;
	Graph_FillImage( graph, &bg->image, fill_mode, bg->color );
}

/* ִ�в���ͼ�θ��²��� */
LCUI_API void
Widget_ExecDraw(LCUI_Widget *widget)
{
	LCUI_Graph *graph;
	LCUI_WidgetEvent event;

	if( !widget ) {
		return;
	}
	/* �ȸ���һ�β��� */
	WidgetFunc_Call( widget, FUNC_TYPE_UPDATE );
	/* Ȼ����ݲ�����ʽ�����Ʊ���ͼ�� */
	Widget_ExecDrawBackground( widget );
	/* ���ø����Ͳ���Ĭ�ϵĺ������д��� */
	WidgetFunc_Call( widget, FUNC_TYPE_DRAW );
	graph = Widget_GetSelfGraph( widget );
	/* ���Ʊ߿��� */
	Graph_DrawBorder( graph, widget->border );
	Graph_UpdateAttr( graph );

	event.type = EVENT_REDRAW;
	/* ��������RESIZE�¼� */
	Widget_DispatchEvent( widget, &event );
}

/* ��ȡָ�򲿼�����ͼ�����ݵ�ָ�� */
LCUI_API LCUI_Graph*
Widget_GetSelfGraph( LCUI_Widget *widget )
{
	return GraphLayer_GetSelfGraph( widget->main_glayer );
}

/* ��ȡ����ʵ����ʾ��ͼ�� */
LCUI_API int
Widget_GetGraph(
	LCUI_Widget *widget,
	LCUI_Graph *graph_buff,
	LCUI_Rect rect )
{
	return GraphLayer_GetGraph( widget->main_glayer,
		graph_buff, rect );
}

LCUI_API LCUI_Pos
Widget_GetValidPos( LCUI_Widget *widget, LCUI_Pos pos )
/* ��ȡ��Ч������������ݣ���ʵ���ǽ������Ʒ�Χ������괦��������Ʒ�Χ�ڵ� */
{
	if( pos.x > widget->max_x.px ) {
		pos.x = widget->max_x.px;
	}
	if( pos.x < widget->min_x.px ) {
		pos.x = widget->min_x.px;
	}
	if( pos.y > widget->max_y.px ) {
		pos.y = widget->max_y.px;
	}
	if( pos.y < widget->min_y.px ) {
		pos.y = widget->min_y.px;
	}
	return pos;
}

LCUI_API void
Widget_Move(LCUI_Widget *widget, LCUI_Pos new_pos)
/*
 * ���ܣ��ƶ�����λ��
 * ˵������������Ĳ���ΪALIGN_NONE����ô���Ϳ����ƶ�����λ�ã������޷��ƶ�λ��
 * */
{
	if( !widget ) {
		return;
	}
	widget->x.px = new_pos.x;
	widget->y.px = new_pos.y;
	/* ��¼�����ĸ������ݣ��ȴ����и��� */
	Record_WidgetUpdate( widget, &new_pos, DATATYPE_POS, 0 );
}

LCUI_API void
Widget_UpdatePos(LCUI_Widget *widget)
/* ���ܣ����²�����λ�� */
{
	Record_WidgetUpdate( widget, NULL, DATATYPE_POS, 0 );
}

LCUI_API void
Widget_ExecUpdatePos( LCUI_Widget *widget )
/* ���²�����λ�ã��Լ�λ�ñ䶯��Χ */
{
	LCUI_Pos pos;
	pos = _Widget_GetPos( widget );
	widget->max_pos = _Widget_GetMaxPos( widget );
	widget->min_pos = _Widget_GetMinPos( widget );
	/* ֻ���ڲ������뷽ʽ��ΪALIGN_NONE�����߼�������겻Ϊ(0,0)ʱ�ı�λ�� */
	if( widget->align != ALIGN_NONE || (pos.x != 0 && pos.y != 0)) {
		Widget_ExecMove( widget, pos );
	}
}

LCUI_API void
Widget_UpdateSize( LCUI_Widget *widget )
/* �����ߴ���� */
{
	LCUI_Size size;

	size = _Widget_GetSize(widget);
	Widget_Resize( widget, size );
}

LCUI_API void
Widget_UpdateChildSize(LCUI_Widget *widget)
/*
 * ���ܣ�����ָ���������Ӳ����ĳߴ�
 * ˵�����������ߴ�ı���еĲ����ĳߴ��Լ�λ���ǰ��ٷֱ���ģ���Ҫ���¼��㡣
 * */
{
	int i, total;
	LCUI_Widget *child;

	if( !widget ) {
		return;
	}
	total = Queue_GetTotal(&widget->child);
	for(i=0; i<total; ++i) {
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if( !child ) {
			continue;
		}
		Widget_UpdateSize( child );
		Widget_UpdatePos( child );
	}
}

LCUI_API void
Widget_ExecUpdateSize( LCUI_Widget *widget )
{
	widget->size = _Widget_GetSize( widget );
	widget->w.px = widget->size.w;
	widget->h.px = widget->size.h;
}

LCUI_API void
Widget_UpdateChildPos(LCUI_Widget *widget)
/*
 * ���ܣ�����ָ���������Ӳ�����λ��
 * ˵��������Ϊ�Ӳ��������������ĳߴ�ı���еĲ����Ĳ��ֲ�ΪALIGN_NONE������Ҫ����
 * ����λ�á�
 * */
{
	LCUI_Widget *child;
	int i, total;
	total = Queue_GetTotal(&widget->child);
	for(i=0; i<total; ++i) {
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		Widget_UpdatePos( child );
	}
	/* ���¸ò����ڶ�λ����Ϊstatic���Ӳ�����λ�� */
	Widget_UpdateChildStaticPos( widget );
}

/* �趨�����ĸ߶ȣ���λΪ���� */
LCUI_API void
Widget_SetHeight( LCUI_Widget *widget, int height )
{
	switch( widget->dock ) {
	    case DOCK_TYPE_NONE:
	    case DOCK_TYPE_TOP:
	    case DOCK_TYPE_BOTTOM:
		widget->h.which_one = 0;
		widget->h.px = height;
		break;
	    default: return;
	}
}

LCUI_API void
Widget_SetSize( LCUI_Widget *widget, char *width, char *height )
/*
 * ���ܣ��趨�����ĳߴ��С
 * ˵��������趨�˲�����ͣ��λ�ã����Ҹ�ͣ������Ĭ�������˿�/�ߣ���ô�����Ŀ�/�߾Ͳ��ܱ��ı䡣
 * �÷�ʾ����
 * Widget_SetSize( widget, "100px", "50px" ); �����ߴ����Ϊ100x50����
 * Widget_SetSize( widget, "100%", "50px" ); ������ȵ���������ȣ��߶�Ϊ50����
 * Widget_SetSize( widget, "50", "50" ); �����ߴ����Ϊ50x50���أ�px����ʡ��
 * Widget_SetSize( widget, NULL, "50%" ); ������ȱ���ԭ�����߶�Ϊ�����߶ȵ�һ��
 * */
{
	switch( widget->dock ) {
	    case DOCK_TYPE_TOP:
	    case DOCK_TYPE_BOTTOM: /* ֻ�ܸı�� */
		get_PX_P_t( height, &widget->h );
		break;
	    case DOCK_TYPE_LEFT:
	    case DOCK_TYPE_RIGHT:/* ֻ�ܸı�� */
		get_PX_P_t( width, &widget->w );
		break;
	    case DOCK_TYPE_FILL:break;
	    case DOCK_TYPE_NONE: /* �ɸı��͸� */
		get_PX_P_t( width, &widget->w );
		get_PX_P_t( height, &widget->h );
		break;
	}
	Widget_UpdateSize( widget );
}

/* ָ�������Ƿ�Ϊģ̬���� */
LCUI_API void
Widget_SetModal( LCUI_Widget *widget, LCUI_BOOL is_modal )
{
	if( !widget ) {
		return;
	}
	widget->modal = is_modal;
	Widget_SetZIndex( widget, 0 );
}

LCUI_API void
Widget_SetDock( LCUI_Widget *widget, DOCK_TYPE dock )
/* �趨������ͣ������ */
{
	switch( dock ) {
	    case DOCK_TYPE_TOP:
		Widget_SetAlign( widget, ALIGN_TOP_CENTER, Pos(0,0) );
		Widget_SetSize( widget, "100%", NULL );
		break;
	    case DOCK_TYPE_BOTTOM:
		Widget_SetAlign( widget, ALIGN_BOTTOM_CENTER, Pos(0,0) );
		Widget_SetSize( widget, "100%", NULL );
		break;
	    case DOCK_TYPE_LEFT:
		Widget_SetAlign( widget, ALIGN_MIDDLE_LEFT, Pos(0,0) );
		Widget_SetSize( widget, NULL, "100%" );
		break;
	    case DOCK_TYPE_RIGHT:
		Widget_SetAlign( widget, ALIGN_MIDDLE_RIGHT, Pos(0,0) );
		Widget_SetSize( widget, NULL, "100%" );
		break;
	    case DOCK_TYPE_FILL:
		Widget_SetAlign( widget, ALIGN_MIDDLE_CENTER, Pos(0,0) );
		Widget_SetSize( widget, "100%", "100%" );
		break;
	    case DOCK_TYPE_NONE:
		Widget_SetAlign( widget, ALIGN_NONE, Pos(0,0) );
		break;
	}
	widget->dock = dock;
}

LCUI_API void
Widget_OffsetPos(LCUI_Widget *widget, LCUI_Pos offset)
/* ���ܣ��Բ���ԭ�е�λ��Ϊ����������ָ����ƫ������ƫ��λ�� */
{
	Widget_Move( widget, Pos_Add(widget->pos, offset) );
}

LCUI_API void
Widget_MoveToPos(LCUI_Widget *widget, LCUI_Pos des_pos, int speed)
/*
 * ���ܣ���������ָ�����ٶ���ָ��λ���ƶ�
 * ˵����des_pos��Ŀ��λ�ã�speed�Ǹò������ƶ��ٶȣ���λΪ������/��
 * */
{
	int i, j;
	double w, h, l, n, x, y;

	if(speed <= 0) {
		return;
	}
	x = Widget_GetPos(widget).x;
	y = Widget_GetPos(widget).y;
	/* ������֮��ľ��� */
	w = des_pos.x-x;
	h = des_pos.y-y;
	l = sqrt(pow(w, 2) + pow(h, 2));

	n = l/speed;/* ���ƶ�����ʱ�� */
	n = n*100;	/* ���ƶ�����������100������Ϊÿ��0.01�ƶ�һ��λ�� */
	w = w/n;	/* ����ÿ���ƶ���x��y��������ƶ����� */
	h = h/n;
	j = (int)(n>(int)n?n+1:n);
	for(i=0; i<j; i++) {
		x += w;
		y += h;
		Widget_Move(widget, Pos(x, y));
		LCUI_MSleep(10);/* ͣ��0.01�� */
	}
}

LCUI_API void
Widget_Refresh(LCUI_Widget *widget)
/* ���ܣ�ˢ����ʾָ����������������ͼ�� */
{
	if( !widget ) {
		return;
	}
	Record_WidgetUpdate( widget, NULL, DATATYPE_AREA, 0 );
}

/* ���������ĳߴ� */
LCUI_API void 
Widget_Resize( LCUI_Widget *widget, LCUI_Size new_size )
{
	LCUI_Size max_size, min_size;

	if( widget == NULL || new_size.w < 0 || new_size.h < 0) {
		return;
	}
	max_size = Widget_GetMaxSize( widget );
	min_size = Widget_GetMinSize( widget );
	/* ����ָ���Ĳ����ĳߴ緶Χ�������ߴ� */
	if(new_size.w > max_size.w) {
		new_size.w = max_size.w;
	}
	if(new_size.h > max_size.h) {
		new_size.h = max_size.h;
	}
	if(new_size.w < min_size.w) {
		new_size.w = min_size.w;
	}
	if(new_size.h < min_size.h) {
		new_size.h = min_size.h;
	}
	widget->w.px = new_size.w;
	widget->h.px = new_size.h;

	Record_WidgetUpdate( widget, &new_size, DATATYPE_SIZE, 0 );
	if( widget->pos_type == POS_TYPE_STATIC
	 || widget->pos_type == POS_TYPE_RELATIVE ) {
		Record_WidgetUpdate( widget, NULL, DATATYPE_POS_TYPE, 0 );
	}
}

LCUI_API void
Widget_Draw(LCUI_Widget *widget)
/* ���ܣ����»��Ʋ��� */
{
	if( !widget ) {
		return;
	}
	Record_WidgetUpdate( widget, NULL, DATATYPE_GRAPH, 0 );
}

/* ���ٲ��� */
LCUI_API void
Widget_Destroy( LCUI_Widget *widget )
{
	if( !widget ) {
		return;
	}
	Cancel_Focus( widget ); /* ȡ������ */
	Record_WidgetUpdate( widget, NULL, DATATYPE_DESTROY, 0 );
}

LCUI_API void
Widget_Update(LCUI_Widget *widget)
/*
 * ���ܣ��ò����������趨�����ԣ�������Ӧ���ݵĸ���
 * ˵�����˼�¼����������У������������һ����ͬ��¼���򸲸�������¼��
 * */
{
	if( !widget ) {
		return;
	}
	Record_WidgetUpdate( widget, NULL, DATATYPE_UPDATE, 0 );
}

LCUI_API void
__Widget_Update(LCUI_Widget *widget)
/*
 * ���ܣ��ò����������趨�����ԣ�������Ӧ���ݵĸ���
 * ˵�������ϸ���������һ�������ǣ����������������������ͬ��¼��
 * */
{
	if( !widget ) {
		return;
	}
	Record_WidgetUpdate( widget, NULL, DATATYPE_UPDATE, 1 );
}

/* ��ʾ���� */
LCUI_API void
Widget_Show(LCUI_Widget *widget)
{
	if( !widget ) {
		return;
	}
	Record_WidgetUpdate( widget, NULL, DATATYPE_SHOW, 0 );
	Record_WidgetUpdate( widget, NULL, DATATYPE_SORT, 0 );
}

LCUI_API void
Widget_Hide(LCUI_Widget *widget)
/* ���ܣ����ز��� */
{
	if( !widget ) {
		return;
	}
	Cancel_Focus( widget ); /* ȡ������ */
	Record_WidgetUpdate( widget, NULL, DATATYPE_HIDE, 0 );
}

/* �ı䲿����״̬ */
LCUI_API int
Widget_SetState( LCUI_Widget *widget, int state )
{
	if( !widget ) {
		return -1;
	}
	/* �����״̬�����ò���֧�� */
	if( (widget->valid_state & state) != state ) {
		return 1;
	}
	if( !widget->enabled ) {
		state = WIDGET_STATE_DISABLE;
	}
	Record_WidgetUpdate( widget, &state, DATATYPE_STATUS, 0 );
	return 0;
}
/************************* Widget End *********************************/



/**************************** Widget Update ***************************/
typedef union union_widget_data
{
	LCUI_Pos pos;
	LCUI_Size size;
	int state;
}
u_wdata;

typedef struct _WidgetData
{
	int type;	/* ���ݵ����� */
	int valid;	/* �����Ƿ���Ч */
	u_wdata data;	/* ��Ҫ���µ����� */
	LCUI_Widget *widget;
} WidgetData;

/* ��ʼ����¼�������ݸ��µĶ��� */
LCUI_API void
WidgetUpdateBuff_Init( LCUI_Queue *queue )
{
	Queue_Init( queue, sizeof(WidgetData), NULL );
}

/*
 * ���ܣ���¼��Ҫ�������ݸ��µĲ���
 * ˵����������ָ���Լ�����µ�������������У����ݲ�������ʾ˳�������ж���
 * ����ֵ���������ز�С��0��ֵ
 *  */
LCUI_API int
Record_WidgetUpdate( LCUI_Widget *widget, void *data, DATATYPE type, int flag )
{
	int i, total, n_found, result = 0;
	WidgetData temp, *tmp_ptr;
	LCUI_Queue *des_queue;

	if( !widget ) {
		return -1;
	}
	/* �������� */
	temp.type = type;
	if( data ) {
		temp.valid = TRUE;
	} else {
		temp.valid = FALSE;
	}
	switch(type) {
	    case DATATYPE_POS :
		if(temp.valid) {
			temp.data.pos = *((LCUI_Pos*)data);
		}
		break;
	    case DATATYPE_SIZE :
		if(temp.valid) {
			temp.data.size = *((LCUI_Size*)data);
		}
		break;
	    case DATATYPE_STATUS :
		if(temp.valid) {
			temp.data.state = *((int*)data);
		}
		break;
	    case DATATYPE_GRAPH	:
	    case DATATYPE_AREA:
	    case DATATYPE_HIDE:
	    case DATATYPE_POS_TYPE:
	    case DATATYPE_UPDATE:
	    case DATATYPE_SHOW:
	    case DATATYPE_SORT:
	    case DATATYPE_DESTROY:
		temp.valid = FALSE;
		break;
	    default: return -2;
	}
	if( widget->parent ) {
		des_queue = &widget->parent->update_buff;
	} else {
		des_queue = &LCUI_Sys.update_buff;
	}
	total = Queue_GetTotal( des_queue );
	for(n_found=0,i=0; i<total; ++i) {
		tmp_ptr = Queue_Get( des_queue, i );
		if( !tmp_ptr || tmp_ptr->widget != widget ) {
			continue;
		}
		if(tmp_ptr->valid != temp.valid
		|| tmp_ptr->type != temp.type) {
			continue;
		}
		++n_found;
		/* ����Ѵ��ڵ���������2 */
		if( flag == 1 && n_found < 2 ) {
			continue;
		}
		/* ������Ҫ�����滻 */
		switch(type) {
		    case DATATYPE_POS :
			if(temp.valid) {
				tmp_ptr->data.pos = temp.data.pos;
				tmp_ptr->valid = TRUE;
			} else {
				tmp_ptr->valid = FALSE;
			}
			break;
		    case DATATYPE_SIZE :
			if(temp.valid) {
				tmp_ptr->data.size = temp.data.size;
				tmp_ptr->valid = TRUE;
			} else {
				tmp_ptr->valid = FALSE;
			}
			break;
		    case DATATYPE_STATUS :
			if(temp.valid) {
				tmp_ptr->data.state = temp.data.state;
			} else {
				tmp_ptr->valid = FALSE;
			}
			break;
		    case DATATYPE_GRAPH	:
		    case DATATYPE_AREA:
		    case DATATYPE_HIDE:
		    case DATATYPE_POS_TYPE:
		    case DATATYPE_UPDATE:
		    case DATATYPE_SHOW:
		    case DATATYPE_SORT:
			temp.valid = FALSE;
			break;
		    default: return -1;
		}
		break;
	}
	/* δ�ҵ���������µ� */
	if( i>= total ) {
		temp.widget = widget;
		result = Queue_Add( des_queue, &temp );
	}
	return result;
}

LCUI_API void
Widget_ProcessUpdate( LCUI_Widget *widget )
{
	int n;
	WidgetData *tmp_ptr;
	LCUI_Widget *child;
	LCUI_Queue *update_buff, *child_list;
	if( widget ) {
		update_buff = &widget->update_buff;
		child_list = &widget->child;
	} else {
		update_buff = &LCUI_Sys.update_buff;
		child_list = &LCUI_Sys.widget_list;
	}
	Queue_Lock( update_buff );
	n = Queue_GetTotal( update_buff );
	while(n--) {
		tmp_ptr = Queue_Get( update_buff, 0 );
		if( tmp_ptr == NULL || tmp_ptr->widget == NULL ) {
			continue;
		}
		/* ���ݲ�ͬ�����������д��� */
		switch(tmp_ptr->type) {
		    case DATATYPE_SIZE	:
			/* �����ߴ���£������²�����λ�� */
			if( !tmp_ptr->valid ) {
				Widget_ExecUpdateSize( tmp_ptr->widget );
			} else {
				Widget_ExecResize(tmp_ptr->widget, tmp_ptr->data.size);
			}
			Widget_ExecDraw(tmp_ptr->widget);
			/* ��Ҫ����λ�ã����Բ���break */
		    case DATATYPE_POS	:
			/*
			 * ���ڸ���λ�ÿ��ܻ��Ǹ��²����ߴ�ĸ��Ӳ�������Ҫ�ж�һ�¸�������
			 * �Ƿ�ΪDATATYPE_POS
			 * */
			if( tmp_ptr->type == DATATYPE_POS
			 && tmp_ptr->widget->align == ALIGN_NONE
			 && tmp_ptr->valid ) {
				Widget_ExecMove(tmp_ptr->widget, tmp_ptr->data.pos);
			} else {
				Widget_ExecUpdatePos( tmp_ptr->widget );
			}
			break;
		    case DATATYPE_POS_TYPE:
			Widget_UpdateChildStaticPos( tmp_ptr->widget->parent );
			break;
		    case DATATYPE_UPDATE:
			Widget_ExecUpdate( tmp_ptr->widget );
			break;
		    case DATATYPE_STATUS:
			/* ֻ��״̬��һ�����ػ沿�� */
			if( tmp_ptr->widget->state == tmp_ptr->data.state ) {
				break;
			}
			tmp_ptr->widget->state = tmp_ptr->data.state;
			/* �ı䲿��״̬����Ҫ�����ػ棬���Բ���break */
		    case DATATYPE_GRAPH	:
			Widget_ExecDraw(tmp_ptr->widget);
			break;
		    case DATATYPE_HIDE:
			Widget_ExecHide(tmp_ptr->widget);
			break;
		    case DATATYPE_SORT:
			Widget_ExecSortChild(widget);
			break;
		    case DATATYPE_SHOW:
			Widget_ExecShow(tmp_ptr->widget);
			/* ���¸������е�STATIC��λ���͵��Ӳ�����λ�� */
			Widget_UpdateChildStaticPos( tmp_ptr->widget->parent );
			break;
		    case DATATYPE_AREA:
			Widget_ExecRefresh(tmp_ptr->widget);
			break;
		    case DATATYPE_DESTROY:
			/* ���ˢ������ */
			Widget_ExecRefresh(tmp_ptr->widget);
			/* ��ʼ���ٲ������� */
			Widget_ExecDestroy(tmp_ptr->widget);
			break;
		    default: break;
		}
		Queue_Delete( update_buff, 0 );
	}
	Queue_Unlock( update_buff );
	n = Queue_GetTotal( child_list );
	/* ��β����,�ݹ鴦���Ӳ����ĸ��� */
	while(n--) {
		child = Queue_Get( child_list, n );
		if( child ) {
			Widget_ProcessUpdate( child );
		}
	}
}

/************************ Widget Update End ***************************/
