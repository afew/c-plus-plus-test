
package com.netmarble.elemental;

public class APPCONST
{
	public static final String TAG                               = "jpp";
	public static final int NSDK_EFAIL                           = -1;
	public static final int NSDK_OK                              =  0;
	public static final int NSDK_FALSE                           =  0;
	public static final int NSDK_TRUE                            =  1;
	public static final int NSDK_REQUEST                         =  0x30006;	// same to IN_PROGRESS

	public static final int CUSTOMERSUPPORTPAGE_HOME             =  0;
	public static final int CUSTOMERSUPPORTPAGE_FAQ              =  1;
	public static final int CUSTOMERSUPPORTPAGE_INQUIRY          =  2;
	public static final int CUSTOMERSUPPORTPAGE_GUIDE            =  3;
	public static final int CUSTOMERSUPPORTPAGE_INQUIRYHISTORY   =  4;

	public static final int NSDK_LOCATION_NOTICE_INTRO           =  1;
	public static final int NSDK_LOCATION_NOTICE_INGAME          =  2;
	public static final int NSDK_LOCATION_PROMOTION_MAIN         =  1;
	public static final int NSDK_LOCATION_PROMOTION_EVENT        =  2;
	public static final int NSDK_LOCATION_PROMOTION_ETC          =  3;

	public static final int NSDK_WINDOW_FAILED                   = -1;
	public static final int NSDK_WINDOW_CLOSED                   =  0;
	public static final int NSDK_WINDOW_OPENED                   =  1;
	public static final int NSDK_WINDOW_REWARDED                 =  2;
	public static final int NSDK_WINDOW_TERMINATE                =100;

	public static final int NSDK_SIGN_IN_EFAIL                   = -1;
	public static final int NSDK_SIGN_IN_NONE                    =  0;
	public static final int NSDK_SIGN_IN_TRUE                    =  1;
	public static final int NSDK_SIGN_IN_MODIFIED                =  2;

	public static final int NSDK_CHANNEL_EVERY_NETMARBLE         =  0;
	public static final int NSDK_CHANNEL_FACEBOOK                =  1;
	public static final int NSDK_CHANNEL_KAKAO                   =  2;
	public static final int NSDK_CHANNEL_GOOGLE_PLUS             =  5;
	public static final int NSDK_CHANNEL_APPLE_GAME_CENTER       =  6;
	public static final int NSDK_CHANNEL_NAVER                   =  7;
	public static final int NSDK_CHANNEL_GAME_START              =  8;
	public static final int NSDK_CHANNEL_OPTION_REPLACE          =  0;
	public static final int NSDK_CHANNEL_OPTION_SELECT           =  1;
	public static final int NSDK_CHANNEL_OPTION_SIZE             =  4;
	public static final int NSDK_CHANNEL_EXIST_OFFSET            =100;

	public static final int NSDK_QMENU_TOP_TO_BOTTOM             =  0;
	public static final int NSDK_QMENU_RIGHT_TO_LEFT             =  1;
	public static final int NSDK_QMENU_BOTTOM_TO_TOP             =  2;
	public static final int NSDK_QMENU_LEFT_TO_RIGHT             =  3;

	public static final int NSDK_IME_CANCEL                      = NSDK_EFAIL;
	public static final int NSDK_IME_OK                          = NSDK_OK;

	public static final int MSGPOPUP_UNKNOWN                     =  0;
	public static final int MSGPOPUP_GAMECENTER_CONNECT_OK       =  1;			// 게임 데이터가 연결되었습니다.
	public static final int MSGPOPUP_GAMECENTER_EXIST_ID         =  100+2;		// 이 계정에 연결된 다른 게임 데이터가 있습니다. 이 데이퍼르 불러 오시겠습니까?
}


