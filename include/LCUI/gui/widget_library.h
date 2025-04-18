#ifndef __LCUI_WIDGET_LIBRARY_H__
#define __LCUI_WIDGET_LIBRARY_H__

LCUI_BEGIN_HEADER

/*
 * ���ܣ�Ϊָ�����͵Ĳ������������͵ĺ���
 * ����ֵ���������Ͳ����ڣ�����-1���������󷵻�-2
 **/
LCUI_API int 
WidgetFunc_Add(
	const char	*type_name,
	void		(*widget_func)(LCUI_Widget*),
	FuncType	func_type );

/*
 * ���ܣ����һ���µĲ���������������
 * ����ֵ�������ӵ��²��������Ѵ��ڣ�����-1���ɹ��򷵻�0
 **/
LCUI_API int
WidgetType_Add( const char *type_name );

/* ��ʼ�������� */
LCUI_API void
WidgetLib_Init(LCUI_Queue *w_lib);

/* ɾ��ָ���������͵�������� */
LCUI_API int
WidgetType_Delete(const char *type);

/* ��ȡָ�����Ͳ���������ID */
LCUI_API LCUI_ID WidgetType_GetID( const char *widget_type );

/* ��ȡָ������ID���������� */
LCUI_API int WidgetType_GetByID(LCUI_ID id, char *widget_type);

/* ��ȡָ����������ID�ĺ����ĺ���ָ�� */
LCUI_API WidgetCallBackFunc
WidgetFunc_GetByID(LCUI_ID id, FuncType func_type);

/* ��ȡָ���������Ĳ����ĺ���ָ�� */
LCUI_API WidgetCallBackFunc
WidgetFunc_Get(const char *widget_type, FuncType func_type );

/* ���ָ�����������Ƿ���Ч */
LCUI_API int
WidgetType_Valid( const char *widget_type );

/* ����ָ�����͵Ĳ������� */
LCUI_API void
WidgetFunc_Call( LCUI_Widget *widget, FuncType type );

/* Ϊ����Ĳ��������Ĭ�ϵĲ������� */
LCUI_API void Register_DefaultWidgetType(void);

LCUI_END_HEADER

#endif
