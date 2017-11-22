
# The ARMv7 is significanly faster due to the use of the hardware FPU

NDK_TOOLCHAIN_VERSION := 4.9

APP_PLATFORM := android-16
APP_ABI      := armeabi armeabi-v7a
APP_OPTIM    := release
APP_STL      := gnustl_static

APP_CPPFLAGS += -fexceptions -frtti
APP_CPPFLAGS += -fno-common -march=armv5te -mtune=arm9tdmi -mapcs -msoft-float -fno-short-enums
APP_CPPFLAGS += -ftree-vectorize
#APP_CPPFLAGS += -mfpu=neon -mfloat-abi=softfp

APP_CPPFLAGS += -Wall
APP_CPPFLAGS += -Wno-psabi
APP_CPPFLAGS += -Wno-unknown-pragmas
APP_CPPFLAGS += -Wno-unused-but-set-variable
APP_CPPFLAGS += -Wno-unused-function
APP_CPPFLAGS += -Wno-unused-local-typedefs
APP_CPPFLAGS += -Wno-unused-variable
APP_CPPFLAGS += -Wno-unused-value

APP_CPPFLAGS += -Wno-switch
APP_CPPFLAGS += -Wno-write-strings
APP_CPPFLAGS += -Wno-reorder
APP_CPPFLAGS += -Wno-sign-compare

APP_CPPFLAGS += -Wno-conversion-null
APP_CPPFLAGS += -Wno-maybe-uninitialized
APP_CPPFLAGS += -Wno-sequence-point
APP_CPPFLAGS += -Wno-delete-non-virtual-dtor
APP_CPPFLAGS += -Wno-parentheses

APP_CPPFLAGS += -Warray-bounds
APP_CPPFLAGS += -Waggressive-loop-optimizations
