/* ***************************************************************************
 * LCUI_Window.h -- C++ class and function for window widget
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
 * LCUI_Window.h -- ���ڲ�����C++��ͺ���
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
#ifndef __LCUI_WINDOW_HPP_
#define __LCUI_WINDOW_HPP_ 

LCUIWidget::_Window::_Window( LCUIWidget *object )
{
	parent = object;
}

int LCUIWidget::_Window::setTitle( const char *text )
{
	if( parent->isError() ) {
		return WIDGET_IS_NULL;
	} 
	Set_Window_Title_Text(parent->getWidget(), text);
	return 0;
}

int LCUIWidget::_Window::setIcon( LCUIGraph &icon )
{
	if( parent->isError() ) {
		return WIDGET_IS_NULL;
	} 
	Set_Window_Title_Icon(parent->getWidget(), &icon.graph);
	return 0;
}

int LCUIWidget::_Window::setIcon( LCUI_Graph *icon )
{
	if( parent->isError() ) {
		return WIDGET_IS_NULL;
	} 
	Set_Window_Title_Icon(parent->getWidget(), icon);
	return 0;
}

int LCUIWidget::_Window::addToClientArea( LCUIWidget &object )
{
	if( parent->isError() ) {
		return WIDGET_IS_NULL;
	}
	Window_Client_Area_Add(parent->getWidget(), object.getWidget());
	return 0;
}
#endif





