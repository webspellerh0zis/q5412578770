/* ***************************************************************************
 * LCUI_Label.c -- LCUI's Label widget
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
 * LCUI_Label.c -- LCUI ���ı���ǩ����
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
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_MEM_H
#include LC_MISC_H
#include LC_FONT_H
#include LC_GRAPHICS_H
#include LC_LABEL_H

#include <stdarg.h>

static void Label_Init(LCUI_Widget *widget)
/* ���ܣ���ʼ��label�������� */
{ 
	LCUI_Label *label;
	label = (LCUI_Label*)Malloc_Widget_Private(widget, sizeof(LCUI_Label));

	label->auto_size	= IS_TRUE; 				/* �����Զ������ߴ� */
	label->image		= NULL;
	label->image_align	= ALIGN_MIDDLE_CENTER;	/* ���뷽ʽΪ���� */
	label->contents		= NULL; 
	label->rows			= 0;					/* ������0 */
	label->text_align	= ALIGN_TOP_LEFT;		/* �ı����뷽ʽΪ���ϽǶ��� */  
	
	Font_Init(&label->font); /* ��ʼ��������Ϣ */
	String_Init(&label->text);/* ��ʼ���ַ��� */
}

static void Destroy_Label(LCUI_Widget *widget)
/* ���ܣ��ͷ�label����ռ�õ���Դ */
{
	LCUI_Label *label;
	label = (LCUI_Label*)Get_Widget_Private_Data(widget);
	Free_WString(label->contents);
	label->contents = NULL;
	Free_String(&label->text);
}

static void Exec_Update_Label(LCUI_Widget *widget)
/* ���ܣ�ִ�и���label�����Ĳ��� */
{
    int len;
	int i, k, refresh_flag = 0;
	int max_height = 0, max_width = 0;
	
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget); 
	
	len = label->text.size; 
	
	if(len <= 0)
	{/* �������Ϊ�� */
		if(label->auto_size == IS_TRUE) 
			/*����������Զ�������С*/
			Resize_Widget(widget, Size(0, 0)); 
			
		widget->graph.flag = HAVE_ALPHA; 
	}
	else
	{
		/* ����һ�����ڱ���ָ���wchar_t���ַ�����LCUI_Wchar_T��ָ�� */
		LCUI_WString *new_list; 
		/* �����������ڱ����ַ������������ͱ��� */
		int old_rows, new_rows; 
		/* �ָ��wchar_t������ */ 
		new_rows = String_To_List(label->text.string , &new_list); 
		/* ��ȡ֮ǰ�ı������� */
		old_rows = label->rows;
		
		if(old_rows > 0) /* �ı��ڴ�ռ��С */
            label->contents = (LCUI_WString *)realloc(label->contents, 
										sizeof(LCUI_WString) * new_rows); 
		else 
			label->contents = (LCUI_WString *)calloc(new_rows, 
												sizeof(LCUI_WString));
		
		for(i = 0; i < new_rows; ++i)
		{/* ����ÿһ�� */
			/* �ı���һ������ռ���ڴ�ռ��С */
			if(old_rows > i)
				label->contents[i].string = (LCUI_WChar_T*)realloc(
								label->contents[i].string, 
								sizeof(LCUI_WChar_T) * 
								(new_list[i].size + 1));
			else
			{
				label->contents[i].string = (LCUI_WChar_T*)malloc(
							sizeof(LCUI_WChar_T) * 
							(new_list[i].size + 1));
				label->contents[i].size = 0;
			}
			
			for(k = 0; k < new_list[i].size; ++k)
			{/* �������е��ַ�����ÿ��Ԫ�� */
				if(new_list[i].string[k].color_type == DEFAULT)
				/* ʹ��Ĭ�ϵ���ɫ */
					label->contents[i].string[k].color = label->font.fore_color;
				else  /* ����ʹ���Զ�����ɫ */
					label->contents[i].string[k].color = new_list[i].string[k].color; 
				
				if(k < label->contents[i].size)
				{/* ����ַ���λ�û���label->contents[i]�ķ�Χ�� */
					if(label->contents[i].string[k].char_code != new_list[i].string[k].char_code)
					{/* ������ֲ�һ��������Ҫˢ�¸��� */
						label->contents[i].string[k].char_code = new_list[i].string[k].char_code; 
						/* ��ȡ�ַ�λͼ */ 
						Get_WChar_Bitmap(&label->font, 
								label->contents[i].string[k].char_code , 
								&label->contents[i].string[k].bitmap );
                        label->contents[i].update = IS_TRUE;/* ��һ���ı���Ҫˢ�� */
                        ++refresh_flag;/* ���ڱ�ʶlabel�����е��ı������Ƿ�ı� */
					}
				}
				else
				{/* ����˵��new_list[i]��Ԫ�ظ�����label->contents[i]�Ķ� */
					label->contents[i].string[k].char_code = new_list[i].string[k].char_code; 
					/* �ȳ�ʼ�����ݽṹ��Ȼ���ȡ�ַ�λͼ */
					Bitmap_Init(&label->contents[i].string[k].bitmap);
					Get_WChar_Bitmap(&label->font, 
							label->contents[i].string[k].char_code , 
							&label->contents[i].string[k].bitmap );
					label->contents[i].update = IS_TRUE;
                    ++refresh_flag;/* ���ڱ�ʶlabel�����е��ı������Ƿ�ı� */
				} 
			}
			/* �ĳ��µ��ַ������� */
			label->contents[i].size = new_list[i].size;
			/* �ͷ��ڴ棬���е�����û���� */
			free(new_list[i].string);
		}
		/* �ͷ��ڴ棬new_list�Ѿ�����Ҫ���� */
		free(new_list);
		new_list = NULL;
		if(refresh_flag > 0)
		{
			/* �����ı����ݵĳߴ� */
			label->rows = new_rows;
			Count_Contents_Size(label->contents, &max_width, &max_height, 
					label->rows, label->font.space, label->font.linegap);
			if(label->auto_size == IS_TRUE 
			&& max_width != widget->size.w 
			&& max_height != widget->size.h)
			{/* ����������Զ�������С,���ҳߴ��иı� */
				Resize_Widget(widget, Size(max_width, max_height + 2));
				Refresh_Widget(widget);
			}
		}
	}
	
	int flag;
    if(Valid_Graph(label->image))
	{ /* ����б�ǩͼ����Ҫ��ʾ */
        if(Valid_Graph(&widget->background_image))
            flag = label->image_align | GRAPH_MIX_FLAG_OVERLAY; /* ����ģʽ */ 
        else 
			flag = label->image_align | GRAPH_MIX_FLAG_REPLACE; /* �滻ģʽ */
        /* ���ݶ��뷽ʽ�Լ�����ģʽ������ͼ�� */
		Align_Image(&widget->graph, label->image, flag); 
	}
    else
    {
        if(!Valid_Graph(&widget->background_image)) 
			flag = GRAPH_MIX_FLAG_REPLACE; /* �滻ģʽ */
        else 
			flag = GRAPH_MIX_FLAG_OVERLAY; /* ����ģʽ */
    }
	/* �ϳ�����λͼ */
	Mix_Widget_FontBitmap(widget, 1, 1, label->contents, label->rows, 
							label->font.space, label->font.linegap, flag);
}

void Set_Label_Image(LCUI_Widget *widget, LCUI_Graph *img, LCUI_Align align)
/* 
 * ���ܣ�����label�����ı���ͼ��
 * ����˵����
 * widget ����Ҫ���в����Ĳ���
 * img    ����Ҫ��Ϊ����ͼ��ͼ������
 * align  ������ͼ��Ĳ���
 * */
{
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget);
    
	if(img != NULL)
	{
		if(Valid_Graph(img))
		{
			label->image = img;
			label->image_align = align;
			Draw_Widget(widget);
		}
		else label->image = NULL;
	}
}

int Get_Label_Row_Len(LCUI_Widget *widget, int row)
/*
 * ���ܣ���ȡlabel������ָ���е��ַ�������
 * ����˵����
 * widget ����Ҫ���в����Ĳ���
 * row   : �ڼ���
 * ����ֵ��ʧ���򷵻�-2���ɹ����س��ȣ����Ͳ�������-1
 * */
{
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget);
	
	if(row > 0 && row <= label->rows) 
		return label->contents[row - 1].size; 
	return -2;
}

void Set_Label_Text(LCUI_Widget *widget, const char *fmt, ...)
/* ���ܣ��趨���ǩ�������ı����� */
{
	char text[LABEL_TEXT_MAX_SIZE];
    memset(text, 0, sizeof(text)); 
    
	LCUI_Label *label;
	
	label = (LCUI_Label*)Get_Widget_Private_Data(widget);
	
    va_list ap;
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE, fmt, ap);
	va_end(ap); 
	 
	Strcpy(&label->text, text);/* �����ַ��� */ 
	Draw_Widget(widget);/* ���²��� */ 
}

int Set_Label_Font(LCUI_Widget *widget, int font_size, char *font_file)
/*
 * ���ܣ�Ϊ��ǩ�����趨�����С���������͡�������ɫ
 * ����˵����
 * widget    ����Ҫ���в����Ĳ���
 * font_file �������ļ���λ�ã����Ϊ����·��
 * color     ���������ɫ��Ҳ����������ʾ����ɫ
 * */
{
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget);
    
	int refresh_flag = 0;
	if(label->font.size != font_size)
	{
		label->font.size = font_size;
		refresh_flag = 1;
	}
	if(NULL != font_file)
	{
		if(strcmp(label->font.font_file.string, font_file) != 0)
		{
			Open_Fontfile(&label->font, font_file);
			Strcpy(&label->font.font_file, font_file);
			refresh_flag = 1;
		}
	}
	if(refresh_flag == 1)
    {/* ˢ�±�ǩ�ڵ�����λͼ���� */
		Set_Label_Text(widget, label->text.string);   
		Refresh_Widget(widget);
	}
	return 0;
}

void Set_Label_Font_Default_Color(LCUI_Widget *widget, LCUI_RGB color)
/*
 * ���ܣ�����label�����������ı���������ɫ��������ɫΪRGB��ɫ�Ļ��ɫ
 * ����˵����
 * label ����Ҫ���в�����label����
 * color : ��ɫ
 * */
{
	int i, k; 
    
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget);
    
    label->font.fore_color = color;

	if(label->contents != NULL && label->rows > 0)
	{
		for(i = 0; i < label->rows; ++i)
		{/* ����ÿһ�� */
			for(k = 0; k < label->contents[i].size; ++k)
			{/* �������е��ַ�����ÿ��Ԫ�� */
                if(label->contents[i].string[k].color_type == DEFAULT)
                {/* ����ı���label������Ĭ�ϵ�������ɫ����ô��Щʹ��ȱʡ��ɫ�����ֽ�ˢ�� */
                    label->contents[i].string[k].color = label->font.fore_color; 
                }
			}
            label->contents[i].update = IS_TRUE;
		}
		Draw_Widget(widget);/* ���²��� */
	} 
}

void Register_Label()
/* ���ܣ�ע�Ჿ������-�ı���ǩ�������� */
{
	/* ��Ӽ����������� */
	WidgetType_Add("label");
	
	/* Ϊ�������͹�����غ��� */
	WidgetFunc_Add("label",	Label_Init,			FUNC_TYPE_INIT);
	WidgetFunc_Add("label",	Exec_Update_Label,	FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("label", Destroy_Label,		FUNC_TYPE_DESTROY);
}
