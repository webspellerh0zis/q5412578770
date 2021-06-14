// ʱ�ӵ����ӳ���

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_GRAPHICS_H
#include LC_THREAD_H
#include LC_MISC_H
#include LC_PICBOX_H
#include <unistd.h>

void *update_clock(void *arg)
{
    time_t rawtime;
    struct tm * timeinfo;
    LCUI_Graph h_temp, m_temp, clock_bg, hour_pointer, minute_pointer;
    LCUI_Widget *widget = (LCUI_Widget *)arg; /* ת������ */
    LCUI_Widget *bg, *hour, *minute; 
    int h_angle, m_angle;
    /* ��ʼ��ͼ�����ݽṹ */
    Graph_Init(&clock_bg);
    Graph_Init(&minute_pointer);
    Graph_Init(&hour_pointer);
    Graph_Init(&h_temp);
    Graph_Init(&m_temp);
    /* ������������ */
    bg = Create_Widget("picture_box");
    hour = Create_Widget("picture_box");
    minute = Create_Widget("picture_box");
    /* PictureBox����������ʾͼƬ */
    Set_PictureBox_Size_Mode(bg, SIZE_MODE_CENTER);
    Set_PictureBox_Size_Mode(hour, SIZE_MODE_CENTER);
    Set_PictureBox_Size_Mode(minute, SIZE_MODE_CENTER);
    /* ����ͼƬ������ͼ������ */
    Load_Image("new_daytime_background.png", &clock_bg);
    Load_Image("new_daytime_hour.png", &hour_pointer);
    Load_Image("new_daytime_minute.png", &minute_pointer);
    /* �趨PictureBox������ʾ��ͼ�� */
    Set_PictureBox_Image_From_Graph(bg, &clock_bg);
    Set_PictureBox_Image_From_Graph(hour, &hour_pointer);
    Set_PictureBox_Image_From_Graph(minute, &minute_pointer);
    /* ����Щ�����������Ӧ������ */
    Widget_Container_Add(bg, hour);
    Widget_Container_Add(bg, minute);
    /* ��������������ڿͻ����� */
    Window_Client_Area_Add(widget, bg);
    /* �ı䲿���ߴ磬ʹ�ù̶��ĳߴ� */
    Resize_Widget(bg, Size(280, 280));
    Resize_Widget(hour, Size(120, 120));
    Resize_Widget(minute, Size(120, 120));
    /* �ı䲿���Ĳ��ַ�ʽ�����Ǿ�����ʾ */
    Set_Widget_Align(bg, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    Set_Widget_Align(hour, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    Set_Widget_Align(minute, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    /* ��ʾ */
    Show_Widget(hour);
    Show_Widget(minute);
    Show_Widget(bg);
    
    while(1)
    {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime ); /* ��ȡϵͳ��ǰʱ�� */
        /* ����ʱ�����ĽǶ� */
        h_angle = 360*timeinfo->tm_hour / 12.0;
        m_angle = 360*timeinfo->tm_min / 60.0;
        h_angle += m_angle / 60;
        /* ��������Ƕ�����תͼ�� */
        Rotate_Graph(&hour_pointer, h_angle, &h_temp);
        Rotate_Graph(&minute_pointer, m_angle, &m_temp);
        /* ����PictureBox������ʾ��ͼ�� */
        Set_PictureBox_Image_From_Graph(hour, &h_temp);
        Set_PictureBox_Image_From_Graph(minute, &m_temp);
        sleep(1);/* ��ͣ1�� */
    }
    
    LCUI_Thread_Exit(NULL);
}

int main(int argc, char*argv[])
/* ���������������� */
{
    LCUI_Widget *window;
    pthread_t t;
	Set_Default_Font("../../fonts/msyh.ttf");
    LCUI_Init(argc, argv);
    /* �������� */
    window  = Create_Widget("window");
    /* �ı䴰�ڵĳߴ� */
    Resize_Widget(window, Size(320, 320));
    /* �趨���ڱ������� */
    Set_Window_Title_Text(window, "ʱ��");
    /* �����߳� */
    LCUI_Thread_Create(&t, NULL, update_clock, (void*)window); 
    Show_Widget(window); 
    return LCUI_Main(); /* ������ѭ�� */  
}


