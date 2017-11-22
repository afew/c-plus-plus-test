// stdafx.h 
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


#ifndef __stdafx_h_
#define __stdafx_h_

#ifndef _WIN32_WINNT        
#define _WIN32_WINNT 0x0500
#endif

#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <tchar.h>
#include <errno.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>


#ifndef GL_ALPHA16
#define GL_ALPHA16						0x803E
#endif

#ifndef GL_LUMINANCE16_ALPHA16
#define GL_LUMINANCE16_ALPHA16			0x8048
#endif

#ifndef GL_R16
#define GL_R16							0x822A
#endif

#ifndef GL_RG16
#define GL_RG16							0x822C
#endif

#ifndef GL_RGB8
#define GL_RGB8                         0x8051
#endif

#ifndef GL_RGBA8
#define GL_RGBA8                        0x8058
#endif

#ifndef GL_RGB16
#define GL_RGB16                        0x8054
#endif

#ifndef GL_RGBA16
#define GL_RGBA16                       0x805B
#endif

#endif
