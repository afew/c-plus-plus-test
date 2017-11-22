
package com.netmarble.elemental;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import net.netmarble.Channel;
import net.netmarble.Cipher;
import net.netmarble.CipherType;
import net.netmarble.Configuration;
import net.netmarble.Configuration.Language;

import net.netmarble.EveryNetmarble;
import net.netmarble.EveryNetmarble.EveryNetmarbleProfile;
import net.netmarble.Facebook;
import net.netmarble.Facebook.DeleteInvitersListener;
import net.netmarble.Facebook.FacebookProfile;
import net.netmarble.Facebook.InviteFriendsListener;
import net.netmarble.Facebook.PostPhotoListener;
import net.netmarble.Facebook.PostStatusUpdateListener;
import net.netmarble.Facebook.RequestFriendsListener;
import net.netmarble.Facebook.RequestInvitersListener;
import net.netmarble.Facebook.RequestMyProfileListener;
import net.netmarble.GooglePlus;
import net.netmarble.GooglePlus.GooglePlusProfile;
import net.netmarble.Kakao;
import net.netmarble.Kakao.KakaoProfile;
import net.netmarble.Kakao.PostStoryListener;
import net.netmarble.Kakao.UnregisterListener;
import net.netmarble.Naver;
import net.netmarble.Naver.NaverProfile;

import net.netmarble.Push;
import net.netmarble.Push.UsePush;
import net.netmarble.Push.GetUsePushNotificationListListener;
import net.netmarble.Push.GetUsePushNotificationListener;
import net.netmarble.Push.SendPushNotificationListener;
import net.netmarble.Push.SetUsePushNotificationListener;

import net.netmarble.Result;
import net.netmarble.Session;
import net.netmarble.Session.ChannelConnectOption;
import net.netmarble.Session.ChannelConnectOptionType;
import net.netmarble.Session.ChannelSignInListener;
import net.netmarble.Session.ConnectToChannelListener;
import net.netmarble.Session.DisconnectFromChannelListener;
import net.netmarble.Session.SelectChannelConnectOptionListener;
import net.netmarble.Session.SignInListener;

import net.netmarble.UiView;
import net.netmarble.UiView.CloseGameListener;
import net.netmarble.UiView.CustomerSupportPage;
import net.netmarble.UiView.QuickMenuOptions;
import net.netmarble.UiView.QuickMenutype;
import net.netmarble.UiView.RequestRewardViewDataListener;
import net.netmarble.UiView.RewardViewData;
import net.netmarble.UiView.ShowViewListener;

import net.netmarble.impl.SessionDataManager;
import net.netmarble.impl.SessionImpl;

import net.netmarble.m.billing.raven.Purchase;
import net.netmarble.m.billing.raven.listener.*;
import net.netmarble.m.billing.raven.refer.*;
import net.netmarble.m.billing.raven.sku.SkuConsts;

import android.app.Activity;
import android.app.AlertDialog;

import android.content.*;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.AssetManager;
import android.content.ServiceConnection;
import android.graphics.BitmapFactory;

import android.os.*;
import android.os.Bundle;

import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;

import android.view.*;
import android.view.inputmethod.*;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.View.OnTouchListener;
import android.view.ViewGroup;
import android.view.WindowManager;

import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.android.vending.billing.IInAppBillingService;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.plus.Plus;

import blueberry.google.IabBroadcastReceiver;
import blueberry.google.IabBroadcastReceiver.IabBroadcastListener;
import blueberry.network.data.*;
import blueberry.network.Network;
import blueberry.network.callback.*;

//import com.netmarble.elemental.CustomEditText.OnBackPressListener;
//import com.crittercism.app.Crittercism;


public class GL2JNIActivity extends Activity
{
	public static String         main_external_path              = null;								// project +"/asset"
	public static GL2JNIActivity main_activity                   = null;
	public static Context        main_context                    = null;
	public static AssetManager   main_asset_manager              = null;
	public static GL2JNIView     main_view                       = null;

	public static Session        session                         ;										// netmarble sdk session

	public static String         reward_list                     = null;								// reward data
	public static List<String>   mFacebookInvitee                = new ArrayList<String>();

	public static List<String>   push_send_playerIDList          = new ArrayList<String>();
	public static String         push_send_message               = "";
	public static int            push_send_notificationID        = 0;
	public static int            push_local_notificationID       = 0;
	public static int            push_local_second               = 0;
	public static String         push_local_message              = "";
	public static int            push_local_built_notificationID = 0;

	public static List<ChannelConnectOption> main_channel_connect_option = new ArrayList<ChannelConnectOption>();
	public static int                        main_channel_connect_code   = -1;

	static
	{


		System.loadLibrary("epp-jni");
	}

	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	IInAppBillingService        mService;
	private ServiceConnection   mServiceConn = new ServiceConnection()
	{
		@Override public void onServiceDisconnected(ComponentName name)
		{
			mService = null;
		}

		@Override public void onServiceConnected(ComponentName name, IBinder service)
		{
			mService = IInAppBillingService.Stub.asInterface(service);
		}
	};

	public void callback(String strText)
	{
		new AlertDialog.Builder(this).setMessage(strText).show();
	};


	//--------------------------------------------------------------------------
//--java to c++ for game system
	//--------------------------------------------------------------------------

	public native static void create(AssetManager mgr );
	public native static void render();
	public native static void touch(int t_type, float x, float y);


















	//--------------------------------------------------------------------------
	// popup window ...
	//--------------------------------------------------------------------------

	public native static void retShowSoftKeyboard        (int ret, long caller_func, long caller_owner, long caller_id, String str);
	public native static void retRemoteImagePixel32Cb    (long cb_func, long cb_owner, android.graphics.Bitmap bmp);
	public native static void retShowExitView            (int ret);
	public native static void retSignIn                  (int ret);
	public native static void retChannelSignIn           (int ret);
	public native static void retShowQuickMenuView       (int ret);
	public native static void retHideQuickMenuView       (int ret);

	public native static void retShowCouponView          (int ret);
	public native static void retShowFreeChargeView      (int ret);
	public native static void retShowGameReviewView      (int ret);
	public native static void retShowNoticeView          (int ret);
	public native static void retShowPromotionView       (int ret);
	public native static void retShowCustomerSupportView (int ret);
	public native static void retRequestRewardView       (int ret);
	public native static void retShowCafeView            (int ret);
	public native static void retShowAgreementView       (int ret);		// APPCONST.NSDK_TRUE|FALSE
	public native static void retShowPushMessageAll      (int ret);		// APPCONST.NSDK_TRUE|FALSE


	//--------------------------------------------------------------------------
	// configuration, utilities, and popup windows
	//--------------------------------------------------------------------------

	public static int nsdk_glVersion()
	{
		int ret = AppUtil.glVersion();
		return ret;
	}

	public static int nsdk_isActive()
	{
		int ret = AppUtil.isActive();
		return ret;
	}

	public static long nsdk_getFreeDiskSpace()
	{
		long ret = AppUtil.getFreeDiskSpace();
		return ret;
	}

	public static String nsdk_getIPAddress(int useIPv4)
	{
		String ret = AppUtil.IPAddress(useIPv4);
		return ret;
	}

	public static int nsdk_config_ConnectWIFI()
	{
		int ret = AppUtil.config_ConnectWIFI();
		return ret;
	}

	public static int nsdk_config_ConnectOnline()
	{
		int ret = AppUtil.config_ConnectOnline();
		return ret;
	}

	public static int nsdk_showSoftKeyboard(  final long callerFunc, final long callerOwner, final long callerId
											, final int maxLen, final int multi)
	{
		int ret = AppUtil.showSoftKeyboard(callerFunc, callerOwner, callerId, maxLen, multi);
		return ret;
	}

	public static int nsdk_getSoftKeyboardState()
	{
		int ret = AppUtil.getSoftKeyboardState();
		return ret;
	}

	public static void nsdk_hideNavigationBar()
	{
		AppUtil.navigationBarHide();
	}

	public static android.graphics.Bitmap nsdk_getRemoteImagePixel32(final String src_url)
	{
		android.graphics.Bitmap ret= AppUtil.remoteImagePixel32(src_url);
		return ret;
	}

	public static void nsdk_getRemoteImagePixel32Cb(final String src_url, final long cb_func, final long cb_owner)
	{
		AppUtil.remoteImagePixel32Cb(src_url, cb_func, cb_owner);
	}

	public static void nsdk_showToast(final String message)
	{
		AppUtil.Toast(message);
	}
	
	public static void nsdk_prepareAgreementView()
	{
		AppUtil.prepareAgreement();
	}
	
	public static void nsdk_showAgreementView()
	{
		AppUtil.showAgreementView();
	}
	
	public static void nsdk_destroyAgreementView()
	{
		AppUtil.destroyAgreementView();
	}


	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	public static int nsdk_initialize()
	{
		int ret = main_activity.initialize();
		return ret;
	}

	public static void nsdk_showExit()				// exit
	{
		main_activity.showExitView();
	}

	public static void nsdk_signIn()
	{
		main_activity.signIn();
	}

	public static void nsdk_resetSession()
	{
		main_activity.resetSession();
	}

	public static void nsdk_showQuickMenuView(int x, int y, int width, int height, int qmenu)
	{
		main_activity.showQuickMenuView(x, y, width, height, qmenu);
	}

	public static void nsdk_hideQuickMenuView()
	{
		main_activity.hideQuickMenuView();
	}

	public static void nsdk_showCouponView()
	{
		main_activity.showCouponView();
	}

	public static void nsdk_showFreeChargeView()
	{
		main_activity.showFreeChargeView();
	}

	public static void nsdk_showGameReviewView()
	{
		main_activity.showGameReviewView();
	}

	public static void nsdk_showNoticeView(int location)
	{
		main_activity.showNoticeView(location);
	}

	public static void nsdk_showPromotionView(int location)
	{
		main_activity.showPromotionView(location);
	}

	public static void nsdk_removePromotionViewCache()
	{
		main_activity.removePromotionViewCache();
	}


	public static void nsdk_showCustomerSupportView(int page)
	{
		switch(page)
		{
			case APPCONST.CUSTOMERSUPPORTPAGE_HOME          : main_activity.showCustomerSupportView(CustomerSupportPage.Home          ); break;
			case APPCONST.CUSTOMERSUPPORTPAGE_FAQ           : main_activity.showCustomerSupportView(CustomerSupportPage.FAQ           ); break;
			case APPCONST.CUSTOMERSUPPORTPAGE_INQUIRY       : main_activity.showCustomerSupportView(CustomerSupportPage.Inquiry       ); break;
			case APPCONST.CUSTOMERSUPPORTPAGE_GUIDE         : main_activity.showCustomerSupportView(CustomerSupportPage.Guide         ); break;
			case APPCONST.CUSTOMERSUPPORTPAGE_INQUIRYHISTORY: main_activity.showCustomerSupportView(CustomerSupportPage.InquiryHistory); break;
			default: AppUtil.LOGE("nsdk_showCustomerSupportView:: invalid query");
		}
	}

	public static void nsdk_showCafeView(String cafe_id)
	{
		main_activity.showCafeView(cafe_id);
	}

	//--------------------------------------------------------------------------

	public static byte[] nsdk_config_PlayerID()
	{
		byte[] ret = session.getPlayerID().getBytes();
		return ret;
	}

	public static byte[] nsdk_config_GameToken()
	{
		byte[] ret = session.getGameToken().getBytes();
		return ret;
	}

	protected static String getStringChannelID(int channel_code)
	{
		String ret = null;
		switch(channel_code)
		{
			case APPCONST.NSDK_CHANNEL_FACEBOOK       : ret = session.getChannelID(net.netmarble.Channel.Facebook      );
			case APPCONST.NSDK_CHANNEL_EVERY_NETMARBLE: ret = session.getChannelID(net.netmarble.Channel.EveryNetmarble);
			case APPCONST.NSDK_CHANNEL_GOOGLE_PLUS    : ret = session.getChannelID(net.netmarble.Channel.GooglePlus    );
			case APPCONST.NSDK_CHANNEL_KAKAO          : ret = session.getChannelID(net.netmarble.Channel.Kakao         );
			case APPCONST.NSDK_CHANNEL_NAVER          : ret = session.getChannelID(net.netmarble.Channel.Naver         );
			default: AppUtil.LOGE("getStringChannelID:: invalid channel");
		}
		return ret;
	}

	public static byte[] nsdk_config_CountryCode()
	{
		byte[] ret = session.getCountryCode().getBytes();
		return ret;
	}

	public static byte[] nsdk_config_GameCode()
	{
		byte[] ret = Configuration.getGameCode().getBytes();
		return ret;
	}

	public static byte[] nsdk_config_Zone()
	{
		byte[] ret = Configuration.getZone().getBytes();
		return ret;
	}

	public static byte[] nsdk_config_Market()
	{
		byte[] ret = Configuration.getMarket().getBytes();
		return ret;
	}

	public static int nsdk_config_UseLog()
	{
		int ret = Configuration.getUseLog() ? 1: 0;
		return ret;
	}

	public static int nsdk_config_HttpTimeOutSec()
	{
		int ret = Configuration.getHttpTimeOutSec();
		return ret;
	}

	public static int nsdk_config_MaxGameLogDataCount()
	{
		int ret = Configuration.getMaxGameLogDataCount();
		return ret;
	}

	public static int nsdk_config_SendGameLogDataIntervalMin()
	{
		int ret = Configuration.getSendGameLogDataIntervalMin();
		return ret;
	}

	public static int nsdk_config_isGlobalGame()
	{
		int ret = APPCONST.NSDK_TRUE;//Configuration.isGlobalGame() ? APPCONST.NSDK_TRUE : APPCONST.NSDK_FALSE;
		return ret;
	}

	public static byte[] nsdk_config_RegionMode()
	{
		byte[] ret = Configuration.getRegionMode().getBytes();
		return ret;
	}


	//--------------------------------------------------------------------------
	// Channel, Facebook, GooglePlus, EveryNetmarble process
	//--------------------------------------------------------------------------

	public native static void retChannelIDClear ();
	public native static void retChannelIDAdd   (int channel, String channelID);

	public native static void retConnectToChannel(int channel, int isEnable);
	public static void      nsdk_connectToChannel(int channel)
	{
		switch(channel)
		{
			case APPCONST.NSDK_CHANNEL_EVERY_NETMARBLE: main_activity.connectToChannel(Channel.EveryNetmarble); break;
			case APPCONST.NSDK_CHANNEL_FACEBOOK       : main_activity.connectToChannel(Channel.Facebook      ); break;
			case APPCONST.NSDK_CHANNEL_KAKAO          : main_activity.connectToChannel(Channel.Kakao         ); break;
			case APPCONST.NSDK_CHANNEL_GOOGLE_PLUS    : main_activity.connectToChannel(Channel.GooglePlus    ); break;
			case APPCONST.NSDK_CHANNEL_NAVER          : main_activity.connectToChannel(Channel.Naver         ); break;
			default: AppUtil.LOGD("nsdk_connectToChannel:: invalid channel");
		}
	}

	public native static void retDisconnectFromChannel(int channel, int isEnable);
	public static void      nsdk_disconnectFromChannel(int channel)
	{
		switch(channel)
		{
			case APPCONST.NSDK_CHANNEL_EVERY_NETMARBLE: main_activity.disconnectFromChannel(Channel.EveryNetmarble); break;
			case APPCONST.NSDK_CHANNEL_FACEBOOK       : main_activity.disconnectFromChannel(Channel.Facebook      ); break;
			case APPCONST.NSDK_CHANNEL_KAKAO          : main_activity.disconnectFromChannel(Channel.Kakao         ); break;
			case APPCONST.NSDK_CHANNEL_GOOGLE_PLUS    : main_activity.disconnectFromChannel(Channel.GooglePlus    ); break;
			case APPCONST.NSDK_CHANNEL_NAVER          : main_activity.disconnectFromChannel(Channel.Naver         ); break;
			default: AppUtil.LOGD("nsdk_disconnectFromChannel:: invalid channel");
		}
	}

	public native static void retChannelConnectOption(int channel, int ret);
	public static void      nsdk_channelConnectOption(int location)
	{
		main_activity.channelConnectOption(location);
	}


	public native static void retRequestFacebookMyProfile(int ret
														, byte[] playerID
														, byte[] facebookID, byte[] name
														, byte[] profileImageURL, byte[] profileThumbnailURL);
	public static void      nsdk_requestFacebookMyProfile()
	{
		main_activity.requestFacebookMyProfile();
	}

	public native static void retRequestFacebookFriends(int ret);
	public native static void retRequestFacebookFriendsList(  int idx
															, byte[] playerID
															, byte[] facebookID, byte[] name
															, byte[] profileImageURL, byte[] profileThumbnailURL);
	public static void      nsdk_requestFacebookFriends()
	{
		main_activity.requestFacebookFriends();
	}

	public native static void retInviteFacebookFriends(int ret);
	public native static void retInviteFacebookFriendsList(int idx, byte[] facebookID);
	public static void      nsdk_inviteFacebookFriends(String message, String title)
	{
		main_activity.inviteFacebookFriends(message, title);
	}

	public native static void retRequestFacebookInviters(int ret);
	public native static void retRequestFacebookInvitersList( int idx
															, byte[] playerID
															, byte[] facebookID, byte[] name
															, byte[] profileImageURL, byte[] profileThumbnailURL);
	public static void      nsdk_requestFacebookInviters()
	{
		main_activity.requestFacebookInviters();
	}

	public native static void retDeleteFacebookInviters(int ret);
	public static void      nsdk_deleteFacebookInviters()
	{
		main_activity.deleteFacebookInviters();
	}

	public native static void retPostFacebookPhoto(int ret);
	public static void      nsdk_postFacebookPhoto(int[] colors, int width, int height)
	{
		android.graphics.Bitmap image = android.graphics.Bitmap.createBitmap(colors, width, height, android.graphics.Bitmap.Config.ARGB_8888);
		main_activity.postFacebookPhoto(image);
	}

	public native static void retPostFacebookStatusUpdate(int ret);
	public static void      nsdk_postFacebookStatusUpdate(String message, String name, String picture, String link, String caption, String description)
	{
		//String message     = "세기말, 인류에게 희망을 전하는 퀵보이로 태어나다. 함께 할래?";
		//String name        = "12 스테이지 멋지게 클리어!";
		//String picture     = "http://quickboy.gcdn.netmarble.com/quickboy/image/stage/stage14.jpg";
		//String link        = "http://www.netmarble.net";
		//String caption     = "QuickBoy";
		//String description = "2014-11-18";
		main_activity.postFacebookStatusUpdate(message, name, picture, link, caption, description);
	}

	//--------------------------------------------------------------------------

	public native static void retRequestGooglePlusMyProfile(int ret
														  , byte[] playerID
														  , byte[] googlePlusID, byte[] nickName
														  , byte[] profileImageURL, int gender);
	public static void      nsdk_requestGooglePlusMyProfile()
	{
		main_activity.requestGooglePlusMyProfile();
	}

	public native static void retRequestGooglePlusBuddies       ( int ret);
	public native static void retRequestGooglePlusBuddiesList   ( int idx
																, byte[] playerID
																, byte[] googlePlusID, byte[] nickName
																, byte[] profileImageURL, int gender);
	public native static void retRequestGooglePlusAppBuddiesList(  int idx
																, byte[] playerID
																, byte[] googlePlusID, byte[] nickName
																, byte[] profileImageURL, int gender);
	public static void      nsdk_requestGooglePlusFriends()
	{
		main_activity.requestGooglePlusFriends();
	}

	//--------------------------------------------------------------------------

	public native static void retRequestEveryNetmarbleMyProfile(  int ret
																, byte[] playerID
																, byte[] everyNetmarbleID, byte[] nickName
																, byte[] statusMessage
																, byte[] profileImageURL, byte[] thumbnailURL);
	public static void      nsdk_requestEveryNetmarbleMyProfile()
	{
		main_activity.requestEveryNetmarbleMyProfile();
	}

	public native static void retRequestEveryNetmarbleBuddies       ( int ret);
	public native static void retRequestEveryNetmarbleBuddiesList   ( int idx
																	, byte[] playerID
																	, byte[] everyNetmarbleID, byte[] nickName
																	, byte[] statusMessage
																	, byte[] profileImageURL, byte[] thumbnailURL);
	public native static void retRequestEveryNetmarbleAppBuddiesList(  int idx
																	, byte[] playerID
																	, byte[] everyNetmarbleID, byte[] nickName
																	, byte[] statusMessage
																	, byte[] profileImageURL, byte[] thumbnailURL);
	public static void      nsdk_requestEveryNetmarbleFriends()
	{
		main_activity.requestEveryNetmarbleFriends();
	}



	//--------------------------------------------------------------------------
	// push message process
	//--------------------------------------------------------------------------

	// setup the playerID list, message, and notification ID
	public static void      nsdk_setSendPushPlayerIDClear()
	{
		push_send_playerIDList.clear();
	}

	public static void      nsdk_setSendPushPlayerIDAdd(String playerID)
	{
		push_send_playerIDList.add(playerID);
	}

	public static void      nsdk_setSendPushNotificationID(int notificationID)
	{
		push_send_notificationID = notificationID;
	}

	public static void      nsdk_setSendPushMessage(String message)
	{
		push_send_message = message;
		AppUtil.LOGI("nsdk_setSendPushMessage: " +  push_send_message);
	}


	public native static void retSendPushNotification(int ret);
	public static void      nsdk_sendPushNotification()
	{
		main_activity.sendPushNotification();
	}


	public native static void retSendPushNotificationUseNotificationID(int ret);
	public static void      nsdk_sendPushNotificationUseNotificationID()
	{
		main_activity.sendPushNotificationUseNotificationID();
	}


	public native static void retSetUsePushNotification(int ret);
	public static void      nsdk_setUsePushNotification(int bUse)
	{
		boolean use = (0 == bUse) ? false: true;
		main_activity.setUsePushNotification(use);
	}

	public native static void retGetUsePushNotification(int ret, int use);
	public static void      nsdk_getUsePushNotification()
	{
		main_activity.getUsePushNotification();
	}

	//--------------------------------------------------------------------------

	public static void      nsdk_setPushLocalNotificationID(int notificationID)
	{
		push_local_notificationID = notificationID;
	}

	public static int       nsdk_getPushLocalNotificationID()
	{
		return push_local_notificationID;
	}

	public static void      nsdk_setPushLocalSecond(int sec)
	{
		push_local_second = sec;
	}

	public static int       nsdk_getPushUseLocalSecond()
	{
		return push_local_second;
	}

	public static void      nsdk_setPushLocalMessage(String message)
	{
		push_local_message = message;
		AppUtil.LOGI("push local message:" + push_local_message);
	}

	public static int       nsdk_setLocalNotification()
	{
		int ret = main_activity.setLocalNotification();
		return ret;
	}


	public native static void retSetUseLocalNotification(int ret);
	public static void      nsdk_setUseLocalNotification(int bUse)
	{
		boolean use = (0 == bUse) ? false: true;
		main_activity.setUseLocalNotification(use);
	}

	public native static void retGetUseLocalPushNotification(int ret);
	public native static void retGetUseLocalPushNotificationList(int idx, int notificationID, int isUse);
	public static void      nsdk_getUseLocalPushNotification()
	{
		main_activity.getUseLocalPushNotification();
	}

	public native static void retCancelLocalNotification(int ret);
	public static void      nsdk_cancelLocalNotification()
	{
		main_activity.cancelLocalNotification();
	}


	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//-- from netmarble sdk example: configuration, initialize...
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	public int initialize()
	{
		AppUtil.LOGI("initialize:: Netmarble SDK Version : " + Configuration.getSDKVersion());

		// manually define...
		//Configuration.setGameCode(gameCode);
		//Configuration.setZone(zone);
		//Configuration.setUseLog(true);
		//Configuration.setMarket("google");
		//Configuration.setRegionMode(regionMode);

		boolean isCreateSession = Session.createSession(GL2JNIActivity.this);
		if(isCreateSession)
		{
			AppUtil.LOGW("initialize: success");

			session = Session.getInstance();
			setChannelSignInListener();
			getUsePushNotification();

			// TODO : 만약 당신이 Region을 자체적으로 판단한다면, 아래 코드를 반드시 추가해아합니다.
			// 1. region 정보 조회
			// String region = session.getRegion();
			// if(TextUtils.isEmpty(region)){
			//
			// //TODO : region 정보를 조회하는 서버에 region 정보를 요청해야합니다.
			//
			// }
			//
			// 2. region 정보 저장
			// String yourRegion = null;
			// session.setRegion(yourRegion);
			getConfiguration();
			return APPCONST.NSDK_OK;
		}

		AppUtil.LOGW("initialize: failed");
		// createSession fail. send to client ...
		return APPCONST.NSDK_EFAIL;
	}

	private String getCustomLanguage()
	{
		String str = Configuration.getCustomLanguage();
		return str;
	}

	private void setLanguage(Language language)
	{
		Configuration.setLanguage(language);
		getLanguage();
	}

	private void getLanguage()
	{
		Language language = Configuration.getLanguage();
		String value = language.getValue();

		if(TextUtils.isEmpty(value))
		{
			value = "Default";
		}

		AppUtil.LOGI("getLanguage::language : " + value);
	}

	private void getConfiguration()
	{
		AppUtil.LOGW("getConfiguration::============================================================================");
		AppUtil.LOGW("   gameCode : "               + Configuration.getGameCode());
		AppUtil.LOGW("   zone : "                   + Configuration.getZone());
		AppUtil.LOGW("   market : "                 + Configuration.getMarket());
		AppUtil.LOGW("   useLog : "                 + Configuration.getUseLog());
		AppUtil.LOGW("   httpTimeOutSec : "         + Configuration.getHttpTimeOutSec());
		AppUtil.LOGW("   maxGameLogDataCount : "    + Configuration.getMaxGameLogDataCount());
		AppUtil.LOGW("   sendGameLogDataInterval: " + Configuration.getSendGameLogDataIntervalMin());
		AppUtil.LOGW("   isGlobalGame : "           + Configuration.isGlobalGame());
		AppUtil.LOGW("   region: "                  + session.getRegion());
		AppUtil.LOGW("   PlayerID : "               + session.getPlayerID());
		AppUtil.LOGW("   UseFixedPlayerID : "       + Configuration.getUseFixedPlayerID());
		AppUtil.LOGW("==============================================================================================");
		getLanguage();
	}

	private void setChannelInfo(Channel channel)
	{
		switch (channel)
		{
			case EveryNetmarble: AppUtil.LOGI("EMAID : "     + session.getChannelID(Channel.EveryNetmarble)); break;
			case Facebook      : AppUtil.LOGI("FBID : "      + session.getChannelID(Channel.Facebook      )); break;
			case GooglePlus    : AppUtil.LOGI("Google+ID : " + session.getChannelID(Channel.GooglePlus    )); break;
			case Kakao         : AppUtil.LOGI("KakaoID : "   + session.getChannelID(Channel.Kakao         )); break;
			case Naver         : AppUtil.LOGI("NaverID: "    + session.getChannelID(Channel.Naver         )); break;
			default:             AppUtil.LOGI("no support: "                                               ); break;
		}
	}

	private synchronized void getChannelInfo()
	{
		AppUtil.LOGI("============================== get channel info BEGIN ========================================");

		String id_fb = session.getChannelID(Channel.Facebook);
		String id_gp = session.getChannelID(Channel.GooglePlus);
		String id_em = session.getChannelID(Channel.EveryNetmarble);

		int cc_fb = Integer.parseInt(Channel.Facebook.getChannelCode() );
		int cc_gp = Integer.parseInt(Channel.GooglePlus.getChannelCode() );
		int cc_em = Integer.parseInt(Channel.EveryNetmarble.getChannelCode() );

		AppUtil.LOGI("FBID      : " + id_fb);
		AppUtil.LOGI("Google+ID : " + id_gp);
		AppUtil.LOGI("EMAID     : " + id_em);

		// remove and add the id to c++.
		retChannelIDClear();

		if(null != id_fb) { AppUtil.LOGI("FACEBOOK:  code : " + cc_fb + ",  " + id_fb); retChannelIDAdd(cc_fb, id_fb); }
		if(null != id_gp) { AppUtil.LOGI("GOOGLE+:   code : " + cc_gp + ",  " + id_gp); retChannelIDAdd(cc_gp, id_gp); }
		if(null != id_em) { AppUtil.LOGI("NETMARBLE: code : " + cc_em + ",  " + id_em); retChannelIDAdd(cc_em, id_em); }

		AppUtil.LOGI("============================== get channel info END ==========================================");
	}


	private void setChannelSignInListener()
	{
		session.setChannelSignInListener(new ChannelSignInListener()
		{
			@Override public void onChannelSignIn(Result result, Channel channel)
			{
				int ret = result.getCode();

				AppUtil.LOGI("setChannelSignInListener:: " + result + ":: " +channel);
				AppUtil.Toast("setChannelSignInListener:: " + result + ":: " +channel);

				//setChannelInfo(channel);
				getChannelInfo();

				if(Result.SUCCESS == ret)
				{
					//이제 채널 관련 API를 사용 할 수 있습니다.
					//ex) Channel.Facebook이 성공이라면, Facebook.requestMyProfile(listener)를 호출 해도 됩니다!

					retChannelSignIn(APPCONST.NSDK_SIGN_IN_TRUE);
					return;
				}

				retChannelSignIn(APPCONST.NSDK_SIGN_IN_EFAIL);

				//채널 사인인 실패 예외처리 하기

				//일반적으로 channelSignIn이 실패하는 경우는 네트워크가 안좋은 상황일 가능 성이 높습니다
				//일부 유저는 오랜기간 게임을 접속하지 않아 채널 accessToken이 만료 될 수 있습니다

				//게임에서 사용하는 채널 인증의 중요도에 따라서 다르게 처리 할 수 있습니다.

				//1. 인증이 반드시 필요한 게임은 사용자에게 알린 후 채널 연결을 재 시도해 주세요.
				//  (1) onChannelSignIn 응답에 false == result.isSuccess() 확인
				//  (2) 유저에게 채널 연결 실패 알림 : ex) Alert Dialog
				//  (3) 알림 확인 후 session.connectToChannel(channel, listener) 호출

				//2. 인증이 반드시 필요하지 않은 게임은 채널 연결 UI를 다시 보여주세요
				//  (1) onChannelSignIn 응답에 false == result.isSuccess() 확인
				//  (2) 채널 연결 UI 표시 ex) 페이스북 로그인을 시도해 주세요!
			}
		});
	}


	private void resetSession()
	{
		session.resetSession();
		getChannelInfo();
		getPlayerIDAndGameTokenAndRegion();
	}


	private void setRegion()
	{
		AppUtil.Toast("Region이 설정되어 있지 않습니다");

		final AlertDialog.Builder alert = new AlertDialog.Builder(GL2JNIActivity.this);
		alert.setCancelable(false);
		alert.setTitle("Set Region");

		final EditText input = new EditText(GL2JNIActivity.this);
		input.setHint("KOREA_REGION");

		alert.setView(input);
		alert.setPositiveButton("Ok", new DialogInterface.OnClickListener()
		{
			public void onClick(DialogInterface dialog, int whichButton)
			{
				String region = input.getText().toString().trim();

				if(TextUtils.isEmpty(region))
				{
					region = "KOREA_REGION";
				}

				session.setRegion(region);				// region set
				signIn();								// finally, region is setting so procedure the signIn
				dialog.dismiss();
			}
		});
		alert.show();
	}

	private void checkRegionModeManually()				// manually RegionMode must be checked
	{
		String regionMode = Configuration.getRegionMode();
		if(regionMode.equals(Configuration.REGION_MODE_MANUALLY))
		{
			String region = session.getRegion();
			if(TextUtils.isEmpty(region))
			{
				// TODO : session.setRegion();
				setRegion();
				return;
			}
		}
		signIn();
	}


	private void signIn()
	{
		SignInListener signInListener = new SignInListener()
		{
			@Override public void onSignIn(Result result, final List<ChannelConnectOption> channelConnectOptionList)
			{
				int ret = result.getCode();

				AppUtil.LOGD("signInListener: " + ret + "     ##############################################################");

				if(Result.SUCCESS == ret)
				{
					AppUtil.Toast("SignIn success");
					AppUtil.LOGW("=================================      SignIn success    =====================================");

					getChannelInfo();
					getPlayerIDAndGameTokenAndRegion();

					getUsePushNotification();

					retSignIn(APPCONST.NSDK_SIGN_IN_TRUE);

					//startStore();								// setup the activity

					return;
				}

				if(Result.CONNECT_CHANNEL_OPTION_CHANNEL_CONNECTION_MODIFIED == ret)
				{
					int optionSize = channelConnectOptionList.size();

					for (int i = 0; i < optionSize; i = i + 2)
					{
						final int fInt = i;
						final CharSequence[] seq = new CharSequence[2];

						for (int j = 0; j < 2; j++)
						{
							ChannelConnectOption option = channelConnectOptionList.get(i + j);

							if(option.getType() == ChannelConnectOptionType.Cancel)
							{
								seq[j]	= option.getChannel() + " PID 로그 아웃. 현 프로그램 PID 사용\n"
										+ "PID: "   + option.getPlayerID()  + "\n"
										+ "CID: "   + option.getChannelID() + "\n"
										+ "Region:" + option.getRegion()    + "\n";

							}
							else if(option.getType() == ChannelConnectOptionType.UpdateChannelConnection)
							{
								seq[j]	= option.getChannel() + " PID 로 현 프로그램 PID 교체\n"
										+ "PID: "   + option.getPlayerID()  + "\n"
										+ "CID: "   + option.getChannelID() + "\n"
										+ "Region:" + option.getRegion()    + "\n";
							}
						}

						AlertDialog.Builder builder = new AlertDialog.Builder(GL2JNIActivity.this);
						builder	.setTitle("Select playerID & channelID")
						.setSingleChoiceItems(seq, -1, new DialogInterface.OnClickListener()
						{
							public void onClick( DialogInterface dialogInterface, int location)
							{
								dialogInterface.dismiss();

								ChannelConnectOption channelConnectOption = channelConnectOptionList.get(location - fInt);

								SelectChannelConnectOptionListener selectChannelConnectOptionListener = new SelectChannelConnectOptionListener()
								{
									@Override public void onSelect(Result result)
									{
										AppUtil.LOGI("selectChannelConnectOptionListener: " + result);

										if(result.isSuccess())
										{
											AppUtil.Toast("selectChannelConnectOption success");

											getChannelInfo();
											getPlayerIDAndGameTokenAndRegion();

											retSignIn(APPCONST.NSDK_SIGN_IN_TRUE);
										}
									}
								};

								session.selectChannelConnectOption(channelConnectOption, selectChannelConnectOptionListener);
							}
						});

						builder.setCancelable(false).create().show();
					}

					AppUtil.LOGW("==============================   NSDK_SIGN_IN_MODIFIED    ===================================");

					getChannelInfo();
					retSignIn(APPCONST.NSDK_SIGN_IN_MODIFIED);
					return;
				}

				AppUtil.LOGW("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   NSDK_SIGN_IN_EFAIL   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

				getChannelInfo();
				retSignIn(APPCONST.NSDK_SIGN_IN_EFAIL);
			}
		};

		Session session = Session.getInstance();
		session.signIn(signInListener);
	}


	private void getPlayerIDAndGameTokenAndRegion()
	{
		String gameToken   = session.getGameToken();
		String playerID    = session.getPlayerID();
		String implPlayer  = SessionImpl.getInstance().getPlayer().toString();
		String countryCode = session.getCountryCode();
		String regionMode  = Configuration.getRegionMode();
		String region      = session.getRegion();

		if(false == regionMode.equals(Configuration.REGION_MODE_NONE))
			region = session.getRegion();

		AppUtil.LOGI("GameToken   : " + gameToken   );
		AppUtil.LOGI("PlayerID    : " + playerID    );
		AppUtil.LOGI("implPlayer  : " + implPlayer  );
		AppUtil.LOGI("CountryCode : " + countryCode );
		AppUtil.LOGI("regionMode  : " + regionMode  );
		AppUtil.LOGI("region      : " + region      );
	}

	private void connectToChannel(final Channel channel)
	{
		final Session session = Session.getInstance();

		ConnectToChannelListener channelLoginListener = new ConnectToChannelListener()
		{
			@Override public void onConnect(Result result, final List<ChannelConnectOption> channelConnectOptionList)
			{
				int channel_code = Integer.parseInt( channel.getChannelCode() );
				int result_code  = result.getCode();
				
				AppUtil.LOGW("connectToChannel:  ###########################################################################");
				AppUtil.LOGW("result_code: " + result_code);
				AppUtil.LOGW("result: " + result);
				AppUtil.LOGW("connectToChannel:  ###########################################################################");

				if(Result.SUCCESS == result_code)
				{
					AppUtil.LOGI("channelLoginListener success: " + channel);
					AppUtil.Toast("login success: " + channel);

					getChannelInfo();
					getPlayerIDAndGameTokenAndRegion();

					AppUtil.LOGW("connectToChannel:  success           ---------------------------------------------------------");
					retConnectToChannel(channel_code, APPCONST.NSDK_TRUE);
					return;
				}


				if( Result.CONNECT_CHANNEL_OPTION_NEW_CHANNELID  == result_code ||
					Result.CONNECT_CHANNEL_OPTION_USED_CHANNELID == result_code)
				{
					main_channel_connect_option.clear();
					main_channel_connect_code = Integer.parseInt( channel.getChannelCode() );

					// TODO : 유저에게 선택할 수 있는 UI를 보여줘야 합니다.
					// 선택된 ChannelConnectOption 값을 selectChannelConnectOption API에 포함하여 호출 합니다.

					int optionSize           = channelConnectOptionList.size();
					final CharSequence[] seq = new CharSequence[optionSize];


					for (int i = 0; i < optionSize; i++)
					{
						ChannelConnectOption option = channelConnectOptionList.get(i);
						main_channel_connect_option.add(option);
					}

					AppUtil.LOGW("==============================================================================================");
					AppUtil.LOGW("connectToChannel:  channel    : " + main_channel_connect_code );
					AppUtil.LOGW("connectToChannel:  option size: " + main_channel_connect_option.size() );
					AppUtil.LOGW("==============================================================================================");
					
					retConnectToChannel(channel_code, APPCONST.NSDK_CHANNEL_EXIST_OFFSET + optionSize);
					return;
				}
					/*
					for (int i = 0; i < optionSize; i++)
					{
						ChannelConnectOption option = channelConnectOptionList.get(i);

						if(0 == i)
						{
							seq[i]  = option.getChannel() + " PID 로그아웃(정보삭제)\n"
									+ "CID: "    + option.getChannelID() + "\n"
									+ "Region: " + option.getRegion()       ;
						}
						else if(1 == i)
						{
							seq[i]  = option.getChannel() + " PID 정보 삭제. 현 프로그램 PID로 채널의 PID 교체\n"
									+ "CID: "    + option.getChannelID() + "\n"
									+ "PID: "    + option.getPlayerID()  + "\n"
									+ "Region: " + option.getRegion()       ;

						}
						else if(2 == i)
						{
							seq[i]  = option.getChannel() + " PID 로 현 프로그램의 PID 교체\n"
									+ "CID: "    + option.getChannelID() + "\n"
									+ "PID: "    + option.getPlayerID()  + "\n"
									+ "Region: " + option.getRegion()       ;
						}
					}


					new AlertDialog.Builder(GL2JNIActivity.this)
					.setTitle("Select playerID & channelID")
					.setMessage("이 계정에 연결된 게임데이터가 있습니다.\n이 계정으로 접속하시겠습니까?")

					.setPositiveButton("불러오기", new DialogInterface.OnClickListener()
					{
						@Override public void onClick(DialogInterface dialog, int which)
						{
							dialog.dismiss();

							int location = 2;
							ChannelConnectOption channelConnectOption = channelConnectOptionList.get(location);

							SelectChannelConnectOptionListener selectChannelConnectOptionListener = new SelectChannelConnectOptionListener()
							{
								@Override public void onSelect(Result result)
								{
									AppUtil.LOGI("selectChannelConnectOptionListener: " + result);

									if(result.isSuccess())
									{
										AppUtil.Toast("selectChannelConnectOption success");
										int channel_code = Integer.parseInt( channel.getChannelCode() );

										getChannelInfo();
										getPlayerIDAndGameTokenAndRegion();

										retConnectToChannel(channel_code, APPCONST.NSDK_TRUE);
									}
								}
							};

							session.selectChannelConnectOption(channelConnectOption, selectChannelConnectOptionListener);
						}
					})
					.setNegativeButton("취소", new DialogInterface.OnClickListener()
					{
						@Override public void onClick(DialogInterface dialog, int which)
						{
							dialog.dismiss();

							int location = 1;
							ChannelConnectOption channelConnectOption = channelConnectOptionList.get(location);

							SelectChannelConnectOptionListener selectChannelConnectOptionListener = new SelectChannelConnectOptionListener()
							{
								@Override public void onSelect(Result result)
								{
									AppUtil.LOGI("selectChannelConnectOptionListener: " + result);

									if(result.isSuccess())
									{
										AppUtil.Toast("selectChannelConnectOption success");
										int channel_code = Integer.parseInt( channel.getChannelCode() );

										getChannelInfo();
										getPlayerIDAndGameTokenAndRegion();

										retConnectToChannel(channel_code, APPCONST.NSDK_FALSE);
									}
								}
							};

							session.selectChannelConnectOption(channelConnectOption, selectChannelConnectOptionListener);
						}
					})

					.show();
					*/

					/*

					if(APPCONST.NSDK_CHANNEL_OPTION_REPLACE == option)
					{
						int location = 2;
						AppUtil.LOGI(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> selectChannelConnectOptionListener location: " + location);

						ChannelConnectOption channelConnectOption = channelConnectOptionList.get(location);
						SelectChannelConnectOptionListener selectChannelConnectOptionListener = new SelectChannelConnectOptionListener()
						{
							@Override public void onSelect(Result result)
							{
								AppUtil.LOGI("selectChannelConnectOptionListener: " + result);

								if(result.isSuccess())
								{
									AppUtil.Toast("selectChannelConnectOption success");
									int channel_code = Integer.parseInt( channel.getChannelCode() );

									getChannelInfo();
									getPlayerIDAndGameTokenAndRegion();

									retConnectToChannel(channel_code, APPCONST.NSDK_TRUE);
								}
							}
						};

						session.selectChannelConnectOption(channelConnectOption, selectChannelConnectOptionListener);
						return;
					}


					AlertDialog.Builder builder = new AlertDialog.Builder(GL2JNIActivity.this);
					builder.setTitle("Select playerID & channelID")
					.setSingleChoiceItems(seq, -1, new DialogInterface.OnClickListener()
					{
						public void onClick(DialogInterface dialogInterface, int location)
						{
							dialogInterface.dismiss();

							AppUtil.LOGI(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> selectChannelConnectOptionListener location: " + location);

							ChannelConnectOption channelConnectOption = channelConnectOptionList.get(location);
							SelectChannelConnectOptionListener selectChannelConnectOptionListener = new SelectChannelConnectOptionListener()
							{
								@Override public void onSelect(Result result)
								{
									AppUtil.LOGI("selectChannelConnectOptionListener: " + result);

									if(result.isSuccess())
									{
										AppUtil.Toast("selectChannelConnectOption success");
										int channel_code = Integer.parseInt( channel.getChannelCode() );

										getChannelInfo();
										getPlayerIDAndGameTokenAndRegion();

										retConnectToChannel(channel_code, APPCONST.NSDK_TRUE);
									}
								}
							};

							session.selectChannelConnectOption(channelConnectOption, selectChannelConnectOptionListener);
						}
					});

					builder.setCancelable(false).create().show();
					*/


				getChannelInfo();
				retConnectToChannel(channel_code, result_code);

				String domain = result.getDomain();
				if(domain.equals(Result.GOOGLEPLUS_DOMAIN))
				{
					// Google Play 서비스 에러입니다.
					// 유저가 취해야할 액션이 알림바에 표시됩니다.
					AppUtil.Toast("connectToChannel::google play error: " + result);

					AppUtil.LOGW("connectToChannel::google play error: " + result);

					return;
				}

				// TODO : network 오류일 가능성이 높습니다.
				// 에러인 상황을 인지 할 수 있도록 유저에게 에러 팝업 노출을 권장 드립니다.
				AppUtil.Toast("connectToChannel::network error" + result);
			}
		};

		session.connectToChannel(channel, channelLoginListener);
	}


	private void disconnectFromChannel(final Channel channel)
	{
		DisconnectFromChannelListener disconnectFromChannelListener = new DisconnectFromChannelListener()
		{
			@Override public void onDisconnect(Result result)
			{
				AppUtil.LOGW ("disconnectFromChannel:: #####################################################################");
				AppUtil.LOGW ("disconnectFromChannel:: channelLogoutListener: " + result);
				AppUtil.Toast("disconnectFromChannel:: " + channel + " :: " +result);

				int channel_code = Integer.parseInt( channel.getChannelCode() );

				getChannelInfo();
				retDisconnectFromChannel(channel_code, APPCONST.NSDK_FALSE);
			}
		};

		Session session = Session.getInstance();
		session.disconnectFromChannel(channel, disconnectFromChannelListener);
	}


	private void channelConnectOption(final int location)
	{
		ChannelConnectOption channelConnectOption = main_channel_connect_option.get(location);

		SelectChannelConnectOptionListener selectChannelConnectOptionListener = new SelectChannelConnectOptionListener()
		{
			@Override public void onSelect(Result result)
			{
				AppUtil.LOGI("selectChannelConnectOptionListener: " + result);

				if(result.isSuccess())
				{
					AppUtil.Toast("selectChannelConnectOption success");
					int channel_code = main_channel_connect_code;

					getChannelInfo();
					getPlayerIDAndGameTokenAndRegion();
					retChannelConnectOption(channel_code, APPCONST.NSDK_TRUE);
				}
				else
				{
					AppUtil.Toast("selectChannelConnectOption failed");
					int channel_code = main_channel_connect_code;

					getChannelInfo();
					getPlayerIDAndGameTokenAndRegion();
					retChannelConnectOption(channel_code, APPCONST.NSDK_FALSE);
				}
			}
		};

		session.selectChannelConnectOption(channelConnectOption, selectChannelConnectOptionListener);
	}


	//--------------------------------------------------------------------------
	// for using Facebook functions
	//--------------------------------------------------------------------------

	private void requestFacebookMyProfile()	// requestMyProfile()
	{
		Facebook.requestMyProfile(new RequestMyProfileListener()
		{
			@Override public void onReceived(Result result, FacebookProfile profile)
			{
				AppUtil.LOGI("Facebook.requestMyProfile: ret:" + result);

				int  ret = result.getCode();
				byte [] playerID            = null;
				byte [] facebookID          = null;
				byte [] name                = null;
				byte [] profileImageURL     = null;
				byte [] profileThumbnailURL = null;

				if(result.isSuccess())
				{
					playerID            = profile.getPlayerID().getBytes();
					facebookID          = profile.getFacebookID().getBytes();
					name                = profile.getName().getBytes();
					profileImageURL     = profile.getProfileImageURL().getBytes();
					profileThumbnailURL = profile.getProfileThumbnailImageURL().getBytes();
				}

				retRequestFacebookMyProfile(ret, playerID, facebookID, name, profileImageURL, profileThumbnailURL);
				AppUtil.LOGI("Facebook.requestMyProfile: " + profile);
				//AppUtil.Toast("requestFacebookMyProfile::" + result + "\nProfile :" + facebookProfile);
			}
		});
	}

	private void requestFacebookFriends() // requestFriends()
	{
		Facebook.requestFriends(new RequestFriendsListener()
		{
			@Override public void onReceived(Result result, List<FacebookProfile> profileList)
			{
				AppUtil.LOGI("Facebook.requestFriends: ret:" + result);

				int ret = result.getCode();
				retRequestFacebookFriends(ret);			// return the result

				if(result.isSuccess())					// add list to c++
				{
					int idx = 0;
					for(FacebookProfile profile : profileList)
					{
						byte [] playerID            = profile.getPlayerID().getBytes();
						byte [] facebookID          = profile.getFacebookID().getBytes();
						byte [] name                = profile.getName().getBytes();
						byte [] profileImageURL     = profile.getProfileImageURL().getBytes();
						byte [] profileThumbnailURL = profile.getProfileThumbnailImageURL().getBytes();

						retRequestFacebookFriendsList(idx, playerID, facebookID, name, profileImageURL, profileThumbnailURL);
						++idx;
					}

					AppUtil.LOGI("Facebook.requestFriends: ProfileList :" + profileList);
					//AppUtil.Toast("requestFacebookFriends::" + result + "\nProfileList :" + profileList);
				}
			}
		});
	}

	// Invite Facebook friends who doesn't play this application.
	private void inviteFacebookFriends(String message, String title) // inviteFriends()
	{
		Facebook.inviteFriends(message, title, new InviteFriendsListener()//Facebook.inviteFriends("Facebook.inviteFriends!!", "Title", new InviteFriendsListener()
		{
			@Override public void onInvite(Result result, List<String> inviteeFacebookIDList)
			{
				AppUtil.LOGI("Facebook.inviteFriends: ret: " + result);

				int ret = result.getCode();
				retInviteFacebookFriends(ret);

				if(result.isSuccess())
				{
					int idx = 0;
					for(String facebookID : inviteeFacebookIDList)
					{
						byte [] inviteeFacebookID   = facebookID.getBytes();

						retInviteFacebookFriendsList(idx, inviteeFacebookID);
						++idx;
					}
					AppUtil.LOGI("Facebook.inviteFriends: inviteFriends: " + inviteeFacebookIDList);
					//AppUtil.Toast("inviteFacebookFriends::onInvite::"" + result + "\nIDList :" + inviteeFacebookIDList);
				}
			}
		});
	}

	//the list of inviter who send invite request to me.
	private void requestFacebookInviters() // requestInviters
	{
		Facebook.requestInviters(new RequestInvitersListener()
		{
			@Override public void onReceived(Result result, List<FacebookProfile> profileList)
			{
				AppUtil.LOGI("Facebook.requestInviters: ret: " + result);

				int ret = result.getCode();
				retRequestFacebookInviters(ret);

				if(result.isSuccess())
				{
					mFacebookInvitee.clear();								// clear the invitee list

					int idx = 0;
					for(FacebookProfile profile : profileList)
					{
						mFacebookInvitee.add(profile.getPlayerID());		// add to invitee list

						byte [] playerID            = profile.getPlayerID().getBytes();
						byte [] facebookID          = profile.getFacebookID().getBytes();
						byte [] name                = profile.getName().getBytes();
						byte [] profileImageURL     = profile.getProfileImageURL().getBytes();
						byte [] profileThumbnailURL = profile.getProfileThumbnailImageURL().getBytes();

						retRequestFacebookInvitersList(idx, playerID, facebookID, name, profileImageURL, profileThumbnailURL);
						++idx;
					}

					AppUtil.LOGI("Facebook.requestInviters: " + profileList);
					//AppUtil.Toast("requestFacebookInviters::onReceived::" + result + "\nProfileList:" + vInviterProfile);
				}
			}
		});
	}

	// Delete inviters in Facebook service.
	private void deleteFacebookInviters() // deleteInviters()
	{
		if (0 == mFacebookInvitee.size())
		{
			AppUtil.LOGI("deleteFacebookInviters: Fail, you have to call first requestFacebookInviters()");
			//AppUtil.Toast("deleteFacebookInviters\n" + "Fail to delete inviters, you have to call first 'getInviters' API");
			return;
		}

		Facebook.deleteInviters(mFacebookInvitee, new DeleteInvitersListener()
		{
			@Override public void onDelete(Result result)
			{
				AppUtil.LOGI("deleteFacebookInviters: " + result);
				//AppUtil.Toast("Facebook.deleteInviters::"+ result);

				mFacebookInvitee.clear();
			}
		});
	}

	private void postFacebookPhoto(android.graphics.Bitmap image) // postPhoto()
	{
		Facebook.postPhoto("postPhoto", image, new PostPhotoListener()
		{
			@Override public void onPost(Result result)
			{
				AppUtil.LOGI("postPhoto: " + result);
				retPostFacebookPhoto(result.getCode());
				//AppUtil.Toast(result);
			}
		});
	}

	private void postFacebookStatusUpdate(String message, String name, String picture, String link, String caption, String description) // postStatusUpdate()
	{
		Facebook.postStatusUpdate(message, name, picture, link, caption, description,new PostStatusUpdateListener()
		{
			@Override public void onPost(Result result)
			{
				AppUtil.LOGI("postFeed: " + result);
				retPostFacebookStatusUpdate(result.getCode());

				//AppUtil.Toast(result);
			}
		});
	}

//	private void postStatusUpdateWithFacebookDialog()
//	{
//		String name = "12 스테이지 멋지게 클리어!";
//		String picture = "http://quickboy.gcdn.netmarble.com/quickboy/image/stage/stage14.jpg";
//		String link = "http://www.netmarble.net";
//		String caption = "QuickBoy";
//		String description = "2015-05-11";
//		Map<String, String> extras = new HashMap<String, String>();
//		// extras.put("to", "facebookID");

//		Facebook.postStatusUpdate(name, picture, link, caption, description, extras, new PostStatusUpdateListener()
//		{
//			@Override public void onPost(Result result)
//			{
//				AppUtil.LOGI("postFeed: " + result);
//				retPostFacebookStatusUpdate(result.getCode());
//				//AppUtil.Toast(result);
//			}
//		});
//	}


	//--------------------------------------------------------------------------
	// GooglePlus functions
	//--------------------------------------------------------------------------

	private void requestGooglePlusMyProfile()
	{
		GooglePlus.requestMyProfile(new net.netmarble.GooglePlus.RequestMyProfileListener()
		{
			@Override public void onReceived(Result result, GooglePlusProfile profile)
			{
				AppUtil.LOGI("GooglePlus.requestMyProfile: ret: " + result);

				int    ret = result.getCode();
				byte [] playerID            = null;
				byte [] googlePlusID        = null;
				byte [] nickName            = null;
				byte [] profileImageURL     = null;
				int     gender              = 2;		// 0: Male, 1: Female, 2: other

				if(result.isSuccess())
				{
					playerID            = profile.getPlayerID()       .getBytes();
					googlePlusID        = profile.getGooglePlusID()   .getBytes();
					nickName            = profile.getNickname()       .getBytes();
					profileImageURL     = profile.getProfileImageURL().getBytes();
					gender              = profile.getGender()         .getValue();
				}

				retRequestGooglePlusMyProfile(ret, playerID, googlePlusID, nickName, profileImageURL, gender);
			}
        });
	}

	private void requestGooglePlusFriends()
	{
		GooglePlus.requestFriends(new net.netmarble.GooglePlus.RequestFriendsListener()
		{
			@Override public void onReceived(Result result, List<GooglePlusProfile> appBuddyList, List<GooglePlusProfile> buddyList)
			{
				AppUtil.LOGI("GooglePlus.requestFriends: ret: " + result);

				int ret = result.getCode();
				retRequestGooglePlusBuddies(ret);

				if(result.isSuccess())
				{
					int idx = 0;
					for(GooglePlusProfile profile : appBuddyList)
					{
						byte [] playerID            = profile.getPlayerID()       .getBytes();
						byte [] googlePlusID        = profile.getGooglePlusID()   .getBytes();
						byte [] nickName            = profile.getNickname()       .getBytes();
						byte [] profileImageURL     = profile.getProfileImageURL().getBytes();
						int     gender              = profile.getGender()         .getValue();		// 0: Male, 1: Female, 2: other

						retRequestGooglePlusAppBuddiesList(idx, playerID, googlePlusID, nickName, profileImageURL, gender);
						++idx;
					}

					AppUtil.LOGI("GooglePlus.requestFriends.AppBuddyList: " + appBuddyList);
					//AppUtil.Toast("GooglePlus.requestFriends.AppBuddyList:\n" + appBuddyList);

					idx = 0;
					for(GooglePlusProfile profile : buddyList)
					{
						byte [] playerID            = profile.getPlayerID()       .getBytes();
						byte [] googlePlusID        = profile.getGooglePlusID()   .getBytes();
						byte [] nickName            = profile.getNickname()       .getBytes();
						byte [] profileImageURL     = profile.getProfileImageURL().getBytes();
						int     gender              = profile.getGender()         .getValue();		// 0: Male, 1: Female, 2: other

						retRequestGooglePlusBuddiesList(idx, playerID, googlePlusID, nickName, profileImageURL, gender);
						++idx;
					}

					AppUtil.LOGI("GooglePlus.requestFriends.BuddyList: " + buddyList);
					//AppUtil.Toast("GooglePlus.requestFriends.BuddyList:\n" + buddyList);
				}
			}
		});
	}


	//--------------------------------------------------------------------------
	// for using Kakao functions
	//--------------------------------------------------------------------------

	private void requestKakaoProfile()
	{
		Kakao.requestMyProfile(new net.netmarble.Kakao.RequestMyProfileListener()
		{
			@Override public void onReceived(Result result, KakaoProfile profile)
			{
				if(result.isSuccess())
				{
					AppUtil.LOGI("Kakao.requestMyProfile\n" + result + "\nProfile :" + profile);
					//AppUtil.Toast("Kakao.requestMyProfile::" + result + "\nProfile :" + profile);
				}
				else
				{
					AppUtil.LOGI("Kakao.requestMyProfile\n" + result);
					//AppUtil.Toast("Kakao.requestMyProfile::" + result);
				}
			}
		});
	}

	private void requestKakaoFriends()
	{
		Kakao.requestFriends(new net.netmarble.Kakao.RequestFriendsListener()
		{
			@Override public void onReceived(Result result, List<KakaoProfile> appBuddyList, List<KakaoProfile> buddyList)
			{
				if(result.isSuccess())
				{
					AppUtil.LOGI("Kakao.requestFriends" + result + "\nappBuddyProfileList :" + appBuddyList + "\nbuddyProfileList :" + buddyList);
					//AppUtil.Toast("Kakao.requestFriends" + result + "\nappBuddyProfileList :" + appBuddyList + "\nbuddyProfileList :" + buddyList);
				}
				else
				{
					AppUtil.LOGI("Kakao.requestFriends\n" + result);
					//AppUtil.Toast("Kakao.requestFriends::" + result);
				}
			}
		});
	}

	private void getExecuteUri()
	{
		String uri = Kakao.getExecuteUrl();
		AppUtil.LOGI("getExecuteUri: " + uri);
		//AppUtil.Toast("Kakao.getExecuteUri: " + uri);
	}

	private void unregisterKakao()
	{
		Kakao.unregister(new UnregisterListener()
		{
			@Override public void onUnregister(Result result)
			{
				AppUtil.LOGI("unregisterKakao\n" + result);
				//AppUtil.Toast("Kakao.unregisterKakao: " + result);
				getChannelInfo();
			}
		});
	}

	private android.graphics.Bitmap getBitmapFromAssets(String imageFileName)
	{
		AssetManager assetManager = getAssets();
		InputStream stream;
		android.graphics.Bitmap bitmap = null;

		try
		{
			stream = assetManager.open(imageFileName);
			bitmap = BitmapFactory.decodeStream(stream);
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}

		return bitmap;
	}

	private void postStory()
	{
		android.graphics.Bitmap bitmap = getBitmapFromAssets("kk_netmarble.jpg");

		Kakao.postStory(bitmap, "http://m.netmarble.net", "Post Story", new PostStoryListener()
		{
			@Override public void onPost(Result result)
			{
				AppUtil.LOGI("sendMessage\n" + result);
				//AppUtil.Toast("Kakao.postStory:: " + result);
			}
		});
	}


	//--------------------------------------------------------------------------
	// for using EveryNetmarble functions
	//--------------------------------------------------------------------------

	private void requestEveryNetmarbleMyProfile()
	{
		EveryNetmarble.requestMyProfile(new net.netmarble.EveryNetmarble.RequestMyProfileListener()
		{
			@Override public void onReceived(Result result, EveryNetmarbleProfile profile)
			{
				AppUtil.LOGI("EveryNetmarble.requestMyProfile: ret: " + result);

				int    ret = result.getCode();
				byte [] playerID            = null;
				byte [] everyNetmarbleID    = null;
				byte [] nickName            = null;
				byte [] statusMessage       = null;
				byte [] imageURL            = null;
				byte [] thumbnailURL        = null;

				if(result.isSuccess())
				{
					playerID            = profile.getPlayerID()                .getBytes();
					everyNetmarbleID    = profile.getEveryNetmarbleID()        .getBytes();
					nickName            = profile.getNickname()                .getBytes();
					statusMessage       = profile.getStatusMessage()           .getBytes();
					imageURL            = profile.getProfileImageURL()         .getBytes();
					thumbnailURL        = profile.getProfileThumbnailImageURL().getBytes();
				}

				retRequestEveryNetmarbleMyProfile(ret, playerID, everyNetmarbleID, nickName, statusMessage, imageURL, thumbnailURL);

				AppUtil.LOGI("EveryNetmarble.requestMyProfile: " + profile);
				//AppUtil.Toast("(getApplicationContext(), result + "\neveryNetmarbleProfile :" + profile);
			}
		});
	}

	private void requestEveryNetmarbleFriends()
	{
		EveryNetmarble.requestFriends(new net.netmarble.EveryNetmarble.RequestFriendsListener()
		{
			@Override public void onReceived(Result result, List<EveryNetmarbleProfile> appBuddyList, List<EveryNetmarbleProfile> buddyList)
			{
				AppUtil.LOGI("EveryNetmarble.requestFriends: ret: " + result);

				int ret = result.getCode();
				retRequestGooglePlusBuddies(ret);

				if(result.isSuccess())
				{
					int idx = 0;
					for(EveryNetmarbleProfile profile : appBuddyList)
					{
						byte [] playerID            = profile.getPlayerID()                .getBytes();
						byte [] everyNetmarbleID    = profile.getEveryNetmarbleID()        .getBytes();
						byte [] nickName            = profile.getNickname()                .getBytes();
						byte [] statusMessage       = profile.getStatusMessage()           .getBytes();
						byte [] imageURL            = profile.getProfileImageURL()         .getBytes();
						byte [] thumbnailURL        = profile.getProfileThumbnailImageURL().getBytes();

						retRequestEveryNetmarbleAppBuddiesList(idx, playerID, everyNetmarbleID, nickName, statusMessage, imageURL, thumbnailURL);
						++idx;
					}

					AppUtil.LOGI("EveryNetmarble.requestFriends.AppBuddyList: " + appBuddyList);
					//AppUtil.Toast("EveryNetmarble.requestFriends.AppBuddyList:\n" + appBuddyList);

					idx = 0;
					for(EveryNetmarbleProfile profile : buddyList)
					{
						byte [] playerID            = profile.getPlayerID()                .getBytes();
						byte [] everyNetmarbleID    = profile.getEveryNetmarbleID()        .getBytes();
						byte [] nickName            = profile.getNickname()                .getBytes();
						byte [] statusMessage       = profile.getStatusMessage()           .getBytes();
						byte [] imageURL            = profile.getProfileImageURL()         .getBytes();
						byte [] thumbnailURL        = profile.getProfileThumbnailImageURL().getBytes();

						retRequestEveryNetmarbleBuddiesList(idx, playerID, everyNetmarbleID, nickName, statusMessage, imageURL, thumbnailURL);
						++idx;
					}

					AppUtil.LOGI("EveryNetmarble.requestFriends.BuddyList: " + buddyList);
					//AppUtil.Toast("EveryNetmarble.requestFriends.BuddyList:\n" + buddyList);
				}
			}
		});
	}


	//--------------------------------------------------------------------------
	// for using Naver functions
	//--------------------------------------------------------------------------

	private void requestNaverProfile()
	{
		Naver.requestMyProfile(new net.netmarble.Naver.RequestMyProfileListener()
		{
			@Override public void onReceived(Result result, NaverProfile profile)
			{
				if(result.isSuccess())
				{
					AppUtil.LOGI("Naver.requestMyProfile::" + result + " :: naverProfile :" + profile);
					AppUtil.Toast("Naver.requestMyProfile:: " + result + "\nnaverProfile :" + profile);
				}
				else
				{
					AppUtil.LOGI("requestMyProfile\n" + result);
					AppUtil.Toast("Naver.requestMyProfile:: " + result);
				}
			}
		});
	}


	//--------------------------------------------------------------------------
	// push sdk
	//--------------------------------------------------------------------------

	private void getUsePushNotification()
	{
		Push.getUsePushNotification(new GetUsePushNotificationListener()
		{
			@Override public void onGet(Result result, boolean isUse)
			{
				int ret = result.getCode();
				int use = isUse? APPCONST.NSDK_TRUE: APPCONST.NSDK_FALSE;
				retGetUsePushNotification(ret, use);
				AppUtil.LOGI("getUsePushNotification: " + result + " ::" + use);
			}
		});
	}

	private void setUsePushNotification(boolean use)
	{
		Push.setUsePushNotification(use, new SetUsePushNotificationListener()
		{
			@Override public void onSet(Result result)
			{
				int ret = result.getCode();
				retSetUsePushNotification(ret);
				AppUtil.LOGI("setUsePushNotification: " + result);
			}
		});
	}

	private void setUseLocalNotification(boolean use)
	{
		Push.setUsePushNotification(use, push_local_notificationID, new SetUsePushNotificationListener()
		{
			@Override public void onSet(Result result)
			{
				int    ret = result.getCode();
				retSetUseLocalNotification(ret);
				AppUtil.LOGI("setUseLocalNotification result " + result);
			}
		});
	}

	private void getUseLocalPushNotification()
	{
		Push.getUsePushNotificationList(new GetUsePushNotificationListListener()
		{
			@Override public void onGet(Result result, List<UsePush> usePushList)
			{
				AppUtil.LOGI("getUsePushNotificationList result " + result + "usePushList : " + usePushList);

				int ret = result.getCode();

				retGetUseLocalPushNotification(ret);		// return the result
				if(result.isSuccess())						// add user list to c++
				{
					int idx = 0;
					for(UsePush v : usePushList)
					{
						int notificationID = v.getNotificationID();
				        int isUse          = v.isUse() ? 1: 0;

				        retGetUseLocalPushNotificationList(idx, notificationID, isUse);
						++idx;
					}
				}
			}
		});
	}

	private void sendPushNotification()
	{
		if("" == push_send_message || 0  == push_send_playerIDList.size())
		{
			retSendPushNotification(APPCONST.NSDK_EFAIL);
			return;
		}

		Push.sendPushNotification(push_send_message, push_send_playerIDList, new SendPushNotificationListener()
		{
			@Override public void onSend(Result result)
			{
				AppUtil.LOGI("sendPushNotification result: " + result);
				//AppUtil.Toast("sendPushNotification result " + result);

				int    ret = result.getCode();
				retSendPushNotification(ret);
			}
		});
	}

	private void sendPushNotificationUseNotificationID()
	{
		if(0  >= push_send_notificationID || "" == push_send_message || 0  == push_send_playerIDList.size())
		{
			retSendPushNotificationUseNotificationID(APPCONST.NSDK_EFAIL);
			return;
		}

		Push.sendPushNotification(push_send_message, push_send_notificationID, push_send_playerIDList, new SendPushNotificationListener()
		{
			@Override public void onSend(Result result)
			{
				AppUtil.LOGI("sendPushNotificationUseNotificationID result: " + result);
				//AppUtil.Toast("sendPushNotificationUseNotificationID result " + result);

				int    ret = result.getCode();

				retSendPushNotificationUseNotificationID(ret);
			}
		});
	}

	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	private int setLocalNotification()
	{
		if( 0>= push_local_notificationID || "" == push_local_message)
			return APPCONST.NSDK_EFAIL;


		// If you want to receive extra Information, set extra information.
		Map<String, Object> extras = new HashMap<String, Object>();
		DateFormat dataFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS", Locale.US);
		String curTime = dataFormat.format(Calendar.getInstance().getTime());

		extras.put("date", curTime);
		extras.put("extras.1.key", "extras.1.value");
		extras.put("extras.2.key", "extras.2.value");

		push_local_built_notificationID = Push.setLocalNotification(push_local_second, push_local_message, push_local_notificationID, "sheep.mp3", extras);

		//push_local_notification = String.valueOf(localNotificationID);

		AppUtil.LOGI("setLocalNotification result. localNotificationID : " + push_local_built_notificationID);
		//AppUtil.Toast("localNotificationID : " + push_local_built_notificationID);

		return APPCONST.NSDK_OK;
	}

	private void cancelLocalNotification()
	{
		if(0 >= push_local_built_notificationID)
		{
			retCancelLocalNotification(APPCONST.NSDK_EFAIL);
			return;
		}

		boolean isCancel = Push.cancelLocalNotification(push_local_built_notificationID);

		AppUtil.LOGI("cancleLocalNotification result. isCancel : " + isCancel);
		//AppUtil.Toast("isCancel : " + isCancel);

		int    ret = isCancel? 1: 0;
		retCancelLocalNotification(ret);
	}


	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	// other ...
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	private void getCipherData(CipherType cipherType)
	{
		//(M)::AFEW::need string;
		AppUtil.LOGW("getCipherData:: need string!!!!!!!!!!!!!!!!!");

		Session session = Session.getInstance();
		Cipher cipher = session.getCipherData(cipherType);

		AppUtil.LOGI("cipher " + cipher);

		if(null == cipher)
		{
			AppUtil.Toast(cipherType + "Try to get cipher. but return null");
			return;
		}

		AppUtil.LOGI("getCipherData::cipherType : " + cipher.getCipherType());
		AppUtil.LOGI("getCipherData::secretKey : " + cipher.getSecretKey());
		AppUtil.LOGI("getCipherData::AESInitialVector : " + cipher.getAESInitialVector());

		//(M)::AFEW::need string;
		String plainText = null;//plainTextEdit.getText().toString();
		String en = cipher.encrypt(plainText);
		String de = cipher.decrypt(en);

		AppUtil.LOGI("getCipherData::Encrypted : " + en);
		AppUtil.LOGI("getCipherData::Decrypted : " + de);
	}

	//--------------------------------------------------------------------------

	private void showExitView()
	{
		//AppUtil.LOGW("showExitView onOpened -----------------------");
		//retShowExitView(APPCONST.NSDK_WINDOW_OPENED);
		//UiView.showExitView(null);

		UiView.showExitView("http://google.com", new CloseGameListener()
		{
			@Override public void onGameClosed()
			{
				AppUtil.LOGW("showExitView onGameClosed");
				retShowExitView(APPCONST.NSDK_WINDOW_TERMINATE);


				// 어플리케이션을 종료 해야합니다.
				// 예제 코드에서는 callback 이 온 후 300ms 후에 어플리케이션을 종료 하게 되어있습니다.
				// 필요에 따라서 아래 방식이 아닌 다른 방식으로 어플리케이션을 종료 할 수 있습니다.
				// ex) activity.finish();

				Activity activity = GL2JNIActivity.this;

				Intent intent = new Intent(Intent.ACTION_MAIN);
				intent.addCategory(Intent.CATEGORY_HOME);
				intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
				activity.startActivity(intent);

				Thread thread = new Thread(new Runnable()
				{
					@Override public void run()
					{
						try
						{
							Thread.sleep(300);
						}
						catch (InterruptedException e)
						{
							e.printStackTrace();
						}

						android.os.Process.killProcess(android.os.Process .myPid());
					}
				});

				thread.start();
			}
		});
	}

	//--------------------------------------------------------------------------

	private void showQuickMenuView(int x, int y, int width, int height, int qmenu)
	{
		QuickMenutype quickmenuType;
		switch(qmenu)
		{
			case APPCONST.NSDK_QMENU_RIGHT_TO_LEFT: quickmenuType = QuickMenutype.RIGHT_TO_LEFT; break;
			case APPCONST.NSDK_QMENU_BOTTOM_TO_TOP: quickmenuType = QuickMenutype.BOTTOM_TO_TOP; break;
			case APPCONST.NSDK_QMENU_LEFT_TO_RIGHT: quickmenuType = QuickMenutype.LEFT_TO_RIGHT; break;
			case APPCONST.NSDK_QMENU_TOP_TO_BOTTOM:
			default                      : quickmenuType = QuickMenutype.TOP_TO_BOTTOM; break;
		}

		String cafeID             = "";
		String facebookFanPageUrl = "https://www.facebook.com/cjgames";
		String facebookFanPageID  =	"164648390251118";

		QuickMenuOptions options = new QuickMenuOptions(quickmenuType, width, height, x, y, cafeID, facebookFanPageUrl, facebookFanPageID);
		options.setEnableCafeView(true);
		options.setEnableMoreGamesView(true);
		options.setEnableCustomerSupportView(true);
		options.setEnableFanPageView(true);
		options.setEnableNoticeView(true);

		UiView.showQuickMenuView(options, new ShowViewListener()
		{
			@Override public void onRewarded() { AppUtil.LOGI("showQuickMenuView onRewarded"); retShowQuickMenuView(APPCONST.NSDK_WINDOW_REWARDED);	}
			@Override public void onOpened()   { AppUtil.LOGI("showQuickMenuView onOpened")  ; retShowQuickMenuView(APPCONST.NSDK_WINDOW_OPENED);	}
			@Override public void onFailed()   { AppUtil.LOGI("showQuickMenuView onFailed")  ; retShowQuickMenuView(APPCONST.NSDK_WINDOW_FAILED);    }
			@Override public void onClosed()   { AppUtil.LOGI("showQuickMenuView onClosed")  ; retShowQuickMenuView(APPCONST.NSDK_WINDOW_CLOSED);    }
		});
	}

	private void hideQuickMenuView()
	{
		UiView.hideQuickMenuView();
	}

	//--------------------------------------------------------------------------

	private void showPromotionView(int location)
	{
		final StringBuffer logStringBuffer = new StringBuffer();
		logStringBuffer.append("eventLocation : ");
		logStringBuffer.append(location);

		AppUtil.LOGI("showPromotionView:: " + logStringBuffer);

		UiView.showPromotionView(location, new ShowViewListener()
		{
			@Override public void onRewarded() { AppUtil.LOGI("showEventView onRewarded"); retShowPromotionView(APPCONST.NSDK_WINDOW_REWARDED); }
			@Override public void onOpened()   { AppUtil.LOGI("showEventView onOpened")  ; retShowPromotionView(APPCONST.NSDK_WINDOW_OPENED);   }
			@Override public void onFailed()   { AppUtil.LOGI("showEventView onFailed")  ; retShowPromotionView(APPCONST.NSDK_WINDOW_FAILED);   }
			@Override public void onClosed()   { AppUtil.LOGI("showEventView onClosed")  ; retShowPromotionView(APPCONST.NSDK_WINDOW_CLOSED);   }
		});
	}

	private void removePromotionViewCache()
	{
		File file = new File(getApplicationContext().getFilesDir().getAbsolutePath());
		File[] fileList = file.listFiles();

		StringBuilder stringBuilder = new StringBuilder();
		stringBuilder.append("Remove Cache Data : ");

		if (null == fileList)
		{
			AppUtil.LOGI(stringBuilder.toString());
			return;
		}

		for(int i=0; i < fileList.length; ++i)
		{
			if (0 == i)
			{
				stringBuilder.append(fileList[i].getName());
			}
			else
			{
				stringBuilder.append(",");
				stringBuilder.append(fileList[i].getName());
			}

			if (true == fileList[i].canRead())
			{
				fileList[i].delete();
			}
		}

		AppUtil.LOGI(stringBuilder.toString());
	}

	//--------------------------------------------------------------------------

	private void showNoticeView(int location)
	{
		final StringBuffer logStringBuffer = new StringBuffer();
		logStringBuffer.append("noticeLocation : ");
		logStringBuffer.append(location);

		AppUtil.LOGI("showNoticeView: " + logStringBuffer.toString());

		UiView.showNoticeView(location, new ShowViewListener()
		{
			@Override public void onRewarded() { AppUtil.LOGI("showNoticeView: onRewarded"); retShowNoticeView(APPCONST.NSDK_WINDOW_REWARDED); }
			@Override public void onOpened()   { AppUtil.LOGI("showNoticeView: onOpened");   retShowNoticeView(APPCONST.NSDK_WINDOW_OPENED);   }
			@Override public void onFailed()   { AppUtil.LOGI("showNoticeView: onFailed");   retShowNoticeView(APPCONST.NSDK_WINDOW_FAILED);   }
			@Override public void onClosed()   { AppUtil.LOGI("showNoticeView: onClosed");   retShowNoticeView(APPCONST.NSDK_WINDOW_CLOSED);   }
		});
	}

	private void showCustomerSupportView(CustomerSupportPage page)
	{
		UiView.showCustomerSupportView(page, new ShowViewListener()
		{
			@Override public void onRewarded() { AppUtil.LOGI("showCustomerSupportView onRewarded"); retShowCustomerSupportView(APPCONST.NSDK_WINDOW_REWARDED); }
			@Override public void onOpened()   { AppUtil.LOGI("showCustomerSupportView onOpened");   retShowCustomerSupportView(APPCONST.NSDK_WINDOW_OPENED);   }
			@Override public void onFailed()   { AppUtil.LOGI("showCustomerSupportView onFailed");   retShowCustomerSupportView(APPCONST.NSDK_WINDOW_FAILED);   }
			@Override public void onClosed()   { AppUtil.LOGI("showCustomerSupportView onClosed");   retShowCustomerSupportView(APPCONST.NSDK_WINDOW_CLOSED);   }
		});
	}

	private void showGameReviewView()
	{
		UiView.showGameReviewView(new ShowViewListener()
		{
			@Override public void onRewarded() { AppUtil.LOGI("showGameReviewView onRewarded"); retShowGameReviewView(APPCONST.NSDK_WINDOW_REWARDED); }
			@Override public void onOpened()   { AppUtil.LOGI("showGameReviewView onOpened");   retShowGameReviewView(APPCONST.NSDK_WINDOW_OPENED);   }
			@Override public void onFailed()   { AppUtil.LOGI("showGameReviewView onFailed");   retShowGameReviewView(APPCONST.NSDK_WINDOW_FAILED);   }
			@Override public void onClosed()   { AppUtil.LOGI("showGameReviewView onClosed");   retShowGameReviewView(APPCONST.NSDK_WINDOW_CLOSED);   }
		});
	}

	private void showCouponView()
	{
		UiView.showCouponView(new ShowViewListener()
		{
			@Override public void onRewarded() { AppUtil.LOGI("showCouponView onRewarded"); retShowCouponView(APPCONST.NSDK_WINDOW_REWARDED); }
			@Override public void onOpened()   { AppUtil.LOGI("showCouponView onOpened");   retShowCouponView(APPCONST.NSDK_WINDOW_OPENED);   }
			@Override public void onFailed()   { AppUtil.LOGI("showCouponView onFailed");   retShowCouponView(APPCONST.NSDK_WINDOW_FAILED);   }
			@Override public void onClosed()   { AppUtil.LOGI("showCouponView onClosed");   retShowCouponView(APPCONST.NSDK_WINDOW_CLOSED);   }
		});
	}

	private void showFreeChargeView()
	{
		UiView.showFreeChargeView(new ShowViewListener()
		{
			@Override public void onRewarded() { AppUtil.LOGI("showFreeCharge onRewarded"); retShowFreeChargeView(APPCONST.NSDK_WINDOW_REWARDED); }
			@Override public void onOpened()   { AppUtil.LOGI("showFreeCharge onOpened");   retShowFreeChargeView(APPCONST.NSDK_WINDOW_OPENED); }
			@Override public void onFailed()   { AppUtil.LOGI("showFreeCharge onFailed");   retShowFreeChargeView(APPCONST.NSDK_WINDOW_FAILED); }
			@Override public void onClosed()   { AppUtil.LOGI("showFreeCharge onClosed");   retShowFreeChargeView(APPCONST.NSDK_WINDOW_CLOSED); }
		});
	}

	private void showCafeView(final String cafeId)
	{
		UiView.showCafeView(cafeId, new ShowViewListener()
		{
			@Override public void onRewarded() { AppUtil.LOGI("showCafeView onRewarded"); retShowCafeView(APPCONST.NSDK_WINDOW_REWARDED); }
			@Override public void onOpened()   { AppUtil.LOGI("showCafeView onOpened");   retShowCafeView(APPCONST.NSDK_WINDOW_OPENED);   }
			@Override public void onFailed()   { AppUtil.LOGI("showCafeView onFailed");   retShowCafeView(APPCONST.NSDK_WINDOW_FAILED);   }
			@Override public void onClosed()   { AppUtil.LOGI("showCafeView onClosed");   retShowCafeView(APPCONST.NSDK_WINDOW_CLOSED);   }
		});
	}

	//--------------------------------------------------------------------------

	private void requestRewardViewData()
	{
		List<String> rewardCodeList = new ArrayList<String>();

		if(null == reward_list || TextUtils.isEmpty(reward_list))
		{
			AppUtil.Toast("rewardCode is null. set test_reward01,test_reward02");
			rewardCodeList.add("test_reward01");
			rewardCodeList.add("test_reward02");
		}
		else
		{
			String[] split = reward_list.replace("\n","").replace(" ","").split(",");

			for (String rewardCode : split)
			{
				rewardCodeList.add(rewardCode);
			}
		}

		UiView.reqeustRewardViewData(rewardCodeList, new RequestRewardViewDataListener()
		{
			@Override public void onReceived(Result result, List<RewardViewData> rewardViewDataList)
			{
				AppUtil.LOGI("reqeustRewardViewData result : " + result + " , rewardViewDataList : " + rewardViewDataList);

				if(result.isSuccess())
				{
					AppUtil.Toast("reqeustRewardViewData result : " + result + " , rewardViewDataList : " + rewardViewDataList);
				}
				else
				{
					AppUtil.Toast("reqeustRewardViewData fail. result : " + result);
				}
			}
		});
	}


	//--------------------------------------------------------------------------

	private void requestRewardView()
	{
		List<String> rewardCodeList = new ArrayList<String>();

		if(null == reward_list || TextUtils.isEmpty(reward_list))
		{
			AppUtil.Toast("rewardCode is null. set test_reward01,test_reward02");
			rewardCodeList.add("test_reward01");
			rewardCodeList.add("test_reward02");
		}
		else
		{
			String[] split = reward_list.replace("\n","").replace(" ","").split(",");

			for (String rewardCode : split)
			{
				rewardCodeList.add(rewardCode);
			}
		}

		UiView.showRewardView(rewardCodeList, new ShowViewListener()
		{
			@Override public void onRewarded() { AppUtil.LOGI("showRewardView onRewarded"); retRequestRewardView(APPCONST.NSDK_WINDOW_REWARDED); }
			@Override public void onOpened()   { AppUtil.LOGI("showRewardView onOpened");   retRequestRewardView(APPCONST.NSDK_WINDOW_OPENED);   }
			@Override public void onFailed()   { AppUtil.LOGI("showRewardView onFailed");   retRequestRewardView(APPCONST.NSDK_WINDOW_FAILED);   }
			@Override public void onClosed()   { AppUtil.LOGI("showRewardView onClosed");   retRequestRewardView(APPCONST.NSDK_WINDOW_CLOSED);   }
		});
	}


	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//-- application
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	@Override public void onWindowFocusChanged(boolean hasFocus)
	{
		if(hasFocus)
		{
			AppUtil.navigationBarHide();
		}
	}

	@Override public void onConfigurationChanged(android.content.res.Configuration newConfig)
	{
		super.onConfigurationChanged(newConfig);
		setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
	}

	@Override protected void onCreate(Bundle savedInstanceState)
	{
		AppUtil.LOGW("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
		AppUtil.LOGW("$$$$$$$$$$$$$$$$$$$$                                                      $$$$$$$$$$$$$$$$$$$$");
		AppUtil.LOGW("$$$$$$$$$$$$$$$$$$$$                  application start                   $$$$$$$$$$$$$$$$$$$$");
		AppUtil.LOGW("$$$$$$$$$$$$$$$$$$$$                                                      $$$$$$$$$$$$$$$$$$$$");
		AppUtil.LOGW("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");

		main_activity      = this;
		AppUtil.setActivity(main_activity);

		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		AppUtil.navigationBarInit();

		main_context       = (Context)this.getApplicationContext();
		main_external_path = Environment.getExternalStorageDirectory() + "/Android/data/" + getApplicationContext().getPackageName();
		main_asset_manager = getAssets();
		main_view          = new GL2JNIView(getApplication(), true, 16, 0);

		setContentView(main_view);


		// start .. nsdk..
		onNewIntent(getIntent());

		nsdk_initialize();
		nsdk_signIn();
	}

	@Override public void onNewIntent(Intent intent)
	{
		Bundle extras = intent.getExtras();
		if(null != extras)
		{
			AppUtil.LOGW( "onNewIntent:: extras: " + extras.toString());
			Set<String> keys = extras.keySet();
			for (String key : keys)
			{
				Object value = extras.get(key);
				AppUtil.LOGW( "onNewIntent:: extras: key : " + key + ", value : " + value);
			}
		}
		else
		{
			AppUtil.LOGW( "onNewIntent:: extras is null");
		}
	}


	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	@Override protected void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(9001 == requestCode && 10001 == resultCode)
		{
			// TODO : disconnectFromChannel();

			// at 3.4.1
			GoogleApiClient googleAPIClient = GooglePlus.getGoogleAPIClient();

			if(null == googleAPIClient)
			{
				// connectToChannel or signIn을 시도하지 않은 케이스 입니다.
				// TODO : 로그인 프로세스 체크
				return;
			}

			if(false == googleAPIClient.isConnected())
			{
				// 연결이 되어 있지 않습니다.
				// TODO : 구글 연결이 되어 있지 않습니다. 채널 연결 프로세스 체크
				return;
			}

			// NetmarbleS 내부에서 사용합니다.
			// 변경 하지 않습니다. 변경하면 문제가 생길 수 있습니다.
			SessionDataManager.setChannelID(getApplicationContext(), Channel.GooglePlus, null);
			SessionDataManager.setChannelToken(getApplicationContext(), Channel.GooglePlus, null);

			// 기본 계정 정보를 초기화 합니다.
			Plus.AccountApi.clearDefaultAccount(googleAPIClient);

			// googlePlus disconnect
			googleAPIClient.disconnect();

			// at 3.5.0
			disconnectFromChannel(Channel.GooglePlus);
		}

		if(null != session)
		{
			session.onActivityResult(requestCode, resultCode, data);
		}

		super.onActivityResult(requestCode, resultCode, data);
	}

	@Override public void onLowMemory()
	{
		AppUtil.LOGW("==============================================================================================");
		AppUtil.LOGW(" Low Memory");
		AppUtil.LOGW("==============================================================================================");
		super.onLowMemory();
	}

	@Override protected void onResume()
	{
		nsdk_hideNavigationBar();

		if(null != session)
		{
			session.onResume();
		}

		super.onResume();
		main_view.onResume();
		//resume();
	}

	@Override protected void onPause()
	{
		AppUtil.releaseSoftKeyboard();
		if(null != session)
		{
			session.onPause();
		}

		super.onPause();
		main_view.onPause();
	}

	@Override protected void onDestroy()
	{
		AppUtil.releaseSoftKeyboard();
		unregistGoogleReceiver();

		if(null != session)
		{
            session.onDestroy();
        }

		//destroy();

		super.onDestroy();
		this.finish();
		android.os.Process.killProcess(android.os.Process.myPid());
		System.exit(0);
	}

	@Override protected void onStop()
	{
		AppUtil.releaseSoftKeyboard();
		if(null != session)
		{
			session.onStop();
		}

		//stop();
		super.onStop();
	}

	@Override public void onBackPressed()
	{
		AppUtil.LOGI("onBackPressed");
		UiView.showExitView(null);
	}

	boolean b_multi_touch = false;
	@Override public boolean dispatchTouchEvent( MotionEvent event )
	{
		// TODO Auto-generated method stub
		//AppUtil.LOGI("dispatchTouchEvent:: touch event");

		float x = event.getX();
		float y = event.getY();

		int keyAction = event.getAction();
		switch(keyAction & MotionEvent.ACTION_MASK)
		{
			case MotionEvent.ACTION_DOWN:
			{
				if(APPCONST.NSDK_FALSE == AppUtil.cancelSoftKeyboard(x, y) )
					touch(MotionEvent.ACTION_DOWN, x, y);

				//AppUtil.LOGI("dispatchTouchEvent:: MotionEvent.ACTION_DOWN");
			}break;

			case MotionEvent.ACTION_UP:
			{
				//AppUtil.LOGI("dispatchTouchEvent:: MotionEvent.ACTION_UP: ");
				touch(MotionEvent.ACTION_UP, x, y);
			}break;
		}

		boolean result =  super.dispatchTouchEvent(event);
		return result;
	}

	@Override public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		return super.onKeyDown(keyCode, event);
	}


	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//-- I.A.P
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	// 게임서버 URI 정보( 게임서버를 통해 빌링서버와 연동해야 함 ) :  IAP Server URI(This should be specified in the game server to interlock with IAP server)
	private static final String GAME_SVR_REGIST = "http://112.175.174.38/P/Billing/v3/Service/Billing/Initialize/?";
	private static final String GAME_SVR_VERIFY = "http://112.175.174.38/P/Billing/v3/Service/Billing/Verify/?";

	// Dummy 테스트를 위한 빌링서버 URI 정보( 게임서버에서 연동해야 함 ) : IAP Server URI for dummy test(This should be specified in the game server to interlock with IAP server)
	private static final String DUMMY_SVR_REGIST = "http://112.175.174.38/P/Billing/dummy/Service/Billing/Initialize/?";
	private static final String DUMMY_SVR_VERIFY = "http://112.175.174.38/P/Billing/dummy/Service/Billing/Verify/?";


	private enum VerifyType { Purchase, Remain };


	private Network              store_network        = null;

	private String               billing_regist       = "";
	private String               billing_verify       = "";

	private IabBroadcastReceiver google_receiver      = null;          // 구글 프로모션 코드 리시버

	private User                 pay_user             = null;          // payment info
	private Regist               pay_regist           = null;
	private PurchaseData         pay_purchase         = null;
	private VerifyType           pay_verify           = null;

	public static String             store_type          = "";            // 스토어 타입( 빌링서버와 공통 사용 ) : Store type(common value for IAP server)
	public static String             store_itemCode      = "";
	public static String             store_accessToken   = "";
	public static boolean            store_verify        = false;
	public static ArrayList<String>  store_item          = null;

	public static String             user_game_code      = "blueberry"  ; // 넷마블에서 발급하는 게임코드 : Game code provided by Netmarble
	public static String             user_key            = "875451212"  ; // 플랫폼 사용자 식별 번호( 카카오톡 회원번호 ) : Platform user ID(ex: KakaoTalk ID)
	public static String             user_platform       = "netmarble"  ; // 플랫폼 코드 ( kakao, netmarble, facebook ) : platform code ( kakao, netmarble, facebook )
	public static String             user_game_seq       = "1232455"    ; // 게임별 사용자 식별 번호 : Game-specific user sequence number
	public static String             user_phone          = "01012345678"; // 사용자 핸드폰 번호 : User's phone number
	public static String             user_ip             = "127.0.0.1"  ; // 사용자 아이피 : User's IP
	public static boolean            google_use          = true         ; // use google


	public int startStore()
	{
		user_game_code = Configuration.getGameCode();
		IAP.setSkuZone("dev");
		IAP.startupSkuService(main_activity, StoreType.GooglePlay, user_game_code);

		store_network  = new Network(main_activity);

		if(store_type.equalsIgnoreCase(StoreType.Dummy.getType()))							// 게임 서버 연동 환경 설정 : Environment configuration for interlocking with the game server
		{
			billing_regist = DUMMY_SVR_REGIST;
			billing_verify = DUMMY_SVR_VERIFY;
		}
		else
		{
			billing_regist = GAME_SVR_REGIST;
			billing_verify = GAME_SVR_VERIFY;
		}


		String userPlatform = user_platform;
		String userKey      = user_key     ;
		String userGameSeq  = user_game_seq;
		String userPhone    = user_phone   ;
		String userIp       = user_ip      ;

		pay_user = new User(userPlatform, userKey, userGameSeq, userPhone, userIp);						// 결제 진행을 위해 사용자 정보 세팅. 아래 데이터 값은 빌링 서버 연동 문서 v3 참고하세요. : Information to be sent to IAP server for purchasing process. Please refer to the server guide version 3.0 for the detailed description of the following data.


		// 모든 SDK API를 사용하기 위해서는 IAP 객체 생성을 먼저 해주어야 한다. storeType 은 실행되는 앱의 마켓 종류의 구분이 가능할 경우 로그인시 게임서버에서 전달해주는 것을 권고합니다.
		// IAP object must be created first in order to used all SDK APIs. It is recommended that 'storeType' should send to the game server when a user signs in, if the game server can recognize the client's platform.

		if( !IAP.createIAP(StoreType.getType(store_type),true) )
		{
			AppUtil.LOGE(store_type + " createIAP fail");
			AppUtil.Toast(store_type + " createIAP fail");
			return -1;
		}

		String skuInfo = getBillingSkuData(false);
		IAP.skuList(main_activity, skuInfo, listener_skuListener);										// Optional API : 필여할 경우 아이템 리스트를 조회하여 상점을 구성한다.(글로벌 원빌드게임)
		IAP.getRemainTransactions(main_activity, listener_remainListener);								// 상점 진입시마다 미지급 아이템 확인을 위한 API 를 호출한다. :  Call this API to check the undelivered item whenever a user enter the store.


		if(!store_type.equalsIgnoreCase("googleplay"))
		{
			AppUtil.LOGE("market is not supported");
			//startActivityForResult(new Intent(main_activity,LoginActivity.class),0);					// 카카오상점 로그인 처리: kakao game shop login
			return -1;
		}


		google_receiver          = new IabBroadcastReceiver(listener_google_promo);						// 구글 프로모션 코드 리시버 : google promotion code recevier
		IntentFilter promoFilter = new IntentFilter("com.android.vending.billing.PURCHASES_UPDATED");
		registerReceiver(google_receiver,promoFilter);

		return 0;
	}

	private static void logPurchase(Purchase purchase)
	{
		AppUtil.LOGI("transactionId      : " + purchase.getTransactionId());
		AppUtil.LOGI("storeType          : " + purchase.getStoreType());
		AppUtil.LOGI("applicationId      : " + purchase.getApplicationId());
		AppUtil.LOGI("productId          : " + purchase.getProductId());
		AppUtil.LOGI("marketTransactionId: " + purchase.getTransactionIdOnMarket());
		AppUtil.LOGI("purchaseData       : " + purchase.getPurchaseData());
		AppUtil.LOGI("receipt            : " + purchase.getReceipt());
		AppUtil.LOGI("signature          : " + purchase.getSignature());
		AppUtil.LOGI("* Purchase         : " + purchase.toJSONString());
	}



	// 사용자 요청 결제 시작: Starts purchasing process from a user request
	private void storeSendRegist()
	{
		// 결제 진행을 위해 빌링서버로 보낼 정보 세팅
		// Information to be sent to IAP server for purchasing process
		setBillingRegistData();

		// 결제를 시작하기 위해서는 각 게임에 맞는 통신 프로토콜을 사용하여 게임서버를 통해 빌링서버 initialize API 를 호출해야한다.
		// To start purchasing process, the game server calls 'Initialize' API to IAP server by using communication protocol based on your game.
		store_network.sendRegist(billing_regist, store_type, user_game_code, pay_user, pay_regist, cb_regist);
	}



	// Optional API : 필요할 경우 개인화 아이템 리스트를 조회하여 팝업 상점을 구성한다.
	private void storeSkuList()
	{
		String skuInfo = getBillingSkuData(true);
		IAP.skuList(main_activity, skuInfo, listener_skuListener);
	}


	private static String getBillingSkuData(boolean isPrivate)
	{
		SkuData sd = new SkuData(user_game_code); // (필수값) 넷마블에서 발급하는 게임코드 : (Required) Game code provided by Netmarble

		if(isPrivate)
		{
			sd.setKindType(SkuConsts.KIND_TYPE_PRIVATE);
			sd.setGroupCode("blueberryGGL00001");
		}

		if(!google_use)
		{
			sd.setSource(SkuConsts.SKU_SOURCE_NETMARBLE); // (선택값) 상품 리스트 정보 제공처(Netmarble or Store(default)) : (Optional) (Netmarble or Store(default))
		}

		return sd.toJSONString();
	}


	private void setBillingRegistData()
	{
		// 아래 데이터 값은 빌링 서버 연동 문서 v3 참고하세요. Please refer to the server guide version 3.0 for the detailed description of the following data.

		// 공통 상품코드( 공통 상품코드를 통해 각 마켓의 실제 상품코드를 빌링서버(initialize API)로부터 전달받아 사용한다 )  Item code (Product ID for each market will be provided by IAP server ('Initialize' API) through this item code)
		String commonItemCode = store_itemCode;

		if(commonItemCode.length() <= 0)
		{
			commonItemCode = "item001";
		}

		String purchaseType = "IAP"; // IAP(일반구매) or EXP(대안상점) : IAP(normal purchase) or EXP(external store purchase)
		String giftFlag     = "N"  ; // 선물 구매 여부 : Whether this is for gift or not
		String countryCode  = "KR" ; // 국가 코드 : Country code
		String languageCode = "ko-KR"; // 언어 코드 : Language code
		pay_regist = new Regist(commonItemCode,purchaseType,giftFlag,countryCode,languageCode);
	}

	private void unregistGoogleReceiver()
	{
		if(null == google_receiver)
			return;

		unregisterReceiver(google_receiver);
		google_receiver = null;
	}

	private IabBroadcastListener listener_google_promo = new IabBroadcastListener()
	{
		@Override public void receivedBroadcast()
		{
			IAP.logIAP(APPCONST.TAG,"Google promotion code recevier");
			IAP.getRemainTransactions(main_activity, listener_remainListener);
		}
	};

	// IAP SDK request callback implement: SDK -> Client
	private OnSkuListener listener_skuListener = new OnSkuListener()
	{
		@Override public void onSkuList(final IAPResult result,final List<IAPSku> skuList)
		{
			AppUtil.LOGI("onSkuList: " + result.getResponse());
			AppUtil.LOGI("message: " + result.getMessage());

			if(!result.isSuccess() || null == skuList)
			{
				nsdk_showToast("SDK: There is no available item.");
				return;
			}

			final ArrayList<String> itemList = new ArrayList<String>();

			for(IAPSku sku : skuList)
			{
				AppUtil.LOGI("display Name   : " + sku.getDispName());
				AppUtil.LOGI("display Amount : " + sku.getDispAmount());
				AppUtil.LOGI("currencyCode   : " + sku.getCurrencyCd());
				AppUtil.LOGI("currencySymbol : " + sku.getCurrencySymbol());
				AppUtil.LOGI("amount         : " + sku.getAmount());

				List<IAPSku.Reward> reward_items = sku.getRewards();
				String rewards = "";

				for(int i = 0; i < reward_items.size(); i++)
				{
					rewards += reward_items.get(i).getItemId() + ":" + reward_items.get(i).getItemName() + ", ";
				}
				AppUtil.LOGI("reward items : " + rewards);

				itemList.add(sku.getDispName());
			}
		}
	};

	//--------------------------------------------------------------------------

	private OnInitializeListener listener_initialize = new OnInitializeListener()
	{
		@Override public void onInitialize(IAPResult result)
		{
			AppUtil.LOGI("onInitialize: " + result.getResponse());
			AppUtil.LOGI("message: " + result.getMessage());


			// IAP 초기화가 성공하면 빌링서버 initialize API 에서 전달받은 transactionId productId(itemId)
			// 그리고 각 마켓에 필요한 정보를 사용하여 SDK 결제를 진행한다.
			// If 'initialize' API is successfully completed, call 'purchase' API using received transactionId, productId and other information required for the market
			// The game server received the above information by IAP server through 'Initialize' API.

			if(!result.isSuccess())
			{
				AppUtil.Toast("SDK: Failed to initialize IAP SDK.");
				return;
			}

			AppUtil.LOGI("purchase transactionId : " + pay_purchase.getTransactionId());
			AppUtil.LOGI("purchase purchaseInfo : " + pay_purchase.toJSONString());

			// transactionId 와 purchaseInfo JSON 데이터를 전달하여 구매창을 생성한다.
			// Creates a payment windows by sending JOSN data of 'transactionId' and 'purchaseInfo'
			IAP.purchase(main_activity,pay_purchase.toJSONString(),pay_purchase.getTransactionId(),listener_purchase);
		}
	};

	//--------------------------------------------------------------------------

	private OnGetRemainTransactionsListener listener_remainListener = new OnGetRemainTransactionsListener()
	{
		@Override public void onGetRemainTransactions(final IAPResult result,List<Purchase> purchases)
		{
			AppUtil.LOGI("onGetRemainTransactions: " + result.getResponse());
			AppUtil.LOGI("message: " + result.getMessage());


			// purchase 가 null 이 아니면 게임 서버를 통해 빌링 서버로 verify 요청을 한다.
			// If list of purchase is not null, send this to your server for verify transactions.
			if(result.isSuccess() && purchases != null && purchases.size() > 0)
			{
				JSONArray verifyData = new JSONArray();

				RemainData rd = new RemainData(main_activity,purchases);
				List<Purchase> rList = rd.getRemainData();
				String rApplicationId = rd.getApplicationId(); // 한 게임에서 여러 IAP를 사용할 경우  : If your game useds several PSPs(Payment Service Provider) for IAP
				String rStoreType = rd.getStoreType(); // 한 게임에서 여러 IAP를 사용할 경우  : If your game useds several PSPs(Payment Service Provider) for IAP

				for(int i = 0; i < rList.size(); i++)
				{
					try
					{
						verifyData.put(new JSONObject(rList.get(i).toJSONString()));
					}
					catch(JSONException e)
					{
					}
				}

				AppUtil.LOGI("Server verify purchases data : " + verifyData.toString());

				pay_verify = VerifyType.Remain;

				if(verifyData.length() > 0)
				{
					// 미지급 아이템 내역이 있을경우 각 게임에 맞는 통신 프로토콜을 사용하여 게임서버를 통해 빌링서버 verify API 를 호출한다.
					// If the list of undelivered item exists, the game server calls 'Verify' API to IAP server using communication protocol based on your game.
					store_network.sendVerify(billing_verify, rStoreType, user_game_code, rApplicationId,"PU", pay_user, verifyData.toString(), cb_verify);
				}
				else
				{
					AppUtil.Toast("SDK: There is no undelivered item.");
				}
			}
			else
			{
				if(!result.isSuccess())
				{
					AppUtil.Toast("SDK: Failed to search the undelivered item.");
				}
				else
				{
					AppUtil.Toast("SDK: There is no undelivered item.");
				}
			}
		}
	};

	//--------------------------------------------------------------------------

	private OnPurchaseListener listener_purchase = new OnPurchaseListener()
	{
		@Override public void onPurchase(final IAPResult result,Purchase purchase)
		{
			AppUtil.LOGI("onPurchase: " + result.getResponse());
			AppUtil.LOGI("message: " + result.getMessage());

			if(result.isSuccess() && purchase != null)
			{
				logPurchase(purchase);
				JSONArray verifyData = new JSONArray();

				try
				{
					verifyData.put(new JSONObject(purchase.toJSONString()));
				}
				catch(JSONException e)
				{
				}

				AppUtil.LOGI("Server verify purchases data : " + verifyData.toString());

				pay_verify = VerifyType.Purchase;

				// 테스트용 코드
				if(store_verify)
				{
					AppUtil.Toast("Billing server verify process skiped.");
					return;
				}

				// 결제에 성공했을 경우 각 게임에 맞는 프로토콜을 사용하여 게임서버를 통해 빌링서버로 verify API 를 호출한다.
				// If 'purchase' API is successfully completed, the game server calls 'Verify' API to IAP server using communication protocol based on your game.
				store_network.sendVerify(billing_verify, purchase.getStoreType(), user_game_code, purchase.getApplicationId(),"PU", pay_user, verifyData.toString(), cb_verify);
			}
			else
			{
				AppUtil.Toast("SDK: Failed to call 'purchase' API.(Error code : " + result.getResponse() + ", Error message : " + result.getMessage() + ")");
			}
		}
	};

	//--------------------------------------------------------------------------

	private OnConsumeItemsListener listener_consume = new OnConsumeItemsListener()
	{
		@Override public void onConsumeItems(final IAPResult result)
		{
			AppUtil.LOGI("onConsumeItems: " + result.getResponse());
			AppUtil.LOGI("message: " + result.getMessage());

			if(result.isSuccess())
			{
				AppUtil.Toast("SDK: Succeeded to call 'consumeItems' API.");

				if(pay_verify == VerifyType.Purchase)
				{
					AppUtil.Toast("SDK: Succeeded all purchase processes.");
				}
			}
			else
			{
				AppUtil.Toast("SDK: Failed to call 'consumeItems' API. : " + result.getMessage());
			}
		}
	};

	//--------------------------------------------------------------------------

	// Server request callback implement: Billing Server -> Game Server -> Client Callback
	public static String CLIENT_ID           = "89183114982564704";
	public static String CLIENT_SECRET       = "4TcyiK3HxCrOjaC86bOArXRq48X1RruxuMHrEkfsWlB6bJLxSI0i0CiqfmHK3xUa7BeELGg4PNZ2bZIuCBj/jg==";

	public static String CLIENT_REDIRECT_URI = "kakao" + CLIENT_ID + "://exec";
	public static final String PREF_KEY      = "test.kakao.auth.pref.key";


	private OnRegistCallback cb_regist = new OnRegistCallback()
	{
		@Override public void onRegist(final int errorCode,final String response)
		{
			IAP.logIAP(APPCONST.TAG,"ErrorCode: " + errorCode);
			IAP.logIAP(APPCONST.TAG,"regist response: " + response);

			if(errorCode == 0)
			{
				final PurchaseData pd = new PurchaseData(user_key, user_platform, response);

				if(pd.getResult() == 0)
				{
					AppUtil.Toast("appid : " + pd.getApplicationId() + "\ntid : " + pd.getTransactionId());

					// 카카오톡일 경우 카카오톡에서 발급하는 인증 토큰을 받아서 전달해 준다.
					store_accessToken = getApplicationContext().getSharedPreferences(PREF_KEY, Context.MODE_PRIVATE).getString("access_token","");
					pd.setAccessToken(store_accessToken);

					pay_purchase = pd;

					// 빌링서버 initialize API 에서 전달받은 applicationId 를 사용하여 SDK 초기화를 진행한다.
					// applicationId : 빌링서버에서 전달해주는 앱 아이디( 게임코드, 스토어타입을 기준으로 각 마켓별 applicationId를 빌링서버(initialize API) 에서 전달받아 사용한다. )
					// The game client calls 'initialize' API to initialize IAP SDK using 'applicationId' received by IAP server through 'Initialize' API (in this case 'sendRegist()').
					// applicationId : application ID provided by IAP server. (IAP server('Initialize' API) provides market-specific 'applicationId' based on game code and store type)
					IAP.initialize(main_activity, pd.getApplicationId(), listener_initialize);
				}
			}
			else
			{
				AppUtil.Toast("Server: Failed to call 'Initialize' API to IAP server.(Error code : " + errorCode + ")");
			}
		}
	};

	//--------------------------------------------------------------------------

	private OnVerifyCallback cb_verify = new OnVerifyCallback()
	{
		@Override public void onVerify(final int errorCode,String response)
		{
			IAP.logIAP(APPCONST.TAG,"ErrorCode: " + errorCode);
			IAP.logIAP(APPCONST.TAG,"verify response: " + response);

			// 네트워크 오류가 아닌경우만 진행합니다. 네트워크 오류로 인해 빌링 서버의 응답을 전달 받지 못하였을 경우 skip 하고 다음 getRemainTransactions 프로세스때 진행합니다.
			if(errorCode != -999)
			{
				final ConsumeData cd = new ConsumeData((pay_verify == VerifyType.Purchase),response);

				// 결제 성공(빌링서버 verify 성공)후에는 꼭 consumeItmes API 를 호출해 주어야 한다.
				// 'consumeItems' API must be called after payment is complete(returns successful result of 'Verify' API from IAP server).
				if( 0 >= cd.getConsumeData().size())
				{
					AppUtil.Toast("Server: Failed to call 'Verify' API to IAP server.(resuts error)");
					return;
				}

				main_activity.runOnUiThread(new Runnable()
				{
					@Override public void run()
					{
						IAP.consumeItems(main_activity, cd.getConsumeData(), listener_consume);
					}
				});
			}
		}
	};

}




