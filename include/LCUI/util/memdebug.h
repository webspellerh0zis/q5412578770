/* ***************************************************************************
 * memdebug.h -- memory debug module, used for memory usage statistics.
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
 * memdebug.h -- �ڴ����ģ�飬����ͳ�Ƹ����ڴ���ʹ�������������ԡ�
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

#ifndef LCUI_UTIL_MEMDEBUG_H
#define LCUI_UTIL_MEMDEBUG_H

LCUI_API void LCUI_InitMemDebug( void );

LCUI_API void LCUI_ExitMemDebug( void );

LCUI_API void LCUI_PrintMemStats( void );

#ifdef ENABLE_MEMDEBUG

LCUI_API void *LCUI_Malloc( size_t size, const char *name );

LCUI_API void *LCUI_Calloc( size_t count, size_t size, const char *name );

LCUI_API void *LCUI_Realloc( void *ptr, size_t size );

LCUI_API void LCUI_Free( void *ptr );

LCUI_API void LCUI_SetMemBlockName( void *ptr, const char *name );

#undef NEW
#define NEW(TYPE, COUNT) (TYPE*)LCUI_Calloc(COUNT, sizeof(TYPE), __FUNCTION__)

#else

#define LCUI_Malloc(SIZE, NAME) malloc(SIZE)
#define LCUI_Calloc(COUNT, SIZE, NAME) calloc(COUNT, SIZE)
#define LCUI_Realloc(PTR, SIZE) realloc(PTR, SIZE)
#define LCUI_Free free
#define LCUI_SetMemBlockName(PTR, NAME) 

#endif

#endif
