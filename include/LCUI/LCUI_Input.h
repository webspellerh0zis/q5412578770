/* ***************************************************************************
 * LCUI_Input.h -- Processing  input device's data
 * 
 * Copyright (C) 2012 by
 * Liu Chao
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
 * LCUI_Input.h -- ���������豸������
 *
 * ��Ȩ���� (C) 2012 ������ 
 * ����
 * 
 * ����ļ���LCUI��Ŀ��һ���֣�����ֻ���Ը���GPLv2���Э����ʹ�á����ĺͷ�����
 *
 * (GPLv2 �� GNUͨ�ù������֤�ڶ��� ��Ӣ����д)
 * 
 * ����ʹ�á��޸Ļ򷢲����ļ����������Ѿ��Ķ�����ȫ���ͽ���������Э�顣
 * 
 * LCUI ��Ŀ�ǻ���ʹ��Ŀ�Ķ�����ɢ���ģ��������κε������Σ�����û�������Ի���
 * ����;���������������������GPLv2���Э�顣
 *
 * ��Ӧ���յ������ڱ��ļ���GPLv2���Э��ĸ�������ͨ����LICENSE.TXT�ļ��У����
 * û�У���鿴��<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/

#ifndef __LCUI_INPUT_H__
#define __LCUI_INPUT_H__

#define KEY_ESC    27


#define KEY_ENTER   10

#define _PC_

/* �����ŵ����NP1500ѧϰ������Ϸ��A��B���ļ�ֵ */
#define KEY_AA      342
#define KEY_BB      343

#ifndef _PC_

#define KEY_DOWN    172
#define KEY_UP      171
#define KEY_LEFT    174
#define KEY_RIGHT   173

#else

#define KEY_DOWN    184
#define KEY_UP      183
#define KEY_LEFT    186
#define KEY_RIGHT   185

#endif

#define KEY_DELETE  127
/*
#define KEY_DOWN    264
#define KEY_UP      260
#define KEY_LEFT    272
#define KEY_RIGHT   268
 */

#define	PRESSED	10
#define	FREE	5

#define MOUSE_LEFT_KEY		1
#define MOUSE_RIGHT_KEY		2

/****************************** Mouse *********************************/
int Mouse_LeftButton(LCUI_MouseEvent *event);
/*
 * ���ܣ��������¼�����������״̬
 * ˵�����ú���ֻ��������Ӧ��갴��״̬�����ı�ʱ���жϰ���״̬��
 * ����ֵ��
 *   -2  �¼�ָ��ΪNULL
 *   -1  ��ֵ�����������ļ�ֵ
 *   0   �������Ѿ��ͷ�
 *   1   ���������ڰ���״̬
 **/ 

int Mouse_RightButton(LCUI_MouseEvent *event);
/*
 * ���ܣ��������¼�������Ҽ���״̬
 * ˵�����ú���ֻ��������Ӧ��갴��״̬�����ı�ʱ���жϰ���״̬��
 * ����ֵ��
 *   -2  �¼�ָ��ΪNULL
 *   -1  ��ֵ��������Ҽ��ļ�ֵ
 *   0   ����Ҽ��Ѿ��ͷ�
 *   1   ����Ҽ����ڰ���״̬
 **/ 

int Click_LeftButton (LCUI_MouseEvent *event);
/*
 * ���ܣ�����Ƿ��ǰ�������
 **/ 

void Send_Mouse_Event(int event_id, LCUI_MouseEvent *event);
/* ���ܣ����ѹ�������¼��ĳ��������� */ 

int KeyQueue_Find(LCUI_Queue *queue, int key_code);
/* ����:�ڰ�����ֵ���������ָ����ֵ��λ�� */ 

void Processing_Mouse_Event(int button_type, LCUI_MouseEvent *event);
/* ���ܣ��������������¼� */ 

int Check_Mouse_Support();
/* ���ܣ��������֧�� */ 

int Enable_Mouse_Input();
/* ���ܣ�����������봦�� */ 

int Disable_Mouse_Input();
/* ���ܣ�����������봦�� */ 
/**************************** Mouse End *******************************/


/***************************** Key *************************************/ 
int Set_Raw(int t);

int Check_Key(void);
/* 
 * ���ܣ�����Ƿ��а������� 
 * ����ֵ��
 *   1   �а�������
 *   2   �ް�������
 * */ 


int Get_Key(void);
/* ���ܣ���ȡ�����µİ����ļ�ֵ */ 

int Find_Pressed_Key(int key);
/*
 * ���ܣ����ָ����ֵ�İ����Ƿ��ڰ���״̬
 * ����ֵ��
 *   1 ����
 *   0 ������
 **/ 
 
int Enable_Key_Input();
/* ���ܣ����ð������봦�� */ 

int Disable_Key_Input();
/* ���ܣ������������봦�� */ 
/*************************** Key End **********************************/


/************************* TouchScreen *********************************/ 
int Check_TouchScreen_Support();
/* ���ܣ�����Ƿ�֧�ִ�ƿ */ 

int Enable_TouchScreen_Input();
/* ���ܣ�����������봦�� */ 

int Disable_TouchScreen_Input();
/* ���ܣ�����������봦�� */ 

/*********************** TouchScreen End *******************************/

#endif /* __LCUI_INPUT_H__ */
