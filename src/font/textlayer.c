/* ***************************************************************************
 * textlayer.c -- text bitmap layer processing module.
 * 
 * Copyright (C) 2012-2014 by
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
 * textlayer.c -- �ı�ͼ�㴦��ģ��
 *
 * ��Ȩ���� (C) 2012-2014 ������
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
#include LC_GRAPH_H
#include LC_FONT_H

static void TaskData_Init( TaskData *task )
{
        task->redraw_all = 0;
        task->update_bitmap = 0;
        task->update_typeset = 0;
        task->typeset_start_row = 0;
}

/** ��� �����ı��Ű� ������ */
static inline void TaskData_AddUpdateTypeset( TaskData *task, int start_row )
{
        if( start_row < task->typeset_start_row ) {
                task->typeset_start_row = start_row;
        }
        task->update_typeset = TRUE;
}

LCUI_API void TextLayer_Init( LCUI_TextLayer *layer )
{
        layer->max_width = 0;
        layer->max_height = 0;
        layer->offset_x = 0;
        layer->offset_y = 0;
	layer->new_offset_x = 0;
	layer->new_offset_y = 0;
        layer->insert_x = 0;
        layer->insert_y = 0;
        layer->is_mulitiline_mode = FALSE;
        layer->is_wordwrap_mode = FALSE;
	layer->is_using_style_tags = FALSE;
        layer->text_align = TEXT_ALIGN_LEFT;

        layer->row_list.max_rows = 0;
        layer->row_list.rows = 0;
        layer->row_list.rowdata = 0;
        
        TextStyle_Init( &layer->text_style );
        TaskData_Init( &layer->task );

        layer->is_using_buffer = TRUE;
	Graph_Init( &layer->graph );
}

static void TextRow_Init( TextRowData* p_row )
{
        p_row->bottom_spacing = 1;
        p_row->top_spacing = 1;
        p_row->max_height = 0;
        p_row->max_width = 0;
        p_row->string = NULL;
        p_row->string_len = 0;
        p_row->string_max_len = 0;
}

static inline TextRowData* 
TextRowList_GetRow( TextRowList *p_row_list, int n_row )
{
        if( n_row >= p_row_list->rows ) {
                return NULL;
        }
        return p_row_list->rowdata[n_row];
}

static inline TextRowData* 
TextRowList_AddNewRow( TextRowList *rowlist )
{
        TextRowData *row_ptr, **new_rowlist;
        
        ++rowlist->rows;
        if( rowlist->max_rows <= rowlist->rows ) {
                new_rowlist = (TextRowData**)realloc( rowlist->rowdata, 
			sizeof(TextRowData*)*(rowlist->rows+1) );
		if( !new_rowlist ) {
                        --rowlist->rows;
                        return NULL;
                }
                rowlist->rowdata = new_rowlist;
                rowlist->max_rows = rowlist->rows;
        }
        
        row_ptr = (TextRowData*)malloc( sizeof(TextRowData) );
        if( !row_ptr ) {
                return NULL;
        }
        TextRow_Init( row_ptr );
        rowlist->rowdata[rowlist->rows-1] = row_ptr;
        return row_ptr;
}

static TextRowData* 
TextRowList_InsertNewRow( TextRowList *rowlist, int n_row )
{
        int i;
        TextRowData *row_ptr, **new_rowlist;
        
        if( n_row > rowlist->rows ) {
                return NULL;
        }
        ++rowlist->rows;
        if( rowlist->max_rows <= rowlist->rows ) {
                new_rowlist = (TextRowData**)realloc( rowlist->rowdata, 
			sizeof(TextRowData*)*(rowlist->rows+1) );
                if( !new_rowlist ) {
                        --rowlist->rows;
                        return NULL;
                }
                rowlist->rowdata = new_rowlist;
                rowlist->max_rows = rowlist->rows;
        }

        row_ptr = (TextRowData*)malloc( sizeof(TextRowData) );
        if( !row_ptr ) {
                return NULL;
        }
        TextRow_Init( row_ptr );
        for( i=rowlist->rows-1; i>n_row; --i ) {
                rowlist->rowdata[i] = rowlist->rowdata[i-1];
        }
        rowlist->rowdata[n_row] = row_ptr;
        return row_ptr;
}

/** ���ı����б���ɾ��ָ���ı��� */
static int TextRowList_RemoveRow( TextRowList *p_rowlist, int row )
{
        if( row < 0 || row >= p_rowlist->rows ) {
                return -1;
        }
        for( ; row < p_rowlist->rows; ++row ) {
                p_rowlist->rowdata[row] = p_rowlist->rowdata[row+1];
        }
        p_rowlist->rowdata[row] = NULL;
        --p_rowlist->rows;
        return 0;
}

/** �����ı��еĳߴ� */
static void TextRow_UpdateSize( TextRowData *p_row, int default_height )
{
        int char_h, i;
        TextCharData* p_char;

        p_row->max_width = 0;
        p_row->max_height = default_height;
        for( i=0; i<p_row->string_len; ++i ) {
                p_char = p_row->string[i];
		if( !p_char->bitmap || !p_char->need_display ) {
                        continue;
                }
		p_row->max_width += p_char->bitmap->advance.x;
                if( p_char->style && p_char->style->_pixel_size ) {
                        char_h = p_char->style->pixel_size+2;
                } else {
                        char_h = default_height;
                }

                if( p_row->max_width < char_h ) {
                        p_row->max_width = char_h;
                }
        }
}

/** �����ı��е��ַ������� */
static int TextRow_SetLength( TextRowData *p_row, int new_len )
{
        TextCharData **p_new_str;

        if( new_len < 0 ) {
                new_len = 0;
        }
        p_row->string_len = new_len;
        if( new_len <= p_row->string_max_len ) {
		return 0;
	}

        p_new_str = (TextCharData**)
	realloc( p_row->string, sizeof(TextCharData*)*new_len );
        if( !p_new_str ) {
                --p_row->string_len;
                return -1;
        }
        p_row->string = p_new_str;
        p_row->string_max_len = p_row->string_len;
        return 0;
}

/** ���ı��в���һ���ַ� */
static int 
TextRow_Insert( TextRowData *p_row, int insert_pos, TextCharData *p_char )
{
        int i;
        TextCharData **p_new_str;
        
        if( insert_pos > p_row->string_len ) {
                insert_pos = p_row->string_len;
        }
        ++p_row->string_len;
        if( p_row->string_max_len <= p_row->string_len ) {
                p_new_str = (TextCharData**)realloc( p_row->string, 
			sizeof(TextCharData*)*p_row->string_len );
                if( !p_new_str ) {
                        --p_row->string_len;
                        return -1;
                }
                p_row->string = p_new_str;
                p_row->string_max_len = p_row->string_len;
        }
        for( i=p_row->string_len-1; i>=insert_pos; --i ) {
                p_row->string[i] = p_row->string[i-1];
        }
        p_row->string[insert_pos] = p_char;
        return 0;
}

/* ���ı����е����������ƶ� */
static void TextRow_LeftMove( TextRowData *p_row, int n )
{
        int i, j, m;

        if( n <= 0 ) {
                return;
        }
        if( n > p_row->string_len ) {
                n = p_row->string_len;
        }
        m = p_row->string_len - n;
        for( i=0,j=n; i<m; ++i,++j ) {
                p_row->string[i] = p_row->string[j];
        }
        p_row->string_len = m;
}

static inline LCUI_BOOL TextRow_HasEndChar( TextRowData *p_row )
{
	if( p_row->string_len < 1 ) {
		return FALSE;
	}
	if( p_row->string[p_row->string_len-1]->char_code == L'\n' ) {
                return TRUE;
        }
	return FALSE;
}

/** ��������λͼ */
static inline void TextChar_UpdateBitmap( TextCharData* p_data, LCUI_TextStyle *default_style )
{
        p_data->bitmap = FontLIB_GetExistFontBMP( p_data->char_code, 
			default_style->font_id, default_style->pixel_size );
        //printf("char_code: %c, pixel_size: %d, font_id: %d, bitmap: %p\n", 
        //p_data->char_code, default_style->pixel_size, default_style->font_id, p_data->bitmap);
}

/** ���ò������������� */
LCUI_API void TextLayer_SetCaretPos( LCUI_TextLayer* layer, int row, int col )
{
        if( row < 0 ) {
                row = 0;
        }
        else if( row >= layer->row_list.rows ) {
                if( layer->row_list.rows < 0 ) {
                        row = 0;
                } else  {
                        row = layer->row_list.rows-1;
                }
        }

        if( col < 0 ) {
                col = 0;
        }
        else if( layer->row_list.rows > 0 ) {
                if( col >= layer->row_list.rowdata[row]->string_len ) {
                        col = layer->row_list.rowdata[row]->string_len;
                }
        } else {
                col = 0;
        }
        layer->insert_x = col;
        layer->insert_y = row;
}

/** �����������������ı������������� */
LCUI_API int TextLayer_SetCaretPosByPixelPos( LCUI_TextLayer* layer, int x, int y )
{
        TextRowData* p_row;
        int i, pixel_pos, ins_x, ins_y;

        for( pixel_pos=0, i=0; i<layer->row_list.rows; ++i ) {
                p_row = layer->row_list.rowdata[i];
                pixel_pos += p_row->top_spacing;
                pixel_pos += p_row->max_height;
                pixel_pos += p_row->bottom_spacing;
                if( pixel_pos >= y ) {
                        ins_y = i;
                        break;
                }
        }
        if( i >= layer->row_list.rows ) {
                if( layer->row_list.rows > 0 ) {
                        ins_y = layer->row_list.rows-1;
                } else {
                        layer->insert_x = 0;
                        layer->insert_y = 0;
                        return -1;
                }
        }
        p_row = layer->row_list.rowdata[ins_y];
        ins_x = p_row->string_len;
        /* �����ı����뷽ʽ��ȷ����ǰ���ı�����ʼ���� */
        switch( layer->text_align ) {
        case TEXT_ALIGN_CENTER:
                pixel_pos = (layer->max_width - p_row->max_width)/2;
                break;
        case TEXT_ALIGN_RIGHT:
                pixel_pos = layer->max_width - p_row->max_width;
                break;
        case TEXT_ALIGN_LEFT:
        default:
                pixel_pos = 0;
                break;
        }
        for( i=0; i<p_row->string_len; ++i ) {
                if( !p_row->string[i]->bitmap || !p_row->string[i]->need_display ) {
                        continue;
                }
                pixel_pos += p_row->string[i]->bitmap->advance.x;
                /* ����ڵ�ǰ�����ĵ��ǰ�� */
                if( x <= pixel_pos - p_row->string[i]->bitmap->advance.x/2 ) {
                        ins_x = i;
                        break;
                }
        }
        TextLayer_SetCaretPos( layer, ins_y, ins_x );
	return 0;
}

/** ��ȡָ�����е����ֵ��������� */
LCUI_API int TextLayer_GetCharPixelPos( LCUI_TextLayer* layer, int row,
						int col, LCUI_Pos *pixel_pos )
{
        TextRowData* p_row;
        int i, pixel_x = 0, pixel_y = 0;

        if( row < 0 || row >= layer->row_list.rows ) {
                return -1;
        }
        if( col < 0 ) {
                return -2;
        }
        else if( col > layer->row_list.rowdata[row]->string_len ) {
                return -3;
        }
        /* �ۼ�ǰ���еĸ߶� */
        for( i=0; i<row-1; ++i ) {
                p_row = layer->row_list.rowdata[i];
                pixel_y += p_row->top_spacing;
                pixel_y += p_row->max_height;
                pixel_y += p_row->bottom_spacing;
        }
        p_row = layer->row_list.rowdata[i];
        pixel_y += p_row->top_spacing;

        p_row = layer->row_list.rowdata[row];
        switch( layer->text_align ) {
        case TEXT_ALIGN_CENTER:
                pixel_x = (layer->max_width - p_row->max_width)/2;
                break;
        case TEXT_ALIGN_RIGHT:
                pixel_x = layer->max_width - p_row->max_width;
                break;
        case TEXT_ALIGN_LEFT:
        default:
                pixel_x = 0;
                break;
        }
        for( i=0; i<col; ++i ) {
                if( !p_row->string[i] ) {
                        break;
                }
                if( !p_row->string[i]->bitmap
		 || !p_row->string[i]->need_display ) {
                        continue;
                }
                pixel_x += p_row->string[i]->bitmap->advance.x;
        }
	pixel_pos->x = pixel_x;
        pixel_pos->y = pixel_y;
        return 0;
}

/** ��ȡ�ı������������� */
LCUI_API int TextLayer_GetCaretPixelPos( LCUI_TextLayer *layer, LCUI_Pos *pixel_pos )
{
        return TextLayer_GetCharPixelPos( layer, layer->insert_y, 
						layer->insert_x, pixel_pos );
}

static void DestroyTextRowList( TextRowList *p_list )
{
        int i;
        for( i=0; i<p_list->rows; ++i ) {
                free( p_list->rowdata[i] );
                p_list->rowdata[i] = NULL;
        }
        p_list->max_rows = 0;
        p_list->rows = 0;
}

/** ����ı� */
LCUI_API void TextLayer_Clear( LCUI_TextLayer* layer )
{
	layer->insert_x = 0;
	layer->insert_y = 0;
        DestroyTextRowList( &layer->row_list );
        layer->task.redraw_all = TRUE;
}

static int TextLayer_DoWordWrap( LCUI_TextLayer *layer, int row, int start_col )
{
	int n_cols, char_h;
	TextRowData *p_row, *p_next_row;
	TextCharData *p_char;

	if( row < 0 || row >= layer->row_list.rows ) {
		return -1;
	}
	p_row = layer->row_list.rowdata[row];
	if( p_row->string_len < 1 || start_col >= p_row->string_len-1 ) {
		return -2;
	}
	/* �������ĩβ�л��з������������ */
	if( p_row->string[p_row->string_len-1]->char_code == L'\n' ) {
                p_next_row = TextRowList_InsertNewRow( &layer->row_list, row+1 );
        }
        /* ��ȡ��һ�е�ָ�� */
        p_next_row = TextRowList_GetRow( &layer->row_list, row+1 );
        /* ��û����һ�У���������� */
        if( !p_next_row ) {
                p_next_row = TextRowList_InsertNewRow( &layer->row_list, row+1 );
        }
        p_next_row->max_width = 0;
        p_next_row->max_height = layer->text_style.pixel_size + 2;
        n_cols = p_row->string_len;
        /* ���������ڵ�ǰ�У�����λ�ó����˵�ǰ�У����ƶ�����һ�� */
        if( layer->insert_y == row && layer->insert_x >= p_row->string_len ) {
                ++layer->insert_y;
                ++layer->insert_x;
                layer->insert_x -= p_row->string_len;
        }
        /* ������ʣ������ת������һ�� */
        for( n_cols-=1; n_cols>=start_col; --n_cols ) {
                p_char = p_row->string[n_cols];
                /* �Ƴ������ڱ��еļ�¼ */
                p_row->string[n_cols] = NULL;
                /* ��Ǳ�����Ҫˢ�� */
                p_char->need_update = TRUE;
                /* ���������� */
                TextRow_Insert( p_next_row, 0, p_char );
                if( p_char->style && p_char->style->_pixel_size ) {
                        char_h = p_char->style->pixel_size+2;
                } else {
                        char_h = layer->text_style.pixel_size+2;
                }
                if( p_next_row->max_height < char_h ) {
                        p_next_row->max_height = char_h;
                }
                p_next_row->max_width += p_char->bitmap->advance.x;
        }
        p_row->string_len = start_col;
	return 0;
}

/** ��ָ���е��ı������Ű� */
static void TextLayer_TextRowTypeset( LCUI_TextLayer* layer, int row )
{
        int col, char_h;
        TextRowData *p_row, *p_next_row;
        TextCharData *p_char;

        p_row = layer->row_list.rowdata[row];
        p_row->max_width = 0;
        p_row->max_height = layer->text_style.pixel_size + 2;
        for( col=0; col<p_row->string_len; ++col ) {
                p_char = p_row->string[col];
		/* ����������з� */
		if( layer->is_wordwrap_mode && p_char->char_code == L'\n' ) {
			TextLayer_DoWordWrap( layer, row, col+1 );
			return;
		}
                if( !p_char->bitmap || !p_char->need_display ) {
                        continue;
                }
                if(p_char->style && p_char->style->_pixel_size ) {
                        char_h = p_char->style->pixel_size + 2;
                } else {
                        char_h = layer->text_style.pixel_size + 2;
                }
                /* �ۼ��п�� */
                p_row->max_width += p_char->bitmap->advance.x;
                /* ����ǵ�ǰ�еĵ�һ���ַ��������п��û�г���������� */
                if( layer->max_width <= 0 || !layer->is_wordwrap_mode 
		 || (layer->is_wordwrap_mode && !layer->is_mulitiline_mode)
		 || col < 1 || p_row->max_width <= layer->max_width ) {
                        if( p_row->max_height < char_h ) {
                                p_row->max_height = char_h;
                        }
                        continue;
                }
                /* ���п�Ȼ�ԭ�������ۼ�ǰ��ֵ */
                p_row->max_width -= p_char->bitmap->advance.x;
		TextLayer_DoWordWrap( layer, row, col );
		return;
        }
        /* ��������л��з� */
        if( TextRow_HasEndChar( p_row ) ) {
                return;
        }
        /* ���е��ı����δ�ﵽ���ƿ�ȣ���Ҫ�����е��ı�ת�������� */
        while(1) {
                /* ��ȡ��һ�е�ָ�� */
                p_next_row = TextRowList_GetRow( &layer->row_list, row+1 );
                if( !p_next_row ) {
			return;
                }
                for( col=0; col<p_next_row->string_len; ++col ) {
                        p_char = p_next_row->string[col];
			if( layer->is_wordwrap_mode
			 && p_char->char_code == L'\n' ) {
				TextLayer_DoWordWrap( layer, row, col+1 );
				return;
			}
                        /* ����������λͼ����Ҫ��ʾ������ */
                        if( !p_char->bitmap || !p_char->need_display ) {
                                TextRow_Insert( p_row, p_row->string_len, p_char );
                                p_next_row->string[col] = NULL;
                                continue;
                        }
                        if( p_char->style && p_char->style->_pixel_size ) {
                                char_h = p_char->style->pixel_size+2;
                        } else {
                                char_h = layer->text_style.pixel_size+2;
                        }
                        p_row->max_width += p_char->bitmap->advance.x;
                        /* ���û�г���������� */
                        if( !layer->is_wordwrap_mode || layer->max_width <= 0
			 || (layer->is_wordwrap_mode && !layer->is_mulitiline_mode)
			 || p_row->max_width <= layer->max_width ) {
                                if( p_row->max_height < char_h ) {
                                        p_row->max_height = char_h;
                                }
                                /* ��Ǳ�����Ҫˢ�� */
                                p_char->need_update = 1;
                                TextRow_Insert( p_row, p_row->string_len, p_char );
                                p_next_row->string[col] = NULL;
                                continue;
                        }
                        /* ������������һ�� */
                        if( layer->insert_y == row+1 ) {
                                /* �������㴦�ڱ�ת�Ƶļ��������� */
                                if( layer->insert_x < col ) {
                                        layer->insert_y = row;
                                        layer->insert_x += p_row->string_len;
                                } else {
                                        /* ���򣬼�ȥ��ת�Ƶ������� */
                                        layer->insert_x -= col;
                                }
                        }
                        /* ���򣬳����˿������ */
                        p_next_row->max_width -= p_char->bitmap->advance.x;
                        /* ����һ��ʣ���������ǰ�� */
                        TextRow_LeftMove( p_next_row, col );
                        return;
                }
                /* ɾ����һ�У���Ϊ��һ�е������Ѿ�ת������ǰ�� */
                TextRowList_RemoveRow( &layer->row_list, row+1 );
                /* �������㵱ǰ���ں��� */
                if( layer->insert_y > row ) {
                        --layer->insert_y;
                }
        }
}

/** ��ָ���п�ʼ�����ı������Ű� */
static void TextLayer_TextTypeset( LCUI_TextLayer* layer, int start_row )
{
        int row;
        for( row=start_row; row<layer->row_list.rows; ++row ) {
                TextLayer_TextRowTypeset( layer, row );
        }
}

/** ���ı�����Ԥ���� */ 
static int TextLayer_ProcessText(	LCUI_TextLayer *layer, 
					const wchar_t *new_text,
					TextAddType add_type )
{
        TextRowData *p_cur_row;
        TextCharData *p_char, char_data;
        const wchar_t *p_end, *p, *pp;
        int cur_col, cur_row, ins_x;
	LCUI_Queue tag_buff;

        if( !new_text ) {
                return -1;
        }
        /* ����ǽ��ı�׷�����ı�ĩβ */
        if( add_type == TEXT_ADD_TYPE_APPEND ) {
                if( layer->row_list.rows > 0 ) {
                        cur_row = layer->row_list.rows - 1;
                } else {
                        cur_row = 0;
                }
                p_cur_row = TextRowList_GetRow( &layer->row_list, cur_row );
                if( !p_cur_row ) {
                        p_cur_row = TextRowList_AddNewRow( &layer->row_list );
                }
                cur_col = p_cur_row->string_len;
        } else { /* �����ǽ��ı��������������λ�� */
                cur_row = layer->insert_y;
                cur_col = layer->insert_x;
                p_cur_row = TextRowList_GetRow( &layer->row_list, cur_row );
                if( !p_cur_row ) {
                        p_cur_row = TextRowList_AddNewRow( &layer->row_list );
                }
        }
        ins_x = cur_col;
        StyleTag_Init( &tag_buff );
        p_end = new_text + wcslen(new_text);
        for( p=new_text; p<p_end; ++p, ++ins_x ) {
		if( layer->is_using_style_tags ) {
			/* ������ʽ�Ľ�����ǩ */ 
			pp = StyleTag_ProcessEndingTag( &tag_buff, p );
			if( pp ) {
				p = pp;
			} else {
				/* ������ʽ��ǩ */
				pp = StyleTag_ProcessTag( &tag_buff, p );
				if( pp ) {
					p = pp;
				}
			}
			/* �������ĩβ */
			if( p >= p_end ) {
				break;
			}
		}
		
                char_data.char_code = *p;
                /* ��Ի��з�ģʽΪWin(CR/LF)���ı������д��� */
                if( *p == '\n' || *p == '\r' ) { 
                        char_data.need_display = FALSE;
			char_data.need_update = FALSE;
			char_data.bitmap = NULL;
			/* ��������ǰ�ı����� */
			TextRow_Insert( p_cur_row, ins_x, p_char );
			continue;
                } 
		char_data.need_display = TRUE;
		char_data.need_update = TRUE;
		/* ��ȡ��ǰ�ı���ʽ���� */
		char_data.style = StyleTag_GetCurrentStyle( &tag_buff );
		/* ��������λͼ */
		TextChar_UpdateBitmap( &char_data, &layer->text_style );
        }
        /* ���µ�ǰ�еĳߴ� */
        TextRow_UpdateSize( p_cur_row, layer->text_style.pixel_size+2 );
        if( add_type == TEXT_ADD_TYPE_INSERT ) {
                layer->insert_x = ins_x;
        }
        /* ���������Զ�����ģʽ��������Ҫ���¶��ı������Ű� */
        if( layer->is_wordwrap_mode ) {
                TaskData_AddUpdateTypeset( &layer->task, cur_row );
        }
        return 0;
}

/** �����ı����ݣ����ַ��棩 */
LCUI_API int TextLayer_InsertTextW( LCUI_TextLayer* layer, const wchar_t *unicode_text )
{
        if( !unicode_text ) {
                return -1;
        }
        return TextLayer_ProcessText( layer, unicode_text, TEXT_ADD_TYPE_INSERT );
}

/** �����ı����� */
LCUI_API int TextLayer_InsertTextA( LCUI_TextLayer* layer, const char *ascii_text )
{
        return 0;
}

/** �����ı����ݣ�UTF-8�棩 */
LCUI_API int TextLayer_InsertText( LCUI_TextLayer* layer, const char *utf8_text )
{
        return 0;
}

/** ׷���ı����ݣ����ַ��棩 */
LCUI_API int TextLayer_AppendTextW( LCUI_TextLayer* layer, const wchar_t *unicode_text )
{
        if( !unicode_text ) {
                return -1;
        }
        return TextLayer_ProcessText( layer, unicode_text, TEXT_ADD_TYPE_APPEND );
}

/** ׷���ı����� */
LCUI_API int TextLayer_AppendTextA( LCUI_TextLayer* layer, const char *ascii_text )
{
        return 0;
}

/** ׷���ı����ݣ�UTF-8�棩 */
LCUI_API int TextLayer_AppendText( LCUI_TextLayer* layer, const char *utf8_text )
{
        return 0;
}

/** �����ı����ݣ����ַ��棩 */
LCUI_API int TextLayer_SetTextW( LCUI_TextLayer* layer, const wchar_t *unicode_text )
{
        TextLayer_Clear( layer );
        return TextLayer_AppendTextW( layer, unicode_text );
}

/** �����ı����� */
LCUI_API int TextLayer_SetTextA( LCUI_TextLayer* layer, const char *ascii_text )
{
        return 0;
}

/** �����ı����ݣ�UTF-8�棩 */
LCUI_API int TextLayer_SetText( LCUI_TextLayer* layer, const char *utf8_text )
{
        return 0;
}

/** ��ȡTextWidget�е��ı������ַ��棩 */
LCUI_API int TextLayer_GetTextW( LCUI_TextLayer *layer, int start_pos,
					int max_len, wchar_t *wstr_buff )
{
        int row, col, i;
	TextRowData *row_ptr;

        if( start_pos<0 ) {
                return -1;
        }
        if( max_len <= 0 ) {
                return 0;
        }
	/* �ȸ���һά��������������� */
	for( i=0,row=0; row<layer->row_list.max_rows; ++row ) {
		if( i >= start_pos ) {
			col = start_pos - i;
			break;
		}
		i += layer->row_list.rowdata[row]->string_len;
	}
	for( i=0; row < layer->row_list.max_rows, i < max_len; ++row ) {
		row_ptr = layer->row_list.rowdata[row];
		for( ; col < row_ptr->string_len, i < max_len; ++col,++i ) {
			wstr_buff[i] = row_ptr->string[col]->char_code;
		}
	}
        wstr_buff[i] = L'\0';
        return i;
}

/** ��������ı���� */
LCUI_API int TextLayer_SetMaxWidth( LCUI_TextLayer* layer, int max_width )
{
        if( max_width <= 0 ) {
                return -1;
        }
        if( layer->is_using_buffer && layer->max_height > 0 ) {
		Graph_Create( &layer->graph, max_width, layer->max_height );
        }
        layer->max_width = max_width;
        return 0;
}

/** ��������ı��߶� */
LCUI_API int TextLayer_SetMaxHeight( LCUI_TextLayer* layer, int max_height )
{
        if( max_height <= 0 ) {
                return -1;
        }
        if( layer->is_using_buffer && layer->max_width > 0 ) {
		Graph_Create( &layer->graph, layer->max_width, max_height );
        }
        layer->max_height = max_height;
        return 0;
}

/** �����Ƿ����ö����ı�ģʽ */
LCUI_API void TextLayer_SetMultiline( LCUI_TextLayer* layer, int is_true )
{
        if( layer->is_mulitiline_mode && !is_true
	 || !layer->is_mulitiline_mode && is_true ) {
                layer->is_mulitiline_mode = is_true;
                TaskData_AddUpdateTypeset( &layer->task, 0 );;
        }
}

/** ɾ��ָ�����е����ּ����ұߵ��ı� */
static int TextLayer_DeleteText( LCUI_TextLayer* layer, int char_y, 
						int char_x, int n_char )
{
        int end_x, end_y, i, j, len;
        TextRowData *p_row, *p_end_row;

        if( char_x < 0 ) {
                char_x = 0;
        }
        if( char_y < 0 ) {
                char_y = 0;
        }
        if( n_char <= 0 ) {
                return -1;
        }
        if( char_y >= layer->row_list.rows ) {
                return -2;
        }
        p_row = layer->row_list.rowdata[char_y];
        if( char_x > p_row->string_len ) {
		char_x = p_row->string_len;
        }

        end_x = char_x;
        end_y = char_y;
        --n_char;
        /* ����������λ�� */
	for( end_y=char_y; end_y<layer->row_list.rows && n_char>0; ++end_y ) {
		p_row = layer->row_list.rowdata[end_y];
		if( n_char < p_row->string_len ) {
			end_x = n_char;
			n_char = 0;
			break;
		}
		n_char -= (p_row->string_len - end_x);
	}
        if( end_y >= layer->row_list.rows ) {
                end_y = layer->row_list.rows-1;
        }
        p_end_row = layer->row_list.rowdata[end_y];
        if( end_x > p_end_row->string_len ) {
		end_x = p_end_row->string_len;
        }
        
        /* ������ʼ���������ƴ�Ӻ�ĳ��� */
        len = char_x + p_end_row->string_len - end_x - 1;
        // ��ʼ�У�0 1 2 3 4 5�����λ�ã�2
        // �����У�0 1 2 3 4 5���յ�λ�ã�4
        // ƴ�Ӻ�ĳ��ȣ�2 + 6 - 4 - 1 = 3
        /* �����ͬһ�� */
        if( p_row == p_end_row && end_x < p_end_row->string_len ) {
                for( i=char_x, j=end_x+1; j<p_row->string_len; ++i,++j ) {
                        p_row->string[i] = p_row->string[j];
                }
                /* �����ǰ��Ϊ�գ�Ҳ���ǵ�һ�У����Ƴ���ǰ�� */
                if( len <= 0 && end_y > 0 ) {
                        TextRowList_RemoveRow( &layer->row_list, end_y );
                } else {
                        /* ������ʼ�е����� */
                        TextRow_SetLength( p_row, len );
                        /* �����ı��еĳߴ� */
                        TextRow_UpdateSize( p_row, layer->text_style.pixel_size+2 );
                }
                TaskData_AddUpdateTypeset( &layer->task, char_y );
                return 0;
        }
        /* �������������β�����Ҹ��в������һ�� */
        if( end_x == p_end_row->string_len && end_y < layer->row_list.rows-1 ) {
                ++end_y;
                p_end_row = TextRowList_GetRow( &layer->row_list, end_y );
                end_x = -1;
                len = char_x + p_end_row->string_len;
        }
        TextRow_SetLength( p_row, len );
        /* �Ƴ���ʼ���������֮����ı��� */
        for( i=char_y+1, j=i; j<end_y; ++j ) {
                TextRowList_RemoveRow( &layer->row_list, i );
        }
        end_y = char_y + 1;
        /* �������е�����ƴ������ʼ�� */
        for( i=char_x, j=end_x+1; i<len && j<p_end_row->string_len; ++i,++j ) {
                p_row->string[i] = p_end_row->string[j];
        }
        TextRow_UpdateSize( p_row, layer->text_style.pixel_size+2 );
        /* �Ƴ������� */
        TextRowList_RemoveRow( &layer->row_list, end_y );
        /* �����ʼ�еĳ���С��0��˵����ʼ��Ҳ��Ҫɾ�� */
        if( len < 0 ) {
                TextRowList_RemoveRow( &layer->row_list, char_y );
        }
        TaskData_AddUpdateTypeset( &layer->task, char_y );;
        return 0;
}

/** ɾ���ı����ĵ�ǰ�����ұߵ��ı� */
LCUI_API int TextLayer_Delete( LCUI_TextLayer *layer, int n_char )
{
        return TextLayer_DeleteText(	layer, layer->insert_y, 
					layer->insert_x, n_char );
}

/** �˸�ɾ���ı�����ɾ���ı����ĵ�ǰ������ߵ��ı� */
LCUI_API int TextLayer_Backspace( LCUI_TextLayer* layer, int n_char )
{
        int n_del;
        int char_x, char_y;
        TextRowData* p_row;

        /* �Ȼ�ȡ��ǰ�ֵ�λ�� */
        char_x = layer->insert_x;
        char_y = layer->insert_y;
        /* �ټ���ɾ�� n_char ���ֺ��λ�� */
        for( n_del=n_char; char_y>=0; --char_y ) {
                p_row = layer->row_list.rowdata[char_y];
		if( char_x >= n_del-1 ) {
			char_x = char_x - n_del;
			n_del = 0;
			break;
		}
		char_x = p_row->string_len-1;
		n_del -= p_row->string_len;
	}
        if( char_y < 0 && n_del == n_char ) {
                return -1;
        }
        /* ���ܹ���ɾ�����ֲ��� n_char �����������ɾ�������� */
        if( n_del > 0 ) {
                n_char -= n_del;
        }
        /* ��ʼɾ���ı� */
        TextLayer_DeleteText( layer, char_y, char_x, n_char );
        /* �����һ�б���ȫ�Ƴ������ƶ����������һ�е���β�� */
        if( char_x == 0 && layer->row_list.rows > 0 
	 && char_y >= layer->row_list.rows ) {
                char_y = layer->row_list.rows-1;
                char_x = layer->row_list.rowdata[char_y]->string_len;
        }
        /* �����ı�����λ�� */
        TextLayer_SetCaretPos( layer, char_y, char_x );
        return 0;
}

/** �����Ƿ������Զ�����ģʽ */
LCUI_API void TextLayer_SetWordWrap( LCUI_TextLayer* layer, int is_true )
{
        if( !layer->is_wordwrap_mode && is_true
	 || layer->is_wordwrap_mode && !is_true ) {
                layer->is_wordwrap_mode = is_true;
                TaskData_AddUpdateTypeset( &layer->task, 0 );;
        }
}

/** ���㲢��ȡ�ı��Ŀ�� */
LCUI_API int TextLayer_GetWidth( LCUI_TextLayer* layer )
{
        int i, row, w, max_w;
        TextRowData* p_row;

        for( row=0,max_w=0; row<layer->row_list.rows; ++row ) {
                p_row = layer->row_list.rowdata[row];
                for( i=0, w=0; i<p_row->string_len; ++i ) {
                        if( !p_row->string[i]->bitmap
			 || !p_row->string[i]->need_display ) {
                                continue;
                        }
                        w += p_row->string[i]->bitmap->advance.x;
                }
                if( w > max_w ) {
                        max_w = w;
                }
        }
        return max_w;
}

/** ���㲢��ȡ�ı��ĸ߶� */
LCUI_API int TextLayer_GetHeight( LCUI_TextLayer* layer )
{
        int i, h;
        TextRowData* p_row;

        for( i=0,h=0; i<layer->row_list.rows; ++i ) {
                p_row = layer->row_list.rowdata[i];
                h += p_row->top_spacing;
                h += p_row->max_height;
                h += p_row->bottom_spacing;
        }
        return h;
}

/** ��������������ֵ�����λͼ */
LCUI_API void TextLayer_ReloadCharBitmap( LCUI_TextLayer* layer )
{
        int row, col, char_h;
        TextCharData* p_char;
        TextRowData* p_row;

        for( row=0; row<layer->row_list.rows; ++row ) {
                p_row = layer->row_list.rowdata[row];
                char_h = 0;
                p_row->max_width = 0;
                p_row->max_height = layer->text_style.pixel_size + 2;
                for( col=0; col<p_row->string_len; ++col ) {
                        p_char = p_row->string[col];
                        TextChar_UpdateBitmap( p_char, &layer->text_style );
                        if( !p_char->need_display || !p_char->bitmap ) {
                                continue;
                        }
                        p_row->max_width += p_char->bitmap->advance.x;
                        if( p_char->style && p_char->style->_pixel_size ) {
                                char_h = p_char->style->pixel_size+2;
                        } else {
                                char_h = layer->text_style.pixel_size+2;
                        }
                        if( p_row->max_height < char_h ) {
                                p_row->max_height = char_h;
                        }
                }
        }
}

/** �������� */
LCUI_API void TextLayer_Update( LCUI_TextLayer* layer )
{
        if( layer->task.update_bitmap ) {
                TextLayer_ReloadCharBitmap( layer );
                layer->task.update_bitmap = FALSE;
        }
        if( layer->task.update_typeset ) {
                TextLayer_TextTypeset( layer, layer->task.typeset_start_row );
                layer->task.update_typeset = FALSE;
                layer->task.typeset_start_row = 0;
        }
}

LCUI_API int TextLayer_PaintToGraph( LCUI_TextLayer* layer, LCUI_Graph *graph, 
					LCUI_Rect area, LCUI_Pos paint_pos )
{
        int x, y, row, col;
	LCUI_Pos char_pos;
        TextRowData *p_row;
        TextCharData *p_char;

        /* ���TextWidget��λ���ڻ��������ڣ�������ɻ������� */
        if( area.x < paint_pos.x ) {
                area.width -= (paint_pos.x - area.x);
                area.x = paint_pos.x;
        }
        if( area.y < paint_pos.y ) {
                area.height -= (paint_pos.y - area.y);
                area.y = paint_pos.y;
        }
        /* ��������TextWidget�����ߴ磬����ݸóߴ�����ɻ������� */
        if( layer->max_width > 0 ) {
                if( paint_pos.x + layer->max_width < area.x + area.width ) {
			area.width = paint_pos.x + layer->max_width - area.x;
                }
        }
        if( layer->max_height > 0 ) {
                if( paint_pos.y + layer->max_height < area.y + area.height ) {
                        area.height = paint_pos.y + layer->max_height - area.y;
                }
        }
        /* ����Y������ƫ���� */
        y = paint_pos.y + layer->offset_y;
        /* ��ȷ������һ�п�ʼ���� */
        for( row=0; row<layer->row_list.rows; ++row ) {
                p_row = TextRowList_GetRow( &layer->row_list, row );
                y += p_row->top_spacing;
                y += p_row->max_height;
                if( y > area.y && y > paint_pos.y ) {
                        y -= p_row->top_spacing;
                        y -= p_row->max_height;
                        break;
                }
                y += p_row->bottom_spacing;
        }
        /* ���û�пɻ��Ƶ��ı��� */
        if( row >= layer->row_list.rows ) {
                return -1;
        }
        
        for( ; row < layer->row_list.rows; ++row ) {
                p_row = TextRowList_GetRow( &layer->row_list, row );
                /* ���ݶ��뷽ʽ��������е�λ�� */
                switch( layer->text_align ) {
                case TEXT_ALIGN_CENTER:
                        x = (layer->max_width - p_row->max_width)/2;
                        break;
                case TEXT_ALIGN_RIGHT:
                        x = layer->max_width - p_row->max_width;
                        break;
                case TEXT_ALIGN_LEFT:
                default:
                        x = 0;
                        break;
                }
                y += p_row->top_spacing;
                x += paint_pos.x;
                x += layer->offset_x;
                
                /* ȷ�����ĸ����ֿ�ʼ���� */
                for( col=0; col<p_row->string_len; ++col ) {
                        p_char = p_row->string[col];
                        /* ���Բ���Ҫ��ʾ��������λͼ������ */
                        if( !p_char->need_display || !p_char->bitmap ) {
                                continue;
                        }
                        x += p_char->bitmap->advance.x;
                        if( x > paint_pos.x && x > area.x ) {
                                x -= p_char->bitmap->advance.x;
                                break;
                        }
                }
                /* ��һ���е��ı������ڿɻ��������� */
                if( col >= p_row->string_len ) {
                        y += p_row->max_height;
                        y += p_row->bottom_spacing;
                        continue;
                }
                /* �������е����� */
                for( ; col<p_row->string_len; ++col ) {
                        p_char = p_row->string[col];
                        /* ���Բ���Ҫ��ʾ��������λͼ������ */
                        if( !p_char->need_display || !p_char->bitmap ) {
                                continue;
                        }
                        /* ��������λͼ�Ļ������� */
                        char_pos.x = x + p_char->bitmap->left;
                        char_pos.y = y + p_row->max_height-1;
                        char_pos.y -= p_char->bitmap->top;
                        x += p_char->bitmap->advance.x;
                        /* �ж�����ʹ�õ�ǰ����ɫ���ٽ��л��� */
                        if( p_char->style && p_char->style->_fore_color ) {
				FontBMP_Mix( graph, char_pos, p_char->bitmap,
						p_char->style->fore_color,
						GRAPH_MIX_FLAG_REPLACE );
                        } else {
				FontBMP_Mix( graph, char_pos, p_char->bitmap,
						layer->text_style.fore_color,
						GRAPH_MIX_FLAG_REPLACE );
                        }
                        /* ����������������򲻼������Ƹ����ı� */
                        if( x > area.x + area.width ) {
                                break;
                        }
                }
                y += p_row->max_height;
                y += p_row->bottom_spacing;
                /* ������������Χ�Ͳ������� */
                if( y > area.y + area.height ) {
                        break;
                }
        }
        return 0;
}

/** �����ı� */
LCUI_API int TextLayer_Paint( LCUI_TextLayer* layer )
{
        /* ����ı�λͼ������Ч */
	if( !Graph_IsValid( &layer->graph ) ) {
                return -1;
        }
	return TextLayer_PaintToGraph( layer, &layer->graph, 
		Rect(0, 0, layer->max_width, layer->max_height), Pos(0,0) );
}
