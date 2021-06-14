/* ***************************************************************************
 * LCUI_Graphics.c -- graphics handle
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
 * LCUI_Graphics.c -- ͼ�δ���
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
#include <png.h>
#include <jpeglib.h>
#include <math.h>
#include <LCUI_Build.h> 
#include LC_LCUI_H
#include LC_GRAPHICS_H
#include LC_MEM_H
#include LC_MISC_H 
#include LC_WIDGET_H
#include LC_CURSOR_H

/* ����ṹ�����ڴ洢bmp�ļ����ļ�ͷ����Ϣ */
typedef struct bmp_head {
	short int BMPsyg;
	short int nic[8];
	short int ix;
	short int nic2;
	short int iy;
	short int nic3[2];
	short int depth;
	short int rle;
	short int nic4[11];
} bmp_head;


struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer,1);
}

LCUI_RGB
RGB (unsigned char red, unsigned char green,
	 unsigned char blue)

/* ���ܣ���������ɫֵת����LCUI_RGB������ */
{
	LCUI_RGB color;

	color.red = red;
	color.green = green;
	color.blue = blue;
	return color;
}

int Get_Graph_Type(LCUI_Graph *pic)
/* ����ͼ�ε����� */
{
	return pic->type;
}

LCUI_Size Get_Graph_Size(LCUI_Graph *pic)
/* ���ܣ���ȡͼ�εĳߴ� */
{
	if(NULL == pic)
		return Size(0,0);
	return Size(pic->width, pic->height);
}

int Graph_Is_PNG(LCUI_Graph *pic)
/* 
 * ���ܣ��ж�ͼ���Ƿ�Ϊpng
 * ����ֵ�����򷵻�1�����򷵻�0
 * */
{
	if( Get_Graph_Type(pic) == TYPE_PNG) return 1;
	else return 0;
}

int Graph_Is_BMP(LCUI_Graph *pic)
/* 
 * ���ܣ��ж�ͼ���Ƿ�Ϊbmp
 * ����ֵ�����򷵻�1�����򷵻�0
 * */
{
	if( Get_Graph_Type(pic) == TYPE_BMP) return 1;
	else return 0;
}

int Graph_Is_JPG(LCUI_Graph *pic)
/* 
 * ���ܣ��ж�ͼ���Ƿ�Ϊjpg 
 * ����ֵ�����򷵻�1�����򷵻�0
 * */
{
	if( Get_Graph_Type(pic) == TYPE_JPG) return 1;
	else return 0;
}

int Graph_Have_Alpha(LCUI_Graph *pic)
/* 
 * ���ܣ��ж�ͼ���Ƿ���д�alphaͨ�� 
 * ����ֵ�����򷵻�1�����򷵻�0
 * */
{
	if( pic->flag == HAVE_ALPHA) return 1;
	else return 0;
}

int Valid_Graph(LCUI_Graph *pic)
/*
 * ���ܣ����ͼ�������Ƿ���Ч
 * ����ֵ����Ч����1����Ч����0
 */
{
	if(NULL != pic && pic->malloc == IS_TRUE 
	&& pic->width > 0 && pic->height > 0)
		return 1; 
	return 0;
}

int Valid_Bitmap(LCUI_Bitmap *bitmap)
/*
 * ���ܣ����λͼ�����Ƿ���Ч
 * ����ֵ����Ч����1����Ч����0
 */
{
	if(NULL != bitmap && bitmap->malloc == IS_TRUE 
		&& bitmap->width * bitmap->height > 0)
		return 1; 
	return 0;
}

void Print_Bitmap_Info(LCUI_Bitmap *bitmap)
/* ���ܣ���ӡλͼ����Ϣ */
{
	printf("address:%p\n",bitmap);
	if(bitmap != NULL) 
		printf("width:%d,height:%d,alpha:%u,malloc = %d(%s)\n", 
		bitmap->width, bitmap->height, bitmap->alpha, bitmap->malloc, 
		bitmap->malloc == IS_TRUE?"is true":"is false");
}

void Print_Graph_Info(LCUI_Graph *pic)
/* ���ܣ���ӡͼ�����Ϣ */
{
	printf("address:%p\n",pic);
	if(pic == NULL) 
		return;
	printf("width:%d,height:%d,alpha:%u,flag = %d(%s),malloc = %d(%s)\n", 
	pic->width, pic->height, pic->alpha, pic->flag, 
	pic->flag == HAVE_ALPHA ? "have alpha":"no alpha",
	pic->malloc, pic->malloc == IS_TRUE?"is true":"is false");
}

void Bitmap_Init(LCUI_Bitmap *in)
/* ���ܣ���ʼ��LCUI_Bitmap�ṹ������ */
{
	in->data = NULL;
	in->alpha = 255;
	in->width = 0;
	in->height = 0;
	in->malloc = IS_FALSE;
}

void Graph_Init(LCUI_Graph *pic)
/* ��ʼ��ͼƬ���ݽṹ�� */
{
	if(pic == NULL) return;
	pic->malloc = IS_FALSE;
	pic->flag   = NO_ALPHA;
	pic->rgba   = NULL;
	pic->alpha  = 255;
	pic->width  = 0;
	pic->height = 0;
	pic->type   = DEFAULT;
	pic->bit_depth = 8;    /* λ�� */
	thread_rwlock_init(&pic->lock);
}

static int detect_jpg(char *filepath, LCUI_Graph *out)
/* ���ܣ�����jpgͼƬ */
{
	FILE *fp;
	fp = fopen(filepath,"r");
	if(fp == NULL) return -1;
	
	int x,y;
	short int JPsyg;
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPARRAY buffer;
	int row_stride,jaka;

	if(fread(&JPsyg,sizeof (short int),1,fp))
	{
		if (JPsyg != -9985)   /* �������jpgͼƬ */
			return 1;
	}
	rewind(fp);
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		return 2;
	}
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo,fp);
	(void) jpeg_read_header(&cinfo,IS_TRUE);
	(void) jpeg_start_decompress(&cinfo);    

	jaka = cinfo.num_components;
	
	//if (jaka==3) printf("color\n"); else printf("grayscale\n");
	out->flag = NO_ALPHA;
	Malloc_Graph(out,cinfo.output_width,cinfo.output_height);
	if(!out->rgba || !out->rgba[0] || !out->rgba[1] || !out->rgba[2]){
		printf("����(jpg):�޷������㹻���ڴ湩�洢����!\n");
		return 1;
	}
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo,JPOOL_IMAGE,row_stride,1);
	y=0;
	while (cinfo.output_scanline <cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo,buffer,1);
		if (jaka==3) {
			for (x=0;x<out->width;x++)
			{
				out->rgba[0][x+y*out->width]=buffer[0][x*3];
				out->rgba[1][x+y*out->width]=buffer[0][x*3+1];
				out->rgba[2][x+y*out->width]=buffer[0][x*3+2];
			} 
		}
		else {
			for (x=0;x<out->width;x++)
			{
				out->rgba[0][x+y*out->width]=buffer[0][x];
				out->rgba[1][x+y*out->width]=out->rgba[0][x+y*out->width];
				out->rgba[2][x+y*out->width]=out->rgba[0][x+y*out->width];
			} 
		}
		y++;
	}
	out->type = TYPE_JPG;//ͼƬ����Ϊjpg
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(fp);
	return 0;	
}

static int detect_bmp(char *filepath, LCUI_Graph *out)
/* ���ܣ�����bmpͼƬ */
{
	FILE *fp;
	fp = fopen(filepath,"r");
	if(fp == NULL) return -1;
	bmp_head bmp;    
	int x,y, temp, tempi, rle,pocz, omin;
	unsigned char rozp;
	unsigned char **bak_rgba;    /* ���ݵ�ͼƬ���� */
	
	temp = fread(&bmp, 1, sizeof(bmp_head),fp);
	if (temp < sizeof(bmp_head) || bmp.BMPsyg != 19778)/* �������bmpͼƬ */ 
		return 1; 
	pocz = bmp.nic[4];
	out->bit_depth = bmp.depth;
	rle = bmp.rle;
	if ((out->bit_depth != 32) && (out->bit_depth != 24) 
		&& (out->bit_depth != 8) && (out->bit_depth !=4))
	{ 
		printf("����(bmp):λ�� %i ��֧��!\n",out->bit_depth);
		return 2;
	}
	out->flag = NO_ALPHA;     /* û��͸��Ч�� */
	temp = Malloc_Graph(out, bmp.ix, bmp.iy);
	if(temp != 0)
	{
		printf("����(bmp):�޷������㹻���ڴ湩�洢����!\n");
		return 1;
	}
	
	int size;
	size = out->width * out->height * sizeof(unsigned char);
	bak_rgba = (unsigned char**)malloc(3 * sizeof(unsigned char*));
	bak_rgba[0] = (unsigned char*)malloc(size);
	bak_rgba[1] = (unsigned char*)malloc(size);
	bak_rgba[2] = (unsigned char*)malloc(size);
	
	if(!bak_rgba || !bak_rgba[0] || !bak_rgba[1] || !bak_rgba[2]){
		printf("����(bmp):�޷������㹻���ڴ湩�洢����!\n");
		return 1;
	}
	rozp = ftell(fp);
	fseek(fp,0,SEEK_END);
	omin = ftell(fp);
	omin = omin-pocz;
	omin = omin-((out->width*out->height)*(out->bit_depth/8));
	omin = omin/(out->height);
	fseek(fp,pocz,SEEK_SET);
	if (out->bit_depth==32) /* out->bit_depth=32 */
	{
		for (y=0;y<out->height;y++)
		{
			for (x=0;x<out->width;x++)
			{
				out->rgba[2][x+(out->height-y-1)*out->width]=fgetc(fp);
				out->rgba[1][x+(out->height-y-1)*out->width]=fgetc(fp);
				out->rgba[0][x+(out->height-y-1)*out->width]=fgetc(fp);
				tempi=fgetc(fp);
			}
			if (omin>0) 
			for (tempi=0;tempi<omin;tempi++) fgetc(fp);
		}
	}  /* end out->bit_depth=32 */

	if (out->bit_depth==24) /* out->bit_depth=24 */
	{
		for (y=0;y<out->height;y++)
		{
			for (x=0;x<out->width;x++)
			{
				out->rgba[2][x+(out->height-y-1)*out->width]=fgetc(fp);
				out->rgba[1][x+(out->height-y-1)*out->width]=fgetc(fp);
				out->rgba[0][x+(out->height-y-1)*out->width]=fgetc(fp);
			}
			if (omin>0) 
			for (tempi=0;tempi<omin;tempi++) fgetc(fp);
			
		}
	}  /* end out->bit_depth=24 */

	if (out->bit_depth==8) /* out->bit_depth=8 */
	{
		fseek(fp,-256*4,SEEK_CUR);
		for (tempi=0;tempi<256;tempi++)
		{
			bak_rgba[2][tempi]=fgetc(fp);	
			bak_rgba[1][tempi]=fgetc(fp);	
			bak_rgba[0][tempi]=fgetc(fp);	
			x=fgetc(fp);
		}
		if (rle==0) {   /* nonRLE */	
			for (y=0;y<out->height;y++)
			{
				for (x=0;x<out->width;x++)
				{
					if ((tempi=fgetc(fp))==-1) return 0;
					out->rgba[0][x+(out->height-y-1)*out->width]=bak_rgba[0][tempi];
					out->rgba[1][x+(out->height-y-1)*out->width]=bak_rgba[1][tempi];
					out->rgba[2][x+(out->height-y-1)*out->width]=bak_rgba[2][tempi];
				}
				if (omin>0)  		 
				for (tempi=0;tempi<omin;tempi++) fgetc(fp);
			}
		}   /* end nonRLE */
		else { /* RLE */
			for (y=0;y<out->height;y++)
			{
				for (x=0;x<out->width;x++)
				{
					if ((out->bit_depth=fgetc(fp))==-1) return 0;
					tempi=fgetc(fp);
					if (out->bit_depth==0) {
						for (rle=0;rle<tempi;rle++){
							out->bit_depth=fgetc(fp);
							out->rgba[0][(x+rle)+(out->height-y-1)*out->width]=bak_rgba[0][out->bit_depth];
							out->rgba[1][(x+rle)+(out->height-y-1)*out->width]=bak_rgba[1][out->bit_depth];
							out->rgba[2][(x+rle)+(out->height-y-1)*out->width]=bak_rgba[2][out->bit_depth];
						}
						if (tempi!=((tempi/2)*2)) tempi=fgetc(fp);
					} else {
						for (rle=0;rle<out->bit_depth;rle++){
							out->rgba[0][(x+rle)+(out->height-y-1)*out->width]=bak_rgba[0][tempi];
							out->rgba[1][(x+rle)+(out->height-y-1)*out->width]=bak_rgba[1][tempi];
							out->rgba[2][(x+rle)+(out->height-y-1)*out->width]=bak_rgba[2][tempi];
						}	
					}
					x+=rle-1;
				}	
			}
		} /* end RLE  */
		out->bit_depth=8;
	} /* end out->bit_depth=8 */

	if (out->bit_depth==4) /* out->bit_depth=4 */
	{
		fseek(fp,-16*4,SEEK_CUR);
		for (tempi=0;tempi<16;tempi++)
		{
			bak_rgba[2][tempi]=fgetc(fp);	
			bak_rgba[1][tempi]=fgetc(fp);	
			bak_rgba[0][tempi]=fgetc(fp);	
			x=fgetc(fp);
		}
		if (rle==0) {  /* nonRLE */	
			for (y=0;y<out->height;y++)
			{
				for (x=0;x<out->width;x++)
				{
					tempi=fgetc(fp);
					rozp=tempi;
					rozp=(rozp & 0xf0)>>4;
					out->rgba[0][x+(out->height-y-1)*out->width]=bak_rgba[0][rozp];
					out->rgba[1][x+(out->height-y-1)*out->width]=bak_rgba[1][rozp];
					out->rgba[2][x+(out->height-y-1)*out->width]=bak_rgba[2][rozp];
					x++;
					rozp=tempi;
					rozp=(rozp & 0x0f);
					out->rgba[0][x+(out->height-y-1)*out->width]=bak_rgba[0][rozp];
					out->rgba[1][x+(out->height-y-1)*out->width]=bak_rgba[1][rozp];
					out->rgba[2][x+(out->height-y-1)*out->width]=bak_rgba[2][rozp];
				}
			}
		}    /* end nonRLE */
		else { /* RLE */
			
		} /* end RLE */
		out->bit_depth=4;
	}    /* end out->bit_depth=4*/
	out->type = TYPE_BMP;     /* ͼƬ����Ϊbmp */
	free(bak_rgba[0]);
	free(bak_rgba[1]);
	free(bak_rgba[2]);
	free(bak_rgba);
	fclose(fp);
	return 0;	
}

int write_png_file(char *file_name , LCUI_Graph *graph)
/* ���ܣ���LCUI_Graph�ṹ�е�����д����png�ļ� */
{
	int j, i, temp, pos;
	png_byte color_type; 

	png_structp png_ptr;
	png_infop info_ptr; 
	png_bytep * row_pointers;
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
	{
		printf("[write_png_file] File %s could not be opened for writing", file_name);
		return -1;
	}
	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
	{
		printf("[write_png_file] png_create_write_struct failed");
		return -1;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		printf("[write_png_file] png_create_info_struct failed");
		return -1;
	}
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during init_io");
		return -1;
	}
	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during writing header");
		return -1;
	}
	Using_Graph(graph, 0);
	if(graph->flag == HAVE_ALPHA) color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	else color_type = PNG_COLOR_TYPE_RGB;
	
	png_set_IHDR(png_ptr, info_ptr, graph->width, graph->height,
		graph->bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		
	png_write_info(png_ptr, info_ptr);

	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during writing bytes");
		End_Use_Graph(graph);
		return -1;
	}
	if(graph->flag == HAVE_ALPHA) temp = (4 * graph->width);
	else temp = (3 * graph->width);
	
	row_pointers = (png_bytep*)malloc(graph->height*sizeof(png_bytep));
	for(i=0,pos=0; i < graph->height; i++)
	{
		row_pointers[i] = (png_bytep)malloc(sizeof(unsigned char)*temp);
		for(j=0; j < temp; ++pos)
		{
			row_pointers[i][j++] = graph->rgba[0][pos]; // red
			row_pointers[i][j++] = graph->rgba[1][pos]; // green
			row_pointers[i][j++] = graph->rgba[2][pos];   // blue
			if(graph->flag == HAVE_ALPHA) 
				row_pointers[i][j++] = graph->rgba[3][pos]; // alpha 
		}
	}
	png_write_image(png_ptr, row_pointers);

	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during end of write");
		End_Use_Graph(graph);
		return -1;
	}
	png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
	for (j=0; j<graph->height; j++)
		free(row_pointers[j]);
	free(row_pointers);

    fclose(fp);
	End_Use_Graph(graph);
    return 0;
}

int detect_png(char *filepath, LCUI_Graph *out)
/* ���ڽ���pngͼƬ */
{
	FILE *pic_fp;
	pic_fp = fopen(filepath, "rb");
	if(pic_fp == NULL) /* �ļ���ʧ�� */
		return -1;
	
	/* ��ʼ�����ֽṹ */
	png_structp png_ptr;
	png_infop   info_ptr;
	char        buf[PNG_BYTES_TO_CHECK];
	int         temp;
	
	png_ptr  = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info_ptr = png_create_info_struct(png_ptr);
	
	setjmp(png_jmpbuf(png_ptr)); // ������Ҫ
	
	temp = fread(buf,1,PNG_BYTES_TO_CHECK,pic_fp);
	temp = png_sig_cmp((void*)buf,(png_size_t)0,PNG_BYTES_TO_CHECK);
	
	/*����Ƿ�Ϊpng�ļ�*/
	if (temp!=0) return 1;
	
	rewind(pic_fp);
	/*��ʼ���ļ�*/
	png_init_io(png_ptr, pic_fp);/*pic_fp��ȫ�ֱ���*/
	// ���ļ���
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	int color_type,channels;
	
	/*��ȡ��ȣ��߶ȣ�λ���ɫ����*/
	channels       = png_get_channels(png_ptr, info_ptr);/*��ȡͨ����*/
	out->bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type     = png_get_color_type(png_ptr, info_ptr);/*��ɫ����*/
	
	int pos = 0;
	/* row_pointers��߾���rgba���� */
	png_bytep* row_pointers;
	row_pointers = png_get_rows(png_ptr, info_ptr);
	int i,j;
	if(channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{/*�����RGB+alphaͨ��������RGB+�����ֽ�*/
		//��ʼ�����ڴ�
		out->flag = HAVE_ALPHA;
		temp = Malloc_Graph(out, png_get_image_width(png_ptr, info_ptr), 
							png_get_image_height(png_ptr, info_ptr));
		if(temp != 0)
		{/* ��������ڴ�ʧ�� */
			fclose(pic_fp);
			printf("����(png):�޷������㹻���ڴ湩�洢����!\n");
			return 1;
		}
		temp = (4 * out->width);
		for(i = 0; i < out->height; i++)
		{
			for(j = 0; j < temp; j += 4)
			{
				out->rgba[0][pos] = row_pointers[i][j]; // red
				out->rgba[1][pos] = row_pointers[i][j+1]; // green
				out->rgba[2][pos] = row_pointers[i][j+2];   // blue
				out->rgba[3][pos] = row_pointers[i][j+3]; // alpha
				++pos;
			}
		}
	}
	else if(channels == 3 || color_type == PNG_COLOR_TYPE_RGB)
	{/*�����RGBͨ��*/
		out->flag = NO_ALPHA;
		temp = Malloc_Graph(out, png_get_image_width(png_ptr, info_ptr), 
							png_get_image_height(png_ptr, info_ptr));
		if(temp != 0)
		{
			fclose(pic_fp);
			printf("����(png):�޷������㹻���ڴ湩�洢����!\n");
			return 1;
		}
		temp = (3 * out->width);
		for(i = 0; i < out->height; i++)
		{
			for(j = 0; j < temp; j += 3)
			{
				out->rgba[0][pos] = row_pointers[i][j]; // red
				out->rgba[1][pos] = row_pointers[i][j+1]; // green
				out->rgba[2][pos] = row_pointers[i][j+2];   // blue
				++pos;
			}
		}
	}
	else return 1;
	out->type = TYPE_PNG;  /* ͼƬ����Ϊpng */
	/* ��������ռ�õ��ڴ� */
	png_destroy_read_struct(&png_ptr, &info_ptr, 0); 
	return 0;
}

int detect_image(char *filepath, LCUI_Graph *out)
/*��������ڼ��ͼƬ��ʽ��������*/
{
	int result = 1;
	if (result == 1) result = detect_png(filepath, out);/*���png*/  
	if (result == 1) result = detect_jpg(filepath, out);/*���jpg*/ 
	if (result == 1) result = detect_bmp(filepath, out);/*���bmp*/ 
	return result;
}


void Zoom_Graph(LCUI_Graph *in, LCUI_Graph *out, int flag, LCUI_Size size)
/* ���ܣ�����ָ��ģʽ����ͼ��������� */
{
	LCUI_Pos pos; 
	int count, temp, x, y, k, m;
	double scale_x,scale_y;
	
	if(!Valid_Graph(in)) 
		return; 
	
	if(size.w <=0 || size.h <= 0)
	{ 
		Free_Graph(out);
		return;
	}
	 
	scale_x = (double)in->width / size.w;
	scale_y = (double)in->height / size.h;
	
	if(flag == DEFAULT)
	{/* ������ŷ�ʽΪȱʡ��ͼƬ�Ŀ�͸ߵ����ű�������һ�� */
		if (scale_x<scale_y) scale_y = scale_x; 
		else scale_x = scale_y;
	}
	out->flag = in->flag;
	Malloc_Graph(out, size.w, size.h);/* �����ڴ� */ 
	Using_Graph(out, 1);
	Using_Graph(in, 0);  
	 
	for (y=0; y < size.h; ++y) 
	{
		pos.y = y*scale_y;
		k = y*size.w;
		m = pos.y*in->width;
		for (x = 0; x < size.w; ++x) 
		{
			pos.x = x*scale_x; 
			temp  = k + x;
			count = m + pos.x;
			out->rgba[0][temp] = in->rgba[0][count];
			out->rgba[1][temp] = in->rgba[1][count];
			out->rgba[2][temp] = in->rgba[2][count];
			if(in->flag == HAVE_ALPHA)
				out->rgba[3][temp] = in->rgba[3][count];
		}
	} 
	End_Use_Graph(out);
	End_Use_Graph(in);
	out->width  = size.w;
	out->height = size.h;
	out->malloc = IS_TRUE;  /* �Ѿ������ڴ� */
}


int Load_Image(char *filepath, LCUI_Graph *out)
/* 
 * ���ܣ�����ָ��ͼƬ�ļ���ͼ������
 * ˵������ͼƬ�ļ������������ڴ棬�򿪵�ͼƬ�ļ�Խ��ռ�õ��ڴ�Ҳ��Խ�� 
 * */
{
	FILE *fp;
	Graph_Init(out); 
	int result = 0;   /* �������Ϊ0 */
	out->flag = NO_ALPHA;
	/*fp��ȫ�ֱ����������������õ���*/
	if ((fp = fopen(filepath,"r")) == NULL) 
	{
		perror(filepath);
		result = OPEN_ERROR; 
	}
	else{
		fgetc(fp);
		if (!ferror (fp)) 
		{/*r���û����*/
			fseek(fp,0,SEEK_END);
			if (ftell(fp)>4) 
			{
				fclose(fp);
				result = detect_image(filepath, out);/*���ͼƬ������*/
			} 
			else 
			{
				result = SHORT_FILE;//�ļ���С 
				fclose(fp);
			}
		}
	}
	return result;   /* ���ش������ */
}

int Cut_Graph(LCUI_Graph *src, LCUI_Rect rect, LCUI_Graph *out)
/* 
 * ���ܣ���Դͼ���н�ȡ��ָ�������ͼ�� 
 * ˵�����ü�����ͼ�����ݣ�ȫ��͸���Ⱥ�Դͼ��һ��
 * */
{
	int x, y;
	int temp,count = 0, k; 
	out->flag = src->flag;
	out->alpha = src->alpha;
	rect = Get_Valid_Area(Size(src->width, src->height), rect);
	/***
	if(rect.x < 0) 
	{
		rect.width = rect.width + rect.x;
		rect.x = 0;
	}
	if(rect.y < 0) 
	{
		rect.height = rect.height + rect.y;
		rect.y = 0;
	}
	if(rect.width + rect.x > src->width) 
	{
		rect.width = src->width - rect.x;
	}
	if(rect.height + rect.y > src->height) 
	{
		rect.height = src->height - rect.y;
	}
	* **/
	if(Valid_Graph(src) && rect.width * rect.height > 0)
	{ 
		temp = Malloc_Graph(out, rect.width, rect.height);
		if(temp != 0) return -1; 
		Using_Graph(out, 1);
		Using_Graph(src, 0);
		/* ��ʼ��ȡͼƬ�е�ͼ�����鲢д�봰�� */ 
		for (y=0;y< rect.height; ++y) 
		{
			k = (rect.y+y)*src->width + rect.x;
			for (x=0;x<rect.width; ++x)
			{
				temp = k + x; /* ����ͼƬ����Ҫ��ȡ������ĸ������� */
				out->rgba[0][count] = src->rgba[0][temp];
				out->rgba[1][count] = src->rgba[1][temp];
				out->rgba[2][count] = src->rgba[2][temp];
				if(src->flag == HAVE_ALPHA)
					out->rgba[3][count] = src->rgba[3][temp];
				++count;
			}
		} 
		End_Use_Graph(out);
		End_Use_Graph(src);
		return 0;
	}
	return -1;
}

int Graph_Flip_Horizontal(LCUI_Graph *src, LCUI_Graph *out)  
/* ���ܣ���ͼ�����ˮƽ��ת */  
{  
    int value = 0, x, y, pos, left, temp, count;  
    int width = src->width, height = src->height;  
    unsigned char buff;  
    
    if(!Valid_Graph(src)) value = -1;
    else
    {
		if(src->flag == HAVE_ALPHA) out->flag = HAVE_ALPHA; 
		Malloc_Graph(out, width, height);
        temp = width / 2;
		for (y = 0; y < height; ++y) 
		{   /* ˮƽ��ת��ʵҲ���ǽ������ߵ����� */  
			pos = y * width;
			left = pos;
			for (x = 0; x <= temp; ++x) 
			{
				count = left + width - x - 1;
				
				buff = src->rgba[0][pos]; 
                out->rgba[0][pos] = src->rgba[0][count];  
                out->rgba[0][count] = buff;
                 
				buff = src->rgba[1][pos]; 
                out->rgba[1][pos] = src->rgba[1][count];  
                out->rgba[1][count] = buff;
                
				buff = src->rgba[2][pos]; 
                out->rgba[2][pos] = src->rgba[2][count];  
                out->rgba[2][count] = buff;
                
                if(src->flag == HAVE_ALPHA)
                {
					buff = src->rgba[3][pos]; 
					out->rgba[3][pos] = src->rgba[3][count];  
					out->rgba[3][count] = buff;
				}
				++pos;
			}
        }
    }
    return value;  
} 

int Cut_And_Overlay_Graph(LCUI_Graph *src, LCUI_Rect cut, LCUI_Pos put, LCUI_Graph *des)
/*
 * ���ܣ���Դͼ���н�ȡ��һ��ͼ�β�����alphaͨ������������ͼ��
 * ˵������������ǽ�ȡ�͸���һ����еģ��ڽ�ȡ��ͬʱ����ͼ�θ�����Ŀ��ͼ���ϣ�Ч�ʱȷֲ����ú�����һ�㡣
 */
{
	int x, y;
	unsigned char alpha;
	int temp,count, k, n;  
	/* ����ü����� */
	cut = Get_Valid_Area(Size(src->width, src->height), cut);
	/**
	if(cut.x < 0) 
	{
		cut.width = cut.width + cut.x;
		cut.x = 0;
	}
	if(cut.y < 0) 
	{
		cut.height = cut.height + cut.y;
		cut.y = 0;
	}
	if(cut.width + cut.x > src->width)  
		cut.width = src->width - cut.x; 
	if(cut.height + cut.y > src->height)  
		cut.height = src->height - cut.y; 
	**/
	/* ����ճ������ճ���������ڱ���ͼ����Ч��Χ�ڵĻ���ҲҪ�����ü����� */
	if(put.x < 0) 
	{
		cut.x -= put.x;
		cut.width = cut.width + put.x;
		put.x = 0;
	}
	if(put.y < 0) 
	{
		cut.y -= put.y;
		cut.height = cut.height + put.y;
		put.y = 0;
	}
	if(cut.width + put.x > des->width)  
		cut.width = des->width - put.x; 
	if(cut.height + put.y > des->height)  
		cut.height = des->height - put.y;
		
	if(Valid_Graph(des) && Valid_Graph(src) && cut.width * cut.height > 0)
	{
		Using_Graph(des, 1);
		Using_Graph(src, 0);
		/* ��ʼ��ȡͼƬ�е�ͼ�����鲢д��Ŀ��ͼ�� */ 
		for (y=0;y<cut.height; ++y) 
		{
			k = (cut.y+y)*src->width + cut.x; /* ��ȡ��ȡ�������λ�� */
			n = (y+put.y)*des->width + put.x; /* ��ȡճ���������λ�� */
			for (x=0;x<cut.width; ++x)
			{
				temp  = k + x; /* ����ͼƬ����Ҫ��ȡ������ĸ������� */
				count = n + x; 
				if(src->flag == HAVE_ALPHA)
				{
					alpha = src->rgba[3][temp];
					switch(alpha)
					{
						case 255:
						des->rgba[0][count] = src->rgba[0][temp];
						des->rgba[1][count] = src->rgba[1][temp];
						des->rgba[2][count] = src->rgba[2][temp];
						break;
						case 0:break;
						default:
						des->rgba[0][count] = (src->rgba[0][temp] * alpha + des->rgba[0][count] * (255 - alpha)) /255;
						des->rgba[1][count] = (src->rgba[1][temp] * alpha + des->rgba[1][count] * (255 - alpha)) /255;
						des->rgba[2][count] = (src->rgba[2][temp] * alpha + des->rgba[2][count] * (255 - alpha)) /255;
						break;
					}
				}
				else
				{
					des->rgba[0][count] = src->rgba[0][temp];
					des->rgba[1][count] = src->rgba[1][temp];
					des->rgba[2][count] = src->rgba[2][temp];
					if(des->flag == HAVE_ALPHA)
						des->rgba[3][count] = 255;
				}  
			}
		} 
		End_Use_Graph(des);
		End_Use_Graph(src);
		return 0;
	}
	return -1;
}

int Cut_And_Replace_Graph(LCUI_Graph *src, LCUI_Rect cut, LCUI_Pos put, LCUI_Graph *des)
/*
 * ���ܣ���Դͼ���н�ȡ��һ��ͼ�β�����������ͼ��
 * ˵����������������ͬ������ǽ�ȡ�ͺϳ�һ����еģ��ڽ�ȡ��ͬʱ����ͼ�θ�����Ŀ��ͼ���ϣ�Ч�ʱȷֲ����ú�����һ�㡣
 */
{
	int x, y; 
	int temp,count, k, n;  
	/* ����ü����� */
	cut = Get_Valid_Area(Size(src->width, src->height), cut);
	/* ����ճ������ */
	if(put.x < 0) 
	{
		cut.x -= put.x;
		cut.width = cut.width + put.x;
		put.x = 0;
	}
	if(put.y < 0) 
	{
		cut.y -= put.y;
		cut.height = cut.height + put.y;
		put.y = 0;
	}
	if(cut.width + put.x > des->width)  
		cut.width = des->width - put.x; 
	if(cut.height + put.y > des->height)  
		cut.height = des->height - put.y; 
		
	if(Valid_Graph(des) && Valid_Graph(src) && cut.width * cut.height > 0)
	{
		Using_Graph(des, 1);
		Using_Graph(src, 0);
		/* ��ʼ��ȡͼƬ�е�ͼ�����鲢д��Ŀ��ͼ�� */ 
		for (y=0;y<cut.height; ++y) 
		{
			k = (cut.y+y)*src->width + cut.x; /* ��ȡ��ȡ�������λ�� */
			n = (y+put.y)*des->width + put.x; /* ��ȡճ���������λ�� */
			for (x=0;x<cut.width; ++x)
			{
				temp  = k + x; /* ����ͼƬ����Ҫ��ȡ������ĸ������� */
				count = n + x;
				des->rgba[0][count] = src->rgba[0][temp];
				des->rgba[1][count] = src->rgba[1][temp];
				des->rgba[2][count] = src->rgba[2][temp];
				if(des->flag == HAVE_ALPHA)
				{
					if(src->flag == HAVE_ALPHA)
						des->rgba[3][count] = src->rgba[3][temp]; 
					else
						des->rgba[3][count] = 255;
				}
			}
		} 
		End_Use_Graph(des);
		End_Use_Graph(src);
		return 0;
	}
	return -1;
}


int Get_Screen_Graph(LCUI_Graph *out)
/* 
 * ���ܣ���ȡ��Ļ����ʾ��ͼ��
 * ˵�����Զ������ڴ��ָ�룬�������ݿ�����ָ����ڴ� 
 * */
{
	unsigned char  *dest;
	int i, temp, h, w;
	if(LCUI_Sys.init != IS_TRUE) /* ���û�г�ʼ���� */
		return -1; 
	
	out->flag = NO_ALPHA;/* ��alphaͨ�� */
	out->type = TYPE_BMP;
	temp = Malloc_Graph(out, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h);
	if(temp != 0) 
		return -2; 
		
	i = 0; 
	dest = LCUI_Sys.screen.fb_mem; /* ָ��ָ��֡������ڴ� */
	for (h = 0; h < LCUI_Sys.screen.size.h; ++h)
	{
		for (w = 0; w < LCUI_Sys.screen.size.w; ++w)
		{
			/* ��ȡ֡��������� */
			out->rgba[2][i] = *(dest++);
			out->rgba[1][i] = *(dest++);
			out->rgba[0][i] = *(dest++);
			dest++;
			++i; 
		}
	}
	return 0;
}

void Fill_Color(LCUI_Graph *pic, LCUI_RGB color)
/* ���ܣ�Ϊ�����ͼ�������ɫ */
{
	int size; 
	if(Valid_Graph(pic))
	{
		Using_Graph(pic, 1); 
		size = sizeof(unsigned char) * pic->height * pic->width;
		memset(pic->rgba[0], color.red,		size);
		memset(pic->rgba[1], color.green,	size);
		memset(pic->rgba[2], color.blue,	size);
		End_Use_Graph(pic);
	}
}


void Tile_Graph(LCUI_Graph *src, LCUI_Graph *out, int width, int height)
/* 
 * ���ܣ�ƽ��ͼ��
 * ����˵����
 * src ԭʼͼ��
 * out �����ͼ��
 * width ���Ŀ��
 * height ���ĸ߶�
 **/
{
	int x,y,w,h,temp,count, m;
	temp = 0;
	if(Valid_Graph(src) && out != NULL && width * height > 0)
	{
		Malloc_Graph(out, width, height);
		Using_Graph(out, 1);
		Using_Graph(src, 0);
		if(src->flag == HAVE_ALPHA)
		{
			for(y = 0;y < height;++y)
			{
				h = y % src->height;
				m = h * src->width;
				for(x = 0;x < width;++x,++temp)
				{
					w = x % src->width;
					count = m + w;
					out->rgba[0][temp] = (src->rgba[0][count] * src->rgba[3][count] + out->rgba[0][temp] * (255 - src->rgba[3][count])) /255;
					out->rgba[1][temp] = (src->rgba[1][count] * src->rgba[3][count] + out->rgba[1][temp] * (255 - src->rgba[3][count])) /255;
					out->rgba[2][temp] = (src->rgba[2][count] * src->rgba[3][count] + out->rgba[2][temp] * (255 - src->rgba[3][count])) /255;
					
				}
			}
		}
		else 
		{
			for(y = 0;y < height;++y)
			{
				h = y%src->height;
				m = h * src->width;
				for(x = 0;x < width;++x)
				{
					w = x % src->width;
					count = m + w;
					out->rgba[0][temp] = src->rgba[0][count];
					out->rgba[1][temp] = src->rgba[1][count];
					out->rgba[2][temp] = src->rgba[2][count];
					out->rgba[3][temp] = 255;
					++temp;
				}
			}
		}
		End_Use_Graph(out);
		End_Use_Graph(src);
	}
}

int Mix_Graph(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos)
/* 
 * ���ܣ���ǰ��ͼ�뱳��ͼ��ϵ���
 * ˵����back_graph�Ǳ���ͼ��fore_graphǰ��ͼ�񣬻�Ϻ�����ݱ����ڱ���ͼ��
 */
{
	unsigned char *r1, *g1, *a1, *b1, *r2, *g2, *b2; 
	
	if(des_pos.x > back_graph->width) return -1;
	if(des_pos.y > back_graph->height) return -1;

	if(Valid_Graph(back_graph) && Valid_Graph(fore_graph))
	{/* �������ͼ�����ݶ���Ч */
		int x = 0, y = 0;
		int temp,count; 
		int m, n;
		unsigned char j;//, alpha; 
		float k;
		LCUI_Rect cut;
		
		/* ���ǰ��ͼ�ߴ糬������ͼ�ķ�Χ����Ҫ����ǰ��ͼ�����ض�ȡ��Χ */ 
		if( Get_Cut_Area(
			Size( back_graph->width, back_graph->height),
			Rect( des_pos.x, des_pos.y, 
				fore_graph->width, fore_graph->height ),
			&cut
		))
		{
			des_pos.x += cut.x;
			des_pos.y += cut.y;
		} 
		/* ��ʼ��ȡͼƬ�е�ͼ�����鲢д�봰�� */ 
		Using_Graph(fore_graph, 0);
		Using_Graph(back_graph, 1);
		if(fore_graph->flag == HAVE_ALPHA)
		{ /* ���ǰ��ͼ����͸��Ч�� */
			k = fore_graph->alpha / 255.0;
			for (y = 0; y < cut.height; ++y) 
			{
				m = (cut.y + y) * fore_graph->width + cut.x;
				n = (des_pos.y + y) * back_graph->width + des_pos.x;
				for (x = 0; x < cut.width; ++x) 
				{
					temp = m + x; /* ����ͼƬ����Ҫ��ȡ������ĸ������� */
					count = n + x;/* ��������������ڵĸ�������� */
					j = fore_graph->rgba[3][temp] * k;
					/* �˳����������϶࣬��֪����Ż� */
					back_graph->rgba[0][count] = (fore_graph->rgba[0][temp] * j + back_graph->rgba[0][count] * (255 - j)) /255;
					back_graph->rgba[1][count] = (fore_graph->rgba[1][temp] * j + back_graph->rgba[1][count] * (255 - j)) /255;
					back_graph->rgba[2][count] = (fore_graph->rgba[2][temp] * j + back_graph->rgba[2][count] * (255 - j)) /255;
				}
			}
		}
		else
		{/* ���ǰ��ͼ��û��͸��Ч�� */
			for (y = 0; y < cut.height; ++y) 
			{ 
				/* ����ǰ��ͼ����Ҫ��ȡ������ĸ�������� */
				m = (cut.y + y) * fore_graph->width + cut.x;
				/* ���㱳��ͼ����Ҫ��ȡ������ĸ�������� */
				n = (des_pos.y + y) * back_graph->width + des_pos.x;
				/* ʹ��ָ�������� */
				r1 = back_graph->rgba[0] + n;
				g1 = back_graph->rgba[1] + n;
				b1 = back_graph->rgba[2] + n;
				a1 = back_graph->rgba[3] + n;
				
				r2 = fore_graph->rgba[0] + m;
				g2 = fore_graph->rgba[1] + m;
				b2 = fore_graph->rgba[2] + m;
				/* ���� */
				memcpy(r1, r2, cut.width);
				memcpy(g1, g2, cut.width);
				memcpy(b1, b2, cut.width);
				if(back_graph->flag == HAVE_ALPHA)
					/* ͸����ȡ����ǰ��ͼ��͸���� */
					memset(a1, fore_graph->alpha, sizeof(unsigned char)*cut.width);
			}
		} 
		
		End_Use_Graph(back_graph);
		End_Use_Graph(fore_graph); 
		return 0;
	}
	return -1;
}


int Replace_Graph(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos)
/* 
 * ���ܣ��滻ԭͼ�ж�Ӧλ�õ������е�ͼ��
 * ˵������Overlay_Graph������ͬ��back_graph�е�ͼ�����ݻᱻfore_graph�е�ͼ�����ݸ���
 */
{
	unsigned char *r1, *g1, *a1, *a2, *b1, *r2, *g2, *b2; 
	if(des_pos.x > back_graph->width) return -1;
	if(des_pos.y > back_graph->height) return -1;

	if(Valid_Graph(back_graph) && Valid_Graph(fore_graph))
	{/* �������ͼ�����ݶ���Ч */
		int x = 0, y = 0;
		int temp,count; 
		int m, n;
		unsigned char j; 
		
		LCUI_Rect cut;
		
		/* ���ǰ��ͼ�ߴ糬������ͼ�ķ�Χ����Ҫ����ǰ��ͼ�����ض�ȡ��Χ */ 
		if( Get_Cut_Area(
			Size( back_graph->width, back_graph->height),
			Rect( des_pos.x, des_pos.y, 
				fore_graph->width, fore_graph->height ),
			&cut
		))
		{
			des_pos.x += cut.x;
			des_pos.y += cut.y;
		}
		/* ��ʼ��ȡͼƬ�е�ͼ�����鲢д�봰�� */
		Using_Graph(fore_graph, 0);
		Using_Graph(back_graph, 1);
		
		if(fore_graph->flag == HAVE_ALPHA && back_graph->flag == NO_ALPHA)
		{ /* ���ǰ��ͼ����͸��Ч��������ͼ��û�� */
			for (y = 0; y < cut.height; ++y) 
			{
				m = (cut.y + y) * fore_graph->width + cut.x;
				n = (des_pos.y + y) * back_graph->width + des_pos.x;
				for (x = 0; x < cut.width; ++x) 
				{
					temp = m + x; /* ����ͼƬ����Ҫ��ȡ������ĸ������� */
					count = n + x;/* ��������������ڵĸ�������� */
					j = fore_graph->rgba[3][temp] * fore_graph->alpha;
					back_graph->rgba[0][count] = (fore_graph->rgba[0][temp] * j + back_graph->rgba[0][count] * (255 - j)) /255;
					back_graph->rgba[1][count] = (fore_graph->rgba[1][temp] * j + back_graph->rgba[1][count] * (255 - j)) /255;
					back_graph->rgba[2][count] = (fore_graph->rgba[2][temp] * j + back_graph->rgba[2][count] * (255 - j)) /255;
				}
			}
		}
		else 
		{
			for (y = 0; y < cut.height; ++y) 
			{
				m = (cut.y + y) *fore_graph->width + cut.x;
				n = (des_pos.y + y) * back_graph->width + des_pos.x;
				/* ʹ��ָ�������� */
				r1 = back_graph->rgba[0] + n;
				g1 = back_graph->rgba[1] + n;
				b1 = back_graph->rgba[2] + n;
				
				r2 = fore_graph->rgba[0] + m;
				g2 = fore_graph->rgba[1] + m;
				b2 = fore_graph->rgba[2] + m; 
				/* ���� */
				memcpy(r1, r2, cut.width);
				memcpy(g1, g2, cut.width);
				memcpy(b1, b2, cut.width);
				if(back_graph->flag == HAVE_ALPHA)
				{
					a1 = back_graph->rgba[3] + n;
					if(fore_graph->flag == NO_ALPHA)
						memset(a1, fore_graph->alpha, cut.width);
					else if(fore_graph->flag == HAVE_ALPHA)
					{
						a2 = fore_graph->rgba[3] + m; 
						memcpy(a1, a2, cut.width);  
					}
				}
			}
		}
		End_Use_Graph(back_graph);
		End_Use_Graph(fore_graph);
		return 0;
	}
	return -1;
} 

int Align_Image(LCUI_Graph *graph, LCUI_Graph *image, int flag)
/* ���ܣ���ͼƬ��ָ�����뷽ʽճ��������ͼ�� */
{
	LCUI_Pos pos;
	pos.x = 0;
	pos.y = 0;
	if(!Valid_Graph(graph) || ! Valid_Graph(image))
	/* ���ͼƬ��Ч */
		return -1; 
		
	if((flag & ALIGN_TOP_LEFT) == ALIGN_TOP_LEFT);
	/* �����ϽǶ��� */
	else if((flag & ALIGN_TOP_CENTER) == ALIGN_TOP_CENTER)
	/* �����м���� */
		pos.x = (graph->width - image->width) / 2;  
	else if((flag & ALIGN_TOP_RIGHT) == ALIGN_TOP_RIGHT)
	/* �����ϽǶ��� */
		pos.x = graph->width - image->width; 
	else if((flag & ALIGN_MIDDLE_LEFT) == ALIGN_MIDDLE_LEFT)
	/* ������ƫ����� */ 
		pos.y = (graph->height - image->height) / 2; 
	else if((flag & ALIGN_MIDDLE_CENTER) == ALIGN_MIDDLE_CENTER)
	{ /* ����������� */
		pos.x = (graph->width - image->width) / 2;
		pos.y = (graph->height - image->height) / 2;
	}
	else if((flag & ALIGN_MIDDLE_RIGHT) == ALIGN_MIDDLE_RIGHT)
	{ /* ������ƫ�ɶ��� */
		pos.x = graph->width - image->width;
		pos.y = (graph->height - image->height) / 2;
	}
	else if((flag & ALIGN_BOTTOM_LEFT) == ALIGN_BOTTOM_LEFT)
	/* ��ײ�ƫ����� */ 
		pos.y = graph->height - image->height; 
	else if((flag & ALIGN_BOTTOM_CENTER) == ALIGN_BOTTOM_CENTER)
	{ /* ��ײ����ж��� */
		pos.x = (graph->width - image->width) / 2;
		pos.y = graph->height - image->height;
	}
	else if((flag & ALIGN_BOTTOM_RIGHT) == ALIGN_BOTTOM_RIGHT)
	{ /* ��ײ�ƫ�Ҷ��� */
		pos.x = graph->width - image->width;
		pos.y = graph->height - image->height;
	}
	
	if( Check_Option(flag, GRAPH_MIX_FLAG_OVERLAY) ) 
	/* �������GRAPH_MIX_FLAG_OVERLAYѡ�� */
		Mix_Graph(graph, image, pos); 
	else if( Check_Option(flag, GRAPH_MIX_FLAG_REPLACE) ) 
	/* �������GRAPH_MIX_FLAG_REPLACEѡ�� */
		Replace_Graph(graph, image, pos); 
	else Mix_Graph(graph, image, pos);
	
	return 0;
}


int Fill_Background_Image(LCUI_Graph *graph, LCUI_Graph *bg, int flag, LCUI_RGB color)
/* ���ܣ�Ϊָ��ͼ����䱳��ͼ�� */
{
	Fill_Color(graph, color); /* ��䱳��ɫ */
	if(Valid_Graph(bg) && graph != NULL)
	{
		LCUI_Pos pos;
		LCUI_Graph temp_bg;
		pos.x = 0;
		pos.y = 0;
		Graph_Init(&temp_bg);
		switch(flag)
		{
		case LAYOUT_ZOOM:/* ���� */
			Zoom_Graph(
			bg,           /* �����ͼƬ���� */
			&temp_bg,      /* ���ź��ͼƬ���ݽ����������ڱ���ͼ */
			DEFAULT,       /* ���ŵı�־(DEFAULT/CUSTOM),��־λȱʡ����ʹ��Ĭ�ϵ� */
			Size(graph->width, graph->height)
			);
			pos.x = (graph->width - temp_bg.width) / 2.0;
			pos.y = (graph->height - temp_bg.height) / 2.0;
			Mix_Graph(graph, &temp_bg, pos);
			break;
		case LAYOUT_STRETCH:/* ���� */
			Zoom_Graph(
			bg, 
			&temp_bg, 
			CUSTOM,        /* ���ŵı�־(DEFAULT/CUSTOM),ʹ���Զ��� */
			Size(graph->width, graph->height) 
			);
			Mix_Graph(graph, &temp_bg, pos);
			break;
		case LAYOUT_CENTER:/* ���� */
			pos.x = (graph->width - bg->width) / 2;
			pos.y = (graph->height - bg->height) / 2;
			Mix_Graph(graph, bg, pos);
			break;
		case LAYOUT_TILE:/* ƽ�� */
			Tile_Graph(bg, graph, graph->width, graph->height);
			break;
		default: 
			Mix_Graph(graph, bg, pos);
			break;
		}
		Free_Graph(&temp_bg);
		return 0;
	}
	return -1;
}

/* ������ͼ��Բ�ǻ��㷨����δ��� */
#ifdef use_round_rect
int Set_Graph_To_Rounded_Rectangle(LCUI_Graph *graph, int radius, int mode, int line_size, LCUI_RGB *color)
/* 
 * ���ܣ���ͼ��Բ�ǻ�
* ����˵����
* graph����Ҫ�����ͼ������
* radius��Բ�İ뾶
* mode�� ����ģʽ(draw_border_line ���Ʊ߿��� / none ������)
* line_size���ߵ�ֱ��
* color���ߵ���ɫ
*/
{
	Using_Graph(graph);/* ʹ������ */
	if(Valid_Graph(graph) && graph->flag == HAVE_ALPHA)
	{/* ���ͼ��������Ч,������alphaͨ�� */
		if(graph->width < radius*2 || graph->height < radius*2)
		{
			return 1;
		}
		int mini, count, x, y, temp, a, b, start_x, start_y;
		/* Բ�ķ��̣�(x��a)^2+(y��b)^2=r^2��ѧ����ѧ���㶮�ģ� */
		/* ׼�������Ͻ�Բ�ǻ� */
		start_x = 0;
		start_y = 0;
		a = radius;
		b = radius;
		for(y = 0; y <= radius; ++y)
		{
			mini = -1;
			for(x = 0; x <= radius; ++x)
			{
				temp = radius * radius - (x - a) * (x - a) - (y - b) * (y - b) ;
				temp = abs(temp);
				if(x == 0)
				{
					mini = temp;
					count = y * graph->width + x;
					graph->rgba[3][count] = 0;/* �������������ص�͸�� */
				}
				else
				{
					if(temp <= mini && temp != 0)
					{
						mini = temp;
						count = y * graph->width + x;
						graph->rgba[3][count] = 0;/* �������������ص�͸�� */
					}
					else
					{
						//printf("draw line : y = %d, x = %d, temp = %d\n", y, x, temp);
						for(temp = x - 1; temp < x -1 + line_size; ++temp) 
						{/* ���Ʊ߿��� */
							count = y * graph->width + temp;
							graph->rgba[0][count] = color->red;
							graph->rgba[1][count] = color->green;
							graph->rgba[2][count] = color->blue;
							graph->rgba[3][count] = 255;
							//printf("2222\n");
						}
						break;/* ����������Բ�Ͼ��˳�����forѭ�� */
					}
				}
			}
		}
	}
	End_Use_Graph(graph);
	return -1;
}

#endif

int Fill_Graph_Alpha(LCUI_Graph *src, unsigned char alpha)
/* ���ܣ����ͼ�ε�alphaͨ��������ֵ */
{
	if(Valid_Graph(src) && Graph_Have_Alpha(src))
	{
		memset(src->rgba[3], alpha, sizeof(unsigned char) * src->width * src->height);
		return 0;
	}
	return -1;
}

int Draw_Graph_Border(LCUI_Graph *src,LCUI_RGB color, LCUI_Border border)
/* ���ܣ�Ϊͼ�α�Ե���ƾ��α߿� */
{
	int x,y,count, k, start_x,start_y;
	if(Valid_Graph(src))
	{
		/* �����ϱߵ��� */
		Using_Graph(src, 1);
		for(y=0;y<border.top;++y)
		{
			k = y * src->width;
			for(x = 0; x < src->width; ++x)
			{
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(src->flag == HAVE_ALPHA) src->rgba[3][count] = 255;
			}
		}
		start_y = src->height - border.bottom;
		/* �����±ߵ��� */
		for(y=0;y<border.bottom;++y)
		{
			k = (start_y+y) * src->width;
			for(x=0;x<src->width;++x)
			{
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(src->flag == HAVE_ALPHA) 
					src->rgba[3][count] = 255;
			}
		}
		/* ������ߵ��� */
		for(y=0;y<src->height;++y)
		{
			k = y * src->width;
			for(x=0;x<border.left;++x)
			{
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(src->flag == HAVE_ALPHA) 
					src->rgba[3][count] = 255;
			}
		}
		/* �����ұߵ��� */
		start_x = src->width - border.right;
		for(y=0;y<src->height;++y)
		{
			k = y * src->width + start_x;
			for(x=0;x<border.right;++x) 
			{
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(src->flag == HAVE_ALPHA) 
					src->rgba[3][count] = 255;
			}
		}
		/* �߿��߻������ */
		End_Use_Graph(src);
	}
	else return -1;
	return 0;
}


void Get_Overlay_Widget(LCUI_Rect rect, LCUI_Widget *widget, LCUI_Queue *queue)
/* ���ܣ���ȡ��ָ�������ص��Ĳ��� */
{
	int i, total;
	LCUI_Pos pos;
	LCUI_Rect tmp;
	LCUI_Widget *child; 
	LCUI_Queue *widget_list;
	 
	if(widget == NULL) 
		widget_list = &LCUI_Sys.widget_list; 
	else 
	{
		if(widget->visible == IS_FALSE) 
			return;
		widget_list = &widget->child; 
	}
	
	total = Queue_Get_Total(widget_list); 
	
	for(i=total-1; i>=0; --i)
	{/* �ӵ׵��������Ӳ��� */
		child = (LCUI_Widget*)Queue_Get(widget_list, i); 
		if(child != NULL && child->visible == IS_TRUE)
		{/* ����пɼ����Ӳ��� */ 
			tmp = Get_Widget_Valid_Rect(child); 
			pos = Get_Widget_Global_Pos(child);
			tmp.x += pos.x;
			tmp.y += pos.y; 
			if(!Rect_Valid(tmp)) continue;
			if (Rect_Is_Overlay(tmp, rect)) 
			{ 
				Queue_Add_Pointer(queue, child);/* ��¼��������ص��Ĳ��� */
				Get_Overlay_Widget(rect, child, queue);  /* �ݹ���� */
			} 
		}
	} 
}

LCUI_RGBA Get_Graph_Pixel(LCUI_Graph *graph, LCUI_Pos pos)
/* ���ܣ���ȡͼ����ָ����������ص����ɫ */
{
	int i;
	LCUI_RGBA rgba;
	
	i = graph->width*pos.y + pos.x; 
	
	rgba.red = graph->rgba[0][i];
	rgba.green = graph->rgba[1][i];
	rgba.blue = graph->rgba[2][i];
	
	if(Graph_Have_Alpha(graph))
		rgba.alpha = graph->rgba[3][i];
	else 
		rgba.alpha = 255;
		
	return rgba;
}

int Graph_Is_Opaque(LCUI_Graph *graph)
/* 
 * ���ܣ����ͼ���Ƿ�Ϊ��͸�� 
 * ˵������ȫ͸���򷵻�-1����͸���򷵻�1����͸��Ч���򷵻�0
 * */
{ 
	if( !Graph_Have_Alpha(graph) )
		return 1; 
	if( graph->alpha == 0 )
		return -1;
	else if(graph->alpha < 255)
		return 0;
	//������µ��㷨�����Կ��ټ���һ��ͼ���Ƿ�ȫ͸����͸���������������
	#ifdef have_new_method
	LCUI_Pos pos;
	LCUI_RGBA rgba;
	int i, total, not_visbale = 0;
	/* ���ݳߴ磬����ȡ�������� */
	total = (graph->width + graph->height)/4.0;
	for(i=0; i<total; ++i)
	{
		pos.x = rand()%graph->width;
		pos.y = rand()%graph->height;
		rgba = Get_Graph_Pixel(graph, pos);
		if( rgba.alpha == 0 && not_visbale == 0)
			not_visbale = 1;
		else if(rgba.alpha < 255)
			return 0;
	}
	if(not_visbale == 1)
		return -1;
	#endif
	return 0;
}

int Widget_Layer_Is_Opaque(LCUI_Widget *widget)
/* ���ܣ��жϲ���ͼ���Ƿ�͸�� */
{ 
	return Graph_Is_Opaque(&widget->graph);
}

int Widget_Layer_Not_Visible(LCUI_Widget *widget)
/* ���ܣ���ⲿ��ͼ���Ƿ���ȫ͸�� */
{
	if(Graph_Is_Opaque(&widget->graph) == -1)
		return 1;
	return 0;
}

int Get_Screen_Real_Graph (LCUI_Rect rect, LCUI_Graph * graph)
/* 
 * ���ܣ���ȡ��Ļ��ָ��������ʵ��Ҫ��ʾ��ͼ�� 
 * ˵����ָ��������������벿�����򲻲����ص���
 * */
{ 
	LCUI_Pos pos;
	LCUI_Widget *widget; 
	LCUI_Queue widget_buff;
	
	Queue_Init(&widget_buff, sizeof(LCUI_Widget), NULL);
	Queue_Using_Pointer(&widget_buff); /* ֻ���ڴ��ָ�� */
	/* �����������Ƿ���Ч */
	if (rect.x < 0) return -1; 
	if (rect.y < 0) return -1; 
	if (rect.x + rect.width > Get_Screen_Width ())
		return -1;
	if (rect.y + rect.height > Get_Screen_Height ())
		return -1;
	
	if (rect.width <= 0 && rect.height <= 0)
		return -2;
	
	int i, total; 
	/* ����ָ���ĳߴ磬�����ڴ�ռ䣬���ڴ���ͼ������ */
	Malloc_Graph(graph, rect.width, rect.height);
	/* ��ȡ��������ص��Ĳ���������¼������widget_buff�� */
	Get_Overlay_Widget(rect, NULL, &widget_buff); 
	
	total = Queue_Get_Total(&widget_buff); 
	if(total > 0)
	{
		//printf("rect(%d,%d,%d,%d), list cover widget:\n",
		//rect.x, rect.y, rect.width, rect.height
		//);
		//printf("list cover widget:\n"); 
		for(i=total-1; i>=0; --i)
		{/* ������ĩ������ǰ����ʾ�Ĳ��������Դ�β��ͷ���� */
			widget = (LCUI_Widget*)Queue_Get(&widget_buff, i);
			//print_widget_info(widget); 
			/* ���ͼ����ȫ���ɼ�������ȫ͸�� */
			if(Widget_Layer_Not_Visible(widget))  
				Queue_Delete_Pointer(&widget_buff, i); 
			else if( Widget_Layer_Is_Opaque( widget) )
			{/* �������ͼ�㲻͸�� */
				//print_widget_info(widget); 
				//printf("Is_Opaque!\n");
				break; 
			}
		}
		//printf("list end\n");
		if(i >= 0)
		{/* ����ҵ���ǰ�˵Ĳ�͸����ͼ�� */
			total = Queue_Get_Total(&widget_buff);
			for(; i<total; ++i)
			{
				widget = (LCUI_Widget*)Queue_Get(&widget_buff, i); 
				pos = Get_Widget_Global_Pos(widget);
				/* ��ȡ������� */
				pos.x -= rect.x;
				pos.y -= rect.y;
				/* ���� */ 
				Mix_Graph(graph, &widget->graph, pos);
			}
		}
		else
		{/* ���򣬽�ȡ����ͼ��Ȼ���ٻ�ϵ��Ӳ���ͼ�� */
			graph->flag = NO_ALPHA; 
			Cut_Graph (&LCUI_Sys.screen.buff, rect, graph);
			for(i=0; i<total; ++i)
			{
				widget = (LCUI_Widget*)Queue_Get(&widget_buff, i);
				pos = Get_Widget_Global_Pos(widget);
				pos.x -= rect.x;
				pos.y -= rect.y; 
				Mix_Graph(graph, &widget->graph, pos);
			}
		}
	}
	else
	{/* ����ֱ��������ͼ */
		graph->flag = NO_ALPHA; 
		Cut_Graph (&LCUI_Sys.screen.buff, rect, graph);
	}
	
	if (LCUI_Sys.cursor.visible == IS_TRUE)
	{ /* ����α�ɼ� */
		/* ���������Ƿ����α��ͼ�������ص� */ 
		if (Rect_Is_Overlay( rect, Get_Cursor_Rect()) )
		{ /* ����ص� */
			pos.x = LCUI_Sys.cursor.pos.x - rect.x;
			pos.y = LCUI_Sys.cursor.pos.y - rect.y;
			/* ��ͼ�κϳ� */ 
			Mix_Graph (graph, &LCUI_Sys.cursor.graph, pos);
		}
	} 
	/* �ͷŶ���ռ�õ��ڴ�ռ� */
	Destroy_Queue(&widget_buff);
	return 0;
}

LCUI_RGBA RGBA_Mix(LCUI_RGBA back, LCUI_RGBA fore)
/* ���ܣ�����������ص����ɫ */
{
	if(fore.alpha == 255)
	{
		back.red	= fore.red;
		back.blue	= fore.blue;
		back.green	= fore.green;
	}
	else if(fore.alpha == 0);
	else
	{
		back.red	= (fore.red * fore.alpha + back.red * (255 - back.alpha)) /255;
		back.green	= (fore.green * fore.alpha + back.green * (255 - back.alpha)) /255;
		back.blue	= (fore.blue * fore.alpha + back.blue * (255 - back.alpha)) /255;
	}
	
	return back;
}

static long max(long a, long b)
{
	return a > b ? a:b;
}

static double radian(int angle)
/* ���ܣ����㻡�� */
{
	return angle*3.1415926/180;
}

int Rotate_Graph(LCUI_Graph *src, int rotate_angle, LCUI_Graph *des)
/* 
 * ���ܣ���תͼ��
 * ˵����ָ����ת���ĵ������Լ���ת�Ƕȣ����ɵõ���ת���ͼ��
 * ��Դ����ο��ԣ�http://read.pudn.com/downloads154/sourcecode/graph/684994/%E5%9B%BE%E5%83%8F%E6%97%8B%E8%BD%AC/%E5%9B%BE%E5%83%8F%E6%97%8B%E8%BD%AC/%E6%BA%90%E4%BB%A3%E7%A0%81/MyDIPView.cpp__.htm
 * �㷨�д��Ż����ơ�
 */
{
	if(!Valid_Graph(src)) return -1;
    // Դͼ��Ŀ�Ⱥ͸߶�
    int    width, height;   
       
    // ��ת��ͼ��Ŀ�Ⱥ͸߶�   
    int    new_width,new_height;

    // ��ת�Ƕȣ����ȣ�   
    float   fRotateAngle;
    
    // ��ת�Ƕȵ����Һ�����   
    float   fSina, fCosa;
       
    // Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩   
    float   fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;   
       
    // ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩   
    float   fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
    
    // �����м䳣��   
    float   f1,f2;
    
    // ��ȡͼ���"���"��4�ı�����   
    width = src->width;
       
    // ��ȡͼ��ĸ߶�   
    height = src->height;   
       
    // ����ת�ǶȴӶ�ת��������   
    fRotateAngle = (float) radian(rotate_angle);   
       
    // ������ת�Ƕȵ�����   
    fSina = (float) sin((double)fRotateAngle);   
       
    // ������ת�Ƕȵ�����   
    fCosa = (float) cos((double)fRotateAngle);   
    
    // ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩   
    fSrcX1 = (float) (- (width  - 1) / 2);   
    fSrcY1 = (float) (  (height - 1) / 2);   
    fSrcX2 = (float) (  (width  - 1) / 2);   
    fSrcY2 = (float) (  (height - 1) / 2);   
    fSrcX3 = (float) (- (width  - 1) / 2);   
    fSrcY3 = (float) (- (height - 1) / 2);   
    fSrcX4 = (float) (  (width  - 1) / 2);   
    fSrcY4 = (float) (- (height - 1) / 2);   
       
    // ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩   
    fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;   
    fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;   
    fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;   
    fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;   
    fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;   
    fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;   
    fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;   
    fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;   
       
    // ������ת���ͼ��ʵ�ʿ��   
    new_width  = (long) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);   
    // ������ת���ͼ��߶�   
    new_height = (long) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);   
       
    // �������������������Ժ�ÿ�ζ�������   
    f1 = (float) (-0.5 * (new_width - 1) * fCosa - 0.5 * (new_height - 1) * fSina   
        + 0.5 * (width  - 1));   
    f2 = (float) ( 0.5 * (new_width - 1) * fSina - 0.5 * (new_height - 1) * fCosa   
        + 0.5 * (height - 1));   
       
    if(Valid_Graph(des))
		Free_Graph(des);/* �Ƚ�����ڴ��ͷ� */
		
    des->flag = src->flag;
    // �����ڴ棬�����µ�ͼ��
    if(Malloc_Graph(des, new_width, new_height) != 0)
		return -1;

	long m, n, z;
	long src_x, src_y, des_x, des_y;
    // ���ͼ��ÿ�н��в���
    Using_Graph(src, 0);
    Using_Graph(des, 1);
    for(des_y = 0; des_y < new_height; ++des_y)   
    {
		m = new_width * des_y;
        // ���ͼ��ÿ�н��в���   
        for(des_x = 0; des_x < new_width; ++des_x)   
        {
			n = m + des_x;
            // �����������Դͼ�е�����   
            src_y = (long) (-((float) des_x) * fSina + ((float) des_y) * fCosa + f2 + 0.5);   
            src_x = (long) ( ((float) des_x) * fCosa + ((float) des_y) * fSina + f1 + 0.5);   
               
            // �ж��Ƿ���Դͼ��Χ��   
            if( (src_x >= 0) && (src_x < width) && (src_y >= 0) && (src_y < height))   
            {
                // ָ��ԴDIB��i0�У���j0�����ص�ָ��
                z = width * src_y + src_x;
                des->rgba[0][n] = src->rgba[0][z];
                des->rgba[1][n] = src->rgba[1][z];
                des->rgba[2][n] = src->rgba[2][z];
                if(des->flag == HAVE_ALPHA)
					des->rgba[3][n] = src->rgba[3][z];
            }
            else   
            {
                // ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255   
                des->rgba[0][n] = 255;
                des->rgba[1][n] = 255;
                des->rgba[2][n] = 255;
                if(des->flag == HAVE_ALPHA)
					des->rgba[3][n] = 0;
            }
        }
    }
    End_Use_Graph(src);
    End_Use_Graph(des);
    return 0;
}
