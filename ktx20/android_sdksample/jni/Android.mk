#########1#########2#########3#########4#########5#########6#########7#########8
# static library compile

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

NDK_ROOT        := $(ANDROID_NDK_ROOT)
EPP_PATH        := $(LOCAL_PATH)/../../..

NDK_PLATFORM    := $(NDK_ROOT)/platforms/android-17/arch-arm/usr
NDK_STDC        := $(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.8
NDK_STD_EABI    := $(NDK_STDC)/libs/armeabi/include

EPP_LIB_STATIC  := $(EPP_PATH)/epp_aos_lib
PROUD_BROKER    := $(EPP_PATH)/proudnetbroker

LOCAL_MODULE    := epp-jni

LOCAL_SRC_FILES := gl_util.cpp gl_app.cpp app_nsdk_jni.cpp

LOCAL_CPPFLAGS   += -D__ARM__
LOCAL_CPPFLAGS   += -DSTUB_LIB_PACKAGE -DANDROID_DEVICE -DUNIT_TEST

LOCAL_CPPFLAGS   += -I./
LOCAL_CPPFLAGS   += -I$(NDK_PLATFORM)/include
LOCAL_CPPFLAGS   += -I$(NDK_STDC)/include
LOCAL_CPPFLAGS   += -I$(NDK_STD_EABI)
LOCAL_CPPFLAGS   += -I$$(EPP_PATH)/ktx20/include

LOCAL_C_INCLUDES += $(PROUD_BROKER)/include

LOCAL_CPPFLAGS   += -fpic -frtti -O2
LOCAL_CPPFLAGS   += -fomit-frame-pointer
LOCAL_CPPFLAGS   += -fno-strict-aliasing
LOCAL_CPPFLAGS   += -finline-limit=100
LOCAL_CPPFLAGS   += -fstack-protector
LOCAL_CPPFLAGS   += -fno-short-enums
LOCAL_CPPFLAGS   += -marm -ftree-vectorize
LOCAL_CPPFLAGS   += -std=c++11

LOCAL_LDLIBS += -L$(EPP_LIB_STATIC)
LOCAL_LDLIBS += -L$(NDK_STDC)/libs/armeabi

LOCAL_LDLIBS += -lproudnetbroker -lProudNetClient-wchar -lProudNetClient

LOCAL_LDLIBS += -lGLESv2 -lEGL -landroid -llog -ljnigraphics -lktx20
LOCAL_LDLIBS += -lstdc++ -lgnustl_static

include $(BUILD_SHARED_LIBRARY)
