/* ***************************************************************************
 * win32.c -- win32 platform support for graphical output
 * 
 * Copyright (C) 2012-2013 by
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
 * win32.c -- win32平台上的图形输出支持
 *
 * 版权所有 (C) 2013 归属于
 * 刘超
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/

#include <LCUI_Build.h>
#include LC_LCUI_H

#ifdef LCUI_VIDEO_DRIVER_WIN32
#include LC_GRAPH_H
#include LC_INPUT_H
#include LC_CURSOR_H
#include LC_DISPLAY_H
#include <Windows.h>

static HWND current_hwnd = NULL;
static unsigned char *pixel_mem = NULL;
static HDC hdc_client, hdc_framebuffer;
static HBITMAP client_bitmap;
static HINSTANCE win32_hInstance = NULL;

LCUI_API void
Win32_LCUI_Init( HINSTANCE hInstance )
{
	win32_hInstance = hInstance;
}
#include <time.h>
static LRESULT CALLBACK 
Win32_LCUI_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	LCUI_Rect area;

	switch (message) {
	case WM_KEYDOWN:
		printf("WM_KEYDOWN: %ld\n",wParam);
		LCUIKeyboard_HitKey( wParam );
		return 0;
	case WM_KEYUP:
		printf("WM_KEYUP: %ld\n",wParam);
		LCUIKeyboard_FreeKey( wParam );
		return 0;
	case WM_RBUTTONDOWN:
		Win32_LCUIMouse_ButtonDown( LCUIKEY_RIGHTBUTTON );
		return 0;
	case WM_RBUTTONUP:
		Win32_LCUIMouse_ButtonUp( LCUIKEY_RIGHTBUTTON );
		return 0;
	case WM_LBUTTONDOWN:
		//_DEBUG_MSG("left button down\n");
		Win32_LCUIMouse_ButtonDown( LCUIKEY_LEFTBUTTON );
		return 0;
	case WM_LBUTTONUP:
		//_DEBUG_MSG("left button up\n");
		Win32_LCUIMouse_ButtonUp( LCUIKEY_LEFTBUTTON );
		return 0;
	case WM_PAINT:
		DEBUG_MSG("WM_PAINT\n");
		BeginPaint( hwnd, &ps );
		/* 获取区域坐标及尺寸 */
		area.x = ps.rcPaint.left;
		area.y = ps.rcPaint.top;
		area.width = ps.rcPaint.right - area.x;
		area.height = ps.rcPaint.bottom - area.y;
		LCUIScreen_InvalidArea( area );
		EndPaint( hwnd, &ps );
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		LCUI_Quit();
		return 0;
	default:break;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

LCUI_API HWND
Win32_GetSelfHWND( void )
{
	return current_hwnd;
}

LCUI_API void
Win32_SetSelfHWND( HWND hwnd )
{
	current_hwnd = hwnd;
}

LCUI_API void
LCUIScreen_FillPixel( LCUI_Pos pos, LCUI_RGB color )
{
	return;
}

LCUI_API int
LCUIScreen_GetGraph( LCUI_Graph *out )
{
	return -1;
}

LCUI_API int
LCUIScreen_Init( void )
{
	RECT client_rect;
	LCUI_Graph *graph;
	WNDCLASS wndclass;
	TCHAR szAppName[] = TEXT ("Typer");
	
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = Win32_LCUI_WndProc;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = win32_hInstance;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass)) {
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
		szAppName, MB_ICONERROR) ;
		return 0;
	}
	
	current_hwnd = CreateWindow (
			szAppName, TEXT ("LCUI"),
			WS_OVERLAPPEDWINDOW &~WS_THICKFRAME,
			CW_USEDEFAULT, CW_USEDEFAULT,
			WIN32_WINDOW_WIDTH, WIN32_WINDOW_HEIGHT,
			NULL, NULL, win32_hInstance, NULL);
	
	GetClientRect( current_hwnd, &client_rect );

	LCUI_Sys.screen.fb_dev_fd = -1;
	LCUI_Sys.screen.fb_dev_name = "win32";
	LCUI_Sys.screen.bits = 32;
	LCUI_Sys.screen.size.w = client_rect.right;
	LCUI_Sys.screen.size.h = client_rect.bottom; 
	LCUI_Sys.screen.smem_len = LCUI_Sys.screen.size.w * LCUI_Sys.screen.size.h * 4;
	/* 分配内存，储存像素数据 */ 
	pixel_mem = malloc ( LCUI_Sys.screen.smem_len );
	LCUI_Sys.screen.fb_mem = pixel_mem;
	LCUI_Sys.root_glayer = GraphLayer_New();
	GraphLayer_Resize( LCUI_Sys.root_glayer, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h );
	graph = GraphLayer_GetSelfGraph( LCUI_Sys.root_glayer );
	Graph_FillColor( graph, RGB(255,255,255) );

	/* 获取客户区的DC */
	hdc_client = GetDC( current_hwnd );
	/* 为帧缓冲创建一个DC */
	hdc_framebuffer = CreateCompatibleDC( hdc_client );
	/* 为客户区创建一个Bitmap */ 
	client_bitmap = CreateCompatibleBitmap( hdc_client, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h );
	/* 为帧缓冲的DC选择client_bitmap作为对象 */
	SelectObject( hdc_framebuffer, client_bitmap );
	
	GraphLayer_Show( LCUI_Sys.root_glayer );
	ShowWindow( current_hwnd, SW_SHOWNORMAL );
	UpdateWindow( current_hwnd );
	return 0;
}

LCUI_API int
LCUIScreen_Destroy( void )
{
	LCUI_Graph *graph;
	
	LCUI_Sys.state = KILLED;
	graph = GraphLayer_GetSelfGraph( LCUI_Sys.root_glayer );
	GraphLayer_Free( LCUI_Sys.root_glayer );
	DeleteDC( hdc_framebuffer );
	ReleaseDC( Win32_GetSelfHWND(), hdc_client );
	free( pixel_mem );
	return 0;
}

LCUI_API void
LCUIScreen_SyncFrameBuffer( void )
{
	SetBitmapBits( client_bitmap, LCUI_Sys.screen.smem_len, LCUI_Sys.screen.fb_mem );
	/* 将帧缓冲内的位图数据更新至客户区内指定区域（area） */
	BitBlt( hdc_client, 0, 0, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h, 
		hdc_framebuffer, 0, 0, SRCCOPY );
	ValidateRect( current_hwnd, NULL );
}

LCUI_API int
LCUIScreen_PutGraph (LCUI_Graph *src, LCUI_Pos pos )
{
	uchar_t *dest;
	int x, y, n, k, count;
	LCUI_Rect cut_rect;
	LCUI_Graph temp, *pic;

	if (!Graph_IsValid (src)) {
		return -1;
	}
	/* 指向帧缓冲 */
	dest = LCUI_Sys.screen.fb_mem;		
	pic = src; 
	Graph_Init (&temp);
	
	if ( LCUIRect_GetCutArea ( LCUIScreen_GetSize(), 
			Rect ( pos.x, pos.y, src->width, src->height ), 
			&cut_rect
		) ) {/* 如果需要裁剪图形 */
		if(!LCUIRect_IsValid(cut_rect)) {
			return -2;
		}
		pos.x += cut_rect.x;
		pos.y += cut_rect.y;
		Graph_Cut (pic, cut_rect, &temp);
		pic = &temp;
	}
	
	Graph_Lock( pic );
	k = pos.y * LCUI_Sys.screen.size.w + pos.x;
	for (n=0,y = 0; y < pic->height; ++y) {
		for (x = 0; x < pic->width; ++x, ++n) {
			count = k + x;//count = 4 * (k + x);/* 计算需填充的像素点的坐标 */
			count = count << 2; 
			dest[count++] = pic->rgba[2][n]; 
			dest[count++] = pic->rgba[1][n]; 
			dest[count++] = pic->rgba[0][n];
			dest[count] = 255;
		}
		k += LCUI_Sys.screen.size.w;
	}
	Graph_Unlock( pic );
	Graph_Free( &temp );
	return 0;
}

LCUI_API void
LCUIScreen_CatchGraph( LCUI_Rect area, LCUI_Graph *out )
{
	return;
}
#endif
