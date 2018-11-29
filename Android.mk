LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    libhook/hook.cpp \
    cflumen/maps.cpp \
    cflumen/cflumen.cpp \
    hook_main.cpp

LOCAL_MODULE := cflumen
#LOCAL_CFLAGS += -DDEBUG
LOCAL_CFLAGS += -std=c++11
LOCAL_LDLIBS := -lm -ldl -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    libinject/inject.cpp \
    inject_main.cpp

LOCAL_MODULE := inject
#LOCAL_CFLAGS += -DDEBUG
LOCAL_LDLIBS := -ldl -llog

include $(BUILD_EXECUTABLE)

all:
    -mv $(NDK_APP_PROJECT_PATH)/libs/armeabi/libcflumen.so $(NDK_APP_PROJECT_PATH)/assets/libcflumen.arm.bin
    -mv $(NDK_APP_PROJECT_PATH)/libs/armeabi/inject $(NDK_APP_PROJECT_PATH)/assets/inject.arm.bin
    -mv $(NDK_APP_PROJECT_PATH)/libs/armeabi-v7a/libcflumen.so $(NDK_APP_PROJECT_PATH)/assets/libcflumen.armv7.bin
    -mv $(NDK_APP_PROJECT_PATH)/libs/armeabi-v7a/inject $(NDK_APP_PROJECT_PATH)/assets/inject.armv7.bin
    -mv $(NDK_APP_PROJECT_PATH)/libs/arm64-v8a/libcflumen.so $(NDK_APP_PROJECT_PATH)/assets/libcflumen.arm64.bin
    -mv $(NDK_APP_PROJECT_PATH)/libs/arm64-v8a/inject $(NDK_APP_PROJECT_PATH)/assets/inject.arm64.bin
    -mv $(NDK_APP_PROJECT_PATH)/libs/x86/libcflumen.so $(NDK_APP_PROJECT_PATH)/assets/libcflumen.x86.bin
    -mv $(NDK_APP_PROJECT_PATH)/libs/x86/inject $(NDK_APP_PROJECT_PATH)/assets/inject.x86.bin
    -mv $(NDK_APP_PROJECT_PATH)/libs/x86_64/libcflumen.so $(NDK_APP_PROJECT_PATH)/assets/libcflumen.x64.bin
    -mv $(NDK_APP_PROJECT_PATH)/libs/x86_64/inject $(NDK_APP_PROJECT_PATH)/assets/inject.x64.bin
