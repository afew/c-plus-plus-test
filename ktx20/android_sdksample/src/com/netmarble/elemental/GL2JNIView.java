
package com.netmarble.elemental;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;


class GL2JNIView extends GLSurfaceView
{
	public static int EGL_CTX_VERSION     = 0x3098;
	public static int EGL_OPENGL_ES2_BIT  = 0x0004;

	public static GLFBO	mFBO = null;

	public GL2JNIView(Context context)
	{
		super(context);
		init(false, 16, 8);
	}

	public GL2JNIView(Context context, boolean translucent, int depth, int stencil)
	{
		super(context);
		init(translucent, depth, stencil);
	}

	private void init(boolean translucent, int depth, int stencil)
	{
		if (translucent)
		{
			this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
		}


		setEGLContextFactory(new ContextFactory());
		setEGLConfigChooser( new ConfigChooser(8, 8, 8, 8, depth, stencil) );
		setPreserveEGLContextOnPause(true);
		setRenderer(new Renderer());


		//int gl_ver = com.netmarble.elemental.GL2JNIActivity.nsdk_glVersion();
		//setEGLContextClientVersion(gl_ver);
		//setEGLConfigChooser(8, 8, 8, 8, depth, stencil);
		//setPreserveEGLContextOnPause(true);
		//setRenderer(new Renderer());

		//AppUtil.LOGW("###################################################");
		//AppUtil.LOGW("GL2JNIView::init success");
		//AppUtil.LOGW("###################################################");
	}

	private static class ContextFactory implements GLSurfaceView.EGLContextFactory
	{
		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig)
		{
			int gl_ver = com.netmarble.elemental.GL2JNIActivity.nsdk_glVersion();
			AppUtil.LOGW("###################################################");
			AppUtil.LOGW("createContext: OpenGL ES " + gl_ver);
			AppUtil.LOGW("###################################################");
			checkEglError("Before eglCreateContext", egl);

			int[] attrib_list = {EGL_CTX_VERSION, gl_ver, EGL10.EGL_NONE };
			EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);


			checkEglError("After eglCreateContext", egl);

			return context;
		}

		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
		{
			egl.eglDestroyContext(display, context);
		}
	}


	private static void checkEglError(String prompt, EGL10 egl)
	{
		int error;
		while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS)
		{
			AppUtil.LOGE(String.format("%s: EGL error: 0x%x", prompt, error));
		}
	}

	private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser
	{
		public ConfigChooser(int r, int g, int b, int a, int depth, int stencil)
		{
			pxl_red     = r;
			pxl_green   = g;
			pxl_blue    = b;
			plx_alpha   = a;
			pxl_depth   = depth;
			pxl_stencil = stencil;
		}


		private static int[] s_configAttribs2 =
			{
			EGL10.EGL_RED_SIZE  , 8,
			EGL10.EGL_GREEN_SIZE, 8,
			EGL10.EGL_BLUE_SIZE , 8,
			EGL10.EGL_BLUE_SIZE , 8,
			EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL10.EGL_NONE
			};


		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
		{
			int[] num_config = new int[1];
			egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

			int numConfigs = num_config[0];

			if (numConfigs <= 0)
			{
				throw new IllegalArgumentException("No configs match configSpec --------------------------------------------------------");
			}


			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

			//printConfigs(egl, display, configs);


			return chooseConfig(egl, display, configs);
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs)
		{
			for(EGLConfig config : configs)
			{
				int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);

				// We need at least pxl_depth and pxl_stencil bits
				if (d < pxl_depth || s < pxl_stencil)
					continue;

				int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);
				int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
				int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
				int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);

				if (r == pxl_red && g == pxl_green && b == pxl_blue && a == plx_alpha)
					return config;
			}

			return null;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue)
		{
			if (egl.eglGetConfigAttrib(display, config, attribute, mValue))
			{
				return mValue[0];
			}

			return defaultValue;
		}

		private void printConfigs(EGL10 egl, EGLDisplay display, EGLConfig[] configs)
		{
			int numConfigs = configs.length;
			AppUtil.LOGW(String.format("%d configurations", numConfigs));

			for (int i = 0; i < numConfigs; i++)
			{
				AppUtil.LOGW(String.format("Configuration %d:\n", i));
				printConfig(egl, display, configs[i]);
			}
		}

		private void printConfig(EGL10 egl, EGLDisplay display, EGLConfig config)
		{
			int[] attributes =
				{
					EGL10.EGL_BUFFER_SIZE,
					EGL10.EGL_ALPHA_SIZE,
					EGL10.EGL_BLUE_SIZE,
					EGL10.EGL_GREEN_SIZE,
					EGL10.EGL_RED_SIZE,
					EGL10.EGL_DEPTH_SIZE,
					EGL10.EGL_STENCIL_SIZE,
					EGL10.EGL_CONFIG_CAVEAT,
					EGL10.EGL_CONFIG_ID,
					EGL10.EGL_LEVEL,
					EGL10.EGL_MAX_PBUFFER_HEIGHT,
					EGL10.EGL_MAX_PBUFFER_PIXELS,
					EGL10.EGL_MAX_PBUFFER_WIDTH,
					EGL10.EGL_NATIVE_RENDERABLE,
					EGL10.EGL_NATIVE_VISUAL_ID,
					EGL10.EGL_NATIVE_VISUAL_TYPE,
					0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
					EGL10.EGL_SAMPLES,
					EGL10.EGL_SAMPLE_BUFFERS,
					EGL10.EGL_SURFACE_TYPE,
					EGL10.EGL_TRANSPARENT_TYPE,
					EGL10.EGL_TRANSPARENT_RED_VALUE,
					EGL10.EGL_TRANSPARENT_GREEN_VALUE,
					EGL10.EGL_TRANSPARENT_BLUE_VALUE,
					0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
					0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
					0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
					0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
					EGL10.EGL_LUMINANCE_SIZE,
					EGL10.EGL_ALPHA_MASK_SIZE,
					EGL10.EGL_COLOR_BUFFER_TYPE,
					EGL10.EGL_RENDERABLE_TYPE,
					0x3042 // EGL10.EGL_CONFORMANT
				};

			String[] names =
				{
					"EGL_BUFFER_SIZE",
					"EGL_ALPHA_SIZE",
					"EGL_BLUE_SIZE",
					"EGL_GREEN_SIZE",
					"EGL_RED_SIZE",
					"EGL_DEPTH_SIZE",
					"EGL_STENCIL_SIZE",
					"EGL_CONFIG_CAVEAT",
					"EGL_CONFIG_ID",
					"EGL_LEVEL",
					"EGL_MAX_PBUFFER_HEIGHT",
					"EGL_MAX_PBUFFER_PIXELS",
					"EGL_MAX_PBUFFER_WIDTH",
					"EGL_NATIVE_RENDERABLE",
					"EGL_NATIVE_VISUAL_ID",
					"EGL_NATIVE_VISUAL_TYPE",
					"EGL_PRESERVED_RESOURCES",
					"EGL_SAMPLES",
					"EGL_SAMPLE_BUFFERS",
					"EGL_SURFACE_TYPE",
					"EGL_TRANSPARENT_TYPE",
					"EGL_TRANSPARENT_RED_VALUE",
					"EGL_TRANSPARENT_GREEN_VALUE",
					"EGL_TRANSPARENT_BLUE_VALUE",
					"EGL_BIND_TO_TEXTURE_RGB",
					"EGL_BIND_TO_TEXTURE_RGBA",
					"EGL_MIN_SWAP_INTERVAL",
					"EGL_MAX_SWAP_INTERVAL",
					"EGL_LUMINANCE_SIZE",
					"EGL_ALPHA_MASK_SIZE",
					"EGL_COLOR_BUFFER_TYPE",
					"EGL_RENDERABLE_TYPE",
					"EGL_CONFORMANT"
				};

			int[] value = new int[1];
			for (int i = 0; i < attributes.length; i++)
			{
				int attribute = attributes[i];
				String name = names[i];

				if ( egl.eglGetConfigAttrib(display, config, attribute, value))
				{
					AppUtil.LOGW(String.format("  %s: %d\n", name, value[0]));
				}
				else
				{
					// AppUtil.LOGW(String.format("  %s: failed\n", name));
					while (egl.eglGetError() != EGL10.EGL_SUCCESS);
				}
			}
		}

		// Subclasses can adjust these values:
		protected int pxl_red;
		protected int pxl_green;
		protected int pxl_blue;
		protected int plx_alpha;
		protected int pxl_depth;
		protected int pxl_stencil;
		private int[] mValue = new int[1];
	}

	private static class Renderer implements GLSurfaceView.Renderer
	{
		private static long time_bgn   = System.currentTimeMillis();
		private static long time_cur   = System.currentTimeMillis();
		private static long time_cnt   = 0;
		private static double time_fps = 60.0;

		public void onDrawFrame(GL10 gl)
		{
			time_cur = System.currentTimeMillis();
			++time_cnt;
			if(32 <= time_cnt)
			{
				time_fps = (double)(time_cur - time_bgn)/time_cnt;
				time_fps = 1000.0 /time_fps;
				time_bgn = time_cur;
				time_cnt = 0;

				//net.netmarble.sample.MainActivity.LOGI("FPS   : " + time_fps);
			}


			mFBO.Begin();

			GL2JNIActivity.render();			// draw scene

			mFBO.End();

			mFBO.Draw();					// test
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
			mFBO = new GLFBO();
			mFBO.Create(1024, 1024);

			AppUtil.LOGW("###################################################");
			AppUtil.LOGW("onSurfaceCreated");
			AppUtil.LOGW("###################################################");

			GL2JNIActivity.create(GL2JNIActivity.main_asset_manager);

			time_bgn = System.currentTimeMillis();
			time_cur = time_bgn;
			time_cnt = 0;
		}

		public void onSurfaceChanged(GL10 gl, int w, int h)
		{
			AppUtil.LOGW("onSurfaceChanged ----------------------------------");
		}

	}

}
