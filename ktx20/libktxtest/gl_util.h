
#ifndef _GL_UTIL_H_
#define _GL_UTIL_H_

typedef unsigned int   UINT32;
typedef const char*    COSTR;
typedef const void*    COVOID;

extern void consoleColor(int color);
enum
{
	CONSOLE_COLOR_RED		= 12,
	CONSOLE_COLOR_GREEN		= 10,
	CONSOLE_COLOR_BLUE		=  9,
	CONSOLE_COLOR_YELLO		= 14,
	CONSOLE_COLOR_MAGENTA	= 13,
	CONSOLE_COLOR_CYAN		= 11,
	CONSOLE_COLOR_GRAY		=  7,
	CONSOLE_COLOR_WHITE		= 15,
};

#undef LOGD
#undef LOGI
#undef LOGW
#undef LOGE

#define  LOGD(...) {{char b[2048+8]="[D]:";::sprintf(b+4,__VA_ARGS__);::strcat(b,"\n");::consoleColor(CONSOLE_COLOR_WHITE  );::printf(b);::consoleColor(CONSOLE_COLOR_GRAY);}}
#define  LOGI(...) {{char b[2048+8]="[I]:";::sprintf(b+4,__VA_ARGS__);::strcat(b,"\n");::consoleColor(CONSOLE_COLOR_GREEN  );::printf(b);::consoleColor(CONSOLE_COLOR_GRAY);}}
#define  LOGW(...) {{char b[2048+8]="[W]:";::sprintf(b+4,__VA_ARGS__);::strcat(b,"\n");::consoleColor(CONSOLE_COLOR_YELLO  );::printf(b);::consoleColor(CONSOLE_COLOR_GRAY);}}
#define  LOGE(...) {{char b[2048+8]="[E]:";::sprintf(b+4,__VA_ARGS__);::strcat(b,"\n");::consoleColor(CONSOLE_COLOR_MAGENTA);::printf(b);::consoleColor(CONSOLE_COLOR_GRAY);}}


struct TGL_SHADER
{
	UINT32 prog;
	UINT32 apos;
	UINT32 adif;
	UINT32 atex;

	UINT32 udif;
	UINT32 usmp;

	TGL_SHADER();

	int createProgram(COSTR sh_vtx, COSTR sh_frg);
	void setAttribute(COSTR at_pos, COSTR at_dif, COSTR at_tex, COSTR uf_dif, COSTR us_tex);
};

void DrawSprite(TGL_SHADER* sh, UINT32 tex, COVOID vtx_pos, COVOID vtx_dif);
void DrawRect  (TGL_SHADER* sh, COVOID vtx_pos, COVOID vtx_dif);
void DrawBuffer(TGL_SHADER* sh, COVOID vtx_pos, COVOID dif, UINT32 vtx_count);

void gl_printLog(COSTR name, int s);
void gl_checkError(COSTR op);
UINT32 gl_createProgram(COSTR src_vtx, COSTR src_frg);
UINT32 gl_createTexture(void* pixel, int imgW, int imgH, int imgF);
void gl_releaseTexture(UINT32* tex);

#endif

