/* ***************************************************************************
 * LCUI_Thread.h -- basic thread management
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
 * LCUI_Thread.h -- �������̹߳���
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

#ifndef __LCUI_THREAD_H__
#define __LCUI_THREAD_H__ 

#define RWLOCK_READ		1
#define RWLOCK_WRITE	2

typedef struct _thread_rwlock
{
	pthread_t host;
	pthread_rwlock_t lock;
	//pthread_cond_t cond;
	pthread_mutex_t mutex;
}
thread_rwlock;

/************************ thread R/W lock *****************************/ 

void thread_perror(char *msg, int err);

void thread_rwlock_init(thread_rwlock *rwlock);
/* ���ܣ���ʼ����д�� */ 

void thread_rwlock_destroy(thread_rwlock *rwlock);
/* ���ܣ����ٶ�д�� */ 
	
int thread_wait_mutex(thread_rwlock *rwlock);
/* ���ܣ��ȴ�������Ϊ����״̬ */ 

int thread_rwlock_rdlock(thread_rwlock *rwlock);
/* ���ܣ��趨�������� */ 

int thread_rwlock_wrlock(thread_rwlock *rwlock);
/* ���ܣ��趨��д���� */ 

int thread_rwlock_unlock(thread_rwlock *rwlock);
/* ���ܣ��⿪��д�� */ 

int thread_mutex_lock(thread_rwlock *rwlock);
/* ���ܣ��趨������ */ 

int thread_mutex_unlock(thread_rwlock *rwlock);
/* ���ܣ��⿪������ */ 

/*********************** End thread R/W lock **************************/

void Thread_TreeNode_Init(Thread_TreeNode *ttn);
/* ���ܣ���ʼ���߳������ */ 

Thread_TreeNode *
Search_Thread_Tree(Thread_TreeNode *ttn, pthread_t tid);
/*
 * ���ܣ���ָ���߳����Ľ��������ƥ����߳�ID���������߳�������ָ��
 * ��ʾ���ñ��������㷨�Ƚϼ�ª���д����ơ�
 **/ 

Thread_TreeNode *
Thread_TreeNode_Add_New(Thread_TreeNode *ttn, pthread_t tid);
/* ���ܣ����߳���������µĽ�� */

int Thread_TreeNode_Delete(Thread_TreeNode *ttn, pthread_t tid);
/* ���ܣ����߳�����ɾ��һ����� */ 

int LCUI_Thread_Create( pthread_t *tidp,
						const pthread_attr_t *restrict_attr,
						void *(*start_rtn)(void*),
						void * arg);
/*
 * ���ܣ�����һ���߳�
 * ˵������Ҫ�ǵ���pthread_create�����������̣߳������и��Ӵ���
 * */ 

int LCUI_Thread_Join(pthread_t thread, void **retval);
/* ���ܣ��ȴ�һ���̵߳Ľ��� */ 

int LCUI_Thread_Cancel(pthread_t thread) ;
/* ���ܣ�����һ���߳� */ 

void LCUI_Thread_Exit(void* retval)  __attribute__ ((__noreturn__));
/*
 * ���ܣ���ֹ���������̲߳�����һ��ָ��ĳ�������ָ��
 * ˵�����߳�ͨ������LCUI_Thread_Exit������ִֹ�У�����ͬ�����ڽ�
 * ��ʱ����exit����һ����
 * */ 
 
int LCUI_Thread_Tree_Cancel(Thread_TreeNode *ttn);
/* ���ܣ������̹߳�ϵ���Ľ���е��߳��Լ������������߳� */ 

int LCUI_App_Thread_Cancel(LCUI_ID app_id);
/* ���ܣ�����ָ��ID�ĳ����ȫ���߳� */ 
#endif

