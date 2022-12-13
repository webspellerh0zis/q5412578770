#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_FONT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

/* 代码转换:从一种编码转为另一种编码，主要是调用iconv的API实现字符编码转换 */
static int code_convert(
			char *src_charset,	char *des_charset, 
			const char *inbuf,	unsigned int inlen,
			unsigned char *outbuf,	unsigned int outlen
	)
{
	iconv_t cd;
	const char **pin = &inbuf;
	unsigned char **pout = &outbuf;
	
	cd = iconv_open(des_charset, src_charset);
	if (cd==0) {
		return -1;
	}
	
	memset(outbuf, 0, outlen);
	
	if (iconv(cd,(char**)pin, &inlen, (char**)pout, &outlen)==-1) {
		return -1;
	}
	
	iconv_close(cd);
	return 0;
}

int Get_EncodingType()
/* 获取字符编码类型 */
{
	LCUI_App *app;
	app = Get_Self_AppPointer();
	if( !app ) {
		return -1;
	}
	return app->encoding_type;
}

static int Set_EncodingType(int type)
{
	LCUI_App *app;
	app = Get_Self_AppPointer();
	if(app == NULL) {
		return -1;
	}
	app->encoding_type = type;
	return 0;
}

int Using_GB2312()
/* 
 * 说明：如果你的系统只能使用GB2312编码，不能使用UTF-8编码，可以使用这
 * 个函数进行设置，让相关函数正常转换字符编码 
 * */
{
	return Set_EncodingType(ENCODEING_TYPE_GB2312);
}

#define MAX_SAVE_NUM   20
static wchar_t covernt_code(unsigned char in[MAX_SAVE_NUM])
{
 	wchar_t unicode;
 	unicode = in[0];
	if (unicode >= 0xF0) {
		unicode = (wchar_t) (in[0] & 0x07) << 18;
		unicode |= (wchar_t) (in[1] & 0x3F) << 12;
		unicode |= (wchar_t) (in[2] & 0x3F) << 6;
		unicode |= (wchar_t) (in[3] & 0x3F);
	} else if (unicode >= 0xE0) {
		unicode = (wchar_t) (in[0] & 0x0F) << 12;
		unicode |= (wchar_t) (in[1] & 0x3F) << 6;
		unicode |= (wchar_t) (in[2] & 0x3F);
	} else if (unicode >= 0xC0) {
		unicode = (wchar_t) (in[0] & 0x1F) << 6;
		unicode |= (wchar_t) (in[1] & 0x3F);
	}
	return unicode;
}

static int utf8_to_unicode(char *in_utf8_str, wchar_t **out_unicode)
/* 功能：将GB2312编码的字符串转换成Unicode编码字符串 */
{
	wchar_t *buff;
	unsigned char *p, t, save[MAX_SAVE_NUM];
	unsigned int len, i, j, n, count;
 	
	len = strlen(in_utf8_str)+1;  
	buff = (wchar_t *)calloc(sizeof(wchar_t), len); 
	
	for(count=0,i=0,j=0; i<len; ++i) {
		t = in_utf8_str[i];
		/* 结束符的判断 */
		if(t == 0) break;
			
		if((t>>7) == 0) {// 0xxxxxxx
			buff[j] = t; 
			++j;
		}
		else if((t>>5) == 6) {// 110xxxxx 
			count = 2; 
		}
		else if((t>>4) == 14) {// 1110xxxx 
			count = 3; 
		}
		else if((t>>3) == 30) {// 11110xxx 
			count = 4; 
		}
		else if((t>>2) == 62) {// 111110xx 
			count = 5; 
		}
		else if((t>>1) == 126) {// 1111110x 
			count = 6; 
		}
		if(count > 0) {
			p = (unsigned char*)&in_utf8_str[i];
			for(n=0; n<count; ++n) {
				save[n] = *p++;
			}
				
			count = 0; 
			buff[j] = covernt_code(save);
			memset(save, 0, sizeof(save));
			++j;
		}
	}
	/****
	printf("result code:");
	for(i=0; i<j; ++i)
		printf("%02x ", buff[i]);
	printf("\n");
	* *****/
	*out_unicode = buff;
	return j;
}

static int gb2312_to_unicode(char *in_gb2312_str, wchar_t **out_unicode)
/* 功能：将GB2312编码的字符串转换成Unicode编码字符串 */
{
	char *buff;
	unsigned char *p;
	unsigned int len, new_len;
 
	len = strlen(in_gb2312_str);
	new_len = len*3;
	buff = (char *) calloc ( sizeof(char), new_len );
	p = (unsigned char*) buff;
	if(code_convert("gb2312", "utf8", in_gb2312_str, len, p, new_len)) {
		printf("gb2312_to_unicode(): error: "ERROR_CONVERT_ERROR);
		return -1;
	}
	len = utf8_to_unicode(buff, out_unicode);
	free(buff);
	return len;
}

int Char_To_Wchar_T(char *in_text, wchar_t **unicode_text)
/*
 * 功能：将char型字符串转换成wchar_t字符串
 * 参数说明：
 * in_text      ：传入的char型字符串
 * unicode_text ：输出的wchar_t型字符串
 * 返回值：正常则wchar_t型字符串的长度，否则返回-1
 * */
{
	switch(Get_EncodingType()) {
	    case ENCODEING_TYPE_GB2312:
		return gb2312_to_unicode(in_text, unicode_text); 
	    case ENCODEING_TYPE_UTF8:
	    default: 
		return utf8_to_unicode(in_text, unicode_text); 
	}
}
