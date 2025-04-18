#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_ERROR_H

typedef struct _WidgetTypeData
{
	LCUI_String type_name;
	LCUI_ID type_id;
	LCUI_Queue func;
}
WidgetTypeData;

/*
 * ���ܣ�Ϊָ�����͵Ĳ������������͵ĺ���
 * ����ֵ���������Ͳ����ڣ�����-1���������󷵻�-2
 **/
LCUI_API int 
WidgetFunc_Add(
	const char	*type_name,
	void		(*widget_func)(LCUI_Widget*),
	FuncType	func_type )
{
	LCUI_App *app;
	WidgetTypeData *temp;
	LCUI_Func *temp_func, func_data;
	int total, i, found = 0;

	app = LCUIApp_GetSelf();
	if( !app ) {
		printf("%s(): %s", __FUNCTION__, APP_ERROR_UNRECORDED_APP);
		return -1;
	}

	total = Queue_GetTotal(&app->widget_lib);
	/* �������ݣ��ҵ���Ӧ��λ�� */
	for(i = 0; i < total; ++i) {
		temp = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if( temp == NULL ) {
			continue;
		}
		if(_LCUIString_Cmp( &temp->type_name, type_name) != 0) {
			continue;
		}
		total = Queue_GetTotal(&temp->func);
		for(i=0; i<total; i++) {
			temp_func = (LCUI_Func *)Queue_Get(&temp->func, i);
			if(temp_func->id == func_type) {
				found = 1;
				break;
			}
		}

		Get_FuncData(&func_data, (CallBackFunc)widget_func, NULL, NULL);
		func_data.id = func_type; /* ��������ID */
		if(found == 1) {/* ����Ѿ����ڣ��͸��� */
			Queue_Replace(&temp->func, i, &func_data);
		} else {
			Queue_Add(&temp->func, &func_data);
		}
		return 0;
	}
	return -1;
}

/*
 * ���ܣ����һ���µĲ���������������
 * ����ֵ�������ӵ��²��������Ѵ��ڣ�����-1���ɹ��򷵻�0
 **/
LCUI_API int
WidgetType_Add( const char *type_name )
{
	int total, i;
	LCUI_App *app;
	WidgetTypeData *wd, new_wd;

	app = LCUIApp_GetSelf();
	if( !app ) {
		printf("WidgetType_Add():"APP_ERROR_UNRECORDED_APP);
		exit(-1);
	}

	//printf("WidgetType_Add(): add widget type: %s\n", type);
	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if(_LCUIString_Cmp(&wd->type_name, type_name) == 0) {
			//printf("WidgetType_Add(): the widget type is already registered\n");
			return -1;
		}
	}

	/* ������Ҫ��ʼ�� */
	FuncQueue_Init(&new_wd.func);
	new_wd.type_id = rand(); /* ���������Ϊ����ID */
	//printf("WidgetType_Add(): widget type id: %ld\n", new_wd.type_id);
	LCUIString_Init( &new_wd.type_name );
	_LCUIString_Copy( &new_wd.type_name, type_name );
	Queue_Add( &app->widget_lib, &new_wd );
	return 0;
}

static void WidgetType_Destroy(void *arg)
/* ���ܣ��Ƴ������������� */
{
	WidgetTypeData *wd = (WidgetTypeData*)arg;
	Queue_Destroy(&wd->func);
}

/* ���ܣ���ʼ�������� */
LCUI_API void
WidgetLib_Init(LCUI_Queue *w_lib)
{
	Queue_Init(w_lib, sizeof(WidgetTypeData), WidgetType_Destroy);
}

/* ���ܣ�ɾ��ָ���������͵�������� */
LCUI_API int
WidgetType_Delete(const char *type)
{
	int total,  i;
	LCUI_App *app;
	WidgetTypeData *wd;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return -2;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		/* �������һ�� */
		if(_LCUIString_Cmp(&wd->type_name, type) == 0) {
			return Queue_Delete(&app->widget_lib, i);
		}
	}

	return -1;
}

/*
 * ���ܣ��պ����������κβ���
 * ˵���������ȡָ���������͵ĺ���ָ��ʧ�ܣ���������������ĺ���ָ��
 **/
LCUI_API void
NULL_Widget_Func(LCUI_Widget *widget)
{
	;
}

/* ��ȡָ�����Ͳ���������ID */
LCUI_API LCUI_ID
WidgetType_GetID( const char *widget_type )
{
	int total, i;
	LCUI_App *app;
	WidgetTypeData *wd;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return -2;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if(_LCUIString_Cmp(&wd->type_name, widget_type) == 0) {
			return wd->type_id;
		}
	}

	return -1;
}

LCUI_API int
WidgetType_GetByID(LCUI_ID id, char *widget_type)
/* ���ܣ���ȡָ������ID���������� */
{
	int total, i;
	LCUI_App *app;
	WidgetTypeData *wd;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return -2;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if(wd->type_id == id) {
			strcpy(widget_type, wd->type_name.string);
			return 0;
		}
	}

	return -1;
}

/* ��ȡָ����������ID�ĺ����ĺ���ָ�� */
LCUI_API WidgetCallBackFunc
WidgetFunc_GetByID(LCUI_ID id, FuncType func_type)
{
	LCUI_App *app;
	LCUI_Func *f = NULL;
	WidgetTypeData *wd;
	int total, i, found = 0;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return NULL_Widget_Func;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if( wd == NULL || wd->type_id != id ) {
			continue;
		}
		total = Queue_GetTotal(&wd->func);
		for(i=0; i<total; i++) {
			f = (LCUI_Func *)Queue_Get(&wd->func, i);
			if(f->id == func_type) {
				found = 1;
				break;
			}
		}
		break;
	}

	/* ����Ѿ����� */
	if(found == 1) {
		return (void(*)(LCUI_Widget*))f->func;
	}
	return NULL_Widget_Func;
}

/* ��ȡָ���������Ĳ����ĺ���ָ�� */
LCUI_API WidgetCallBackFunc
WidgetFunc_Get(const char *widget_type, FuncType func_type )
{
	LCUI_App *app;
	int total, i, found = 0;
	WidgetTypeData *wd;
	LCUI_Func *f = NULL;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return NULL_Widget_Func;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if( wd == NULL ) {
			continue;
		}
		if(_LCUIString_Cmp(&wd->type_name, widget_type) != 0) {
			continue;
		}
		total = Queue_GetTotal(&wd->func);
		for(i=0; i<total; i++) {
			f = (LCUI_Func *)Queue_Get(&wd->func, i);
			if(f->id == func_type) {
				found = 1;
				break;
			}
		}
		break;
	}

	if(found == 1) {
		return (void(*)(LCUI_Widget*))f->func;
	}
	return NULL_Widget_Func;
}

/* ���ָ�����������Ƿ���Ч */
LCUI_API int
WidgetType_Valid( const char *widget_type )
{
	int total, i;
	WidgetTypeData *wd;
	LCUI_App *app;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return 0;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if( wd == NULL ) {
			continue;
		}
		if(_LCUIString_Cmp(&wd->type_name, widget_type) == 0) {
			return 1;
		}
	}

	return 0;
}

/* ����ָ�����͵Ĳ������� */
LCUI_API void
WidgetFunc_Call( LCUI_Widget *widget, FuncType type )
{
	void (*func)(LCUI_Widget*);

	func = WidgetFunc_GetByID( widget->type_id, type );
	func( widget );
}

extern void Register_Window(void);
extern void Register_Label(void);
extern void Register_Button(void);
extern void Register_PictureBox(void);
extern void Register_ProgressBar(void);
extern void Register_Menu(void);
extern void Register_CheckBox(void);
extern void Register_RadioButton(void);
extern void Register_ActiveBox(void);
extern void Register_TextBox(void);
extern void Register_ScrollBar(void);

LCUI_API void
Register_DefaultWidgetType()
/* ���ܣ�Ϊ����Ĳ��������Ĭ�ϵĲ������� */
{
	WidgetType_Add(NULL);	/* ���һ��NULL���͵Ĳ��� */
	Register_Window();	/* ע�ᴰ�ڲ��� */
	Register_Label();	/* ע���ı���ǩ���� */
	Register_Button();	/* ע�ᰴť���� */
	Register_PictureBox();	/* ע��ͼƬ���Ӳ��� */
	Register_ProgressBar();
	Register_Menu();
	Register_CheckBox();
	Register_RadioButton();
	Register_ActiveBox();
	Register_TextBox();
	Register_ScrollBar();
}
