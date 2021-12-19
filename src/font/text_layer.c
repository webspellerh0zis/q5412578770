
//#define DEBUG

#include <LCUI_Build.h>

#include LC_LCUI_H 
#include LC_GRAPH_H
#include LC_WIDGET_H
#include LC_FONT_H

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Special_KeyWord	Special_KeyWord;
typedef struct _LCUI_CharData	LCUI_CharData;
typedef struct _Text_RowData		Text_RowData; 
typedef struct _tag_style_data	tag_style_data;

typedef enum _tag_id	enum_tag_id;

enum _tag_id
{
	TAG_ID_FAMILY = 0,
	TAG_ID_STYLE = 1,
	TAG_ID_WIEGHT = 2,
	TAG_ID_DECORATION = 3,
	TAG_ID_SIZE = 4,
	TAG_ID_COLOR = 5
}; 

/********* ����������������Լ�λͼ ********/
struct _LCUI_CharData
{
	wchar_t char_code;	/* �ַ��� */
	LCUI_FontBMP bitmap;	/* ����λͼ */
	BOOL display:2;		/* ��־���Ƿ���Ҫ��ʾ���� */
	BOOL need_update:2;	/* ��־����ʾ�Ƿ���Ҫˢ�¸��ֵ�����λͼ���� */
	//BOOL using_quote:2;	/* ��־����ʾ�Ƿ��������ֳɵ��ı���ʽ */
	LCUI_TextStyle *data;	/* �ı���ʽ���� */
};
/***************************************/

/********* ����һ�е��ı����� *************/
struct _Text_RowData
{
	LCUI_Size max_size;	/* ��¼���ߴ� */
	LCUI_Queue string;	/* ��������еĳ�Ա��������Դ�ı����������� */
};
/***************************************/

/*************************** ����ؼ��� *****************************/
struct _Special_KeyWord
{
	LCUI_String keyword;		/* �ؼ��� */
	LCUI_TextStyle *data;		/* �ùؼ���ʹ�õ��������� */
	LCUI_Queue *text_source_data;	/* �ؼ���������Դ�ı� */
};
/******************************************************************/

struct _tag_style_data 
{
	enum_tag_id tag;
	void *style;
};

/**************************** TextStyle *******************************/
void 
TextStyle_Init ( LCUI_TextStyle *data )
/* ��ʼ��������ʽ���� */
{
	memset( data->family, 0, sizeof(data->family) );
	data->style = FONT_STYLE_NORMAL;
	data->weight = FONT_WEIGHT_NORMAL;
	data->decoration = FONT_DECORATION_NONE;
	data->need_back_color = IS_FALSE;
	data->need_fore_color = IS_FALSE;
	data->fore_color = RGB(0,0,0);
	data->back_color = RGB(0,0,0);
	data->pixel_size = 12;
}

void
TextStyle_FontFamily( LCUI_TextStyle *style, const char *fontfamily )
{
	strncpy( style->family, fontfamily, sizeof(style->family) );
}

void
TextStyle_FontSize( LCUI_TextStyle *style, int fontsize )
{
	style->pixel_size = fontsize;
}

void
TextStyle_FontColor( LCUI_TextStyle *style, LCUI_RGB color )
{
	style->fore_color = color;
}

void
TextStyle_FontBackColor( LCUI_TextStyle *style, LCUI_RGB color )
{
	style->back_color = color;
}

void
TextStyle_FontStyle( LCUI_TextStyle *style, enum_font_style fontstyle )
{
	style->style = fontstyle;
}

void
TextStyle_FontWeight( LCUI_TextStyle *style, enum_font_weight fontweight )
{
	style->weight = fontweight;
}

void
TextStyle_FontDecoration( LCUI_TextStyle *style, enum_font_decoration decoration )
{
	style->decoration = decoration;
}

int 
TextStyle_Cmp( LCUI_TextStyle *a, LCUI_TextStyle *b )
{
	return 0;
}

/************************** End TextStyle *****************************/


/***************************** ˽�к��� *********************************/

static void 
destroy_tag_style_data( tag_style_data *data )
{ 
	//free( data->style );
}

static void 
Destroy_Special_KeyWord(Special_KeyWord *key)
{
	/* �ڸùؼ���������ԭ�ı��в�����ɸùؼ��ʵ��֣����޸�����ʹ�õ��������� */
}

static void 
Destroy_CharData(LCUI_CharData *data)
{ 
	FontBMP_Free( &data->bitmap );
	free( data->data );
	//if( data->using_quote == IS_FALSE ) {
		//free( data->data );
	//}
}

static void 
Destroy_Text_RowData(Text_RowData *data)
{
	Destroy_Queue ( &data->string );
}


static void 
TextLayer_TagStyle_Add( LCUI_TextLayer *layer, tag_style_data *data )
/* ��������ʽ���ݼ������ */
{
	Queue_Add( &layer->tag_buff, data );
}

#define MAX_TAG_NUM 1

static LCUI_TextStyle *
TextLayer_Get_Current_TextStyle ( LCUI_TextLayer *layer )
/* ��ȡ��ǰ��������ʽ���� */
{
	int i, total, equal = 0,flags[MAX_TAG_NUM];
	LCUI_TextStyle *data;
	tag_style_data *p; 
	
	data = (LCUI_TextStyle*) malloc (sizeof(LCUI_TextStyle));
	TextStyle_Init( data );
	
	total = Queue_Get_Total( &layer->tag_buff );
	//DEBUG_MSG("total tag: %d\n", total);
	if(total <= 0) {
		free( data );
		return NULL;
	}
	/* ����ʽ���ݶ����л�ȡ������ʽ���� */
	for(equal=0,i=total-1; i>=0; --i) {
		p = Queue_Get( &layer->tag_buff, i );
		DEBUG_MSG("tag id: %d\n", p->tag);
		switch( p->tag ) {
		    case TAG_ID_COLOR: 
			if( flags[0] == 0 ) {
				data->need_fore_color = IS_TRUE;
				data->fore_color = *((LCUI_RGB*)p->style);
				++equal;
			}
			break;
		    default: break;
		}
		if(equal == MAX_TAG_NUM) {
			break;
		}
	} 
	if( equal != MAX_TAG_NUM ) {
		free( data );
		return NULL;
	}
	return data;
}

static void 
TextLayer_TagStyle_Delete( LCUI_TextLayer *layer, enum_tag_id tag)
/* ��ָ����ǩ����ʽ���ݴӶ�����ɾ����ֻɾ������β����һ��ƥ��ı�ǩ */
{
	int i, total;
	tag_style_data *p; 
	 
	total = Queue_Get_Total( &layer->tag_buff );
	DEBUG_MSG("delete start, total tag: %d\n", Queue_Get_Total( &layer->tag_buff ));
	if(total <= 0) {
		return;
	}
	for(i=total-1; i>=0; --i) {
		p = Queue_Get( &layer->tag_buff, i );
		if( p->tag == tag ) {
			Queue_Delete( &layer->tag_buff, i );
			break;
		}
	} 
	DEBUG_MSG("delete end, total tag: %d\n", Queue_Get_Total( &layer->tag_buff ));
}

void clear_space(char *in, char *out)
/* ����ַ����еĿո� */
{
	int j, i, len = strlen(in);
	for(j=i=0; i<len; ++i) {
		if(in[i] == ' ') {
			continue;
		}
		out[j] = in[i];
		++j;
	}
	out[j] = 0;
}

static wchar_t *
get_style_endtag ( wchar_t *str, char *out_tag_name )
/* ���ַ����л�ȡ��ʽ�Ľ�����ǩ��������Ǳ�ǩ�� */
{
	wchar_t *p;
	int i, j, len, tag_found = 0;
	
	len = wcslen ( str );
	//printf("string: %S\n", str);
	if(str[0] != '<' || str[1] != '/') { 
		return NULL;
	} 
	/* ƥ���ǩ,��ȡ��ǩ�� */
	for(j=0,i=2; i<len; ++i) {
		switch(str[i]) {
		    case ' ': 
			if(  tag_found == 0 ) {
				break;
			} 
			return NULL;
		    case '>': goto end_tag_search;
		    default: out_tag_name[j] = str[i]; ++j; break;
		}
	}
	
end_tag_search:;

	out_tag_name[j] = 0;
	if( j < 1 ) { 
		return NULL;
	}
	p = &str[i];
	return p;
}

static wchar_t *
get_style_tag( wchar_t *str, const char *tag, char *out_tag_data )
/* ���ַ����л�ȡָ����ʽ��ǩ�е����� */
{
	wchar_t *p;
	int i, j, len, tag_len; 
	
	len = wcslen ( str );
	tag_len = strlen ( tag );
	if(str[0] != '<') {
		return NULL;
	} 
	/* ƥ���ǩǰ�벿�� */
	for(j=0,i=1; i<len; ++i) {
		if( str[i] == ' ' ) { 
			if( j == 0 || j >= tag_len ) { 
				continue;
			}
			return NULL;
		}
		else if( str[i] == tag[j] ) { 
			++j;
		} else { 
			/* �����ǩ�������Ѿ�ƥ���� */
			if( j>= tag_len ) { 
				if( str[i] == '=' ) {
					++i; 
					break;
				}
			}
			/* �������� */
			return NULL;
		}
	}
	/* ��ȡ��ǩ��벿�� */
	for(j=0; i<len; ++i) {
		if( str[i] == ' ' ) {
			continue; 
		} else {
			/* ��ǩ�������˳� */
			if( str[i] == '>' ) {
				break;
			}
			/* �����ǩ�ڵ����� */
			out_tag_data[j] = str[i];
			++j;
		}
	}
	out_tag_data[j] = 0;
	if(i >= len ) {
		return NULL;
	}
	p = &str[i];
	return p;
}

static wchar_t *
covernt_tag_to_style_data (wchar_t *str, tag_style_data *out_data)
/* �����ַ����еı�ǩ�õ���Ӧ����ʽ���ݣ�������ָ���ǩ�����ַ���ָ�� */
{
	wchar_t *p, *q; 
	char tag_data[256];
	
	p = str; 
	//DEBUG_MSG("covernt_tag_to_style_data(): enter\n");
	if( (q = get_style_tag ( p, "color", tag_data)) ) {
		int r,g,b, len, i, j;
		LCUI_RGB rgb;
		
		p = q;
		//DEBUG_MSG("is color style tag, data: %s\n", tag_data);
		len = strlen(tag_data); 
		for(j=0,i=0; i<len; ++i) {
			if(tag_data[i] == ',') {
				++j;
			}
		}
		if(j == 2) {
			sscanf( tag_data, "%d,%d,%d", &r, &g, &b ); 
		}
		else if(tag_data[0] == '#') {
			switch(len) {
			    case 4: 
				sscanf( tag_data, "#%1X%1X%1X", &r, &g, &b );
				r<<=4; g<<=4; b<<=4;
				break;
			    case 7:
				sscanf( tag_data, "#%2X%2X%2X", &r, &g, &b ); 
				break;
			    default:
				r=0; g=0; b=0;
				break;
			}
		} else {
			r=0; g=0; b=0;
		}
		DEBUG_MSG("color: %d,%d,%d\n", r,g,b);
		rgb = RGB(r, g, b);
		out_data->tag = TAG_ID_COLOR;
		out_data->style = malloc( sizeof(LCUI_RGB) );
		memcpy( out_data->style, &rgb, sizeof(LCUI_RGB) );
	} else {
		p = NULL;
	}
	//DEBUG_MSG("covernt_tag_to_style_data(): quit\n");
	return p;
}

static wchar_t *
handle_style_tag( LCUI_TextLayer *layer, wchar_t *str )
{
	wchar_t *q;
	tag_style_data data;
	
	/* ��ʼ������ʽ��ǩ */
	q = covernt_tag_to_style_data ( str, &data );
	//DEBUG_MSG("handle_style_tag():%p\n", q);
	if( q != NULL ) {
		//DEBUG_MSG("add style data\n");
		/* ����ǩ��ʽ���ݼ������ */
		TextLayer_TagStyle_Add( layer, &data ); 
	}
	return q;
}

static wchar_t *
handle_style_endtag( LCUI_TextLayer *layer, wchar_t *str )
/* ������ʽ�Ľ�����ǩ */
{
	wchar_t *p;
	char tag_name[256];
	/* ��ȡ��ǩ�� */
	p = get_style_endtag( str, tag_name );
	if( p == NULL ) {
		return NULL;
	}
	if( strcasecmp(tag_name, "color") == 0 ) {
		/* �����ñ�ǩ��ӵ�������ʽ */
		TextLayer_TagStyle_Delete ( layer, TAG_ID_COLOR );
	}
	return p;
}

static void 
TextLayer_Get_Char_BMP ( LCUI_CharData *data )
/* ��ȡ����λͼ���������ʽ���ı�ͼ���м�¼��������ʽ���� */
{
	LCUI_Font font;
	//font = LCUI_Sys.default_font;
	Font_Init( &font );
	if(data->data != NULL) {
		if(data->data->pixel_size != -1) {
			font.size = data->data->pixel_size;
		}
	}
	Get_FontBMP( &font, data->char_code, &data->bitmap );
}

static int 
TextLayer_Text_Add_NewRow ( LCUI_TextLayer *layer )
/* ������� */
{
	Text_RowData data;
	data.max_size = Size(0,0);
	Queue_Init( &data.string, sizeof(LCUI_CharData), NULL );
	/* ʹ������ģʽ���������ݵĲ��� */
	Queue_Set_DataMode( &data.string, QUEUE_DATA_MODE_LINKED_LIST );
	/* ���г�Աʹ��ָ�룬��Ҫ������text_source_data��������� */
	Queue_Using_Pointer( &data.string );
	return Queue_Add( &layer->rows_data, &data );
}

static Text_RowData *
TextLayer_Get_Current_RowData ( LCUI_TextLayer *layer )
/* ��ȡָ��ǰ�е�ָ�� */
{
	return Queue_Get( &layer->rows_data, layer->current_des_pos.y );
}

static void
TextLayer_Update_RowSize (LCUI_TextLayer *layer, int row )
/* ����ָ�����ı�λͼ�ĳߴ� */
{
	int total, i;
	LCUI_Size size;
	LCUI_CharData *char_data;
	Text_RowData *row_data;
	
	row_data = Queue_Get( &layer->rows_data, row );
	total = Queue_Get_Total( &row_data->string );
	for( size.w=0,size.h=0,i=0; i<total; ++i ) {
		char_data = Queue_Get( &row_data->string, i );
		size.w += char_data->bitmap.width;
		size.w += char_data->bitmap.left;
		//height = char_data->bitmap.top;
		if( char_data->data != NULL ) {
			if( char_data->data->pixel_size != -1 ) {
				if( size.h < char_data->data->pixel_size + 2) {
					size.h = char_data->data->pixel_size + 2;
				}
			} else {
				if( size.h < 14) {
					size.h = 14;
				}
			}
		} else {
			if( size.h < 14) {
				size.h = 14;
			}
		}
	}
	row_data->max_size = size;
}
/**********************************************************************/


/*************************** �����Ĵ��� *********************************/
void 
TextLayer_Init( LCUI_TextLayer *layer )
/* ��ʼ���ı�ͼ��������� */
{
	layer->using_code_mode = IS_FALSE; 
	layer->using_style_tags = IS_FALSE; 
	layer->enable_word_wrap = IS_FALSE; 
	layer->enable_multiline = IS_FALSE; 
	
	layer->have_select = IS_FALSE;
	layer->start = 0;
	layer->end = 0;
	
	Queue_Init( &layer->color_keyword, sizeof(Special_KeyWord), Destroy_Special_KeyWord );
	/* ������ʹ����������Щ���� */
	Queue_Init( &layer->text_source_data, sizeof(LCUI_CharData), Destroy_CharData );
	Queue_Set_DataMode( &layer->text_source_data, QUEUE_DATA_MODE_LINKED_LIST ); 
	Queue_Init( &layer->rows_data, sizeof(Text_RowData), Destroy_Text_RowData ); 
	TextLayer_Text_Add_NewRow ( layer );/* ������� */
	Queue_Init( &layer->tag_buff, sizeof(tag_style_data), destroy_tag_style_data );
	Queue_Init( &layer->style_data, sizeof(LCUI_TextStyle), NULL );
	
	layer->default_data.pixel_size = 12;
	layer->current_src_pos = 0;
	layer->current_des_pos = Pos(0,0);
	layer->max_text_len = 5000; 
	TextStyle_Init ( &layer->default_data );
}

void 
Destroy_TextLayer( LCUI_TextLayer *layer )
/* �����ı�ͼ��ռ�õ���Դ */
{
	
}

void 
TextLayer_Draw( LCUI_Widget *widget, LCUI_TextLayer *layer, int mode )
/* �����ı�ͼ�� */
{
	LCUI_Pos pos;
	int i, j, n, rows, fontheight;
	LCUI_RGB color;
	LCUI_CharData *p_data;
	Text_RowData *p_row;
	
	/* ��ʼճ���ı�λͼ */
	rows = Queue_Get_Total( &layer->rows_data );
	for(pos.y=0,i=0; i<rows; ++i) {
		p_row = Queue_Get( &layer->rows_data, i );
		n = Queue_Get_Total( &p_row->string );
		//DEBUG_MSG("char num: %d\n", n);
		for(pos.x=0,j=0; j<n; ++j) {
			p_data = Queue_Get( &p_row->string, j );
			//DEBUG_MSG("j:%d, char: %c, p:%p, data: %p\n", j, p_data->char_code, p_data, p_data->data);
			if( p_data->data != NULL ) {
				if( p_data->data->pixel_size > 0 ) {
					fontheight = p_data->data->pixel_size;
				} else { 
					fontheight = layer->default_data.pixel_size;
				}
				fontheight += 2;
				
				if( p_data->data->need_fore_color ) {
					color = p_data->data->fore_color;
				} else {
					color = layer->default_data.fore_color;
				}
			} else {
				fontheight = layer->default_data.pixel_size + 2; 
				color = layer->default_data.fore_color;
			}
			//DEBUG_MSG("RGB: %d,%d,%d\n", color.red, color.green, color.blue);
			
			pos.x += p_data->bitmap.left;
			FontBMP_Mix( &widget->graph, Pos( pos.x, 
				pos.y + fontheight - p_data->bitmap.top),
				&p_data->bitmap, color, mode );
			Add_Widget_Refresh_Area( widget, Rect(pos.x, pos.y, 
						fontheight, fontheight) );
			pos.x += p_data->bitmap.width;
			if( p_data->need_update ) {
				
			}
		}
		pos.y += p_row->max_size.h;
	}
}

void
TextLayer_Refresh( LCUI_TextLayer *layer )
/* ����ı�ͼ����ÿ���ֵ�λͼ���ȴ������ı�ͼ��ʱ���и��� */
{
	
}
/**********************************************************************/

/************************ �ı�ͼ�㲿������չ���� ****************************/
/* ���а� */
//static LCUI_String clip_board;

LCUI_Size 
TextLayer_Get_Size ( LCUI_TextLayer *layer )
/* ��ȡ�ı�ͼ���ʵ�ʳߴ� */
{
	int i, rows;
	LCUI_Size size;
	Text_RowData *p_row; 
	
	rows = Queue_Get_Total( &layer->rows_data );
	for(size.w=0,size.h=0,i=0; i<rows; ++i,++size.h) {
		p_row = Queue_Get( &layer->rows_data, i );
		if( size.w < p_row->max_size.w ) {
			size.w = p_row->max_size.w;
		}
		size.h += p_row->max_size.h;
	}
	return size;
}

void
TextLayer_Text_Set_Default_Style( LCUI_TextLayer *layer, LCUI_TextStyle style )
/* �趨Ĭ�ϵ��ı���ʽ */
{
	memcpy( &layer->default_data, &style, sizeof(LCUI_TextStyle) );
}

void
TextLayer_Clear( LCUI_TextLayer *layer )
/* ����ı����� */
{
	Destroy_Queue( &layer->text_source_data );
	Destroy_Queue( &layer->rows_data );
	Destroy_Queue( &layer->style_data );
	layer->current_src_pos = 0;
	layer->current_des_pos = Pos(0,0);
}

int TextLayer_Text_Length( LCUI_TextLayer *layer )
/* ��ȡ�ı�λͼ�е��ı����� */
{
	return Queue_Get_Total( &layer->text_source_data );
}

int
TextLayer_Text( LCUI_TextLayer *layer, char *new_text )
/* �趨�����ı�ͼ��������ʾ���ı���ԭ��ѡ���ı���ɾ�� */
{
	TextLayer_Clear( layer );
	return TextLayer_Text_Add( layer, new_text );
}

int 
TextLayer_Text_Append( LCUI_TextLayer *layer, char *new_text )
/* ���ı�ĩβ׷���ı������ƶ���꣬��ɾ��ԭ��ѡ���ı� */
{
	return 0;
}

int 
TextLayer_Text_Add( LCUI_TextLayer *layer, char *new_text )
/* �ڹ�괦����ı�������ѡ���ı�������ɾ�� */
{
	int total; 
	uint32_t rows, tt, n_ignore = 0;
	wchar_t *finish, *buff, *p, *q;
	
	LCUI_CharData char_data; 
	Text_RowData *current_row_data;
	
	/* �����ѡ�е��ı����Ǿ�ɾ�� */
	//...... 
	tt = 0;
	total = Char_To_Wchar_T( new_text, &buff ); 
	//DEBUG_MSG("total: %d\n", total);
	current_row_data = TextLayer_Get_Current_RowData ( layer );
	if( !current_row_data ) {
		TextLayer_Text_Add_NewRow( layer );
		current_row_data = TextLayer_Get_Current_RowData ( layer );
	}
	/* ������ʽ��ǩ���ɶ�Ӧ����ʽ���� */
	for(p=buff, finish=buff+total; p<finish; ++p) { 
		if( layer->using_style_tags ) {
			/* ������ʽ�Ľ�����ǩ */
			//DEBUG_MSG("using style tags\n");
			q = handle_style_endtag ( layer, p );
			if(q != NULL) { 
				//DEBUG_MSG("found style endtag\n");
				/* ��������Ե��ַ��� */
				n_ignore = q-p+1;
			} else {
				/* ������ʽ��ǩ */
				q = handle_style_tag ( layer, p ); 
				if( q != NULL ) {
					//DEBUG_MSG("found style tag\n");
					n_ignore = q-p+1;
				}
			}
		}
		if(*p == '\n') { 
			/* ������Ҫ���ԵĻ��з������� */
			for( n_ignore=0,q=p; *q == '\n'; ++q,++n_ignore);
		}
		if(n_ignore > 0) {
			/* �����Ե��ַ������Զ�һ��������ֻ�踳һ��ֵ */
			char_data.data = NULL;
			char_data.display = IS_FALSE; 
			char_data.need_update = IS_FALSE; 
			FontBMP_Init( &char_data.bitmap );
			//DEBUG_MSG("ignore char: \n");
			while(n_ignore--) { 
				char_data.char_code = *p++;
				//DEBUG_MSG("current pos: %d, char %c\n", layer->current_src_pos, char_data.char_code);
				//DEBUG_MSG("[%02d]char:%c, code: %d\n", layer->current_src_pos, char_data.char_code, char_data.char_code);
				Queue_Insert( &layer->text_source_data, 
					layer->current_src_pos, &char_data );
				/* �������з����Ǿ��������� */
				if(char_data.char_code == '\n') {
					//DEBUG_MSG("add new row\n");
					TextLayer_Update_RowSize( layer, layer->current_des_pos.y );
					rows = TextLayer_Text_Add_NewRow( layer );
					current_row_data = Queue_Get( &layer->rows_data, rows );
					layer->current_des_pos.x = 0;
					layer->current_des_pos.y = rows;
				} else {
					++layer->current_src_pos; 
				}
			}
			n_ignore = 0;
			--p;
			tt=0;
			continue;
		} else {
			if( tt == 0) {
				//DEBUG_MSG("normal char: \n");
			}
			tt++;
		}
		
		char_data.char_code = *p;
		char_data.display = IS_TRUE; 
		char_data.need_update = IS_TRUE;
		/* ��ȡ��ǰ������ʽ���� */
		char_data.data = TextLayer_Get_Current_TextStyle( layer );
		//DEBUG_MSG("char: %c, code: %d, style data:%p\n", *p, *p, char_data.data);
		/* ��ȡ����λͼ */
		FontBMP_Init( &char_data.bitmap );
		TextLayer_Get_Char_BMP ( &char_data );
		/* ������� */
		Queue_Insert( &layer->text_source_data, 
				layer->current_src_pos, &char_data ); 
		/* ��Ӹ��ֵ����ݵ�����ָ�������� */
		Queue_Insert( &current_row_data->string, 
				layer->current_des_pos.x, &char_data ); 
		
		/* ��¼��һ���ı��Ŀ�Ⱥ͸߶� */
		//current_row_data->max_size.w += char_data.bitmap.width;
		//current_row_data->max_size.w += char_data.bitmap.left;
		
		//if( char_data.data != NULL) {
			//if( char_data.data->pixel_size > 0 ) {
				//bitmap_height = char_data.data->pixel_size;
			//} else {
				//bitmap_height = 12;
			//}
			//bitmap_height += 2;
		//} else {
			//bitmap_height = 14;
		//}
		
		//if( current_row_data->max_size.h < bitmap_height ) {
			//current_row_data->max_size.h = bitmap_height;
		//}
		//DEBUG_MSG("[%02d]char:%c, code: %d\n", layer->current_src_pos, char_data.char_code, char_data.char_code);
		++layer->current_src_pos;
		++layer->current_des_pos.x;
	}
	TextLayer_Update_RowSize( layer, layer->current_des_pos.y );
	return 0;
}

int 
TextLayer_Text_Paste( LCUI_TextLayer *layer )
/* �����а������ճ�����ı�ͼ�� */
{
	return 0;
}


int 
TextLayer_Text_Backspace( LCUI_TextLayer *layer, int n )
/* ɾ�������ߴ�n���ַ� */
{
	return 0;
}

int 
TextLayer_Text_Delete( LCUI_TextLayer *layer, int n )
/* ɾ������ұߴ�n���ַ� */
{
	return 0;
}

LCUI_Pos 
TextLayer_Get_Pixel_Pos( LCUI_TextLayer *layer, uint32_t char_pos )
/* ����Դ�ı��е�λ�ã���ȡ��λ�õ��ַ�������ı�ͼ������� */
{
	LCUI_Pos pos;
	pos.x = pos.y = 0;
	return pos;
}

uint32_t 
TextLayer_Get_Char_Pos( LCUI_TextLayer *layer, LCUI_Pos pixel_pos )
/* �����ı�ͼ���������꣬��ȡ�������Ӧ��Դ�ı��е��ַ� */
{
	return 0;
}

int 
TextLayer_Get_Select_Text( LCUI_TextLayer *layer, char *out_text )
/* ��ȡ�ı�ͼ���ڱ�ѡ�е��ı� */
{ 
	/* ���ѡ�����ı� */
	if( layer->have_select ) {
		/* ��ȡѡ�е��ı����� */
		//......
		return 0;
	}
	return -1;
}

int 
TextLayer_Copy_Select_Text( LCUI_TextLayer *layer )
/* �����ı�ͼ���ڱ�ѡ�е��ı� */
{
	/* ���ѡ�����ı� */
	if( layer->have_select ) {
		/* ��ѡ�е��ı�������������а� */
		//......
		return 0;
	}
	return -1;
}

int 
TextLayer_Cut_Select_Text( LCUI_TextLayer *layer )
/* �����ı�ͼ���ڱ�ѡ�е��ı� */
{
	/* ���ѡ�����ı� */
	if( layer->have_select ) {
		/* ��ѡ�е��ı�������������а棬��ɾ����ѡ�е��ı� */
		//......
		return 0;
	}
	return -1;
}

void 
TextLayer_Using_StyleTags( LCUI_TextLayer *layer, BOOL flag )
/* ָ���ı�ͼ���Ƿ�����ʽ��ǩ */
{
	layer->using_style_tags = flag;
}

/**********************************************************************/
