// ��LCUIʵ�ֵļ�����Ƭ�鿴��

#include <LCUI_Build.h>
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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <locale.h>
#include <math.h>
/* ����Щ�����������ݣ�ֻ��ʹ��ȫ�ֱ��������������鷳 */
static LCUI_Widget	*window, *image_box, *tip_text, *tip_box,
						*image_info_text, *image_info_box,
						*btn_zoom[2], *btn_switch[2], *container[2];
						
static float	mini_scale = 0, scale = 1;   /* ��¼���ű��� */
static char	name[256], **filename = NULL;
static int		total_files = 0, current = 0;

static pthread_t thread_hide, thread_loading, thread_viewer;

void image_zoom_in()
/* ���ܣ��Ŵ�ͼ�� */
{
	LCUI_Size size;
	LCUI_Graph *image; 
	scale += 0.25; /* ���ű�������25% */
	if(scale >= 2) 
	{
		Disable_Widget(btn_zoom[1]);/* ���á��Ŵ󡱰�ť */
		scale = 2.0; /* ����ܳ���200% */
	}
	/* Ϊ���ܴﵽ100%�����˸��жϣ�������ű����ӽ�100%���Ǿ�ֱ�ӵ���100% */
	if(scale > 0.85 && scale < 1.10) scale = 1; 
	/* ����������������� */
	Zoom_PictureBox_View_Area(image_box, scale);
	scale = Get_PictureBox_Zoom_Scale(image_box);/* ��ȡ���ű��� */
	image = Get_PictureBox_Graph(image_box); /* ��ȡͼ��ָ�� */
	size = Get_Graph_Size(image);
	Set_Label_Text(
		image_info_text, 
		"�ļ��� (%d/%d) %s\n"
		"�ߴ磺 %d x %d ����\n"
		"���ţ� %.2f%%",
		current+1, total_files, name,
		size.w, size.h, 100*scale);
	/* ���á���С����ť */
	Enable_Widget(btn_zoom[0]); 
}

void image_zoom_out()
/* ���ܣ���Сͼ�� */
{
	LCUI_Size size;
	LCUI_Graph *image;
	
	scale -= 0.25; /* ���ű����Լ�25% */
	/* ��С���ܵ���mini_scale�б�ʾ�ı��� */
	if(scale <= mini_scale) 
	{
		scale = mini_scale;
		Disable_Widget(btn_zoom[0]);
	}
	if(scale > 0.85 && scale < 1.10 && scale != mini_scale)  
		scale = 1; 
	if(scale > 0.05 && scale < mini_scale + 0.05) 
		scale = mini_scale;
	/* ����������������� */
	Zoom_PictureBox_View_Area(image_box, scale);
	scale = Get_PictureBox_Zoom_Scale(image_box);
	image = Get_PictureBox_Graph(image_box); /* ��ȡͼ��ָ�� */
	size = Get_Graph_Size(image);
	Set_Label_Text(
		image_info_text, 
		"�ļ��� (%d/%d) %s\n"
		"�ߴ磺 %d x %d ����\n"
		"���ţ� %.2f%%",
		current+1, total_files, name,
		size.w, size.h, 100*scale);
		 
	if(fabs(mini_scale - scale) < 0.01)
		Disable_Widget(btn_zoom[0]); 
	else
		Enable_Widget(btn_zoom[0]);
	Enable_Widget(btn_zoom[1]); 
}


void move_view_area(LCUI_Widget *widget, LCUI_DragEvent *event)
/* ���ܣ��ƶ�������� */
{
	LCUI_Pos pos; 
	static LCUI_Pos old_pos, offset_pos;
	if(event->first_click)
	{/* ����Ǹձ���� */
		/* ��¼�����ʱ��λ�ã��Ժ���ƶ������Ը�λ��Ϊ�����������λ�� */
		old_pos = Get_PictureBox_View_Area_Pos(widget);
		/* �������λ����ͼƬ��λ�õ�ƫ�ƾ��� */
		offset_pos = Pos_Sub(Get_Widget_Global_Pos(widget), old_pos); 
	} 
	else if(event->end_click);
	else
	{/* �����Ǿ��������϶� */
		/* ��ȡ����������λ�� */
		pos = Pos_Sub(event->new_pos, offset_pos);
		/* �ó���λ������λ�õġ�� */
		pos = Pos_Sub(old_pos, pos);
		/* �������������������ƶ������෴�ķ����ƶ�����ˣ���λ��ʵ������λ���롰��ĺ� */
		pos = Pos_Add(old_pos, pos); 
		Move_PictureBox_View_Area(widget, pos);
	}
}

//������ð�ť����������򣬾�ʹ��������ĸ�����
void left_move_area()
{
	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(image_box);
	rect.x -= rect.width/2.0;
	if(rect.x < 0) rect.x = 0;
	Move_PictureBox_View_Area(image_box, Pos(rect.x, rect.y));
}

void right_move_area()
{ 
	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(image_box);
	rect.x += rect.width/2.0; 
	Move_PictureBox_View_Area(image_box, Pos(rect.x, rect.y));
}

void up_move_area()
{

	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(image_box);
	rect.y -= rect.height/2.0;
	if(rect.y < 0) rect.y = 0;
	Move_PictureBox_View_Area(image_box, Pos(rect.x, rect.y));
}

void down_move_area()
{ 
	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(image_box);
	rect.y += rect.height/2.0; 
	Move_PictureBox_View_Area(image_box, Pos(rect.x, rect.y));
}

//�������߳��������ذ�ť�Լ�ͼ����Ϣ
static int wait_hide_btn = 4, wait_hide_info = 4;
void *hide()
{
	while(LCUI_Active())
	{
		if(wait_hide_btn == 0)
		{
			Hide_Widget ( container[0] );
			Hide_Widget ( container[1] );
		}
		if(wait_hide_info == 0)
			Hide_Widget ( image_info_box );
			
		if(wait_hide_btn > -1)
			wait_hide_btn--;
		if(wait_hide_info > -1)
			wait_hide_info--;
		sleep(1);
	}
	LCUI_Thread_Exit(NULL);
}

void show_image_info()
{
	Show_Widget ( image_info_box );
	wait_hide_info = 4;
}

void show_button()
{
	Show_Widget ( container[0] );
	Show_Widget ( container[1] );
	wait_hide_btn = 4;
}

void get_filename(char *filepath, char *out_name)
/* ���ܣ����ڻ�ȡ�ļ��� */
{
	int m,n = 0;
	char *p;
	for(m=0; m<strlen(filepath); ++m) 
	{
		if(filepath[m]=='/') 
		{
			n = m+1;
		}
	}
	p = filepath + n;
	strcpy(out_name, p);
}

void get_filepath(char *filepath, char *out_path)
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
	memset(out_path, 0, sizeof(char)*strlen(out_path)); 
}
 
int get_format(char *format, char *filename)
/* ���ܣ���ȡ�ļ���ʽ */
{
	int i, j, len;
	len = strlen(filename);
	for(i=len-1; i>=0; --i)
	{
		if(filename[i] == '.')
		{
			for(j=0, ++i; i<len; ++i, ++j)
			{
				format[j] = filename[i];
			}
			format[j] = 0;
			return 0;
		}
	}
	strcpy(format, "");
	return -1;
} 

 
char **scan_imgfile(char *dir, int *file_num)
/* ���ܣ�ɨ��ͼƬ�ļ���������ļ��б� */
{
	int i, len, n; 
	char **filelist, format[256], path[1024];
	struct dirent **namelist;
	if(strlen(dir) == 0)
		n = scandir(".", &namelist, 0, alphasort);
	else	
	n = scandir(dir, &namelist, 0, alphasort);
	
	if (n < 0)
		return 0; 
	else 
	{
		filelist = (char **)malloc(sizeof(char *)*n);
		for(i=0, *file_num=0; i<n; i++)
		{
			if(namelist[i]->d_type==8)/* ������ļ� */ 
			{
				get_format(format, namelist[i]->d_name);
				if(strlen(format) > 2 && 
				(strcasecmp(format, "png") == 0
				|| strcasecmp(format, "bmp") == 0
				|| strcasecmp(format, "jpg") == 0
				|| strcasecmp(format, "jpeg") == 0))
				{
					sprintf(path, "%s%s", dir, namelist[i]->d_name);
					len = strlen( path );
					filelist[*file_num] = (char *)malloc(sizeof(char)*(len+1)); 
					strcpy(filelist[*file_num], path);  
					++*file_num;
				}
			}
		}
		if(*file_num > 0)
			filelist = (char**)realloc(filelist, *file_num*sizeof(char*));
		else
		{
			free(filelist);
			filelist = NULL;
		}
	}
	for(i=0;i<n;++i)
		free(namelist[i]); 
	free(namelist); 
	
	return filelist;
}

void *load_imagefile(void *file)
/* ���ܣ�����ͼƬ�ļ� */
{
	LCUI_Size size;
	LCUI_Graph *image;
	int *result, i;
	
	result = calloc(1, sizeof(int));
	get_filename(file, name);
	Set_Label_Text(tip_text, "������ͼƬ..."); 
	tip_box->set_alpha(tip_box, 200);
	Show_Widget(tip_text); 
    Show_Widget(tip_box);
    
    /* ���ұ��ļ�����λ�� */ 
    for(i=0; i<total_files; ++i)
    {
		if(strcmp(file, filename[i]) == 0)
		{ 
			current = i;
			break;
		}
	}
	
    image = Get_PictureBox_Graph(image_box); 
	Free_Graph(image); 
    
	Set_PictureBox_Size_Mode(image_box, SIZE_MODE_CENTER); 
	*result = Set_PictureBox_Image_From_File(image_box, file); 
	
	if(*result != 0)
	{/* ���ͼƬ�ļ���ȡʧ�� */
		Set_Label_Text(tip_text, "ͼƬ����ʧ��!");
		size.w = 0;
		size.h = 0;
	}
	else 
	{
		image = Get_PictureBox_Graph(image_box);
		size = Get_Graph_Size(image);
		if( 1 == Size_Cmp(size, Get_Widget_Size(image_box) ))
		{
		/* ���ͼƬ�ߴ����PictureBox�ĳߴ磬�͸ı�PictureBox��ͼ����ģʽ */
			Set_PictureBox_Size_Mode(image_box, SIZE_MODE_ZOOM);
			mini_scale = Get_PictureBox_Zoom_Scale(image_box);
		}
		else
			mini_scale = 0.25;
		Hide_Widget(tip_box);
	}
		
	scale = Get_PictureBox_Zoom_Scale(image_box); 
	Set_Label_Text(
		image_info_text, 
		"�ļ��� (%d/%d) %s\n"
		"�ߴ磺 %d x %d ����\n"
		"���ţ� %.2f%%",
		current+1, total_files, name,
		size.w, size.h, 100*scale);
	/* ����͸���� */
	image_info_box->set_alpha(image_info_box, 200);
	Show_Widget(image_info_text);
	Show_Widget(image_info_box);
	/* ���̷߳��ؽ�� */
	LCUI_Thread_Exit(result);
}

void *viewer(void *file)
/* ���ܣ��������ͼƬ��ʾ���� */
{
	LCUI_Graph *image, bg;
	int *result = NULL; 
	Graph_Init(&bg);
	/* �ȴ�ͼƬ�������������ȡ�̷߳���ֵ */
	LCUI_Thread_Join(thread_loading, (void**)&result);
	if(*result == 0)
	{/* �����������ͼƬ */
		scale = Get_PictureBox_Zoom_Scale(image_box); /* ��ȡ���ű��� */
		image = Get_PictureBox_Graph(image_box); /* ��ȡͼ��ָ�� */
		if(Graph_Is_PNG(image) && Graph_Have_Alpha(image))
		{
			Load_Graph_Mosaics(&bg);/* ����������ͼ�� */
			Set_Widget_Background_Image(image_box, &bg, LAYOUT_TILE);/* ƽ�̱���ͼ */
			Free_Graph(&bg);
		}
		else Set_Widget_Background_Image(image_box, NULL, LAYOUT_NONE);
	}
	/* ����ӽ���С���ű������Ǿͽ��á���С����ť���������� */
	if(fabs(mini_scale - scale) < 0.01)
		Disable_Widget(btn_zoom[0]); 
	else
		Enable_Widget(btn_zoom[0]);
	/* ���÷Ŵ�ť */
	Enable_Widget(btn_zoom[1]);
	free(result);
	LCUI_Thread_Exit(NULL);
}

int open_image_file(char *filename)
/* ���ܣ���ͼƬ�ļ� */
{
	show_button();
	show_image_info();
	/* �����������̣߳�һ����������ͼ��һ����������������ͼ�� */
	LCUI_Thread_Create(&thread_loading, NULL, load_imagefile, (void*)filename); 
	LCUI_Thread_Create(&thread_viewer, NULL, viewer, (void*)filename);
	return LCUI_Thread_Join(thread_viewer, NULL);
}

void prev_image(LCUI_Widget *widget, void *arg)
/* ���ܣ��л�����һ��ͼ */
{ 
	if(total_files <= 0)
		return;
		
	if(current <= 0)
		current = total_files-1;
	else
		current--;
		
	open_image_file(filename[current]);
}

void next_image(LCUI_Widget *widget, void *arg)
/* ���ܣ��л�����һ��ͼ */
{
	if(total_files <= 0)
		return;
		
	if(current == total_files-1)
		current = 0;
	else
		current++;
		
	open_image_file(filename[current]);
}

int main(int argc, char*argv[]) 
{ 
	int i;
    LCUI_Graph app_icon, btn_zoom_pic[6], btn_switch_pic[6];
	/* �趨Ĭ�������ļ�λ�� */
	Set_Default_Font("../../fonts/msyh.ttf");
    LCUI_Init(argc, argv); 
    Graph_Init(&app_icon); 
    
    for(i=0; i<6; ++i)
    {
		Graph_Init(&btn_zoom_pic[i]); 
		Graph_Init(&btn_switch_pic[i]); 
	}
    /* �������ڲ��� */
    window			= Create_Widget("window");
    /* ������������ */
    container[0]	= Create_Widget(NULL);
    container[1]	= Create_Widget(NULL);
    /* ����������ť���ڷŴ���С */
	btn_zoom[0]		= Create_Widget("button"); /* ����С����ť */
	btn_zoom[1]		= Create_Widget("button"); /* ���Ŵ󡱰�ť */
	/* ����������ť�����л�ͼƬ */
	btn_switch[0]	= Create_Widget("button"); /* ����һ�š���ť */
	btn_switch[1]	= Create_Widget("button"); /* ����һ�š���ť */
	
	image_info_box = Create_Widget(NULL);
	image_info_text = Create_Widget("label");
    
    tip_text = Create_Widget("label"); /* ��ʾ�ı� */
    tip_box  = Create_Widget(NULL); /* ��Ϊ��ʾ�ı������� */
	
	image_box = Create_Widget("picture_box");/* ����һ��ͼƬ���ӣ�������ʾͼƬ */
	
	/* �������ͼ�� */ 
	Load_Image("drawable/icon.png", &app_icon);
	/* �����⼸����ť�ڸ���״̬����ʾ��ͼ�� */ 
	Load_Image("drawable/btn_zoom_up_normal.png", &btn_zoom_pic[0]); 
	Load_Image("drawable/btn_zoom_up_pressed.png", &btn_zoom_pic[1]); 
	Load_Image("drawable/btn_zoom_up_disabled.png", &btn_zoom_pic[2]); 
	Load_Image("drawable/btn_zoom_down_normal.png", &btn_zoom_pic[3]); 
	Load_Image("drawable/btn_zoom_down_pressed.png", &btn_zoom_pic[4]); 
	Load_Image("drawable/btn_zoom_down_disabled.png", &btn_zoom_pic[5]); 
	
	Load_Image("drawable/btn_next_up_normal.png", &btn_switch_pic[0]); 
	Load_Image("drawable/btn_next_up_pressed.png", &btn_switch_pic[1]); 
	Load_Image("drawable/btn_next_up_disabled.png", &btn_switch_pic[2]);  
	Load_Image("drawable/btn_prev_down_normal.png", &btn_switch_pic[3]); 
	Load_Image("drawable/btn_prev_down_pressed.png", &btn_switch_pic[4]); 
	Load_Image("drawable/btn_prev_down_disabled.png", &btn_switch_pic[5]);
	/* �趨��ťͼ����ʽ */
	Custom_Button_Style(	btn_zoom[0], &btn_zoom_pic[3], &btn_zoom_pic[3], 
							&btn_zoom_pic[4], NULL, &btn_zoom_pic[5] );
	Custom_Button_Style(	btn_zoom[1], &btn_zoom_pic[0], &btn_zoom_pic[0], 
							&btn_zoom_pic[1], NULL, &btn_zoom_pic[2] );
							
	Custom_Button_Style(	btn_switch[0], &btn_switch_pic[0], &btn_switch_pic[0], 
							&btn_switch_pic[1], NULL, &btn_switch_pic[2] );
	Custom_Button_Style(	btn_switch[1], &btn_switch_pic[3], &btn_switch_pic[3], 
							&btn_switch_pic[4], NULL, &btn_switch_pic[5] );
							
	Set_Widget_Border (image_info_box, RGB(50,50,50), Border(1,1,1,1));/* ���ñ߿� */
	Set_Widget_Border (tip_box, RGB(50,50,50), Border(1,1,1,1));
	Set_Widget_Backcolor (image_info_box, RGB(180,215,255));/* ��������Ϊ��ɫ */ 
	Set_Widget_Backcolor (tip_box, RGB(180,215,255)); 
	Set_Widget_BG_Mode (image_info_box, BG_MODE_FILL_BACKCOLOR);
	Set_Widget_BG_Mode (tip_box, BG_MODE_FILL_BACKCOLOR);
	
	Enable_Widget_Auto_Size(image_info_box);
	
	LCUI_Size size[2], btn_switch_size[2], btn_zoom_size[2];
	btn_zoom_size[0] = Get_Graph_Size(&btn_zoom_pic[0]);
	btn_zoom_size[1] = Get_Graph_Size(&btn_zoom_pic[0]);
	btn_switch_size[0] = Get_Graph_Size(&btn_switch_pic[0]);
	btn_switch_size[1] = Get_Graph_Size(&btn_switch_pic[1]);
	size[0] = Size(	btn_zoom_size[0].w, 
					btn_zoom_size[0].h + btn_zoom_size[1].h);
	size[1] = Size( btn_switch_size[0].w+btn_switch_size[1].w,
					btn_switch_size[0].h);
	/* ���ĸ��������ĳߴ� */
    Resize_Widget(window, Size(320, 240));
	Resize_Widget(btn_zoom[0], btn_zoom_size[0]);
	Resize_Widget(btn_zoom[1], btn_zoom_size[1]);
	Resize_Widget(btn_switch[0], btn_switch_size[0]);
	Resize_Widget(btn_switch[1], btn_switch_size[1]);
	Resize_Widget(container[0], size[0]);
	Resize_Widget(container[1], size[1]);
	Resize_Widget(image_box, Size(318, 213));
	Resize_Widget(tip_box, Size(150, 30));
	
	/* �趨����������ʾ�ĳ���ͼ���Լ��ı� */
	Set_Window_Title_Icon(window, &app_icon); 
	
	char version[20];
	Get_LCUI_Version(version);
	if(strcmp(version, "0.12.4") == 0)
		Set_Window_Title_Text(window, "��Ƭ�鿴�� v0.3"); 
	else
		Set_Window_Title_Text(window, "��Ƭ�鿴�� v0.3 <color=240,0,0>(���ڼ���������)</color>"); 
	
    /* �趨�����Ĳ��� */
    Set_Widget_Align(btn_zoom[0], ALIGN_BOTTOM_CENTER, Pos(0, 0));
    Set_Widget_Align(btn_zoom[1], ALIGN_TOP_CENTER, Pos(0, 0));
    Set_Widget_Align(btn_switch[0], ALIGN_MIDDLE_RIGHT, Pos(0, 0));
    Set_Widget_Align(btn_switch[1], ALIGN_MIDDLE_LEFT, Pos(0, 0));
    Set_Widget_Align(container[0], ALIGN_MIDDLE_RIGHT, Pos(-2, 0));
    Set_Widget_Align(container[1], ALIGN_BOTTOM_LEFT, Pos(2, -2));
    Set_Widget_Align(image_box, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    Set_Widget_Align(tip_box, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    Set_Widget_Align(tip_text, ALIGN_MIDDLE_LEFT, Pos(25, 0));
    Set_Widget_Align(image_info_text, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    Set_Widget_Align(image_info_box, ALIGN_TOP_LEFT, Pos(3, 3));
    
    /* ������������Ӧ�������� */
    Widget_Container_Add(container[0], btn_zoom[0]);
    Widget_Container_Add(container[0], btn_zoom[1]);
    Widget_Container_Add(container[1], btn_switch[0]);
    Widget_Container_Add(container[1], btn_switch[1]);
    Widget_Container_Add(tip_box, tip_text);
    Widget_Container_Add(image_info_box, image_info_text);
    Window_Client_Area_Add(window, container[0]); 
    Window_Client_Area_Add(window, container[1]); 
    Window_Client_Area_Add(window, image_box);
    Window_Client_Area_Add(window, tip_box);
    Window_Client_Area_Add(window, image_info_box);
    /* ����������������¼� */
    Widget_Clicked_Event_Connect(btn_switch[0], next_image, NULL);
    Widget_Clicked_Event_Connect(btn_switch[1], prev_image, NULL);
    Widget_Clicked_Event_Connect(btn_zoom[0], image_zoom_out, NULL);
    Widget_Clicked_Event_Connect(btn_zoom[1], image_zoom_in, NULL);
    Widget_Drag_Event_Connect(image_box, move_view_area);
    /* ��ʾ���� */
    Show_Widget(image_box);
    Show_Widget(container[0]);
    Show_Widget(container[1]);
    Show_Widget(window); 
    
	LCUI_Key_Event_Connect( KEY_ESC,   Main_Loop_Quit,  NULL);
    if(argc == 2)
	{/* ����ܹ���2������ */ 
		char path[1024];
		get_filepath(argv[1], path);
		filename = scan_imgfile(path, &total_files); 
		open_image_file (argv[1]);
		Show_Widget(btn_zoom[0]);
		Show_Widget(btn_zoom[1]);
		Show_Widget(btn_switch[0]);
		Show_Widget(btn_switch[1]);
		/* ���������¼���������ָ����ֵ�İ����󣬻�����ѹ����Ļص����� */
		LCUI_Key_Event_Connect( '=',   image_zoom_in,    NULL);
		LCUI_Key_Event_Connect( '-',   image_zoom_out,   NULL);
		LCUI_Key_Event_Connect( KEY_AA,   image_zoom_in,    NULL);
		LCUI_Key_Event_Connect( KEY_BB,   image_zoom_out,   NULL);
		LCUI_Key_Event_Connect( KEY_LEFT,   left_move_area,  NULL);
		LCUI_Key_Event_Connect( KEY_RIGHT,  right_move_area, NULL);
		LCUI_Key_Event_Connect( KEY_UP,     up_move_area,    NULL);
		LCUI_Key_Event_Connect( KEY_DOWN,   down_move_area,  NULL);
		LCUI_Key_Event_Connect( 'n',   next_image,  NULL);
		LCUI_Key_Event_Connect( 'N',   next_image,  NULL);
		LCUI_Key_Event_Connect( 'b',   prev_image,  NULL);
		LCUI_Key_Event_Connect( 'b',   prev_image,  NULL);
		LCUI_Key_Event_Connect( 'i',   show_image_info,  NULL);
		Widget_Clicked_Event_Connect( image_box, show_button, NULL);
		LCUI_Thread_Create(&thread_hide, NULL, hide, NULL);
	}
	else
	{
		char myname[256];
		LCUI_Widget *help_text;
		get_filename(argv[0], myname);
		help_text = Create_Widget("label");
		Set_Label_Text(help_text,
		"<color=30,145,220>�÷���</color>\n"
		"<color=30,220,60>%s</color>  �ļ�\n\n"
		"<color=30,145,220>���ƣ�</color>\n"
		"<color=255,0,0>�������</color> �ƶ��������\n"
		"<color=255,0,0>���ؼ���</color> �˳�����\n"
		"<color=255,0,0>+/-����</color>  �Ŵ����Сͼ��\n" 
		"<color=255,0,0>N/B����</color> �����л�ͼ��\n" 
		"<color=255,0,0> I ����</color>  ��ʾͼ����Ϣ\n"
		"��Ҳ����ʹ������������ϵİ�ť���п��ơ�\n"
		"������Ŀǰ֧�����bmp, png, jpeg��ʽ��ͼƬ��\n",
		myname
		);
		Set_Widget_Align(help_text, ALIGN_TOP_LEFT, Pos(10, 10));
		Window_Client_Area_Add(window, help_text);
		Show_Widget(help_text);
		Hide_Widget(container[0]);
		Hide_Widget(container[1]); 
	}
    return LCUI_Main(); /* ������ѭ�� */  
}

