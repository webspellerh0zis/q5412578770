﻿/*
 * widget_util.h -- widget utility functions
 *
 * Copyright (c) 2020, Liu chao <lc-soft@live.cn> All rights reserved.
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

#ifndef LCUI_WIDGET_UTIL_H
#define LCUI_WIDGET_UTIL_H

INLINE float PaddingX(LCUI_Widget w)
{
	return w->padding.left + w->padding.right;
}

INLINE float PaddingY(LCUI_Widget w)
{
	return w->padding.top + w->padding.bottom;
}

INLINE float BorderX(LCUI_Widget w)
{
	return w->computed_style.border.left.width +
	       w->computed_style.border.right.width;
}

INLINE float BorderY(LCUI_Widget w)
{
	return w->computed_style.border.top.width +
	       w->computed_style.border.bottom.width;
}

INLINE float MarginX(LCUI_Widget w)
{
	return w->margin.left + w->margin.right;
}

INLINE float MarginY(LCUI_Widget w)
{
	return w->margin.top + w->margin.bottom;
}

INLINE float ToBorderBoxWidth(LCUI_Widget w, float content_width)
{
	return content_width + PaddingX(w) + BorderX(w);
}

INLINE float ToBorderBoxHeight(LCUI_Widget w, float content_height)
{
	return content_height + PaddingY(w) + BorderY(w);
}

INLINE float Widget_GetLimitedWidth(LCUI_Widget w, float width)
{
	if (w->computed_style.max_width > -1 &&
	    width > w->computed_style.max_width) {
		width = w->computed_style.max_width;
	}
	if (w->computed_style.min_width > -1 &&
	    width < w->computed_style.min_width) {
		width = w->computed_style.min_width;
	}
	return width;
}

INLINE float Widget_GetLimitedHeight(LCUI_Widget w, float height)
{
	if (w->computed_style.max_height > -1 &&
	    height > w->computed_style.max_height) {
		height = w->computed_style.max_height;
	}
	if (w->computed_style.min_height > -1 &&
	    height < w->computed_style.min_height) {
		height = w->computed_style.min_height;
	}
	return height;
}

#endif
