// ���԰�ť�����ĳ�������
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H 
#include LC_MISC_H
#include LC_BUTTON_H //�õ��˰�ť��������Ҫ�������ͷ�ļ�
#include <unistd.h>

int main(int argc, char*argv[]) 
{
    LCUI_Init(argc, argv);
    LCUI_Widget *window, *button;
    /* �������� */
    window  = Create_Widget("window");
    button  = Create_Widget("button");
    
    /* �趨���ڱ�����ı� */
    Set_Window_Title_Text(window, "���Դ���"); 
    /* �ı䴰�ڵĳߴ磬Ҳ������Resize_Widget() */
    window->resize(window, Size(320, 240));
    /* �����ڿͻ�����Ϊlael������������ӽ�ȥ */
    Window_Client_Area_Add(window, button);
    /* ������ʾ��ť,Ҳ������Set_Widget_Align() */
    button->set_align(button, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    /* ���ò������Զ��ߴ���� */
    Disable_Widget_Auto_Size(button);
    /* �Զ��尴ť�ĳߴ� */
    button->resize(button, Size(80, 30));
    /* �趨��ť����ʾ���ı����ݣ���label�������÷�һ�� */
    Set_Button_Text(button, 
                "<color=255,0,0>��</color>"
                "<color=0,255��0>ɫ</color>"
                "<color=0,0,255>��</color>"
                "<color=255,255,0>��</color>"
                "<color=255,255,255>ť</color>");
    /* ��ʾ������Ҳ������Show_Widget()���� */
    button->show(button);
    window->show(window); 
    return LCUI_Main(); /* ������ѭ�� */
}

