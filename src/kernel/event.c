/* ***************************************************************************
 * event.c -- event processing module
 *
 * Copyright (C) 2012-2015 by
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
 * event.c -- 事件处理模块
 *
 * 版权所有 (C) 2012-2015 归属于
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
//#define DEBUG
#define __IN_EVENT_SOURCE_FILE__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <LCUI_Build.h>
#include <LCUI/misc/linkedlist.h>
#include <LCUI/misc/rbtree.h>
#include <LCUI/kernel/event.h>

#define NEW_ONE(type) (type*)malloc(sizeof(type))

/** 为本文件内的函数名加上前缀 */
#define $(FUNC_NAME) LCUIEventBox_##FUNC_NAME

/** 事件处理器 */
typedef struct LCUI_EventHandlerRec_ {
	int id;				/**< 标识号 */
	EventCallBack func;		/**< 回调函数 */
	void *func_data;		/**< 传给回调函数的数据 */
	void (*destroy_data)(void*);	/**< 用于销毁数据的回调函数 */
} LCUI_EventHandler;

/** 事件槽，记录与事件链接的响应函数 */
typedef struct EventSlotRec_ {
	int id;			/**< 事件标识号 */
	char *name;		/**< 事件名 */
	LinkedList handlers;	/**< 事件处理器列表 */
} LCUI_EventSlot;

/** 事件处理器相关数据 */
struct LCUI_EventBoxRec_ {
	LCUI_RBTree event_slot;		/**< 事件槽记录 */
	LCUI_RBTree event_name;		/**< 事件名称记录 */
	LCUI_RBTree event_handler;	/**< 事件处理器记录 */
	LinkedList events[2];		/**< 2个事件任务列表 */
	int current;			/**< 当前使用的事件列表号（下标） */
	int event_id;			/**< 事件的标识号计数器 */
	int handler_id;			/**< 事件处理器的标识号计数器 */
};

static int CompareEventName( void *data, const void *keydata )
{
	return strcmp(*(char**)data, (const char*)keydata);
}

static void DestroyEventSlot( void *data )
{
	LCUI_EventSlot *slot = (LCUI_EventSlot*)data;
	free( slot->name );
	slot->name = NULL;
	LinkedList_Destroy( &slot->handlers );
}

static void DestroyEventHandler( void *data )
{
	LCUI_EventHandler* handler = (LCUI_EventHandler*)data;
	if( handler->destroy_data ) {
		handler->destroy_data( handler->func_data );
	}
}

static void DestroyEvent( void *data )
{
	LCUI_Event *event = (LCUI_Event*)data;
	if( event->destroy_data && event->data ) {
		event->destroy_data( event->data );
		event->data = NULL;
	}
}

/** 创建一个事件容器实例 */
LCUI_EventBox $(Create)(void)
{
	LCUI_EventBox box;
	box = NEW_ONE(struct LCUI_EventBoxRec_);
	box->event_id = 100;
	box->handler_id = 100;
	box->current = 0;
	RBTree_Init( &box->event_slot );
	RBTree_Init( &box->event_name );
	RBTree_Init( &box->event_handler );
	RBTree_OnJudge( &box->event_name, CompareEventName );
	RBTree_OnDestroy( &box->event_handler, DestroyEventHandler );
	RBTree_OnDestroy( &box->event_slot, DestroyEventSlot );
	RBTree_SetDataNeedFree( &box->event_handler, 0 );
	RBTree_SetDataNeedFree( &box->event_name, 0 );
	LinkedList_Init( &box->events[0], sizeof(LCUI_Event) );
	LinkedList_Init( &box->events[1], sizeof(LCUI_Event) );
	LinkedList_SetDataNeedFree( &box->events[0], 1 );
	LinkedList_SetDataNeedFree( &box->events[1], 1 );
	LinkedList_SetDestroyFunc( &box->events[0], DestroyEvent );
	LinkedList_SetDestroyFunc( &box->events[1], DestroyEvent );
	DEBUG_MSG("eventbox: %p, handler_id: %d\n", box, box->handler_id);
	return box;
}

/** 销毁事件容器实例 */
void $(Destroy)( LCUI_EventBox box )
{
	RBTree_Destroy( &box->event_name );
	RBTree_Destroy( &box->event_handler );
	RBTree_Destroy( &box->event_slot );
	LinkedList_Destroy( &box->events[0] );
	LinkedList_Destroy( &box->events[1] );
}

/** 注册事件，指定事件名称和ID */
int $(AddEvent)( LCUI_EventBox box, const char *event_name, int id )
{
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;

	/* 查找事件槽记录 */
	if( RBTree_CustomSearch( &box->event_name, event_name ) ) {
		return -1;
	}
	if( RBTree_Search( &box->event_slot, id ) ) {
		return -2;
	}
	/* 新建一个事件槽 */
	slot = NEW_ONE(LCUI_EventSlot);
	slot->name = (char*)malloc(sizeof(char)*(strlen(event_name)+1));
	slot->id = id;
	strcpy( slot->name, event_name );
	LinkedList_Init( &slot->handlers, sizeof(LCUI_EventHandler) );
	/* 添加事件槽记录 */
	RBTree_Insert( &box->event_slot, slot->id, slot );
	/* 添加事件名记录 */
	node = RBTree_CustomInsert(
		&box->event_name, (const void*)event_name, &slot->name
	);
	/* 结点的 key 就是事件槽的 id */
	node->key = slot->id;
	return 0;
}

/** 检测事件名是否已经存在（已注册） */
int $(IsExistEventName)( LCUI_EventBox box, const char *event_name )
{
	if( RBTree_CustomSearch( &box->event_name, (const void*)event_name) ) {
		return 1;
	}
	return 0;
}

/** 检测事件ID是否已经存在 */
int $(IsExistEventId)( LCUI_EventBox box, int id )
{
	if( RBTree_Search( &box->event_slot, id ) ) {
		return 1;
	}
	return 0;
}

/** 获取指定事件ID的名称 */
const char *$(GetEventName)( LCUI_EventBox box, int id )
{
	LCUI_RBTreeNode *node = RBTree_Search( &box->event_slot, id );
	if( node ) {
		return ((LCUI_EventSlot*)node->data)->name;
	}
	return NULL;
}

/** 绑定指定ID的事件 */
int $(BindById)( LCUI_EventBox box, int event_id, EventCallBack func,
		 void *func_data, void (*destroy_data)(void*) )
{
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventHandler *handler;
	void *data;

	node = RBTree_Search( &box->event_slot, event_id );
	if( !node ) {
		return -1;
	}
	slot = (LCUI_EventSlot*)node->data;
	handler = NEW_ONE(LCUI_EventHandler);
	DEBUG_MSG("eventbox: %p, handler_id: %d\n", box, box->handler_id);
	handler->id = ++box->handler_id;
	handler->func = func;
	handler->func_data = func_data;
	handler->destroy_data = destroy_data;
	LinkedList_Append( &slot->handlers, handler );
	/* 将int类型的值转换成void×类型的值 */
	data = &slot->id;
	data = *(void**)data;
	RBTree_Insert( &box->event_handler, handler->id, data );
	return handler->id;
}

/** 绑定指定名称的事件 */
int $(Bind)(	LCUI_EventBox box, const char *event_name, EventCallBack func,
		void *func_data, void (*destroy_data)(void*) )
{
	int id;
	LCUI_RBTreeNode *node;

	/* 查找事件槽记录 */
	node = RBTree_CustomSearch( &box->event_name, 
				    (const void*)event_name );
	/** 没有就注册一个事件槽 */
	if( !node ) {
		id = ++box->event_id;
		if( $(AddEvent)( box, event_name, id ) != 0 ) {
			return -1;
		}
	} else {
		id = node->key;
	}
	return $(BindById)( box, id, func, func_data, destroy_data );
}

/** 解除事件连接 */
int $(Unbind)( LCUI_EventBox box, int handler_id )
{
	int i, n, *data;
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventHandler *handler;

	node = RBTree_Search(&box->event_handler, handler_id);
	if( !node ) {
		return -1;
	}
	/* 将data指针的值转换成 int 型数据 */
	data = (int*)&node->data;
	node = RBTree_Search(&box->event_slot, *data);
	if( !node ) {
		return -2;
	}
	slot = (LCUI_EventSlot*)node->data;
	LinkedList_Goto( &slot->handlers, 0 );
	n = LinkedList_GetTotal( &slot->handlers );
	for( i=0; i<n; ++i ) {
		handler = (LCUI_EventHandler*)LinkedList_Get( &slot->handlers );
		if( handler->id == handler_id ) {
			LinkedList_Delete( &slot->handlers );
			RBTree_Erase( &box->event_handler, handler_id );
			return 0;
		}
		LinkedList_ToNext( &slot->handlers );
	}
	return -3;
}

/** 直接将事件发送至事件处理器进行处理 */
int $(Send)( LCUI_EventBox box, const char *name, void *data )
{
	int i, n;
	LCUI_Event event;
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LCUI_EventHandler *handler;
	DEBUG_MSG("send event: %s\n", name);
	if( !(node = RBTree_CustomSearch(&box->event_name, (const void*)name)) ) {
		return -1;
	}
	if( !(node = RBTree_Search(&box->event_slot, node->key)) ) {
		return -2;
	}
	slot = (LCUI_EventSlot*)node->data;
	event.id = slot->id;
	event.name = slot->name;
	event.data = data;
	event.destroy_data = NULL;
	n = LinkedList_GetTotal( &slot->handlers );
	for( i=0; i<n; ++i ) {
		LinkedList_Goto( &slot->handlers, i );
		handler = (LCUI_EventHandler*)LinkedList_Get( &slot->handlers );
		if( handler && handler->func ) {
			handler->func( &event, handler->func_data );
		}
	}
	return 0;
}

/** 将事件投递给事件处理器，等待处理 */
int $(Post)(	LCUI_EventBox box, const char *name, void *data,
		 void (*destroy_data)(void*) )
{
	LCUI_Event *e;
	LCUI_RBTreeNode *node;
	LCUI_EventSlot *slot;
	LinkedList *elist = &box->events[box->current];

	DEBUG_MSG("eventbox: %p, handler_id: %d\n", box, box->handler_id);
	if( !(node = RBTree_CustomSearch(&box->event_name, (const void*)name)) ) {
		return -1;
	}
	if( !(node = RBTree_Search(&box->event_slot, node->key)) ) {
		return -2;
	}
	e = (LCUI_Event*)LinkedList_Alloc( elist );
	slot = (LCUI_EventSlot*)node->data;
	e->id = slot->id;
	e->name = slot->name;
	e->data = data;
	e->destroy_data = destroy_data;
	return 0;
}

/** 派发当前待处理的事件至对应的处理器 */
int $(Dispatch)( LCUI_EventBox box )
{
	int i, n;
	LCUI_Event *e;
	LinkedList *elist;

	elist = &box->events[box->current];
	box->current = box->current ? 0:1;
	n = LinkedList_GetTotal( elist );
	LinkedList_Goto( elist, 0 );
	DEBUG_MSG("event total: %d\n", n);
	for( i=0; i<n; ++i ) {
		e = (LCUI_Event*)LinkedList_Get( elist );
		$(Send)( box, e->name, e->data );
		e->destroy_data ? e->destroy_data( e->data ):0;
		e->data = NULL;
		LinkedList_Goto( elist, 0 );
		LinkedList_Delete( elist );
	}
	DEBUG_MSG("quit\n");
	return 0;
}

/** 从已触发的事件记录中取出（不会移除）一个事件信息 */
int $(GetEvent)( LCUI_EventBox box, LCUI_Event *ebuff )
{
	int n;
	LCUI_Event *event;
	LinkedList *elist;

	elist = &box->events[box->current];
	n = LinkedList_GetTotal( elist );
	if( n <= 0 ) {
		return -1;
	}
	LinkedList_Goto( elist, 0 );
	event = (LCUI_Event*)LinkedList_Get( elist );
	*ebuff = *event;
	return 0;
}

/** 从已触发的事件记录中删除一个事件信息 */
int $(DeleteEvent)( LCUI_EventBox box )
{
	LinkedList *elist;
	elist = &box->events[box->current];
	if( LinkedList_GetTotal( elist ) <= 0 ) {
		return -2;
	}
	LinkedList_Delete( elist );
	return 0;
}
