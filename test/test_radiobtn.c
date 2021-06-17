// ���Ե�ѡ�򲿼���ʾ������
#include <LCUI_Build.h> /* ����LCUI��ͷ�ļ� */
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_RADIOBTN_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_RES_H
#include <unistd.h>

LCUI_Widget *age_rb[6], *button, *window, *age_label; 

void view_result(LCUI_Widget *widget, void *arg)
{
	char str[256]; 
	strcpy(str, "���������ǣ� ");
	if(RadioButton_Is_On(age_rb[0]))
		strcat(str, "15������");
	if(RadioButton_Is_On(age_rb[1]))
		strcat(str, "15-20��");
	if(RadioButton_Is_On(age_rb[2]))
		strcat(str, "21-25��");
	if(RadioButton_Is_On(age_rb[3]))
		strcat(str, "26-30��");
	if(RadioButton_Is_On(age_rb[4]))
		strcat(str, "31-40��");
	if(RadioButton_Is_On(age_rb[5]))
		strcat(str, "40������");
	
	strcat(str, "��");
    Set_Label_Text(age_label, str);  
	Disable_Widget(button);
}

int main(int argc, char*argv[])
/* ���������������� */
{
    int i;
    LCUI_Init(argc, argv);
    /* �������� */
    window  = Create_Widget("window");
    age_label = Create_Widget("label"); 
    button	= Create_Button_With_Text("�ύ"); 
    age_rb[0] = Create_RadioButton_With_Text("A. 15������");
    age_rb[1] = Create_RadioButton_With_Text("B. 15-20��");
    age_rb[2] = Create_RadioButton_With_Text("C. 21-25��");
    age_rb[3] = Create_RadioButton_With_Text("D. 26-30��");
    age_rb[4] = Create_RadioButton_With_Text("E. 31-40��");
    age_rb[5] = Create_RadioButton_With_Text("F. 40������");
    /* �趨���ڱ�����ı� */
    Set_Window_Title_Text(window, "���Ը�ѡ�򲿼�"); 
    /* �ı�ߴ� */
    Resize_Widget(window, Size(320, 240));
    Disable_Widget_Auto_Size(button);
    Resize_Widget(button, Size(60, 25));
    /* �����ڿͻ�����Ϊ��Щ���������� */ 
    Window_Client_Area_Add(window, age_label);
    Window_Client_Area_Add(window, button); 
    for(i=0; i<6; i++) 
		Window_Client_Area_Add(window, age_rb[i]); 
    /* ���������Ĳ���*/
    Set_Widget_Align(age_label, ALIGN_MIDDLE_CENTER, Pos(0,-40));
    Set_Widget_Align(button, ALIGN_MIDDLE_CENTER, Pos(0,60));
	Set_Widget_Align(age_rb[0], ALIGN_MIDDLE_LEFT, Pos(5,-20));
	Set_Widget_Align(age_rb[1], ALIGN_MIDDLE_LEFT, Pos(100,-20));
	Set_Widget_Align(age_rb[2], ALIGN_MIDDLE_LEFT, Pos(200,-20)); 
	Set_Widget_Align(age_rb[3], ALIGN_MIDDLE_LEFT, Pos(5,0));
	Set_Widget_Align(age_rb[4], ALIGN_MIDDLE_LEFT, Pos(100,0));
	Set_Widget_Align(age_rb[5], ALIGN_MIDDLE_LEFT, Pos(200,0)); 
    Set_Label_Text(age_label, "���������ǣ�");  
    
    RadioButton_Create_Mutex(age_rb[0], age_rb[1]);
    RadioButton_Create_Mutex(age_rb[0], age_rb[2]);
    RadioButton_Create_Mutex(age_rb[0], age_rb[3]);
    RadioButton_Create_Mutex(age_rb[0], age_rb[4]);
    RadioButton_Create_Mutex(age_rb[0], age_rb[5]);
    
    /* ��ʾ���� */
    Show_Widget(age_label);
    Show_Widget(button);
	Show_Widget(age_rb[0]); 
    for(i=0; i<6; ++i) 
		Show_Widget(age_rb[i]);  
    Show_Widget(window); 
    
	Widget_Clicked_Event_Connect(button, view_result, NULL);
    LCUI_Main(); /* ������ѭ�� */ 
    return 0;
}

