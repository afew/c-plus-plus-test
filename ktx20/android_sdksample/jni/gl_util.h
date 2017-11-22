
#ifndef _GL_UTIL_H_
#define _GL_UTIL_H_

typedef unsigned int   UINT32;
typedef const char*    COSTR;
typedef const void*    COVOID;

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

