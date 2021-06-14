/* ***************************************************************************
 * LCUI.h -- Records with common data type definitions, macro definitions and 
 * function declarations
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
 * LCUI.h -- ��¼�ų��õ��������Ͷ��壬�궨�壬�Լ���������
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

/* Ϊ�˽���ṹ�屻�ظ����壬�ú����ͷ�ļ�����(�����ط��������Ƶ����ݣ�������ע��) */
#ifndef __LCUI_H__  /* ���û�ж��� __LCUI_H__ �� */
#define __LCUI_H__  /* ���� __LCUI_H__ �� */ 
#define LCUI_VERSION "0.12.4"
//#define __TSLIB__ /* �Ƿ�֧�ִ��� */ 
#include <pthread.h>
#include <ft2build.h>
#include <tslib.h>
#include FT_FREETYPE_H 

/* ���ļ�ʱ�Ĵ��� */
#define SHORT_FILE		1
#define BIG_FILE		2
#define UNKNOWN_FORMAT  3
#define OPEN_ERROR		4

/* ͼ������ */
#define TYPE_PNG		1
#define TYPE_JPG		2
#define TYPE_BMP		3

#define PNG_BYTES_TO_CHECK 4

/* ״̬ */ 
#define ACTIVE	1
#define KILLED	-1
#define REMOVE	-1
#define INSIDE	1

#define NO_ALPHA		0	 /* ��alphaͨ�� */
#define HAVE_ALPHA		1	 /* ��alphaͨ�� */

#define YES	1		 /* �� */
#define NO	0		 /* �� */

#define NONE	0		 /* �� */

#define VIEW	0		 /* ��ʾ */
#define HIDE	1		 /* ����  */ 

#define DEFAULT		0	 /* ȱʡ */
#define CUSTOM		1	 /* �Զ��� */

/***************** һЩ��������豸 *********************/
#define FB_DEV		"/dev/fb0"			/* ͼ������豸 */
#define TS_DEV		"/dev/jz_ts"		/* ���������豸 */
#define MS_DEV		"/dev/input/mice"	/* ����豸 */
/*****************************************************/

enum _BOOL
{
	IS_FALSE = 0,
	IS_TRUE = 1,
	IS_ERROR = -1
};

#define FORCE			2

/* ����У׼����ļ� */
#define LCUI_CALIBFILE "/mnt/Data/LC-SOFT/pointercal"

/* ������ͼ�εĻ�Ϸ�ʽ */
#define GRAPH_MIX_FLAG_OVERLAY	 0
#define GRAPH_MIX_FLAG_REPLACE	 1
/* Ĭ�ϵ������ļ�·�� */
#define LCUI_DEFAULT_FONTFILE	"../fonts/msyh.ttf"

/* ����¼� */
#define MOUSE_EVENT_MOVE		1
#define MOUSE_EVENT_CLICK		2

#define LABEL_TEXT_MAX_SIZE			2048

/****************** ͼ��Ĳ��� *****************/
#define LAYOUT_ZOOM		1	  /* ���� */
#define LAYOUT_STRETCH	2	  /* ���� */
#define LAYOUT_CENTER	3	  /* ���� */
#define LAYOUT_TILE		4	  /* ƽ�� */
#define LAYOUT_NONE		0	  /* �� */
#define LAYOUT_NORMAL	0 
/**********************************************/



/************ ��������ģʽ ***************/

#define ADD_MODE_ADD_NEW		0	/* ���� */
#define ADD_MODE_NOT_REPEAT		1	/* �����ظ� */
#define ADD_MODE_REPLACE		2	/* ���� */

#define AND_ARG_F				1<<3	/* ��һ������ */
#define AND_ARG_S 				1<<4	/* �ڶ������� */ 
/*****************************************/



#define LCUI_Menu_Style_Default		20
#define LCUI_Style_Default			1
#define LCUI_Style_Standard			2 /* LCUI��׼��� */
#define Custom_Style				5
#define NONE						0

/* ������λ������ */
#define POS_TYPE_IN_WIDGET	1
#define POS_TYPE_IN_SCREEN	0

#define ICON_CUSTOM 2
#define ICON_LCUI   1
#define ICON_None   0


/****************** ͼ��Ĵ���ʽ *****************/
#define FILL_MODE_ZOOM		LAYOUT_ZOOM	/* ���� */
#define FILL_MODE_STRETCH   LAYOUT_STRETCH /* ���� */
#define FILL_MODE_CENTER	LAYOUT_CENTER  /* ���� */
#define FILL_MODE_TILE		LAYOUT_TILE	/* ƽ�� */
#define FILL_MODE_NONE		LAYOUT_NONE	/* �� */
#define FILL_MODE_NORMAL	LAYOUT_NORMAL 
/*************************************************/

/* ��Ϣ��������ʾ��ͼ�� */
#define ICON_Warning	1
#define ICON_Error		2
#define ICON_Question	3
#define ICON_Asterisk	4
/* ��Ϣ����ʹ�õİ������� */
#define MB_None					0
#define MB_Abort_Retry_Ignore	1
#define MB_OK					2
#define MB_OK_Cancel			3
#define MB_Retry_Cancel			4
#define MB_Yes_No				5
#define MB_Yes_No_Cancel		6

/*************** �߿��� *****************/
enum _LCUI_Border_Style
{
	BORDER_STYLE_NONE,
	BORDER_STYLE_STANDARD,
	BORDER_STYLE_LINE_BORDER,
	BORDER_STYLE_ROUND_BORDER
};
/*****************************************/

/************ �����ı���ģʽ ***************/
enum _LCUI_BG_Mode
{
	BG_MODE_TRANSPARENT,
	BG_MODE_FILL_BACKCOLOR
};
/*****************************************/

/*********************** ������һЩ���뷽ʽ *********************/
enum _LCUI_Align
{
	ALIGN_NONE,				/* �� */
	ALIGN_TOP_LEFT,	  		/* �����ϽǶ��� */
	ALIGN_TOP_CENTER,		/* �����м���� */
	ALIGN_TOP_RIGHT,		/* �����ϽǶ��� */
	ALIGN_MIDDLE_LEFT,		/* ������ƫ����� */
	ALIGN_MIDDLE_CENTER,	/* ����������� */
	ALIGN_MIDDLE_RIGHT,		/* ������ƫ�ɶ��� */
	ALIGN_BOTTOM_LEFT,		/* ��ײ�ƫ����� */
	ALIGN_BOTTOM_CENTER,	/* ��ײ����ж��� */
	ALIGN_BOTTOM_RIGHT		/* ��ײ�ƫ�Ҷ��� */
};
/*****************************************************************/

/*************** �����ļ���״̬ ***************/
enum _LCUI_Widget_Status
{
	WIDGET_STATUS_NORMAL,	/* ��ͨ״̬ */
	WIDGET_STATUS_CLICKED,	/* ������������ɿ� */
	WIDGET_STATUS_OVERLAY,	/* ����긲�� */
	WIDGET_STATUS_CLICKING,	/* ���������δ�ɿ� */
	WIDGET_STATUS_FOCUS,	/* ���� */
	WIDGET_STATUS_DISABLE	/* ������ */
};
/*******************************************/

/****************** ͼ��Ĵ���ʽ *****************/
#define SIZE_MODE_ZOOM		LAYOUT_ZOOM		/* ���� */
#define SIZE_MODE_STRETCH	LAYOUT_STRETCH	/* ���� */
#define SIZE_MODE_CENTER	LAYOUT_CENTER	/* ���� */
#define SIZE_MODE_TILE		LAYOUT_TILE		/* ƽ�� */
#define SIZE_MODE_NONE		LAYOUT_NONE		/* �� */
#define SIZE_MODE_NORMAL	LAYOUT_NORMAL 
/*************************************************/


/*********** ͼ�����ı��ļ������λ�� *******************/
#define Overlay			10   /* ���ָ�����ͼƬ���� */
#define ImageAboveText	11   /* ͼƬ���ϣ��������� */
#define TextAboveImage	12   /* ͼƬ���£��������� */
#define ImageBeforeText	13   /* ͼƬ������������ */
#define TextBeforeImage	14   /* ͼƬ���ң��������� */
/***************************************************/



typedef unsigned long int LCUI_ID; 
/* ��ʹ��typedefΪ�ṹ�崴��ͬ���֣�֮���ٶ���ṹ�� */
typedef struct	_LCUI_System			LCUI_System;
typedef struct	_LCUI_Cursor			LCUI_Cursor;
typedef struct	_LCUI_App				LCUI_App;
typedef struct	_LCUI_WString			LCUI_WString;
typedef struct	_LCUI_Border			LCUI_Border;
typedef struct	_LCUI_WChar_T			LCUI_WChar_T;
typedef struct	_LCUI_Rect				LCUI_Rect; 
typedef struct	_LCUI_String			LCUI_String;
typedef struct	_LCUI_Widget			LCUI_Widget;  
typedef struct	_LCUI_RGB				LCUI_RGB;
typedef struct	_LCUI_RGBA				LCUI_RGBA;
typedef struct	_LCUI_ImageInfo		LCUI_ImageInfo;
typedef struct	_LCUI_Bitmap			LCUI_Bitmap;
typedef struct	_LCUI_Graph			LCUI_Graph;
typedef struct	_LCUI_Font				LCUI_Font;
typedef struct	_LCUI_TS				LCUI_TS;
typedef struct	_LCUI_Mouse			LCUI_Mouse;
typedef struct	_LCUI_MouseEvent		LCUI_MouseEvent;
typedef struct	_LCUI_Key				LCUI_Key;
typedef struct	_LCUI_Pos				LCUI_Pos;
typedef struct	_LCUI_Size				LCUI_Size;
typedef struct	_Thread_Queue			Thread_Queue;
typedef struct	_Thread_TreeNode		Thread_TreeNode;
typedef struct	_LCUI_Screen			LCUI_Screen;
typedef struct	_Dirty_Rect			Dirty_Rect;
typedef struct	_Matrix				Matrix;
typedef struct	_Pixel					Pixel; 
typedef struct	_LCUI_Queue			LCUI_MovObjQueue;
typedef struct	_LCUI_Queue 			LCUI_AppList;


typedef enum	_LCUI_Border_Style		LCUI_Border_Style;
typedef enum	_LCUI_BG_Mode				LCUI_BG_Mode;
typedef enum	_LCUI_Widget_Status		LCUI_Widget_Status; 
typedef enum	_LCUI_Align				LCUI_Align;

/********** ������Ϣ ************/
struct _LCUI_Key
{
	int code;
	int status;
};
/******************************/

/************ ����RGB��ɫ���� *************/
struct _LCUI_RGB
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}; 
/****************************************/

/************ ����RGBA��ɫ���� *************/
struct _LCUI_RGBA
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
}; 
/****************************************/


/***************** �����¼� ************************/
struct _LCUI_Widget_Event
{
	int		status;		/* ��¼������Ҫ�ı�����ĳ��״̬ */
	void	*arg;		/* ���ӵĲ��� */
};
/**************************************************/


/*********** ���� ************/
struct _LCUI_Pos
{
	int x, y;
};
/*****************************/

/************* �ߴ� *************/
struct _LCUI_Size
{
	int w, h;
};
/*******************************/

#include LC_QUEUE_H
#include LC_WORK_H

/********************** ��������Ϣ ***************************/
struct _LCUI_Mouse
{
	int fd, status;  /* �����״̬ */
	float move_speed;
	pthread_t thread;
	LCUI_EventQueue event;
};
/*************************************************************/


/************************ ����¼� **************************/
struct _LCUI_MouseEvent
{
	LCUI_Widget *widget;	/* ��ǰ���ָ�븲�ǵ��Ĳ��� */
	LCUI_Key	key;		/* ������ֵ�Լ�״̬ */
	LCUI_Pos	global_pos;	/* ���ָ���ȫ������ */
	LCUI_Pos	pos;		/* �뵱ǰ���ǵ��Ĳ������������ */
};
/***********************************************************/


/*********** ������� **************/
struct _LCUI_TS
{
	struct tsdev *td;
	int status;  /* ״̬ */ 
	pthread_t thread;
};
/**********************************/
 
/*************** ��¼���ص���Ϣ *******************/
struct _Pixel
{
	LCUI_Pos	pos;	/* λ�� */ 
	LCUI_RGB	rgb;	/* RGBAֵ */
};
/***********************************************/

/***************************** ����� *********************************/
struct _Dirty_Rect
{
	int				redraw;			/* 
									 * ָ���Ƿ��ػ棬ͨ���ڽ��оֲ�ˢ��ʱ����
									 * �������Լ��ٸþֲ����򲻱�Ҫ��ͼ�λ��
									 * ���� 
									 * */
									 
	LCUI_Size		size;					/* �ߴ� */
	Pixel			*first_sample_point;	/* �����һ�β����Ĳ����� */
	Pixel			*second_sample_point;	/* ����ڶ��β����Ĳ����� */
	int				total_point;			/* �ܲ������� */
};
/*********************************************************************/

/***************************** ���� *********************************/
struct _Matrix
{
	Dirty_Rect		**rect;				/* ��ɾ��������� */
	LCUI_Size		size, pixel_size;	/* ��ͬ��λ�ĳߴ� */
	LCUI_Pos		pos;				/* ���ڼ�¼����Ļ��������λ�� */
};
/********************************************************************/


/****************************** ͼƬ��Ϣ *********************************/
struct _LCUI_ImageInfo
{
	int			   type;			/* �洢ͼƬ���� */
	int			   width, height;	/* �� */
	int			   bit_depth;		/* λ�� */
	int			   channel;			/* �Ƿ���Ҫ͸���ȣ������ڴ�ʱ����������� */
};/*  ͼƬ��Ϣ */
/************************************************************************/

/*******************��������λͼ����***************************/
struct _LCUI_Bitmap
{
	unsigned char  **data;	  /* ����λͼ���� */
	unsigned char  alpha;	   /* ȫ��͸���� */
	
	int	width;	   /* λͼ�Ŀ�� */
	int	height;	  /* λͼ�ĸ߶� */
	int	malloc;	  /* �Ƿ�������ڴ����ڴ洢 */ 
};/* ����λͼ���� */
/**************************************************************/

/******************************ͼƬ����*********************************/
struct _LCUI_Graph
{
	int	type;					/* �洢ͼƬ���� */
	int	width, height;			/* �ߴ� */
	int	bit_depth;				/* λ�� */
	
	unsigned char	**rgba;		/* ͼƬ���� */
	unsigned char	alpha;		/* ȫ��͸���ȣ���ʾ����ͼƬ��͸���ȣ�Ĭ��Ϊ255 */
	int				flag;		/* �Ƿ���Ҫ͸���ȣ������ڴ�ʱ����������� */
	int				malloc;		/* �Ƿ�������ڴ� */
	thread_rwlock	lock;		/* �����������ݱ��� */
};/*  �洢ͼƬ���� */
/**********************************************************************/

/*********** �ַ��� ***********/
struct _LCUI_String
{
	char   *string; /* �ַ������� */
	size_t size;	/* ռ�õĿռ��С */
};
/*****************************/


/****************** ��������1 **********************/
struct _LCUI_Rect /* �ɼ�¼��ˢ�µ����� */
{
	int x,y;
	int width,height;
	double center_x,center_y; /* ���ĵ������ */
};
/**************************************************/

/************** wchar_t���ַ��� ***********/
struct _LCUI_WString
{
	LCUI_WChar_T *string;
	int	update;
	size_t size;
};
/*****************************************/


/******************************����������Ϣ********************************/
struct _LCUI_Font/* ������Ϣ���� */
{
	int				type;			/* ����(DEFAULT / CUSTOM) */
	unsigned int	size;			/* �����С */
	LCUI_RGB		fore_color;		/* ǰ��ɫ */
	LCUI_String	font_file;		/* �����ļ���·�� */
	LCUI_String	family_name;	/* �������� */
	LCUI_String	style_name;		/* ���������� */
	int				space;			/* ��������Ҽ�ࣨ��λΪ���أ� */
	int				linegap;		/* ������оࣨ��λΪ���أ� */
	FT_Library		ft_lib;			/* FreeType2��ľ��  */
	FT_Face			ft_face;		/* FreeType2��face����ľ�� */
	int				load_flags;		/* ���������־ */
	int				render_mode;	/* ����ת��ģʽ��־ */
	int				status;			/* ״̬���Ƿ��������� */
};
/************************************************************************/



/*************** �߿� ******************/
struct _LCUI_Border
{
	int left,top,right,bottom;
};
/*************************************/

/************************wchar_t���ַ�***************************/
struct _LCUI_WChar_T
{
	wchar_t			char_code;		/* �ַ��� */
	LCUI_Bitmap	bitmap;			/* �ַ���λͼ���� */
	LCUI_RGB		color;			/* �����ֵ���ɫ */
	int				update;			/* ��������ַ��Ƿ���Ҫˢ�� */ 
	int				color_type;		/* ��ɫ����(DEFAULT / CUSTOM) */		   
};
/****************************************************************/

/******************************* ���� **********************************/
struct _LCUI_Widget /* �洢���������пؼ���ͼ������ */
{ 
	int		lock_display;	/* ָʾ�Ƿ�������������ʾ˳�� */
	
	LCUI_String	type;		/* ���������� */
	LCUI_ID		type_id;	/* ����������ID */
	LCUI_String	style;		/* �����ķ�񣬶�ĳЩ������Ч */
	
	LCUI_Widget	*parent;	/* ���游������ָ�� */
	LCUI_Queue		child;		/* �����Ӳ�����ָ�� */
	
	LCUI_ID app_id;	/* ���������id */
	
	int response_flag;	/* ��־��ָʾ�Ƿ���Ӧ������״̬�ı� */
	
	int enabled;		/* �����Ƿ����� */
	int visible;		/* ȷ���ò����ǿɼ��Ļ������ص� */
	int status;			/* ������״̬ */
	
	void *private;   /* �ò���˽�е����ݵ�ָ�룬�������Ǹ����������õ����� */
	
	LCUI_Pos	pos;		/* λ�� */
	/* max_pos��min_pos������widget���ƶ���Χ */
	int			limit_pos;	/* ָ���Ƿ����Ʋ�����λ�� */
	LCUI_Pos	max_pos;	/* ���λ�� */
	LCUI_Pos	min_pos;	/* ��Сλ�� */
	int			pos_type;	/* λ������ */
	int			align;		/* �����Ĳ��� */
	LCUI_Pos	offset;		/* xy���ƫ���� */
	
	int			auto_size;	/* ָ���Ƿ��Զ���������Ĵ�С������Ӧ���ݵĴ�С */ 
	int			limit_size;	/* ָ���Ƿ����Ƴߴ��С */
	LCUI_Size	size;		/* �����ĳߴ磬��λΪ���� */
	/* max_size��min_size������widget�ĳߴ�䶯��Χ */
	LCUI_Size	max_size;	/* ���ߴ� */
	LCUI_Size	min_size;	/* ��С�ߴ� */
	
	LCUI_RGB  back_color;  /* ����ɫ */
	LCUI_RGB  fore_color;  /* ǰ��ɫ */
	
	LCUI_Border	border;			/* �߿� */
	LCUI_RGB		border_color;	/* �߿���ɫ */
	int				border_style;	/* �߿�����(NONE:û��) */
	
	LCUI_Queue		event;			/* ���沿�����¼����������� */
	LCUI_Queue		update_area;	/* ��������Ҫˢ�µ����� */ 
	
	LCUI_Queue		data;		/* ��¼��Ҫ���и��µ����� */
	
	int				bg_mode;					/* ����ģʽ��ָ��û�б���ʱ��ʹ��͸����������ʹ�ñ���ɫ��� */
	LCUI_Graph		background_image;			/* ����ͼ */
	int				background_image_layout;	/* ����ͼ�Ĳ��� */
	
	LCUI_Widget	*focus; /* ��ý���Ĳ��� */
	LCUI_Graph		graph; /* ������ͼ������ */ 
	/* �����Ǻ���ָ�� */
	void (*resize)(LCUI_Widget*, LCUI_Size);
	void (*move)(LCUI_Widget*, LCUI_Pos); 
	void (*show)(LCUI_Widget*);
	void (*hide)(LCUI_Widget*);
	void (*disable)(LCUI_Widget*);
	void (*enable)(LCUI_Widget*);
	void (*set_align)(LCUI_Widget*, LCUI_Align, LCUI_Pos); 
	void (*set_alpha)(LCUI_Widget*, unsigned char); 
	void (*set_border)(LCUI_Widget*, LCUI_RGB, LCUI_Border);
};
/**********************************************************************/



/************************ LCUI��������� *******************************/
struct _LCUI_App 
{
	int			stop_loop;	/* һ����־������ָʾ�Ƿ�ֹͣ��ѭ�� */
	int			flag;		/* һ����־��Ŀǰ�ǽ�����Ϊ�Ƿ������ֲ�ˢ���������ݵı�־ */ 
	LCUI_ID	id;			/* LCUI�����ID,�������̷߳�ʽ���еĻ�����������߳�ID */ 
	
	LCUI_Queue		task_queue;		/* ������������ */
	LCUI_Queue		widget_lib;		/* �������Ϳ� */
};
/**********************************************************************/


/***********  �����α��λ�� **************/
struct _LCUI_Cursor
{
	LCUI_Pos	pos;
	int			visible;
	LCUI_Graph	graph;
};
/****************************************/

#define MAX_APP_IDLE_TIME	50000
#define MAX_LCUI_IDLE_TIME  50000



/************ �̶߳��� **************/
struct _Thread_Queue
{
	Thread_TreeNode **queue;	/* ������г�Ա */
	int max_num;				/* ����Ա���� */
	int total_num;				/* ��ǰ��Ա���� */
};
/***********************************/

/****************************** ��Ļ��Ϣ ********************************/
struct _LCUI_Screen
{
	LCUI_Size		size;			/* ��Ļ�ߴ� */
	LCUI_Graph		buff;			/* ��������LCUIǰ����Ļ���� */
	char			*fb_dev_name;	/* ͼ������豸������ */
	unsigned char	*fb_mem;		/* ָ��ͼ������豸ӳ�䵽���ڴ��ָ�� */
	int				fb_dev_fd;		/* ͼ����ʾ�豸�ľ�� */
	size_t			smem_len;		/* �ڴ�ռ�Ĵ�С */
	int				bits;			/* ÿ�����ص��ö���λ��ʾ */
	//Matrix			*matrix;		/* ��Ļ����ξ��� */
};
/***********************************************************************/

/************ �߳����Ľ�� ***************/
struct _Thread_TreeNode
{
	Thread_TreeNode *parent;	/* ���߳̽��ָ�� */
	pthread_t tid;				/* ���߳�ID */
	Thread_Queue child;		/* ���̶߳��� */
};
/***************************************/


/***************************����LCUI������ *****************************/
struct _LCUI_System
{
	Thread_TreeNode thread_tree; /* �̹߳�ϵ�� */
	
	pthread_t	self_id;		/* ����LCUI��������߳�ID */
	pthread_t	core_thread;	/* ������Ĵ�����߳�ID */
	pthread_t	key_thread;		/* ���水��������߳�ID */
	
	int		status;		/* ״̬ */
	int		init;		/* һ����־����ʾLCUI�Ƿ��ʼ���� */
	int		shift_flag;	/* һ����־����ʾ�Ƿ���Ҫת�Ʋ����м�¼���������� */
	
	LCUI_ID max_app_idle_time;	/* �������Ŀ���ʱ�� */
	LCUI_ID max_lcui_idle_time; /* LCUI���Ŀ���ʱ�� */
	
	LCUI_Cursor	cursor;		/* �α� */
	LCUI_Mouse		mouse;		/* ��� */
	LCUI_TS		ts;		 	/* ���� */
	
	LCUI_Screen	screen;			/* ��Ļ��Ϣ */
	
	LCUI_Queue		press_key;		/* �����ѱ����µİ����ļ�ֵ */
	LCUI_Queue		key_event;		/* �����밴���¼����������� */
	
	LCUI_Queue			widget_list;	/* �������У���Ӧ������ʾ˳�� */
	LCUI_AppList		app_list;		/* LCUI�����б� */ 
	LCUI_Font			default_font;	/* Ĭ�ϵ��������� */ 
	LCUI_Queue			update_area;	/* ��������Ҫˢ�µ����� */
};
/***********************************************************************/

extern LCUI_System  LCUI_Sys;
void Set_Default_Font(char *fontfile);
/* 
 * ���ܣ��趨Ĭ�ϵ������ļ�·��
 * ˵������Ҫ��LCUI��ʼ��ǰʹ�ã���ΪLCUI��ʼ��ʱ���Ĭ�ϵ������ļ�
 *  */ 

/************************* App Manage *********************************/
LCUI_App *Find_App(LCUI_ID id);
/* ���ܣ����ݳ����ID����ȡָ��������ݽṹ��ָ�� */ 

LCUI_App* Get_Self_AppPointer();
/* ���ܣ���ȡ�����ָ�� */ 

int App_Quit();
/* ���ܣ��˳����� */ 

void Main_Loop_Quit();
/* ���ܣ��ó����˳���ѭ�� */ 
/*********************** App Manage End ******************************/

int Get_Screen_Width ();
/*
 * ���ܣ���ȡ��Ļ���
 * ����ֵ����Ļ�Ŀ�ȣ���λΪ���أ�������ʹ��LCUI_Init()������ʹ�ã�������Ч
 * */ 

int Get_Screen_Height ();
/*
 * ���ܣ���ȡ��Ļ�߶�
 * ����ֵ����Ļ�ĸ߶ȣ���λΪ���أ�������ʹ��LCUI_Init()������ʹ�ã�������Ч
 * */ 

LCUI_Size Get_Screen_Size ();
/* ���ܣ���ȡ��Ļ�ߴ� */ 

void Fill_Pixel(LCUI_Pos pos, LCUI_RGB color);
/* ���ܣ����ָ��λ�õ����ص����ɫ */ 

void Write_Graph_To_FB (LCUI_Graph * src, LCUI_Pos pos);
/* 
 * ���ܣ�д��ͼ��������֡���棬�Ӷ�����Ļ����ʾͼ�� 
 * ˵�����˺�������Ҫ���룬�ο���mgaveiw 0.1.4 ��mga_vfb.c�ļ��е�
 * write_to_fb������Դ����.
 * */
 
void Catch_Screen_Graph_By_Cache(LCUI_Rect area, LCUI_Graph *out);
/* 
 * ���ܣ�ͨ���ڴ��е�ͼ�����ݣ���������Ļ����ʾ��ͼ��
 * ˵����Ч�ʽϵͣ���Ϊ��Ҫ���кϳɡ�
 *  */ 

void Catch_Screen_Graph_By_FB (LCUI_Rect area, LCUI_Graph *out);
/* 
 * ���ܣ�ֱ�Ӷ�ȡ֡�����е�ͼ������
 * ˵����Ч�ʽϸߣ��������ͼ���п��ܻ������⡣
 * */ 

int Add_Screen_Refresh_Area (LCUI_Rect rect);
/* ���ܣ���������Ļ�������Ҫˢ�µ����� */ 

void nobuff_print(const char *fmt, ...);
/* ���ܣ���ӡ��ִ�еĲ�������Ϣ */

int Get_Screen_Bits();
/* ���ܣ���ȡ��Ļ�е�ÿ�����صı�ʾ���õ�λ�� */ 

LCUI_Pos Get_Screen_Center_Point();
/* ���ܣ���ȡ��Ļ���ĵ������ */ 

int LCUI_Active();
/* ���ܣ����LCUI�Ƿ� */ 

int LCUI_Init(int argc, char *argv[]);
/* 
 * ���ܣ����ڶ�LCUI���г�ʼ������ 
 * ˵����ÿ��ʹ��LCUIʵ��ͼ�ν���ĳ��򣬶���Ҫ�ȵ��ô˺�������LCUI�ĳ�ʼ��
 * */ 

int Need_Main_Loop(LCUI_App *app);
/* ���ܣ������ѭ���Ƿ���Ҫ�������� */ 

int LCUI_Main ();
/* 
 * ���ܣ�LCUI�������ѭ��
 * ˵����ÿ��LCUI������Ҫ���������˺������ó���ִ��LCUI���������
 *  */ 

int Get_LCUI_Version(char *out);
/* ���ܣ���ȡLCUI�İ汾 */
#endif /* __LCUI_H__ */



