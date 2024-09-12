#ifndef __LCUI_IME_H__
#define __LCUI_IME_H__

LCUI_BEGIN_HEADER

typedef struct LCUIIME_Func_ {
	LCUI_BOOL (*prockey)(int);
	void (*settarget)(LCUI_Widget*);
	LCUI_Widget *(*gettarget)(void);
	void (*totext)(char);
	LCUI_BOOL (*open)(void);
	LCUI_BOOL (*close)(void);
} LCUIIME_Func;

/* ע��һ�����뷨 */
LCUI_EXPORT(int)
LCUIIME_Register( const char *ime_name, LCUIIME_Func *ime_func );

/* ѡ�����뷨 */
LCUI_EXPORT(LCUI_BOOL)
LCUIIME_Select( int ime_id );

LCUI_EXPORT(LCUI_BOOL)
LCUIIME_SelectByName( const char *name );

/* �л�����һ�����뷨 */
LCUI_EXPORT(void)
LCUIIME_Switch(void);

/* ������뷨�Ƿ�Ҫ�������¼� */
LCUI_EXPORT(LCUI_BOOL)
LCUIIME_ProcessKey( const LCUI_KeyboardEvent *event );

/* �ύ���뷨�����������Ŀ�� */
LCUI_EXPORT(int)
LCUIIME_Commit( const wchar_t *str );

/* �������뷨��Ŀ�� */
LCUI_EXPORT(int)
LCUIIME_SetTarget( LCUI_Widget *widget );

/* ��ʼ��LCUI���뷨ģ�� */
LCUI_EXPORT(void)
LCUIModule_IME_Init(void);

/* ͣ��LCUI���뷨ģ�� */
LCUI_EXPORT(void)
LCUIModule_IME_End(void);

LCUI_END_HEADER

#endif