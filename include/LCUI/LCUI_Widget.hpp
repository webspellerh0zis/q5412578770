/* ***************************************************************************
 * LCUI_Widget.hpp -- C++ class for GUI widget 
 * 
 * Copyright (C) 2013 by
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
 * LCUI_Widget.hp -- GUI������C++��
 *
 * ��Ȩ���� (C) 2013 ������
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

#ifndef __LCUI_WIDGET_HPP__
#define __LCUI_WIDGET_HPP__

#ifdef __cplusplus
class LCUIWidget
{
public: 
	LCUIWidget( const char* widget_type );
	~LCUIWidget(void);

	LCUI_Widget *getWidget( void );
	LCUI_Size getSize( void );
	int getHeight( void );
	int getWidth( void );
	LCUI_Rect getRect( void );
	LCUI_Pos getPos( void );
	void *getPrivateData( void );
	int InvalidateArea( LCUI_Rect );
	void show( LCUI_BOOL );
	void enable( LCUI_BOOL );
	void modal( LCUI_BOOL );
	void move( LCUI_Pos );
	void resize( LCUI_Size );
	void dock( DOCK_TYPE );
protected:
	LCUI_Widget *widget;
};

LCUIWidget::LCUIWidget( const char *widget_type )
{
	widget = Widget_New( widget_type );
}

LCUIWidget::~LCUIWidget(void)
{
	Widget_Destroy( widget );
}

LCUI_Widget *LCUIWidget::getWidget( void )
{
	return widget;
}

LCUI_Size LCUIWidget::getSize( void )
{
	return Widget_GetSize( widget );
}

int LCUIWidget::getHeight( void )
{
	return Widget_GetHeight( widget );
}

int LCUIWidget::getWidth( void )
{
	return Widget_GetWidth( widget );
}

LCUI_Rect LCUIWidget::getRect( void )
{
	return Widget_GetRect( widget );
}

LCUI_Pos LCUIWidget::getPos( void )
{
	return Widget_GetPos( widget );
}

void *LCUIWidget::getPrivateData( void )
{
	return Widget_GetPrivData( widget );
}

int LCUIWidget::InvalidateArea( LCUI_Rect area )
{
	return Widget_InvalidArea( widget, area );
}

void LCUIWidget::show( LCUI_BOOL need_show )
{
	if(need_show) {
		Widget_Show( widget );
	} else {
		Widget_Hide( widget );
	}
}

void LCUIWidget::modal( LCUI_BOOL is_modal )
{
	Widget_SetModal( widget, is_modal );
}

void LCUIWidget::dock( DOCK_TYPE dock_type )
{
	Widget_SetDock( widget, dock_type );
}

void LCUIWidget::move( LCUI_Pos new_pos )
{
	Widget_Move( widget, new_pos );
}

void LCUIWidget::resize( LCUI_Size new_size )
{
	Widget_Resize( widget, new_size );
}

#endif

#endif
