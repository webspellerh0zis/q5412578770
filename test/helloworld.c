// ��LCUIʵ��hello,world����

#include <LCUI_Build.h> /* ����LCUI��ͷ�ļ� */
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_RES_H
#include <unistd.h>

int main(int argc, char*argv[]) 
{
	/* Ŀǰ���ݸ�LCUI_Init()�����Ĳ���û�ã��Ժ�����ƣ�����ָ������·�����Ƿ���ʾ����α�� */
    LCUI_Init(argc, argv);
    LCUI_Widget *window, *label;
    LCUI_Graph pic;
    /* ��ʼ���ṹ�� */
    Graph_Init(&pic);
    /* ��������Դ���ͼ�����ݣ����ͼ����18x18�ߴ��LCUI��ͼ�� */
    Load_Graph_Icon_LCUI_18x18(&pic);
    /* �������� */
    window  = Create_Widget("window");
    label   = Create_Widget("label");
    
    /* �趨���ڱ�����ı� */
    Set_Window_Title_Text(window, "���Դ���");
    /* �趨���ڱ����ͼ�� */
    Set_Window_Title_Icon(window, &pic);
    /* �ı䴰�ڵĳߴ� */
    window->resize(window, Size(320, 240));
    /* �����ڿͻ�����Ϊlabel������������ӽ�ȥ */
    Window_Client_Area_Add(window, label);
    /* ������ʾ */
    label->set_align(label, ALIGN_MIDDLE_CENTER, Pos(0,0));
    /* 
     * �趨label������ʾ���ı�
     * <color=R,G,B>����</color> ��ʾ���ǣ������֡���ʹ���Զ�����ɫ 
     * Set_Label_Text����ʹ�÷�����sprintf��������һ����֧�ֿ��Ʒ�
     **/
    Set_Label_Text(label, 
                "Hello,World!\n"
                "<color=255,0,0>��</color>"
                "<color=100,155,0>��</color>"
                "<color=0,0,0>,</color>"
                "<color=0,255,0>��</color>"
                "<color=0,100,155>��</color>"
                "<color=0,0,255>��</color>\n"); 
    
    /* 
     * �ı�label�����������С����λΪpixel(����)������pt��������
     * �������������Զ���ʹ�õ������ļ������趨ΪNULL����ʾ��ʹ�á�
     **/
    Set_Label_Font(label, 24, NULL);
    /* ��ʾ���� */
    label->show(label);
    window->show(window);
    return LCUI_Main(); /* ������ѭ�� */
}

