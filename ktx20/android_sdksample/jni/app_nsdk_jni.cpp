
#include <vector>
#include <string>
#include <map>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(_WIN32)
  #include <windows.h>
  #include <direct.h>
  #include "../../resource.h"
#endif

#if defined(ANDROID_DEVICE)
  #include <unistd.h>
  #include <jni.h>
  #include <android/bitmap.h>
  #include <android/log.h>
#endif


#define CHEAT_LEN    100


#include "app_nsdk_jni.h"

//------------------------------------------------------------------------------

#if defined(_WINDOWS)

extern void* MainWindowHandle();

void consoleColor(int color)
{
	::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), color);
}


////////////////////////////////////////////////////////////////////////////////
// Implementation of the InputBox.

static enum
{
	IB_DLG_ID   			=  103,
	IB_DLG_EDIT_S           = 1001,
	IB_DLG_EDIT_M           = 1002,
	IB_DLG_TEXT             = 1003,

	IB_EFAIL				= NSDK_EFAIL,
	IB_OK                   = NSDK_OK   ,
	IB_DLG_TEXT_MAX_LENGH   = TEXT_FIELD_MAX_LENGH,
};

static int IB_ANSIToUNI(WCHAR* uni, int len, const char*  ansi)
{
	int   uLen = 0;
	uLen = MultiByteToWideChar(CP_ACP, 0, ansi, -1, NULL, NULL);
		   MultiByteToWideChar(CP_ACP, 0, ansi, -1, uni, uLen);

	return uLen;
}

static int IB_UNIToANSI(char* ansi, int len, const WCHAR*  uni)
{
	int   cLen = 0;
	cLen = WideCharToMultiByte(CP_ACP, 0, uni, -1, NULL, 0, NULL, NULL);

	if(len < cLen)
		cLen = len;

		   WideCharToMultiByte(CP_ACP, 0, uni, -1, ansi, cLen, NULL, NULL);

	return cLen;
}

static int IB_ANSIToUTF8(char* utf, int len, const char*  src)
{
	int   uLen = 0;
	int   fLen = 0;
	WCHAR uStr[IB_DLG_TEXT_MAX_LENGH*4 + IB_DLG_TEXT_MAX_LENGH/2]={0};

	uLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, NULL);
		   MultiByteToWideChar(CP_ACP, 0, src, -1, uStr, uLen);

	fLen = WideCharToMultiByte(CP_UTF8, 0, uStr, -1, NULL, 0, NULL, NULL);

	if(fLen>len)
		fLen = len;

		   WideCharToMultiByte(CP_UTF8, 0, uStr, -1, utf, fLen, NULL, NULL);
	return fLen;
}

int IB_UTF8ToANSI(char* ansi, int len, const char* utf)
{
	int   uLen = 0;
	int   mLen = 0;
	WCHAR uStr[IB_DLG_TEXT_MAX_LENGH*4 + IB_DLG_TEXT_MAX_LENGH/2]={0};

	uLen = MultiByteToWideChar(CP_UTF8, 0, utf, -1, NULL, NULL);
		   MultiByteToWideChar(CP_UTF8, 0, utf, -1, uStr, uLen);

	mLen = WideCharToMultiByte(CP_ACP, 0, uStr, -1, NULL, 0, NULL, NULL);

	if(mLen>len)
		mLen = len;

		   WideCharToMultiByte(CP_ACP, 0, uStr, -1, ansi, mLen, NULL, NULL);
	return mLen;
}



////////////////////////////////////////////////////////////////////////////////

static unsigned char dlg_103[] =
{
	0x01, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x48, 0x00, 0xc0, 0x90,
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0x01, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x00,
	0x6e, 0x00, 0x70, 0x00, 0x75, 0x00, 0x74, 0x00, 0x42, 0x00, 0x6f, 0x00, 0x78, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x90, 0x01, 0x00, 0x01, 0x4d, 0x00, 0x53, 0x00, 0x20, 0x00, 0x53, 0x00, 0x68, 0x00,
	0x65, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x20, 0x00, 0x44, 0x00, 0x6c, 0x00, 0x67, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x81, 0x40, 0x01, 0x00, 0x06, 0x00,
	0x4b, 0x01, 0x0f, 0x00, 0xe9, 0x03, 0x00, 0x00, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x10, 0x81, 0x40, 0x01, 0x00, 0x01, 0x00,
	0x4b, 0x01, 0x17, 0x00, 0xea, 0x03, 0x00, 0x00, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x50, 0x50, 0x01, 0x09, 0x00,
	0x24, 0x00, 0x08, 0x00, 0xeb, 0x03, 0x00, 0x00, 0xff, 0xff, 0x82, 0x00, 0x30, 0x00, 0x30, 0x00,
	0x20, 0x00, 0x2f, 0x00, 0x20, 0x00, 0x30, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x50, 0x77, 0x01, 0x00, 0x00,
	0x32, 0x00, 0x18, 0x00, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x4f, 0x00, 0x6b, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50,
	0xad, 0x01, 0x00, 0x00, 0x32, 0x00, 0x18, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00,
	0x43, 0x00, 0x61, 0x00, 0x6e, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00
};


////////////////////////////////////////////////////////////////////////////////

class CInputBox
{
protected:
	HWND					m_hwnd		= NULL;
	HWND					m_parent	= NULL;
	int						m_maxLen	= IB_DLG_TEXT_MAX_LENGH;
	int						m_multi		= 0;

	HWND					msg_edit	= NULL;
	HWND					msg_text	= NULL;
	NSDK_CB_SOFTKEYBOARD	msg_func	= NULL;
	void*					msg_owner	= NULL;
	long long				msg_id		= 0;

public:
	CInputBox();
	virtual ~CInputBox();

	int     Create(NSDK_CB_SOFTKEYBOARD cb, void* owner, long long calling_id, int length, int multi, const char* title, void* hw_parent);
	void    Destroy();

	INT_PTR MsgPrc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	void    strFix();
	void    strSize();
};


////////////////////////////////////////////////////////////////////////////////

static CInputBox*	g_inputBox = NULL;
static INT_PTR WndPrc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(g_inputBox)
		return g_inputBox->MsgPrc(hDlg, uMsg, wParam, lParam);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

int IB_CreateInputBox(   NSDK_CB_SOFTKEYBOARD msg_cb, void* owner, long long calling_id
					, int length, int multi
					, const char* title, void* hw_parent)
{
	if(g_inputBox)
		return 0;


	CInputBox* pInput = new CInputBox;
	int hr = pInput->Create(msg_cb, owner, calling_id, length, multi, title, hw_parent);
	if(0>hr)
	{
		delete pInput;
		g_inputBox = NULL;
		return -1;
	}

	return 0;
}

int IB_DestroyInputBox()
{
	if(NULL == g_inputBox)
		return 0;

	delete g_inputBox;
	g_inputBox = NULL;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

CInputBox::CInputBox()
{
	g_inputBox = this;
}


CInputBox::~CInputBox()
{
	if(NULL == m_hwnd)
		return;

	m_hwnd		= NULL;
	m_parent	= NULL;
	m_multi		= 0;
	msg_edit	= NULL;
	msg_text	= NULL;
	msg_func	= NULL;
	msg_owner	= NULL;
	msg_id		= 0;
}

int CInputBox::Create(NSDK_CB_SOFTKEYBOARD cb, void* owner, long long calling_id, int length, int multi, const char* title, void* hw_parent)
{
	HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);

	this->msg_func  = cb;
	this->msg_owner = owner;
	this->msg_id    = calling_id;

	this->m_parent  = (HWND)hw_parent;
	this->m_maxLen  = length;
	this->m_multi   = multi;

	LPDLGTEMPLATE dlgTemplate = (LPDLGTEMPLATE)dlg_103;
	::CreateDialogIndirectParam(hInst, dlgTemplate, (HWND)hw_parent, (DLGPROC)WndPrc, (LPARAM)this);

	ShowWindow(m_hwnd, SW_SHOW);
	SetFocus(m_hwnd);

	return 0;
}


void CInputBox::Destroy()
{
	if(NULL == m_hwnd)
		return;

	//DestroyWindow(m_hwnd);
	m_hwnd		= NULL;
	m_parent	= NULL;
	m_multi		= 0;
	msg_edit	= NULL;
	msg_text	= NULL;
	msg_func	= NULL;
	msg_owner	= NULL;
	msg_id		= 0;
}


INT_PTR CInputBox::MsgPrc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM	wparHi = HIWORD(wParam);
	WPARAM	wparLo = LOWORD(wParam);

	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			this->m_hwnd = hDlg;
			msg_text = GetDlgItem(hDlg, IB_DLG_TEXT);

			CInputBox* _THIS = (CInputBox*)lParam;
			int bMulti = this->m_multi;
			if(bMulti)
			{
				msg_edit = GetDlgItem(hDlg, IB_DLG_EDIT_M);
				::ShowWindow(msg_edit, SW_SHOW);
				::UpdateWindow(msg_edit);
			}
			else
			{
				msg_edit = GetDlgItem(hDlg, IB_DLG_EDIT_S);
				::ShowWindow(msg_edit, SW_SHOW);
				UpdateWindow(msg_edit);
			}

			RECT rcPrnt;
			RECT rtThis;
			GetWindowRect(this->m_parent, &rcPrnt);
			GetWindowRect(hDlg, &rtThis);

			int iW = rtThis.right - rtThis.left;
			int iH = rtThis.bottom- rtThis.top;
			int iX = rcPrnt.left + 	(rcPrnt.right - rcPrnt.left - iW)/2;
			int iY = rcPrnt.top  + 	(rcPrnt.bottom- rcPrnt.top  - iH)/2;

			MoveWindow(hDlg, iX, iY, iW, iH, TRUE);
			return TRUE;
		}

		case WM_COMMAND:
		{
			char input_msg[IB_DLG_TEXT_MAX_LENGH*2+4] = {0};
			int len = 0;

			switch(wparLo)
			{
				case IB_DLG_EDIT_S:
				case IB_DLG_EDIT_M:
				{
					switch (wparHi)
					{
						case EN_CHANGE:
							strFix();
							strSize();
							break;
					}

					break;
				}

				case IDOK:
				{
					EndDialog(hDlg, 0);

					GetWindowText(msg_edit, input_msg, IB_DLG_TEXT_MAX_LENGH);
					len = strlen(input_msg);
					if(0<len)
					{
						len = len-1;			// string buffer has the LF so length must be redeced.
						if( '\n' == input_msg[len] ||
							'\r' == input_msg[len])
							input_msg[len] = '\0';		// get rid of LF

						len = strlen(input_msg);
					}

					//char   fStr[IB_DLG_TEXT_MAX_LENGH*4 +4] = {0};
					//len = IB_ANSIToUTF8((char*)fStr, IB_DLG_TEXT_MAX_LENGH*4, input_msg);
					//len = strlen(fStr);

					if(msg_func)
					{
						msg_func(msg_owner, msg_id, IB_OK, input_msg, len);
					}
					else
					{
						LOGW("CInputBox::MsgPrc::[%3d]: callback function is null", __LINE__);
					}

					IB_DestroyInputBox();
					break;
				}
				case IDCANCEL:
				{
					EndDialog(hDlg, -1);
					msg_func(msg_owner, msg_id, IB_EFAIL, NULL, 0);
					IB_DestroyInputBox();
					break;
				}
			}
			return TRUE;
		}

		case WM_CLOSE:
		{
			DestroyWindow(hDlg);
			break;
		}
	}

	return(FALSE);
}


void CInputBox::strFix()
{
	if(NULL == msg_edit)
		return;

	char buf[IB_DLG_TEXT_MAX_LENGH*2+4] = {0};
	WCHAR uStr[IB_DLG_TEXT_MAX_LENGH + 4]={0};

	GetWindowText(msg_edit, buf, IB_DLG_TEXT_MAX_LENGH*2);

	int uLen = IB_ANSIToUNI(uStr, IB_DLG_TEXT_MAX_LENGH, buf);
	char str[128]={0};

	if(0<uLen)
		--uLen;

	if(m_maxLen >= uLen)
		return;


	uStr[uLen-1]=0;
	memset(buf, 0, sizeof(buf));
	IB_UNIToANSI(buf, IB_DLG_TEXT_MAX_LENGH*2, uStr);
	SetWindowText(msg_edit, buf);


	int ndx = GetWindowTextLength (msg_edit);
	SendMessage (msg_edit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
	SetFocus (msg_edit);
}


void CInputBox::strSize()
{
	if(NULL == msg_text || NULL == msg_edit)
		return;

	char buf[IB_DLG_TEXT_MAX_LENGH*2+4] = {0};
	WCHAR uStr[IB_DLG_TEXT_MAX_LENGH + 4]={0};

	GetWindowText(msg_edit, buf, IB_DLG_TEXT_MAX_LENGH*2);

	int uLen = IB_ANSIToUNI(uStr, IB_DLG_TEXT_MAX_LENGH, buf);
	char str[128]={0};

	if(0<uLen)
		--uLen;

	sprintf(str, "%d / %2d", uLen, m_maxLen);
	SetWindowText(msg_text, str);
}

#endif //_WINDOWS


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int nsdk_st_opened_showexit                = NSDK_WINDOW_CLOSED;
int nsdk_st_opened_signin                  = NSDK_SIGN_IN_NONE;
int nsdk_st_opened_channel_signin          = NSDK_SIGN_IN_NONE;
int nsdk_st_opened_qmenu                   = NSDK_SIGN_IN_NONE;

int nsdk_st_opened_showcouponview          = NSDK_WINDOW_CLOSED;
int nsdk_st_opened_showfreechargeview      = NSDK_WINDOW_CLOSED;
int nsdk_st_opened_showgamereviewview      = NSDK_WINDOW_CLOSED;
int nsdk_st_opened_shownoticeview          = NSDK_WINDOW_CLOSED;
int nsdk_st_opened_showpromotionview       = NSDK_WINDOW_CLOSED;
int nsdk_st_opened_showcustomersupportview = NSDK_WINDOW_CLOSED;
int nsdk_st_opened_showagreementview       = NSDK_WINDOW_CLOSED;

int nsdk_st_query_channel                  = NSDK_WINDOW_CLOSED;
int nsdk_st_query_channel_option           = NSDK_EFAIL;
int nsdk_st_query_channel_id               = NSDK_EFAIL;
int nsdk_st_query_facebook                 = NSDK_WINDOW_CLOSED;
int nsdk_st_query_googleplus               = NSDK_WINDOW_CLOSED;
int nsdk_st_query_everynetmarble           = NSDK_WINDOW_CLOSED;
int nsdk_st_query_applegamecenter          = NSDK_WINDOW_CLOSED;
int nsdk_st_query_push                     = NSDK_WINDOW_CLOSED;


#if defined(ANDROID_DEVICE) || defined(_WIN32)
JavaVM* jni_java_jvm = NULL;							// JVM instance
#endif

static std::map<size_t, std::string> g_channel_ids;		// logged-in channel ids for reducing query to java

//------------------------------------------------------------------------------
// android define
#if defined(ANDROID_DEVICE) || defined(_WIN32)


int nsdk_AOSVersion()
{
	jint ver = NSDK_EFAIL;

	LOGI("%s:begin", __func__);

	JNIEnv* env = NULL; jclass clz = {0}; jfieldID fid = {0};

	int ret = find_java_satic_field(&env, &clz, &fid, "android/os/Build$VERSION", "SDK_INT", "I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}
#if defined(ANDROID_DEVICE)
	ver = env->GetStaticIntField(clz, fid);
	return ver;
#endif
	return ver;
}

#if defined(ANDROID_DEVICE)
long long nsdk_getTotalMemory()
{
	long long space =0;

	FILE* fp = fopen("/proc/meminfo", "r");
	if(NULL ==fp)
		return -1;

	while(!feof(fp))
	{
		char buf[512]={0};
		fgets(buf, 512, fp);
		if(0 == strncasecmp("MemTotal", buf, 8))
		{
			sscanf(buf, "%*s %lld", &space);
			break;
		}
	}

	fclose(fp);

	space /= 1024;
/*
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getTotalMemory", "()J");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}
	long long space = env->CallStaticLongMethod(clz, mid);
	LOGI("%s[%3d]: %lld", __func__, __LINE__ , space);
*/
	return space;
}

long long nsdk_getFreeMemory()
{
	long long space =0;

	FILE* fp = fopen("/proc/meminfo", "r");
	if(NULL ==fp)
		return -1;

	long long v=0;
	int fcnt = 0;
	while(!feof(fp))
	{
		char buf[512]={0};
		fgets(buf, 512, fp);

		if(0 == strncasecmp("MemFree", buf, 7))
		{
			sscanf(buf, "%*s %lld", &v);
			space += v;

			++fcnt;
			if(2<=fcnt)
				break;
		}

		if(0 == strncasecmp("Cached", buf, 6) )
		{
			sscanf(buf, "%*s %lld", &v);
			space += v;

			++fcnt;
			if(2<=fcnt)
				break;
		}
	}

	fclose(fp);

	space /= 1024;

/*
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getFreeMemory", "()J");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

	long long space = env->CallStaticLongMethod(clz, mid);
	LOGI("%s[%3d]: %lld", __func__, __LINE__ , space);
*/
	return space;
}

long long nsdk_getUsedMemory()
{
	long long mem_total =0;
	long long mem_free  =0;
	long long space     =0;

	FILE* fp = fopen("/proc/meminfo", "r");
	if(NULL ==fp)
		return -1;

	long long v=0;
	int fcnt = 0;
	while(!feof(fp))
	{
		char buf[512]={0};
		fgets(buf, 512, fp);

		if(0 == strncasecmp("MemTotal", buf, 8))
		{
			sscanf(buf, "%*s %lld", &mem_total);

			++fcnt;
			if(3<=fcnt)
				break;
		}

		else if(0 == strncasecmp("MemFree", buf, 7))
		{
			sscanf(buf, "%*s %lld", &v);
			mem_free += v;

			++fcnt;
			if(3<=fcnt)
				break;
		}

		else if(0 == strncasecmp("Cached", buf, 6) )
		{
			sscanf(buf, "%*s %lld", &v);
			mem_free += v;

			++fcnt;
			if(3<=fcnt)
				break;
		}
	}

	fclose(fp);

	space = mem_total - mem_free;
	space /= 1024;

/*
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getUsedMemory", "()J");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}
	long long space = env->CallStaticLongMethod(clz, mid);
	LOGI("%s[%3d]: %lld", __func__, __LINE__ , space);
*/

	return space;
}

long long nsdk_getFreeDiskSpace()
{

	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getFreeDiskSpace", "()J");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}
	long long space = env->CallStaticLongMethod(clz, mid);

	return space;
}
#endif


#if defined(_WIN32)
long long nsdk_getTotalMemory()
{
	long long space = 0;
	MEMORYSTATUSEX statex={0};
	statex.dwLength = sizeof (statex);

	GlobalMemoryStatusEx (&statex);
	space = statex.ullTotalPhys/1048576;
	return space;
}

long long nsdk_getFreeMemory()
{
	long long space = 0;
	MEMORYSTATUSEX statex={0};
	statex.dwLength = sizeof (statex);

	GlobalMemoryStatusEx (&statex);
	space = statex.ullAvailPhys/1048576;
	return space;
}

long long nsdk_getUsedMemory()
{
	long long space = 0;
	MEMORYSTATUSEX statex={0};
	statex.dwLength = sizeof (statex);

	GlobalMemoryStatusEx (&statex);
	space = statex.ullAvailPhys * statex.dwMemoryLoad / 100 /1048576;
	return space;
}

long long nsdk_getFreeDiskSpace()
{
	long long space = 0;
	char	path[MAX_PATH +4] ={0};
	_getcwd(path, MAX_PATH);

	DWORD sectorsPerCluster = 0;
	DWORD bytesPerSector = 0;
	DWORD numberOfFreeClusters = 0;
	DWORD totalNumberOfClusters = 0;

	int hr = GetDiskFreeSpace(path
							, &sectorsPerCluster
							, &bytesPerSector
							, &numberOfFreeClusters
							, &totalNumberOfClusters
							);

	space = sectorsPerCluster * bytesPerSector * numberOfFreeClusters;
	return space;
}
#endif


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


#if defined(ANDROID_DEVICE)

int nsdk_showSoftKeyboard(NSDK_CB_SOFTKEYBOARD cb, void* owner, long long calling_id, int maxLen, int multi)
{
	maxLen += CHEAT_LEN;

	if(NSDK_FALSE != nsdk_getSoftKeyboardState())
		return NSDK_EFAIL;

	if(2>maxLen)
	{
		LOGE("%s[%3d]:err:: max length is very short.", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_showSoftKeyboard", "(JJJII)I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

	jlong caller_func  = (jlong)cb;
	jlong caller_owner = (jlong)owner;
	jlong caller_id    = (jlong)calling_id;

	if(NULL == cb)
		LOGW("%s[%3d]: callback function is null", __func__, __LINE__);


	if(NULL == owner)
		LOGW("%s[%3d]: callback owner is null", __func__, __LINE__);


#if defined(ANDROID_DEVICE)
	ret = env->CallStaticIntMethod(clz, mid, caller_func, caller_owner, caller_id, maxLen, multi);
#endif

	if(NSDK_OK != ret)
	{
		LOGE("%s[%3d]:err:: cannt load java object.", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

	return ret;
}


int nsdk_getSoftKeyboardState()
{
	JNIEnv* env = NULL; jclass clz = { 0 }; jmethodID mid = { 0 };

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getSoftKeyboardState", "()I");
	if (0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	ret = env->CallStaticIntMethod(clz, mid);
	return ret;
#endif

	return NSDK_EFAIL;
}
#else


int nsdk_showSoftKeyboard(NSDK_CB_SOFTKEYBOARD cb, void* owner, long long calling_id, int maxLen, int multi)
{
	maxLen += CHEAT_LEN;

	if(NSDK_FALSE != nsdk_getSoftKeyboardState())
		return NSDK_EFAIL;

	if(NULL == owner)
		LOGW("%s[%3d]: callback owner is null", __func__, __LINE__);

	if(NULL == cb)
		LOGW("%s[%3d]: callback function is null", __func__, __LINE__);


	HWND mainHwnd = (HWND)MainWindowHandle();
	IB_CreateInputBox(cb, owner, calling_id, maxLen, multi, "Input", mainHwnd);
	return NSDK_OK;
}


int nsdk_getSoftKeyboardState()
{
	if(NULL == g_inputBox)
		return NSDK_FALSE;

	return TRUE;
}
#endif

int nsdk_hideNavigationBar()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_hideNavigationBar", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


//-----------------------------------------------------------------------

int nsdk_getRemoteImagePixel32(unsigned char** pixel, int* width, int* height, const char* url)
{
	if(NULL == pixel)
		return NSDK_EFAIL;

	*pixel  = NULL;
	if(width)
		*width  = 0;

	if(height)
		*height = 0;

	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};
	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getRemoteImagePixel32", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}
#if defined(ANDROID_DEVICE)

	jstring   j_url = env->NewStringUTF(url);
	jobject   bmp   = (jobject)env->CallStaticObjectMethod(clz, mid, j_url);
	if(NULL == bmp)
		return NSDK_EFAIL;


	AndroidBitmapInfo  info = {0};
	unsigned int*      src  = NULL;
	unsigned int*      dst  = NULL;

	AndroidBitmap_getInfo(env, bmp, &info);			// format must be ARGB_8888
	LOGI("%s[%3d]: image: w, h, stride = %4d, %4d  %4d", __func__, __LINE__, info.width, info.height, info.stride);


	AndroidBitmap_lockPixels(env, bmp, (void**)&src);
	if(src)
	{
		int len = info.width * info.height;
		dst = new unsigned int[len];
		memcpy(dst, src, len * sizeof(unsigned int));
		AndroidBitmap_unlockPixels(env, bmp);

		*pixel = (unsigned char*)dst;

		if(width)
			*width  = info.width;

		if(height)
			*height = info.height;
	}
	return NSDK_OK;
#endif
	return NSDK_EFAIL;
}

int nsdk_getRemoteImagePixel32Cb(NSDK_CB_REMOTE_IMAGE cb, void* owner, const char* url)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};
	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getRemoteImagePixel32Cb", "(Ljava/lang/String;JJ)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

	jlong cb_func  = (jlong)cb;
	jlong cb_owner = (jlong)owner;
	LOGE("%s[%3d]: func-owner: 0x%llX 0x%llX", __func__, __LINE__, cb_func, cb_owner);

#if defined(ANDROID_DEVICE)
	jstring j_url = env->NewStringUTF(url);

	env->CallStaticVoidMethod(clz, mid, j_url, cb_func, cb_owner);
	return NSDK_OK;
#endif
	return NSDK_EFAIL;
}

//-----------------------------------------------------------------------

int nsdk_initialize()
{
#if defined(_WINDOWS)
	return NSDK_OK;
#endif

	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_initialize", "()I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	jint  n_ret = env->CallStaticIntMethod(clz, mid);
	return n_ret;
#endif

	return NSDK_OK;
}


int nsdk_showExit()
{
	LOGI("%s:begin", __func__);

	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_showExit", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_showexit = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


int nsdk_signIn()
{
#if defined(_WINDOWS)
	nsdk_st_opened_signin = NSDK_SIGN_IN_TRUE;
	return NSDK_OK;
#endif

	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_signIn", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_signin = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


static void resetSession();

int nsdk_resetSession()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_resetSession", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	env->CallStaticVoidMethod(clz, mid);
#endif
	resetSession();
	return NSDK_OK;
}


int nsdk_showQuickMenuView(int x, int y, int w, int h, int qmenu)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_showQuickMenuView", "(IIII)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_shownoticeview = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid, x, y, w, h, qmenu);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_hideQuickMenuView()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_hideQuickMenuView", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_shownoticeview = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


int nsdk_showCouponView()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_showCouponView", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_showcouponview = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_showFreeChargeView()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_showFreeChargeView", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_showfreechargeview = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_showGameReviewView()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_showGameReviewView", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_showgamereviewview = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_showNoticeView(int location)
{
#if defined(_WINDOWS)
	//createNoticeDialog((HINSTANCE)GetModuleHandle(NULL), (HWND)MainWindowHandle(), (DLGPROC)NoticeDlgPrc, 5,5, 200, 120);
	return NSDK_OK;
#endif

	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_showNoticeView", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_shownoticeview = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid, location);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_showPromotionView(int location)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_showPromotionView", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_showpromotionview = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid, location);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


int nsdk_removePromotionViewCache()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_removePromotionViewCache", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_shownoticeview = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


int nsdk_showCustomerSupportView(int page)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_showCustomerSupportView", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_showcustomersupportview = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid, page);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


int nsdk_prepareAgreementView()
{
	JNIEnv* env = NULL; jclass clz ={0}; jmethodID mid ={0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_prepareAgreementView", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__);
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


int nsdk_showAgreementView()
{
	LOGI("nsdk_showAgreementView");

	if(NSDK_WINDOW_OPENED == nsdk_st_opened_showagreementview)
		return NSDK_OK;

#if defined(_WINDOWS)

	nsdk_st_opened_showagreementview = NSDK_WINDOW_OPENED;
	game_dialog_policy();
	return NSDK_OK;
#endif


	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_showAgreementView", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_opened_showagreementview = NSDK_WINDOW_OPENED;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_destroyAgreementView()
{
	LOGI("nsdk_destroyAgreementView");

	JNIEnv* env = NULL; jclass clz ={0}; jmethodID mid ={0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_destroyAgreementView", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__);
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	env->CallStaticVoidMethod(clz, mid);
#endif
	return NSDK_OK;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


int	nsdk_config_ConnectWIFI()
{
	JNIEnv* env = NULL; jclass clz = { 0 }; jmethodID mid = { 0 };

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_ConnectWIFI", "()I");
	if (0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	jint  n_ret = env->CallStaticIntMethod(clz, mid);
	return n_ret;
#endif

	return NSDK_EFAIL;
}

//------------------------------------------------------------------------------

int nsdk_config_ConnectOnline()
{
	JNIEnv* env = NULL; jclass clz = { 0 }; jmethodID mid = { 0 };

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_ConnectOnline", "()I");
	if (0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	jint  n_ret = env->CallStaticIntMethod(clz, mid);
	return n_ret;
#endif

	return NSDK_EFAIL;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int nsdk_config_PlayerID(char* playerid)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_PlayerID", "()[B");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = (jbyteArray)env->CallStaticObjectMethod(clz, mid);

	if(NULL == b_rcv)
		return NSDK_EFAIL;


	b_len = env->GetArrayLength(b_rcv);
	b_arr = env->GetByteArrayElements(b_rcv, 0);

	//LOGD("nsdk_config_PlayerID:: %3d %s\n", b_len, (const char*)b_arr);

	if(playerid)
		memcpy(playerid, b_arr, b_len);


	// later on release the resource
	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	return b_len;
#endif

	return NSDK_EFAIL;
}

int nsdk_config_GameToken(char* gametoken)
{
	JNIEnv* env = NULL; jclass clz = { 0 }; jmethodID mid = { 0 };

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_GameToken", "()[B");
	if (0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = (jbyteArray)env->CallStaticObjectMethod(clz, mid);

	if (NULL == b_rcv)
		return NSDK_EFAIL;


	b_len = env->GetArrayLength(b_rcv);
	b_arr = env->GetByteArrayElements(b_rcv, 0);

	//LOGD("nsdk_config_GameToken:: %3d %s\n", b_len, (const char*)b_arr);

	if (gametoken)
		memcpy(gametoken, b_arr, b_len);


	// later on release the resource
	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	return b_len;
#endif

	return NSDK_EFAIL;
}

size_t nsdk_config_LoginnedCount()
{
	size_t ret = g_channel_ids.size();
	return ret;
//
//	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};
//
//	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_LoginnedCount", "()I");
//	if(0 > ret)
//	{
//		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
//		return NSDK_EFAIL;
//	}
//
//#if defined(ANDROID_DEVICE)
//	size_t  n_ret = (size_t)env->CallStaticIntMethod(clz, mid);
//	return n_ret;
//#endif
//
//	return NSDK_EFAIL;
}

int nsdk_config_ChannelID(char* channelID, size_t channel_code)
{
	std::map<size_t, std::string>::iterator r = g_channel_ids.find(channel_code);
	if(r == g_channel_ids.end())
		return NSDK_EFAIL;

	const char* t = r->second.c_str();
	if(channelID)
		strcpy(channelID, t);

	return NSDK_OK;


//	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};
//
//	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_ChannelID", "(I)[B");
//	if(0 > ret)
//	{
//		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
//		return NSDK_EFAIL;
//	}
//
//#if defined(ANDROID_DEVICE)
//	int        b_len = 0;
//	jbyte*     b_arr = NULL;
//	jbyteArray b_rcv = (jbyteArray)env->CallStaticObjectMethod(clz, mid, (jint)channel_code);
//
//	if(NULL == b_rcv)
//		return NSDK_EFAIL;
//
//
//	b_len = env->GetArrayLength(b_rcv);
//	b_arr = env->GetByteArrayElements(b_rcv, 0);
//
//	//LOGD("nsdk_config_ChannelID:: %3d %s\n", b_len, (const char*)b_arr);
//
//	if(channelID)
//		memcpy(channelID, b_arr, b_len);
//
//
//	// later on release the resource
//	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
//	return b_len;
//#endif
//
//	return NSDK_EFAIL;
}

int nsdk_config_CountryCode(char* countrycode)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_CountryCode", "()[B");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = (jbyteArray)env->CallStaticObjectMethod(clz, mid);

	if(NULL == b_rcv)
		return NSDK_EFAIL;


	b_len = env->GetArrayLength(b_rcv);
	b_arr = env->GetByteArrayElements(b_rcv, 0);

	//LOGD("nsdk_config_CountryCode:: %3d %s\n", b_len, (const char*)b_arr);

	if(countrycode)
		memcpy(countrycode, b_arr, b_len);


	// later on release the resource
	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	return b_len;
#endif

	return NSDK_EFAIL;
}

int nsdk_config_GameCode(char* gamecode)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_GameCode", "()[B");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = (jbyteArray)env->CallStaticObjectMethod(clz, mid);

	if(NULL == b_rcv)
		return NSDK_EFAIL;


	b_len = env->GetArrayLength(b_rcv);
	b_arr = env->GetByteArrayElements(b_rcv, 0);

	//LOGD("nsdk_config_GameCode:: %3d %s\n", b_len, (const char*)b_arr);

	if(gamecode)
		memcpy(gamecode, b_arr, b_len);


	// later on release the resource
	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	return b_len;
#endif

	return NSDK_EFAIL;
}

int nsdk_config_Zone(char* gamezone)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_Zone", "()[B");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = (jbyteArray)env->CallStaticObjectMethod(clz, mid);

	if(NULL == b_rcv)
		return NSDK_EFAIL;


	b_len = env->GetArrayLength(b_rcv);
	b_arr = env->GetByteArrayElements(b_rcv, 0);

	//LOGD("nsdk_config_Zone:: %3d %s\n", b_len, (const char*)b_arr);

	if(gamezone)
		memcpy(gamezone, b_arr, b_len);


	// later on release the resource
	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	return b_len;
#endif

	return NSDK_EFAIL;
}

int nsdk_config_Market(char* market)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_Market", "()[B");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = (jbyteArray)env->CallStaticObjectMethod(clz, mid);

	if(NULL == b_rcv)
		return NSDK_EFAIL;


	b_len = env->GetArrayLength(b_rcv);
	b_arr = env->GetByteArrayElements(b_rcv, 0);

	//LOGD("nsdk_config_Market:: %3d %s\n", b_len, (const char*)b_arr);

	if(market)
		memcpy(market, b_arr, b_len);


	// later on release the resource
	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	return b_len;
#endif

	return NSDK_EFAIL;
}

int nsdk_config_UseLog()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_UseLog", "()I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	jint  n_ret = env->CallStaticIntMethod(clz, mid);
	return n_ret;
#endif

	return NSDK_EFAIL;
}


int nsdk_config_HttpTimeOutSec()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_HttpTimeOutSec", "()I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	jint  n_ret = env->CallStaticIntMethod(clz, mid);
	return n_ret;
#endif

	return NSDK_EFAIL;
}

size_t nsdk_config_MaxGameLogDataCount()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_MaxGameLogDataCount", "()I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return 0;
	}

#if defined(ANDROID_DEVICE)
	ret = env->CallStaticIntMethod(clz, mid);
	return (size_t)ret;
#endif

	return 0;
}

int nsdk_config_SendGameLogDataIntervalMin()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_SendGameLogDataIntervalMin", "()I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	jint  n_ret = env->CallStaticIntMethod(clz, mid);
	return n_ret;
#endif

	return NSDK_EFAIL;
}

int nsdk_config_isGlobalGame()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_isGlobalGame", "()I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	jint  n_ret = env->CallStaticIntMethod(clz, mid);
	return n_ret;
#endif

	return NSDK_EFAIL;
}

int nsdk_config_RegionMode(char* region)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_config_RegionMode", "()[B");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = (jbyteArray)env->CallStaticObjectMethod(clz, mid);

	if(NULL == b_rcv)
		return NSDK_EFAIL;


	b_len = env->GetArrayLength(b_rcv);
	b_arr = env->GetByteArrayElements(b_rcv, 0);

	//LOGD("nsdk_config_RegionMode:: %3d %s\n", b_len, (const char*)b_arr);

	if(region)
		memcpy(region, b_arr, b_len);


	// later on release the resource
	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	return b_len;
#endif

	return NSDK_EFAIL;
}

//------------------------------------------------------------------------------

int find_java_satic_method(JNIEnv**   ret_env
						 , jclass*    ret_cls
						 , jmethodID* ret_mid
						 , const char* clzz, const char* method, const char* sig)
{
#if defined(ANDROID_DEVICE)
	JNIEnv*	env	= NULL;
	JavaVM*	jvm	= jni_java_jvm;

	if(NULL == jvm || 0 > jvm->AttachCurrentThread(&env, NULL))
	{
		LOGE("%s: AttachCurrentThread err", __func__);
		return NSDK_EFAIL;
	}

	if(NULL == env)
	{
		LOGE("%s:env null", __func__);
		return NSDK_EFAIL;
	}

	jclass clz = env->FindClass(clzz);
	if(NULL == clz)
	{
		LOGE("%s: %s class not found", __func__, clzz);
		return NSDK_EFAIL;
	}

	jmethodID mid = env->GetStaticMethodID(clz, method, sig);
	if(NULL == mid)
	{
		LOGE("%s: %s : %s : method not found", __func__, clzz, method);
		return NSDK_EFAIL;
	}

	*ret_env = env;
	*ret_cls = clz;
	*ret_mid = mid;
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

//------------------------------------------------------------------------------

int find_java_satic_field(JNIEnv**  ret_env
						, jclass*   ret_cls
						, jfieldID* ret_fid
						, const char* clzz, const char* field, const char* sig)
{
#if defined(ANDROID_DEVICE)
	JNIEnv*	env	= NULL;
	JavaVM*	jvm	= jni_java_jvm;

	if(NULL == jvm || 0 > jvm->AttachCurrentThread(&env, NULL))
	{
		LOGE("%s: AttachCurrentThread err", __func__);
		return NSDK_EFAIL;
	}

	if(NULL == env)
	{
		LOGE("%s:env null", __func__);
		return NSDK_EFAIL;
	}

	jclass clz = env->FindClass(clzz);
	if(NULL == clz)
	{
		LOGE("%s: %s class not found", __func__, clzz);
		return NSDK_EFAIL;
	}

	jfieldID fid = env->GetStaticFieldID(clz, field, sig);
	if(NULL == fid)
	{
		LOGE("%s: %s : %s : field not found", __func__, clzz, field);
		return NSDK_EFAIL;
	}

	*ret_env = env;
	*ret_cls = clz;
	*ret_fid = fid;
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


//------------------------------------------------------------------------------


jmethodID find_java_instance_method(JNIEnv* env
								, const char* clzz, const char* method, const char* sig)
{
#if defined(ANDROID_DEVICE)
	jclass clz = env->FindClass(clzz);
	if(NULL == clz)
	{
		LOGE("%s: class not found", clzz);
		return NULL;
	}

	jmethodID mid = env->GetMethodID(clz, method, sig);
	if(NULL == mid)
	{
		LOGE("%s: %s : method not found", clzz, method);
		return NULL;
	}

	return mid;
#endif

	return NULL;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// channel, facebook, googleplus, everynetmarble, apple game center
//------------------------------------------------------------------------------

struct TPROFILE_SNS_S
{
	std::string playerID     ;
	std::string snsID        ;
	std::string nickName     ;		// facebook name, googleplus, everynetmarble, or apple game cente id
	std::string imageURL     ;
	std::string thumbnailURL ;
	std::string statusMessage;		// every netmarble status message
	int         gender       ;		// google gender: 0:MALE, 1:FEMALE, 2: other

	void Clear()
	{
		playerID     .clear();
		snsID        .clear();
		nickName     .clear();
		imageURL     .clear();
		thumbnailURL .clear();
		statusMessage.clear();
		gender       = -1;
	}

	void Set( const char* s_playerID, const char* s_snsID, const char* s_nickName
			, const char* s_imageURL =NULL, const char* s_thumbnailURL =NULL
			, const char* s_statusMessage =NULL, int google_gender =-1)
	{
		if(s_playerID     )	playerID     = s_playerID;
		if(s_snsID        )	snsID        = s_snsID;
		if(s_nickName     )	nickName     = s_nickName;
		if(s_imageURL     )	imageURL     = s_imageURL;
		if(s_thumbnailURL )	thumbnailURL = s_thumbnailURL;
		if(s_statusMessage)	statusMessage= s_statusMessage;
		gender       = google_gender;
	}

	void CopyTo(TPROFILE_SNS* profile)
	{
		profile->playerID      = 2<playerID     .length()? (char*)playerID     .c_str() : (char*)NULL;
		profile->snsID         = 2<snsID        .length()? (char*)snsID        .c_str() : (char*)NULL;
		profile->nickName      = 2<nickName     .length()? (char*)nickName     .c_str() : (char*)NULL;
		profile->imageURL      = 2<imageURL     .length()? (char*)imageURL     .c_str() : (char*)NULL;
		profile->thumbnailURL  = 2<thumbnailURL .length()? (char*)thumbnailURL .c_str() : (char*)NULL;
		profile->statusMessage = 0<statusMessage.length()? (char*)statusMessage.c_str() : (char*)NULL;
		profile->gender        = gender                                                              ;
	}

	void Log()
	{
		LOGI("profile +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		LOGI("    playerID     : %s", playerID     .c_str());
		LOGI("    snsID        : %s", snsID        .c_str());
		LOGI("    nickName     : %s", nickName     .c_str());
		LOGI("    imageUrl     : %s", imageURL     .c_str());
		LOGI("    thumbnailUrl : %s", thumbnailURL .c_str());
		LOGI("    statusMessage: %s", statusMessage.c_str());
		LOGI("    gender       : %d", gender               );
		LOGI("+++++++_+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	}
};

static int g_channel_state[NSDK_CHANNEL_TOT] =
{
	NSDK_EFAIL, NSDK_EFAIL,  NSDK_EFAIL, NSDK_EFAIL, NSDK_EFAIL,
	NSDK_EFAIL, NSDK_EFAIL,  NSDK_EFAIL, NSDK_EFAIL,
};

static TPROFILE_SNS_S                facebook_profile;					// facebook profile
static std::vector<TPROFILE_SNS_S >  facebook_friends;					// facebook friends
static std::vector<std::string    >  facebook_invites;					// Invite Facebook friends who doesn't play this application.
static std::vector<TPROFILE_SNS_S >  facebook_invitee;					// the list of inviter who send invite request to me.

static TPROFILE_SNS_S                googleplus_profile;				// googleplus profile
static std::vector<TPROFILE_SNS_S >  googleplus_friends;				// googleplus friends
static std::vector<TPROFILE_SNS_S >  googleplus_friends_app;			// googleplus app friends

static TPROFILE_SNS_S                everynetmarble_profile;			// everynetmarble profile
static std::vector<TPROFILE_SNS_S >  everynetmarble_friends;			// everynetmarble friends
static std::vector<TPROFILE_SNS_S >  everynetmarble_friends_app;		// everynetmarble app friends

static void clearChannelEveryNetmarble()
{
	g_channel_state[NSDK_CHANNEL_EVERY_NETMARBLE] = NSDK_EFAIL;
}

static void clearChannelFacebook()
{
	g_channel_state[NSDK_CHANNEL_FACEBOOK] = NSDK_EFAIL;

	facebook_profile.Clear();
	facebook_friends.clear();
	facebook_invites.clear();
	facebook_invitee.clear();
}

static void clearChannelKakao()
{
	g_channel_state[NSDK_CHANNEL_KAKAO] = NSDK_EFAIL;
}

static void clearChannelGooglePlus()
{
	g_channel_state[NSDK_CHANNEL_GOOGLE_PLUS] = NSDK_EFAIL;

	googleplus_profile    .Clear();
	googleplus_friends    .clear();
	googleplus_friends_app.clear();
}

static void clearEveryNetmarble()
{
	g_channel_state[NSDK_CHANNEL_GOOGLE_PLUS] = NSDK_EFAIL;

	everynetmarble_profile    .Clear();
	everynetmarble_friends    .clear();
	everynetmarble_friends_app.clear();
}

static void clearChannelAppleGameCenter()
{
	g_channel_state[NSDK_CHANNEL_APPLE_GAME_CENTER] = NSDK_EFAIL;
}

static void clearChannelNaver()
{
	g_channel_state[NSDK_CHANNEL_NAVER] = NSDK_EFAIL;
}

void setup_channel_state(int channel, int st)
{
	if(	0                >  channel ||
		NSDK_CHANNEL_TOT <= channel)
		return;

	g_channel_state[channel] = st;
}

// channel
int nsdk_connectToChannel(size_t channel)
{
	if(NSDK_CHANNEL_GAME_START == channel)
	{
		setup_channel_state(NSDK_CHANNEL_GAME_START, NSDK_TRUE);
		return NSDK_OK;
	}

	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_connectToChannel", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_channel_id = (int)channel;
	nsdk_st_query_channel = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid, (jint)channel);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_disconnectFromChannel(size_t channel)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_disconnectFromChannel", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_channel_id = (int)channel;
	nsdk_st_query_channel = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid, (jint)channel);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_channel_connect_option(int location)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_channelConnectOption", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_channel = NSDK_REQUEST;
	nsdk_st_query_channel_option = NSDK_EFAIL;	// nothing
	env->CallStaticVoidMethod(clz, mid, (jint)location);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

// channel saved result
int nsdk_rsp_ChannelState(size_t channel)
{
	if(NSDK_CHANNEL_TOT <= channel)
		return NSDK_EFAIL;

	return g_channel_state[channel];
}

// channel saved result
int nsdk_rsp_isChannelCanceled(size_t channel)
{
	if(NSDK_CHANNEL_TOT <= channel)
		return NSDK_EFAIL;

	int ret = g_channel_state[channel] == NSDK_MSG_USER_CANCELED ? NSDK_TRUE: NSDK_FALSE;
	return ret;
}

//------------------------------------------------------------------------------
// face book
int nsdk_requestFacebookMyProfile()
{
	//if(NSDK_FALSE >= g_channel_state[NSDK_CHANNEL_FACEBOOK])
	//{
	//	LOGW("%s[%3d]:: nsdk_requestFacebookMyProfile: try to login to facebook", __func__, __LINE__ );
	//	return NSDK_EFAIL;
	//}

	//if(NSDK_REQUEST == nsdk_st_query_facebook)
	//{
	//	LOGW("%s[%3d]:: nsdk_requestFacebookMyProfile requesting", __func__, __LINE__ );
	//	return NSDK_EFAIL;
	//}


	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_requestFacebookMyProfile", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_facebook = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_requestFacebookFriends()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_requestFacebookFriends", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_facebook = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_inviteFacebookFriends(const char* message, const char* title)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_inviteFacebookFriends", "(Ljava/lang/String;Ljava/lang/String;)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}
#if defined(ANDROID_DEVICE)
	nsdk_st_query_facebook = NSDK_REQUEST;

	jstring j_message = env->NewStringUTF(message);						// const char* to jstring
	jstring j_title   = env->NewStringUTF(title  );						// const char* to jstring
	env->CallStaticVoidMethod(clz, mid, j_message, j_title);
	return NSDK_OK;
#endif
	return NSDK_EFAIL;
}

int nsdk_requestFacebookInviters()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_requestFacebookInviters", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}
#if defined(ANDROID_DEVICE)
	nsdk_st_query_facebook = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif
	return NSDK_EFAIL;
}

int nsdk_deleteFacebookInviters()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};
	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_deleteFacebookInviters", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}
#if defined(ANDROID_DEVICE)
	nsdk_st_query_facebook = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif
	return NSDK_EFAIL;
}

int nsdk_postFacebookPhoto(int* colors, int w, int h)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};
	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_postFacebookPhoto", "([III)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}
#if defined(ANDROID_DEVICE)
	nsdk_st_query_facebook = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid, colors, w, h);
	return NSDK_OK;
#endif
	return NSDK_EFAIL;
}

int nsdk_postFacebookStatusUpdate(const char* message, const char* name, const char* picture
								, const char* link, const char* caption, const char* description)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};
	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_postFacebookStatusUpdate", "(Ljava/lang/String;Ljava/lang/String;[B[B[B[B)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}
#if defined(ANDROID_DEVICE)
	nsdk_st_query_facebook = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif
	return NSDK_EFAIL;
}


// facebook saved result MyProfile.
int nsdk_rsp_FacebookMyProfile(TPROFILE_SNS* profile)
{
	if(2 > facebook_profile.snsID.length())
	{
		profile->playerID      = (char*)NULL;
		profile->snsID         = (char*)NULL;
		profile->nickName      = (char*)NULL;
		profile->imageURL      = (char*)NULL;
		profile->thumbnailURL  = (char*)NULL;
		return NSDK_EFAIL;
	}

	profile->playerID      = (char*)facebook_profile.playerID    .c_str();
	profile->snsID         = (char*)facebook_profile.snsID       .c_str();
	profile->nickName      = (char*)facebook_profile.snsID       .c_str();
	profile->imageURL      = (char*)facebook_profile.imageURL    .c_str();
	profile->thumbnailURL  = (char*)facebook_profile.thumbnailURL.c_str();
	return NSDK_OK;
}

size_t nsdk_rsp_FacebookFriendsCount()
{
	size_t iSize = facebook_friends.size();
	return iSize;
}

int nsdk_rsp_FacebookFriendProfile(size_t idx, TPROFILE_SNS* profile)
{
	size_t iSize = facebook_friends.size();
	if(iSize <= idx)
		return NSDK_EFAIL;

	TPROFILE_SNS_S* elm = &facebook_friends[idx];
	elm->CopyTo(profile);
	return NSDK_OK;
}

size_t nsdk_rsp_FacebookInviteFriendsCount()
{
	size_t iSize = facebook_invites.size();
	return iSize;
}

int nsdk_rsp_FacebookInviteFriend()
{
	int iSize = (int)facebook_invites.size();
	return iSize;
}

const char* nsdk_rsp_FacebookInviteFriend(size_t idx)
{
	size_t iSize = facebook_invites.size();
	if(iSize <= idx)
		return NULL;

	const std::string& elm = facebook_invites[idx];
	const char* ret = elm.c_str();
	return ret;
}

size_t nsdk_rsp_FacebookInvitersCount()
{
	size_t iSize = facebook_invitee.size();
	return iSize;
}

int nsdk_rsp_FacebookInviter(size_t idx, TPROFILE_SNS* profile)
{
	size_t iSize = facebook_invitee.size();
	if(iSize <= idx)
		return NSDK_EFAIL;

	TPROFILE_SNS_S* elm = &facebook_invitee[idx];
	elm->CopyTo(profile);
	return NSDK_OK;
}

int nsdk_rsp_FacebookDeleteInviters()
{
	if(NSDK_REQUEST == nsdk_st_query_facebook)
		return NSDK_REQUEST;

	if(facebook_invitee.empty())
		return NSDK_OK;

	return NSDK_EFAIL;
}

//------------------------------------------------------------------------------
// GooglePlus

int nsdk_requestGooglePlusMyProfile()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_requestGooglePlusMyProfile", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_googleplus = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_requestGooglePlusFriends()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_requestGooglePlusFriends", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_googleplus = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_rsp_GooglePlusMyProfile(TPROFILE_SNS* profile)
{
	if(2 > googleplus_profile.snsID.length())
	{
		profile->playerID  = (char*)NULL;
		profile->snsID     = (char*)NULL;
		profile->nickName  = (char*)NULL;
		profile->imageURL  = (char*)NULL;
		profile->gender    = 2;
		return NSDK_EFAIL;
	}

	profile->playerID      = (char*)googleplus_profile.playerID.c_str();
	profile->snsID         = (char*)googleplus_profile.snsID   .c_str();
	profile->nickName      = (char*)googleplus_profile.nickName.c_str();
	profile->imageURL      = (char*)googleplus_profile.imageURL.c_str();
	profile->gender        =        googleplus_profile.gender;

	return NSDK_OK;
}

size_t nsdk_rsp_GooglePlusFriendsCount()
{
	size_t iSize = googleplus_friends.size();
	return iSize;
}

int nsdk_rsp_GooglePlusFriendProfile( size_t idx, TPROFILE_SNS* profile)
{
	size_t iSize = googleplus_friends.size();
	if(iSize <= idx)
		return NSDK_EFAIL;

	TPROFILE_SNS_S* elm = &googleplus_friends[idx];
	elm->CopyTo(profile);
	return NSDK_OK;
}

size_t nsdk_rsp_GooglePlusAppFriendsCount()
{
	size_t iSize = googleplus_friends_app.size();
	return iSize;
}

int nsdk_rsp_GooglePlusAppFriendProfile(size_t idx, TPROFILE_SNS* profile)
{
	size_t iSize = googleplus_friends_app.size();
	if(iSize <= idx)
		return NSDK_EFAIL;

	TPROFILE_SNS_S* elm = &googleplus_friends_app[idx];
	elm->CopyTo(profile);
	return NSDK_OK;
}

//------------------------------------------------------------------------------
// EveryNetmarble

int nsdk_requestEveryNetmarbleMyProfile()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_requestEveryNetmarbleMyProfile", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_everynetmarble = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_requestEveryNetmarbleFriends()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_requestEveryNetmarbleFriends", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_everynetmarble = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_rsp_EveryNetmarbleMyProfile(TPROFILE_SNS* profile)
{
	if(2 > everynetmarble_profile.snsID.length())
	{
		profile->playerID      = (char*)NULL;
		profile->snsID         = (char*)NULL;
		profile->nickName      = (char*)NULL;
		profile->statusMessage = (char*)NULL;
		profile->imageURL      = (char*)NULL;
		profile->thumbnailURL  = (char*)NULL;
		return NSDK_EFAIL;
	}

	profile->playerID         = (char*)everynetmarble_profile.playerID     .c_str();
	profile->snsID            = (char*)everynetmarble_profile.snsID        .c_str();
	profile->nickName         = (char*)everynetmarble_profile.nickName     .c_str();
	profile->statusMessage    = (char*)everynetmarble_profile.statusMessage.c_str();
	profile->imageURL         = (char*)everynetmarble_profile.imageURL     .c_str();
	profile->thumbnailURL     = (char*)everynetmarble_profile.thumbnailURL .c_str();

	return NSDK_OK;
}

size_t nsdk_rsp_EveryNetmarbleFriendsCount()
{
	size_t iSize = everynetmarble_friends.size();
	return iSize;
}

int nsdk_rsp_EveryNetmarbleFriendProfile( size_t idx, TPROFILE_SNS* profile)
{
	size_t iSize = everynetmarble_friends.size();
	if(iSize <= idx)
		return NSDK_EFAIL;

	TPROFILE_SNS_S* elm = &everynetmarble_friends[idx];
	elm->CopyTo(profile);
	return NSDK_OK;
}

size_t nsdk_rsp_EveryNetmarbleAppFriendsCount()
{
	size_t iSize = everynetmarble_friends_app.size();
	return iSize;
}

int nsdk_rsp_EveryNetmarbleAppFriendProfile(size_t idx, TPROFILE_SNS* profile)
{
	size_t iSize = everynetmarble_friends_app.size();
	if(iSize <= idx)
		return NSDK_EFAIL;

	TPROFILE_SNS_S* elm = &everynetmarble_friends_app[idx];
	elm->CopyTo(profile);
	return NSDK_OK;
}

//------------------------------------------------------------------------------
// apple game center

int    nsdk_requestAppleGameCenterMyProfile    ()                     { return NSDK_EFAIL; }
int    nsdk_requestAppleGameCenterFriends      ()                     { return NSDK_EFAIL; }
int    nsdk_rsp_AppleGameCenterMyProfile       (TPROFILE_SNS*)        { return NSDK_EFAIL; }
size_t nsdk_rsp_AppleGameCenterFriendsCount    ()                     { return 0; }
int    nsdk_rsp_AppleGameCenterFriendProfile   (size_t, TPROFILE_SNS*){ return NSDK_EFAIL; }
size_t nsdk_rsp_AppleGameCenterAppFriendsCount ()                     { return 0; }
int    nsdk_rsp_AppleGameCenterAppFriendProfile(size_t, TPROFILE_SNS*){ return NSDK_EFAIL; }


//------------------------------------------------------------------------------
//push
//------------------------------------------------------------------------------


static std::vector<TLOCAL_PUSH > local_push_list;								// local push list from java or ios

size_t nsdk_rsp_LocalPushCount()
{
	size_t iSize = local_push_list.size();
	return iSize;
}

int nsdk_rsp_LocalPushInfo(int* notificationID, int* bUse, size_t index)
{
	size_t iSize = local_push_list.size();
	if(iSize <= index)
	{
		if(notificationID)
			*notificationID = -1;

		if(bUse)
			*bUse = 0;

		return NSDK_EFAIL;
	}

	const TLOCAL_PUSH& push = local_push_list[index];

	if(notificationID)
		*notificationID = push.notificationID;

	if(bUse)
		*bUse = push.isUse;

	return NSDK_OK;
}


//------------------------------------------------------------------------------

// push message ...
int nsdk_setSendPushPlayerIDClear()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setSendPushPlayerIDClear", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_setSendPushPlayerIDAdd(const char* _playerID)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setSendPushPlayerIDAdd", "(Ljava/lang/String;)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)

	jstring playerID = env->NewStringUTF(_playerID);						// const char* to jstring
	env->CallStaticVoidMethod(clz, mid, playerID);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_setSendPushNotificationID(int notificationID)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setSendPushNotificationID", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid, notificationID);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_setSendPushMessage(const char* str)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	//int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setSendPushMessage", "([B)V");

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setSendPushMessage", "(Ljava/lang/String;)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	jstring msg = env->NewStringUTF(str);						// const char* to jstring
	env->CallStaticVoidMethod(clz, mid, msg);

	//int len = strlen(str);
	//jbyteArray msg = env->NewByteArray(len);
	//env->SetByteArrayRegion(msg, 0, len, (jbyte *)str);
	//env->CallStaticVoidMethod(clz, mid, msg);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_sendPushNotification()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_sendPushNotification", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_sendPushNotificationUseNotificationID()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_sendPushNotificationUseNotificationID", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_setUsePushNotification(int bUse)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setUsePushNotification", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid, bUse);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_getUsePushNotification()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getUsePushNotification", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


int nsdk_setPushLocalNotificationID(int notificationID)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setPushLocalNotificationID", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid, notificationID);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_getPushLocalNotificationID()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getPushLocalNotificationID", "()I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	ret = env->CallStaticIntMethod(clz, mid);
	return ret;
#endif

	return NSDK_EFAIL;
}

int nsdk_setPushLocalSecond(int second)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setPushLocalSecond", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid, second);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_getPushUseLocalSecond()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getPushUseLocalSecond", "()I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	ret = env->CallStaticIntMethod(clz, mid);
	return ret;
#endif

	return NSDK_EFAIL;
}

int nsdk_setPushLocalMessage(const char* str)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setPushLocalMessage", "(Ljava/lang/String;)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	jstring msg = env->NewStringUTF(str);						// const char* to jstring
	env->CallStaticVoidMethod(clz, mid, msg);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_setLocalNotification()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setLocalNotification", "()I");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	ret = env->CallStaticIntMethod(clz, mid);
	return ret;
#endif

	return NSDK_EFAIL;
}

int nsdk_setUseLocalNotification(int bUse)
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_setUseLocalNotification", "(I)V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid, bUse);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_getUseLocalPushNotification()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_getUseLocalPushNotification", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)

	nsdk_st_query_push = NSDK_REQUEST;

	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}

int nsdk_cancelLocalNotification()
{
	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};

	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_cancelLocalNotification", "()V");
	if(0 > ret)
	{
		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
		return NSDK_EFAIL;
	}

#if defined(ANDROID_DEVICE)
	nsdk_st_query_push = NSDK_REQUEST;
	env->CallStaticVoidMethod(clz, mid);
	return NSDK_OK;
#endif

	return NSDK_EFAIL;
}


//int nsdk_message_txt(char* buf, int idx)
//{
//	JNIEnv* env = NULL; jclass clz = {0}; jmethodID mid = {0};
//	int ret = find_java_satic_method(&env, &clz, &mid, PROJECT_MAIN_CLASS, "nsdk_message_txt", "(I)Ljava/lang/String;");
//	if(0 > ret)
//	{
//		LOGE("%s[%3d]:err:: cannt find method", __func__, __LINE__ );
//		return NSDK_EFAIL;
//	}
//#if defined(ANDROID_DEVICE)
//
//	jobject j_str = (jobject)env->CallStaticObjectMethod(clz, mid, idx);
//	if(NULL == j_str)
//		return NSDK_EFAIL;
//
//	char* utf_str = (char*)env->GetStringUTFChars(j_str, NULL);
//	if(NULL == utf_str)
//		return NSDK_EFAIL;
//
//	strcpy(buf, utf_str);
//	env->ReleaseStringUTFChars(j_str, utf_str);
//	return NSDK_OK;
//#endif
//	return NSDK_EFAIL;
//}


void resetSession()
{
	g_channel_ids.clear();

	memset(g_channel_state, 0xFF, sizeof g_channel_state);

	for(size_t iz = 0; iz<NSDK_CHANNEL_TOT; ++iz)
		g_channel_state[iz] = NSDK_EFAIL;

	facebook_profile.Clear();
	facebook_friends.clear();
	facebook_invites.clear();
	facebook_invitee.clear();

	googleplus_profile    .Clear();
	googleplus_friends    .clear();
	googleplus_friends_app.clear();

	everynetmarble_profile    .Clear();
	everynetmarble_friends    .clear();
	everynetmarble_friends_app.clear();

	local_push_list.clear();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if defined(ANDROID_DEVICE)


extern "C" {
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
	jint    result = -1;
	JNIEnv*	env = NULL;
	jni_java_jvm = jvm;

	LOGW("JNI_OnLoad----------------------");

	// test..
	if(jvm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK)
	{
		LOGE("GetEnv failed!---------------");
		return result;
	}

	if(!env)
		LOGE("Could not retrieve the env!---------------");

	jni_java_jvm = jvm;
	return JNI_VERSION_1_6;
}

//------------------------------------------------------------------------------

JNI_FUNC_DCL(void, retShowSoftKeyboard)(JNIEnv* env, jobject obj
										, jint ret, jlong caller_func, jlong caller_owner, jlong caller_id, jstring jStr)
{
	LOGI("%s[%3d]:ret: %d    owner : %lld   id: %lld,  str: %p", __func__, __LINE__, ret, caller_owner, caller_id, jStr);

	NSDK_CB_SOFTKEYBOARD txt_field_f = (NSDK_CB_SOFTKEYBOARD)caller_func;
	void*                txt_field_a = (void*               )caller_owner;
	long long            calling_id  = (long long           )caller_id;


	char* str = NULL;
	int   len = 0;

	if(jStr)
	{
		str = (char*)env->GetStringUTFChars(jStr, 0);
		len = (int  )env->GetStringUTFLength(jStr);

		LOGI("%s[%3d]: %s", __func__, __LINE__, str);

		if(NULL == txt_field_f)
			LOGW("%s[%3d]: callback function is null", __func__, __LINE__);
		else
			txt_field_f(txt_field_a, calling_id, 0, str, len);

		env->ReleaseStringUTFChars(jStr, str);
		return;
	}

	if(NULL == txt_field_f)
		LOGW("%s[%3d]: callback function is null", __func__, __LINE__);
	else
		txt_field_f(txt_field_a, calling_id, 0, NULL, 0);
}

//------------------------------------------------------------------------------

JNI_FUNC_DCL(void, retRemoteImagePixel32Cb)(JNIEnv* env, jobject obj, jlong jcb_func, jlong jcb_owner, jobject bmp)
{
	LOGI("%s[%3d]: cb_funtion: 0x%llX 0x%llX   %p", __func__, __LINE__, jcb_func, jcb_owner, bmp);

	NSDK_CB_REMOTE_IMAGE func  = (NSDK_CB_REMOTE_IMAGE)jcb_func;
	void*                owner = (void*               )jcb_owner;

	if(NULL == bmp)
	{
		func(owner, NULL, 0, 0);
		return;
	}

	AndroidBitmapInfo  info = {0};
	unsigned int*      src  = NULL;
	unsigned int*      dst  = NULL;

	AndroidBitmap_getInfo(env, bmp, &info);			// format must be ARGB_8888
	LOGI("%s[%3d]: image: w, h, stride = %4d, %4d  %4d", __func__, __LINE__, info.width, info.height, info.stride);


	AndroidBitmap_lockPixels(env, bmp, (void**)&src);
	if(src)
	{
		int len = info.width * info.height;
		dst = new unsigned int[len];
		memcpy(dst, src, len * sizeof(unsigned int));
		AndroidBitmap_unlockPixels(env, bmp);
	}

	func(owner, (unsigned char*)dst, info.width, info.height);
}

//------------------------------------------------------------------------------

JNI_FUNC_DCL(void, retShowExitView)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_showexit = ret;
}

JNI_FUNC_DCL(void, retSignIn)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_signin = ret;
}

JNI_FUNC_DCL(void, retChannelSignIn)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_channel_signin = ret;
}

JNI_FUNC_DCL(void, retShowQuickMenuView)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_qmenu = ret;
}

JNI_FUNC_DCL(void, retHideQuickMenuView)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_qmenu = ret;
}


JNI_FUNC_DCL(void, retShowCouponView)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_showcouponview = ret;
}

JNI_FUNC_DCL(void, retShowFreeChargeView)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_showfreechargeview = ret;
}

JNI_FUNC_DCL(void, retShowGameReviewView)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_showgamereviewview = ret;
}

JNI_FUNC_DCL(void, retShowNoticeView)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_shownoticeview  = ret;
}

JNI_FUNC_DCL(void, retShowPromotionView)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_showpromotionview  = ret;
}

JNI_FUNC_DCL(void, retShowCustomerSupportView)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_showcustomersupportview  = ret;
}

JNI_FUNC_DCL(void, retShowAgreementView)(JNIEnv* env, jobject obj, jint ret)
{
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
	nsdk_st_opened_showagreementview  = NSDK_WINDOW_CLOSED;

	LOGI(" retShowAgreementView ++++++++++++++++++++++++++++++++++++++++++++++++++++++ %d", ret);
	save_contract_agreement(ret);
}


//------------------------------------------------------------------------------
// channel
//------------------------------------------------------------------------------

JNI_FUNC_DCL(void, retConnectToChannel)(JNIEnv* env, jobject obj, jint channel, jint ret)
{
	nsdk_st_query_channel = NSDK_WINDOW_CLOSED;
	setup_channel_state(channel, ret);

	LOGI(">>>>>>>>>> %s[%3d]:channel[%d]: ret: %d", __func__, __LINE__, channel, ret);

	// channelLoginListener success
	if( NSDK_FALSE == ret)
	{
		LOGI(" retConnectToChannel failed >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		return;
	}

	// channelLoginListener success
	if( NSDK_TRUE == ret)
	{
		LOGI(" retConnectToChannel success >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		return;
	}

	//channel option size, ret must be greater then NSDK_CHANNEL_EXIST_OFFSET
	if(NSDK_CHANNEL_EXIST_OFFSET <= ret && ret <= NSDK_CHANNEL_EXIST_OFFSET + NSDK_CHANNEL_OPTION_SIZE)
	{
		LOGI(" retConnectToChannel success. has previous id. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		return;
	}

	//channel error
	LOGE(" retConnectToChannel Errrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
	ret |= NSDK_ERR_PROC;				// add error flag
	setup_channel_state(channel, ret);
	return;
}


JNI_FUNC_DCL(void, retDisconnectFromChannel)(JNIEnv* env, jobject obj, jint channel, jint ret)
{
	nsdk_st_query_channel = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:channel %d: ret: %d", __func__, __LINE__, channel, ret);

	setup_channel_state(channel, ret);

	if     (NSDK_CHANNEL_EVERY_NETMARBLE    == channel)	{	clearChannelEveryNetmarble();  }
	else if(NSDK_CHANNEL_FACEBOOK           == channel)	{	clearChannelFacebook();        }
	else if(NSDK_CHANNEL_KAKAO              == channel)	{	clearChannelKakao();           }
	else if(NSDK_CHANNEL_GOOGLE_PLUS        == channel)	{	clearChannelGooglePlus();      }
	else if(NSDK_CHANNEL_APPLE_GAME_CENTER  == channel)	{	clearChannelAppleGameCenter(); }
	else if(NSDK_CHANNEL_NAVER              == channel)	{	clearChannelNaver();           }
}



JNI_FUNC_DCL(void, retChannelConnectOption)(JNIEnv* env, jobject obj, jint channel, jint ret)
{
	nsdk_st_query_channel = NSDK_WINDOW_CLOSED;
	nsdk_st_query_channel_option = ret;

	LOGI("%s[%3d]:channel[%d]: ret: %d", __func__, __LINE__, channel, ret);

	if( NSDK_FALSE == ret)
		LOGI(" retChannelConnectOption failed >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	else
		LOGI(" retChannelConnectOption success >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
}



JNI_FUNC_DCL(void, retChannelIDClear)(JNIEnv* env, jobject obj)
{
	g_channel_ids.clear();
	LOGI("%s[%3d]:channel ids clear success.", __func__, __LINE__);
}

JNI_FUNC_DCL(void, retChannelIDAdd)(JNIEnv* env, jobject obj, jint channel, jstring jChannelID)
{
	char cid[NSDK_LENGTH_CHANNEL_ID]={0};
	size_t idx = channel;

	if( NSDK_CHANNEL_TOT <= channel)
	{
		LOGE("%s[%3d]: exceed max channel index:  %d", __func__, __LINE__, channel);
		return;
	}

	char* channel_id = (char*)env->GetStringUTFChars(jChannelID, 0);

	strcpy(cid, channel_id);
	env->ReleaseStringUTFChars(jChannelID, channel_id);

	LOGI("%s[%3d]:channel[%d]: id: %s", __func__, __LINE__, idx, cid);

	g_channel_ids.insert(std::pair<size_t, std::string>(idx, std::string(cid)));
}

//------------------------------------------------------------------------------
// facebook
//------------------------------------------------------------------------------

JNI_FUNC_DCL(void, retRequestFacebookMyProfile)(JNIEnv* env, jobject obj, jint ret
												, jbyteArray playerID
												, jbyteArray facebookID, jbyteArray name
												, jbyteArray imageURL, jbyteArray thumbnailURL)
{
	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = NULL;

	nsdk_st_query_facebook = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);

	char _playerID    [1024]={0};
	char _snsID       [1024]={0};
	char _nickName    [1024]={0};
	char _imageURL    [1024]={0};
	char _thumbnailURL[1024]={0};

	if(NSDK_MSG_SUCCESS != ret)
	{
		facebook_profile.playerID     = "";
		facebook_profile.snsID        = "";
		facebook_profile.nickName     = "";
		facebook_profile.imageURL     = "";
		facebook_profile.thumbnailURL = "";
		return;
	}

	b_rcv = playerID    ;	b_len = env->GetArrayLength(b_rcv);	b_arr = env->GetByteArrayElements(b_rcv, 0);	strncpy(_playerID    , (const char*)b_arr,  b_len);	facebook_profile.playerID     = _playerID    ;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = facebookID  ;	b_len = env->GetArrayLength(b_rcv);	b_arr = env->GetByteArrayElements(b_rcv, 0);	strncpy(_snsID       , (const char*)b_arr,  b_len);	facebook_profile.snsID        = _snsID       ;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = name        ;	b_len = env->GetArrayLength(b_rcv);	b_arr = env->GetByteArrayElements(b_rcv, 0);	strncpy(_nickName    , (const char*)b_arr,  b_len);	facebook_profile.nickName     = _nickName    ;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = imageURL    ;	b_len = env->GetArrayLength(b_rcv);	b_arr = env->GetByteArrayElements(b_rcv, 0);	strncpy(_imageURL    , (const char*)b_arr,  b_len);	facebook_profile.imageURL     = _imageURL    ;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = thumbnailURL;	b_len = env->GetArrayLength(b_rcv);	b_arr = env->GetByteArrayElements(b_rcv, 0);	strncpy(_thumbnailURL, (const char*)b_arr,  b_len);	facebook_profile.thumbnailURL = _thumbnailURL;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);

	LOGW("retRequestFacebookMyProfile --------------------------------------------");
	LOGW("playerID    : %s", _playerID    );
	LOGW("snsID       : %s", _snsID       );
	LOGW("nickName    : %s", _nickName    );
	LOGW("imageURL    : %s", _imageURL    );
	LOGW("thumbnailURL: %s", _thumbnailURL);

	LOGW("playerID    : %s", facebook_profile.playerID    .c_str());
	LOGW("snsID       : %s", facebook_profile.snsID       .c_str());
	LOGW("nickName    : %s", facebook_profile.nickName    .c_str());
	LOGW("imageURL    : %s", facebook_profile.imageURL    .c_str());
	LOGW("thumbnailURL: %s", facebook_profile.thumbnailURL.c_str());
}


JNI_FUNC_DCL(void, retRequestFacebookFriends)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_facebook = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);

	if(NSDK_MSG_SUCCESS != ret)
		return;

	facebook_friends.clear();
}

JNI_FUNC_DCL(void, retRequestFacebookFriendsList)(JNIEnv* env, jobject obj, jint idx
														, jbyteArray playerID
														, jbyteArray facebookID, jbyteArray name
														, jbyteArray imageURL, jbyteArray thumbnailURL)
{
	LOGI("%s[%3d]: idx: %d", __func__, __LINE__, idx);

	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = NULL;

	TPROFILE_SNS_S profile;

	b_rcv = playerID    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.playerID     = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = facebookID  ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.snsID        = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = name        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.nickName     = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = imageURL    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.imageURL     = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = thumbnailURL;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.thumbnailURL = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	facebook_friends.push_back(profile);
}


JNI_FUNC_DCL(void, retInviteFacebookFriends)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_facebook = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);

	if(NSDK_MSG_SUCCESS != ret)
		return;

	facebook_invites.clear();
}

JNI_FUNC_DCL(void, retInviteFacebookFriendsList)(JNIEnv* env, jobject obj, jint idx, jbyteArray facebookID)
{
	nsdk_st_query_facebook = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]: idx: %d", __func__, __LINE__, idx);

	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = NULL;

	std::string sFacebookID;

	b_rcv = facebookID  ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	sFacebookID          = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	facebook_invites.push_back(sFacebookID);
}

JNI_FUNC_DCL(void, retRequestFacebookInviters)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_facebook = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);

	if(NSDK_MSG_SUCCESS != ret)
		return;

	facebook_invitee.clear();
}

JNI_FUNC_DCL(void, retRequestFacebookInvitersList)(JNIEnv* env, jobject obj, jint idx
														, jbyteArray playerID
														, jbyteArray facebookID, jbyteArray name
														, jbyteArray imageURL, jbyteArray thumbnailURL)
{
	nsdk_st_query_facebook = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]: idx: %d", __func__, __LINE__, idx);

	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = NULL;

	TPROFILE_SNS_S profile;

	b_rcv = playerID    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.playerID     = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = facebookID  ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.snsID        = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = name        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.nickName     = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = imageURL    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.imageURL     = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = thumbnailURL;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.thumbnailURL = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	facebook_invitee.push_back(profile);
}


JNI_FUNC_DCL(void, retDeleteFacebookInviters)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_facebook = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);

	if(NSDK_MSG_SUCCESS != ret)
		return;

	facebook_invitee.clear();
}

JNI_FUNC_DCL(void, retPostFacebookPhoto)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_facebook = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
}


JNI_FUNC_DCL(void, retPostFacebookStatusUpdate)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_facebook = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
}



//------------------------------------------------------------------------------
// GooglePlus
//------------------------------------------------------------------------------

JNI_FUNC_DCL(void, retRequestGooglePlusMyProfile)(JNIEnv* env, jobject obj, jint ret
												, jbyteArray playerID
												, jbyteArray googlePlusID, jbyteArray nickName
												, jbyteArray imageURL, jint gender)
{
	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = NULL;

	nsdk_st_query_googleplus = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);

	char _playerID    [1024]={0};
	char _snsID       [1024]={0};
	char _nickName    [1024]={0};
	char _imageURL    [1024]={0};

	if(NSDK_MSG_SUCCESS != ret)
	{
		googleplus_profile.playerID  = "";
		googleplus_profile.snsID     = "";
		googleplus_profile.nickName  = "";
		googleplus_profile.imageURL  = "";
		googleplus_profile.gender    = -1;
		return;
	}

	b_rcv = playerID    ;	b_len = env->GetArrayLength(b_rcv);	b_arr = env->GetByteArrayElements(b_rcv, 0);	strncpy(_playerID    , (const char*)b_arr,  b_len);	googleplus_profile.playerID     = _playerID    ;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = googlePlusID;	b_len = env->GetArrayLength(b_rcv);	b_arr = env->GetByteArrayElements(b_rcv, 0);	strncpy(_snsID       , (const char*)b_arr,  b_len);	googleplus_profile.snsID        = _snsID       ;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = nickName    ;	b_len = env->GetArrayLength(b_rcv);	b_arr = env->GetByteArrayElements(b_rcv, 0);	strncpy(_nickName    , (const char*)b_arr,  b_len);	googleplus_profile.nickName     = _nickName    ;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = imageURL    ;	b_len = env->GetArrayLength(b_rcv);	b_arr = env->GetByteArrayElements(b_rcv, 0);	strncpy(_imageURL    , (const char*)b_arr,  b_len);	googleplus_profile.imageURL     = _imageURL    ;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);

	googleplus_profile.gender = gender;

	LOGW("retRequestGoogle+MyProfile --------------------------------------------");
	LOGW("playerID    : %s", googleplus_profile.playerID    .c_str());
	LOGW("snsID       : %s", googleplus_profile.snsID       .c_str());
	LOGW("nickName    : %s", googleplus_profile.nickName    .c_str());
	LOGW("imageURL    : %s", googleplus_profile.imageURL    .c_str());
	LOGW("gender      : %d", googleplus_profile.gender              );
}

JNI_FUNC_DCL(void, retRequestGooglePlusBuddies)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_googleplus = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);

	if(NSDK_MSG_SUCCESS != ret)
		return;

	googleplus_friends    .clear();
	googleplus_friends_app.clear();
}

JNI_FUNC_DCL(void, retRequestGooglePlusBuddiesList)(JNIEnv* env, jobject obj, jint idx
													, jbyteArray playerID
													, jbyteArray googlePlusID, jbyteArray nickName
													, jbyteArray imageURL, jint gender)
{
	LOGI("%s[%3d]: idx: %d", __func__, __LINE__, idx);

	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = NULL;

	TPROFILE_SNS_S profile;

	b_rcv = playerID    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.playerID = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = googlePlusID;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.snsID    = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = nickName    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.nickName = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = imageURL    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.imageURL = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	profile.gender = gender;

	googleplus_friends.push_back(profile);
}

JNI_FUNC_DCL(void, retRequestGooglePlusAppBuddiesList)(JNIEnv* env, jobject obj, jint idx
													, jbyteArray playerID
													, jbyteArray googlePlusID, jbyteArray nickName
													, jbyteArray imageURL, jint gender)
{
	LOGI("%s[%3d]: idx: %d", __func__, __LINE__, idx);

	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = NULL;

	TPROFILE_SNS_S profile;

	b_rcv = playerID    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.playerID = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = googlePlusID;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.snsID    = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = nickName    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.nickName = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = imageURL    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.imageURL = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	profile.gender = gender;

	googleplus_friends_app.push_back(profile);
}




//------------------------------------------------------------------------------
// EveryNetmarble
//------------------------------------------------------------------------------

JNI_FUNC_DCL(void, retRequestEveryNetmarbleMyProfile)(JNIEnv* env, jobject obj, jint ret
													, jbyteArray playerID
													, jbyteArray everyNetmarbleID, jbyteArray nickName
													, jbyteArray statusMessage
													, jbyteArray imageURL, jbyteArray thumbnailURL)
{
	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = NULL;

	nsdk_st_query_everynetmarble = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);

	if(NSDK_MSG_SUCCESS != ret)
	{
		everynetmarble_profile.playerID      = "";
		everynetmarble_profile.snsID         = "";
		everynetmarble_profile.nickName      = "";
		everynetmarble_profile.statusMessage = "";
		everynetmarble_profile.imageURL      = "";
		everynetmarble_profile.thumbnailURL  = "";
		return;
	}

	b_rcv = playerID        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	everynetmarble_profile.playerID      = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = everyNetmarbleID;	b_arr = env->GetByteArrayElements(b_rcv, 0);	everynetmarble_profile.snsID         = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = nickName        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	everynetmarble_profile.nickName      = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = statusMessage   ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	everynetmarble_profile.statusMessage = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = imageURL        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	everynetmarble_profile.imageURL      = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = thumbnailURL    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	everynetmarble_profile.thumbnailURL  = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
}

JNI_FUNC_DCL(void, retRequestEveryNetmarbleBuddies)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_everynetmarble = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);

	if(NSDK_MSG_SUCCESS != ret)
		return;

	everynetmarble_friends    .clear();
	everynetmarble_friends_app.clear();
}

JNI_FUNC_DCL(void, retRequestEveryNetmarbleBuddiesList)(  JNIEnv* env, jobject obj, jint idx
														, jbyteArray playerID
														, jbyteArray everyNetmarbleID, jbyteArray nickName
														, jbyteArray statusMessage
														, jbyteArray imageURL, jbyteArray thumbnailURL)
{
	LOGI("%s[%3d]: idx: %d", __func__, __LINE__, idx);

	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = NULL;

	TPROFILE_SNS_S profile;

	b_rcv = playerID        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.playerID      = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = everyNetmarbleID;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.snsID         = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = nickName        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.nickName      = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = statusMessage   ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.statusMessage = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = imageURL        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.imageURL      = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = thumbnailURL    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.thumbnailURL  = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);

	everynetmarble_friends.push_back(profile);
}

JNI_FUNC_DCL(void, retRequestEveryNetmarbleAppBuddiesList)(JNIEnv* env, jobject obj, jint idx
														, jbyteArray playerID
														, jbyteArray everyNetmarbleID, jbyteArray nickName
														, jbyteArray statusMessage
														, jbyteArray imageURL, jbyteArray thumbnailURL)
{
	LOGI("%s[%3d]: idx: %d", __func__, __LINE__, idx);

	int        b_len = 0;
	jbyte*     b_arr = NULL;
	jbyteArray b_rcv = NULL;

	TPROFILE_SNS_S profile;

	b_rcv = playerID        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.playerID      = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = everyNetmarbleID;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.snsID         = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = nickName        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.nickName      = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = statusMessage   ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.statusMessage = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = imageURL        ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.imageURL      = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);
	b_rcv = thumbnailURL    ;	b_arr = env->GetByteArrayElements(b_rcv, 0);	profile.thumbnailURL  = (char*)b_arr;	env->ReleaseByteArrayElements(b_rcv, b_arr, 0);

	everynetmarble_friends_app.push_back(profile);
}




//-----------------------------------------------------------------------------
// push ...
//-----------------------------------------------------------------------------

JNI_FUNC_DCL(void, retSendPushNotification)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
}

JNI_FUNC_DCL(void, retSendPushNotificationUseNotificationID)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
}

JNI_FUNC_DCL(void, retSetUsePushNotification)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
}

JNI_FUNC_DCL(void, retGetUsePushNotification)(JNIEnv* env, jobject obj, jint ret, jint use)
{
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d use: %d", __func__, __LINE__, ret, use);
}

JNI_FUNC_DCL(void, retSetUseLocalNotification)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
}


JNI_FUNC_DCL(void, retGetUseLocalPushNotification)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);

	// clear the notification list
	local_push_list.clear();
}

JNI_FUNC_DCL(void, retGetUseLocalPushNotificationList)(JNIEnv* env, jobject obj, jint notificationID, jint isUse)
{
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:notificationID: %d", __func__, __LINE__, notificationID);

	// add the push list
	TLOCAL_PUSH push;
	push.notificationID = notificationID;
	push.isUse = isUse;
	local_push_list.push_back(push);
}


JNI_FUNC_DCL(void, retCancelLocalNotification)(JNIEnv* env, jobject obj, jint ret)
{
	nsdk_st_query_push = NSDK_WINDOW_CLOSED;
	LOGI("%s[%3d]:ret: %d", __func__, __LINE__, ret);
}

};

#endif // ANDROID_DEVICE

#endif // #if defined(ANDROID_DEVICE) || defined(_WIN32)




//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ios interface

#if defined(IOS_MOBILE_DEVICE) && !defined(UNIT_TEST)

#include "app_nsdk_jni.h"

#include "external_data.h"
#include "game_manager.h"
#include "game_gui_container.h"
#include "game_object_if.h"
#include "game_gui_dialog_login.h"
#include "key_state_def.h"

#include "EPP_Tick_Time.h"

using namespace EPP;

static int g_bInit  = 0;	// is initialize


int appi_init(int w, int h, const char* s_resource, const char* s_accessible)
{
	const char* gl_ver = gl_Version();
	LOGI("OGL VERSION: %s", gl_ver);

	sEPP_VIEWPORT vp;
	vp._uiTopX = 0;
	vp._uiTopY = 0;
	vp._fWidth = w;
	vp._fHeight = h;
	vp._fMinDepth = 0;
	vp._fMaxDepth = 1;

	if(0 == g_bInit)
	{
		game_manager::make_instance();
		game_manager::get_singleton().create(w, h, (char*)s_resource, (char*)s_accessible);
	}

	g_engine_core.set_window_size(w,h);
	g_engine_core.set_screen_size_by_process(w,h);
	g_engine_core.set_viewport_by_process(&vp);
	g_engine_core.set_camera_aspect_width(w);
	g_engine_core.set_camera_aspect_height(h);

	float fWScale = w / (1920.0f * 0.5f);
	float fHScale = h / (1080.0f * 0.5f);
	float fStd_Aspect = 1920.0f / 1080.0f;
	float fAspect = (float)w / (float)h;
	if (fAspect < fStd_Aspect){
		float fDestWidth = fStd_Aspect * (float)h;
		float fOffsetMultiply = (float)w / fDestWidth;
		fHScale *= fOffsetMultiply;
	}

	g_engine_core.set_scale_w_all_interface_control(fHScale);
	g_engine_core.set_scale_h_all_interface_control(fHScale);
	g_engine_core.set_offset_dist_scale_w_all_interface_control(fHScale);
	g_engine_core.set_offset_dist_scale_h_all_interface_control(fHScale);

	//g_engine_core.set_error_message_funct(error_message);

	g_bInit = 1;
	return g_bInit;
}


void appi_update()
{
}

void appi_render()
{
	float fDeltaTime = (float)g_tick_time.get_tick_delta_time() * 0.001f;

	game_manager::get_singleton().pre_process(fDeltaTime);
	game_manager::get_singleton().process();
	game_manager::get_singleton().post_process();
	game_manager::get_singleton().render();
}

void appi_stop()
{
}

void appi_pause()
{
	if(!is_alive_engine_core())
		return;

	g_engine_core.set_all_sound_volume_percentage(0.0f);
}

void appi_resume()
{
	if(!is_alive_engine_core())
		return;

	g_engine_core.set_all_sound_volume_percentage(1.0f);
}

void appi_destroy()
{
	g_engine_core.destroy();
	destroy_engine_core();
	g_game_manager.destroy();
	game_manager::remove_instance();
	g_bInit = false;
}

void appi_restart()
{
	/*g_engine_core.set_present_value_for_restore_device_data(-2);
	g_engine_core.restore_device_data();
	g_engine_core.set_present_value_for_restore_device_data(0);*/
}

void appi_backBtnDown()
{
	//g_game_manager.event_btn_back();
}

void appi_touchEvent(int nEvent, float x_1, float y_1, float x_2, float y_2)
{
	float f_screen_height = static_cast<float>(g_engine_core.get_screen_height_by_process());
	float f_screen_width = static_cast<float>(g_engine_core.get_screen_width_by_process());

	sEPP_IF_TOUCH_INFO* touch_0 = g_engine_core.get_touch_info(0);
	sEPP_IF_TOUCH_INFO* touch_1 = g_engine_core.get_touch_info(1);

	EPP_VECTOR2 v_touch_0 = EPP_VECTOR2(touch_0->x, touch_0->y);
	EPP_VECTOR2 v_touch_1 = EPP_VECTOR2(touch_1->x, touch_1->y);

	char sz_text[32] = { 0, };
	EPP_sprintf(sz_text, 32, "v_touch_0.x : %f", v_touch_0.x);
	//g_engine_core.log_info("touchevent :: ", sz_text);
	EPP_sprintf(sz_text, 32, "v_touch_0.y : %f", v_touch_0.y);
	//g_engine_core.log_info("touchevent :: ", sz_text);
	EPP_sprintf(sz_text, 32, "v_touch_1.x : %f", v_touch_1.x);
	//g_engine_core.log_info("touchevent :: ", sz_text);
	EPP_sprintf(sz_text, 32, "v_touch_1.y : %f", v_touch_1.y);
	//g_engine_core.log_info("touchevent :: ", sz_text);

	static int _n_touch_0 = 0;

	switch(nEvent)
	{
		case 0:	// down
		{
			g_engine_core.set_touch_info(0, x_1, y_1, eEPP_IF_BTN_STATE_DOWN, eEPP_IF_BTN_TYPE_L);
			if (g_engine_core.is_in_previous_rendered_interface_control((uint16)x_1, (uint16)y_1) == false)
			{
				game_manager::get_singleton().set_key_state(0, true, (uint16)x_1, (uint16)y_1);
			}
			//GAME_LOG("key_event = down_0");
		}break;

		case 1: // up
		{
			g_engine_core.set_touch_info(0, x_1, y_1, eEPP_IF_BTN_STATE_UP, eEPP_IF_BTN_TYPE_L);
			if (g_engine_core.is_in_previous_rendered_interface_control((uint16)x_1, (uint16)y_1) == false)
			{
				game_manager::get_singleton().set_key_state(0, false, (uint16)x_1, (uint16)y_1);
			}
			//GAME_LOG("key_event = up_1");
		}break;

		case 2: // pointer down
		{
			g_engine_core.set_touch_info(1, x_2, y_2, eEPP_IF_BTN_STATE_DOWN, eEPP_IF_BTN_TYPE_L);
			//GAME_LOG("key_event = pointer down_2");
		}break;

		case 3: // pointer up
 		{
			g_engine_core.set_touch_info(1, x_2, y_2, eEPP_IF_BTN_STATE_UP, eEPP_IF_BTN_TYPE_L);

			//인터페이스에 있던 입력을 리셋
			sEPP_IF_TOUCH_INFO* touch_info = g_epp_interface_mng.get_touch_info(1);
			touch_info->eType = eEPP_IF_BTN_TYPE_STD;
			touch_info->eState = eEPP_IF_BTN_STATE_NONE;
			game_manager::get_singleton().set_key_state(1, false, (uint16)x_2, (uint16)y_2);
			//g_epp_interface_mng.exit_screen(1);

			//GAME_LOG("key_event = pointer up_3");

//			g_engine_core.set_touch_info(1, x_2, y_2, eEPP_IF_BTN_STATE_UP, eEPP_IF_BTN_TYPE_L);
//
// 			EPP_VECTOR2 v_curr_touch(x_2, y_2);
// 			if( (v_curr_touch - v_touch_0).vec2Length() < (v_curr_touch - v_touch_1).vec2Length() )	{
// 				_n_touch_0 = 1;
// 				g_engine_core.set_touch_info(0, x_2, y_2, eEPP_IF_BTN_STATE_UP, eEPP_IF_BTN_TYPE_L);
// 			}
// 			else
// 			{`
// 				_n_touch_0 = 0;
// 				g_engine_core.set_touch_info(1, x_2, y_2, eEPP_IF_BTN_STATE_UP, eEPP_IF_BTN_TYPE_L);
// 			}
		}break;

		case 4: // move
		{
			//GAME_LOG("key_event = move_4  ");
			g_engine_core.set_touch_info(0, x_1, y_1, eEPP_IF_BTN_STATE_DOWN, eEPP_IF_BTN_TYPE_L);
			if (g_engine_core.is_in_previous_rendered_interface_control((uint16)x_1, (uint16)y_1) == false)
			{
				game_manager::get_singleton().set_key_state(0, true, (uint16)x_1, (uint16)y_1);
			}
// 			if( g_engine_core.get_touch_info(1)->eState != eEPP_IF_BTN_STATE_STD ) {
// 				g_engine_core.set_touch_info(1, 0, 0, eEPP_IF_BTN_STATE_UP, eEPP_IF_BTN_TYPE_L);
// 			}
//
// 			if( g_engine_core.is_in_previous_rendered_interface_control((uint16)x_1, (uint16)y_1) == false )
// 			{
// 				game_manager::get_singleton().set_key_state(0, true, (uint16)x_1, (uint16)y_1);
// 			}
		}break;

		case 5: // move
		{
			//GAME_LOG("key_event = move_5  ");
			g_engine_core.set_touch_info(0, x_1, y_1, eEPP_IF_BTN_STATE_DOWN, eEPP_IF_BTN_TYPE_L);
			g_engine_core.set_touch_info(1, x_2, y_2, eEPP_IF_BTN_STATE_DOWN, eEPP_IF_BTN_TYPE_L);

// 			if( _n_touch_0 == 0)
// 			{
//				g_engine_core.set_touch_info(0, x_1, y_1, eEPP_IF_BTN_STATE_DOWN, eEPP_IF_BTN_TYPE_L);
//				g_engine_core.set_touch_info(1, x_2, y_2, eEPP_IF_BTN_STATE_DOWN, eEPP_IF_BTN_TYPE_L);
// 			}
// 			else
// 			{
// 				g_engine_core.set_touch_info(1, x_1, y_1, eEPP_IF_BTN_STATE_DOWN, eEPP_IF_BTN_TYPE_L);
// 				g_engine_core.set_touch_info(0, x_2, y_2, eEPP_IF_BTN_STATE_DOWN, eEPP_IF_BTN_TYPE_L);
// 			}

// 			if( g_engine_core.is_in_previous_rendered_interface_control((uint16)x_2, (uint16)y_2) == false )
// 			{
// 				game_manager::get_singleton().set_key_state(1, true, (uint16)x_2, (uint16)y_2);
// 			}
		}break;
	}
} // appTouchEvent


#endif //IOS_MOBILE_DEVICE




#if defined(UNIT_TEST)

void save_contract_agreement(int v)
{
	LOGI("%s[%3d]:ret: %d\n", __func__, __LINE__, v);
}

void game_dialog_policy(void)
{
	LOGI("%s[%3d]:  ------\n", __func__, __LINE__);
}

#endif


static int abc = 12;
