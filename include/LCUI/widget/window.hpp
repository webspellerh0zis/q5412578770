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

/* LCUIWindow��̳���LCUIWidget�� */
class LCUIWindow:public LCUIWidget
{
public:
	LCUIWindow():LCUIWidget("window"){};
	void setTitleIcon( LCUI_Graph* );
	void setTitleIcon( LCUIGraph & );
	void setTitleText( const char* );
	void addToTitleBar( LCUIWidget& );
	void addToClientArea( LCUIWidget& );
};

void LCUIWindow::setTitleIcon( LCUI_Graph *icon )
{
	Set_Window_Title_Icon( getWidget(), icon );
}

void LCUIWindow::setTitleIcon( LCUIGraph &icon )
{
	Set_Window_Title_Icon( getWidget(), icon.getGraph() );
}

void LCUIWindow::setTitleText( const char *text )
{
	Set_Window_Title_Text( getWidget(), text );
}

void LCUIWindow::addToTitleBar( LCUIWidget &obj )
{
	Window_TitleBar_Add( getWidget(), obj.getWidget() );
}

void LCUIWindow::addToClientArea( LCUIWidget &obj )
{
	Window_Client_Area_Add( getWidget(), obj.getWidget() );
}
#endif





