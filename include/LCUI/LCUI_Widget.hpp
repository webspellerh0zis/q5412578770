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

#ifndef __LCUI_WIDGET_HPP__
#define __LCUI_WIDGET_HPP__

#ifdef __cplusplus

#define WIDGET_IS_NULL -5

#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_GRAPHICS_H

class LCUIWidget
{
	private:
	int error;
	LCUI_Widget **widget;
	
	public: 
	LCUIWidget(const char* widget_type):Label(this),Window(this)
	{
		error = 0;
		widget = new (LCUI_Widget *);
		*widget = Create_Widget(widget_type);
		if(*widget == NULL){
			error = 1;
		}
	}
	~LCUIWidget()
	{
		delete widget;
	}
	int isError()
	{
		return error;
	}
	LCUI_Widget *getWidget()
	{
		return *widget;
	}
	int resize(LCUI_Size new_size)
	{
		if( error ) return WIDGET_IS_NULL;
		Resize_Widget( getWidget() , new_size);
		return 0;
	}
	int move(LCUI_Pos new_pos)
	{
		if(error) return WIDGET_IS_NULL;
		Move_Widget( getWidget() , new_pos);
		return 0;
	}
	int show()
	{
		if(error) return WIDGET_IS_NULL;
		Show_Widget( getWidget() );
		return 0;
	}
	int hide()
	{
		if(error) return WIDGET_IS_NULL;
		Hide_Widget( getWidget() );
		return 0;
	}
	int refresh()
	{
		if(error) return WIDGET_IS_NULL;
		Refresh_Widget( getWidget() );
		return 0;
	}
	int draw()
	{
		if(error) return WIDGET_IS_NULL;
		Draw_Widget( getWidget() );
		return 0;
	}
	int enable()
	{
		if(error) return WIDGET_IS_NULL;
		Enable_Widget( getWidget() );
		return 0;
	}
	int disable()
	{
		if(error) return WIDGET_IS_NULL;
		Disable_Widget( getWidget() );
		return 0;
	}
	int setAlpha(unsigned char alpha)
	{
		if(error) return WIDGET_IS_NULL;
		Set_Widget_Alpha( getWidget() , alpha);
		return 0;
	}
	int setAlign(LCUI_Align align, LCUI_Pos offset_pos)
	{
		if(error) return WIDGET_IS_NULL; 
		Set_Widget_Align( getWidget() , align, offset_pos);
		return 0;
	}
	int setBorder(LCUI_RGB color, LCUI_Border border)
	{
		if(error) return WIDGET_IS_NULL;
		Set_Widget_Border( getWidget() , color, border);
		return 0;
	}
	int setBackgroundImage(LCUI_Graph *img, int flag)
	{
		if(error) return WIDGET_IS_NULL;
		return Set_Widget_Background_Image( getWidget() , img, flag); 
	}
	
	/* label widget */
	class _Label
	{
		public: 
		_Label(LCUIWidget *object)
		{
			parent = object; 
		}
		int setText(const char *fmt, ...)
		{
			if( parent->isError() ) {
				return WIDGET_IS_NULL;
			}
			
			char text[LABEL_TEXT_MAX_SIZE];
			memset(text, 0, sizeof(text)); 
			
			LCUI_Label *p_label;
			p_label = (LCUI_Label*)Get_Widget_Private_Data(parent->getWidget());
			
			va_list ap;
			va_start(ap, fmt);
			vsnprintf(text, LABEL_TEXT_MAX_SIZE, fmt, ap);
			va_end(ap); 
			 
			Strcpy( &p_label->text, text );/* �����ַ��� */ 
			Draw_Widget( parent->getWidget() );/* ���²��� */ 
			return 0;
		}
		int setFont(int font_pixel_size, char *fontfile)
		{
			if( parent->isError() ) {
				return WIDGET_IS_NULL;
			}
			return Set_Label_Font(parent->getWidget(), font_pixel_size, fontfile);
		}
		private:
		LCUIWidget *parent; 
	};
	_Label Label;
	/* label widget end */ 
	
	/* window widget */
	class _Window
	{ 
		public: 
		_Window( LCUIWidget *object )
		{
			parent = object;
		}
		int setTitle( const char *text )
		{
			if( parent->isError() ) {
				return WIDGET_IS_NULL;
			} 
			Set_Window_Title_Text(parent->getWidget(), text);
			return 0;
		}
		int setIcon( LCUIGraph &icon )
		{
			if( parent->isError() ) {
				return WIDGET_IS_NULL;
			} 
			Set_Window_Title_Icon(parent->getWidget(), &icon.graph);
			return 0;
		}
		int setIcon( LCUI_Graph *icon )
		{
			if( parent->isError() ) {
				return WIDGET_IS_NULL;
			} 
			Set_Window_Title_Icon(parent->getWidget(), icon);
			return 0;
		}
		int addToClientArea(LCUIWidget &object)
		{
			if( parent->isError() ) {
				return WIDGET_IS_NULL;
			}
			Window_Client_Area_Add(parent->getWidget(), object.getWidget());
			return 0;
		}
		
		private:
		LCUIWidget *parent; 
	}; 
	_Window Window;
	/* window widget end */
};

#endif

#endif
