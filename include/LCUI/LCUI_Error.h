/* ***************************************************************************
 * LCUI_Error.c -- LCUI��s errors handling, at present, only macro definition
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
 * LCUI_Error.h -- LCUI �Ĵ�����Ŀǰֻ�к궨��
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


#define APP_ERROR_UNRECORDED_APP	"error: app was never recorded\n"
#define ERROR_MALLOC_ERROR			"error: can not allocate memory space\n"

#define FT_INIT_ERROR                 "init error!\n"
#define FT_UNKNOWN_FILE_FORMAT        "unknown file format!\n"
#define FT_OPEN_FILE_ERROR            "open file error!\n"

#define INIT_ERROR_OPEN_FB_DEV_ERROR  "��ʼ������:��ͼ������豸����"
#define INIT_ERROR_FONTFILE_ERROR     "��ʼ������:Ĭ�������ļ��򿪳���"
#define INIT_ERROR_MALLOC_GRAPH_ERROR "��ʼ������:�޷������㹻���ڴ�ռ�"

#define WIDGET_ERROR_GRAPH_ERROR		"��������:δΪ���������ڴ�ռ�!\n"
#define WIDGET_ERROR_TYPE_NOT_FOUND		"��������:δ�ҵ������͵Ĳ���!\n"

#define QUIT_PARENT_WINDOW_ERROR		"Quit_Parent_Window()����:����û�и�����"


#define LABLE_MALLOC_ERROR            "Lable_Text():�޷������㹻���ڴ�!\n"
#define WINDOWS_NUM_TOO_MUCH          "�޷���������!\n"
#define MALLOC_ERROR                  "�޷������㹻���ڴ�!\n" 
#define MSG_UNKNOWN_FORMAT            "δ֪�ļ���ʽ!\n"
#define MSG_SHORT_FILE                "�ļ����ȹ���!\n"

#define WIDGET_TYPE_ERROR             "����������֧�ֵĲ������Ͳ�һ��!\n"
#define WIDGET_REALLOC_ERROR          "�޷������㹻���ڴ����ڴ洢���ڲ��������ݣ�\n"
#define WIDGET_MALLOC_ERROR           "�޷�Ϊ�ò��������㹻���ڴ�ռ䣡\n"

#define ADD_NEW_WIDGET_TYPE_ERROR		"����µĲ������ͳ���\n"

#define ERROR_WINDOW_CLOSED            -10
