/* ***************************************************************************
 * LCUI_Resources.h -- LCUI��s Built-in resources, usually are graphics
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
 * LCUI_Resources.h -- LCUI ��������Դ��ͨ����һЩͼ��
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
int Load_Graph_Default_Cursor(LCUI_Graph *pic);
/* ���ܣ�����Ĭ�ϵ����ָ���ͼ�� */ 

int Load_Graph_Mosaics(LCUI_Graph *pic);
/* ���ܣ�����������ͼ�� */ 

int Load_Graph_ProgressBar_Img(LCUI_Graph *out);

int Load_Graph_ProgressBar_Fore(LCUI_Graph *out);

int Load_Graph_Icon_Error(LCUI_Graph *pic);
/* ���ܣ�����ͼ�Σ���ͼ��Ϊ������ͼ�� */ 

int Load_Graph_Icon_Help(LCUI_Graph *pic);
/* ���ܣ�����ͼ�Σ���ͼ��Ϊ��������ͼ�� */ 

int Load_Graph_Icon_Info(LCUI_Graph *pic);
/* ���ܣ�����ͼ�Σ���ͼ��Ϊ����Ϣ��ͼ�� */ 

int Load_Graph_Icon_Warning(LCUI_Graph *pic);
/* ���ܣ�����ͼ�Σ���ͼ��Ϊ�����桱ͼ�� */ 

int Load_Graph_Icon_LCUI(LCUI_Graph *pic);
/* ���ܣ�����ͼ�Σ���ͼ��ΪLCUI��LOGO */

int Load_Graph_Icon_LCUI_18x18(LCUI_Graph *out);

int Load_Graph_Default_TitleBar_CloseBox_Normal(LCUI_Graph *pic);
/* ���ܣ�����ͼ�Σ���ͼ��Ϊ�������ϵĹرհ�ť����ťΪ����״̬ */ 

int Load_Graph_Default_TitleBar_CloseBox_HighLight(LCUI_Graph *pic);
/* ���ܣ�����ͼ�Σ���ͼ��Ϊ�������ϵĹرհ�ť����ťΪ����״̬ */ 

int Load_Graph_Default_TitleBar_CloseBox_Down(LCUI_Graph *pic);
/* ���ܣ�����ͼ�Σ���ͼ��Ϊ�������ϵĹرհ�ť����ťΪ����״̬ */ 

int Load_Graph_Default_TitleBar_BG(LCUI_Graph *pic);
/*
 * ���ܣ�����Ĭ�ϵı���������ͼ
 */

int Load_Graph_Default_CheckBox_Off_Normal(LCUI_Graph *pic);

int Load_Graph_Default_CheckBox_Off_Selected(LCUI_Graph *pic);

int Load_Graph_Default_CheckBox_Off_Disabled(LCUI_Graph *pic);

int Load_Graph_Default_CheckBox_On_Normal(LCUI_Graph *pic);

int Load_Graph_Default_CheckBox_On_Selected(LCUI_Graph *pic);

int Load_Graph_Default_CheckBox_On_Disabled(LCUI_Graph *pic);


int Load_Graph_Default_RadioButton_Off_Normal(LCUI_Graph *pic);

int Load_Graph_Default_RadioButton_Off_Selected(LCUI_Graph *pic);

int Load_Graph_Default_RadioButton_Off_Pressed(LCUI_Graph *pic);

int Load_Graph_Default_RadioButton_On_Normal(LCUI_Graph *pic);

int Load_Graph_Default_RadioButton_On_Selected(LCUI_Graph *pic);

int Load_Graph_Default_RadioButton_On_Pressed(LCUI_Graph *pic);

