/* ***************************************************************************
 * LCUI_Thread.c -- base thread management
 * 
 * Copyright (C) 2012 by
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
 * LCUI_Thread.c -- �������̹߳���
 *
 * ��Ȩ���� (C) 2012 ������ 
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

/*
 * �ļ�����LCUI_Thread.c
 * ˵��������ʵ�ֶ��̵߳Ĺ�������LCUI��δʵ�ֶ����ͨ�ţ�ֻ�ܿ��߳�ID�������ĸ�������
 * �ĸ�����ģ�Ŀǰ�����ǽ��������̵߳���ʽ���У��������Խ��̵���ʽ���У����߳�Ϊ����ƽ̨��
 * ����������ڸ�ƽ̨�����У���Ȼ��ʵ�����ݵĹ������ǣ�һ��ĳ��������ִ����������̾�
 * ����ֹ��
 * ��ʹ�ñ��ļ��ڵĺ���ʱ������̹߳�ϵ�����Ҿ��ý�������ֺ�һЩ��������Ӧ���������磺
 * �����̣߳��ͻ��ȵõ�����ʱ�ĸ��߳�ID������������е�ÿ����㣬�����ƥ����߳�ID����
 * ��������ӳ�ID����������̵߳����̶߳����У����û��ƥ��ģ��Ǿ�����һ�����������С�
 * �������̣߳�Ҳ���ƣ��Ȳ��ң����Ƴ���
 * Ϊȷ��LCUI�ڽ����󣬲�����Ϊ���������߳���ʹ��LCUI�Ѿ��ͷŵ���Դ�������¶δ���LCUI
 * �����˳�ǰ�����������Ѽ�¼���̣߳�Ȼ�������Դ�ͷš�
 * */

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include <unistd.h>

void Thread_TreeNode_Init(Thread_TreeNode *ttn)
/* ���ܣ���ʼ���߳������ */
{
	ttn->tid = 0;
	ttn->child.queue = NULL;
	ttn->child.max_num = 0;
	ttn->child.total_num = 0;
	ttn->parent = NULL;
}


Thread_TreeNode *
Search_Thread_Tree(Thread_TreeNode *ttn, thread_t tid)
/*
 * ���ܣ���ָ���߳����Ľ��������ƥ����߳�ID���������߳�������ָ��
 * ��ʾ���ñ��������㷨�Ƚϼ�ª���д����ơ�
 **/
{ 
	if(NULL == ttn) {
		return NULL;
	}
	if(ttn->tid == tid) {
		return ttn;
	}
	
	int i;
	Thread_TreeNode *new_ttn;
	for(i=0; i<ttn->child.max_num; ++i) {
		if( !ttn->child.queue[i] ) {
			continue;
		}
		/* ������߳������ָ����Ч���ͼ����ݹ���� */
		new_ttn = Search_Thread_Tree(ttn->child.queue[i], tid);
		if( new_ttn ) {
			return new_ttn;
		}
	}
	return NULL;/* û����forѭ���ﷵ��������ָ�룬��ô�������ﷵ��NULL */
}

Thread_TreeNode *
Thread_TreeNode_Add_New(Thread_TreeNode *ttn, thread_t tid)
/* ���ܣ����߳���������µĽ�� */
{
	int temp = 0, i;
	LCUI_ID pos = 0;
	if(ttn->child.total_num > 0) {
		/* �ȱ������ݣ���δʹ�õĿ�λ */
		for(i = 0; i < ttn->child.max_num; ++i) {
			if(ttn->child.queue[i] == NULL) {
				if(temp == 0) {
					temp = 1;
					pos = i;
				}
			}
			else if(ttn->child.queue[i]->tid == tid) {
				 /* ������߳�IDһ�� */
				return ttn->child.queue[i];
			}
		}
		ttn->child.total_num += 1; /* ���Ӹó�����ܴ������� */ 
	} else {
		pos = 0;
		ttn->child.total_num = 1;
		ttn->child.max_num = ttn->child.total_num; 
		ttn->child.queue = (Thread_TreeNode**)calloc(1, 
				sizeof( Thread_TreeNode*));
		
		if(ttn->child.queue == NULL) {/* ��������ڴ�ʧ�� */ 
		//
			return NULL;
		}
		temp = 1;
	}
	
	if(temp == 0) {/* ���û�п�λ���Ǿ������ڴ� */ 
		pos = ttn->child.max_num;
		++ttn->child.max_num; 
		ttn->child.queue = (Thread_TreeNode**)realloc(ttn->child.queue, 
				ttn->child.max_num * sizeof(Thread_TreeNode*));
		if(ttn->child.queue == NULL) {/* ��������ڴ�ʧ�� */
			return NULL;
		}
	}
	
	ttn->child.queue[pos] = (Thread_TreeNode*)calloc(1, sizeof(Thread_TreeNode));
	/* ������Ҫ��ʼ�� */
	Thread_TreeNode_Init(ttn->child.queue[pos]);
	ttn->child.queue[pos]->tid = tid; /* �����߳�ID */
	ttn->child.queue[pos]->parent = ttn; /* ���游�̵߳Ľ��ָ�� */
	return ttn->child.queue[pos];
}


int Thread_TreeNode_Delete(Thread_TreeNode *ttn, thread_t tid)
/* ���ܣ����߳�����ɾ��һ����� */
{
	int i;
	Thread_TreeNode *tt;
	tt = Search_Thread_Tree(&LCUI_Sys.thread_tree, tid);
	if(tt == NULL) {
		return -1;
	}
	tt = tt->parent; /* �õ����̵߳Ľ��ָ�� */
	if(tt == NULL) {
		return -2;
	}
	if(tt->child.total_num <= 0) {
		return -3;
	}
	for(i = 0; i < tt->child.total_num; ++i) { 
		if(tt->child.queue[i] != NULL 
		  && tt->child.queue[i]->tid == tid
		) {
			--tt->child.total_num; 
			free(tt->child.queue[i]);
			tt->child.queue[i] = NULL;
			return 0; 
		}
	}
	return -3;
}

int LCUI_Thread_Create( thread_t *tidp,
			const thread_attr_t *attr,
			void *(*start_rtn)(void*),
			void * arg )
/*
 * ���ܣ�����һ���߳�
 * ˵������Ҫ�ǵ���pthread_create�����������̣߳������и��Ӵ���
 * */
{
	Thread_TreeNode *tt;
	/* �Ȼ�ȡ��ǰ�����̵߳�ID */
	thread_t t = thread_self();
	/* ���߳����в���ƥ����߳�ID�����õ��ý���ָ�� */
	tt = Search_Thread_Tree(&LCUI_Sys.thread_tree, t);
	if(tt == NULL) {/* ���û�����������Ǿ�����������øý��ָ�� */
		tt = Thread_TreeNode_Add_New(&LCUI_Sys.thread_tree, t);
		if(tt == NULL) {
			return -1; /* ������Ļ��ͷ���-1 */
		}
	}
	/* ����pthread_create�����������߳� */
	thread_create(tidp, attr, start_rtn, arg);
	//printf("create thread:%lu, parent thread:%lu\n", *tidp, t);
	Thread_TreeNode_Add_New(tt, *tidp);/* ���õ����߳�ID�������� */
	return 0;
}

int LCUI_Thread_Join(thread_t thread, void **retval)
/* ���ܣ��ȴ�һ���̵߳Ľ��� */
{
	int tmp;
	tmp = thread_join(thread, retval);
	/* ���߳�����ɾ������̵߳Ľ�� */
	if(tmp == 0) {/* ���pthread_join�����ķ���ֵ���� */
		Thread_TreeNode_Delete(&LCUI_Sys.thread_tree, thread);
	}
	return tmp;
}

int LCUI_Thread_Cancel(thread_t thread) 
/* ���ܣ�����һ���߳� */
{
	//printf("cancel thread:%lu\n", thread);
	int tmp;
	tmp = thread_cancel(thread);
	/* ���߳�����ɾ������̵߳Ľ�� */
	if(tmp == 0) {
		Thread_TreeNode_Delete(&LCUI_Sys.thread_tree, thread);
	}
	return tmp;
}

void LCUI_Thread_Exit(void* retval)
/*
 * ���ܣ���ֹ���������̲߳�����һ��ָ��ĳ�������ָ��
 * ˵�����߳�ͨ������LCUI_Thread_Exit������ִֹ�У�����ͬ�����ڽ�
 * ��ʱ����exit����һ����
 * */
{
	thread_exit(retval);
}


int LCUI_Thread_Tree_Cancel(Thread_TreeNode *ttn)
/* ���ܣ������̹߳�ϵ���Ľ���е��߳��Լ������������߳� */
{
	if(ttn == NULL) {
		return 0;
	}
	
	int i;
	for(i=0; i<ttn->child.max_num; ++i) {
		if(ttn->child.queue[i] == NULL) {
			continue;
		}
		/* ������߳������ָ����Ч���ͼ����ݹ���ã�ֱ��ĩβ���ſ�ʼ��ǰ�����߳� */
		LCUI_Thread_Tree_Cancel(ttn->child.queue[i]); 
	}
	return LCUI_Thread_Cancel(ttn->tid);
}

int LCUI_App_Thread_Cancel(LCUI_ID app_id)
/* ���ܣ�����ָ��ID�ĳ����ȫ���߳� */
{
	Thread_TreeNode *ttn;
	ttn = Search_Thread_Tree(&LCUI_Sys.thread_tree, app_id);
	if(ttn == NULL) {
		return -1;
	}
	
	int i;
	for(i=0; i<ttn->child.max_num; ++i) {
		if(ttn->child.queue[i] != NULL) {
		/* ֻ����Ԥ�ȳ�������̣߳����̻߳��ڳ���������Լ����� */
			LCUI_Thread_Tree_Cancel(ttn->child.queue[i]); 
		}
	}
	return 0; 
}
