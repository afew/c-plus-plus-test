
#include <functional>
#include <algorithm>
#include <array>
#include <vector>
#include <map>
#include <bitset>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wchar.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#include <pnet_client.h>
#include <ProudNetEnums.h>

#include <epp_ktx.h>

#include "app_nsdk_jni.h"
#include "gl_util.h"


static void callback_connect_complete(int io_type, int io_ret, void* buf, long long etc0, long long etc1, long long etc2)
{
	LOGD("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	LOGD("+                                                                                                  +");
	LOGD("+ callback_connect_complete: %d: %d", io_type, io_ret);

	if(PNET_IO_CONNECT == io_type && Proud::ErrorType_Ok == io_ret)
	{
		LOGD("callback_connect_complete:: connect");
	}
	else
	{
		LOGD("callback_connect_complete:: disconnect");
	}

	LOGD("+                                                                                                  +");
	LOGD("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
}

static void callback_excption(int io_type, int io_ret, void* buf, long long etc0, long long etc1, long long etc2)
{
	LOGD("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	LOGD("+                                                                                                  +");
	LOGD("+ callback_excption: %d: %d", io_type, io_ret);
	LOGD("+                                                                                                  +");
	LOGD("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
}

static void callback_recv(int io_type, int io_ret, void* buf, long long etc0, long long etc1, long long etc2)
{
	LOGD("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	LOGD("+                                                                                                  +");
	LOGD("+ callback_recv: %d: %d", io_type, io_ret);
	LOGD("+                                                                                                  +");
	LOGD("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

static JNIEnv*			g_env  = NULL;
static AAssetManager*	g_assm = NULL;



std::array<std::string, 4> images =
{
	"icon_etc1_discard_alpha.ktx",
	"icon_etc1_discard_alpha_mip.ktx",
	"icon_etc2_rgb8.ktx",
	"icon_etc2_rgba8.ktx"
};

static int     touch_cur  = -1;
static float   touch_x    = -1;
static float   touch_y    = -1;
static int     screen_w   = 0;
static int     screen_h   = 0;


static TGL_SHADER sh_0;
static TGL_SHADER sh_1;

static std::array<UINT32, 8> gl_tex={};



static const char glsl_vtx0[] =
	"attribute vec4 at_pos;			\n"
	"void main() {					\n"
	"  gl_Position = at_pos;		\n"
	"}								\n"
;

static const char glsl_frg0[] =
	"precision mediump float;		\n"
	"uniform vec4 uf_dif  ;			\n"
	"void main() {					\n"
	"  gl_FragColor = uf_dif;		\n"
	"}\n"
;


static const char glsl_vtx1[] =
	"attribute vec4 at_pos;			\n"
	"attribute vec2 at_tex;			\n"
	"varying   vec2 vr_tex;			\n"
	"void main() {					\n"
	"  gl_Position = at_pos;		\n"
	"  vr_tex = at_tex;				\n"
	"}								\n"
;

static const char glsl_frg1[] =
	"precision mediump float;						\n"
	"varying   vec2      vr_tex;					\n"
	"uniform   sampler2D us_tex;					\n"
	"void main() {									\n"
	"  gl_FragColor = texture2D(us_tex, vr_tex);	\n"
	"}\n"
;

static const float tri_dif[] ={0.4f, 0.6f, 0.3f, 1.0f};					// diffuse
static const float tri_pos[] ={0.0f, 0.8f, -0.8f, -0.8f, 0.8f, -0.8f};	// position

static const float btn_dif[][4] =
{
	{ 1.0f, 1.00f,  0.0f,  1.0f},
	{ 1.0f, 0.80f,  0.0f,  1.0f},
	{ 1.0f, 0.60f,  0.0f,  1.0f},
	{ 1.0f, 0.40f,  0.0f,  1.0f},

	{ 1.00f, 0.0f,  1.0f,  1.0f},
	{ 0.80f, 0.0f,  1.0f,  1.0f},
	{ 0.60f, 0.0f,  1.0f,  1.0f},
	{ 0.40f, 0.0f,  1.0f,  1.0f},
};

static const float btn_pos[][8] =
{
	//   0               1               2       3       4
	{	-0.95f,  0.20f,	-0.55f,  0.20f,	-0.55f,  0.95f,	-0.95f,  0.95f},
	{	-0.50f,  0.20f,	-0.10f,  0.20f,	-0.10f,  0.95f,	-0.50f,  0.95f},
	{	-0.05f,  0.20f,	 0.35f,  0.20f,	 0.35f,  0.95f,	-0.05f,  0.95f},
	{	+0.40f,  0.20f,	 0.80f,  0.20f,	 0.80f,  0.95f,	+0.40f,  0.95f},

	{	-0.95f, -0.70f,	-0.55f, -0.70f,	-0.55f,  0.05f,	-0.95f,  0.05f},
	{	-0.50f, -0.70f,	-0.10f, -0.70f,	-0.10f,  0.05f,	-0.50f,  0.05f},
	{	-0.05f, -0.70f,	 0.35f, -0.70f,	 0.35f,  0.05f,	-0.05f,  0.05f},
	{	+0.40f, -0.70f,	 0.80f, -0.70f,	 0.80f,  0.05f,	+0.40f,  0.05f},
};


static const float btn_tex[] =
{
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

static void touchReset(){  touch_cur = -1; touch_x = -1; touch_y = -1; }


static bool AppInit()
{
	::srand((unsigned)time(NULL));
	float vpt[8]={0.0F, };


	glGetFloatv(GL_VIEWPORT, vpt);
	LOGI("glGetFloatv: %.f %.f %.f %.f------------------", vpt[0], vpt[1], vpt[2], vpt[3]);

	screen_w = (int)vpt[2];
	screen_h = (int)vpt[3];

	LOGI("AppInit (%d, %d) -----------------------------------", screen_w, screen_h);
	gl_printLog("OGL Version   ", GL_VERSION);
	gl_printLog("OGL Vendor    ", GL_VENDOR);
	gl_printLog("OGL Renderer  ", GL_RENDERER);
	gl_printLog("OGL Extensions", GL_EXTENSIONS);



	sh_0.createProgram(glsl_vtx0, glsl_frg0);
	if(0 > sh_0.createProgram(glsl_vtx0, glsl_frg0))
	{
		LOGE("create shader: 0");
		return false;
	}

	sh_0.setAttribute("at_pos", NULL, NULL, "uf_dif", NULL);


	if(0 > sh_1.createProgram(glsl_vtx1, glsl_frg1))
	{
		LOGE("create shader: 1");
		return false;
	}

	sh_1.setAttribute("at_pos", NULL, "at_tex", NULL, "us_tex");

	glViewport(0, 0, screen_w, screen_h);
	gl_checkError("glViewport");



	for(auto& r : gl_tex)
		glGenTextures(1, &r);



	for(int i=0; i<images.size(); ++i)
	{
		const char*  filename = images[i].c_str();
		LOGW(">>>>>>>>>>>>>>>>>>>>>> ktx file: %s \n", (const char*)filename);

		//char* utf8 = (char*)g_env->GetStringUTFChars(filename, NULL);
		//LOGW(">>>>>>>>>>>>>>>>>>>>>> ktx utf8: %s \n", (const char*)utf8);

		AAsset* asset = AAssetManager_open(g_assm, filename, AASSET_MODE_UNKNOWN);

		LOGW(">>>>>>>>>>>>>>>>>>>>>> AAsset* asset: %p \n", asset);

		int size = (int)AAsset_getLength(asset);
		char* buf = (char*)malloc(size);

		AAsset_read(asset, buf, size);
		AAsset_close(asset);

		//g_env->ReleaseStringUTFChars(filename, utf8);


		LOGW(">>>>>>>>>>>>>>>>>>>>>> AAsset* read: %d \n", size);


		int target = 0;
		KTX_DIMENSIONS dimension;
		unsigned int mipmapLevel = 0;
		int err = 0;

		int mipLvlBgn = i*2;
		int mipLvlEnd = i;
		int hr = KTX_LoadTextureM(buf, size, &gl_tex[0*4 + i], &target, &dimension, &mipmapLevel, &err, NULL, NULL, mipLvlBgn, mipLvlEnd);
		free(buf);
	}


	return true;
}

static int isTouchRect(float _x0, float _y0, float _x1, float _y1)
{
	float x0 = (1.0f + _x0)*0.5F * screen_w;	float y0 = (1.0f - _y1)*0.5F * screen_h;
	float x1 = (1.0f + _x1)*0.5F * screen_w;	float y1 = (1.0f - _y0)*0.5F * screen_h;

	if( x0 < touch_x && touch_x < x1 &&
		y0 < touch_y && touch_y < y1 )
	{
		return 1;
	}

	return 0;
}


static void AppRender()
{
	static int rot_angle = 0;
	++rot_angle;

	if(rot_angle > 360.0f)
		rot_angle = 0;

	float rot_radian = rot_angle* 3.14159265358979F /180.0F;
	float rot_cos = cos(rot_radian);
	float rot_sin = sin(rot_radian);

	float vtx_buf[6] ={0.0f};
	vtx_buf[0] = tri_pos[0] * rot_cos - tri_pos[1] * rot_sin;
	vtx_buf[1] = tri_pos[0] * rot_sin + tri_pos[1] * rot_cos;
	vtx_buf[2] = tri_pos[2] * rot_cos - tri_pos[3] * rot_sin;
	vtx_buf[3] = tri_pos[2] * rot_sin + tri_pos[3] * rot_cos;
	vtx_buf[4] = tri_pos[4] * rot_cos - tri_pos[5] * rot_sin;
	vtx_buf[5] = tri_pos[4] * rot_sin + tri_pos[5] * rot_cos;


	glClearColor(0.0F, 0.4F, 0.6F, 1.0f);									//gl_checkError("glClearColor");
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);					//gl_checkError("glClear");

	DrawBuffer(&sh_0, vtx_buf, tri_dif, 3);

	for(int i=0; i<images.size() * 2; ++i)
	{
		DrawRect(&sh_0, &btn_pos[i][0],  &btn_dif[i][0]);
	}

	for(int i=0; i<images.size(); ++i)
	{
		DrawSprite(&sh_1, gl_tex[0*4 + i], &btn_pos[0*4 + i][0], btn_tex);
	}
}



extern "C" {
JNI_FUNC_DCL(void, create)(JNIEnv* env, jobject obj, jobject assm/*asset mamager*/);
JNI_FUNC_DCL(void, render)(JNIEnv* env, jobject obj);
JNI_FUNC_DCL(void, touch)(JNIEnv* env, jobject obj, jint t_type, jfloat x, jfloat y);
};


JNI_FUNC_DCL(void, create)(JNIEnv* env, jobject obj, jobject assm/*asset mamager*/)
{
	g_env  = env;
	g_assm = AAssetManager_fromJava(g_env, assm);

	AppInit();
}

JNI_FUNC_DCL(void, render)(JNIEnv* env, jobject obj)
{
	AppRender();
}

JNI_FUNC_DCL(void, touch)(JNIEnv* env, jobject obj, jint t_type, jfloat x, jfloat y)
{
	touch_cur = t_type; touch_x = x; touch_y = y;
}



