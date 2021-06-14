
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_RES_H
#include <unistd.h>
void *change_graph(void *arg)
/* ���ܣ��ı���ʾ��ͼ�� */
{
	LCUI_Widget *widget = (LCUI_Widget *)arg;
	int i;
	LCUI_Graph frame, temp;
	Graph_Init(&frame);/* ��ʼ�� */
	Load_Image("ring.png", &frame);/* ����֮ */  
	/* ������ʾͼ�� */
	Set_Widget_Align(widget, ALIGN_MIDDLE_CENTER, Pos(0, -20));
	Set_PictureBox_Size_Mode(widget, SIZE_MODE_CENTER);
	Show_Widget(widget);
	while(1)
	{
		Set_PictureBox_Image_From_Graph(widget, &frame);
		for(i=10; i<=360; i+=10)
		{
			Rotate_Graph(&frame, i, &temp);
			Set_PictureBox_Image_From_Graph(widget, &temp);
			usleep(20000);
		}
	}
	LCUI_Thread_Exit(NULL);
}

int main(int argc,char*argv[])
{ 
	/* ������������ */
	LCUI_Widget	*window, 
					*label,
					*logo,
					*img;
					
	int			 width, height; 
	/* �Զ���Ĭ�������ļ�λ�� */
	//Set_Default_Font("msyh.ttf");
	/* ��ʼ��LCUI */ 
	LCUI_Init(argc, argv);
	/* ����һ��LCUI���򴰿� */
	width  = 320;//Get_Screen_Width(); /* ���ڵĿ�� */
	height = 240;//Get_Screen_Height();; /* ���ڵĸ߶� */
	
	/* �������� */
	window		= Create_Widget("window");
	label		= Create_Widget("label");
	logo		= Create_Widget("picture_box");
	img			= Create_Widget("picture_box");
	
	//Set_Widget_Border_Style(window, BORDER_STYLE_NONE);
	Resize_Widget(img, Size(180, 180));
	Resize_Widget(logo, Size(128,128));
	Resize_Widget(window, Size(width, height));
	 
	Set_Window_Title_Text(window, "LCUI");
	
	Set_Widget_Backcolor(window, RGB(94,200,255));
	Set_PictureBox_Size_Mode(logo, SIZE_MODE_CENTER);
	Set_PictureBox_Image_From_File(logo, "logo_128x128.png"); 
	
	Set_Label_Text(label, "LCUI ��������...");
	Set_Label_Font(label, 15, NULL);
	Set_Widget_Align(logo, ALIGN_MIDDLE_CENTER, Pos(0, -20));
	Set_Widget_Align(label, ALIGN_MIDDLE_CENTER, Pos(0, +70));
    /* ���������������봰�ڿͻ����� */
	Window_Client_Area_Add(window, label);
	Window_Client_Area_Add(window, img); 
	Window_Client_Area_Add(window, logo);
	
	pthread_t t;
    LCUI_Thread_Create(&t, NULL, change_graph, (void*)img); 
	/* ��ʾ�����Լ����� */
	Show_Widget(label);
	Show_Widget(logo);
	Show_Widget(window); 
	LCUI_Main(); /* ������ѭ�� */
	return 0;
}

