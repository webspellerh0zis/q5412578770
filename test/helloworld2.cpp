// C++���hello,world����
#include <LCUI_Build.h> 
#include LC_LCUI_H	/* ����LCUI�����ͷ�ļ� */
#include LC_WIDGET_H
#include <unistd.h>
#include <iostream>

using namespace std;
/* �����߳� */
void *test_thread(void *object)
{
	LCUIThread *t = (LCUIThread *)object;
	/* ��ӡ�߳�ID */
	cout<<"thread id: "<<t->getid()<<endl;
	/* �߳��˳� */
	t->exit( NULL );
}

int main(int argc, char*argv[]) 
{
	LCUI		app(argc, argv); 
	LCUIWidget	window( "window" ), label( "label" );
	LCUIGraph	icon;
	LCUIThread	t;
	
	Using_GB2312();
	/* �����߳� */
	t.create( NULL, test_thread, &t );
	/* �ȴ��߳��˳� */
	t.join( NULL );
	/* ����LCUI��18x18�ߴ��ͼ�� */
	icon.load.icon.LCUI_18x18();
	/* Ϊ�����趨ͼ�� */
	window.Window.setIcon( icon );
	/* �ı䴰�ڵĳߴ� */
	window.resize( Size(320, 240) ); 
	/* �趨���ڵı����ı� */
	window.Window.setTitle( "���Դ���" ); 
	/* ��label������������ڿͻ��� */
	window.Window.addToClientArea( label ); 
	/* �趨label�����Ķ��뷽ʽ */
	label.setAlign( ALIGN_MIDDLE_CENTER, Pos(0,0) ); 
	/* �趨label������ʾ���ı� */
	label.Label.setText(
		"Hello,World!\n"
		"<color=255,0,0>��</color><color=100,155,0>��</color>"
		"<color=0,0,0>,</color><color=0,255,0>��</color>"
		"<color=0,100,155>��</color><color=0,0,255>��</color>\n" 
	); 
	/* �趨�����С */
	label.Label.setFont( 24, NULL );
	/* ��ʾ���������� */
	label.show();
	window.show();
	/* ������ѭ�� */
	return app.main(); 
}

