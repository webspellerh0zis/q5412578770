// ���Ը�ѡ�򲿼��ĳ�������

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_CHECKBOX_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_RES_H
#include <unistd.h>

LCUI_Widget *checkbox[5], *button, *window, *label, *result_text; 

void view_result(LCUI_Widget *widget, void *arg)
{
	char str[256]; 
	strcpy(str, "�����Ȥ�����У� ");
	/* ��������ѡ���Ƿ�ѡ�У���ȷ������ */
	if(CheckBox_Is_On(checkbox[0]))
		strcat(str, "��� ");
	if(CheckBox_Is_On(checkbox[1]))
		strcat(str, "��Ӱ ");
	if(CheckBox_Is_On(checkbox[2]))
		strcat(str, "���� ");
	if(CheckBox_Is_On(checkbox[3]))
		strcat(str, "���� ");
	if(CheckBox_Is_On(checkbox[4]))
		strcat(str, "˯�� ");
	
	strcat(str, "��");
	/* �趨�ı����� */
	Set_Label_Text(result_text, str);
	/* ���ð�ť���� */
	Disable_Widget(button);
}

int main(int argc, char*argv[]) 
{
    int i;
    LCUI_Init(argc, argv);
    /* �������� */
    window  = Create_Widget("window");
    label   = Create_Widget("label");
    button	= Create_Button_With_Text("�ύ");
    result_text = Create_Widget("label");
    checkbox[0] = Create_CheckBox_With_Text("���");
    checkbox[1] = Create_CheckBox_With_Text("��Ӱ");
    checkbox[2] = Create_CheckBox_With_Text("����");
    checkbox[3] = Create_CheckBox_With_Text("����");
    checkbox[4] = Create_CheckBox_With_Text("˯��");
    /* �趨���ڱ�����ı� */
    Set_Window_Title_Text(window, "���Ը�ѡ�򲿼�"); 
    /* �ı䴰�ڳߴ� */
    window->resize(window, Size(320, 240));
    /* ���ð�ť�������Զ��ߴ��������ΪҪ�Զ���ߴ� */
    Disable_Widget_Auto_Size(button);
    Resize_Widget(button, Size(60, 25));
    /* �����ڿͻ�����Ϊ��Щ���������� */
    Window_Client_Area_Add(window, label);
    Window_Client_Area_Add(window, button);
    Window_Client_Area_Add(window, result_text);
    for(i=0; i<5; i++) 
		Window_Client_Area_Add(window, checkbox[i]); 
    /* ���������Ĳ���*/
    Set_Widget_Align(label, ALIGN_MIDDLE_CENTER, Pos(0,-20));
    Set_Widget_Align(button, ALIGN_MIDDLE_CENTER, Pos(0,60));
    Set_Widget_Align(result_text, ALIGN_MIDDLE_CENTER, Pos(0, 30));
	Set_Widget_Align(checkbox[0], ALIGN_MIDDLE_CENTER, Pos(-120 ,5));
	Set_Widget_Align(checkbox[1], ALIGN_MIDDLE_CENTER, Pos(-60 ,5));
	Set_Widget_Align(checkbox[2], ALIGN_MIDDLE_CENTER, Pos(0 ,5));
	Set_Widget_Align(checkbox[3], ALIGN_MIDDLE_CENTER, Pos(60 ,5));
	Set_Widget_Align(checkbox[4], ALIGN_MIDDLE_CENTER, Pos(120 ,5));
	/* �趨label������ʾ���ı����Լ������С */
    Set_Label_Text(label, "�㶼����Щ��Ȥ���ã�\n"); 
    Set_Label_Font(label, 14, NULL);
    /* ��ʾ���� */
    Show_Widget(label);
    Show_Widget(result_text);
    Show_Widget(button);
    for(i=0; i<5; ++i) 
		Show_Widget(checkbox[i]);  
    Show_Widget(window); 
    /* Ϊ��ť������������¼����������ĺ�����view_result�������ڵ����ť�󱻵��� */
	Widget_Clicked_Event_Connect(button, view_result, NULL);
    return LCUI_Main(); /* ������ѭ�� */
}

