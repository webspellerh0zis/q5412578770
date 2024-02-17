/* ***************************************************************************
 * win32.c -- win32 platform support for graphical output
 * 
 * Copyright (C) 2013 by
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
 * win32.c -- win32ƽ̨�ϵ�ͼ�����֧��
 *
 * ��Ȩ���� (C) 2013 ������
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

LCUI_EXPORT(void)
Win32_LCUI_Init( HINSTANCE hInstance )
{
	win32_hInstance = hInstance;
}

static LRESULT CALLBACK 
Win32_LCUI_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	LCUI_Rect area;
	PAINTSTRUCT ps;
	LCUI_Graph graph;
	
	Win32_LCUIMouse_UpdatePos();

	switch (message) {
	case WM_KEYDOWN:
		printf("WM_KEYDOWN: %ld\n",lParam);
		break;
	case WM_KEYUP:
		break;
	case WM_LBUTTONDOWN:
		Win32_LCUIMouse_ButtonDown( LCUIKEY_LEFTBUTTON );
		break;
	case WM_LBUTTONUP:
		Win32_LCUIMouse_ButtonUp( LCUIKEY_LEFTBUTTON );
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_PAINT: 
		Graph_Init( &graph );
		BeginPaint( hwnd, &ps );
		/* ��ȡ�������꼰�ߴ� */
		area.x = ps.rcPaint.left;
		area.y = ps.rcPaint.top;
		area.width = ps.rcPaint.right - area.x;
		area.height = ps.rcPaint.bottom - area.y;
		LCUIScreen_GetRealGraph( area, &graph );
		LCUIScreen_PutGraph( &graph, Pos(area.x, area.y) );
		/* ��֡�����ڵ�λͼ���ݸ������ͻ�����ָ������area�� */
		BitBlt( hdc_client, area.x, area.y, area.width, area.height, 
			hdc_framebuffer, area.x, area.y, SRCCOPY );
		
		EndPaint( hwnd, &ps );
		Graph_Free( &graph );
		return 0;
	case WM_DESTROY:
		PostQuitMessage (0) ;
		return 0;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

LCUI_EXPORT(HWND)
Win32_GetSelfHWND( void )
{
	return current_hwnd;
}

LCUI_EXPORT(void)
Win32_SetSelfHWND( HWND hwnd )
{
	current_hwnd = hwnd;
}

LCUI_EXPORT(void)
LCUIScreen_FillPixel( LCUI_Pos pos, LCUI_RGB color )
{
	return;
}

LCUI_EXPORT(int)
LCUIScreen_GetGraph( LCUI_Graph *out )
{
	return -1;
}

LCUI_EXPORT(int)
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
	/* �����ڴ棬������������ */ 
	pixel_mem = malloc ( LCUI_Sys.screen.smem_len );
	LCUI_Sys.screen.fb_mem = pixel_mem;
	LCUI_Sys.root_glayer = GraphLayer_New();
	GraphLayer_Resize( LCUI_Sys.root_glayer, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h );
	graph = GraphLayer_GetSelfGraph( LCUI_Sys.root_glayer );
	Graph_Fill_Color( graph, RGB(255,255,255) );

	/* ��ȡ�ͻ�����DC */
	hdc_client = GetDC( current_hwnd );
	/* Ϊ֡���崴��һ��DC */
	hdc_framebuffer = CreateCompatibleDC( hdc_client );
	/* Ϊ�ͻ�������һ��Bitmap */ 
	client_bitmap = CreateCompatibleBitmap( hdc_client, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h );
	/* Ϊ֡�����DCѡ��client_bitmap��Ϊ���� */
	SelectObject( hdc_framebuffer, client_bitmap );
	
	GraphLayer_Show( LCUI_Sys.root_glayer );
	ShowWindow( current_hwnd, SW_SHOWNORMAL );
	UpdateWindow( current_hwnd );
	return 0;
}

LCUI_EXPORT(int)
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

LCUI_EXPORT(int)
LCUIScreen_PutGraph (LCUI_Graph *src, LCUI_Pos pos )
{
	uchar_t *dest;
	int x, y, n, k, count;
	LCUI_Rect cut_rect;
	LCUI_Graph temp, *pic;

	if (!Graph_Valid (src)) {
		return -1;
	}
	/* ָ��֡���� */
	dest = LCUI_Sys.screen.fb_mem;		
	pic = src; 
	Graph_Init (&temp);
	
	if ( Get_Cut_Area ( LCUIScreen_GetSize(), 
			Rect ( pos.x, pos.y, src->width, src->height ), 
			&cut_rect
		) ) {/* �����Ҫ�ü�ͼ�� */
		if(!Rect_Valid(cut_rect)) {
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
			count = k + x;//count = 4 * (k + x);/* �������������ص������ */
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
	SetBitmapBits( client_bitmap, LCUI_Sys.screen.smem_len, LCUI_Sys.screen.fb_mem );
	return 0;
}

LCUI_EXPORT(void)
LCUIScreen_CatchGraph( LCUI_Rect area, LCUI_Graph *out )
{
	return;
}
#endif
