LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MyLibName

LOCAL_CPPFLAGS := \
    -std=c++17 \
    -Wall \
    -Wextra \
    -Wno-unused-parameter

LOCAL_CFLAGS := \
    -Wall \
    -Wextra

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/engine \
    $(LOCAL_PATH)/config \
    $(LOCAL_PATH)/third_party

LOCAL_SRC_FILES := \
    Main.cpp \
    engine/MockGameEngine.cpp \
    config/ConfigManager.cpp \
    config/JsonConfigProvider.cpp

LOCAL_LDLIBS := \
    -llog \
    -landroid

include $(BUILD_SHARED_LIBRARY)
