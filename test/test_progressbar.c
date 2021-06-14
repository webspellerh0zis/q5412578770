
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
//���Խ�����������ʾ������
#include LC_WORK_H
#include LC_MISC_H
#include LC_THREAD_H
#include LC_PROGBAR_H
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
void *change_progress(void *arg)
/* ���ܣ���̬�ı�label�������ı����� */
{
    int i, max;
    max = 100;
    LCUI_Widget *widget = (LCUI_Widget *)arg; /* ת������ */
    Set_ProgressBar_Max_Value(widget, max); /* ���ֵ */
    srand(time(NULL));
    for(i=0; i<max; i+=rand()%5)
    {
        Set_ProgressBar_Value(widget, i);/* ��ǰֵ */
        usleep(100000);/* ��ͣ0.1�� */
    }
    Set_ProgressBar_Value(widget, max);
    LCUI_Thread_Exit(NULL);
}


int main(int argc, char*argv[])
/* ���������������� */
{
    pthread_t t[2];
    LCUI_Init(argc, argv);
    
    LCUI_Widget *window, *pb_a, *pb_b;
    /* �������� */
    window = Create_Widget("window");
    pb_a = Create_Widget("progress_bar");
    pb_b = Create_Widget("progress_bar");
    /* �趨���ڱ�����ı� */
    Set_Window_Title_Text(window, "���Խ���������"); 
    /* �ı䴰�ڵĳߴ� */
    Resize_Widget(window, Size(320, 240));
    /* �����ڿͻ�����Ϊ������������ӽ�ȥ */
    Window_Client_Area_Add(window, pb_a);
    Window_Client_Area_Add(window, pb_b);
    /* ������ʾ */
    Set_Widget_Align(pb_a, ALIGN_MIDDLE_CENTER, Pos(0, -25)); 
    Set_Widget_Align(pb_b, ALIGN_MIDDLE_CENTER, Pos(0, 25));
    /* �趨�����ķ�� */
    Set_Widget_Style(pb_a, "dynamic");
    Set_Widget_Style(pb_b, "classic");
    /* �ı䲿���ߴ� */
    Resize_Widget(pb_a, Size(300, 25));
    Resize_Widget(pb_b, Size(300, 25));
    /* �����̣߳��˺�����pthread_create�����÷�һ�� */
    LCUI_Thread_Create(&t[0], NULL, change_progress, (void*)pb_a);
    LCUI_Thread_Create(&t[1], NULL, change_progress, (void*)pb_b); 
    /* ��ʾ���� */
    Show_Widget(pb_a);
    Show_Widget(pb_b);
    Show_Widget(window); 
    return LCUI_Main(); /* ������ѭ�� */  
}

