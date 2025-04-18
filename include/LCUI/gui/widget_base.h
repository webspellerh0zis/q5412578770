/* ***************************************************************************
 * widget_base.h -- the widget base operation set.
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
 * widget_base.h -- �����Ļ�����������
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

#ifndef __LCUI_WIDGET_BASE_H__
#define __LCUI_WIDGET_BASE_H__
/* �����������ͱ�ʶ */
typedef enum DATATYPE_{
	DATATYPE_POS,
	DATATYPE_POS_TYPE,
	DATATYPE_SIZE,
	DATATYPE_GRAPH,
	DATATYPE_UPDATE,
	DATATYPE_STATUS,
	DATATYPE_SHOW,
	DATATYPE_SORT,
	DATATYPE_HIDE,
	DATATYPE_AREA,
	DATATYPE_DESTROY
} DATATYPE;

/*************************** Container ********************************/
/* �������������Ϊ�����Ĳ����� */
LCUI_API int
Widget_Container_Add( LCUI_Widget *ctnr, LCUI_Widget *widget );

/* ͨ������ó�ָ�������������Ŀ�ȣ���λΪ���� */
LCUI_API int
_Widget_GetContainerWidth(LCUI_Widget *widget);

/* ͨ������ó�ָ�������������ĸ߶ȣ���λΪ���� */
LCUI_API int
_Widget_GetContainerHeight(LCUI_Widget *widget);

/* ��ȡ�����Ŀ�� */
LCUI_API int
Widget_GetContainerWidth( LCUI_Widget *widget );

/* ��ȡ�����ĸ߶� */
LCUI_API int
Widget_GetContainerHeight( LCUI_Widget *widget );

/* ��ȡ�����ĳߴ� */
LCUI_API LCUI_Size
Widget_GetContainerSize( LCUI_Widget *widget );
/************************* Container End ******************************/

/***************************** Widget *********************************/

/* ��ȡ��������ͼ��ָ�� */
LCUI_API LCUI_GraphLayer *
Widget_GetMainGraphLayer( LCUI_Widget *widget );

/* ��ȡ�����Ŀͻ���ͼ��ָ�� */
LCUI_API LCUI_GraphLayer *
Widget_GetClientGraphLayer( LCUI_Widget *widget );

/* ��ȡ�������Ӳ������� */
LCUI_API LCUI_Queue*
Widget_GetChildList( LCUI_Widget *widget );

/* ��ȡ�����ľ���������� */
LCUI_API LCUI_RectQueue*
Widget_GetInvalidAreaQueue( LCUI_Widget *widget );

/*-------------------------- Widget Pos ------------------------------*/
/* ͨ������ó�������x������ */
LCUI_API int
_Widget_GetX( LCUI_Widget *widget );

/* ͨ������ó�������y������ */
LCUI_API int
_Widget_GetY( LCUI_Widget *widget );


/* ���ܣ�ͨ������ó�������λ�ã���λΪ���� */
LCUI_API LCUI_Pos
_Widget_GetPos(LCUI_Widget *widget);

/*
 * ���ܣ���ȡ�������������������λ��
 * ˵������λ��������������������Ͻǵ㣬���������������ڱ߾ࡣ
 *  */
LCUI_API LCUI_Pos
Widget_GetPos(LCUI_Widget *widget);

/*
 * ���ܣ���ȡ������������������������λ��
 * ˵�������������������򲢲�һ����������������������Ϊ���������С�ܵ�
 * �����������ڱ߾��Ӱ�졣
 *  */
LCUI_API LCUI_Pos
Widget_GetRelPos(LCUI_Widget *widget);

/*
 * ���ܣ�ȫ������ת�����������
 * ˵���������ȫ�����꣬�����ݴ���Ĳ���ָ�룬ת��������ڸò���������
 *  */
LCUI_API LCUI_Pos
Widget_ToRelPos(LCUI_Widget *widget, LCUI_Pos global_pos);

LCUI_API int
_Widget_GetMaxX( LCUI_Widget *widget );

LCUI_API int
_Widget_GetMaxY( LCUI_Widget *widget );

LCUI_API LCUI_Pos
_Widget_GetMaxPos( LCUI_Widget *widget );

LCUI_API LCUI_Pos
Widget_GetMaxPos(LCUI_Widget *widget);

LCUI_API int
_Widget_GetMinX( LCUI_Widget *widget );

LCUI_API int
_Widget_GetMinY( LCUI_Widget *widget );

LCUI_API LCUI_Pos
_Widget_GetMinPos( LCUI_Widget *widget );

LCUI_API LCUI_Pos
Widget_GetMinPos(LCUI_Widget *widget);
/*------------------------ END Widget Pos -----------------------------*/

/*------------------------- Widget Size -------------------------------*/
LCUI_API int
_Widget_GetMaxWidth( LCUI_Widget *widget );

LCUI_API int
_Widget_GetMaxHeight( LCUI_Widget *widget );

LCUI_API LCUI_Size
_Widget_GetMaxSize( LCUI_Widget *widget );

LCUI_API LCUI_Size
Widget_GetMaxSize( LCUI_Widget *widget );

LCUI_API int
_Widget_GetMinWidth( LCUI_Widget *widget );

LCUI_API int
_Widget_GetMinHeight( LCUI_Widget *widget );

LCUI_API int
_Widget_GetHeight(LCUI_Widget *widget);

LCUI_API int
_Widget_GetWidth(LCUI_Widget *widget);

LCUI_API int
Widget_GetHeight(LCUI_Widget *widget);

LCUI_API int
Widget_GetWidth(LCUI_Widget *widget);

/* ��ȡ�����ĳߴ� */
LCUI_API LCUI_Size
Widget_GetSize(LCUI_Widget *widget);

/* ͨ�������ȡ�����ĳߴ� */
LCUI_API LCUI_Size
_Widget_GetSize(LCUI_Widget *widget);

/*------------------------ END Widget Size ----------------------------*/


/* ��ȡ���������� */
LCUI_API LCUI_Rect
Widget_GetRect(LCUI_Widget *widget);

/* ��ȡ������˽�����ݽṹ���ָ�� */
LCUI_API void*
Widget_GetPrivData(LCUI_Widget *widget);

LCUI_API int
Widget_PrintChildList( LCUI_Widget *widget );

/*
 * ���ܣ���ӡwidget����Ϣ
 * ˵�����ڵ���ʱ��Ҫ�õ���������ȷ��widget�Ƿ�������
 *  */
LCUI_API void
print_widget_info(LCUI_Widget *widget);

/* ��ָ���������ڲ��������趨��Ҫˢ�µ����� */
LCUI_API int
Widget_InvalidArea( LCUI_Widget *widget, LCUI_Rect rect );

/* �����пɼ���������Ч���� ͬ���� ��Ļ��Ч��������� */
LCUI_API void Widget_SyncInvalidArea( void );

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
Widget_SetValidState( LCUI_Widget *widget, int state );

/*
 * ���ܣ���ȡ������ָ�����͵ĸ�������ָ��
 * ˵�������������ڲ�����ϵ������ͷ�����Ҹ�����ָ�룬���ж�����������Ƿ�Ϊ�ƶ�����
 * ����ֵ��û�з���Ҫ��ĸ��������ͷ���NULL�����򷵻ز���ָ��
 **/
LCUI_API LCUI_Widget*
Widget_GetParent(LCUI_Widget *widget, char *widget_type);

/* ��ȡָ��ID���Ӳ��� */
LCUI_API LCUI_Widget*
Widget_GetChildByID( LCUI_Widget *widget, LCUI_ID id );

/* ����ָ��ID�ĳ�������в��� */
LCUI_API void
LCUIApp_DestroyAllWidgets( LCUI_ID app_id );

/* ���ָ�������Ƿ��ڽ���״̬ */
LCUI_API LCUI_BOOL
Widget_GetFocus( LCUI_Widget *widget );

/* ���ָ�������Ƿ���ȫ�ֽ��㣬Ҳ���Ǹò����Լ��ϼ����и������Ƿ񶼵õ��˽��� */
LCUI_API LCUI_BOOL
Widget_GetGlobalFocus( LCUI_Widget *widget );

/* ��ȡ�����ķ������ */
LCUI_API LCUI_String
Widget_GetStyleName( LCUI_Widget *widget );

/* �趨�����ķ������ */
LCUI_API void
Widget_SetStyleName( LCUI_Widget *widget, const char *style_name );

/* �趨�����ķ��ID */
LCUI_API void
Widget_SetStyleID( LCUI_Widget *widget, int style_id );

/* ��ȡָ�������ϵ��Ӳ��������򷵻��Ӳ���ָ�룬���򷵻�NULL */
LCUI_API LCUI_Widget*
Widget_At( LCUI_Widget *ctnr, LCUI_Pos pos );

/* ���ܣ�Ϊ����˽�нṹ��ָ������ڴ� */
LCUI_API void*
WidgetPrivData_New( LCUI_Widget *widget, size_t size );

/* ��ʼ���������� */
LCUI_API void
WidgetQueue_Init(LCUI_Queue *queue);

/***********************************************************
<Function>
	Widget_New

<Description>
	����ָ�����͵Ĳ���

<Input>
	widget_type :: ������������

<Return>
	�ɹ��򷵻�ָ��ò�����ָ�룬ʧ���򷵻�NULL
************************************************************/
LCUI_API LCUI_Widget* Widget_New( const char *widget_type );

/* ��ȡ������ȫ������ */
LCUI_API LCUI_Pos
Widget_GetGlobalPos(LCUI_Widget *widget);

/* �趨�����ɱ�����������͸���� */
LCUI_API void
Set_Widget_ClickableAlpha( LCUI_Widget *widget, uchar_t alpha, int mode );

/* �趨�����Ķ��뷽ʽ�Լ�ƫ�ƾ��� */
LCUI_API void
Widget_SetAlign(LCUI_Widget *widget, ALIGN_TYPE align, LCUI_Pos offset);

/*
 * ���ܣ��趨���������ߴ�
 * ˵������ֵΪ0ʱ�������ĳߴ粻�����ƣ��÷�ʾ���ɲο�Set_Widget_Size()����
 * */
LCUI_API int
Widget_SetMaxSize( LCUI_Widget *widget, char *width, char *height );

/* �趨��������С�ߴ� */
LCUI_API int
Widget_SetMinSize( LCUI_Widget *widget, char *width, char *height );

/* ���Ʋ����ĳߴ�䶯��Χ */
LCUI_API void
Widget_LimitSize(LCUI_Widget *widget, LCUI_Size min_size, LCUI_Size max_size);

/* ���Ʋ������ƶ���Χ */
LCUI_API void
Widget_LimitPos(LCUI_Widget *widget, LCUI_Pos min_pos, LCUI_Pos max_pos);

/* �趨�����ı߿� */
LCUI_API void
Widget_SetBorder( LCUI_Widget *widget, LCUI_Border border );

/* �趨�����߿���ĸ��ǵ�Բ�ǰ뾶 */
LCUI_API void
Widget_SetBorderRadius( LCUI_Widget *widget, unsigned int radius );

/* �趨�����ı���ͼ�� */
LCUI_API void
Widget_SetBackgroundImage( LCUI_Widget *widget, LCUI_Graph *img );

/* �趨����ͼ�Ĳ��� */
LCUI_API void
Widget_SetBackgroundLayout( LCUI_Widget *widget, LAYOUT_TYPE layout );

/* �趨�����ı�����ɫ */
LCUI_API void
Widget_SetBackgroundColor( LCUI_Widget *widget, LCUI_RGB color );

/* �趨���������Ƿ�͸�� */
LCUI_API void
Widget_SetBackgroundTransparent( LCUI_Widget *widget, LCUI_BOOL flag );

/* ���ò��� */
LCUI_API void
Widget_Enable(LCUI_Widget *widget);

/* ���ò��� */
LCUI_API void
Widget_Disable(LCUI_Widget *widget);

/* ָ�������Ƿ�ɼ� */
LCUI_API void
Widget_Visible( LCUI_Widget *widget, LCUI_BOOL flag );

/*
 * ���ܣ��趨������λ��
 * ˵����ֻ�޸����꣬�����оֲ�ˢ��
 * */
LCUI_API void
Widget_SetPos(LCUI_Widget *widget, LCUI_Pos pos);

/* ���ò������ڱ߾� */
LCUI_API void
Widget_SetPadding( LCUI_Widget *widget, LCUI_Padding padding );

/* ���ò�������߾� */
LCUI_API void
Widget_SetMargin( LCUI_Widget *widget, LCUI_Margin margin );

/* �趨�����Ķ�λ���� */
LCUI_API void
Widget_SetPosType( LCUI_Widget *widget, POS_TYPE pos_type );

/* ���ò����Ķѵ�˳�� */
LCUI_API int
Widget_SetZIndex( LCUI_Widget *widget, int z_index );

/* �趨������͸���� */
LCUI_API void
Widget_SetAlpha(LCUI_Widget *widget, unsigned char alpha);

/*
 * ���ܣ�ִ���ƶ�����λ�õĲ���
 * ˵�������Ĳ���λ�ã�����Ӿֲ�ˢ������
 **/
LCUI_API void
Widget_ExecMove( LCUI_Widget *widget, LCUI_Pos pos );

/* ִ�����ز����Ĳ��� */
LCUI_API void
Widget_ExecHide(LCUI_Widget *widget);

/* ��������ʾ��ͬ��z-indexֵ�Ĳ�����ǰ�� */
LCUI_API int
Widget_Front( LCUI_Widget *widget );

LCUI_API void
Widget_ExecShow(LCUI_Widget *widget);

/* �Զ�����������С������Ӧ�����ݴ�С */
LCUI_API void
Widget_AutoResize(LCUI_Widget *widget);

/* ִ�иı䲿���ߴ�Ĳ��� */
LCUI_API void
Widget_ExecResize(LCUI_Widget *widget, LCUI_Size size);

/* ���û���ò������Զ��ߴ�������� */
LCUI_API void
Widget_SetAutoSize(	LCUI_Widget *widget,
			LCUI_BOOL flag, AUTOSIZE_MODE mode );

/* ִ��ˢ����ʾָ����������������ͼ�εĲ��� */
LCUI_API void
Widget_ExecRefresh(LCUI_Widget *widget);

/* ִ�в����ĸ��²��� */
LCUI_API void
Widget_ExecUpdate(LCUI_Widget *widget);

/* ִ�в���ͼ�θ��²��� */
LCUI_API void
Widget_ExecDraw(LCUI_Widget *widget);

/* ��ȡָ�򲿼�����ͼ�����ݵ�ָ�� */
LCUI_API LCUI_Graph*
Widget_GetSelfGraph( LCUI_Widget *widget );

/* ��ȡ����ʵ����ʾ��ͼ�� */
LCUI_API int
Widget_GetGraph(
	LCUI_Widget *widget,
	LCUI_Graph *graph_buff,
	LCUI_Rect rect );

/* ��ȡ��Ч������������ݣ���ʵ���ǽ������Ʒ�Χ������괦��������Ʒ�Χ�ڵ� */
LCUI_API LCUI_Pos
Widget_GetValidPos( LCUI_Widget *widget, LCUI_Pos pos );

/*
 * ���ܣ��ƶ�����λ��
 * ˵������������Ĳ���ΪALIGN_NONE����ô���Ϳ����ƶ�����λ�ã������޷��ƶ�λ��
 * */
LCUI_API void
Widget_Move(LCUI_Widget *widget, LCUI_Pos new_pos);

/* ���²�����λ�� */
LCUI_API void
Widget_UpdatePos(LCUI_Widget *widget);

/* ���²�����λ�ã��Լ�λ�ñ䶯��Χ */
LCUI_API void
Widget_ExecUpdatePos( LCUI_Widget *widget );

/* �����ߴ���� */
LCUI_API void
Widget_UpdateSize( LCUI_Widget *widget );

/*
 * ���ܣ�����ָ���������Ӳ����ĳߴ�
 * ˵�����������ߴ�ı���еĲ����ĳߴ��Լ�λ���ǰ��ٷֱ���ģ���Ҫ���¼��㡣
 * */
LCUI_API void
Widget_UpdateChildSize(LCUI_Widget *widget);

LCUI_API void
Widget_ExecUpdateSize( LCUI_Widget *widget );

/*
 * ���ܣ�����ָ���������Ӳ�����λ��
 * ˵��������Ϊ�Ӳ��������������ĳߴ�ı���еĲ����Ĳ��ֲ�ΪALIGN_NONE������Ҫ����
 * ����λ�á�
 * */
LCUI_API void
Widget_UpdateChildPos(LCUI_Widget *widget);

/* �趨�����ĸ߶ȣ���λΪ���� */
LCUI_API void
Widget_SetHeight( LCUI_Widget *widget, int height );

/*
 * ���ܣ��趨�����ĳߴ��С
 * ˵��������趨�˲�����ͣ��λ�ã����Ҹ�ͣ������Ĭ�������˿�/�ߣ���ô�����Ŀ�/�߾Ͳ��ܱ��ı䡣
 * �÷�ʾ����
 * Widget_SetSize( widget, "100px", "50px" ); �����ߴ����Ϊ100x50����
 * Widget_SetSize( widget, "100%", "50px" ); ������ȵ���������ȣ��߶�Ϊ50����
 * Widget_SetSize( widget, "50", "50" ); �����ߴ����Ϊ50x50���أ�px����ʡ��
 * Widget_SetSize( widget, NULL, "50%" ); ������ȱ���ԭ�����߶�Ϊ�����߶ȵ�һ��
 * */
LCUI_API void
Widget_SetSize( LCUI_Widget *widget, char *width, char *height );

/* �趨�����Ƿ�Ϊģ̬���� */
LCUI_API void
Widget_SetModal( LCUI_Widget *widget, LCUI_BOOL is_modal );

/* �趨������ͣ������ */
LCUI_API void
Widget_SetDock( LCUI_Widget *widget, DOCK_TYPE dock );

/* ���ܣ�ˢ����ʾָ����������������ͼ�� */
LCUI_API void
Widget_Refresh(LCUI_Widget *widget);

/* ���������ĳߴ� */
LCUI_API void 
Widget_Resize( LCUI_Widget *widget, LCUI_Size new_size );

/* ���»��Ʋ��� */
LCUI_API void
Widget_Draw(LCUI_Widget *widget);

/* ���ٲ��� */
LCUI_API void
Widget_Destroy( LCUI_Widget *widget );

/*
 * ���ܣ��ò����������趨�����ԣ�������Ӧ���ݵĸ���
 * ˵�����˼�¼����������У������������һ����ͬ��¼���򸲸�������¼��
 * */
LCUI_API void
Widget_Update(LCUI_Widget *widget);

/*
 * ���ܣ��ò����������趨�����ԣ�������Ӧ���ݵĸ���
 * ˵�������ϸ���������һ�������ǣ����������������������ͬ��¼��
 * */
LCUI_API void
__Widget_Update(LCUI_Widget *widget);


/* ��ʾ���� */
LCUI_API void
Widget_Show(LCUI_Widget *widget);

/* ���ز��� */
LCUI_API void
Widget_Hide(LCUI_Widget *widget);

/* �ı䲿����״̬ */
LCUI_API int
Widget_SetState( LCUI_Widget *widget, int state );
/************************* Widget End *********************************/



/**************************** Widget Update ***************************/

/* ��ʼ����¼�������ݸ��µĶ��� */
LCUI_API void
WidgetUpdateBuff_Init( LCUI_Queue *queue );

/*
 * ���ܣ���¼��Ҫ�������ݸ��µĲ���
 * ˵����������ָ���Լ�����µ�������������У����ݲ�������ʾ˳�������ж���
 * ����ֵ���������ز�С��0��ֵ
 *  */
LCUI_API int
Record_WidgetUpdate( LCUI_Widget *widget, void *data, DATATYPE type, int flag );

LCUI_API void
Widget_ProcessUpdate( LCUI_Widget *widget );

/************************ Widget Update End ***************************/

#endif
