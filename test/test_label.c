// ����label������ʾ������

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_THREAD_H
#include LC_LABEL_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_RES_H
#include <unistd.h>
#include <pthread.h>
#include <time.h>
void *change_label_text(void *arg)
/* ���ܣ���̬�ı�label�������ı����� */
{
    time_t rawtime;
    struct tm * timeinfo;
    char day[][20] = {"������","����һ","���ڶ�","������","������","������","������"};
    LCUI_Widget *widget = (LCUI_Widget *)arg; /* ת������ */
    while(1)
    {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime ); /* ��ȡϵͳ��ǰʱ�� */
        /* �����ı����� */
        Set_Label_Text(widget, 
            "��ǰʱ��: %4d��%02d��%02d�� %s %02d:%02d:%02d",
            timeinfo->tm_year+1900, timeinfo->tm_mon+1, 
            timeinfo->tm_mday, day[(int) (timeinfo->tm_wday)], 
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec
        );
        sleep(1);/* ��ͣ1�� */
    }
    LCUI_Thread_Exit(NULL);
}

int main(int argc, char*argv[])
/* ���������������� */
{
    pthread_t t;
    LCUI_Widget *window, *label, *time_text;
    
    LCUI_Init(argc, argv);/* ��ʼ��LCUIͼ�ν���� */
    /* �������� */
    window      = Create_Widget("window");
    label       = Create_Widget("label");
    time_text   = Create_Widget("label");
    
    /* �趨���ڱ�����ı� */
    Set_Window_Title_Text(window, "����label�������ı���ʾ"); 
    /* �ı䴰�ڵĳߴ� */
    Resize_Widget(window, Size(320, 240));
    /* �����ڿͻ�����Ϊlael������������ӽ�ȥ */
    Window_Client_Area_Add(window, label);
    Window_Client_Area_Add(window, time_text);
    /* ���Ͻ���ʾ������ƫ��4������ƫ��4 */
    Set_Widget_Align(label, ALIGN_TOP_LEFT, Pos(4, 4));
    /* ���½���ʾ������ƫ��10������ƫ��5 */
    Set_Widget_Align(time_text, ALIGN_BOTTOM_RIGHT, Pos(-10, -5));
    /* 
     * �趨label������ʾ���ı�
     * <color=R,G,B>����</color> ��ʾ���ǣ������֡���ʹ���Զ�����ɫ 
     * Set_Label_Text����ʹ�÷�����sprintf��������һ����֧�ֿ��Ʒ�
     **/
    Set_Label_Text(label, 
        "<color=50,200,50>/* ����C���Դ��� */</color>\n"
        "<color=0,155,100>#include</color> <color=255,135,0><stdio.h></color>\n"
        "<color=50,50,200>int</color> main(<color=50,50,200>int</color> argc, <color=50,50,200>char</color> *argv[])\n"
        "{\n"
        "  printf(<color=100,100,100>\"hello world!\\n\"</color>);\n"
        "  <color=0,0,200>return</color> 0;\n"
        "}\n"
    ); 
    /* �����̣߳��˺�����pthread_create�����÷�һ�� */
    LCUI_Thread_Create(&t, NULL, change_label_text, (void*)time_text); 
    Show_Widget(label);
    Show_Widget(time_text);
    Show_Widget(window);
    return LCUI_Main(); 
}

