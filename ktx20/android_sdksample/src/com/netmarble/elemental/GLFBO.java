
package com.netmarble.elemental;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import android.opengl.GLES20;


class GLFBO
{
	private int  [] fbo_obj   = new int[1];
	private int  [] fbo_dpt   = new int[1];
	private int     fbo_w     = 512;
	private int     fbo_h     = 256;

	private int  [] fbo_dif   = new int[1];
	private int  [] fbo_vbo   = new int[1];

	private int     org_sce   = 0;						// scissor enable
	private int  [] org_scr   = new int  [8];			// scissor region

	private int  [] org_vpt   = new int  [8];			// view port
	private float[] org_bk    = new float[4];			// clear color
	private int  [] org_fbo   = new int  [1];			// fbo id


	private int mProgram;
	private int at_pos;
	private int at_tex;
	private int uf_tex;

	private static final int FLOAT_SIZE_BYTES           = 4;
	private static final int VERTICES_DATA_STRIDE_BYTES = 4 * FLOAT_SIZE_BYTES;
	private static final int VERTICES_DATA_POS_OFFSET   = 0 * FLOAT_SIZE_BYTES;
	private static final int VERTICES_DATA_TEX_OFFSET   = 2 * FLOAT_SIZE_BYTES;

	public int Create(int _w, int _h)
	{
		fbo_w = _w;
		fbo_h = _h;

		final String shd_vtx =
			"precision mediump float;                       \n" +
			"attribute vec2 at_pos;                         \n" +
			"attribute vec2 at_tex;                         \n" +

			"varying vec2 vr_tex;                           \n" +

			"void main() {                                  \n" +
			"  gl_Position = vec4(at_pos, 0.0, 1.0);        \n" +
			"  vr_tex      = at_tex;                        \n" +
			"}                                              \n"
			;

		final String shd_frg =
			"precision mediump float;                       \n" +
			"varying vec2      vr_tex;                      \n" +
			"uniform sampler2D uf_tex;                      \n" +

			"void main() {                                  \n" +
			"  gl_FragColor = texture2D(uf_tex, vr_tex);    \n" +
			"}                                              \n"
			;

		mProgram = createProgram(shd_vtx, shd_frg);
		at_pos = GLES20.glGetAttribLocation (mProgram, "at_pos");
		at_tex = GLES20.glGetAttribLocation (mProgram, "at_tex");
		uf_tex = GLES20.glGetUniformLocation(mProgram, "uf_tex");


		GLES20.glGenBuffers(1, fbo_vbo, 0);
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, fbo_vbo[0]);
		{
			float[] vtx_src = // X,Y, U,V
			{
				-1.0f, -1.0f, 0.0f, 0.0f,
				+1.0f, -1.0f, 1.0f, 0.0f,
				+1.0f,  1.0f, 1.0f, 1.0f,
				-1.0f,  1.0f, 0.0f, 1.0f,
			};

			FloatBuffer vtx_buf = ByteBuffer.allocateDirect(vtx_src.length * FLOAT_SIZE_BYTES)
											.order(ByteOrder.nativeOrder())
											.asFloatBuffer();
			vtx_buf.put(vtx_src).position(0);
			GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, vtx_src.length * FLOAT_SIZE_BYTES, vtx_buf, GLES20.GL_STATIC_DRAW);
		}
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);


		GLES20.glGetIntegerv(GLES20.GL_FRAMEBUFFER_BINDING, org_fbo, 0);	// Get the currently bound frame buffer object. On most platforms this just gives 0.
		GLES20.glGenTextures     (1, fbo_dif, 0);							// Create a color buffer
		GLES20.glGenRenderbuffers(1, fbo_dpt, 0);							// Generate and bind a render buffer which will become a depth buffer shared between our two FBOs
		GLES20.glGenFramebuffers (1, fbo_obj, 0);							// Create the object that will allow us to render to the aforementioned texture


		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, fbo_dif[0]);
		{
			GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0
									, GLES20.GL_RGB
									, fbo_w, fbo_h, 0
									, GLES20.GL_RGB, GLES20.GL_UNSIGNED_SHORT_5_6_5, null);

			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
		}
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);


		GLES20.glBindRenderbuffer(GLES20.GL_RENDERBUFFER, fbo_dpt[0]);
		{
			GLES20.glRenderbufferStorage(GLES20.GL_RENDERBUFFER								// setup the depth buffer storage
										, GLES20.GL_DEPTH_COMPONENT16
										, fbo_w, fbo_h);
		}
		GLES20.glBindRenderbuffer(GLES20.GL_RENDERBUFFER, 0);


		GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, fbo_obj[0]);

		GLES20.glFramebufferTexture2D(    GLES20.GL_FRAMEBUFFER								// Attach the texture to the FBO
										, GLES20.GL_COLOR_ATTACHMENT0
										, GLES20.GL_TEXTURE_2D, fbo_dif[0], 0);

		GLES20.glFramebufferRenderbuffer( GLES20.GL_FRAMEBUFFER								// Attach the depth buffer we created earlier to our FBO.
										, GLES20.GL_DEPTH_ATTACHMENT
										, GLES20.GL_RENDERBUFFER, fbo_dpt[0]);


		int uStatus = GLES20.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER);				// Check that our FBO creation was successful
		if(uStatus != GLES20.GL_FRAMEBUFFER_COMPLETE)
		{
			AppUtil.LOGE("ERROR: Failed to initialise FBO");
			return -1;
		}

		GLES20.glGetFloatv(GLES20.GL_COLOR_CLEAR_VALUE, org_bk, 0);

		GLES20.glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);			// Clear the colour and depth buffers for the FBO / PBuffer surface

		GLES20.glClearColor(org_bk[0], org_bk[1], org_bk[2], org_bk[3]);
		GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, org_fbo[0]);						// Unbind the frame buffer object so rendering returns back to the backbuffer



		AppUtil.LOGW("###################################################");
		AppUtil.LOGW("FBO created");
		AppUtil.LOGW("###################################################");

		return 0;
	}

	public int Begin()
	{
		org_sce = GLES20.glIsEnabled(GLES20.GL_SCISSOR_TEST)? 1: 0;

		GLES20.glGetIntegerv(GLES20.GL_SCISSOR_BOX, org_scr, 0);
		GLES20.glGetIntegerv(GLES20.GL_FRAMEBUFFER_BINDING, org_fbo, 0);					// Get the currently bound frame buffer object. On most platforms this just gives 0.
		GLES20.glGetIntegerv(GLES20.GL_VIEWPORT, org_vpt, 0);
		GLES20.glGetFloatv  (GLES20.GL_COLOR_CLEAR_VALUE, org_bk, 0);


		GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, fbo_obj[0]);						// bind render target


		GLES20.glDisable(GLES20.GL_SCISSOR_TEST);
		GLES20.glClearColor(1.0F, 1.0F, 0.0F, 1.0F);
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);			// clear the original frame buffer

		//GLES20.glEnable(GLES20.GL_SCISSOR_TEST);											// restrict the rendering region to target viewport
		//GLES20.glScissor (0,0, fbo_w, fbo_h);
		GLES20.glViewport(0,0, fbo_w, fbo_h);

		return 0;
	}

	public int End()
	{
		if(0 == org_sce)
			GLES20.glDisable(GLES20.GL_SCISSOR_TEST);
		else
			GLES20.glEnable(GLES20.GL_SCISSOR_TEST);

		GLES20.glClearColor(org_bk[0], org_bk[1], org_bk[2], org_bk[3]);
		GLES20.glViewport(org_vpt[0], org_vpt[1], org_vpt[2], org_vpt[3]);
		GLES20.glScissor(org_scr[0], org_scr[1], org_scr[2], org_scr[3]);

		GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, org_fbo[0]);						// restore original frame buffer

		return 0;
	}

	public int Draw()
	{
		// draw processing the texture copied from scene
		GLES20.glUseProgram(mProgram);

		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, fbo_dif[0]);
		GLES20.glUniform1i(uf_tex, 0);


		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, fbo_vbo[0]);
		GLES20.glEnableVertexAttribArray(at_pos);
		GLES20.glEnableVertexAttribArray(at_tex);

		GLES20.glVertexAttribPointer(at_pos, 2, GLES20.GL_FLOAT, false, VERTICES_DATA_STRIDE_BYTES, VERTICES_DATA_POS_OFFSET);
		GLES20.glVertexAttribPointer(at_tex, 2, GLES20.GL_FLOAT, false, VERTICES_DATA_STRIDE_BYTES, VERTICES_DATA_TEX_OFFSET);

		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, 4);
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);

		GLES20.glDisableVertexAttribArray(at_pos);
		GLES20.glDisableVertexAttribArray(at_tex);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);

		GLES20.glUseProgram(0);

		return 0;
	}

	private int loadShader(int shaderType, String source)
	{
		int shader = GLES20.glCreateShader(shaderType);
		if (shader != 0)
		{
			GLES20.glShaderSource(shader, source);
			GLES20.glCompileShader(shader);
			int[] compiled = new int[1];
			GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
			if (compiled[0] == 0) {
				AppUtil.LOGE("Could not compile shader " + shaderType + ":");
				AppUtil.LOGE(GLES20.glGetShaderInfoLog(shader));
				GLES20.glDeleteShader(shader);
				shader = 0;
			}
		}
		return shader;
	}

	private int createProgram(String vertexSource, String fragmentSource)
	{
		int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
		if (vertexShader == 0)
		{
			return 0;
		}
		int pixelShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
		if (pixelShader == 0)
		{
			return 0;
		}

		int program = GLES20.glCreateProgram();
		if (program != 0)
		{
			GLES20.glAttachShader(program, vertexShader);			checkGlError("glAttachShader");
			GLES20.glAttachShader(program, pixelShader);			checkGlError("glAttachShader");
			GLES20.glLinkProgram(program);

			int[] linkStatus = new int[1];
			GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
			if (linkStatus[0] != GLES20.GL_TRUE)
			{
				AppUtil.LOGE("Could not link program: ");
				AppUtil.LOGE(GLES20.glGetProgramInfoLog(program));
				GLES20.glDeleteProgram(program);
				program = 0;
			}
		}
		return program;
	}

	private void checkGlError(String op)
	{
		int error;
		while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR)
		{
			AppUtil.LOGE(op + ": glError " + error);
			//throw new RuntimeException(op + ": glError " + error);
		}
	}
}

