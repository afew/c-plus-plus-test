
package com.netmarble.elemental;

import java.lang.reflect.InvocationTargetException;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.URL;
import java.net.URLConnection;
import java.util.Collections;
import java.util.List;

import android.annotation.SuppressLint;
import android.app.*;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.drawable.*;

import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.opengl.GLES20;
import android.os.*;
import android.os.Build.*;

import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;

import android.util.*;
import android.view.*;
import android.view.inputmethod.*;
import android.view.Display;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;

import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.webkit.JavascriptInterface;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import org.apache.http.conn.util.InetAddressUtils;
import org.apache.http.entity.BufferedHttpEntity;



@SuppressLint({ "SetJavaScriptEnabled", "JavascriptInterface" })

public class AppUtil
{
	public static GL2JNIActivity	main_activity   = null;
	public static SoftKeyboard      main_ime        = null;					// IME
	public static WebView           agreementViewO  = null;					// agreement view Object
	public static Handler           agreementViewH  = new Handler();		// agreement view Handle

//------------------------------------------------------------------------------

	public static void setActivity(Activity act)
	{
		main_activity = (GL2JNIActivity)act;
		main_ime = new SoftKeyboard(main_activity);

		android.view.Display display = ((android.view.WindowManager) main_activity.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
	}

	public static int scnW()
	{
		int ret = main_activity.getResources().getDisplayMetrics().widthPixels;
		return ret;
	}

	public static int scnH()
	{
		int ret = main_activity.getResources().getDisplayMetrics().heightPixels;
		return ret;
	}

	public static float scaledDensity()
	{
		float ret = main_activity.getResources().getDisplayMetrics().scaledDensity;
		return ret;
	}
	public static int scaledBase2(int v)
	{
		// scaledDensity base 2.0F(nexus 4)
		float scl = main_activity.getResources().getDisplayMetrics().scaledDensity;
		int ret = (int)(scl * v / 2.0F);
		return ret;
	}

	public static int glVersion()
	{
		ActivityManager am  = (ActivityManager)main_activity.getSystemService(Context.ACTIVITY_SERVICE);
		android.content.pm.ConfigurationInfo inf = am.getDeviceConfigurationInfo();
		int ver = inf.reqGlEsVersion >>16;
		return ver;
	}

	public static int isActive()
	{
		ActivityManager am  = (ActivityManager)main_activity.getSystemService(Context.ACTIVITY_SERVICE);
		List<android.app.ActivityManager.RunningTaskInfo> info;
		info = am.getRunningTasks(1);
		if(info.size() == 0)
			return APPCONST.NSDK_FALSE;

		android.app.ActivityManager.RunningTaskInfo runningTaskInfo = info.get(0);
		if(runningTaskInfo.topActivity.getPackageName().equals(main_activity.getPackageName()))
		{
			return APPCONST.NSDK_TRUE;
		}

		return APPCONST.NSDK_FALSE;
	}

	public static long getFreeDiskSpace()								// available free external disk space.
	{
		android.os.StatFs stat = new android.os.StatFs(Environment.getExternalStorageDirectory().getPath());
		long space = (long)stat.getBlockSize() *(long)stat.getBlockCount();
		//long megAvailable = space / 1048576;
		return space;
	}

	public static void navigationBarInit()
	{
		if(VERSION_CODES.KITKAT <= VERSION.SDK_INT)
		{
			android.view.View  decorView = main_activity.getWindow().getDecorView();
			decorView.setOnSystemUiVisibilityChangeListener(
			new View.OnSystemUiVisibilityChangeListener()
			{
				@Override public void onSystemUiVisibilityChange(int visibility)
				{
					//LOGW("NAVIGAION BAR::VISIBILITY:: " + visibility);
					if(View.SYSTEM_UI_FLAG_VISIBLE == visibility)
					{
						Handler h = main_activity.getWindow().getDecorView().getHandler();
						if(null != h)
						{
							h.postDelayed(new Runnable()
							{
								int bStart = 0;
								@Override public void run()
								{
									if(0 != bStart)
										return;

									bStart = 1;
									navigationBarHide();
								}
							}, 1400);
						}
					}
				}
			});
		}
		navigationBarHide();
	}

	public static void navigationBarHide()
	{
		//int visibility =	View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
		//					View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN ;

		int visibility = 0x00001000 | 0x00000100 | 0x00000200 | 0x00000400 | 0x00000002 | 0x00000004 ;
		if(VERSION_CODES.KITKAT > VERSION.SDK_INT)
		{
			return;
		}
		android.view.View  decorView = main_activity.getWindow().getDecorView();
		decorView.setSystemUiVisibility(visibility);
	}

//------------------------------------------------------------------------------

	public static int config_ConnectWIFI()
	{
		try
		{
			ConnectivityManager conMan = (ConnectivityManager)main_activity.getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo         wifi   = conMan.getNetworkInfo(ConnectivityManager.TYPE_WIFI);

			if(null == wifi)
				return APPCONST.NSDK_FALSE;

			if(wifi.isConnected())
			{
				return APPCONST.NSDK_TRUE;
			}
		}
		catch (NullPointerException e)
		{
			return APPCONST.NSDK_FALSE;
		}

		return APPCONST.NSDK_FALSE;
	}

	public static int config_ConnectOnline()
	{
		try
		{
			ConnectivityManager conMan = (ConnectivityManager)main_activity.getSystemService(Context.CONNECTIVITY_SERVICE);
			State               wifi   = conMan.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState(); // wifi

			if(wifi == NetworkInfo.State.CONNECTED || wifi == NetworkInfo.State.CONNECTING)
				return APPCONST.NSDK_TRUE;

			State mobile = conMan.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState(); // mobile ConnectivityManager.TYPE_MOBILE

			if(mobile == NetworkInfo.State.CONNECTED || mobile == NetworkInfo.State.CONNECTING)
				return APPCONST.NSDK_TRUE;

		}
		catch (NullPointerException e)
		{
			return APPCONST.NSDK_FALSE;
		}

		return APPCONST.NSDK_FALSE;
	}
//------------------------------------------------------------------------------

	public static class WebviewBridge
	{
		@JavascriptInterface public void ToWebViewMessage(final String s_ret)
		{
			agreementViewH.post(new Runnable()
			{
				int bStart = 0;
				public void run()
				{
					if(0 != bStart)
						bStart = 1;

					//LOGW("WEBVIEW:: FROM Web: " + s_ret);
					if( s_ret.equals("YES") )
					{
						LOGW("WEBVIEW:: From Web: YES");

						destroyAgreementView();
						GL2JNIActivity.retShowAgreementView(APPCONST.NSDK_TRUE);
						return;
					}
					else if( s_ret.equals("NO") )
					{
						LOGW("WEBVIEW:: From Web: NO");
						return;
					}
					return;
				}
			});
		}
	}

	synchronized public static String agreementViewURL()
	{
		String url = "file:///android_asset/agreement/kr/agreement.html";

		//int country_code;
		//switch()
		//{
		//}

		return url;
	}

	synchronized public static void prepareAgreement()
	{
		main_activity.runOnUiThread( new Runnable()
		{
			int bStart = 0;
			@Override public void run()
			{
				if(0 != bStart)
					return;

				bStart = 1;
				
				if(null == agreementViewO)
				{
					LOGD("prepareAgreement  1 ------------------------------------------------");

					if(null != agreementViewO)
						return;
			
					LOGD("prepareAgreement  2 ------------------------------------------------");
			
			
					final String url = agreementViewURL();
		
					agreementViewO = new WebView(main_activity);

					int   scn_w = AppUtil.scnW();
					int   scn_h = AppUtil.scnH();
		
					int PIC_WIDTH= agreementViewO.getRight()-agreementViewO.getLeft();
					double val = (double)(scn_w)/PIC_WIDTH;
		
		
					agreementViewO.setPadding(0, 0, 0, 0);
					agreementViewO.getSettings().setLoadWithOverviewMode(true);
					agreementViewO.getSettings().setUseWideViewPort(true);
		
					agreementViewO.setLayerType(WebView.LAYER_TYPE_SOFTWARE, null);
					agreementViewO.setBackgroundColor(0);
		
					LinearLayout.LayoutParams wparam = new LinearLayout.LayoutParams(scn_w, scn_h, 1.0F);
					agreementViewO.setLayoutParams(wparam);
					agreementViewO.setScrollContainer(true);
					agreementViewO.getSettings().setJavaScriptEnabled(true);
					agreementViewO.setWebChromeClient(new WebChromeClient());
		
					agreementViewO.loadUrl(url);
					agreementViewO.addJavascriptInterface(new WebviewBridge(), "android");
					agreementViewO.setFocusable(true);
					agreementViewO.getSettings().setSupportZoom(true);
					agreementViewO.getSettings().setBuiltInZoomControls(true);
					agreementViewO.getSettings().setDisplayZoomControls(false);
				}
			}
		});
	}


	synchronized public static void showAgreementView()
	{
		main_activity.runOnUiThread( new Runnable()
		{
			int bStart = 0;
			@Override public void run()
			{
				if(0 != bStart)
					return;

				bStart = 1;
				
				if(null == agreementViewO)
					prepareAgreement();

				main_activity.addContentView(agreementViewO, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
			}
		});
	}

	synchronized public static void destroyAgreementView()
	{
		main_activity.runOnUiThread( new Runnable()
		{
			int bStart = 0;
			@Override public void run()
			{
				if(0 != bStart)
					return;

				bStart = 1;
				if(null != agreementViewO)
				{
					ViewGroup parent = (ViewGroup)agreementViewO.getParent();
					if(null != parent)
						parent.removeView(agreementViewO);

					LOGI("destroyAgreementView  ------------------------------------------------");

					agreementViewO.setFocusable(false);
					agreementViewO.setVisibility(View.GONE);
					agreementViewO.stopLoading();
					agreementViewO.clearCache(true);
					agreementViewO.clearHistory();
					agreementViewO.removeAllViews();
					agreementViewO.destroy();
					agreementViewO = null;
				}
			}
		});

	}


	//------------------------------------------------------------------------------
	// SoftKeyboard interface
	//------------------------------------------------------------------------------

	synchronized public static int getSoftKeyboardState()
	{
		if(null == main_ime || null == main_ime.ime_dev)
			return APPCONST.NSDK_FALSE;

		return APPCONST.NSDK_TRUE;
	}

	synchronized public static int showSoftKeyboard(   final long callerFunc
										, final long callerOwner
										, final long callerId
										, final int maxLen, final int multi)
	{
		int st = getSoftKeyboardState();
		if(APPCONST.NSDK_FALSE != st)			// not inactive
			return APPCONST.NSDK_EFAIL;

		main_activity.runOnUiThread(new Runnable()
		{
			int bStart = 0;
			@Override public void run()
			{
				if(0 != bStart)
					return;

				bStart = 1;
				int ret = 0;

				if(null == main_ime)
				{
					main_ime = new SoftKeyboard(main_activity);
				}

				ret = initSoftKeyboard(callerFunc, callerOwner, callerId, maxLen, multi);
				LOGW("SoftKeyboard", "run::ret: " + ret);
            }
		});
		return APPCONST.NSDK_OK;
	}

	synchronized public static int initSoftKeyboard(   final long callerFunc
										, final long callerOwner
										, final long callerId
										, final int maxLen, final int multi)
	{
		if(APPCONST.NSDK_FALSE != getSoftKeyboardState())
			return APPCONST.NSDK_EFAIL;

		GLES20.glFinish();
		main_ime.init(callerFunc, callerOwner, callerId, maxLen, multi);
		return APPCONST.NSDK_OK;
	}

	synchronized public static String stringSoftKeyboard()
	{
		if(null == main_ime || null == main_ime.ime_dev)
			return null;

		String str = main_ime.getEditText();
		return str;
	}

	synchronized public static int releaseSoftKeyboard()
	{
		if(null == main_ime || null == main_ime.ime_dev)
			return APPCONST.NSDK_OK;

		LOGW("SoftKeyboard", "releaseSoftKeyboard");

		main_ime.Destroy();
		//main_ime = null;
		return APPCONST.NSDK_OK;
	}

	synchronized public static int cancelSoftKeyboard()
	{
		if(null == main_ime || null == main_ime.ime_dev)
			return APPCONST.NSDK_OK;

		LOGW("SoftKeyboard", "cancelSoftKeyboard");

		long   callerFunc = main_ime.getCallerFunc();
		long   callerOwner= main_ime.getCallerOwner();
		long   callerId   = main_ime.getCallerId();
		GL2JNIActivity.retShowSoftKeyboard(APPCONST.NSDK_IME_OK, callerFunc, callerOwner, callerId, null);

		main_ime.Destroy();
		//main_ime = null;
		return APPCONST.NSDK_OK;
	}

	public static int cancelSoftKeyboard(float x, float y)
	{
		float complementH = 0;
		if(null == main_ime || null == main_ime.ime_dev)
			return APPCONST.NSDK_FALSE;

		complementH = main_ime.msg_deltaY - main_ime.height_body;
		if(complementH < y)
			return APPCONST.NSDK_FALSE;

		LOGW("SoftKeyboard", "cancelSoftKeyboard");

		long   callerFunc = main_ime.getCallerFunc();
		long   callerOwner= main_ime.getCallerOwner();
		long   callerId   = main_ime.getCallerId();
		GL2JNIActivity.retShowSoftKeyboard(APPCONST.NSDK_IME_OK, callerFunc, callerOwner, callerId, null);

		main_ime.Destroy();
		return APPCONST.NSDK_TRUE;
	}


	//------------------------------------------------------------------------------
	//-- SoftKeyboard
	//------------------------------------------------------------------------------

	public static class SoftKeyboard extends LinearLayout
	{
		private  Activity            ime_act       = null;
		private InputMethodManager   ime_dev       = null;
		private EEEditText           msg_edit      = null;		// edit
		private Button               msg_btn       = null;
		private TextView             msg_zmm       = null;		// layout dummy
		private int                  msg_cntLF     = 0;			// LF counter
		private int                  msg_maxLF     = 2;			// max LF
		private int                  msg_maxLen    = 32;
		private int                  msg_deltaY    = 230;		// Defining default height of keyboard which is equal to 230 dip
		private int                  height_body   = 170;
		private int                  height_edit   = 130;
		private int                  height_delta  = 30;
		private int                  width_rct     = 0;
		private int                  width_edt     = 0;
		private int                  width_btn     = 200;		// send button size
		private int                  width_zmm     = 0;			// dummy
		private int                  width_padding = 7;			// edit padding

		private long                 caller_func   = 0;			// callback function
		private long                 caller_owner  = 0;			// callback owner
		private long                 caller_id     = 0;			// callback id

		public  LinearLayout         layout_body   = null;

		public SoftKeyboard(  Activity act)
		{
			super(act);
			ime_act = act;

			final SoftKeyboard root_kb = this;
			int   scn_w = AppUtil.scnW();
			int   scn_h = AppUtil.scnH();

			height_body = (VERSION_CODES.KITKAT > VERSION.SDK_INT) ? 175 : 180;

			int[] scnInfo = getNavigationBarSize(main_activity);
			if(null != scnInfo && 1 == scnInfo[0])
			{
				width_zmm = (VERSION_CODES.KITKAT > VERSION.SDK_INT) ? 2: scnInfo[1] + 2;
			}

			msg_deltaY = (int)(scn_h*2.5/10);
			width_rct  = scn_w;
			width_edt  = width_rct - scaledBase2(width_btn + width_padding*(4+4) + width_zmm);


			msg_deltaY    = scaledBase2(msg_deltaY   );
			height_body   = scaledBase2(height_body  );
			height_edit   = scaledBase2(height_edit  );
			height_delta  = scaledBase2(height_delta );
			width_rct     = scaledBase2(width_rct    );
			//width_edt     = scaledBase2(width_edt    );
			width_btn     = scaledBase2(width_btn    );
			width_zmm     = scaledBase2(width_zmm    );
			width_padding = scaledBase2(width_padding);


			LOGW(	  " width_rct: " + width_rct
					+ " width_edt: " + width_edt
					+ " width_zmm: " + width_zmm);


			LinearLayout.LayoutParams param_main = new LinearLayout .LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);

			this.setLayoutParams(param_main);
			this.setPadding(0, 0, 0, 0);
			this.setOrientation(LinearLayout.VERTICAL);
			//this.setBackgroundColor(Color.parseColor("#44AA00"));
			this.setAlpha(1.0F);


			layout_body = new LinearLayout(ime_act);
			LinearLayout.LayoutParams param_layout_body = new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
			param_layout_body.height = height_body;
			layout_body.setLayoutParams(param_layout_body);
			layout_body.setPadding(width_padding, scaledBase2(15), width_padding, scaledBase2(0));

			layout_body.setBackgroundColor(Color.parseColor("#FFFFFF"));
			layout_body.setAlpha(1.0F);
			layout_body.setOrientation(LinearLayout.HORIZONTAL);
			layout_body.setY(msg_deltaY - param_layout_body.height);



			msg_btn  = new Button(ime_act);
			msg_btn.setOnClickListener( new View.OnClickListener()
			{
				@Override public void onClick(View v)
				{
					String str        = root_kb.getEditText();
					long   callerFunc = root_kb.getCallerFunc();
					long   callerOwner= root_kb.getCallerOwner();
					long   callerId   = root_kb.getCallerId();

					//LOGD("SoftKeyboard", ">>>>> msg_btn:: " + str);
					GL2JNIActivity.retShowSoftKeyboard(APPCONST.NSDK_IME_OK, callerFunc, callerOwner, callerId, str);
					releaseSoftKeyboard();
				}
			});


			LinearLayout.LayoutParams param_btn = new LinearLayout.LayoutParams(width_btn, scaledBase2(100), 1.0F);
			param_btn.setMargins(width_padding*2, scaledBase2(7), 0, 0);

			msg_btn.setLayoutParams(param_btn);
			msg_btn.setFocusable(true);

			msg_btn.setVisibility(View.VISIBLE);


			String btn_txt = getBtnText();
			msg_btn.setText(btn_txt);

			msg_btn.setTextSize(18);
			//msg_btn.setTextColor(Color.parseColor("#FFFFFF"));
			//msg_btn.setBackgroundColor(Color.parseColor("#777777"));
			msg_btn.setTextColor(Color.parseColor("#ff9000"));
			msg_btn.setBackgroundColor(Color.parseColor("#FFFFFF"));
			msg_btn.setAlpha(1.0F);

			GradientDrawable gd_btn = new GradientDrawable();
			gd_btn.setCornerRadius(5);
			//gd_btn.setColor(Color.parseColor("#DDDDDD"));
			//gd_btn.setStroke(5, Color.parseColor("#BBBBBB"));
			gd_btn.setColor(Color.parseColor("#FFFFFF"));
			gd_btn.setStroke(5, Color.parseColor("#FFFFFF"));

			msg_btn.setBackground(gd_btn);
			msg_btn.setPadding(0,0, 0, 0);

			msg_zmm  = new TextView(ime_act);
			LinearLayout.LayoutParams param_dmm = new LinearLayout.LayoutParams(width_zmm, scaledBase2(80), 1.0F);
			param_dmm.setMargins(0, 0, 0, 0);
			param_dmm.weight = 0.0F;

			msg_zmm.setLayoutParams(param_dmm);
			msg_zmm.setFocusable(false);

			msg_zmm.setVisibility(View.VISIBLE);
			msg_zmm.setText("");
			msg_zmm.setTextSize(18);

			msg_zmm.setTextColor(Color.parseColor("#FFFFFF"));
			msg_zmm.setBackgroundColor(Color.parseColor("#FFFFFF"));
			msg_zmm.setAlpha(1.0F);

			GradientDrawable gd_dmm = new GradientDrawable();
			gd_dmm.setColor(Color.parseColor("#FFFFFF"));
			gd_dmm.setStroke(5, Color.parseColor("#FFFFFF"));
			msg_zmm.setBackground(gd_dmm);
			msg_zmm.setPadding(0, 0, 0, 0);
		}


		public int init(   final long callerFunc, final long callerOwner, final long callerId
						 , final int maxLen, final int multi)
		{
			//LOGD("SoftKeyboard", ">>>>>  init:: ");

			caller_func  = callerFunc;
			caller_owner = callerOwner;
			caller_id    = callerId;

			msg_maxLen   = maxLen;
			if(APPCONST.NSDK_FALSE == multi)
			{
				msg_maxLF    = 1;
				height_edit  = 115;
			}
			else
			{
				msg_maxLF    = 2;
				height_edit  = 130;
			}

			height_edit  = scaledBase2(height_edit  );

			msg_zmm    .setVisibility(View.VISIBLE);	msg_zmm    .setFocusable(true);
			msg_btn    .setVisibility(View.VISIBLE);	msg_btn    .setFocusable(true);
			layout_body.setVisibility(View.VISIBLE);	layout_body.setFocusable(true);
			this       .setVisibility(View.VISIBLE);	this       .setFocusable(true);


			String btn_txt = getBtnText();
			msg_btn.setText(btn_txt);


			int h = layout_body.getHeight();
			int y = msg_deltaY - h;
			if(1 < msg_maxLF)
				layout_body.setY(y);
			else
				layout_body.setY(y+height_delta);


			final SoftKeyboard root_kb = this;
			msg_edit = new EEEditText(ime_act, this);
			msg_edit.setOnEditorActionListener(new TextView.OnEditorActionListener()
			{
				@Override public boolean onEditorAction(TextView v, int actionId, KeyEvent event)
				{
					switch (actionId)
					{
						case EditorInfo.IME_ACTION_DONE       :
						case EditorInfo.IME_ACTION_GO         :
						case EditorInfo.IME_ACTION_NEXT       :
						case EditorInfo.IME_ACTION_NONE       :
						case EditorInfo.IME_ACTION_SEARCH     :
						case EditorInfo.IME_ACTION_SEND       :
						{
							if(1 >= msg_maxLF)
							{
								String str        = root_kb.getEditText();
								long   callerFunc = root_kb.getCallerFunc();
								long   callerOwner= root_kb.getCallerOwner();
								long   callerId   = root_kb.getCallerId();

								//LOGD("SoftKeyboard", ">>>>> msg_btn:: " + str);
								GL2JNIActivity.retShowSoftKeyboard(APPCONST.NSDK_IME_OK, callerFunc, callerOwner, callerId, str);
								releaseSoftKeyboard();
								return true;					// after no processing
							}

							if(msg_maxLF <= (msg_cntLF+1))
								return true;

							//++msg_cntLF;					// move to afterTextChanged
							msg_edit.append("\n");
							return true;					// after no processing
						}
					}
					return false;
				}
			});

			msg_edit.addTextChangedListener(new TextWatcher()
			{
				public void afterTextChanged(Editable s)
				{
					int count = 0;
					int len_cur = 0;
					String str  = null;

					if(null != msg_edit)
					{
						str  = msg_edit.stringSoftKeyboard();
						if(null != str)
							len_cur = str.length();
					}


					if(null != str)
					{
						int len = str.length();

						for(int i=0; i< len; ++i)
						{
							if('\n' == str.charAt(i))
								count++;
						}

						msg_cntLF = count;
						//LOGI("EEEditText",    "afterTextChanged:: string:" + str
						//					+ ":: count lf:" + msg_cntLF);
					}

					int len = msg_edit.getText().length();
					//Editable edt = msg_edit.getText(); Selection.setSelection(edt, len);
					msg_edit.setSelection(len);
				}

				public void beforeTextChanged(CharSequence s, int start, int count, int after)
				{
					//LOGI("EEEditText", "beforeTextChanged::" +s);
				}

				public void onTextChanged(CharSequence s, int start, int before, int count)
				{
					//LOGI("EEEditText", "onTextChanged:: count::" +count);
				}
			});


			LinearLayout.LayoutParams param_edt = new LinearLayout.LayoutParams(width_edt, height_edit, 1.0F);
			param_edt.setMargins(width_padding, 0, 0, 0);
			msg_edit.setLayoutParams(param_edt);

			msg_edit.setFilters(new android.text.InputFilter[] {new android.text.InputFilter.LengthFilter(msg_maxLen)});

			msg_edit.setMinimumWidth(width_edt);
			msg_edit.setMaxWidth(width_edt);

			GradientDrawable gd_edit = new GradientDrawable();
			gd_edit.setColor(Color.parseColor("#FFFFFF"));
			gd_edit.setCornerRadius(8);
			gd_edit.setStroke(5, Color.parseColor("#ff9000"));
			msg_edit.setBackground(gd_edit);

			msg_edit.setSingleLine( 1 < msg_maxLF ? false:true);
			msg_edit.setLines(msg_maxLF);
			msg_edit.setMaxLines(msg_maxLF);
			msg_edit.setVisibility(View.VISIBLE);
			msg_edit.setFocusable(true);
			msg_edit.setCursorVisible(true);
			msg_edit.setText("");

			msg_edit.setRawInputType(	InputType.TYPE_CLASS_TEXT |
										InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS |
										( 1 < msg_maxLF ? InputType.TYPE_TEXT_FLAG_IME_MULTI_LINE : 0x0)
										);

			msg_edit.setImeOptions(EditorInfo.IME_FLAG_NO_FULLSCREEN | EditorInfo.IME_ACTION_DONE);

			keypadHeight();

			ime_dev = (InputMethodManager) ime_act.getApplicationContext().getSystemService(Context.INPUT_METHOD_SERVICE);
			ime_dev.toggleSoftInput(InputMethodManager.SHOW_FORCED, InputMethodManager.HIDE_NOT_ALWAYS);

			Attach();
			ime_act.addContentView(this, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));

			//LOGD("SoftKeyboard", ">>>>> call showSoftInput");
			ime_dev.showSoftInput(msg_edit, InputMethodManager.SHOW_FORCED);
			msg_edit.requestFocus();
			return 0;
		}


		private void keypadHeight()
		{
			final View         root = GL2JNIActivity.main_view;
			final SoftKeyboard layout_main = this;

			root.getViewTreeObserver().addOnGlobalLayoutListener
			(
				new ViewTreeObserver.OnGlobalLayoutListener()
				{
					android.graphics.Rect rc = new android.graphics.Rect();

					@Override public void onGlobalLayout()
					{
						root.getWindowVisibleDisplayFrame(rc);

						int scnH  = root.getRootView().getHeight();

						int deltaY = rc.bottom;

						if( null == layout_main ||
							null == layout_main.ime_dev)
							return;


						if(layout_main.msg_deltaY == deltaY)
						{
							//LOGE("SoftKeyboard", "+++++++++++++++++++++++ same Height:" + msg_deltaY);
							return;
						}

						if(Math.abs(scnH-deltaY)<200 )
						{
							//LOGE("SoftKeyboard", "+++++++++++++++++++++++ not available height");
							return;
						}

						layout_main.msg_deltaY = deltaY;


						int h = layout_main.layout_body.getHeight();
						int y = deltaY - h;

						//LOGE("SoftKeyboard", "+++++++++++++++++++++++ OnGlobalLayoutListener:: " + y);
						Detach();

						layout_main.setVisibility(View.VISIBLE);

						if(1<layout_main.msg_maxLF)
							layout_main.layout_body.setY(y);
						else
							layout_main.layout_body.setY(y+height_delta);

						layout_main.layout_body.setVisibility(View.VISIBLE);
						layout_main.msg_edit.setVisibility(View.VISIBLE);

						layout_main.msg_edit.requestFocus();

						LOGW("SoftKeyboard", ">>>>> scnH: " + scnH
														+ " y: " + y
														+ " sg_editW: " + layout_main.msg_edit.getWidth());

						Attach();
					}
				}
			);
		}

		public void Attach()
		{
			//LOGD("SoftKeyboard", ">>>>> Attach");
			layout_body.addView(msg_edit);
			layout_body.addView(msg_btn);
			layout_body.addView(msg_zmm);
			this.addView(layout_body);
		}

		public void Detach()
		{
			//LOGD("SoftKeyboard", ">>>>> Detach");
			ViewGroup parent = null;

			if(null != msg_edit)
			{
				parent = (ViewGroup)msg_edit.getParent();
				if(null != parent)
					parent.removeView(msg_edit);
			}

			parent = (ViewGroup)msg_btn.getParent();
			if(null != parent)
				parent.removeView(msg_btn);

			parent = (ViewGroup)msg_zmm.getParent();
			if(null != parent)
				parent.removeView(msg_zmm);

			parent = (ViewGroup)layout_body.getParent();
			if(null != parent)
				parent.removeView(layout_body);
		}

		public void Destroy()
		{
			//LOGD("SoftKeyboard", ">>>>> Destroy");
			ViewGroup parent = null;

			if(null == ime_dev)
				return;


			ime_dev.hideSoftInputFromWindow(msg_edit.getWindowToken(), 0);
			Detach();
			parent = (ViewGroup)this.getParent();
			if(null != parent)
				parent.removeView(this);

			if(null != msg_zmm)
			{
				msg_zmm    .setFocusable(false);	msg_zmm    .setVisibility(View.GONE);
			}

			if(null != msg_btn)
			{
				msg_btn    .setFocusable(false);	msg_btn    .setVisibility(View.GONE);
			}

			if(null != msg_edit)
			{
				msg_edit   .setFocusable(false);	msg_edit   .setVisibility(View.GONE);
			}

			if(null != layout_body)
			{
				layout_body.setFocusable(false);	layout_body.setVisibility(View.GONE);
			}

			this       .setFocusable(false);	this       .setVisibility(View.GONE);
			ime_dev  = null;
		}

		public String getEditText()
		{
			if(null == msg_edit)
				return null;

			String str = this.msg_edit.stringSoftKeyboard();
			return str;
		}

		public long getCallerFunc  (){ return caller_func;  }
		public long getCallerOwner (){ return caller_owner; }
		public long getCallerId    (){ return caller_id;    }

		public static String getBtnText()
		{
			String ui_ok = main_activity.getResources().getString(net.netmarble.sdk.R.string.nm_uiview_ok);
			if(null == ui_ok)
				ui_ok = "Confirm";

			return ui_ok;
		}


		class EEEditText extends EditText
		{
			SoftKeyboard root_kb = null;
			public EEEditText(Activity act, SoftKeyboard r){ super(act); root_kb=r; }
			public EEEditText(Activity act, AttributeSet a)  { super(act, a);       }
			@Override public boolean onKeyPreIme(int k, KeyEvent e)
			{
				if( k == KeyEvent.KEYCODE_BACK)
				{
					this.setEnabled(false);
					cancelSoftKeyboard();
					return true;
				}
				else if( k == KeyEvent.KEYCODE_ENTER)
				{
				}
				return super.onKeyPreIme(k, e);
			}

			@Override public boolean onKeyDown(int k, KeyEvent e)
			{
				if( k == KeyEvent.KEYCODE_BACK)
				{
					this.setEnabled(false);
					cancelSoftKeyboard();
					return true;
				}
				else if( k == KeyEvent.KEYCODE_ENTER)
				{
				}
				return super.onKeyDown(k, e);
			}


			public String stringSoftKeyboard()
			{
				String str = this.getText().toString();
				return str;
			}
		}
	}


	//------------------------------------------------------------------------------
	// Navigation bar size
	//------------------------------------------------------------------------------

	public static int[] getNavigationBarSize(Context context)
	{
		int[] ret = null;

		Point appUsableSize  = getAppUsableScreenSize(context);
		Point realScreenSize = getRealScreenSize(context);

		// navigation bar on the right
		if (appUsableSize.x < realScreenSize.x)
		{
			ret = new int[]{1, realScreenSize.x - appUsableSize.x, appUsableSize.y};
		}
		else if (appUsableSize.y < realScreenSize.y)	// navigation bar at the bottom
		{
			ret = new int[]{2, appUsableSize.x, realScreenSize.y - appUsableSize.y};
		}
		else
		{
			ret = new int[]{0,0,0};	// nothing
		}
		return ret;
	}

	public static Point getAppUsableScreenSize(Context context)
	{
		WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
		Display display = windowManager.getDefaultDisplay();
		Point size = new Point();
		display.getSize(size);
		return size;
	}

	public static Point getRealScreenSize(Context context)
	{
		WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
		Display display = windowManager.getDefaultDisplay();
		Point size = new Point();

		if (Build.VERSION.SDK_INT >= 17)
		{
			display.getRealSize(size);
		}
		else if (Build.VERSION.SDK_INT >= 14)
		{
			try
			{
				size.x = (Integer) Display.class.getMethod("getRawWidth").invoke(display);
				size.y = (Integer) Display.class.getMethod("getRawHeight").invoke(display);
			}
			catch (IllegalAccessException e)	{}
			catch (InvocationTargetException e)	{}
			catch (NoSuchMethodException e)		{}
		}
		return size;
	}


	//------------------------------------------------------------------------------
	// remote Image
	//------------------------------------------------------------------------------

	public static Bitmap remoteImagePixel32(final String src_url)
	{
		try
		{
			String url = src_url;

			final URL aURL = new URL(url);
			final URLConnection conn = aURL.openConnection();
			conn.connect();
			final BufferedInputStream bis = new BufferedInputStream(conn.getInputStream());
			final Bitmap bmp_src = BitmapFactory.decodeStream(bis);
			bis.close();

			Bitmap.Config bmp_cfg = bmp_src.getConfig();
			Bitmap bmp_dst = bmp_src;

			if(Bitmap.Config.ARGB_8888 != bmp_cfg)
				bmp_dst = bmp_src.copy(Bitmap.Config. ARGB_8888, false);

			return bmp_dst;
		}
		catch (IOException e)
		{
		}

		LOGW("remoteImagePixel32:: failed:: " + src_url);
		return null;
	}

	public static void remoteImagePixel32Cb(final String src_url, final long cb_func, final long cb_owner)
	{
		new Runnable()
		{
			int bStart = 0;
			@Override public void run()
			{
				if(0 != bStart)
					return;

				bStart = 1;
				Bitmap bmp_dst = null;

				try
				{
					String url = src_url;

					URL aURL = new URL(url);
					URLConnection conn = aURL.openConnection();
					conn.connect();
					BufferedInputStream bis = new BufferedInputStream(conn.getInputStream());
					Bitmap bmp_src = BitmapFactory.decodeStream(bis);
					bis.close();


					if(null != bmp_src)
					{
						Bitmap.Config bmp_cfg = bmp_src.getConfig();
						bmp_dst = bmp_src;

						if(Bitmap.Config.ARGB_8888 != bmp_cfg)
							bmp_dst = bmp_src.copy(Bitmap.Config. ARGB_8888, true);
					}
					else	// redirect
					{
						HttpClient client = new DefaultHttpClient();
						HttpGet    get = new HttpGet(src_url);

						if(null != get)
						{
							HttpResponse rsp = client.execute(get);

							if(null != rsp)
							{
								HttpEntity entity = rsp.getEntity();
								BufferedHttpEntity bhe = new BufferedHttpEntity(entity);

								if(null!= bhe)
								{
									bmp_src = BitmapFactory.decodeStream(bhe.getContent());

									if(null != bmp_src)
									{
										Bitmap.Config bmp_cfg = bmp_src.getConfig();
										bmp_dst = bmp_src;
										if(Bitmap.Config.ARGB_8888 != bmp_cfg)
											bmp_dst = bmp_src.copy(Bitmap.Config. ARGB_8888, true);
									}
								}
							}

							get.abort();
							client.getConnectionManager().shutdown();
						}
					}
				}
				catch (IOException e)
				{
				}

				//if(null != bmp_dst)
				//	LOGI("nsdk_getRemoteImagePixel32("+ src_url +", "+ cb_func +") success");
				//else
				//	LOGW("nsdk_getRemoteImagePixel32("+ src_url +", "+ cb_func +") failed");

				LOGW("remoteImagePixel32Cb("+ src_url +", 0x"+ Long.toHexString(cb_func) +", ox" + Long.toHexString(cb_owner) +")");
				GL2JNIActivity.retRemoteImagePixel32Cb(cb_func, cb_owner, bmp_dst);
            }
		}.run();
	}

	public static String IPAddress(int useIPv4)
	{
		try
		{
			List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
			for (NetworkInterface intf : interfaces)
			{
				List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
				for (InetAddress addr : addrs)
				{
					if(!addr.isLoopbackAddress())
					{
						String sAddr = addr.getHostAddress().toUpperCase();
						boolean isIPv4 = InetAddressUtils.isIPv4Address(sAddr);

						if(APPCONST.NSDK_FALSE == useIPv4)
						{
							if(isIPv4)
								return sAddr;
						}
						else
						{
							if(!isIPv4)
							{
								int delim = sAddr.indexOf('%'); // drop ip6 port suffix
								return delim < 0 ? sAddr : sAddr.substring(0,delim);
							}
						}
					}
				}
			}
		}
		catch (Exception ex){} // for now eat exceptions
		return "";
	}


	public static void Toast(final String message)
	{
		main_activity.runOnUiThread(new Runnable()
		{
			int bStart = 0;
			@Override public void run()
			{
				if(0 != bStart)
					return;

				bStart = 1;
				Toast.makeText(main_activity, message, Toast.LENGTH_SHORT).show();
            }
		});
	}


	//------------------------------------------------------------------------------
	// LOG
	//------------------------------------------------------------------------------

	public static void LOGI(final String message)
	{
		android.util.Log.i(APPCONST.TAG, "-------------:: " + message);
	}

	public static void LOGI(final String tag, final String message)
	{
		android.util.Log.i(tag, "-------------:: " + message);
	}

	public static void LOGD(final String message)
	{
		android.util.Log.d(APPCONST.TAG, "=============:: " + message);
	}

	public static void LOGD(final String tag, final String message)
	{
		android.util.Log.d(tag, "=============:: " + message);
	}

	public static void LOGW(final String message)
	{
		android.util.Log.w(APPCONST.TAG, "$$$$$$$$$$$$$:: " + message);
	}

	public static void LOGW(final String tag, final String message)
	{
		android.util.Log.w(tag, "$$$$$$$$$$$$$:: " + message);
	}

	public static void LOGE(final String message)
	{
		android.util.Log.e(APPCONST.TAG, "@@@@@@@@@@@@@:: " + message);
	}

	public static void LOGE(final String tag, final String message)
	{
		android.util.Log.e(tag, "@@@@@@@@@@@@@:: " + message);
	}

}

