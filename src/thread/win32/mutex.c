﻿/* mutex.c -- The win32 edition mutex lock
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>

#ifdef LCUI_THREAD_WIN32

int LCUIMutex_Init( LCUI_Mutex *mutex )
{
	*mutex = CreateMutex( NULL, FALSE, NULL );
	return 0;
}

/* Free the mutex */
void LCUIMutex_Destroy( LCUI_Mutex *mutex )
{
	CloseHandle( *mutex );
}

/* Try lock the mutex */
int LCUIMutex_TryLock( LCUI_Mutex *mutex )
{
	switch ( WaitForSingleObject( *mutex, 0 ) ) {
	case WAIT_FAILED:
		return -1;
	case WAIT_OBJECT_0:
		break;
	default:
		return -2;
	}
	return 0;
}

/* Lock the mutex */
int LCUIMutex_Lock( LCUI_Mutex *mutex )
{
	if ( WaitForSingleObject( *mutex, INFINITE ) == WAIT_FAILED ) {
		return -1;
	}
	return 0;
}

/* Unlock the mutex */
int LCUIMutex_Unlock( LCUI_Mutex *mutex )
{
	if ( ReleaseMutex( *mutex ) == FALSE ) {
		return -1;
	}
	return 0;
}
#endif
