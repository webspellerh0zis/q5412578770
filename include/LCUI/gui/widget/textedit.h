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
#ifndef LCUI_TEXTBOX_H
#define LCUI_TEXTBOX_H

LCUI_BEGIN_HEADER

#define TextEdit_New() Widget_New("textedit")

/** ָ���ı����Ƿ�����Ʒ� */
LCUI_API void TextEdit_SetUsingStyleTags( LCUI_Widget widget, LCUI_BOOL is_true );

/** ָ���ı����Ƿ����ö����ı���ʾ */
LCUI_API void TextEdit_SetMultiline( LCUI_Widget widget, LCUI_BOOL is_true );

/** ����ı����� */
LCUI_API void TextEdit_ClearText( LCUI_Widget widget );

/** Ϊ�ı��������ı������ַ��棩 */
LCUI_API int TextEdit_SetTextW( LCUI_Widget widget, const wchar_t *wstr );

LCUI_API int TextEdit_SetText( LCUI_Widget widget, const char *utf8_str );

/** Ϊ�ı���׷���ı������ַ��棩 */
LCUI_API int TextEdit_AppendTextW( LCUI_Widget widget, const wchar_t *wstr );

/** Ϊ�ı�������ı������ַ��棩 */
LCUI_API int TextEdit_InsertTextW( LCUI_Widget widget, const wchar_t *wstr );

LCUI_END_HEADER

#endif