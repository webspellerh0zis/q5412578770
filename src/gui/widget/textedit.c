/* ***************************************************************************
 * textedit.c -- textedit widget, used to allow user edit text.
 *
 * Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
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
 * textedit.c -- �ı��༭�������������û��༭�ı���
 *
 * ��Ȩ���� (C) 2016 ������ ���� <lc-soft@live.cn>
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

//#define DEBUG
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/font.h>
#include <LCUI/input.h>
#include <LCUI/timer.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textedit.h>
#include <LCUI/ime.h>

#define CARET_HIDE 0
#define CARET_SHOW 1
#define TEXT_BLOCK_SIZE	512

enum TaskType {
	TASK_SET_TEXT,
	TASK_TOTAL
};

/** �ı������������� */
typedef struct LCUI_TextCaretRec_ {
	int type;
	int state;		/**< ��ǰ״̬ */
	int blink_time;		/**< ��˸��� */
	int timer_id;		/**< ��ʱ�� */
	LCUI_BOOL need_show;
} LCUI_TextCaretRec, *LCUI_TextCaret;

typedef struct LCUI_TextEditRec_ {
	LCUI_TextLayer text;			/**< ʵ���ı� */
	LCUI_TextLayer mask_text;		/**< ���κ���ı� */
	LCUI_Widget scrollbar[2];		/**< ���������� */
	LCUI_Widget caret;			/**< �ı������ */
	LCUI_BOOL read_only;			/**< �Ƿ�ֻ�� */
	int block_size;				/**< ���С */
	LinkedList text_blocks;			/**< �ı��黺���� */
	LinkedList text_tags;			/**< ��ǰ����ı�ǩ�б� */
	LCUI_BOOL show_placeholder;		/**< ��ʾռλ���Ƿ��Ѿ���ʾ */
	wchar_t *placeholder;			/**< �ı����ռλ�� */
	wchar_t *allow_input_char;		/**< ����������ַ� */
	wchar_t password_char;			/**< ���η��ĸ��� */
	LCUI_TextStyle placeholder_style;	/**< ռλ�����ı���ʽ */
	LCUI_TextStyle textstyle_bak;		/**< �ı������ı���ʽ�ĸ��� */
	LCUI_BOOL tasks[TASK_TOTAL];		/**< ����������� */
	LCUI_Mutex mutex;			/**< ������ */
} LCUI_TextEditRec, *LCUI_TextEdit;

enum TextBlockType_ {
	TEXT_BLOCK_BEGIN,
	TEXT_BLOCK_BODY,
	TEXT_BLOCK_END
};

/** �ı������ݽṹ */
typedef struct LCUI_TextBlockRec_ {
	int type;			/**< �ı������� */
	int add_type;			/**< ָ�����ı������ӷ�ʽ */
	wchar_t *text;			/**< �ı���(��) */
} LCUI_TextBlockRec, *LCUI_TextBlock;

/*---------------------------------- Caret ----------------------------------*/

static const char *textcaret_css = ToString(
textcaret {
	pointer-events: none;
	width: 1px;
	height:14px;
	position: absolute;
	background-color: #000;
}
);

static const char *textedit_css = ToString(
textedit {
min-width: 124px;
min-height:14px;
background-color: #fff;
border: 1px solid #eee;
}
);

static void TextCaret_BlinkShow( LCUI_Widget widget )
{
	LCUI_TextCaret caret = widget->private_data;
	if( !caret->need_show ) {
		return;
	}
	caret->state = CARET_SHOW;
	LCUITimer_Reset( caret->timer_id, caret->blink_time );
	Widget_Show( widget );
}

static void TextCaret_BlinkHide( LCUI_Widget widget )
{
	LCUI_TextCaret caret = widget->private_data;
	caret->state = CARET_HIDE;
	LCUITimer_Reset( caret->timer_id, caret->blink_time );
	Widget_Hide( widget );
}

static void TextCaret_OnBlink( void *arg )
{
	LCUI_Widget widget = arg;
	LCUI_TextCaret caret = widget->private_data;
	if( caret->need_show ) {
		if( caret->state == CARET_HIDE ) {
			caret->state = CARET_SHOW;
			Widget_Show( widget );
		} else {
			caret->state = CARET_HIDE;
			Widget_Hide( widget );
		}
	}
}

static void TextCaret_SetVisible( LCUI_Widget widget, LCUI_BOOL visible )
{
	LCUI_TextCaret caret = widget->private_data;
	caret->need_show = visible;
	if( caret->need_show ) {
		TextCaret_BlinkShow( widget );
	} else {
		TextCaret_BlinkHide( widget );
	}
}

static void TextCaret_OnInit( LCUI_Widget widget )
{
	LCUI_TextCaret caret;
	caret = Widget_NewPrivateData( widget, LCUI_TextCaretRec );
	caret->type = 0;
	caret->need_show = FALSE;
	caret->state = CARET_HIDE;
	caret->blink_time = 500;
	caret->timer_id = LCUITimer_Set( caret->blink_time, 
					 TextCaret_OnBlink, caret, TRUE );
	LCUICSS_LoadString( textcaret_css );
}

/** ������˸��ʱ���� */
static void TextCaret_SetBlinkTime( LCUI_Widget widget, unsigned int n_ms )
{
	LCUI_TextCaret caret = widget->private_data;
	caret->blink_time = n_ms;
	LCUITimer_Reset( caret->timer_id, caret->blink_time );
}

/** ���ٲ���� */
static void TextCaret_OnDestroy( LCUI_Widget widget )
{
	LCUI_TextCaret caret = widget->private_data;
	LCUITimer_Free( caret->timer_id );
	caret->timer_id = -1;
}

static void LCUIWidget_AddTextCaret( void )
{
	LCUI_WidgetClass *wc = LCUIWidget_AddClass( "textcaret" );
	wc->methods.init = TextCaret_OnInit;
	wc->methods.destroy = TextCaret_OnDestroy;
	LCUICSS_LoadString( textcaret_css );
}

/*-------------------------------- End Caret --------------------------------*/

/** �ƶ��ı����ڵ��ı���������������� */
static void TextEdit_MoveCaret( LCUI_Widget widget, int row, int col )
{
	LCUI_Pos pos;
	LCUI_TextEdit edit = widget->private_data;
	TextLayer_SetCaretPos( edit->text, row, col );
	if( TextLayer_GetCaretPixelPos( edit->text, &pos ) == 0 ) {
		int height = TextLayer_GetRowHeight( edit->text, row );
		LCUI_StyleSheet sheet = edit->caret->custom_style;
		SetStyle( sheet, key_height, height, px );
		Widget_Move( edit->caret, pos.x, pos.y );
	}
	TextCaret_BlinkShow( edit->caret );
}

/*------------------------------- TextBlock ---------------------------------*/
static void TextBlock_OnDestroy( void *arg )
{
	LCUI_TextBlock blk = arg;
	free( blk->text );
	blk->text = NULL;
}

static int TextEdit_AddTextToBuffer( LCUI_Widget widget, const wchar_t *wtext,
				     int add_type )
{
	const wchar_t *p;
	LCUI_TextEdit edit;
	LCUI_TextBlock txtblk;
	int i, j, len, tag_len, size;

	edit = widget->private_data;
	len = wcslen( wtext );
	for( i = 0; i < len; ++i ) {
		txtblk = NEW( LCUI_TextBlockRec, 1 );
		if( !txtblk ) {
			return ENOMEM;
		}
		if( i == 0 ) {
			txtblk->type = TEXT_BLOCK_BEGIN;
		} else if( len - i > edit->block_size ) {
			size = edit->block_size;
			txtblk->type = TEXT_BLOCK_BODY;
		} else {
			size = len - i;
			txtblk->type = TEXT_BLOCK_END;
		}
		size += 1;
		txtblk->add_type = add_type;
		txtblk->text = NEW( wchar_t, size );
		if( !txtblk->text ) {
			return ENOMEM;
		}
		/* ���δ������ʽ��ǩ���� */
		if( !edit->text->is_using_style_tags ) {
			for( j = 0; i < len && j < size - 1; ++j, ++i ) {
				txtblk->text[j] = wtext[i];
			}
			--i;
			txtblk->text[j] = 0;
			LinkedList_Append( &edit->text_blocks, txtblk );
			continue;
		}
		for( j = 0; i < len&&j < size - 1; ++j, ++i ) {
			wchar_t *text;
			txtblk->text[j] = wtext[i];
			/* ����Ƿ�����ʽ��ǩ */
			p = scan_style_tag( wtext + i, NULL, 0, NULL );
			if( !p ) {
				p = scan_style_ending_tag( wtext + i, NULL );
				if( !p ) {
					continue;
				}
			}
			/* �����ǩ�ĳ��� */
			tag_len = p - wtext - i;
			/* ����ǰ���С�ܹ����������ǩ */
			if( j + tag_len <= size - 1 ) {
				continue;
			}
			/* ���¼�����ı���Ĵ�С�������·����ڴ�ռ� */
			size = j + tag_len + 1;
			text = realloc( txtblk->text, sizeof(wchar_t) * size );
			if( !text ) {
				return ENOMEM;
			}
			txtblk->text = text;
		}
		--i;
		txtblk->text[j] = 0;
		/* ����ı����������� */
		LinkedList_Append( &edit->text_blocks, txtblk );
	}
	edit->tasks[TASK_SET_TEXT] = TRUE;
	Widget_AddTask( widget, WTT_USER );
	return 0;
}

/** �����ı����ڵ�����λͼ */
static void TextEdit_ProcTextBlock( LCUI_Widget widget, LCUI_TextBlock txtblk )
{
	LCUI_TextEdit edit;
	edit = widget->private_data;
	if( edit->text_blocks.length <= 0 ) {
		return;
	}
	switch( txtblk->add_type ) {
	case TEXT_ADD_TYPE_APPEND:
		/* �����ı���׷�����ı�ĩβ */
		TextLayer_AppendTextW( edit->text, txtblk->text, 
				       &edit->text_tags );
		break;
	case TEXT_ADD_TYPE_INSERT:
		/* �����ı���������ı���������ڴ� */
		TextLayer_InsertTextW( edit->text, txtblk->text, 
				       &edit->text_tags );
	default: break;
	}
}


/** �����ı�����ı�ͼ�� */
static void TextEdit_UpdateTextLayer( LCUI_Widget widget )
{
	LinkedList rects;
	LCUI_TextEdit edit;
	LCUI_TextLayer layer;
	LinkedListNode *node;

	LinkedList_Init( &rects );
	edit = widget->private_data;
	/* �����Ƿ��������η����ж�������ĸ��ı�ͼ������� */
	if( edit->password_char ) {
		layer = edit->mask_text;
	} else {
		layer = edit->text;
	}
	LinkedList_ForEach( node, &rects ) {
		Widget_InvalidateArea( widget, node->data, SV_CONTENT_BOX );
	}
	LinkedList_Clear( &rects, free );
	TextLayer_ClearInvalidRect( layer );
}

static void TextEdit_OnTask( LCUI_Widget widget )
{
	LCUI_TextEdit edit = widget->private_data;
	if( edit->tasks[TASK_SET_TEXT] ) {
		LinkedList blocks;
		LinkedListNode *node;
		LCUIMutex_Lock( &edit->mutex );
		blocks = edit->text_blocks;
		LinkedList_Init( &edit->text_blocks );
		LCUIMutex_Unlock( &edit->mutex );
		LinkedList_ForEach( node, &blocks ) {
			TextEdit_ProcTextBlock( widget, node->data );
		}
		LinkedList_Clear( &blocks, TextBlock_OnDestroy );
	}
	TextEdit_UpdateTextLayer( widget );
}

/*----------------------------- End TextBlock ---------------------------------*/

/** ָ���ı����Ƿ�����Ʒ� */
void TextEdit_SetUsingStyleTags( LCUI_Widget widget, LCUI_BOOL is_true )
{
	LCUI_TextEdit edit = widget->private_data;
	TextLayer_SetUsingStyleTags( edit->text, is_true );
}

/** ָ���ı����Ƿ����ö����ı���ʾ */
void TextEdit_SetMultiline( LCUI_Widget widget, LCUI_BOOL is_true )
{
	LCUI_TextEdit edit = widget->private_data;
	TextLayer_SetMultiline( edit->text, is_true );
}

/** ����ı����� */
void TextEdit_ClearText( LCUI_Widget widget )
{
	LCUI_TextEdit edit;
	Widget_Lock( widget );
	edit = widget->private_data;
	LCUIMutex_Lock( &edit->mutex );
	TextLayer_ClearText( edit->text );
	StyleTags_Clear( &edit->text_tags );
	LCUIMutex_Unlock( &edit->mutex );
	Widget_InvalidateArea( widget, NULL, SV_PADDING_BOX );
	Widget_Unlock( widget );
}

/** Ϊ�ı��������ı������ַ��棩 */
int TextEdit_SetTextW( LCUI_Widget widget, const wchar_t *wstr )
{
	TextEdit_ClearText( widget );
	return TextEdit_AddTextToBuffer( widget, wstr, TEXT_ADD_TYPE_APPEND );
}

int TextEdit_SetText( LCUI_Widget widget, const char *utf8_str )
{
	int ret;
	size_t len = strlen( utf8_str ) + 1;
	wchar_t *wstr = malloc( len * sizeof( wchar_t ) );
	if( !wstr ) {
		return ENOMEM;
	}
	LCUI_DecodeString( wstr, utf8_str, len, ENCODING_UTF8 );
	ret = TextEdit_SetTextW( widget, wstr );
	free( wstr );
	return ret;
}

/** Ϊ�ı���׷���ı������ַ��棩 */
int TextEdit_AppendTextW( LCUI_Widget widget, const wchar_t *wstr )
{
	return TextEdit_AddTextToBuffer( widget, wstr, TEXT_ADD_TYPE_APPEND );
}

/** Ϊ�ı�������ı������ַ��棩 */
int TextEdit_InsertTextW( LCUI_Widget widget, const wchar_t *wstr )
{
	return TextEdit_AddTextToBuffer( widget, wstr, TEXT_ADD_TYPE_INSERT );
}

static void TextEdit_OnFocus( LCUI_Widget widget, LCUI_WidgetEvent e, void *arg )
{
	LCUI_TextEdit edit;
	edit = widget->private_data;
	/* �趨���뷨��Ŀ�� */
	LCUIIME_SetTarget( widget );
	TextCaret_SetVisible( edit->caret, TRUE );
	TextCaret_BlinkHide( edit->caret );
}

static void TextEdit_OnBlur( LCUI_Widget widget, LCUI_WidgetEvent e, void *arg )
{
	LCUI_TextEdit edit;
	edit = widget->private_data;
	LCUIIME_ClearTarget();
	TextCaret_SetVisible( edit->caret, TRUE );
}

static void TextEdit_TextBackspace( LCUI_Widget widget, int n_ch )
{
	LCUI_TextEdit edit;
	Widget_Lock( widget );
	edit = widget->private_data;
	LCUIMutex_Lock( &edit->mutex );
	TextLayer_Backspace( edit->text, n_ch );
	TextCaret_BlinkShow( edit->caret );
	LCUIMutex_Unlock( &edit->mutex );
	Widget_Unlock( widget );
}

static void TextEdit_TextDelete(LCUI_Widget widget, int n_ch )
{
	LCUI_TextEdit edit;
	Widget_Lock( widget );
	edit = widget->private_data;
	LCUIMutex_Lock( &edit->mutex );
	TextLayer_Delete( edit->text, n_ch );
	TextCaret_BlinkShow( edit->caret );
	LCUIMutex_Unlock( &edit->mutex );
	Widget_Unlock( widget );
}

/** �������¼� */
static void TextEdit_OnKeyDown( LCUI_Widget widget, LCUI_WidgetEvent e, void *arg )
{
	LCUI_TextEdit edit;
	int cols, rows, cur_col, cur_row;

	edit = widget->private_data;
	cur_row = edit->text->insert_y;
	cur_col = edit->text->insert_x;
	cols = TextLayer_GetRowTextLength( edit->text, cur_row );
	rows = TextLayer_GetRowTotal( edit->text );
	switch( e->key.code ) {
	case LCUIKEY_HOMEPAGE: // home���ƶ����������
		cur_col = 0;
		break;
	case LCUIKEY_END: // end���ƶ��������β
		cur_col = cols;
		break;
	case LCUIKEY_LEFT:
		if( cur_col > 0 ) {
			--cur_col;
		} else if( cur_row > 0 ) {
			--cur_row;
			cur_col = TextLayer_GetRowTextLength( edit->text,
							      cur_row );
		}
		break;
	case LCUIKEY_RIGHT:
		if( cur_col < cols ) {
			++cur_col;
		} else if( cur_row < rows - 1 ) {
			++cur_row;
			cur_col = 0;
		}
		break;
	case LCUIKEY_UP:
		if( cur_row > 0 ) {
			--cur_row;
		}
		break;
	case LCUIKEY_DOWN:
		if( cur_row < rows - 1 ) {
			++cur_row;
		}
		break;
	case LCUIKEY_BACKSPACE: // ɾ�������ߵ��ַ�
		TextEdit_TextBackspace( widget, 1 );
		break;
	case LCUIKEY_DELETE: // ɾ������ұߵ��ַ�
		TextEdit_TextDelete( widget, 1 );
		break;
	default:break;
	}
	TextEdit_MoveCaret( widget, cur_row, cur_col );
}

/** �������뷨���ı������������ */
static void TextEdit_OnInput( LCUI_Widget widget, LCUI_WidgetEvent e, void *arg )
{
	uint_t i;
	LCUI_TextEdit edit;
	wchar_t *ptr, *tmp_ptr, *ptr_last;

	ptr = e->input.text;
	edit = widget->private_data;
	ptr_last = ptr + sizeof( e->input.text ) / sizeof( wchar_t );
	/* ����ı�����ֻ���� */
	if( edit->read_only ) {
		return;
	}
	for( ; ptr < ptr_last && *ptr != '\0'; ++ptr ) {
		if( edit->allow_input_char ) {
			/* �жϵ�ǰ�ַ��Ƿ�Ϊ���Ʒ�Χ�ڵ��ַ� */
			for( i = 0; i < edit->allow_input_char[i]; ++i ) {
				if( edit->allow_input_char[i] == *ptr ) {
					break;
				}
			}
			/* �������ǰ����ѭ�����������ǰ�ַ�������� */
			if( edit->allow_input_char[i] ) {
				continue;
			}
		} else {
			continue;
		}
		/* ������������ַ������Ƴ����ַ� */
		tmp_ptr = ptr;
		while( tmp_ptr < ptr_last && *tmp_ptr != '\0' ) {
			*tmp_ptr = *(tmp_ptr + 1);
			++tmp_ptr;
		}
	}
	TextEdit_InsertTextW( widget, e->input.text );
}

static void TextEdit_OnInit( LCUI_Widget widget )
{
	LCUI_TextEdit edit;
	edit = Widget_NewPrivateData( widget, LCUI_TextEditRec );
	edit->read_only = FALSE;
	edit->text = TextLayer_New();
	edit->allow_input_char = NULL;
	edit->show_placeholder = FALSE;
	edit->mask_text = TextLayer_New();
	edit->block_size = TEXT_BLOCK_SIZE;
	edit->caret = LCUIWidget_New( "textcaret" );
	TextLayer_SetUsingStyleTags( edit->text, FALSE );
	TextEdit_SetMultiline( widget, FALSE );
	LinkedList_Init( &edit->text_blocks );
	Widget_Append( widget, edit->caret );
	TextLayer_SetAutoWrap( edit->text, TRUE );
	TextLayer_SetAutoWrap( edit->mask_text, TRUE );
	Widget_BindEvent( widget, "keydown", TextEdit_OnKeyDown, NULL, NULL );
	Widget_BindEvent( widget, "input", TextEdit_OnInput, NULL, NULL );
	Widget_BindEvent( widget, "focus", TextEdit_OnFocus, NULL, NULL );
	Widget_BindEvent( widget, "blur", TextEdit_OnBlur, NULL, NULL );
	LCUIMutex_Init( &edit->mutex );
}

static void TextEdit_OnDestroy( LCUI_Widget widget )
{
	LCUI_TextEdit edit = widget->private_data;
	TextLayer_Destroy( edit->text );
	TextLayer_Destroy( edit->mask_text );
	LinkedList_Clear( &edit->text_blocks, TextBlock_OnDestroy );
}

static void TextEdit_OnResize( LCUI_Widget w, LCUI_WidgetEvent e, void *arg )
{
	LinkedList rects;
	LinkedListNode *node;
	LCUI_Size new_size = {16, 16};
	LCUI_TextEdit edit = w->private_data;
	if( w->box.content.width > new_size.w ) {
		new_size.w = w->box.content.width;
	}
	if( w->box.content.height > new_size.h ) {
		new_size.h = w->box.content.height;
	}
	LinkedList_Init( &rects );
	TextLayer_SetMaxSize( edit->text, new_size );
	TextLayer_Update( edit->text, &rects );
	LinkedList_ForEach( node, &rects ) {
		Widget_InvalidateArea( w, node->data, SV_CONTENT_BOX );
	}
	LinkedList_Clear( &rects, free );
	TextLayer_ClearInvalidRect( edit->text );
}

static void TextEdit_OnPaint( LCUI_Widget w, LCUI_PaintContext paint )
{
	LCUI_TextEdit edit;
	LCUI_Pos layer_pos;
	LCUI_TextLayer layer;
	LCUI_Rect content_rect, rect;

	edit = w->private_data;
	content_rect.width = w->box.content.width;
	content_rect.height = w->box.content.height;
	content_rect.y = w->box.content.top - w->box.graph.top;
	content_rect.x = w->box.content.left - w->box.graph.left;
	LCUIRect_GetOverlayRect( &content_rect, &paint->rect, &rect );
	layer_pos.x = content_rect.x - paint->rect.x;
	layer_pos.y = content_rect.y - paint->rect.y;
	rect.x -= content_rect.x;
	rect.y -= content_rect.y;
	if( edit->password_char ) {
		layer = edit->mask_text;
	} else {
		layer = edit->text;
	}
	TextLayer_DrawToGraph( layer, rect, layer_pos, &paint->canvas );
}


/** ��� TextEdit �������� */
void LCUIWidget_AddTextEdit( void )
{
	LCUI_WidgetClass *wc = LCUIWidget_AddClass( "textedit" );
	wc->methods.init = TextEdit_OnInit;
	wc->methods.paint = TextEdit_OnPaint;
	wc->methods.destroy = TextEdit_OnDestroy;
	wc->methods.set_text = TextEdit_SetText;
	wc->task_handler = TextEdit_OnTask;
	LCUICSS_LoadString( textedit_css );
	LCUIWidget_AddTextCaret();
}
