// �˳������ڲ��Բ�������ʽ�Զ��幦��

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_GRAPH_H
#include LC_WINDOW_H
#include LC_LABEL_H

#ifdef LCUI_BUILD_IN_WIN32
#include <io.h>
#include <fcntl.h>

/* �����г���ʱ��򿪿���̨���Բ鿴��ӡ�ĵ�����Ϣ */
static void InitConsoleWindow(void)
{
	int hCrt;
	FILE *hf;
	AllocConsole();
	hCrt=_open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE),_O_TEXT );
	hf=_fdopen( hCrt, "w" );
	*stdout=*hf;
	setvbuf (stdout, NULL, _IONBF, 0);
	// test code
	printf ("InitConsoleWindow OK!\n");
}
#endif

char style_buff[256]={
	".void-widget {"
	"background-color:#B5E61D;"
	"background-image:logo_128x128.png;"
	"background-transparent:0;"
	"background-layout:center;"
	"}"
};

int LCUIMainFunc( LCUI_ARGLIST )
{
	LCUI_Widget *widget;
	
#ifdef LCUI_BUILD_IN_WIN32
	InitConsoleWindow();
#endif
	LCUI_Init( LCUI_DEFAULT_CONFIG );
	WidgetStyle_LoadFromString( style_buff );
	
	/* �������� */
	widget  = Widget_New(NULL);
	Widget_SetBorder(widget, Border(1,BORDER_STYLE_SOLID, RGB(0,0,0)));
	Widget_Resize( widget, Size(320, 240) );
	Widget_Draw( widget );
	Widget_Show( widget ); 
	return LCUI_Main(); /* ������ѭ�� */
}
