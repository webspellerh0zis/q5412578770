/* ***************************************************************************
 * fontconfig.c -- The Fontconfig support module.
 *
 * Copyright (c) 2018, Liu Chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LCUI_BUILD_IN_WIN32

#include <LCUI/config.h>
#include <stdlib.h>
#include <string.h>
#include <fontconfig/fontconfig.h>

char* Fontconfig_GetPath( char* name )
{
#ifdef USE_FONTCONFIG
	FcConfig* config = FcInitLoadConfigAndFonts();
	char* path = "";

	FcPattern* pat = FcNameParse( (const FcChar8*)name );
	FcConfigSubstitute( config, pat, FcMatchPattern );
	FcDefaultSubstitute( pat );

	FcResult result;
	FcPattern* font = FcFontMatch( config, pat, &result );

	if( font ) {
		FcChar8* file = NULL;
		if( FcPatternGetString( font, FC_FILE, 0, &file ) == FcResultMatch ) {
			size_t path_len = strlen( (char*)file ) + 1;
			path = (char*)malloc( path_len );
			memset( path, '\0', path_len );
			strncpy( path, (char*)file, path_len );
		}
		FcPatternDestroy( font );
	}

	FcPatternDestroy( pat );
	FcConfigDestroy( config );

	return path;
#else
	return NULL;
#endif
}

#endif
