LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Sets output module name to match competitor masking convention
LOCAL_MODULE    := skia_ext
LOCAL_SRC_FILES := memread.cpp

# Link core Android logging and optimize C++ flags
LOCAL_LDLIBS    := -llog
LOCAL_CPPFLAGS  := -std=c++17 -O3 -fvisibility=hidden

include $(BUILD_SHARED_LIBRARY)
