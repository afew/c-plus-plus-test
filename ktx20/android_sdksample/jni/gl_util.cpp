
#include <vector>
#include <string>


#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wchar.h>

#include "app_nsdk_jni.h"
#include "gl_util.h"


TGL_SHADER::TGL_SHADER()
{
	prog = 0;

	apos = 0;
	adif = 0;
	atex = 0;

	udif = 0;
	usmp = 0;
}

int TGL_SHADER::createProgram(COSTR sh_vtx, COSTR sh_frg)
{
	prog = gl_createProgram(sh_vtx, sh_frg);
	if(prog)
		return 0;

	LOGE("Could not create program.");
	return -1;
}


void TGL_SHADER::setAttribute(COSTR at_pos, COSTR at_dif, COSTR at_tex, COSTR uf_dif, COSTR us_tex)
{
	if(at_pos)	this->apos = glGetAttribLocation (prog, at_pos);	//gl_checkError("glGetAttribLocation");	LOGI("glGetAttribLocation (at_pos) = %d\n", apos);
	if(at_dif)	this->adif = glGetAttribLocation (prog, at_dif);	//gl_checkError("glGetAttribLocation");	LOGI("glGetAttribLocation (at_dif) = %d\n", adif);
	if(at_tex)	this->atex = glGetAttribLocation (prog, at_tex);	//gl_checkError("glGetAttribLocation");	LOGI("glGetAttribLocation (at_tex) = %d\n", atex);

	if(uf_dif)	this->udif = glGetUniformLocation(prog, uf_dif);	//gl_checkError("glGetUniformLocation");	LOGI("glGetUniformLocation(uf_dif) = %d\n", udif);
	if(us_tex)	this->usmp = glGetUniformLocation(prog, us_tex);	//gl_checkError("glGetUniformLocation");	LOGI("glGetUniformLocation(us_tex) = %d\n", usmp);
}


void gl_printLog(COSTR name, int s)
{
	COSTR v = (COSTR ) glGetString(s);
	LOGI("GL %s = %s\n", name, v);
}

void gl_checkError(COSTR op)
{
	for (int error = glGetError(); error; error = glGetError())
	{
		//LOGI("after %s() glError (0x%x)\n", op, error);
	}
}


static UINT32 gl_buildShader(int shaderType, COSTR pSource)
{
	//LOGI("%s: %d\n", __func__, __LINE__);

	UINT32 shader = glCreateShader(shaderType);

	if (shader)
	{
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		int compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			int infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen)
			{
				char* buf = (char*) malloc(infoLen);
				if (buf)
				{
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE("Could not compile shader %d:\n%s\n",
					shaderType, buf);
					free(buf);
				}

				glDeleteShader(shader);
				shader = 0;
			}
		}
	}

	return shader;
}

UINT32 gl_createProgram(COSTR src_vtx, COSTR src_frg)
{
	UINT32 vertexShader = gl_buildShader(GL_VERTEX_SHADER, src_vtx);
	if(!vertexShader)
		return 0;

	UINT32 pixelShader = gl_buildShader(GL_FRAGMENT_SHADER, src_frg);
	if(!pixelShader)
		return 0;

	UINT32 program = glCreateProgram();
	if(program)
	{
		glAttachShader(program, vertexShader);
		gl_checkError("glAttachShader");
		glAttachShader(program, pixelShader);
		gl_checkError("glAttachShader");
		glLinkProgram(program);
		int linkStatus = GL_FALSE;

		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

		if (linkStatus != GL_TRUE)
		{
			int bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);

			if (bufLength)
			{
				char* buf = (char*) malloc(bufLength);
				if (buf)
				{
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGE("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}

			glDeleteProgram(program);
			program = 0;
		}
	}

	return program;
}


UINT32 gl_createTexture(void* pixel, int imgW, int imgH, int imgF)
{
	unsigned tex = 0;
	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, imgF, imgW, imgH, 0, imgF, GL_UNSIGNED_BYTE, pixel);

	glBindTexture (GL_TEXTURE_2D, 0);
	return tex;
}

void gl_releaseTexture(UINT32* tex)
{
	if(NULL == tex || 0 == *tex)
		return;

	glDeleteTextures(1, tex);
	*tex = 0;
}

//------------------------------------------------------------------------------

void DrawSprite(TGL_SHADER* sh, unsigned tex, COVOID vtx_pos, COVOID vtx_dif)
{
	if(!tex)
		return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(sh->prog);
	glUniform1i(sh->usmp, 0);
	{
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		glEnableVertexAttribArray(sh->apos);	glVertexAttribPointer(sh->apos, 2, GL_FLOAT, GL_FALSE, 0, vtx_pos);
		glEnableVertexAttribArray(sh->atex);	glVertexAttribPointer(sh->atex, 2, GL_FLOAT, GL_FALSE, 0, vtx_dif);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture  (GL_TEXTURE_2D, 0);
		glDisable      (GL_TEXTURE_2D);
	}

	glUseProgram(0);

	glDisableVertexAttribArray(sh->atex);
	glDisable(GL_BLEND);
}


void DrawRect(TGL_SHADER* sh, COVOID vtx_pos, COVOID vtx_dif)
{
	glUseProgram(sh->prog);

	glUniform4fv(sh->udif, 1, (float*)vtx_dif);

	glEnableVertexAttribArray(sh->apos);	glVertexAttribPointer(sh->apos, 2, GL_FLOAT, GL_FALSE, 0, vtx_pos);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUseProgram(0);
}


void DrawBuffer(TGL_SHADER* sh, COVOID vtx_pos, COVOID dif, UINT32 vtx_count)
{
	glUseProgram(sh->prog);													//gl_checkError("glUseProgram");

	glUniform4fv(sh->udif, 1, (float*)dif);

	glEnableVertexAttribArray(sh->apos);									//gl_checkError("glEnableVertexAttribArray");
	glVertexAttribPointer(sh->apos, 2, GL_FLOAT, GL_FALSE, 0, vtx_pos);		//gl_checkError("glVertexAttribPointer");

	glDrawArrays(GL_TRIANGLES, 0, vtx_count);								//gl_checkError("glDrawArrays");
	glUseProgram(0);
}


