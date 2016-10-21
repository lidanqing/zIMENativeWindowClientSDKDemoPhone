LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := CommonUtilitiesLib
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libCommonUtilitiesLib.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := LightLogLib
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libLightLog.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := RTCPLib
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libRTCPLib.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := RTPPackerLib
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libRTPPackerLib.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := RTPParserLib
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libRTPParser.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := H264DEC_ANDROID
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libzteH264Dec_ARM11.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE := H264DEC_NEON_ANDROID
#LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libzteH264Dec.so
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libzteH264DecARMv7Android.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE := H265DEC_NEON_ANDROID
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libzteH265DecARMv7Android.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE := H264ENC_ANDROID
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libzteH264EncARMv5Android.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := H264ENC_NEON_ANDROID
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libzteH264EncARMv7Android.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := H265ENC_NEON_ANDROID
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libzteH265EncARMv7Android.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := FEC
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libRTPFECLib.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ZIMEClientSDK
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libZIMEClientSDKAndroid.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VideoScaleLib
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libyuv_static.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VideoEnhanceA11
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libzteVideoEnhanceARM11Android.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := VideoEnhanceNEON
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libzteVideoEnhanceNEON_Android.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ZIMECodecDevCallBack
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libZIMECodecDevCallBack.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := AEC_ARM_ANDROID
LOCAL_SRC_FILES := ../../../../bin/Android/Android_av/libAEC_ARM_ANDROID.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../Include
include $(PREBUILT_SHARED_LIBRARY)

################################Android ZIMEVideoClientJNI Project Makefile##############################
#LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ZIMEClientSDKJNI

LOCAL_CFLAGS := -D_ZM_ANDROID_

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../include \
										$(LOCAL_PATH)/../../CodecDevCallBack \
										$(LOCAL_PATH)/../../CodecDevCallBack/include \
										$(LOCAL_PATH)/.. \
										$(LOCAL_PATH)/../../../Common/include \
										$(LOCAL_PATH)/../../../Common/libs/CommonUtilitiesLib \
										$(LOCAL_PATH)/../../../Common/libs/LightLogLib \
										
LOCAL_SHARED_LIBRARIES :=  CommonUtilitiesLib LightLogLib RTCPLib RTPPackerLib RTPParserLib \
						   FEC ZIMEClientSDK VideoScaleLib ZIMECodecDevCallBack \
						   

LOCAL_SRC_FILES := 	./zime_media_ZIMEVideoClientJNI.cpp\
			./zime_audio_ZIMEClientJni.cpp\
			./ZIMEAndroidQoSNotify.cpp\
			./../../../Common/src/ZIMEExtnTransThread.cpp\
			./../../../Common/src/ZIMEExtTransport.cpp\

LOCAL_LDLIBS    += -llog -landroid

include $(BUILD_SHARED_LIBRARY)
