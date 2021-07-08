/* ***************************************************************************
 * LCUI_Misc.c -- no specific categories of miscellaneous function
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
 * LCUI_Misc.c -- û�о������������
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
 
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H

void String_Init(LCUI_String *in)
/* ���ܣ���ʼ��String�ṹ���е����� */
{
	in->size = 0;
	in->string = NULL;
}


void Rect_Init(LCUI_Rect *rect)
/* ���ܣ���ʼ��������������� */
{
	rect->x      = 0;
	rect->y      = 0;
	rect->width  = 0;
	rect->height = 0; 
	rect->center_x = 0;
	rect->center_y = 0;
}


void Border_Init(LCUI_Border *in)
/* ���ܣ���ʼ���߿� */
{ 
	in->left = 0;
	in->bottom = 0;
	in->right = 0;
	in->top = 0; 
}



void Strcpy (LCUI_String * des, char *src)
/* ���ܣ������ַ�����String�ṹ�������� */
{
	if(des == NULL) 
		return;
	if (src != NULL)
	{
		if (des->size != 0)
		{
			free (des->string);
		}
		des->size = sizeof (char) * (strlen (src) + 1);
		des->string = calloc (1, des->size);
		strcpy (des->string, src);
	}
	else
	{
		des->size = sizeof (char) * (2);
		des->string = calloc (1, des->size);
	}
}

int Strcmp(LCUI_String *str1, char *str2)
/* ���ܣ��Ա�str1��str2 */
{
	if (str1 != NULL && str1->size > 0 && str2 != NULL) 
		return strcmp(str1->string, str2); 
	else return -1;
}

void Get_Moved_Rect_Refresh_Area (int new_x, int new_y, LCUI_Rect rect,
							 LCUI_Rect * rect_a, LCUI_Rect * rect_b)
/* ���ܣ���ȡһ�������ƶ�����Ҫˢ�µĲ�������Ҳ����A��B�������� */
{
	Rect_Init (rect_a);
	Rect_Init (rect_b);
	if (rect.x < new_x)
	{							/* ����������� */
		rect_a->x = rect.x;
		rect_a->y = rect.y;
		rect_a->width = new_x - rect.x;
		rect_a->height = rect.height;
		if (rect.y > new_y)
		{						/* ����������� */
			rect_b->x = new_x;
			rect_b->y = new_y + rect.height;
			rect_b->width = rect.width - rect_a->width;
			rect_b->height = rect.y - new_y;
		}
		else
		{
			rect_b->x = new_x;
			rect_b->y = rect.y;
			rect_b->width = rect.width - rect_a->width;
			rect_b->height = new_y - rect.y;
		}
	}
	else if (rect.x > new_x)
	{							/* ����������� */
		rect_a->x = new_x + rect.width;
		rect_a->y = rect.y;
		rect_a->width = rect.x - new_x;
		rect_a->height = rect.height;

		if (rect.y > new_y)
		{						/* ����������� */
			rect_b->x = rect.x;
			rect_b->y = new_y + rect.height;
			rect_b->width = rect.width - rect_a->width;
			rect_b->height = rect.y - new_y;
		}
		else
		{
			rect_b->x = rect.x;
			rect_b->y = rect.y;
			rect_b->width = rect.width - rect_a->width;
			rect_b->height = new_y - rect.y;
		}
	}
	else
	{							/* ����û���������ƶ���ֻ�����Ϻ����� */
		rect_a->x = rect.x;
		rect_a->y = rect.y;
		rect_a->width = 0;
		rect_a->height = 0;
		if (rect.y > new_y)
		{						/* ����������� */
			rect_b->x = rect.x;
			rect_b->y = new_y + rect.height;
			rect_b->width = rect.width;
			rect_b->height = rect.y - new_y;
		}
		else
		{
			rect_b->x = rect.x;
			rect_b->y = rect.y;
			rect_b->width = rect.width;
			rect_b->height = new_y - rect.y;
		}
	}
}


int Rect_Cross_Overlay(LCUI_Rect a, LCUI_Rect b)
/* 
 * ���ܣ�������������Ƿ��ʮ�ּ�ʽ���� 
 * ����ֵ��
 *  1 a����b��
 * -1 a�⣬b��
 *  0 ������
 **/
{
	/* ������������Ƿ��ʮ�ּ�ʽ���� */
	if(a.x < b.x && a.y > b.y 
	&& a.x + a.width  > b.x + b.width 
	&& a.y + a.height < b.y + b.height) 
		return 1;
	if(b.x < a.x && b.y > a.y 
	&& b.x + b.width  > a.x + a.width  
	&& b.y + b.height < a.y + a.height) 
		return -1;
	return 0;
}

int Check_Option(int result, int option)
/* ���ܣ����result�Ƿ����option */
{
	if((result & option) == option) return 1; 
	return 0;
}

int Get_Cut_Area(LCUI_Size container, LCUI_Rect rect, LCUI_Rect *cut)
/* 
 * ���ܣ���ȡ��ü�������
 * ˵����ָ�������ߴ�������е�����λ�ü��ߴ磬���ɵõ�����������Ҫ���вü�����
 *  */
{
	int result = 0;
	cut->x = 0;
	cut->y = 0;
	cut->width = rect.width;
	cut->height = rect.height;
	/* ��ȡ��ü������� */
	if(rect.x < 0) 
	{
		cut->width += rect.x;
		cut->x = 0 - rect.x;
		result = 1;
	}
	if(rect.x + rect.width > container.w)
	{
		cut->width -= (rect.x + rect.width - container.w); 
		result = 1;
	}
	
	if(rect.y < 0)  
	{
		cut->height += rect.y;
		cut->y = 0 - rect.y; 
		result = 1;
	}
	if(rect.y + rect.height > container.h)
	{
		cut->height -= (rect.y + rect.height - container.h); 
		result = 1;
	}
	return result;
}

LCUI_Rect Get_Valid_Area(LCUI_Size container, LCUI_Rect rect)
/* 
 * ���ܣ���ȡָ�������������е���Ч��ʾ���� 
 * ˵����ָ�������������С����ָ�������е�����λ�ü���С�����ܵõ�������ʵ������ʾ
 * ���ĸ�����Χ��
 * */
{
	if (rect.x < 0)
	{
		rect.width += rect.x;
		rect.x = 0;
	}
	if (rect.y < 0)
	{
		rect.height += rect.y;
		rect.y = 0;
	}
	
	if (rect.x + rect.width > container.w) 
		rect.width = container.w - rect.x; 
	if (rect.y + rect.height > container.h) 
		rect.height = container.h - rect.y; 
	
	return rect;
}

int
Rect_Include_Rect (LCUI_Rect a, LCUI_Rect b)
/*
 * ���ܣ�������������У�A�����Ƿ����B����
 * ����ֵ�������������ڰ��й�ϵ����1�����򷵻�0��
 * */
{
	int count = 0, m, n = 0, x[4], y[4];
	b.width -= 1;
	b.height -= 1; 
	/* �ó�����b��4��������� */
	x[0] = b.x;
	y[0] = b.y;
	x[1] = b.x + b.width;
	y[1] = b.y;
	x[2] = b.x;
	y[2] = b.y + b.height;
	x[3] = b.x + b.width;
	y[3] = b.y + b.height; 
	for (m = 0; m < 4; ++m)
	{
		/*printf("[%d](%d>=%d && %d<%d+%d) && (%d>=%d && %d<%d+%d)\n", 
			m, x[m], a.x, x[m], a.x, a.width, 
			y[m], a.y, y[m], a.y, a.height
		);*/
		if ((x[m] >= a.x && x[m] < a.x + a.width)
			&& (y[m] >= a.y && y[m] < a.y + a.height))
		{						/* �����(x[m],y[m])�ھ���A�� */
			//printf("yes\n");
			count += 1;
		}
	}

	if (count == 4)
		n = 1;
	else
		n = 0;
	return n;
}

LCUI_Pos Pos(int x, int y)
/* ���ܣ�ת����LCUI_Pos���� */
{
	LCUI_Pos p;
	p.x = x;
	p.y = y;
	return p;
}

LCUI_Size Size(int w, int h)
/* ���ܣ�ת����LCUI_Size���� */
{
	LCUI_Size s;
	s.w = w;
	s.h = h;
	return s;
}

int Size_Cmp(LCUI_Size a, LCUI_Size b)
/* 
 * ���ܣ��Ա������ߴ�
 * ˵����a����b������1�� b����a������-1������򷵻�0
 * */
{
	if(a.w > b.w || a.h > b.h)
		return 1;
	else if(a.w == b.w && a.h == b.h)
		return 0;
	else 
		return -1;
}
extern int debug_mark;
int Cut_Overlay_Rect (	LCUI_Rect old, LCUI_Rect new, 
						LCUI_Queue *rq	)
/*
 * ���ܣ������ص����ֵ��������Σ����зָ���õ��ָ��ľ���
 * ˵������Ҫ���ھֲ�����ˢ�����ӵ���ˢ�µ������п��ܻ�������ӵ������ص���Ϊ����
 * �ظ�ˢ��ͬһ��������Ҫ�����ʱ�Ծ��ν��зָ�õ���ȫ�ص��Ͳ��ص��ľ��Ρ�
 * ����˵����
 * old �� �Ѵ��ڵľ�������
 * new �� ��Ҫ��ӵľ�������
 * rq  �� ָ����εĶ��е�ָ��
 * ע�⣡���ݲ���ʱ������ߵ�old��newλ�á�
 **/
{
	int i; 
	LCUI_Rect r[5];
	
	for(i=0; i<5; ++i)
		Rect_Init(&r[i]); 
	
	/* ����������ε�x������Ϳ�� */
	r[0].x = new.x;
	r[0].y = new.y; 
	//printf("old,pos(%d,%d), size(%d,%d)\n", old.x, old.y, old.width, old.height);
	//printf("new,pos(%d,%d), size(%d,%d)\n", new.x, new.y, new.width, new.height);
	if(new.x < old.x)
	{/* ���ǰ�������ڱ������ε���� */  
		if(new.x + new.width > old.x)
		{ /* ���X�����뱳�������ص� */  
			r[0].width = old.x - new.x;
			r[1].x = old.x;
			r[2].x = r[1].x;
			r[4].x = r[2].x;
			if(new.x + new.width > old.x + old.width)
			{/* ���ǰ��������X���ϰ����������� */  
				r[1].width = old.width;
				
				r[3].x = old.x + old.width;
				r[3].width = new.x + new.width - r[3].x;
			}
			else  /* �ó�����2�Ŀ�� */ 
				r[1].width = new.x + new.width - old.x;  
			/* �ó�����3��5�Ŀ�� */ 
			r[2].width = r[1].width;
			r[4].width = r[2].width;
		}
		else return -1; 
	}
	else
	{  
		if(old.x + old.width > new.x)
		{ 
			r[1].x = new.x;
			r[2].x = r[1].x; 
			r[4].x = r[2].x;
			
			if(new.x + new.width > old.x + old.width)
			{  
				r[1].width = old.x + old.width - r[1].x;
				r[3].x = old.x + old.width;
				r[3].width = new.x + new.width - r[3].x;
			} 
			else 
				r[1].width = new.width; 
				
			r[2].width = r[1].width;
			r[4].width = r[2].width; 
		}
		else return -1; 
	}
	 
	/* ����������ε�y������͸߶� */
	r[0].height = new.height;
	r[3].y = new.y;
	r[3].height = r[0].height;
	r[4].y = old.y + old.height; 
	if(new.y < old.y)
	{  
		if(new.y + new.height > old.y)
		{  
			r[1].y = new.y; 
			r[1].height = old.y - new.y;
			r[2].y = old.y; 
			
			if(new.y + new.height > old.y + old.height)
			{ /* ���ǰ��������Y���ϰ����������� */ 
				r[2].height = old.height;
				r[4].height = new.y + new.height - r[4].y; 
			}
			else 
			{ 
				r[2].height = new.y + new.height - old.y;  
			}
		}
		else return -1; 
	}
	else
	{  
		if(new.y < old.y + old.height)
		{ 
			r[2].y = new.y; 
			
			if(new.y + new.height > old.y + old.height)
			{  
				r[2].height = old.y + old.height - r[2].y;
				r[4].height = new.y + new.height - r[4].y;
			}
			else 
				r[2].height = new.y + new.height - r[2].y;  
		}
		else return -1; 
	}
	
	//r[0].width -= 1;
	//r[1].height -= 1;
	//r[3].x += 1;
	//r[3].width -= 1;
	//r[4].y += 1;
	//r[4].height -= 1;
	  
	for(i=0; i<5; i++) 
	{ 
		if(debug_mark)
			printf("slip rect[%d]: %d,%d, %d,%d\n", i, r[i].x, r[i].y, r[i].width, r[i].height);
		Queue_Add(rq, &r[i]); 
	}
	return 0;
}



int Get_Overlay_Rect(LCUI_Rect a, LCUI_Rect b, LCUI_Rect *out)
/* ���ܣ���ȡ�������ص����ֵľ��� */
{
	int x = 0,y = 0,w = 0,h = 0;
	
	if(b.x > a.x && b.x+b.width < a.x+a.width)
	{
		x = a.x;
		w = b.width;
	}
	else if(b.x <= a.x && b.x+b.width >= a.x+a.width)
	{
		x = a.x;
		w = a.width;
	}
	else if(b.x+b.width > a.x && b.x+b.width <= a.x+a.width)
	{
		x = a.x;
		w = b.x+b.width - a.x;
	}
	else if(b.x >= a.x && b.x < a.x+a.width)
	{
	   x = b.x;
	   w = a.x+a.width - b.x;
	}
	
	if(b.y > a.y && b.y+b.height < a.y+a.height)
	{
		y = b.y;
		h = b.height;
	}
	else if(b.y <= a.y && b.y+b.height >= a.y+a.height)
	{
	   y = a.y;
	   h = a.height;
	}
	else if(b.y+b.height > a.y && b.y+b.height <= a.y+a.height)
	{
		y = a.y;
		h = b.y+b.height - a.y;
	}
	else if(b.y >= a.y && b.y < a.y+a.height)
	{
		y = b.y;
		h = a.y+a.height - b.y;
	}
	
	*out = Rect(x, y, w, h);
	if (x + w == 0 || y + h == 0) 
		return 0;
	return 1;
}

LCUI_Rect Rect (int x, int y, int width, int height)
/* ���ܣ�����ֵת����LCUI_Rect�ͽṹ�� */
{
	LCUI_Rect s;

	Rect_Init (&s);
	s.x = x;
	s.y = y;
	s.width = width;
	s.height = height;
	return s;
}

LCUI_Border Border(int top, int bottom, int left, int right)
{
	LCUI_Border border;
	border.top = top;
	border.left = left;
	border.right = right;
	border.bottom = bottom;
	return border;
	
}

LCUI_Pos Pos_Add(LCUI_Pos a, LCUI_Pos b)
/* ���ܣ�������LCUI_Pos���ͱ����ĺ� */
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

int Pos_Cmp(LCUI_Pos a, LCUI_Pos b)
/* ���ܣ��Ա����������Ƿ�һ�� */
{
	if(a.x != b.x || a.y != b.y)
		return -1;
	
	return 0;
}

LCUI_Pos Pos_Sub(LCUI_Pos a, LCUI_Pos b)
/* ���ܣ�������LCUI_Pos���ͱ����Ĳ� */
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

int Rect_Inside_Point (LCUI_Pos pos, LCUI_Rect rect)
/* ���ܣ����һ�����Ƿ񱻾��ΰ��� */
{
	if (pos.x >= rect.x && pos.x < rect.x + rect.width-1 && pos.y >= rect.y
		&& pos.y < rect.y + rect.height-1)
		return 1;
	else
		return 0;
}

int
Check_Rect_Overlap (int ax, int ay, int aw, int ah, int bx, int by, int bw,
					int bh)
/*
* ���ܣ�������������Ƿ��ص�
* ����˵����
* ax ������A�����Ͻ��������x�������
* ay ������A�����Ͻ��������y�������
* aw ������A�Ŀ��
* ah ������A�ĸ߶�
* bx ������B�����Ͻ��������x�������
* by ������B�����Ͻ��������y�������
* bw ������B�Ŀ��
* bh ������B�ĸ߶�
* ����ֵ�������ص�����0���ص��򷵻�1��
*/
{
	/* ������������Ƿ��ʮ�ּ�ʽ���� */
	if(ax < bx && ay > by && ax + aw > bx + bw && ay + ah < by + bh) return 1;
	if(bx < ax && by > ay && bx + bw > ax + aw && by + bh < ay + ah) return 1;
	/* 
	 * ������һ����������п������ͼ����ʾ�ϵ�����
	 * ��Ϊ�ֲ�����ˢ�¾��ǿ����������֪�����Ƿ��벿��
	 * �����Ƿ��ص��� 
	 * */
	int m, n = 0, x[4], y[4];
	x[0] = ax;
	y[0] = ay;
	x[1] = ax + aw - 1;
	y[1] = ay;
	x[2] = ax;
	y[2] = ay + ah - 1;
	x[3] = ax + aw - 1; /* ��Ϊ���Ϳ���������Ͻǵ�ĳ���Ҫ-1 */
	y[3] = ay + ah - 1;
	/* ��ֻ�Ǽ���ĸ����Ƿ��ھ����� */
	for (m = 0; m < 4; ++m)
	{
		if ((x[m] >= bx && x[m] < bx + bw)
			&& (y[m] >= by && y[m] < by + bh))
		{
			/*printf("[%d](%d>=%d && %d<%d+%d) && (%d>=%d && %d<%d+%d)\n", 
			m, x[m], bx,x[m], bx,bw, y[m],by, y[m], by,bh);
			*/
			n++;
		}
	}
	if(n == 0)
	{
		//printf("n == 0\n");
		x[0] = bx;
		y[0] = by;
		x[1] = bx + bw - 1;
		y[1] = by;
		x[2] = bx;
		y[2] = by + bh - 1;
		x[3] = bx + bw - 1;
		y[3] = by + bh - 1;
		for (m = 0; m < 4; ++m)
		{
			if ((x[m] >= ax && x[m] < ax + aw)
				&& (y[m] >= ay && y[m] < ay + ah))
			{
				//printf("[%d](%d>=%d && %d<%d+%d) && (%d>=%d && %d<%d+%d)\n", m, x[m],bx,x[m],ax,aw,y[m],ay,y[m],ay,ah);
				n++;
			}
		}
	}
	//printf("1111\n");
	return n;
}


int
Rect_Is_Overlay (LCUI_Rect a, LCUI_Rect b)
/* ���ܣ�������������Ƿ��ص� */
{
	return Check_Rect_Overlap (a.x, a.y, a.width, a.height, b.x, b.y,
							   b.width, b.height);
}

int Rect_Equal (LCUI_Rect a, LCUI_Rect b)
/* ���ܣ��ж����������Ƿ���� */
{
	if(a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height) 
		return 1;
	return 0;
}

int Rect_Valid(LCUI_Rect r)
/* ���ܣ��жϾ����Ƿ���Ч */
{
	if(r.width <= 0 || r.height <= 0) 
		return 0;
	return 1;
}
