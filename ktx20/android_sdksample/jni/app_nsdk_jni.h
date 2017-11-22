// NSDK_JNI is java and ios interface for the netmarble sdk and other utilies.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _APP_NSDK_JNI_H_
#define _APP_NSDK_JNI_H_

//#define UNIT_TEST

#ifndef __func__
#define __func__ __FUNCTION__
#endif

#ifndef ____STRINGIZE____
#define ____STRINGIZE____(x) #x
#endif

#ifndef ____STRINGTO____
#define ____STRINGTO____(x) ____STRINGIZE____(x)
#endif

#ifndef FUNC_STRING
#define FUNC_STRING  ____STRINGTO____(__func__)
#endif

#ifndef LINE_STRING
#define LINE_STRING  ____STRINGTO____(__LINE__)
#endif

//#ifndef FUNCTION_LINE
//#define FUNCTION_LINE   FUNC_STRING/**/"["/**/LINE_STRING/**/"]:"
//#endif

#if defined(ANDROID_DEVICE)
#include <jni.h>
#include <android/log.h>
#endif

// for java native interface
#if defined(__WIN32__) || defined(_WIN32)
typedef unsigned char   jboolean;      /* unsigned 8 bits */
typedef signed char     jbyte   ;      /* signed 8 bits */
typedef unsigned short  jchar   ;      /* unsigned 16 bits */
typedef short           jshort  ;      /* signed 16 bits */
typedef int             jint    ;      /* signed 32 bits */
typedef long long       jlong   ;      /* signed 64 bits */
typedef float           jfloat  ;      /* 32-bit IEEE 754 */
typedef double          jdouble ;      /* 64-bit IEEE 754 */
typedef jint            jsize   ;

class _jobject {};
class _jclass : public _jobject {};
class _jstring : public _jobject {};
class _jarray : public _jobject {};
class _jobjectArray : public _jarray {};
class _jbooleanArray : public _jarray {};
class _jbyteArray : public _jarray {};
class _jcharArray : public _jarray {};
class _jshortArray : public _jarray {};
class _jintArray : public _jarray {};
class _jlongArray : public _jarray {};
class _jfloatArray : public _jarray {};
class _jdoubleArray : public _jarray {};
class _jthrowable : public _jobject {};

typedef _jobject*       jobject;
typedef _jclass*        jclass;
typedef _jstring*       jstring;
typedef _jarray*        jarray;
typedef _jobjectArray*  jobjectArray;
typedef _jbooleanArray* jbooleanArray;
typedef _jbyteArray*    jbyteArray;
typedef _jcharArray*    jcharArray;
typedef _jshortArray*   jshortArray;
typedef _jintArray*     jintArray;
typedef _jlongArray*    jlongArray;
typedef _jfloatArray*   jfloatArray;
typedef _jdoubleArray*  jdoubleArray;
typedef _jthrowable*    jthrowable;
typedef _jobject*       jweak;

struct _jfieldID; typedef struct _jfieldID* jfieldID;
struct _jmethodID; typedef struct _jmethodID* jmethodID;
struct JNIInvokeInterface;
typedef union jvalue { jboolean z; jbyte b; jchar c; jshort s; jint i; jlong j; jfloat f; jdouble d; jobject l; } jvalue;

typedef enum jobjectRefType {
    JNIInvalidRefType = 0, JNILocalRefType = 1,
    JNIGlobalRefType = 2, JNIWeakGlobalRefType = 3
} jobjectRefType;

typedef struct { const char* name; const char* signature; void* fnPtr; } JNINativeMethod;
struct _JNIEnv; struct _JavaVM;
typedef const struct JNINativeInterface* C_JNIEnv;

typedef _JNIEnv JNIEnv;
typedef _JavaVM JavaVM;
struct JNINativeInterface;

#endif

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

#ifndef LOG_TAG
  #define  LOG_TAG    "epp"
#endif

#if defined(ANDROID_DEVICE)
  #define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO ,LOG_TAG,__VA_ARGS__)
  #define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
  #define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN ,LOG_TAG,__VA_ARGS__)
  #define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#elif defined(IOS_MOBILE_DEVICE)
namespace EPP {
	extern void EPP_ios_log(const char* str);
	extern int EPP_sprintf( char *buffer, int nBuffersize,  const char *format,...);
}

	#if defined(UNIT_TEST)
		#define  LOGD(...) {{char b[2048+8]="[D]:";snprintf(b+4, 2048, __VA_ARGS__);NSString* s=[NSString stringWithUTF8String:b];NSLog(@"%@\n",s);}}
		#define  LOGI(...) {{char b[2048+8]="[I]:";snprintf(b+4, 2048, __VA_ARGS__);NSString* s=[NSString stringWithUTF8String:b];NSLog(@"%@\n",s);}}
		#define  LOGW(...) {{char b[2048+8]="[W]:";snprintf(b+4, 2048, __VA_ARGS__);NSString* s=[NSString stringWithUTF8String:b];NSLog(@"%@\n",s);}}
		#define  LOGE(...) {{char b[2048+8]="[E]:";snprintf(b+4, 2048, __VA_ARGS__);NSString* s=[NSString stringWithUTF8String:b];NSLog(@"%@\n",s);}}
	#else
		#define  LOGD(...) {{char b[2048+8]="[D]:";EPP::EPP_sprintf(b+4, 2048, __VA_ARGS__);EPP::EPP_ios_log(b);}}
		#define  LOGI(...) {{char b[2048+8]="[I]:";EPP::EPP_sprintf(b+4, 2048, __VA_ARGS__);EPP::EPP_ios_log(b);}}
		#define  LOGW(...) {{char b[2048+8]="[W]:";EPP::EPP_sprintf(b+4, 2048, __VA_ARGS__);EPP::EPP_ios_log(b);}}
		#define  LOGE(...) {{char b[2048+8]="[E]:";EPP::EPP_sprintf(b+4, 2048, __VA_ARGS__);EPP::EPP_ios_log(b);}}
	#endif

#else

	#define  LOGD(...) {{char b[2048+8]="[D]:";::sprintf(b+4,__VA_ARGS__);::strcat(b,"\n");::consoleColor(CONSOLE_COLOR_WHITE  );::printf(b);::consoleColor(CONSOLE_COLOR_GRAY);}}
  	#define  LOGI(...) {{char b[2048+8]="[I]:";::sprintf(b+4,__VA_ARGS__);::strcat(b,"\n");::consoleColor(CONSOLE_COLOR_GREEN  );::printf(b);::consoleColor(CONSOLE_COLOR_GRAY);}}
	#define  LOGW(...) {{char b[2048+8]="[W]:";::sprintf(b+4,__VA_ARGS__);::strcat(b,"\n");::consoleColor(CONSOLE_COLOR_YELLO  );::printf(b);::consoleColor(CONSOLE_COLOR_GRAY);}}
	#define  LOGE(...) {{char b[2048+8]="[E]:";::sprintf(b+4,__VA_ARGS__);::strcat(b,"\n");::consoleColor(CONSOLE_COLOR_MAGENTA);::printf(b);::consoleColor(CONSOLE_COLOR_GRAY);}}
#endif


#define PROJECT_PACKAGE       "com/netmarble/elemental"
#define PROJECT_MAIN_ACTIVITY "GL2JNIActivity"
#define JNI_FUNC_DCL(ret, f)  JNIEXPORT ret JNICALL Java_com_netmarble_elemental_GL2JNIActivity_/**/##/**/f
#define JNI_FUNC_CALL(f)      Java_com_netmarble_elemental_GL2JNIActivity_/**/##/**/f
#define PROJECT_MAIN_CLASS    PROJECT_PACKAGE/**/"/"/**/PROJECT_MAIN_ACTIVITY

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#ifndef NETMARBLES_DOMAIN
  #define NETMARBLES_DOMAIN        "NETMARBLES_DOMAIN"
  #define FACEBOOK_DOMAIN          "FACEBOOK_DOMAIN"
  #define APPLE_GAMECENTER_DOMAIN  "APPLE_GAMECENTER_DOMAIN"
  #define KAKAO_DOMAIN             "KAKAO_DOMAIN"
#endif


#if defined(STUB_LIB_PACKAGE)

enum
{
	MOTIONEVENT_ACTION_DOWN                 = 0,
	MOTIONEVENT_ACTION_UP                   = 1,
	MOTIONEVENT_ACTION_MOVE                 = 2,
	MOTIONEVENT_ACTION_CANCEL               = 3,
	MOTIONEVENT_ACTION_OUTSIDE              = 4,
	MOTIONEVENT_ACTION_POINTER_DOWN         = 5,
	MOTIONEVENT_ACTION_POINTER_UP           = 6,
	MOTIONEVENT_ACTION_HOVER_MOVE           = 7,
};

#endif

enum
{
	// result NetmarbleS SDK message code
	NSDK_MSG_SUCCESS                     = 0x00000000,					//      0. Success.

	NSDK_MSG_UNKNOWN                     = 0x00010001,					//  65537. Unknown error.
	NSDK_MSG_SERVICE                     = 0x00010002,					//  65538. Service error.
	NSDK_MSG_NETWORK_UNAVAILABLE         = 0x00010003,					//  65539. Network is unavailable.
	NSDK_MSG_TIMEOUT                     = 0x00010004,					//  65540. Time out.
	NSDK_MSG_USER_CANCELED               = 0x00020001,					// 131073. Canceled by user.
	NSDK_MSG_INVALID_TOKEN               = 0x00020002,					// 131074. Invalid token.
	NSDK_MSG_NOT_AUTHENTICATED           = 0x00030001,					// 196609. Not authenticated.
	NSDK_MSG_INVALID_PARAM               = 0x00030002,					// 196610. Invalid Parameter.
	NSDK_MSG_NOT_SUPPORTED               = 0x00030003,					// 196611. Not supported.
	NSDK_MSG_PERMISSION                  = 0x00030004,					// 196612. No permission.
	NSDK_MSG_JSON_PARSING_FAIL           = 0x00030005,					// 196613. JSon parsing failed.
	NSDK_MSG_IN_PROGRESS                 = 0x00030006,					// 196614. In Progress.

	NSDK_MSG_GOOGLE_MISCONFIGURED        = 0x00002714,					//  10004. google service error.

	// Select one channel connect option.
	NSDK_MSG_CHANNEL_CONNECTION_MODIFIED = 0x00050001,					// 327681
	NSDK_MSG_USED_CHANNELID              = 0x00050002,					// 327682
	NSDK_MSG_NEW_CHANNELID               = 0x00050003,					// 327683

	// quick menu
	NSDK_QMENU_TOP_TO_BOTTOM             = 0x00000000,
	NSDK_QMENU_RIGHT_TO_LEFT             = 0x00000001,
	NSDK_QMENU_BOTTOM_TO_TOP             = 0x00000002,
	NSDK_QMENU_LEFT_TO_RIGHT             = 0x00000003,
};

// application defined message
enum
{
	NSDK_EFAIL                              = -1,
	NSDK_OK                                 =  0,						// NSDK_MSG_SUCCESS,
	NSDK_FALSE                              =  0,
	NSDK_TRUE                               =  1,
	NSDK_REQUEST                            =  NSDK_MSG_IN_PROGRESS,	// while query ...
	NSDK_ERR_PROC                           =  0xEE000000,				// error process

	CUSTOMERSUPPORTPAGE_HOME                =  0,
	CUSTOMERSUPPORTPAGE_FAQ                 =  1,
	CUSTOMERSUPPORTPAGE_INQUIRY             =  2,
	CUSTOMERSUPPORTPAGE_GUIDE               =  3,
	CUSTOMERSUPPORTPAGE_INQUIRYHISTORY      =  4,

	NSDK_LOCATION_NOTICE_INTRO              =  1,
	NSDK_LOCATION_NOTICE_INGAME             =  2,
	NSDK_LOCATION_PROMOTION_MAIN            =  1,
	NSDK_LOCATION_PROMOTION_EVENT           =  2,
	NSDK_LOCATION_PROMOTION_ETC             =  3,

	NSDK_WINDOW_FAILED                      = -1,
	NSDK_WINDOW_CLOSED                      =  0,
	NSDK_WINDOW_OPENED                      =  1,
	NSDK_WINDOW_REWARDED                    =  2,
	NSDK_WINDOW_TERMINATE                   =100,

	NSDK_SIGN_IN_EFAIL                      = -1,
	NSDK_SIGN_IN_NONE                       =  0,
	NSDK_SIGN_IN_TRUE                       =  1,
	NSDK_SIGN_IN_MODIFIED                   =  2,

	NSDK_CHANNEL_EVERY_NETMARBLE            =  0,
	NSDK_CHANNEL_FACEBOOK                   =  1,
	NSDK_CHANNEL_KAKAO                      =  2,
	NSDK_CHANNEL_GOOGLE_PLUS                =  5,
	NSDK_CHANNEL_APPLE_GAME_CENTER          =  6,
	NSDK_CHANNEL_NAVER                      =  7,
	NSDK_CHANNEL_GAME_START                 =  8,
	NSDK_CHANNEL_TOT                            ,

	NSDK_CHANNEL_OPTION_REPLACE             =  0,
	NSDK_CHANNEL_OPTION_SELECT              =  1,
	NSDK_CHANNEL_OPTION_SIZE                =  4,
	NSDK_CHANNEL_EXIST_OFFSET               =100,

	NSDK_IME_CANCEL                         = NSDK_EFAIL,
	NSDK_IME_OK                             = NSDK_OK,

#if defined(IOS_MOBILE_DEVICE)
	NSDK_DEFAULT_LOGIN_CHANNEL              = NSDK_CHANNEL_APPLE_GAME_CENTER,
#else
	NSDK_DEFAULT_LOGIN_CHANNEL              = NSDK_CHANNEL_GOOGLE_PLUS,
#endif
};

enum
{
	NSDK_LENGTH_PLAYER_ID                   =   64,
	NSDK_LENGTH_GAME_TOKEN                  = 1024,
	NSDK_LENGTH_CHANNEL_ID                  =  256,
	NSDK_LENGTH_COUNTRY_CODE                =   64,
	NSDK_LENGTH_GAME_CODE                   =  256,
	NSDK_LENGTH_ZONE                        =   64,
	NSDK_LENGTH_MARKET                      =   64,
	NSDK_LENGTH_REGION                      =   64,
};

enum
{
	TOUCH_MONO_DOWN                         = 0,
	TOUCH_MONO_UP                           = 1,
	TOUCH_MULTI_DOWN                        = 2,
	TOUCH_MULTI_UP                          = 3,
	TOUCH_MONO_MOVE                         = 4,
	TOUCH_MULTI_MOVE                        = 5,
};

enum
{
	TEXT_FIELD_OWNER_NONE                   = -1,
	TEXT_FIELD_NONE                         = 0,												// text field state: none state
	TEXT_FIELD_ACTIVE                       = 1,												// text field state: set.. activate
	TEXT_FIELD_ACTIVATED                    = 2,												// text field state: activated
	TEXT_FIELD_MAX_LENGH                    = 256,												// text field max length
};


typedef void (*NSDK_CB_SOFTKEYBOARD)(void* owner, long long calling_id
                                    , int ret, const char* str, int str_len);					// soft keyboard callback function type

typedef void (*NSDK_CB_REMOTE_IMAGE)(void* owner, unsigned char* pixel, int w, int h);			// get the remote image


struct TPROFILE_SNS																				// to get the facebook profile with c-grammar
{
	char* playerID;
	char* snsID;																				// facebook, googleplus, applegamecenter or everynetmarble id
	char* nickName;																				// facebook name, or googleplus, everynetmarble, or apple game center nickName
	char* imageURL;
	char* thumbnailURL;																			// google plus does not support.
	int   gender;																				// google gender: 0:MALE, 1:FEMALE, 2: other
	char* statusMessage;																		// everynetmarble status message

	TPROFILE_SNS() :  playerID(0), snsID(0), nickName(0)
					, imageURL(0), thumbnailURL(0), gender(-1)
					, statusMessage(0){}
};

struct TLOCAL_PUSH																				// to get the local push list with c-grammar
{
	int notificationID;
	int isUse;
};

// nsdk windows state: correspond next functions

extern int     nsdk_st_opened_showexit               ;											// state for exit window
extern int     nsdk_st_opened_signin                 ;											// state for sign in window
extern int     nsdk_st_opened_channel_signin         ;											// state for sign in channel
extern int     nsdk_st_opened_qmenu                  ;											// state for qmenu in window

extern int     nsdk_st_opened_showcouponview         ;											// state for coupon window
extern int     nsdk_st_opened_showfreechargeview     ;											// state for free charge window
extern int     nsdk_st_opened_showgamereviewview     ;											// state for game review window
extern int     nsdk_st_opened_shownoticeview         ;											// state for notice window
extern int     nsdk_st_opened_showpromotionview      ;											// state for promotion window
extern int     nsdk_st_opened_showcustomersupportview;											// state for customer support window
extern int     nsdk_st_opened_showagreementview      ;											// state for agreement window

extern int     nsdk_st_query_channel                 ;											// state for channel: same to window open. none = NSDK_WINDOW_CLOSED, query success: NSDK_WINDOW_OPENED, while query = NSDK_REQUEST
extern int     nsdk_st_query_channel_option          ;											// state for channel connection option: nothing-NSDK_EFAIL, failed-NSDK_FALSE, success-NSDK_TRUE
extern int     nsdk_st_query_channel_id              ;											// query channel id
extern int     nsdk_st_query_facebook                ;											// state for facebook query. same to channel
extern int     nsdk_st_query_googleplus              ;											// state for googleplus query.
extern int     nsdk_st_query_everynetmarble          ;											// state for everynetmarble query.
extern int     nsdk_st_query_applegamecenter         ;											// state for apple game center.
extern int     nsdk_st_query_push                    ;											// state for push query: same to channel

// nsdk windows : all methods are return failed =>NSDK_EFAIL success =>NSDK_OK or number of length(count)
extern int       nsdk_AOSVersion            ();													// get the android os version number
extern long long nsdk_getTotalMemory        ();													// get the total memory(MB). 0>= means that has error.
extern long long nsdk_getFreeMemory         ();													// get the available free memory(MB). 0>= means that has error.
extern long long nsdk_getUsedMemory         ();													// get the used memory(MB). 0>= means that has error.
extern long long nsdk_getFreeDiskSpace      ();													// get the available free space to external storage. 0> means that has error.

extern int     nsdk_showSoftKeyboard        ( NSDK_CB_SOFTKEYBOARD caller_cb					// callback function
                                            , void* caller_owner, long long calling_id			// callback owner, id
                                            , int maxLen, int multi);							// max string length and multi?=NSDK_TRUE:FALSE
extern int     nsdk_getSoftKeyboardState    ();
extern int     nsdk_hideNavigationBar       ();													// hide navigation bar for android

extern int     nsdk_getRemoteImagePixel32   ( unsigned char** pixel, int* width, int* height	// get the remote image pixel. output pixel format will be rgba8888
											, const char* url);									// input url
extern int     nsdk_getRemoteImagePixel32Cb (NSDK_CB_REMOTE_IMAGE cb							// get the remote image pixel. output pixel format will be rgba8888		// input url
											, void* owner, const char* url);

extern int     nsdk_initialize              ();													// sdk initialize
extern int     nsdk_showExit                ();													// show Exit page call
extern int     nsdk_signIn                  ();													// show signin
extern int     nsdk_resetSession            ();													// reset session.
extern int     nsdk_showQuickMenuView       (int x, int y, int w, int h, int qmenu);			// show Quick Menu
extern int     nsdk_hideQuickMenuView       ();													// hide Quick Menu

extern int     nsdk_showCouponView          ();													// show coupon view
extern int     nsdk_showFreeChargeView      ();													// show free charge view
extern int     nsdk_showGameReviewView      ();													// show game review view
extern int     nsdk_showNoticeView          (int location);										// show notive view. needs NSDK_LOCATION_NOTICE_{INTRO|INGAME}
extern int     nsdk_showPromotionView       (int location);										// show promotion view.
extern int     nsdk_removePromotionViewCache();													// remove promotion view cache
extern int     nsdk_showCustomerSupportView (int page);											// show customer support view. needs NSDK_CUSTOMERSUPPORTPAGE_{HOME|...|INQUIRYHISTORY}
extern int     nsdk_prepareAgreementView    ();													// prepare agreement view
extern int     nsdk_showAgreementView       ();													// show contract agreement view
extern int     nsdk_destroyAgreementView    ();													// destroy contract agreement view(remove webview caching)
extern int	   nsdk_config_ConnectWIFI      ();													// get_connect_wifi state. NSDK_TRUE or NSDK_FALSE
extern int     nsdk_config_ConnectOnline    ();													// get_connect_online

// configurations
extern int     nsdk_config_PlayerID      (/*in, out*/ char* playerid);							// input player id length must be greater than 64
extern int     nsdk_config_GameToken     (/*in, out*/ char* gametoken);							// input gametoken length must be greater than 1024
extern size_t  nsdk_config_LoginnedCount ();													// channel: NSDK_CHANNEL_EVERY_NETMARBLE to NSDK_CHANNEL_PC. faied: -1
extern int     nsdk_config_ChannelID     (/*in, out*/ char* channelID, size_t ch_code/*in*/);	// channel: NSDK_CHANNEL_EVERY_NETMARBLE to NSDK_CHANNEL_PC for code
extern int     nsdk_config_CountryCode   (/*in, out*/ char* countryCode);						// input country length must be greater than 64
extern int     nsdk_config_GameCode      (/*in, out*/ char* gamecode);							// input gamecode length must be greater than 256
extern int     nsdk_config_Zone          (/*in, out*/ char* zone);								// input zone length must be greater than 64
extern int     nsdk_config_Market        (/*in, out*/ char* market);							// input market length must be greater than 64

extern int     nsdk_config_UseLog                    ();										// return 0(false) or true(1)
extern int     nsdk_config_HttpTimeOutSec            ();										// return time second
extern size_t  nsdk_config_MaxGameLogDataCount       ();										// return log data count. failed ==0
extern int     nsdk_config_SendGameLogDataIntervalMin();										//
extern int     nsdk_config_isGlobalGame              ();										//
extern int     nsdk_config_RegionMode  (/*in, out*/ char* region);								// input region length must be greater than 64

//---------------------------------------------------------------------------------------------------------------------

// channel
extern int     nsdk_connectToChannel          (size_t channel);									// connect to channel. 0(netmarble), 1(facebook), 2(kakao), 5(google plus), 6(apple game center), 7(naver)
extern int     nsdk_disconnectFromChannel     (size_t channel);									// disconnect to channel
extern int     nsdk_channel_connect_option    (int location);									// select channel connect option
extern int     nsdk_rsp_ChannelState          (size_t channel);									// get the channel state. NSDK_{FALSE|TRUE}
extern int     nsdk_rsp_isChannelCanceled     (size_t channel);									// channel state is equal to NSDK_MSG_USER_CANCELED

// face book
extern int     nsdk_requestFacebookMyProfile  ();												// request to facebook my profile
extern int     nsdk_requestFacebookFriends    ();												// request to facebook my friends
extern int     nsdk_inviteFacebookFriends     (const char* message, const char* title);			// request to facebook inviting friends who doesn't play this application.
extern int     nsdk_requestFacebookInviters   ();												// request to facebook the list of inviter who send invite request to me.
extern int     nsdk_deleteFacebookInviters    ();
extern int     nsdk_postFacebookPhoto         (int* colors, int w, int h);						// sending photo. colors must be rgba format
extern int     nsdk_postFacebookStatusUpdate  ( const char* message, const char* name			// update status
											  , const char* picture, const char* link
											  , const char* caption, const char* description);

// facebook saved result
extern int     nsdk_rsp_FacebookMyProfile     (TPROFILE_SNS*);									// get the facebook profile data
extern size_t  nsdk_rsp_FacebookFriendsCount  ();												// get the my facebook friends count
extern int     nsdk_rsp_FacebookFriendProfile (size_t idx, TPROFILE_SNS*);						// get the my facebook friend profile

extern size_t      nsdk_rsp_FacebookInviteFriendsCount();										// get the inviting friends count. after call nsdk_inviteFacebookFriends()
extern const char* nsdk_rsp_FacebookInviteFriend      (size_t idx);								// get the inviting friend facebook id. no friend will be received NULL

extern size_t  nsdk_rsp_FacebookInvitersCount         ();										// get the invitee count
extern int     nsdk_rsp_FacebookInviter               (size_t idx, TPROFILE_SNS*);				// get the invitee profile

extern int     nsdk_rsp_FacebookDeleteInviters();

// GooglePlus
extern int     nsdk_requestGooglePlusMyProfile          ();										// request to googleplus my profile
extern int     nsdk_requestGooglePlusFriends            ();										// request to googleplus my friends

extern int     nsdk_rsp_GooglePlusMyProfile             (TPROFILE_SNS*);						// get the googleplus profile
extern size_t  nsdk_rsp_GooglePlusFriendsCount          ();										// get the my googleplus friends count
extern int     nsdk_rsp_GooglePlusFriendProfile         (size_t idx, TPROFILE_SNS*);			// get the my googleplus friend profile
extern size_t  nsdk_rsp_GooglePlusAppFriendsCount       ();										// get the my googleplus app friends count
extern int     nsdk_rsp_GooglePlusAppFriendProfile      (size_t idx, TPROFILE_SNS*);			// get the my googleplus app friend profile

// EveryNetmarble
extern int     nsdk_requestEveryNetmarbleMyProfile      ();										// request to everynetmarble my profile
extern int     nsdk_requestEveryNetmarbleFriends        ();										// request to everynetmarble my friends

extern int     nsdk_rsp_EveryNetmarbleMyProfile         (TPROFILE_SNS*);						// get the everynetmarble profile
extern size_t  nsdk_rsp_EveryNetmarbleFriendsCount      ();										// get the my everynetmarble friends count
extern int     nsdk_rsp_EveryNetmarbleFriendProfile     (size_t idx, TPROFILE_SNS*);			// get the my everynetmarble friend profile
extern size_t  nsdk_rsp_EveryNetmarbleAppFriendsCount   ();										// get the my everynetmarble app friends count
extern int     nsdk_rsp_EveryNetmarbleAppFriendProfile  (size_t idx, TPROFILE_SNS*);			// get the my everynetmarble app friend profile

// apple game center
extern int     nsdk_requestAppleGameCenterMyProfile    ();										// request to apple game center my profile
extern int     nsdk_requestAppleGameCenterFriends      ();										// request to apple game center my friends

extern int     nsdk_rsp_AppleGameCenterMyProfile       (TPROFILE_SNS*);							// get the apple game center profile
extern size_t  nsdk_rsp_AppleGameCenterFriendsCount    ();										// get the my apple game center friends count
extern int     nsdk_rsp_AppleGameCenterFriendProfile   (size_t idx, TPROFILE_SNS*);				// get the my apple game center friend profile
extern size_t  nsdk_rsp_AppleGameCenterAppFriendsCount ();										// get the my apple game center app friends count
extern int     nsdk_rsp_AppleGameCenterAppFriendProfile(size_t idx, TPROFILE_SNS*);				// get the my apple game center app friend profile

//---------------------------------------------------------------------------------------------------------------------

// push message
extern int     nsdk_setSendPushPlayerIDClear              ();									// clear the player list for sending push message.
extern int     nsdk_setSendPushPlayerIDAdd                (const char* playerIDs);				// add the player list for sending push message.
extern int     nsdk_setSendPushNotificationID             (int notificationID);					// set the notification push id
extern int     nsdk_setSendPushMessage                    (const char* str);					// set the message.

extern int     nsdk_sendPushNotification                  ();									// send the message without notification id to players in play list without notification id.
extern int     nsdk_sendPushNotificationUseNotificationID ();									// send the message with notification id to players in play list without notification id.
extern int     nsdk_setUsePushNotification                (int bUse);							// enable the push notification
extern int     nsdk_getUsePushNotification                ();									// disable the push notification

extern int     nsdk_setPushLocalNotificationID            (int notificationID);					// set the local notificaiton id
extern int     nsdk_getPushLocalNotificationID            ();									// get the local notification id
extern int     nsdk_setPushLocalSecond                    (int second);							// set the time(second) of local push message.
extern int     nsdk_getPushUseLocalSecond                 ();									// get the setting time of local push message.
extern int     nsdk_setPushLocalMessage                   (const char* msg);					// set the message of local push.
extern int     nsdk_setLocalNotification                  ();									// execure local push.
extern int     nsdk_setUseLocalNotification               (int bUse);							//
extern int     nsdk_getUseLocalPushNotification           ();									// retrive the local push message list.
extern int     nsdk_cancelLocalNotification               ();									// cancel local push message.

// push message saved result
extern size_t  nsdk_rsp_LocalPushCount    ();													// get the stored local push count. received 0~count
extern int     nsdk_rsp_LocalPushInfo     (int* notificationID, int* bUse, size_t idx);			// get the sotred local push info.  failed will be received NSDK_EFAIL



// for java native interface
#if defined(ANDROID_DEVICE) || defined(_WIN32)

extern JavaVM* jni_java_jvm                          ;											// virtual machine pointer

extern int     find_java_satic_method(JNIEnv**   ret_env										// java static method call for static method in java. success:0, failed: -1. return values are env, clzz, methodid
									, jclass*    ret_clz										// ret_env, ret_clz, ret_mid are return value.
									, jmethodID* ret_mid
									, const char* clzz, const char* method, const char* sig);	// class name, method name signature

extern int     find_java_satic_field(JNIEnv**   ret_env											// java static field call for static field in java. success:0, failed: -1. return values are env, clzz, fieldid
									, jclass*   ret_clz											// ret_env, ret_clz, ret_fid are return value.
									, jfieldID* ret_fid
									, const char* clzz, const char* field, const char* sig);	// class name, field name, and signature

extern jmethodID find_java_instance_method(JNIEnv* env											// java instance method call for static method in java. success: jmethodID, failed: null.
									, const char* clzz, const char* method, const char* sig);	// class name, method name signature

extern int     IB_UTF8ToANSI            (char* ansi, int len, const char* utf);
extern void    save_contract_agreement  (int v);
extern void    game_dialog_policy       (void );


#endif // ANDROID_DEVICE || _WIN32


//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


#define MULTI_TOUCH_COUNT   32

struct TIOS_TOUCH                       // for ios multi-touch
{
	int     n    ;                      // index
	float	x    ;                      // dest position
	float	y    ;                      // dest position
	float	cur_x;                      // position x
	float	cur_y;                      // position y
	float	prv_x;                      // position x
	float	prv_y;                      // position y
};

#if defined(IOS_MOBILE_DEVICE)

// glue for xcode.
// defined c++ source

int  appi_init(int w, int h, const char* s_resource, const char* s_accessible);
void appi_update();
void appi_render();
void appi_stop();
void appi_pause();
void appi_resume();
void appi_destroy();
void appi_restart();
void appi_backBtnDown();
void appi_touchEvent(int nEvent, float x_1, float y_1, float x_2, float y_2);

#endif // IOS_MOBILE_DEVICE


#endif


/**
using example

// notice window
     1. checkup the window state by nsdk_st_opened_shownoticeview
     2. using nsdk_showNoticeView(NSDK_LOCATION_NOTICE_INTRO);


if(condition:
	NSDK_WINDOW_CLOSED >= nsdk_st_opened_shownoticeview)
{
	nsdk_showNoticeView(NSDK_LOCATION_NOTICE_INTRO);
}
...

if(condition:
	NSDK_WINDOW_CLOSED >= nsdk_st_opened_shownoticeview)
{
	nsdk_showNoticeView(NSDK_LOCATION_NOTICE_INGAME);
}


// user id
  1. prepare the buffer ~ 256bytes
  2. call the nsdk_config_PlayerID
...
	// get the player id
	char	playerid[128]={0};
	nsdk_config_PlayerID(playerid);
	LOGD("AppRender:: nsdk_config_PlayerID: %s", playerid);


local push example
	nsdk_setPushLocalNotificationID(123);
	nsdk_setPushLocalSecond        (3);
	nsdk_setPushLocalMessage       ("Hello push message");
	int hr = nsdk_setLocalNotification();

push example
	std::vector<std::string>	lsPlayerID;
	char*	playerIDs[2048]={NULL, };
	int		count_playerid = (int)lsPlayerID.size();
	int     notificationID = 0x12344;

	const char* message = "Hello push message";


	nsdk_setSendPushPlayerIDClear();								// clear the player list in java

	for(int i=0; i<count_playerid; ++i)
	{
		const char* pid = lsPlayerID[i].c_str();
		nsdk_setSendPushPlayerIDAdd(pid);							// set player list in java
	}

	nsdk_setSendPushNotificationID(notificationID);					// set the notification push id in java
	nsdk_setSendPushMessage       (message);						// set the message in java

	nsdk_sendPushNotification     ();								// send the message without notification id
	nsdk_sendPushNotificationUseNotificationID ();					// send message


// softkeyboard example
//
nsobject* p_owner = .. get the ns object;

auto fnc = [](void* arg, int calling_id, int ret, const char* str, int len)
{
	LOGI("%s[%3d]: ret: %d  str[%2d]: %s", __func__, __LINE__, ret, len, str);

	// must be casting if you use the owner...
	nsobject* owner = reinterpret_cast<nsobject*>(arg);
	if(owner)
	{
		owner process...
	}
};
nsdk_showSoftKeyboard(fnc, p_owner, 0x12345, 40, 20, TEXT_FIELD_MAX_LENGH, NSDK_FALSE);


//
LOGD("AppRender::nsdk_connectToChannel(NSDK_CHANNEL_FACEBOOK)");				nsdk_connectToChannel(NSDK_CHANNEL_FACEBOOK);	}
LOGD("AppRender::nsdk_disconnectFromChannel(NSDK_CHANNEL_FACEBOOK)");			nsdk_disconnectFromChannel(NSDK_CHANNEL_FACEBOOK, NSDK_CHANNEL_OPTION_SELECT);	}
LOGD("AppRender::nsdk_requestFacebookMyProfile");								nsdk_requestFacebookMyProfile();}
LOGD("AppRender::nsdk_requestFacebookFriends");									nsdk_requestFacebookFriends();	}
LOGD("AppRender::nsdk_inviteFacebookFriends");									nsdk_inviteFacebookFriends(message, title);	}
LOGD("AppRender::nsdk_requestFacebookInviters");								nsdk_requestFacebookInviters();	}

LOGD("AppRender::nsdk_deleteFacebookInviters");									nsdk_deleteFacebookInviters();	}

LOGD("AppRender::nsdk_connectToChannel(NSDK_CHANNEL_GOOGLE_PLUS)"     );		nsdk_connectToChannel(NSDK_CHANNEL_GOOGLE_PLUS);	}
LOGD("AppRender::nsdk_disconnectFromChannel(NSDK_CHANNEL_GOOGLE_PLUS)");		nsdk_disconnectFromChannel(NSDK_CHANNEL_GOOGLE_PLUS);	}
LOGD("AppRender::nsdk_requestGooglePlusMyProfile");								nsdk_requestGooglePlusMyProfile();}
LOGD("AppRender::nsdk_requestGooglePlusFriends"  );								nsdk_requestGooglePlusFriends();	}

LOGD("AppRender::nsdk_connectToChannel(NSDK_CHANNEL_EVERY_NETMARBLE)"     );	nsdk_connectToChannel(NSDK_CHANNEL_EVERY_NETMARBLE);	}
LOGD("AppRender::nsdk_disconnectFromChannel(NSDK_CHANNEL_EVERY_NETMARBLE)");	nsdk_disconnectFromChannel(NSDK_CHANNEL_EVERY_NETMARBLE);	}
LOGD("AppRender::nsdk_requestEveryNetmarbleMyProfile");							nsdk_requestEveryNetmarbleMyProfile();}
LOGD("AppRender::nsdk_requestEveryNetmarbleFriends"  );							nsdk_requestEveryNetmarbleFriends();	}


// channel loggined count
size_t channel_loggined_count = nsdk_config_LoginnedCount();


// channel ID
if(0<channel_loggined_count)
{
	LOGI("----Render:: nsdk_config_LoginnedCount: %d", channel_loggined_count);
	for(int i=0; i<NSDK_CHANNEL_TOT; ++i)
	{
		char  channel_id[NSDK_LENGTH_CHANNEL_ID]={0};
		nsdk_config_ChannelID(channel_id, (size_t)i);

		if(0 < strlen(channel_id))
		{
			LOGI("channel[%d]: %s", i, channel_id);
		}
	}
}

*/

