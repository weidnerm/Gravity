LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := GravityEngine

LOCAL_SRC_FILES := Vector.cpp \
                   GravityObject.cpp \
                   Gravity_lib.cpp   \
                   InitMain.cpp      \
                   SolarSystem.cpp   \
                   GravityEngine.cpp        \
                   GravitySim.cpp        \

include $(BUILD_SHARED_LIBRARY)

