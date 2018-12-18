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
