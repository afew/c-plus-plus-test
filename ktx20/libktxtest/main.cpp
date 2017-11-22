#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "libGLESv2.lib")
#pragma comment(lib, "libEGL.lib")
#pragma comment(lib, "simple-framework2_d.lib")

#if defined(_DEBUG)
#pragma comment(lib, "../lib/libktx_gles2_d.lib")
#else
#pragma comment(lib, "../lib/libktx_gles2.lib")
#endif


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

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wchar.h>

#include <windows.h>

#include "epp_ktx.h"

#include "Timer.h"
#include "Platform.h"
#include "gl_util.h"


using namespace MaliSDK;



#define WINDOW_W 1280
#define WINDOW_H 720


MaliSDK::Platform* g_platform;
int g_iWnd;

static float s_f_offset_delta = 1.0f;




void* MainWindowHandle()
{
	void* ret = NULL;

#if defined(WIN32)
	ret = g_platform->window;
#endif
	return ret;
}


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

static std::array<UINT32, 1> gl_tex={};


static const char glsl_vtx0[] =
"#version 100					\n"
"attribute vec4 at_pos;			\n"
"void main() {					\n"
"  gl_Position = at_pos;		\n"
"}								\n"
;

static const char glsl_frg0[] =
"#version 100					\n"
"precision mediump float;		\n"
"uniform vec4 uf_dif  ;			\n"
"void main() {					\n"
"  gl_FragColor = uf_dif;		\n"
"}\n"
;


static const char glsl_vtx1[] =
"#version 100					\n"
"attribute vec4 at_pos;			\n"
"attribute vec2 at_tex;			\n"
"varying   vec2 vr_tex;			\n"
"void main() {					\n"
"  gl_Position = at_pos;		\n"
"  vr_tex = at_tex;				\n"
"}								\n"
;

static const char glsl_frg1[] =
"#version 100					\n"
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
		// load from buffer
		const char* fileName = images[i].c_str();
		FILE* fp = fopen(fileName, "rb");
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		char* buf = (char*)malloc(size);
		fseek(fp, 0, SEEK_SET);
		fread(buf, 1, size, fp);
		fclose(fp);

		int target = 0;
		KTX_DIMENSIONS dimension;
		unsigned int mipmapLevel = 0;
		int err = 0;

		int mipLvlBgn = i*2;
		int mipLvlEnd = i;
		int hr = KTX_LoadTextureM(buf, size, &gl_tex[0*4 + i], &target, &dimension, &mipmapLevel, &err, NULL, NULL, mipLvlBgn, mipLvlEnd);
		free(buf);


		// load from file pointer
		fp = fopen(fileName, "rb");
		hr = KTX_LoadTextureF(fp, &gl_tex[1*4 + i], &target, &dimension, &mipmapLevel, &err, NULL, NULL, i*2, i);
		fclose(fp);
	}


	return true;
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

	//for(int i=0; i<images.size() * 2; ++i)
	//{
	//	DrawRect(&sh_0, &btn_pos[i][0],  &btn_dif[i][0]);
	//}


	DrawSprite(&sh_1, gl_tex[0], &btn_pos[0][0], btn_tex);


	//for(int i=0; i<images.size(); ++i)
	//{
	//	DrawSprite(&sh_1, gl_tex[0*4 + i], &btn_pos[0*4 + i][0], btn_tex);
	//	DrawSprite(&sh_1, gl_tex[1*4 + i], &btn_pos[1*4 + i][0], btn_tex);
	//}

}



void Update(float _f_delta_time)
{
	RECT rc;
	::GetClientRect(g_platform->window, &rc);


	POINT pt_mouse;
	GetCursorPos(&pt_mouse);

	tagRECT rt_win;
	GetWindowRect(g_platform->window, &rt_win);

	if(rt_win.left > pt_mouse.x || rt_win.right < pt_mouse.x ||
		rt_win.top > pt_mouse.y || rt_win.bottom < pt_mouse.y)
	{
		return;
	}

	ScreenToClient(g_platform->window, &pt_mouse);
	static int sn_wheel_delta = 0;
	int n_wheel_delta = 0;

	static bool m_b_move = false;
	static bool m_b_cam_control = false;
	static UINT down_x = 0;
	static UINT down_y = 0;
	static bool m_b_pre_touch = false;
	if(GetAsyncKeyState(VK_LBUTTON) & 0x8001)
	{
		if(!m_b_pre_touch){
			down_x = (UINT)pt_mouse.x;
			down_y = (UINT)pt_mouse.y;
		}

		m_b_pre_touch = true;
	}
	else
	{
		UINT delta_x = std::abs(INT(pt_mouse.x - down_x));
		UINT delta_y = std::abs(INT(pt_mouse.y - down_y));
	}

	if(s_f_offset_delta < 0.0f) s_f_offset_delta = 0.0f;
	else if(s_f_offset_delta > 5.0f) s_f_offset_delta = 5.0f;

	if(GetAsyncKeyState(VK_OEM_COMMA) & 0x8001)
	{
		s_f_offset_delta -= 0.01f;
	}
	if(GetAsyncKeyState(VK_OEM_PERIOD) & 0x8001)
	{
		s_f_offset_delta += 0.01f;
	}
	if(GetAsyncKeyState(VK_OEM_2) & 0x8001)
	{
		s_f_offset_delta = 1.0f;
	}
	if(GetAsyncKeyState(0x70) & 0x8001)
	{
		s_f_offset_delta = 0.0f;
	}

	tagRECT rt;
	GetClientRect(g_platform->window, &rt);
	glViewport(0, 0, rt.right, rt.bottom);


	AppRender();

	glFlush();
}



int main(void)
{
	setlocale(LC_ALL, "");
	g_platform = MaliSDK::Platform::getInstance();
	g_platform->createWindow(WINDOW_W, WINDOW_H, "EPP Soft Game Simulator");

	EGLRuntime::initializeEGL(EGLRuntime::OPENGLES2);
	EGL_CHECK(eglMakeCurrent(EGLRuntime::display, EGLRuntime::surface, EGLRuntime::surface, EGLRuntime::context));

	char _str_cur_path[1024] ={0};
	GetModuleFileName(GetModuleHandle(NULL), _str_cur_path, sizeof(_str_cur_path)-4);
	SetCurrentDirectory(_str_cur_path);


	AppInit();



	MaliSDK::Timer fpsTimer;
	fpsTimer.reset();

	float frame_skip = 0.016f;

	bool end = false;
	while(!end)
	{
		float fFPS = fpsTimer.getFPS();
		float f_interval = fpsTimer.getInterval();
		if(fpsTimer.isTimePassed(1.0f))
		{
			//LOGI("FPS:\t%.1f %.8f", fFPS, f_interval);
		}


		switch(g_platform->checkWindow())
		{
			case Platform::WINDOW_EXIT:
			{
				end = true;
				break;
			}break;
			case Platform::WINDOW_CLICK:
			case Platform::WINDOW_IDLE:
			{
				Update(f_interval*frame_skip);
				eglSwapBuffers(EGLRuntime::display, EGLRuntime::surface);
			}break;
		}
	}

#ifdef VALID_TEST_TOOL
	test_tool::get_singleton().destroy();
#endif

	EGLRuntime::terminateEGL();
	g_platform->destroyWindow();
	delete g_platform;


	return 0;
}

