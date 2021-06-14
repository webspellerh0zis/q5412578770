/* ***************************************************************************
 * LCUI_Widget.h -- processing GUI widget 
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
 * LCUI_Widget.h -- ����GUI����
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
 
#ifndef __LCUI_WIDGET_H__
#define __LCUI_WIDGET_H__

/* �����������ͱ�ʶ */
#define DATATYPE_POS	1
#define DATATYPE_SIZE	2
#define DATATYPE_GRAPH	3
#define DATATYPE_STATUS	4    
#define DATATYPE_SHOW	5
#define DATATYPE_HIDE	6
#define DATATYPE_AREA	7

/***************** ������غ��������� *******************/
typedef enum _FuncType
{
	FUNC_TYPE_SHOW,
	FUNC_TYPE_HIDE,
	FUNC_TYPE_INIT,
	FUNC_TYPE_RESIZE,
	FUNC_TYPE_UPDATE,
	FUNC_TYPE_DESTROY
}FuncType;
/****************************************************/

/***************************** Widget *********************************/

LCUI_Size Get_Widget_Size(LCUI_Widget *widget);
/* ���ܣ���ȡ�����ĳߴ� */

int Get_Widget_Height(LCUI_Widget *widget);

int Get_Widget_Width(LCUI_Widget *widget);

LCUI_Rect Get_Widget_Rect(LCUI_Widget *widget);
/* ���ܣ���ȡ���������� */ 

LCUI_Pos Get_Widget_Pos(LCUI_Widget *widget);
/* ���ܣ���ȡ������λ�� */ 

void *Get_Widget_Private_Data(LCUI_Widget *widget);
/* ���ܣ���ȡ������˽�����ݽṹ���ָ�� */ 

LCUI_Widget *Get_Widget_Parent(LCUI_Widget *widget);
/* ���ܣ���ȡ�����ĸ����� */ 

void print_widget_info(LCUI_Widget *widget);
/* 
 * ���ܣ���ӡwidget����Ϣ
 * ˵�����ڵ���ʱ��Ҫ�õ���������ȷ��widget�Ƿ�������
 * */ 

int Add_Widget_Refresh_Area (LCUI_Widget * widget, LCUI_Rect rect);
/* ���ܣ���ָ���������ڲ��������趨��Ҫˢ�µ����� */ 

void Set_Widget_Border_Style(LCUI_Widget *widget, LCUI_Border_Style style);
/* ���ܣ��趨�����ı߿��� */ 

void Split_Screen_Area_By_Widget(LCUI_Widget *widget);
/* 
 * ���ܣ����ݲ��������еĲ�������������������е�������зָ�
 * ˵�����ָ�������������rect_queue�У���ɾ�����ָ������
 * */ 

void Response_Status_Change(LCUI_Widget *widget);
/* 
 * ���ܣ���ָ��������Ӧ����״̬�ĸı�
 * ˵������������ʱ��Ĭ���ǲ���Ӧ״̬�ı�ģ���Ϊÿ��״̬�ı�󣬶�Ҫ���ú����ػ沿����
 * �����һЩ�����Ƕ���ģ�û��Ҫ�ػ棬Ӱ��Ч�ʡ�������ò�������ť����������ƶ�����
 * ����ʱ�Լ��������ʱ������ı䰴ť��ͼ����ʽ���Ǿ���Ҫ�������������һ�¡�
 *  */ 

void Shift_Widget_Refresh_Area(LCUI_Widget *widget);
/* ���ܣ�ת�Ʋ�����rect���г�Ա���������� */ 

void Processing_Refresh_Area();
/*
 * ���ܣ������Ѽ�¼��ˢ������
 * ˵�����˺����Ὣ����������rect�����еĴ����������
 * ���յľֲ�ˢ�����������������Ļˢ����������У���
 * ��LCUI������
 **/ 

LCUI_Widget *Get_Parent_Widget(LCUI_Widget *widget, char *widget_type);
/*
 * ���ܣ���ȡ������ָ�����͵ĸ�������ָ��
 * ˵�������������ڲ�����ϵ������ͷ�����Ҹ�����ָ�룬���ж�����������Ƿ�Ϊ�ƶ�����
 * ����ֵ��û�з���Ҫ��ĸ��������ͷ���NULL�����򷵻ز���ָ��
 **/ 

int LCUI_Destroy_App_Widgets(LCUI_ID app_id);
/* ���ܣ�����ָ��ID�ĳ�������в��� */ 

LCUI_String Get_Widget_Style(LCUI_Widget *widget);
/* ���ܣ���ȡ���������� */ 

LCUI_Border_Style Get_Widget_Border_Style(LCUI_Widget *widget);
/* ���ܣ���ȡ�����ı߿����� */ 

void Set_Widget_Style(LCUI_Widget *widget, char *style);
/* ���ܣ��趨�����ķ�� */ 

LCUI_Widget *Get_Widget_By_Pos(LCUI_Widget *widget, LCUI_Pos pos);
/* ���ܣ���ȡ�����а���ָ������ĵ���Ӳ��� */ 

LCUI_Widget *Get_Cursor_Overlay_Widget();
/* ���ܣ���ȡ����굱ǰ���ǵĲ��� */ 

LCUI_Widget *Get_Focus_Widget();
/*
 * ���ܣ���ȡ��ǰ����״̬�µĲ���
 * ����ֵ��������ȡ�򷵻ز�����ָ�룬���򣬷���NULL
 **/ 

int Widget_Is_Active(LCUI_Widget *widget);
/* ���ܣ��жϲ����Ƿ�Ϊ�״̬ */ 

LCUI_Rect Get_Widget_Valid_Rect_By_New_Pos(LCUI_Widget *widget, LCUI_Pos offset);
/* 
 * ���ܣ���ȡ�ƶ�����λ�õĲ�������Ļ�ϵ���Ч��ʾ���� 
 * ˵����offset��ԭλ������λ�õĲ�
 * */ 

LCUI_Rect Get_Widget_Valid_Rect(LCUI_Widget *widget);
/* ���ܣ���ȡ��������Ļ��ʵ����ʾ������ */ 

int Empty_Widget();
/* 
 * ���ܣ����ڼ�����Ĳ����б��Ƿ�Ϊ�� 
 * ����ֵ��
 *   1  ����Ĳ����б�Ϊ��
 *   0  ����Ĳ����б�Ϊ��
 * */ 

LCUI_Widget *Create_Widget(char *widget_type);
/* 
 * ���ܣ�����ָ�����͵Ĵ��ڲ���
 * ˵�������������Ĳ�����Ĭ����û�б���ͼʱ͸����
 * ����ֵ���ɹ��򲿼���ָ�룬ʧ���򷵻�NULL
 */ 

void Delete_Widget(LCUI_Widget *widget);
/* ���ܣ�ɾ��һ������ */ 

LCUI_Pos Count_Widget_Pos(LCUI_Widget *widget);
/* ���ܣ��ۼƲ�����λ������ */ 

LCUI_Pos Get_Widget_Global_Pos(LCUI_Widget *widget);
/* ���ܣ���ȡ������ȫ������ */ 

void Set_Widget_BG_Mode(LCUI_Widget *widget, LCUI_BG_Mode bg_mode);
/*
 * ���ܣ��ı䲿���ı���ģʽ
 * ˵��������ģʽ�����˲�����û�б���ͼ��ʱ����ʹ�ñ���ɫ��仹����ȫ͸����
 **/ 

void Set_Widget_Align(LCUI_Widget *widget, LCUI_Align align, LCUI_Pos offset);
/* ���ܣ��趨�����Ķ��뷽ʽ�Լ�ƫ�ƾ��� */ 

void Limit_Widget_Size(LCUI_Widget *widget, LCUI_Size min_size, LCUI_Size max_size);
/* ���ܣ����Ʋ����ĳߴ�䶯��Χ */ 

void Limit_Widget_Pos(LCUI_Widget *widget, LCUI_Pos min_pos, LCUI_Pos max_pos);
/* ���ܣ����Ʋ������ƶ���Χ */ 

void Set_Widget_Border(LCUI_Widget *widget, LCUI_RGB color, LCUI_Border border);
/* ���ܣ��趨�����ı߿� */ 

void Set_Widget_Backcolor(LCUI_Widget *widget, LCUI_RGB color);
/* ���ܣ��趨�����ı���ɫ */ 

int Set_Widget_Background_Image(LCUI_Widget *widget, LCUI_Graph *img, int flag);
/* ���ܣ�Ϊ������䱳��ͼ�� */ 

int Mix_Widget_FontBitmap(	LCUI_Widget *widget, 
							int start_x, int start_y, 
							LCUI_WString *contents, 
							int rows, int space, 
							int linegap, int flag	);
/* ���ܣ���ϲ����ڵ�����λͼ��ʹ�������ڲ�������ʾ */ 

void Enable_Widget(LCUI_Widget *widget);
/* ���ܣ����ò��� */ 

void Disable_Widget(LCUI_Widget *widget);
/* ���ܣ����ò��� */ 

void Widget_Visible(LCUI_Widget *widget, int flag);
/* ���ܣ����岿���Ƿ�ɼ� */ 

void Set_Widget_Pos(LCUI_Widget *widget, LCUI_Pos pos);
/* 
 * ���ܣ��趨������λ�� 
 * ˵����ֻ�޸����꣬�����оֲ�ˢ��
 * */ 

void Set_Widget_Alpha(LCUI_Widget *widget, unsigned char alpha);
/* ���ܣ��趨������͸���� */ 

void Exec_Move_Widget(LCUI_Widget *widget, LCUI_Pos pos);
/*
 * ���ܣ�ִ���ƶ�����λ�õĲ���
 * ˵�������Ĳ���λ�ã�����Ӿֲ�ˢ������
 **/ 

void Exec_Hide_Widget(LCUI_Widget *widget);
/* ���ܣ�ִ�����ز����Ĳ��� */ 

void Exec_Show_Widget(LCUI_Widget *widget);
/* ���ܣ�ִ����ʾ���������� */ 

void Auto_Resize_Widget(LCUI_Widget *widget);
/* ���ܣ��Զ�����������С������Ӧ�����ݴ�С */ 

void Exec_Resize_Widget(LCUI_Widget *widget, LCUI_Size size);
/* ���ܣ�ִ�иı䲿���ߴ�Ĳ��� */ 

void Enable_Widget_Auto_Size(LCUI_Widget *widget);
/* ���ܣ����ò����Զ��ߴ�������� */ 

void Disable_Widget_Auto_Size(LCUI_Widget *widget);
/* ���ܣ����ò����Զ��ߴ�������� */ 

void Exec_Refresh_Widget(LCUI_Widget *widget);
/* ���ܣ�ִ��ˢ����ʾָ����������������ͼ�εĲ��� */ 

void Exec_Draw_Widget(LCUI_Widget *widget);
/* ���ܣ�ִ�в���ͼ�θ��²��� */ 

LCUI_Pos Widget_Align_Get_Pos(LCUI_Widget *widget);
/* ���ܣ����ݲ����Ĳ�������ȡ������λ�� */ 

void Move_Widget(LCUI_Widget *widget, LCUI_Pos new_pos);
/* 
 * ���ܣ��ƶ�����λ��
 * ˵������������Ĳ���ΪALIGN_NONE����ô���Ϳ����ƶ�����λ�ã������޷��ƶ�λ��
 * */ 

void Update_Widget_Pos(LCUI_Widget *widget);
/* ���ܣ����²�����λ�� */ 

void Update_Child_Widget_Pos(LCUI_Widget *widget);
/* 
 * ���ܣ�����ָ���������Ӳ�����λ��
 * ˵��������Ϊ�Ӳ��������������ĳߴ�ı���еĲ����Ĳ��ֲ�ΪALIGN_NONE������Ҫ����
 * ����λ�á�
 * */ 

void Offset_Widget_Pos(LCUI_Widget *widget, LCUI_Pos offset);
/* ���ܣ��Բ���ԭ�е�λ��Ϊ����������ָ����ƫ������ƫ��λ�� */ 

void Move_Widget_To_Pos(LCUI_Widget *widget, LCUI_Pos des_pos, int speed);
/* 
 * ���ܣ���������ָ�����ٶ���ָ��λ���ƶ� 
 * ˵����des_pos��Ŀ��λ�ã�speed�Ǹò������ƶ��ٶȣ���λΪ������/��
 * */ 

void Refresh_Widget(LCUI_Widget *widget);
/* ���ܣ�ˢ����ʾָ����������������ͼ�� */ 

void Resize_Widget(LCUI_Widget *widget, LCUI_Size new_size);
/* ���ܣ��ı䲿���ĳߴ� */ 

void Draw_Widget(LCUI_Widget *widget);
/* ���ܣ����»��Ʋ��� */ 

void Front_Widget(LCUI_Widget *widget);
/* ���ܣ���ָ����������ʾλ���ƶ�����ǰ�� */ 

void Show_Widget(LCUI_Widget *widget);
/* ���ܣ���ʾ���� */ 

void Hide_Widget(LCUI_Widget *widget);
/* ���ܣ����ز��� */ 

void Set_Widget_Status(LCUI_Widget *widget, int status);
/* ���ܣ��趨������״̬ */ 
/************************* Widget End *********************************/


/**************************** Widget Update ***************************/ 
int Processing_WidgetUpdate(LCUI_Widget *widget);
/* ���ܣ��������ĸ��� */ 

void Processing_All_WidgetUpdate();
/* ���ܣ��������в����ĸ��� */ 
/************************ Widget Update End ***************************/



/************************** Widget Library ****************************/

int WidgetFunc_Add(
			char *type, 
			void (*widget_func)(LCUI_Widget*), 
			FuncType func_type
		);
/*
 * ���ܣ�Ϊָ�����͵Ĳ������������͵ĺ���
 * ����ֵ���������Ͳ����ڣ�����-1���������󷵻�-2
 **/ 

int WidgetType_Add(char *type);
/*
 * ���ܣ����һ���µĲ���������������
 * ����ֵ�������ӵ��²��������Ѵ��ڣ�����-1���ɹ��򷵻�0
 **/ 

void WidgetLib_Init(LCUI_Queue *w_lib);
/* ���ܣ���ʼ�������� */ 

int WidgetType_Delete(char *type);
/* ���ܣ�ɾ��ָ���������͵�������� */ 

void NULL_Widget_Func(LCUI_Widget *widget);
/*
 * ���ܣ��պ����������κβ���
 * ˵���������ȡָ���������͵ĺ���ָ��ʧ�ܣ���������������ĺ���ָ��
 **/ 

LCUI_ID WidgetType_Get_ID(char *widget_type);
/* ���ܣ���ȡָ�����Ͳ���������ID */ 

int Get_Widget_Type_By_ID(LCUI_ID id, char *widget_type);
/* ���ܣ���ȡָ������ID���������� */ 

void ( *Get_WidgetFunc_By_ID(LCUI_ID id, FuncType func_type) ) (LCUI_Widget*);
/* ���ܣ���ȡָ����������ID�ĺ����ĺ���ָ�� */ 

void ( *Get_WidgetFunc(char *widget_type, FuncType func_type) ) (LCUI_Widget*);
/* ���ܣ���ȡָ�����Ͳ����ĺ����ĺ���ָ�� */ 

int Check_WidgetType(char *widget_type);
/* ���ܣ����ָ�����������Ƿ���Ч */

void Register_Default_Widget_Type();
/* ���ܣ�Ϊ����Ĳ��������Ĭ�ϵĲ������� */ 
/************************ Widget Library End **************************/


/*************************** Container ********************************/
void Widget_Container_Add(LCUI_Widget *container, LCUI_Widget *widget);
/* ���ܣ��������������Ϊ�����Ĳ����� */ 
/************************* Container End ******************************/

#endif
