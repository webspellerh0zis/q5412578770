﻿/* linkedlist.c -- Linked LinkedList
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

//#define DEBUG
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/util/linkedlist.h>

void LinkedList_Init(LinkedList *list)
{
	list->length = 0;
	list->head.next = list->tail.next = NULL;
	list->head.data = list->tail.data = NULL;
	list->head.prev = list->tail.prev = NULL;
}

void LinkedList_Unlink(LinkedList *list, LinkedListNode *node)
{
	if (list->length > 0) {
		list->length -= 1;
	}
	if (node->next) {
		node->next->prev = node->prev;
	}
	if (node == list->tail.prev) {
		list->tail.prev = node->prev;
	}
	if (node->prev) {
		node->prev->next = node->next;
	}
	node->prev = NULL;
	node->next = NULL;
}

void LinkedList_ClearEx(LinkedList *list, void (*on_destroy)(void *),
			int free_node)
{
	LinkedListNode *prev, *node;
	node = list->tail.prev;
	list->head.next = NULL;
	list->tail.prev = NULL;
	list->length = 0;
	while (node && node != &list->head) {
		prev = node->prev;
		node->prev = NULL;
		node->next = NULL;
		if (node->data && on_destroy) {
			on_destroy(node->data);
		}
		if (free_node) {
			free(node);
		}
		node = prev;
	}
}

LinkedListNode *LinkedList_GetNodeAtTail(const LinkedList *list, size_t pos)
{
	LinkedListNode *node;
	if (pos >= list->length) {
		return NULL;
	}
	pos += 1;
	node = list->tail.prev;
	while (--pos >= 1 && node) {
		node = node->prev;
	}
	return node;
}

LinkedListNode *LinkedList_GetNode(const LinkedList *list, size_t pos)
{
	LinkedListNode *node;
	if (pos >= list->length) {
		return NULL;
	}
	if (pos > list->length / 2) {
		pos = list->length - pos;
		node = list->tail.prev;
		while (--pos >= 1 && node) {
			node = node->prev;
		}
	} else {
		pos += 1;
		node = list->head.next;
		while (--pos >= 1 && node) {
			node = node->next;
		}
	}
	return node;
}

void LinkedList_Link(LinkedList *list, LinkedListNode *cur,
		     LinkedListNode *node)
{
	node->prev = cur;
	node->next = cur->next;
	if (cur->next) {
		cur->next->prev = node;
	}
	cur->next = node;
	list->length += 1;
}

void LinkedList_InsertNode(LinkedList *list, size_t pos, LinkedListNode *node)
{
	LinkedListNode *target;
	target = LinkedList_GetNode(list, pos);
	if (target) {
		LinkedList_Link(list, target->prev, node);
	} else {
		LinkedList_AppendNode(list, node);
	}
}

LinkedListNode *LinkedList_Insert(LinkedList *list, size_t pos, void *data)
{
	LinkedListNode *node;
	node = malloc(sizeof(LinkedListNode));
	node->data = data;
	LinkedList_InsertNode(list, pos, node);
	return node;
}

void LinkedList_DeleteNode(LinkedList *list, LinkedListNode *node)
{
	LinkedList_Unlink(list, node);
	node->data = NULL;
	free(node);
	node = NULL;
}

void LinkedList_AppendNode(LinkedList *list, LinkedListNode *node)
{
	if (list->head.next) {
		node->prev = list->tail.prev;
		list->tail.prev->next = node;
		list->tail.prev = node;
	} else {
		list->head.next = node;
		list->tail.prev = node;
		node->prev = &list->head;
	}
	node->next = NULL;
	list->length += 1;
}

void LinkedList_Delete(LinkedList *list, size_t pos)
{
	LinkedListNode *node = LinkedList_GetNode(list, pos);
	LinkedList_DeleteNode(list, node);
}

void *LinkedList_Get(const LinkedList *list, size_t pos)
{
	LinkedListNode *node = LinkedList_GetNode(list, pos);
	return node ? node->data : NULL;
}

LinkedListNode *LinkedList_Append(LinkedList *list, void *data)
{
	LinkedListNode *node;
	node = malloc(sizeof(LinkedListNode));
	node->data = data;
	node->next = NULL;
	LinkedList_AppendNode(list, node);
	return node;
}

void LinkedListNode_Delete(LinkedListNode *node)
{
	free(node);
}

void LinkedList_Concat(LinkedList *list1, LinkedList *list2)
{
	if (!list2->head.next) {
		return;
	}
	if (list1->head.next) {
		list1->tail.prev->next = list2->head.next;
		list2->head.next->prev = list1->tail.prev;
	} else {
		list1->head.next = list2->head.next;
		list1->head.next->prev = &list1->head;
	}
	list1->tail.prev = list2->tail.prev;
	list2->head.next = list2->tail.prev = NULL;
	list1->length += list2->length;
	list2->length = 0;
}
