#include <LCUI_Build.h> /* ����LCUI��ͷ�ļ� */
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_PICBOX_H
#include LC_BUTTON_H
#include LC_MEM_H
#include LC_LABEL_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_RES_H
#include LC_INPUT_H
#include LC_THREAD_H

#include <unistd.h>
#include <time.h>   
#include <stdlib.h>

static LCUI_Widget *window, *l_vs, *btn_next, *btn_s, *btn_j, *btn_b; 
static LCUI_Graph  stone, knife, cloth, think; 
static LCUI_Graph  left_stone, left_knife, left_cloth; /* �ѱ�ˮƽ��ת��ͼƬ���� */
static LCUI_Widget *label, *me_pic_box, *cpu_pic_box;
int win = 0, lose = 0, standoff = 0; /* �ɼ� */

void update_text()
/* ���ܣ������ı� */
{
	Set_Label_Text(label, "��ҳɼ�: Ӯ%d��, ��%d��, ƽ��%d��", 
						win, lose, standoff); /* �趨label������ʾ���ı� */
}


void run_game(int select)
/* ���ܣ�������Ϸ */
{
	int cpu_select;
	cpu_select = rand() % 3;  /* ����3���ڵ������ */ 
	/* �����������ȡֵ����PictureBox������ʾ��ͬ��ͼ�� */
	if(cpu_select == 0) /* ʯͷ */
		Set_PictureBox_Image_From_Graph(cpu_pic_box, &stone); 
	else if(cpu_select == 1) /* ���� */
		Set_PictureBox_Image_From_Graph(cpu_pic_box, &knife); 
	else /* �� */
		Set_PictureBox_Image_From_Graph(cpu_pic_box, &cloth); 
	
	if(cpu_select == select) /* �����һ����,ƽ�ִ���+1 */
		standoff += 1; 
	else if((cpu_select == 0 && select == 1) 
			|| (cpu_select == 1 && select == 2) 
			|| (cpu_select == 2 && select == 0)
			)/* ����ǳ���Ӯ��, ��Ĵ���+1 */
		lose += 1; 
	else  /* ����ֻ�����Ӯ��,Ӯ�Ĵ���+1 */
		win += 1; 
	update_text();/* �����ı����� */
}

void clear_game()
/* ���ܣ�������Ϸ���ָ�PictureBox������ʾ��ͼ�� */
{
	Set_PictureBox_Image_From_Graph(me_pic_box, &think); 
	Set_PictureBox_Image_From_Graph(cpu_pic_box, &think);
	Disable_Widget(btn_next);
}

void select_stone(LCUI_Widget *widget, void *junk)
/* ���ܣ����ѡ��ʯͷ */
{
	/* ���û�д洢��ˮƽ��ת��ͼ�񣬾ͽ���ˮƽ��ת */
	if(!Valid_Graph(&left_stone)) 
		Graph_Flip_Horizontal(&stone, &left_stone);
	Set_PictureBox_Image_From_Graph(me_pic_box, &left_stone); /* �趨��ʾ��ͼ��Ϊʯͷ */
	Disable_Widget(btn_j);
	Disable_Widget(btn_s);
	Disable_Widget(btn_b);
	Enable_Widget(btn_next);
	run_game(0);/* ������Ϸ */
}

void select_knife(LCUI_Widget *widget, void *junk)
/* ���ܣ����ѡ����� */
{
	if(!Valid_Graph(&left_knife)) 
		Graph_Flip_Horizontal(&knife, &left_knife);
	Set_PictureBox_Image_From_Graph(me_pic_box, &left_knife); 
	Disable_Widget(btn_j);
	Disable_Widget(btn_s);
	Disable_Widget(btn_b);
	Enable_Widget(btn_next);
	run_game(1);
}

void select_cloth(LCUI_Widget *widget, void *junk)
/* ���ܣ����ѡ�� */
{
	if(!Valid_Graph(&left_cloth)) 
		Graph_Flip_Horizontal(&cloth, &left_cloth);
		
	Set_PictureBox_Image_From_Graph(me_pic_box, &left_cloth); 
	Disable_Widget(btn_j);
	Disable_Widget(btn_s);
	Disable_Widget(btn_b);
	Enable_Widget(btn_next);
	run_game(2);
}

void next()
/* ���ܣ�������һ�� */
{
	Enable_Widget(btn_j);
	Enable_Widget(btn_s);
	Enable_Widget(btn_b);
	Disable_Widget(btn_next); 
	clear_game();
}

void Get_Path(char *filepath, char *out_path)
/* ���ܣ����ڻ�ȡ�������ڵ��ļ�Ŀ¼ */
{
	int num; 
	strcpy(out_path, filepath);
	for(num = strlen(filepath) - 1; num >= 0; --num) 
	{
		if(filepath[num] == '/') 
		{
			out_path[num+1] = 0;
			return;
		}
	}
	out_path[0] = 0;
}


int main(int argc,char*argv[])
/* ���������������� */
{
	LCUI_Graph   icon, btn_normal, btn_focus, btn_down, btn_over;/* �洢��ť����״̬Ҫ��ʾ��ͼ�� */
	int			 width, height;
	/* �Զ���Ĭ�������ļ�λ�� */
	//Set_Default_Font("/mnt/Data/LC-SOFT/fonts/΢���ź�.ttf");
	Set_Default_Font("../../fonts/msyh.ttf");
	/* ��ʼ��LCUI */
	LCUI_Init(argc, argv); 
	/* ��ʼ��ͼƬ���ݽṹ�� */
	Graph_Init(&btn_normal);
	Graph_Init(&btn_focus);
	Graph_Init(&btn_down);
	Graph_Init(&btn_over);
	Graph_Init(&stone);
	Graph_Init(&left_stone);
	Graph_Init(&cloth);
	Graph_Init(&left_cloth);
	Graph_Init(&knife);
	Graph_Init(&left_knife);
	Graph_Init(&think);
	Graph_Init(&icon);
	
	/* ��ȡ�ļ���·����֮��򿪲�����ͼƬ */ 
	Load_Image("drawable/jsb_b.png", &cloth); 
	Load_Image("drawable/jsb_s.png", &stone);  
	Load_Image("drawable/jsb_j.png", &knife); 
	Load_Image("drawable/think.png", &think); 
	Load_Image("drawable/btn_focus.png", &btn_focus); 
	Load_Image("drawable/btn_normal.png", &btn_normal); 
	Load_Image("drawable/btn_over.png", &btn_over); 
	Load_Image("drawable/btn_down.png", &btn_down); 
	Load_Image("drawable/icon.png", &icon);
	
	/* ����һ��LCUI���򴰿� */
	width  = 320; /* ��¼���ڵĿ�� */
	height = 240; /* ��¼���ڵĸ߶� */ 
	
	/* ����һ������ */
	window = Create_Widget("window"); 
	/* �趨��������ʾ���ı� */
	Set_Window_Title_Text(window, "��ȭ");
	Set_Window_Title_Icon(window, &icon);
	Resize_Widget(window, Size(width, height));
	
	/* �ڴ����ڴ�����Ӧ�Ĳ���������ȡ����ָ�� */ 
	label		= Create_Widget("label"); /* label������������ʾ���� */
	l_vs		= Create_Widget("label");
	/* button�������ṩ�ɱ�����İ�ť��������ʹ���Զ�����ı� */
	btn_b		= Create_Button_With_Text("��"); 
	btn_s		= Create_Button_With_Text("ʯͷ");
	btn_j		= Create_Button_With_Text("����");
	btn_next	= Create_Button_With_Text("��һ��");
	/* picture_box������������ʾͼ�� */
	cpu_pic_box = Create_Widget("picture_box"); /* �����������ʾ�������ȭ */
	me_pic_box  = Create_Widget("picture_box"); /* �����������ʾ��ҳ���ȭ */
	/* �趨label������ʾ���ı����� */
	Set_Label_Text(label, "����ȭ��Ϸ��liuchao35758600����");
	
	/* �趨label�����е������СΪ55���أ���ɫΪ��ɫ */
	Set_Label_Font(l_vs, 55, NULL); 
	Set_Label_Text(l_vs, "<color=255,0,0>Vs.</color>");
	/* ���ð�ť���Զ��ߴ���� */
	Disable_Widget_Auto_Size(btn_b);
	Disable_Widget_Auto_Size(btn_s);
	Disable_Widget_Auto_Size(btn_j);
	Disable_Widget_Auto_Size(btn_next);
	/* ������Щ�����Ĵ�С */
	Resize_Widget(btn_b, Size(80, 30));
	Resize_Widget(btn_s, Size(80, 30));
	Resize_Widget(btn_j, Size(80, 30));
	Resize_Widget(btn_next, Size(80, 30));
	Resize_Widget(me_pic_box, Size(110, 140));
	Resize_Widget(cpu_pic_box, Size(110, 140));
	/* �Զ������ĸ���ť�ķ�� */
	Custom_Button_Style(btn_b,    &btn_normal, &btn_over, &btn_down, &btn_focus, NULL); 
	Custom_Button_Style(btn_s,    &btn_normal, &btn_over, &btn_down, &btn_focus, NULL); 
	Custom_Button_Style(btn_j,    &btn_normal, &btn_over, &btn_down, &btn_focus, NULL); 
	Custom_Button_Style(btn_next, &btn_normal, &btn_over, &btn_down, &btn_focus, NULL); 
	/* �趨�������뷽ʽ�Լ�ƫ�ƾ��� */
	Set_Widget_Align(btn_s, ALIGN_BOTTOM_LEFT, Pos(-1, 0));  /* ���� */
	Set_Widget_Align(btn_j, ALIGN_BOTTOM_CENTER, Pos(-40, 0)); /* ���� */
	Set_Widget_Align(btn_b, ALIGN_BOTTOM_CENTER, Pos(40, 0));  /* ���� */
	Set_Widget_Align(btn_next, ALIGN_BOTTOM_RIGHT, Pos(1, 0)); /* ���� */
	Set_Widget_Align(me_pic_box, ALIGN_MIDDLE_LEFT, Pos(5, -5)); /* ���� */
	Set_Widget_Align(cpu_pic_box, ALIGN_MIDDLE_RIGHT, Pos(-5, -5)); /* ���� */
	Set_Widget_Align(label, ALIGN_TOP_CENTER, Pos(0, 3));   /* ���� */
	Set_Widget_Align(l_vs, ALIGN_MIDDLE_CENTER, Pos(0, 0)); /* ���� */
	/* �趨�����ı߿��Լ���ɫ */
	Set_Widget_Border(me_pic_box, RGB(0,0,0), Border(1, 1, 1, 1));
	Set_Widget_Border(cpu_pic_box, RGB(0,0,0), Border(1, 1, 1, 1));
	/* ������Ϸ */
	clear_game();
	/* �����ؼ��� Main_Loop_Quit���������������ؼ������º󣬳����˳���ѭ�� */
	LCUI_Key_Event_Connect(KEY_ESC, Main_Loop_Quit, NULL);
	/* ������Щ��ť�ĵ����¼� */ 
	Widget_Clicked_Event_Connect(btn_s, select_stone, NULL); 
	
	Widget_Clicked_Event_Connect(btn_j, select_knife, NULL); 
	
	Widget_Clicked_Event_Connect(btn_b, select_cloth, NULL); 
	
	Widget_Clicked_Event_Connect(btn_next, next, NULL);
	/* �����Ǽ��������ڿͻ�����Ҳ�����Դ��ڿͻ���Ϊ���� */
	Window_Client_Area_Add(window,btn_s);
	Window_Client_Area_Add(window,btn_j);
	Window_Client_Area_Add(window,btn_b);
	Window_Client_Area_Add(window,label);
	Window_Client_Area_Add(window,l_vs);
	Window_Client_Area_Add(window,me_pic_box);
	Window_Client_Area_Add(window,cpu_pic_box);
	Window_Client_Area_Add(window,btn_next);
	/* ��ʾ�����Լ����� */ 
	Show_Widget(btn_s);
	Show_Widget(btn_j);
	Show_Widget(btn_b);
	Show_Widget(label);
	Show_Widget(l_vs);
	Show_Widget(me_pic_box);
	Show_Widget(cpu_pic_box);
	Show_Widget(btn_next);
	Show_Widget(window);
	return LCUI_Main(); /* ����LCUI����ѭ�� */
}
 
