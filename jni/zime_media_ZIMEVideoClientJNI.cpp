/*******************************************************************************
* ��Ȩ���� (C)2013,����ͨѶ�ɷ����޹�˾��
* ϵͳ����	: 
* �ļ�����	: zime_media_ZIMEVideoClientJNI.cpp
* ����ժҪ	: zime�����Ӧjniʵ��
* ��ǰ�汾	: 
* ��    ��	: 
* �������	: 2013��12��10�� ���ڶ�
* �޸ļ�¼	: 
* ��    ��		��   ��			�޸���		�޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <jni.h>
#include <android/native_window_jni.h>
#include <android/log.h>

#include "ZIMEAVClient_Extend.h"
#include "ZIMEType.h"
#include "zime_media_ZIMEVideoClientJNI.h"

#include "ZIMECallBackCommon.h"
#include "ZIMEVideoDevCallBackInterface.h"
#include "ZIMEVideoDecCallBackInterface.h"
#include "ZIMEAudioDevCallBackInterface.h"
#include "ZIMEAndroidQoSNotify.h"
#include "ZIMEVideoEncCallBackInterface.h"
#include "ZIMEExtVideoEncoderAmlogic.h"
#include "ZIMEExtVideoEncoderSoftware.h"
#include "ZIMEExtVideoEncoderMstar.h"
#include "ZIMEExtnTransThread.h"
#include "ZIMEExtTransport.h"
#include "ZIMEHMediaCodecHardDecode.h"
#include "ZIMEHMediaCodecHardDecodeInterface.h"

#include "LightLog.h"
/********************************** �ꡢ���� **********************************/
#define ZIMEVIDEOCLIENT_LOGI(...) __android_log_print(ANDROID_LOG_INFO, "ZIMEVideoClientJNI", __VA_ARGS__)
#define ZIMEVIDEOCLIENT_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "ZIMEVideoClientJNI", __VA_ARGS__)

/********************************** �������� **********************************/
void  LogCallbackPtr(const char* i_pLog)
{
	LightLog((char *)i_pLog);
}
/************************************ ���� ************************************/

static JavaVM *s_javavm = NULL; 
static JNIEnv *s_env = NULL;
static jint s_JNIVersion = JNI_VERSION_1_6;

static ZIMEAVClient_Extend* g_pVideoClient = NULL;
static T_ZIMEAudioCallBack s_tAudioCallBack;
static T_ZIMEVideoCallBack s_tVideoCallBack;
static T_ZIMEVideoFrameEncodeFun s_tVideoFrameEncFun;

static T_ZIMEVideoFrameDecodeFun s_tVideoFrameDecFun;
//static T_ZIMEVideoEncoderAbility s_tVideoCodecAbility;

static ZIME_NetworkQualityNotify *s_pNetworkNotify = NULL;
static jobject s_jLocalSurface = NULL;
static jobject s_jRemoteSurface = NULL;
static jobject s_jActivity = NULL;
static jboolean s_UsingEncodeCAllback = false;
static ZIMEExtVideoEncoderBase* s_pExtVideoEncoder = NULL;
static jint s_nCodecType = 0;
static ZIMEExtTransport* g_pExtTransport = NULL;
static CAudioRecvThread* g_pAThd		 = NULL;
static CVideoRecvThread* g_pVThd		 = NULL;
static CAudioRTCPRecvThread* g_pARTCPThd = NULL;
static CVideoRTCPRecvThread* g_pVRTCPThd = NULL;
/********************************** ����ʵ�� **********************************/

/*********************************** ��ʵ�� ***********************************/

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_Create(JNIEnv * env, jclass clazc)
{

	g_pVideoClient = ZIMEAVClient_Extend::Create();
	if (g_pVideoClient == NULL)
	{
		ZIMEVIDEOCLIENT_LOGE("CreateClient Engine Failed.");
		return -1;
	}	
	env->GetJavaVM(&s_javavm);
	if (NULL == s_javavm)
	{
		ZIMEVIDEOCLIENT_LOGE("Get javavm failed");
	}
	s_JNIVersion = env->GetVersion();
	// ��ʼ����������Ƶ�豸
	int nRet = ZIME_CallBack_Init(s_javavm, s_JNIVersion);
	if (nRet != 0)
	{
		ZIMEVIDEOCLIENT_LOGE("ZIME_CallBack_Init failed");
		return -1;
	}

	if(NULL == s_pNetworkNotify)
	{
		s_pNetworkNotify = new ZIMEAndroidQoSNotify(s_javavm, s_JNIVersion);
	}

	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_Destroy( JNIEnv* env, jclass )
{
	jint code = ZIMEAVClient_Extend::Destroy(g_pVideoClient);
	env->DeleteGlobalRef(s_jLocalSurface);
	env->DeleteGlobalRef(s_jRemoteSurface);
	env->DeleteGlobalRef(s_jActivity);

	ZIME_CallBack_UnInit();
	g_pVideoClient = NULL;   	

	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_Init( JNIEnv *, jclass )
{
	jint code = g_pVideoClient->ZIME_Init();
	 return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_Terminate( JNIEnv *, jclass )
{
	jint code = 0;	
	if (g_pVideoClient != NULL)
	{
		code = g_pVideoClient->ZIME_Terminate();
	}	
	return code;
}

// Java_zime_media_ZIMEVideoClientJNI_SetCallBack�뱾�����ĵ��÷���ͬһ���߳�
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_Exit( JNIEnv *, jclass )
{	
	jint code = ZIMEAVClient::Exit();
	if(NULL != s_pNetworkNotify)
	{
		delete s_pNetworkNotify;
		s_pNetworkNotify = NULL;
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetLogPath(JNIEnv *env, jclass, jstring i_path)
{
	jint code = 0; 
	
	const char* pPath = env->GetStringUTFChars(i_path, NULL);
	code = ZIMEAVClient::ZIME_SetLogPath(pPath);
	ZIMEVIDEOCLIENT_LOGE("SetLogPath>>>>%s, code[%d]", pPath, code);
	env->ReleaseStringUTFChars(i_path, pPath);
	
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetLogLevel(JNIEnv *, jclass, jint i_LogLevel)
{
	jint code = 0; 
	if (g_pVideoClient != NULL)
	{
		code = g_pVideoClient->ZIME_SetLogLevel(i_LogLevel);
	}
	return code;
		
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_CreateChannel(JNIEnv *, jclass, jboolean UsingEncodeCAllback, jint CodecType)
{
	jint code = g_pVideoClient->ZIME_CreateChannel(); // ��������Ƶ���贫�����FALSE
	::memset(&s_tVideoCallBack.nReserved, 0, sizeof(s_tVideoCallBack.nReserved));
	::memset(&s_tVideoFrameEncFun.nReserved, 0, sizeof(s_tVideoFrameEncFun.nReserved));

	//public static final int enumZIME_GOTAEXTERNENCODER  = 0x0;
	//public static final int enumZIME_AMLOGICHARDWEAR    = 0x1;
	//public static final int enumZIME_MediaCodec = 0x2;

	if(UsingEncodeCAllback)
	{
		switch (CodecType)
		{
		case 0x0:
			s_pExtVideoEncoder = (ZIMEExtVideoEncoderBase*)new ZIMEExtVideoEncoderSoftware();
			break;
		case 0x1:
			s_pExtVideoEncoder = (ZIMEExtVideoEncoderBase*)new ZIMEExtVideoEncoderAmlogic();
			break;
		case 0x2:
		default:
			s_pExtVideoEncoder = NULL;
		}
	}

	s_nCodecType = CodecType;
	s_UsingEncodeCAllback = UsingEncodeCAllback;

	return code;	 
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_DeleteChannel( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pVideoClient->ZIME_DeleteChannel(ChId);

	if(s_pExtVideoEncoder != NULL)
	{
		delete s_pExtVideoEncoder;
		s_pExtVideoEncoder = NULL;
	}
	s_tVideoFrameEncFun.nReserved[0] = 0;
	s_tVideoCallBack.nReserved[0]    = 0;
	s_UsingEncodeCAllback = false;

	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_MaxNumOfChannels( JNIEnv *, jclass )
{
	jint code = g_pVideoClient->ZIME_MaxNumOfChannels();
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetLocalReceiver( JNIEnv *env, jclass, jint ChId, jint AudioRTPPort, jint AudioRTCPPort,
																		   jint VideoRTPPort, jint VideoRTCPPort, jobject localAddr)
{		


	jint code = 0;
	if (AudioRTPPort != -1)
	{
		code = g_pVideoClient->ZIME_SetLocalReceiver(ChId, AudioRTPPort, AudioRTCPPort, VideoRTPPort, VideoRTCPPort);
	}
	else
	{
		code = g_pVideoClient->ZIME_SetVideoLocalReceiver(ChId, VideoRTPPort, VideoRTCPPort);
	}
	
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetLocalReceiver
  (JNIEnv *env, jclass, jint ChId, jobject AudioRTPPort, jobject AudioRTCPPort, jobject VideoRTPPort, jobject VideoRTCPPort, jobject localAddr, jint AddrLen)
{
	int u32AudioRTPPort = 0;
	int u32AudioRTCPPort = 0;
	int u32VideoRTPPort = 0;
	int u32VideoRTCPPort = 0;
	char aLocalAddr[32] = {0};
	int u32LocalAddrLen = 0;
	jint code = g_pVideoClient->ZIME_GetLocalReceiver(ChId, u32AudioRTPPort, u32AudioRTCPPort, u32VideoRTPPort, u32VideoRTCPPort, aLocalAddr, u32LocalAddrLen);

	jclass clazzAudioRTPPort = env->GetObjectClass(AudioRTPPort);
	jfieldID AudioRTPPortretuInt = env->GetFieldID(clazzAudioRTPPort, "retuInt", "I");
    env->SetIntField(AudioRTPPort, AudioRTPPortretuInt, u32AudioRTPPort);

	jclass clazzAudioRTCPPort = env->GetObjectClass(AudioRTCPPort);
	jfieldID AudioRTCPPortretuInt = env->GetFieldID(clazzAudioRTCPPort, "retuInt", "I");
    env->SetIntField(AudioRTCPPort, AudioRTCPPortretuInt, u32AudioRTCPPort);

	jclass clazzVideoRTPPort = env->GetObjectClass(VideoRTPPort);
	jfieldID VideoRTPPortretuInt = env->GetFieldID(clazzVideoRTPPort, "retuInt", "I");
    env->SetIntField(VideoRTPPort, VideoRTPPortretuInt, u32VideoRTPPort);

	jclass clazzVideoRTCPPort = env->GetObjectClass(VideoRTCPPort);
	jfieldID VideoRTCPPortretuInt = env->GetFieldID(clazzVideoRTCPPort, "retuInt", "I");
    env->SetIntField(VideoRTCPPort, VideoRTCPPortretuInt, u32VideoRTCPPort);

	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSendDestination( JNIEnv *env, jclass, jint ChId, jint AudioRTPPort, jint AudioRTCPPort, 
								 jint VideoRTPPort, jint VideoRTCPPort, jobject jDstAddr)
{

	jclass clazz = env->GetObjectClass(jDstAddr);
	jfieldID retuStr = env->GetFieldID(clazz, "retuStr", "Ljava/lang/String;");
	jstring jstr = (jstring)env->GetObjectField(jDstAddr, retuStr);	
	const char* dstAddr = env->GetStringUTFChars(jstr, false);
	jint code = 0;
	if (AudioRTPPort != -1)
	{
		code = g_pVideoClient->ZIME_SetSendDestination(ChId, dstAddr, AudioRTPPort, AudioRTCPPort, VideoRTPPort, VideoRTCPPort);
	}
	else
	{
		code = g_pVideoClient->ZIME_SetVideoSendDestination(ChId, dstAddr, VideoRTPPort, VideoRTCPPort);
	}
	
	env->ReleaseStringUTFChars(jstr, dstAddr);
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetSendDestination
  (JNIEnv *, jclass, jint, jobject, jint, jobject, jobject, jobject, jobject)
{
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartListen( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pVideoClient->ZIME_StartListen(ChId);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopListen( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pVideoClient->ZIME_StopListen(ChId);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartPlayout( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pVideoClient->ZIME_StartPlayout(ChId);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopPlayout( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pVideoClient->ZIME_StopPlayout(ChId);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartSend( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pVideoClient->ZIME_StartSend(ChId);
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopSend( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pVideoClient->ZIME_StopSend(ChId);
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetNumOfAudioCodecs( JNIEnv *, jclass )
{
	jint code = g_pVideoClient->ZIME_GetNumOfAudioCodecs();
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetNumOfVideoCodecs( JNIEnv *, jclass )
{
	jint code = g_pVideoClient->ZIME_GetNumOfVideoCodecs();
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetAudioCodec( JNIEnv *env, jclass, jint CodecIdx, jobject AudioCodecInfo)
{
	T_ZIMEAudioCodecInfo codecInfo;
    jint code = g_pVideoClient->ZIME_GetAudioCodec(CodecIdx, codecInfo);
	
	jclass clazz = env->GetObjectClass(AudioCodecInfo);
	
	jfieldID s32PTId = env->GetFieldID(clazz, "s32PT", "I");
    env->SetIntField(AudioCodecInfo, s32PTId, codecInfo.s32PT);
	
	jfieldID s32SampleRateId = env->GetFieldID(clazz, "s32SampleRate", "I");
    env->SetIntField(AudioCodecInfo, s32SampleRateId, codecInfo.s32SampleRate);
	
	jfieldID s32PacketSizeId = env->GetFieldID(clazz, "s32PacketSize", "I");
    env->SetIntField(AudioCodecInfo, s32PacketSizeId, codecInfo.s32PacketSize);
	
	jfieldID s32ChannelNumId = env->GetFieldID(clazz, "s32ChannelNum", "I");
    env->SetIntField(AudioCodecInfo, s32ChannelNumId, codecInfo.s32ChannelNum);
	
	jfieldID s32BitRateId = env->GetFieldID(clazz, "s32BitRate", "I");
    env->SetIntField(AudioCodecInfo, s32BitRateId, codecInfo.s32BitRate);
	
	jfieldID aPTNameId = env->GetFieldID(clazz, "aPTName", "Ljava/lang/String;");
	jstring nameTemp = env->NewStringUTF(codecInfo.aPTName);
    env->SetObjectField(AudioCodecInfo, aPTNameId, (jobject)nameTemp);

	jfieldID eExtnTypeID = env->GetFieldID(clazz, "eExtnType", "I");
	env->SetIntField(AudioCodecInfo, eExtnTypeID, codecInfo.eExtnType);
	
	jfieldID codecExInfoID = env->GetFieldID(clazz, "codecExInfo", "I");
	env->SetIntField(AudioCodecInfo, codecExInfoID, codecInfo.uCodecExInfo.cAMRPackerMode);

     return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetVideoCodec(JNIEnv *env, jclass, jint CodecIdx, jobject VideoCodecInfo)
{
	T_ZIMEVideoCodecInfo codecInfo;
    jint code = g_pVideoClient->ZIME_GetVideoCodec(CodecIdx, codecInfo);
	
	jclass clazz = env->GetObjectClass(VideoCodecInfo);
	
	jfieldID s32PTId = env->GetFieldID(clazz, "s32PT", "I");
    env->SetIntField(VideoCodecInfo, s32PTId, codecInfo.s32PT);
	
	jfieldID s32SampleRateId = env->GetFieldID(clazz, "s32SampleRate", "I");
    env->SetIntField(VideoCodecInfo, s32SampleRateId, codecInfo.s32SampleRate);
	
	jfieldID s32PacketSizeId = env->GetFieldID(clazz, "s32PacketSize", "I");
    env->SetIntField(VideoCodecInfo, s32PacketSizeId, codecInfo.s32PacketSize);
	
	jfieldID s32ChannelNumId = env->GetFieldID(clazz, "s32ChannelNum", "I");
    env->SetIntField(VideoCodecInfo, s32ChannelNumId, codecInfo.s32ChannelNum);
	
	jfieldID s32BitRateId = env->GetFieldID(clazz, "s32BitRate", "I");
    env->SetIntField(VideoCodecInfo, s32BitRateId, codecInfo.s32BitRate);
	
	jfieldID aPTNameId = env->GetFieldID(clazz, "aPTName", "Ljava/lang/String;");
	jstring nameTemp = env->NewStringUTF(codecInfo.aPTName);
    env->SetObjectField(VideoCodecInfo, aPTNameId, (jobject)nameTemp);

	jfieldID s32Width = env->GetFieldID(clazz, "s32Width", "I");
    env->SetIntField(VideoCodecInfo, s32Width, codecInfo.s32Width);

	jfieldID s32Height = env->GetFieldID(clazz, "s32Height", "I");
    env->SetIntField(VideoCodecInfo, s32Height, codecInfo.s32Height);

	jfieldID u32FrameRate = env->GetFieldID(clazz, "u32FrameRate", "I");
    env->SetIntField(VideoCodecInfo, u32FrameRate, codecInfo.u32FrameRate);

	jfieldID eExtnType = env->GetFieldID(clazz, "eExtnType", "I");
    env->SetIntField(VideoCodecInfo, eExtnType, codecInfo.eExtnType);

	jfieldID s32VBREnable = env->GetFieldID(clazz, "s32VBREnable", "I");
	env->SetIntField(VideoCodecInfo, s32VBREnable, codecInfo.s32VBREnable);
	
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSendCodec( JNIEnv *env, jclass, jint ChId, jobject AudioCodecInfo, jobject VideoCodecInfo)
{
	// ����������Ϣ
	T_ZIMEAudioCodecInfo codecInfo;
	if (NULL != AudioCodecInfo)
	{
		jclass clazzOne = env->GetObjectClass(AudioCodecInfo);	
		jfieldID s32PTId = env->GetFieldID(clazzOne, "s32PT", "I"); 
		codecInfo.s32PT = env->GetIntField(AudioCodecInfo, s32PTId);

		jfieldID s32SampleRateId = env->GetFieldID(clazzOne, "s32SampleRate", "I"); 
		codecInfo.s32SampleRate = env->GetIntField(AudioCodecInfo, s32SampleRateId);

		jfieldID s32PacketSizeId = env->GetFieldID(clazzOne, "s32PacketSize", "I"); 
		codecInfo.s32PacketSize = env->GetIntField(AudioCodecInfo, s32PacketSizeId);

		jfieldID s32ChannelNumId = env->GetFieldID(clazzOne, "s32ChannelNum", "I"); 
		codecInfo.s32ChannelNum = env->GetIntField(AudioCodecInfo, s32ChannelNumId);

		jfieldID s32BitRateId = env->GetFieldID(clazzOne, "s32BitRate", "I"); 
		codecInfo.s32BitRate = env->GetIntField(AudioCodecInfo, s32BitRateId);

		jfieldID aPTNameId = env->GetFieldID(clazzOne, "aPTName", "Ljava/lang/String;"); 
		jstring nameTemp = (jstring)env->GetObjectField(AudioCodecInfo, aPTNameId);
		const char* strName = env->GetStringUTFChars(nameTemp, false);
		strcpy(codecInfo.aPTName, strName); 

		jfieldID eAudioExtnTypeId = env->GetFieldID(clazzOne, "eExtnType", "I"); 
		int iAudioExtnType = env->GetIntField(AudioCodecInfo, eAudioExtnTypeId);
		codecInfo.eExtnType = (E_ZIMERTPExtType)iAudioExtnType;

		jfieldID codecExInfoId = env->GetFieldID(clazzOne, "codecExInfo", "I");
		int icodecExInfo = env->GetIntField(AudioCodecInfo, codecExInfoId);
		codecInfo.uCodecExInfo.cAMRPackerMode = icodecExInfo;
		env->ReleaseStringUTFChars(nameTemp, strName);
	}
	else
	{
		ZIMEVIDEOCLIENT_LOGI("AudioCodecInfo object is NULL");
	}

	// ��Ƶ������Ϣ
	jclass Videoclazz = env->GetObjectClass(VideoCodecInfo);
	T_ZIMEVideoCodecInfo tVideoCodecInfo;
	
	jfieldID s32VideoPTId = env->GetFieldID(Videoclazz, "s32PT", "I"); 
	tVideoCodecInfo.s32PT = env->GetIntField(VideoCodecInfo, s32VideoPTId);

	jfieldID s32VideoSampleRateId = env->GetFieldID(Videoclazz, "s32SampleRate", "I"); 
	tVideoCodecInfo.s32SampleRate = env->GetIntField(VideoCodecInfo, s32VideoSampleRateId);
	
	jfieldID s32VideoPacketSizeId = env->GetFieldID(Videoclazz, "s32PacketSize", "I"); 
	tVideoCodecInfo.s32PacketSize = env->GetIntField(VideoCodecInfo, s32VideoPacketSizeId);

	
	jfieldID s32VideoChannelNumId = env->GetFieldID(Videoclazz, "s32ChannelNum", "I"); 
	tVideoCodecInfo.s32ChannelNum = env->GetIntField(VideoCodecInfo, s32VideoChannelNumId);

	jfieldID s32VideoInitBitRateId = env->GetFieldID(Videoclazz, "s32InitBitRate", "I"); 
	tVideoCodecInfo.s32InitialBitRate = env->GetIntField(VideoCodecInfo, s32VideoInitBitRateId);	
	
	jfieldID s32VideoBitRateId = env->GetFieldID(Videoclazz, "s32BitRate", "I"); 
	tVideoCodecInfo.s32BitRate = env->GetIntField(VideoCodecInfo, s32VideoBitRateId);	

	jfieldID s32VideoWidthId = env->GetFieldID(Videoclazz, "s32Width", "I"); 
	tVideoCodecInfo.s32Width = env->GetIntField(VideoCodecInfo, s32VideoWidthId);

	jfieldID s32VideoHeightId = env->GetFieldID(Videoclazz, "s32Height", "I"); 
	tVideoCodecInfo.s32Height = env->GetIntField(VideoCodecInfo, s32VideoHeightId);

	jfieldID u32VideoFrameRateId = env->GetFieldID(Videoclazz, "u32FrameRate", "I"); 
	tVideoCodecInfo.u32FrameRate = env->GetIntField(VideoCodecInfo, u32VideoFrameRateId);

	jfieldID s32MaxBitrateId = env->GetFieldID(Videoclazz, "s32MaxBitRate", "I");
	tVideoCodecInfo.s32MaxBitRate = env->GetIntField(VideoCodecInfo, s32MaxBitrateId);

	jfieldID s32VBREnableId = env->GetFieldID(Videoclazz, "s32VBREnable", "I");
	tVideoCodecInfo.s32VBREnable = env->GetIntField(VideoCodecInfo, s32VBREnableId);

	jfieldID eExtnTypeId = env->GetFieldID(Videoclazz, "eExtnType", "I"); 
	int iExtnType = env->GetIntField(VideoCodecInfo, eExtnTypeId);
	tVideoCodecInfo.eExtnType = (E_ZIMERTPExtType)iExtnType;

	jfieldID eSceneTypeId = env->GetFieldID(Videoclazz, "eSceneType", "I"); 
	int iSceneType = env->GetIntField(VideoCodecInfo, eSceneTypeId);
	tVideoCodecInfo.eSceneType = (E_ZIMESceneType)iSceneType;

	jfieldID aVideoPTNameId = env->GetFieldID(Videoclazz, "aPTName", "Ljava/lang/String;"); 
	jstring VideonameTemp = (jstring)env->GetObjectField(VideoCodecInfo, aVideoPTNameId);
	const char* VideostrName = env->GetStringUTFChars(VideonameTemp, false);
	strcpy(tVideoCodecInfo.aPTName, VideostrName); 	
	env->ReleaseStringUTFChars(VideonameTemp, VideostrName);
	
	jint code = 0;
	if (NULL != AudioCodecInfo )
	{
		code = g_pVideoClient->ZIME_SetSendCodec(ChId, codecInfo, tVideoCodecInfo);	
	}
	else
	{
		code = g_pVideoClient->ZIME_SetVideoSendCodec(ChId, tVideoCodecInfo);	
	}
    return code;	 
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetSendCodec(JNIEnv *, jclass, jint, jobject, jobject)
{
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetRecPayloadType( JNIEnv *env, jclass, jint ChId, jobject AudioCodecInfo, jobject VideoCodecInfo)
{
	// ����������Ϣ
	T_ZIMEAudioCodecInfo codecInfo;
	if (NULL != AudioCodecInfo)
	{
		jclass clazzOne = env->GetObjectClass(AudioCodecInfo); 
		jfieldID s32PTId = env->GetFieldID(clazzOne, "s32PT", "I"); 
		codecInfo.s32PT = env->GetIntField(AudioCodecInfo, s32PTId);

		jfieldID s32SampleRateId = env->GetFieldID(clazzOne, "s32SampleRate", "I"); 
		codecInfo.s32SampleRate = env->GetIntField(AudioCodecInfo, s32SampleRateId);

		jfieldID s32PacketSizeId = env->GetFieldID(clazzOne, "s32PacketSize", "I"); 
		codecInfo.s32PacketSize = env->GetIntField(AudioCodecInfo, s32PacketSizeId);

		jfieldID s32ChannelNumId = env->GetFieldID(clazzOne, "s32ChannelNum", "I"); 
		codecInfo.s32ChannelNum = env->GetIntField(AudioCodecInfo, s32ChannelNumId);

		jfieldID s32BitRateId = env->GetFieldID(clazzOne, "s32BitRate", "I"); 
		codecInfo.s32BitRate = env->GetIntField(AudioCodecInfo, s32BitRateId);

		jfieldID aPTNameId = env->GetFieldID(clazzOne, "aPTName", "Ljava/lang/String;"); 
		jstring nameTemp = (jstring)env->GetObjectField(AudioCodecInfo, aPTNameId);
		const char* strName = env->GetStringUTFChars(nameTemp, false);
		strcpy(codecInfo.aPTName, strName);	 
		env->ReleaseStringUTFChars(nameTemp, strName);

		jfieldID eAudioExtnTypeId = env->GetFieldID(clazzOne, "eExtnType", "I");
		int iAudioExtnType = env->GetIntField(AudioCodecInfo, eAudioExtnTypeId);
		codecInfo.eExtnType = (E_ZIMERTPExtType)iAudioExtnType;

		jfieldID codecExInfoId = env->GetFieldID(clazzOne, "codecExInfo", "I");
		int icodecExInfo = env->GetIntField(AudioCodecInfo, codecExInfoId);
		codecInfo.uCodecExInfo.cAMRPackerMode = icodecExInfo;
	}
	  

	// ��Ƶ������Ϣ
	jclass Videoclazz = env->GetObjectClass(VideoCodecInfo);	
	T_ZIMEVideoCodecInfo tVideoCodecInfo;

	jfieldID s32VideoPTId = env->GetFieldID(Videoclazz, "s32PT", "I"); 
	tVideoCodecInfo.s32PT = env->GetIntField(VideoCodecInfo, s32VideoPTId);
	
	jfieldID s32VideoSampleRateId = env->GetFieldID(Videoclazz, "s32SampleRate", "I"); 
	tVideoCodecInfo.s32SampleRate = env->GetIntField(VideoCodecInfo, s32VideoSampleRateId);
	
	jfieldID s32VideoPacketSizeId = env->GetFieldID(Videoclazz, "s32PacketSize", "I"); 
	tVideoCodecInfo.s32PacketSize = env->GetIntField(VideoCodecInfo, s32VideoPacketSizeId);
	
	jfieldID s32VideoChannelNumId = env->GetFieldID(Videoclazz, "s32ChannelNum", "I"); 
	tVideoCodecInfo.s32ChannelNum = env->GetIntField(VideoCodecInfo, s32VideoChannelNumId);

	jfieldID s32VideoInitBitRateId = env->GetFieldID(Videoclazz, "s32InitBitRate", "I"); 
	tVideoCodecInfo.s32InitialBitRate = env->GetIntField(VideoCodecInfo, s32VideoInitBitRateId);	
	
	jfieldID s32VideoBitRateId = env->GetFieldID(Videoclazz, "s32BitRate", "I"); 
	tVideoCodecInfo.s32BitRate = env->GetIntField(VideoCodecInfo, s32VideoBitRateId);	
	
	jfieldID s32VideoWidthId = env->GetFieldID(Videoclazz, "s32Width", "I"); 
	tVideoCodecInfo.s32Width = env->GetIntField(VideoCodecInfo, s32VideoWidthId);
	
	jfieldID s32VideoHeightId = env->GetFieldID(Videoclazz, "s32Height", "I"); 
	tVideoCodecInfo.s32Height = env->GetIntField(VideoCodecInfo, s32VideoHeightId);
	
	jfieldID u32VideoFrameRateId = env->GetFieldID(Videoclazz, "u32FrameRate", "I"); 
	tVideoCodecInfo.u32FrameRate = env->GetIntField(VideoCodecInfo, u32VideoFrameRateId);

	jfieldID s32MaxBitrateId = env->GetFieldID(Videoclazz, "s32MaxBitRate", "I");
	tVideoCodecInfo.s32MaxBitRate = env->GetIntField(VideoCodecInfo, s32MaxBitrateId);

	jfieldID s32VBREnableId = env->GetFieldID(Videoclazz, "s32VBREnable", "I");
	tVideoCodecInfo.s32VBREnable = env->GetIntField(VideoCodecInfo, s32VBREnableId);
	
	jfieldID eExtnTypeId = env->GetFieldID(Videoclazz, "eExtnType", "I"); 
	int iExtnType = env->GetIntField(VideoCodecInfo, eExtnTypeId);
	tVideoCodecInfo.eExtnType = (E_ZIMERTPExtType)iExtnType;

	jfieldID eSceneTypeId = env->GetFieldID(Videoclazz, "eSceneType", "I"); 
	int iSceneType = env->GetIntField(VideoCodecInfo, eSceneTypeId);
	tVideoCodecInfo.eSceneType = (E_ZIMESceneType)iSceneType;
	
	jfieldID aVideoPTNameId = env->GetFieldID(Videoclazz, "aPTName", "Ljava/lang/String;"); 
	jstring VideonameTemp = (jstring)env->GetObjectField(VideoCodecInfo, aVideoPTNameId);
	const char* VideostrName = env->GetStringUTFChars(VideonameTemp, false);
	strcpy(tVideoCodecInfo.aPTName, VideostrName); 
	env->ReleaseStringUTFChars(VideonameTemp, VideostrName);

	jint code = 0;
	if (NULL != AudioCodecInfo)
	{
		code = g_pVideoClient->ZIME_SetRecPayloadType(ChId, codecInfo, tVideoCodecInfo);
	}
	else
	{
		code = g_pVideoClient->ZIME_SetVideoRecvCodec(ChId,  tVideoCodecInfo);
	}
     return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetRecCodec(JNIEnv *, jclass, jint, jobject, jobject)
{
	return 0;
}
  
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSourceFilter(JNIEnv *env, jclass, jint ChId, jint audioPort, jint videoPort, jobject peerIP)
{
	jclass clazz = env->GetObjectClass(peerIP);
	jfieldID retuStr = env->GetFieldID(clazz, "retuStr", "Ljava/lang/String;");
	jstring jstr = (jstring)env->GetObjectField(peerIP, retuStr);
	const char* dstAddr = env->GetStringUTFChars(jstr, false);
	jint code = g_pVideoClient->ZIME_SetSourceFilter(ChId, audioPort, videoPort, dstAddr);
	return code;
}
  
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSendSSRC(JNIEnv *, jclass, jint ChId, jint nSSRC)
{
	jint code = g_pVideoClient->ZIME_SetSendSSRC(ChId, nSSRC);

	return code;
}
  
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetSendSSRC(JNIEnv *, jclass, jint, jobject)
{
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetAGCStatus( JNIEnv *, jclass, jboolean Enable, jint Mode)
{
	jint code = g_pVideoClient->ZIME_SetAGCStatus(Enable, (E_ZIMEAGCMode)Mode);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetAGCStatus(JNIEnv *, jclass, jobject, jobject)
{
	return 0;
}
  
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetECStatus( JNIEnv *, jclass, jboolean Enable)
{
	jint code = g_pVideoClient->ZIME_SetECStatus(Enable);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetECStatus(JNIEnv *, jclass, jobject)
{
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetNSStatus( JNIEnv *, jclass, jboolean Enable)
{
	jint code = g_pVideoClient->ZIME_SetNSStatus(Enable);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetNSStatus(JNIEnv *, jclass, jobject)
{
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSpeakerMode( JNIEnv *, jclass, jboolean Enable)
{
	jint code = g_pVideoClient->ZIME_SetSpeakerMode(Enable);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSampleRate(JNIEnv *env, jclass, jint i_sample)
{
	jint code = g_pVideoClient->ZIME_SetSampleRate(i_sample);
	return code;
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetCallBack(JNIEnv *env, jclass)
{	
	// ��Ƶ�ص�����
	s_tAudioCallBack.ProducerStartSend  = ZIME_AudioCallBack_ProducerOpen;
	s_tAudioCallBack.ProducerClose = ZIME_AudioCallBack_ProducerClose;
	s_tAudioCallBack.ProducerStart = ZIME_AudioCallBack_ProducerStart;
	s_tAudioCallBack.ProducerStop  = ZIME_AudioCallBack_ProducerStop;
	s_tAudioCallBack.GetFrame	   = ZIME_AudioCallBack_GetFrame; 
	
	s_tAudioCallBack.ConsumerOpen  = ZIME_AudioCallBack_ConsumerOpen;
	s_tAudioCallBack.ConsumerClose = ZIME_AudioCallBack_ConsumerClose;
	s_tAudioCallBack.ConsumerStart = ZIME_AudioCallBack_ConsumerStart;
	s_tAudioCallBack.ConsumerStop  = ZIME_AudioCallBack_ConsumerStop;
	s_tAudioCallBack.WriteFrame    = ZIME_AudioCallBack_WriteFrame;
	
	
	// ��Ƶ�ص�����
	if(s_UsingEncodeCAllback)
	{
		s_tVideoCallBack.ProducerOpen  = ZIME_ExternalH264_ProducerOpen;
		s_tVideoCallBack.ProducerClose = ZIME_ExternalH264_ProducerClose;
		s_tVideoCallBack.ProducerStart = ZIME_ExternalH264_ProducerStart;
		s_tVideoCallBack.ProducerStop  = ZIME_ExternalH264_ProducerStop;
		s_tVideoCallBack.GetFrame	   = ZIME_ExternalH264_GetFrame;
		s_tVideoCallBack.nReserved[0]  = (long)s_pExtVideoEncoder;
		ZIMEVIDEOCLIENT_LOGI("external H264, nReserver[0]:%p", s_tVideoCallBack.nReserved[0]);
	}
	else
	{
		s_tVideoCallBack.ProducerOpen  = ZIME_VideoCallBack_ProducerOpen;
		s_tVideoCallBack.ProducerClose = ZIME_VideoCallBack_ProducerClose;
		s_tVideoCallBack.ProducerStart = ZIME_VideoCallBack_ProducerStart;
		s_tVideoCallBack.ProducerStop  = ZIME_VideoCallBack_ProducerStop;
		s_tVideoCallBack.GetFrame	   = ZIME_VideoCallBack_GetFrame;
	}

	s_tVideoCallBack.ConsumerOpen  = ZIME_VideoCallBack_ConsumerOpen;
	s_tVideoCallBack.ConsumerClose = ZIME_VideoCallBack_ConsumerClose;
	s_tVideoCallBack.ConsumerStart = ZIME_VideoCallBack_ConsumerStart;
	s_tVideoCallBack.ConsumerStop  = ZIME_VideoCallBack_ConsumerStop;
	s_tVideoCallBack.WriteFrame    = ZIME_VideoCallBack_WriteFrame;

	jint code = g_pVideoClient->ZIME_SetCallBack(&s_tAudioCallBack, &s_tVideoCallBack);

	return code;
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoCallBack(JNIEnv *env, jclass)
{
	// ��Ƶ�ص�����
	if(s_UsingEncodeCAllback)
	{
		s_tVideoCallBack.ProducerOpen  = ZIME_ExternalH264_ProducerOpen;
		s_tVideoCallBack.ProducerClose = ZIME_ExternalH264_ProducerClose;
		s_tVideoCallBack.ProducerStart = ZIME_ExternalH264_ProducerStart;
		s_tVideoCallBack.ProducerStop  = ZIME_ExternalH264_ProducerStop;
		s_tVideoCallBack.GetFrame	   = ZIME_ExternalH264_GetFrame;
		s_tVideoCallBack.nReserved[0]  = (long)s_pExtVideoEncoder;
		ZIMEVIDEOCLIENT_LOGI("external H264, nReserver[0]:%p", s_tVideoCallBack.nReserved[0]);
	}
	else
	{
		s_tVideoCallBack.ProducerOpen  = ZIME_VideoCallBack_ProducerOpen;
		s_tVideoCallBack.ProducerClose = ZIME_VideoCallBack_ProducerClose;
		s_tVideoCallBack.ProducerStart = ZIME_VideoCallBack_ProducerStart;
		s_tVideoCallBack.ProducerStop  = ZIME_VideoCallBack_ProducerStop;
		s_tVideoCallBack.GetFrame	   = ZIME_VideoCallBack_GetFrame;
	}

	s_tVideoCallBack.ConsumerOpen  = ZIME_VideoCallBack_ConsumerOpen;
	s_tVideoCallBack.ConsumerClose = ZIME_VideoCallBack_ConsumerClose;
	s_tVideoCallBack.ConsumerStart = ZIME_VideoCallBack_ConsumerStart;
	s_tVideoCallBack.ConsumerStop  = ZIME_VideoCallBack_ConsumerStop;
	s_tVideoCallBack.WriteFrame    = ZIME_VideoCallBack_WriteFrame;

	jint code = g_pVideoClient->ZIME_SetVideoCallBack(&s_tVideoCallBack);

	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoEncodeFun(JNIEnv *env, jclass, jint i_s32ChId)
{
	//��Ƶ����ص�
	jint code = 0;

	s_tVideoFrameEncFun.Open 			 = ZIME_ExternalH264_EncOpen;
	s_tVideoFrameEncFun.Start 			 = ZIME_ExternalH264_EncStart;
	s_tVideoFrameEncFun.Encode           = ZIME_ExternalH264_EncProc;
	s_tVideoFrameEncFun.Stop			 = ZIME_ExternalH264_EncStop;
	s_tVideoFrameEncFun.Close 			 = ZIME_ExternalH264_EncClose;
	s_tVideoFrameEncFun.Config           = ZIME_ExternalH264_EncConfig;
	s_tVideoFrameEncFun.nReserved[0]     = (long)s_pExtVideoEncoder;

	//s_pExtVideoEncoder->SetVideoEncoderAbility(s_tVideoCodecAbility);
	
	ZIMEVIDEOCLIENT_LOGI("SetVideoEncodeFun, nReserver[0]:%p", s_tVideoFrameEncFun.nReserved[0]);
	code = g_pVideoClient->ZIME_SetVideoEncodeFun(i_s32ChId, &s_tVideoFrameEncFun);

	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoDecodeFun(JNIEnv *env, jclass, jint i_s32ChId)
{
	//��Ƶ����ص�
	jint code = 0;

	//public static final int enumZIME_GOTAEXTERNENCODER  = 0x0;
	//public static final int enumZIME_AMLOGICHARDWEAR    = 0x1;
	//public static final int enumZIME_MediaCodec = 0x2;

	switch(s_nCodecType)
	{
	case 0x1:
		s_tVideoFrameDecFun.Open 			 = ZIME_AmlogicDecodeCallBack_Open;
		s_tVideoFrameDecFun.Start 			 = ZIME_AmlogicDecodeCallBack_StartDecode;
		s_tVideoFrameDecFun.Decode           = ZIME_AmlogicDecodeCallBack_Decode;
		s_tVideoFrameDecFun.Stop			 = ZIME_AmlogicDecodeCallBack_StopDecode;
		s_tVideoFrameDecFun.Close 			 = ZIME_AmlogicDecodeCallBack_Close;
		break;
	case 0x2:
		s_tVideoFrameDecFun.Open 			 = ZIME_MediaCodecHardDecodeCallBack_Open;
		s_tVideoFrameDecFun.Start 			 = ZIME_MediaCodecHardDecodeCallBack_StartDecode;
		s_tVideoFrameDecFun.Decode           = ZIME_MediaCodecHardDecodeCallBack_Decode;
		s_tVideoFrameDecFun.Stop			 = ZIME_MediaCodecHardDecodeCallBack_StopDecode;
		s_tVideoFrameDecFun.Close 			 = ZIME_MediaCodecHardDecodeCallBack_Close;
		break;

	default:
		break;
	}

	code = g_pVideoClient->ZIME_SetVideoDecodeFun(i_s32ChId, &s_tVideoFrameDecFun);

	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVendorType(JNIEnv *, jclass, jint i_s32ChId, jint i_nVendorType)
{
	jint code = g_pVideoClient->ZIME_SetVendorType(i_s32ChId, (E_ZIMEVendorType)i_nVendorType);

	return code;
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetModeSubset(JNIEnv *env, jclass, jint i_s32ChId, jbyteArray i_aSubset, jint i_s32Size, jboolean i_b8UseMin)
{
	unsigned char aSubSet[20];
	jbyte aTmp[20];
	env->GetByteArrayRegion(i_aSubset, 0, i_s32Size,  aTmp);
	memcpy(aSubSet, aTmp, 20);
	jint code = 0;
	code = g_pVideoClient->ZIME_SetModeSubset(i_s32ChId, aSubSet, i_s32Size, i_b8UseMin);

	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetNetworkQualityNotify(JNIEnv *env, jclass, jint i_s32ChId)
{
	ZIMEVIDEOCLIENT_LOGE("in Java_zime_media_ZIMEVideoClientJNI_SetNetworkQualityNotify");
	jint code = g_pVideoClient->ZIME_SetNetworkQualityNotify(i_s32ChId, s_pNetworkNotify);
	return code;
}



JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetFECStatus
  (JNIEnv *, jclass, jint i_s32ChId, jboolean Enable)
{
	jint code = g_pVideoClient->ZIME_SetFECStatus(i_s32ChId, Enable);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetNACKStatus
  (JNIEnv *, jclass, jint i_s32ChId, jboolean Enable)
{
	jint code = g_pVideoClient->ZIME_SetNACKStatus(i_s32ChId, Enable);
    return code;
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetIframeInterval
  (JNIEnv *, jclass, jint i_s32ChId, jint i_s32IframeInterval)
{
	jint code = g_pVideoClient->ZIME_SetIframeInterval(i_s32ChId, i_s32IframeInterval);
    return code;	
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_ConfigVideoIntraFrameRefresh
  (JNIEnv *, jclass, jint i_s32ChId)
{
	jint code = g_pVideoClient->ZIME_ConfigVideoIntraFrameRefresh(i_s32ChId);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetCCE
  (JNIEnv *, jclass, jint i_s32ChId, jboolean Enable)
{
	g_pVideoClient->ZIME_SetCCE(i_s32ChId, Enable);
    return 0;	
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetDEE
  (JNIEnv *, jclass, jint i_s32ChId, jboolean Enable)
{
	g_pVideoClient->ZIME_SetDEE(i_s32ChId, Enable);
    return 0;	
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetDeBlock
  (JNIEnv *, jclass, jint i_s32ChId, jboolean Enable)
{
	g_pVideoClient->ZIME_SetDeBlock(i_s32ChId, Enable);
    return 0;	
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetBLE
  (JNIEnv *, jclass, jint i_s32ChId, jboolean Enable)
{
	g_pVideoClient->ZIME_SetBLE(i_s32ChId, Enable);
    return 0;	
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetDNO
  (JNIEnv *, jclass, jint i_s32ChId, jboolean Enable)
{
	g_pVideoClient->ZIME_SetDNO(i_s32ChId, Enable);
    return 0;	
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoDevCapSize( JNIEnv *, jclass, jint i_s32Width, jint i_s32Height)
{
	jint iRet = g_pVideoClient->ZIME_SetVideoDevCapSize(i_s32Width, i_s32Height);
	return iRet;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoDisplayWnd( JNIEnv *env, jclass , 
																			  jobject surfaceholderLocal, jobject surfaceholderRemote) 
{
    s_jRemoteSurface = env->NewGlobalRef(surfaceholderRemote);
	s_jLocalSurface  = env->NewGlobalRef(surfaceholderLocal);
	ZIME_SetVideoDisplaySurface(env, s_jLocalSurface, s_jRemoteSurface);
	return 0;																										   
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoDevices(JNIEnv *, jclass, jint i_nCameraID)
{
	g_pVideoClient->ZIME_SetVideoDevices(i_nCameraID);
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetInputMute( JNIEnv *env, jclass classTmp, jint ichannel, jboolean Enable)
{
	jint code = g_pVideoClient->ZIME_SetInputMute(ichannel, Enable);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartCapFileAsCamera(JNIEnv * env, jclass, jint i_s32ChId, jstring i_FilePath)
{
	jint code = 0; 
	
	const char* pFilePath = env->GetStringUTFChars(i_FilePath, NULL);
	if(g_pVideoClient != NULL)
	{
		code = g_pVideoClient->ZIME_StartCapFileAsCamera(i_s32ChId, pFilePath);
	}
	ZIMEVIDEOCLIENT_LOGE("StartCapFileAsCamera>>>>%s, code[%d]", pFilePath, code);
	env->ReleaseStringUTFChars(i_FilePath, pFilePath);
	
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopCapFileAsCamera(
		JNIEnv *, jclass, jint i_s32ChId) {
	jint code = 0;
	if (g_pVideoClient != NULL) {
		code = g_pVideoClient->ZIME_StopCapFileAsCamera(i_s32ChId);
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetCapaBilitySet(JNIEnv *, jclass, jint i_s32ChId, jint i_nType)
{
//	T_ZIMEVideoCapability aVideoSoftWareCapabilitySet[] =
//	{
//		{320, 240},
//		{640, 480}
//	};
//
//	T_ZIMEVideoCapability aVideoNvHardWareCapabilitySet[] =
//	{
//		{320, 240},
//		{640, 480},
//		{1280, 720}
//	};
//
//	T_ZIMEVideoCapability aVideoMTKHardWareCapabilitySet[] =
//	{
//		{320, 240},
//		{640, 480},
//		{1280, 720}
//	};
//
//	T_ZIMEVideoCapability aVideoMediaCodecCapabilitySet[] =
//	{
//		{320, 240},
//		{640, 480}
//	};
//
//	T_ZIMEVideoCapability* pCapSet = NULL;
//	unsigned int u32CodecNum = 0;
//	if(i_nType == 0 )
//	{
//		pCapSet = aVideoSoftWareCapabilitySet;
//		u32CodecNum = sizeof(aVideoSoftWareCapabilitySet) / sizeof(T_ZIMEVideoCapability);
//	}
//	else if (i_nType = 3)
//	{
//		pCapSet = aVideoMTKHardWareCapabilitySet;
//		u32CodecNum = sizeof(aVideoMTKHardWareCapabilitySet) / sizeof(T_ZIMEVideoCapability);
//	}
//	else if(i_nType == 6)
//	{
//		pCapSet = aVideoMediaCodecCapabilitySet;
//		u32CodecNum = sizeof(aVideoMediaCodecCapabilitySet) / sizeof(T_ZIMEVideoCapability);
//	}
//	else
//	{
//		pCapSet = aVideoNvHardWareCapabilitySet;
//		u32CodecNum = sizeof(aVideoNvHardWareCapabilitySet) / sizeof(T_ZIMEVideoCapability);
//	}
//
//	jint code = g_pVideoClient->ZIME_SetVideoCapabilitySet(i_s32ChId, u32CodecNum, pCapSet);
//
//	T_ZIMEVideoQualityLevel tVideoQualityLevel[] =
//	{
//			{320, 240, 10, 300000},  {320, 240, 15,  400000},  {320, 240 , 20,  500000},
//			{640, 480,  10, 600000}, {640, 480,  15,  700000}, {640, 480,   20,  800000},
//			{1280, 720, 10, 900000}, {1280, 720, 15, 1200000}, {1280, 720,  20, 1500000}
//	};
//	g_pVideoClient->ZIME_SetVideoQualityLevelSet(i_s32ChId, sizeof(tVideoQualityLevel)/sizeof(T_ZIMEVideoQualityLevel), tVideoQualityLevel);
//
//
//	return code;

	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetActivity
 (JNIEnv *pEnv, jclass, jobject activity)
{
	s_jActivity = pEnv->NewGlobalRef(activity);
	((ZIMEAndroidQoSNotify*)s_pNetworkNotify)->SetActivity(s_jActivity);
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_DisconnectDevice
  (JNIEnv *, jclass, jint i_s32ChId, jint i_nDeviceType)
{
	jint code = 0;
	if(g_pVideoClient != NULL)
	{
		 code = g_pVideoClient->ZIME_DisconnectDevice(i_s32ChId, (E_ZIMEDeviceType)i_nDeviceType);
	}
	return code;
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_ConnectDevice
  (JNIEnv *, jclass, jint i_s32ChId, jint i_nDeviceType)
{
	jint code = 0;
	if(g_pVideoClient != NULL)
	{
		code = g_pVideoClient->ZIME_ConnectDevice(i_s32ChId, (E_ZIMEDeviceType)i_nDeviceType);
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVQEScene( JNIEnv *, jclass, jint i_eScene)
{
	jint code = 0;
	code = g_pVideoClient->ZIME_SetVQEScene((E_ZIMEVQEScene)i_eScene);
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSendDTMFPayloadType
(JNIEnv *, jclass, jint i_s32ChId, jbyte i_u8PT)
{
	jint code = 0;
	if(g_pVideoClient != NULL)
	{
		code = g_pVideoClient->ZIME_SetSendDTMFPayloadType(i_s32ChId, i_u8PT);
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SendDTMF
(JNIEnv *, jclass, jint i_s32ChId, jint i_s32EvtNum, jboolean i_bOutBand, jint i_s32LenMs, jint i_s32Level)
{
	jint code = 0;
	if(g_pVideoClient != NULL)
	{
		code = g_pVideoClient->ZIME_SendDTMF(i_s32ChId, i_s32EvtNum, i_bOutBand, i_s32LenMs, i_s32Level);
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetDTMFFeedbackStatus
(JNIEnv *, jclass, jboolean i_bEnable, jboolean i_bDirectFeedback)
{
	jint code = 0;
	if(g_pVideoClient != NULL)
	{
		code = g_pVideoClient->ZIME_SetDTMFFeedbackStatus(i_bEnable, i_bDirectFeedback);
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSynchronization
  (JNIEnv *, jclass, jint i_s32ChId, jboolean Enable)
{
	jint code = g_pVideoClient->ZIME_SetSynchronization(i_s32ChId, Enable);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetAudioQosStat
(JNIEnv *env, jclass, jint i_s32ChId, jobject o_uplinkQosStat, jobject o_downlinkQosStat )
{
	T_ZIMEAudioUplinkStat tAudioUplinkQosStat;
	T_ZIMEAudioDownlinkStat tAudioDownlinkQosStat;
	jint code = 0;
	if(g_pVideoClient != NULL)
	{
		{
			code =  g_pVideoClient->ZIME_GetAudioUplinkQosStat( i_s32ChId, &tAudioUplinkQosStat);
			jclass clazz = env->GetObjectClass(o_uplinkQosStat);
			jfieldID iCurBitrate = env->GetFieldID(clazz, "iCurBitrate", "I");
			env->SetIntField(o_uplinkQosStat, iCurBitrate, tAudioUplinkQosStat.tCommonStat.iCurBitrate);
			jfieldID iCurJitter = env->GetFieldID(clazz, "iCurJitter", "I");
			env->SetIntField(o_uplinkQosStat, iCurJitter, tAudioUplinkQosStat.tCommonStat.iCurJitter);
			jfieldID iCurFractionLost = env->GetFieldID(clazz, "iCurFractionLost", "I");
			env->SetIntField(o_uplinkQosStat, iCurFractionLost, tAudioUplinkQosStat.tCommonStat.iCurFractionLost);
			jfieldID iTotalFractionLost = env->GetFieldID(clazz, "iTotalFractionLost", "I");
			env->SetIntField(o_uplinkQosStat, iTotalFractionLost, tAudioUplinkQosStat.tCommonStat.iTotalFractionLost);
			jfieldID iCurRTT = env->GetFieldID(clazz, "iCurRTT", "I");
			env->SetIntField(o_uplinkQosStat, iCurRTT, tAudioUplinkQosStat.tCommonStat.iCurRTT);
			jfieldID iAvgBitrate = env->GetFieldID(clazz, "iAvgBitrate", "I");
			env->SetIntField(o_uplinkQosStat, iAvgBitrate, tAudioUplinkQosStat.tCommonStat.iAvgBitrate);
		}
		{
			code =  g_pVideoClient->ZIME_GetAudioDownlinkQosStat( i_s32ChId, &tAudioDownlinkQosStat);
			jclass clazz = env->GetObjectClass(o_downlinkQosStat);
			jfieldID iCurBitrate = env->GetFieldID(clazz, "iCurBitrate", "I");
			env->SetIntField(o_downlinkQosStat, iCurBitrate, tAudioDownlinkQosStat.tCommonStat.iCurBitrate);
			jfieldID iCurJitter = env->GetFieldID(clazz, "iCurJitter", "I");
			env->SetIntField(o_downlinkQosStat, iCurJitter, tAudioDownlinkQosStat.tCommonStat.iCurJitter);
			jfieldID iCurFractionLost = env->GetFieldID(clazz, "iCurFractionLost", "I");
			env->SetIntField(o_downlinkQosStat, iCurFractionLost, tAudioDownlinkQosStat.tCommonStat.iCurFractionLost);
			jfieldID iTotalFractionLost = env->GetFieldID(clazz, "iTotalFractionLost", "I");
			env->SetIntField(o_downlinkQosStat, iTotalFractionLost, tAudioDownlinkQosStat.tCommonStat.iTotalFractionLost);
			jfieldID iCurRTT = env->GetFieldID(clazz, "iCurRTT", "I");
			env->SetIntField(o_downlinkQosStat, iCurRTT, tAudioDownlinkQosStat.tCommonStat.iCurRTT);
			jfieldID iAvgBitrate = env->GetFieldID(clazz, "iAvgBitrate", "I");
		    env->SetIntField(o_downlinkQosStat, iAvgBitrate, tAudioDownlinkQosStat.tCommonStat.iAvgBitrate);
		}
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetVideoQosStat
(JNIEnv *env, jclass, jint i_s32ChId, jobject o_uplinkQosStat, jobject o_downlinkQosStat )
{
	T_ZIMEVideoUplinkStat tVideoUplinkQosStat;
	T_ZIMEVideoDownlinkStat tVideoDownlinkQosStat;
	jint code = 0;
	if(g_pVideoClient != NULL)
	{
		{
			code =  g_pVideoClient->ZIME_GetVideoUplinkQosStat( i_s32ChId, &tVideoUplinkQosStat);
			jclass clazz = env->GetObjectClass(o_uplinkQosStat);
			jfieldID iCurBitrate = env->GetFieldID(clazz, "iCurBitrate", "I");
			env->SetIntField(o_uplinkQosStat, iCurBitrate, tVideoUplinkQosStat.tCommonStat.iCurBitrate);
			jfieldID iCurJitter = env->GetFieldID(clazz, "iCurJitter", "I");
			env->SetIntField(o_uplinkQosStat, iCurJitter, tVideoUplinkQosStat.tCommonStat.iCurJitter);
			jfieldID iCurFractionLost = env->GetFieldID(clazz, "iCurFractionLost", "I");
			env->SetIntField(o_uplinkQosStat, iCurFractionLost, tVideoUplinkQosStat.tCommonStat.iCurFractionLost);
			jfieldID iTotalFractionLost = env->GetFieldID(clazz, "iTotalFractionLost", "I");
			env->SetIntField(o_uplinkQosStat, iTotalFractionLost, tVideoUplinkQosStat.tCommonStat.iTotalFractionLost);
			jfieldID iCurRTT = env->GetFieldID(clazz, "iCurRTT", "I");
			env->SetIntField(o_uplinkQosStat, iCurRTT, tVideoUplinkQosStat.tCommonStat.iCurRTT);
			jfieldID iRealCapFrameRate = env->GetFieldID(clazz, "iRealCapFrameRate", "I");
			env->SetIntField(o_uplinkQosStat, iRealCapFrameRate, (int)tVideoUplinkQosStat.u32RealCapFrameRate);
			jfieldID iAvgBitrate = env->GetFieldID(clazz, "iAvgBitrate", "I");
			env->SetIntField(o_uplinkQosStat, iAvgBitrate, tVideoUplinkQosStat.tCommonStat.iAvgBitrate);
			jfieldID iExpectedFrameRate = env->GetFieldID(clazz, "iExpectedFrameRate", "I");
			env->SetIntField(o_uplinkQosStat, iExpectedFrameRate, (int)(tVideoUplinkQosStat.u32ExpectedFrameRate));
			jfieldID iRealFrameRate = env->GetFieldID(clazz, "iRealFrameRate", "I");
			env->SetIntField(o_uplinkQosStat, iRealFrameRate, (int)(tVideoUplinkQosStat.u32RealFrameRate));
			jfieldID iWidthID = env->GetFieldID(clazz, "iWidth", "I");
			env->SetIntField(o_uplinkQosStat, iWidthID, (int)(tVideoUplinkQosStat.tResolution.iWidth));
			jfieldID iResSwitchTriggerID = env->GetFieldID(clazz, "iResSwitchTrigger", "I");
			env->SetIntField(o_uplinkQosStat, iResSwitchTriggerID, (int)(tVideoUplinkQosStat.eResSwitchTrigger));

			jfieldID iExpectedESBitRate = env->GetFieldID(clazz, "iExpectedESBitRate", "I");
			env->SetIntField(o_uplinkQosStat, iExpectedESBitRate, tVideoUplinkQosStat.iExpectedESBitRate);

			jfieldID iRealESBitRate_Cur = env->GetFieldID(clazz, "iRealESBitRate_Cur", "I");
			env->SetIntField(o_uplinkQosStat, iRealESBitRate_Cur, tVideoUplinkQosStat.iRealESBitRate_Cur);
	
			jfieldID iRedundantBitRate_Cur = env->GetFieldID(clazz, "iRedundantBitRate_Cur", "I");
			env->SetIntField(o_uplinkQosStat, iRedundantBitRate_Cur, tVideoUplinkQosStat.iRedundantBitRate_Cur);
		}
		{
			code =  g_pVideoClient->ZIME_GetVideoDownlinkQosStat( i_s32ChId, &tVideoDownlinkQosStat);
			jclass clazz = env->GetObjectClass(o_downlinkQosStat);
			jfieldID iCurBitrate = env->GetFieldID(clazz, "iCurBitrate", "I");
			env->SetIntField(o_downlinkQosStat, iCurBitrate, tVideoDownlinkQosStat.tCommonStat.iCurBitrate);
			jfieldID iCurJitter = env->GetFieldID(clazz, "iCurJitter", "I");
			env->SetIntField(o_downlinkQosStat, iCurJitter, tVideoDownlinkQosStat.tCommonStat.iCurJitter);
			jfieldID iCurFractionLost = env->GetFieldID(clazz, "iCurFractionLost", "I");
			env->SetIntField(o_downlinkQosStat, iCurFractionLost, tVideoDownlinkQosStat.tCommonStat.iCurFractionLost);
			jfieldID iTotalFractionLost = env->GetFieldID(clazz, "iTotalFractionLost", "I");
			env->SetIntField(o_downlinkQosStat, iTotalFractionLost, tVideoDownlinkQosStat.tCommonStat.iTotalFractionLost);
			jfieldID iCurRTT = env->GetFieldID(clazz, "iCurRTT", "I");
			env->SetIntField(o_downlinkQosStat, iCurRTT, tVideoDownlinkQosStat.tCommonStat.iCurRTT);
			jfieldID iAvgBitrate = env->GetFieldID(clazz, "iAvgBitrate", "I");
		    env->SetIntField(o_downlinkQosStat, iAvgBitrate, tVideoDownlinkQosStat.tCommonStat.iAvgBitrate);
			jfieldID iRecvFrameRate = env->GetFieldID(clazz, "iRecvFrameRate", "I");
			env->SetIntField(o_downlinkQosStat, iRecvFrameRate, (int)(tVideoDownlinkQosStat.u32RecvFrameRate));
			jfieldID iDisplayFrameRate = env->GetFieldID(clazz, "iDisplayFrameRate", "I");
			env->SetIntField(o_downlinkQosStat, iDisplayFrameRate, (int)(tVideoDownlinkQosStat.u32DisplayFrameRate));
			jfieldID iWidth = env->GetFieldID(clazz, "iWidth", "I");
			env->SetIntField(o_downlinkQosStat, iWidth, tVideoDownlinkQosStat.tResolution.iWidth);
			jfieldID iResSwitchTriggerID = env->GetFieldID(clazz, "iResSwitchTrigger", "I");
			env->SetIntField(o_downlinkQosStat, iResSwitchTriggerID, (int)(tVideoDownlinkQosStat.eResSwitchTrigger));

			jfieldID iESBitRate_Cur = env->GetFieldID(clazz, "iESBitRate_Cur", "I");
			env->SetIntField(o_downlinkQosStat, iESBitRate_Cur, tVideoDownlinkQosStat.iESBitRate_Cur);

			jfieldID iRedundantBitRate_Cur = env->GetFieldID(clazz, "iRedundantBitRate_Cur", "I");
			env->SetIntField(o_downlinkQosStat, iRedundantBitRate_Cur, tVideoDownlinkQosStat.iRedundantBitRate_Cur);

			jfieldID iRealPktLostRate = env->GetFieldID(clazz, "iRealPktLostRate", "I");
			env->SetIntField(o_downlinkQosStat, iRealPktLostRate, (int)tVideoDownlinkQosStat.u32RealPktLostRate);
		}
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetExternalTransport
(JNIEnv *env, jclass, jint i_s32ChId, jint i_s32LocalRTPPort, jobject jPeerAddr, jint i_s32PeerRTPPort, jboolean i_bIsHaveVideo)
{
	jint code = 0;
	if(g_pVideoClient != NULL)
	{
		jclass clazz = env->GetObjectClass(jPeerAddr);
		jfieldID retuStr = env->GetFieldID(clazz, "retuStr", "Ljava/lang/String;");
		jstring jstr = (jstring)env->GetObjectField(jPeerAddr, retuStr);
		const char* peerAddr = env->GetStringUTFChars(jstr, false);

		if(NULL == g_pExtTransport)
		{
			g_pExtTransport = new ZIMEExtTransport(i_s32ChId, NULL, g_pVideoClient);
			g_pExtTransport->AudioSocketInit(i_s32LocalRTPPort,i_s32PeerRTPPort,i_s32PeerRTPPort+1,(char*)peerAddr);
			if (i_bIsHaveVideo)
			{
				g_pExtTransport->VideoSocketInit(i_s32LocalRTPPort+2,i_s32PeerRTPPort+2,i_s32PeerRTPPort+3,(char*)peerAddr);
			}
			
		}
		env->ReleaseStringUTFChars(jstr, peerAddr);

		code = g_pVideoClient->ZIME_SetExternalTransport(i_s32ChId, g_pExtTransport);
	}

	return code;
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartRTPExternalTransport
(JNIEnv *, jclass, jboolean i_bIsStartAudio, jboolean i_bIsStartVideo)
{
	if (i_bIsStartAudio)
	{
		if(NULL == g_pAThd)
		{
			g_pAThd	= new CAudioRecvThread(g_pExtTransport);

		}
		g_pAThd->Start();
	}

	if (i_bIsStartVideo)
	{
		if(NULL == g_pVThd)
		{
			g_pVThd	= new CVideoRecvThread(g_pExtTransport);
		}
		g_pVThd->Start();
	}
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartRTCPExternalTransport
(JNIEnv *, jclass, jboolean i_bIsStartAudio, jboolean i_bIsStartVideo)
{
	if (i_bIsStartAudio)
	{
		if(NULL == g_pARTCPThd)
		{

			g_pARTCPThd	= new CAudioRTCPRecvThread(g_pExtTransport);
		}
		g_pARTCPThd->Start();
	}
	
	if (i_bIsStartVideo)
	{
		if(NULL == g_pVRTCPThd)
		{
			g_pVRTCPThd	= new CVideoRTCPRecvThread(g_pExtTransport);
		}
		g_pVRTCPThd->Start();
	}
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopExternalTransportRecv
	(JNIEnv *, jclass)
{
	if(g_pVideoClient != NULL)
	{

		if(NULL != g_pAThd)
		{
			g_pAThd->Stop();
			delete g_pAThd;
			g_pAThd = NULL;
			ZIMEVIDEOCLIENT_LOGE("g_pAThd->Stop");
		}
		if(NULL != g_pARTCPThd)
		{
			g_pARTCPThd->Stop();
			delete g_pARTCPThd;
			g_pARTCPThd = NULL;
			ZIMEVIDEOCLIENT_LOGE("g_pARTCPThd->Stop");
		}
		
		if(NULL != g_pVThd)
		{
			g_pVThd->Stop();
			delete g_pVThd;
			g_pVThd = NULL;
			ZIMEVIDEOCLIENT_LOGE("g_pVThd->Stop");
		}


		if(NULL != g_pVRTCPThd)
		{
			g_pVRTCPThd->Stop();
			delete g_pVRTCPThd;
			g_pVRTCPThd = NULL;
			ZIMEVIDEOCLIENT_LOGE("g_pVRTCPThd->Stop");
		}	
	}
	return 0;
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopExternalTransportSend
	(JNIEnv *, jclass)
{
		if (NULL != g_pExtTransport)
		{
			g_pExtTransport->VideoSocketUnInit();
			g_pExtTransport->AudioSocketUnInit();
			delete g_pExtTransport;
			g_pExtTransport = NULL;
		}			

	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_ToAudio( JNIEnv *, jclass, jint ChId)
{

	if (g_pExtTransport != NULL)
	{
		if(NULL != g_pVThd)
		{
			g_pVThd->Stop();
			delete g_pVThd;
			g_pVThd = NULL;
			ZIMEVIDEOCLIENT_LOGE("g_pVThd->Stop");
		}

		if(NULL != g_pVRTCPThd)
		{
			g_pVRTCPThd->Stop();
			delete g_pVRTCPThd;
			g_pVRTCPThd = NULL;
			ZIMEVIDEOCLIENT_LOGE("g_pVRTCPThd->Stop");
		}			
		
	}

	jint code = g_pVideoClient->ZIME_StopVideoListen(ChId);
	if(0 != code)
	{
		ZIMEVIDEOCLIENT_LOGE("ZIME_StopVideoListen failed eRet=%d", code);		
	}

	code = g_pVideoClient->ZIME_StopVideoPlayout(ChId);
	if(0 != code)
	{
		ZIMEVIDEOCLIENT_LOGE("ZIME_StopVideoPlayout failed eRet=%d", code);		
	}

	code = g_pVideoClient->ZIME_StopVideoSend(ChId);
	if(0 != code)
	{
		ZIMEVIDEOCLIENT_LOGE("ZIME_StopVideoSend failed eRet=%d", code);		
	}	

	if (g_pExtTransport != NULL)
	{						
		g_pExtTransport->VideoSocketUnInit();	
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_ToAudioAndVideo(JNIEnv *env, jclass, jint ChId, jint i_s32LocalRTPPort, jint i_s32PeerRTPPort, jobject jPeerAddr)
{
	jint code = 0;
	jclass clazz = env->GetObjectClass(jPeerAddr);
	jfieldID retuStr = env->GetFieldID(clazz, "retuStr", "Ljava/lang/String;");
	jstring jstr = (jstring)env->GetObjectField(jPeerAddr, retuStr);
	const char* peerAddr = env->GetStringUTFChars(jstr, false);


	if(g_pVideoClient != NULL)
	{
		if (g_pExtTransport != NULL)
		{
			g_pExtTransport->VideoSocketInit(i_s32LocalRTPPort, i_s32PeerRTPPort, i_s32PeerRTPPort+1, (char*)peerAddr);
		}

		code = g_pVideoClient->ZIME_StartVideoListen(ChId);
		if (code != 0)
		{
			ZIMEVIDEOCLIENT_LOGE("start videolisten failed eRet = %d", code);
		}

		code = g_pVideoClient->ZIME_StartVideoPlayout(ChId);
		if (code != 0)
		{
			g_pVideoClient->ZIME_StopListen(ChId);
			ZIMEVIDEOCLIENT_LOGE("start videoplayout failed eRet = %d", code);
		}
		code = g_pVideoClient->ZIME_StartVideoSend(ChId);
		if (code != 0)
		{
			g_pVideoClient->ZIME_StopListen(ChId);
			g_pVideoClient->ZIME_StopPlayout(ChId);
			ZIMEVIDEOCLIENT_LOGE("start videosend failed eRet = %d", code);
		}

		if (g_pExtTransport != NULL)
		{
			if(NULL == g_pVThd)
			{
				g_pVThd	= new CVideoRecvThread(g_pExtTransport);
			}
			g_pVThd->Start();

			if(NULL == g_pVRTCPThd)
			{
				g_pVRTCPThd	= new CVideoRTCPRecvThread(g_pExtTransport);
			}
			g_pVRTCPThd->Start();
		}

	}
	env->ReleaseStringUTFChars(jstr, peerAddr);
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartAudio(JNIEnv *env, jclass, jint ChId)
{
	jint code = 0;
	code = g_pVideoClient->ZIME_StartAudioListen(ChId);
	if (code != 0)
	{
		ZIMEVIDEOCLIENT_LOGE("ZIME_StartAudioListen failed eRet = %d", code);
		return code;
	}

	code = g_pVideoClient->ZIME_StartAudioPlayout(ChId);
	if (code != 0)
	{
		ZIMEVIDEOCLIENT_LOGE("ZIME_StartAudioPlayout failed eRet = %d", code);
		g_pVideoClient->ZIME_StopListen(ChId);
		return code;
	}
	code = g_pVideoClient->ZIME_StartAudioSend(ChId);
	if (code != 0)
	{
		ZIMEVIDEOCLIENT_LOGE("ZIME_StartAudioSend failed eRet = %d", code);
		g_pVideoClient->ZIME_StopListen(ChId);
		g_pVideoClient->ZIME_StopPlayout(ChId);
		return code;
	}
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoQualityLevelSet
 (JNIEnv *env, jclass, int i_s32ChId, int nNum, jobjectArray jlevels_array)
{

	T_ZIMEVideoQualityLevel tVideoQualityLevel[25] = {0};

	int i = 0;
	for(i = 0; i < nNum; i++)
	{

		jobject jobj = env->GetObjectArrayElement(jlevels_array,  i);
		jclass clazz_SwitchLevel = env->GetObjectClass(jobj);

		jfieldID fid_s32Width = env->GetFieldID(clazz_SwitchLevel, "s32Width", "I");
		jfieldID fid_s32Height = env->GetFieldID(clazz_SwitchLevel, "s32Height", "I");
		jfieldID fid_s32Framerate = env->GetFieldID(clazz_SwitchLevel, "s32Framerate", "I");
		jfieldID fid_s32Bitrate = env->GetFieldID(clazz_SwitchLevel, "s32Bitrate", "I");


		tVideoQualityLevel[i].iWidth = env->GetIntField(jobj, fid_s32Width);
		tVideoQualityLevel[i].iHeight = env->GetIntField(jobj, fid_s32Height);
		tVideoQualityLevel[i].u32Framerate = env->GetIntField(jobj, fid_s32Framerate);
		tVideoQualityLevel[i].u32Bitrate = 1000*env->GetIntField(jobj, fid_s32Bitrate);

		ZIMEVIDEOCLIENT_LOGE("Config %d %d, %d, %d, %d ", i, tVideoQualityLevel[i].iWidth, tVideoQualityLevel[i].iHeight,
				tVideoQualityLevel[i].u32Framerate, tVideoQualityLevel[i].u32Bitrate);
	}
     jint code = 0;
	// todo
	code = g_pVideoClient->ZIME_SetVideoQualityLevelSet(i_s32ChId, nNum, tVideoQualityLevel);
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoEncoderAbility
	(JNIEnv *env, jclass, int i_s32ChId, int i_s32NaluNum, int i_s32EncodeProfile)
{
	T_ZIMEVideoEncoderAbility i_tVideoCodecAbility;
	i_tVideoCodecAbility.s32NaluNumInOnePicture = i_s32NaluNum;

	if(0 == i_s32EncodeProfile)
	{
		i_tVideoCodecAbility.eEncodeProfile = enumZIME_H264_BASE_PROFILE;
	}
	else if(1 == i_s32EncodeProfile)
	{
		i_tVideoCodecAbility.eEncodeProfile = enumZIME_H264_HIGH_PROFILE;
	}
	else
	{
		i_tVideoCodecAbility.eEncodeProfile = enumZIME_CODEC_PROFILE_NUM;
	}

	jint code = 0;
	code = g_pVideoClient->ZIME_SetVideoEncoderAbility(i_s32ChId,i_tVideoCodecAbility);
		if (code != 0)
		{
			ZIMEVIDEOCLIENT_LOGE("ZIME_SetVideoEncoderAbility failed eRet = %d", code);
			return code;
		}
//	s_tVideoCodecAbility = i_tVideoCodecAbility;

	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetLogCallBack
	(JNIEnv *env, jclass)
{
	const char* pFileName = "/sdcard/ZMCELogCallBack.log";
	UInt32 u32MaxLineNum  = 100000;
	UInt32 u32MaxFileNum  = 20;
	LightLogSetup(pFileName, u32MaxLineNum, u32MaxFileNum);

	g_pVideoClient->ZIME_SetLogFun(LogCallbackPtr);
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_LogExit
	(JNIEnv *env, jclass)
{
	LightLogExit();
	return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoModeSet
	(JNIEnv *env, jclass)
{
	jint code = 0;
	E_ZIMEVideoModeType i_aVideoModeTypeSet[2] = {enumZIME_VideoTransCode_YUV,enumZIME_VideoForWard_H26X};
	code = g_pVideoClient->ZIME_SetVideoModeSet(2,i_aVideoModeTypeSet);
	if (code != 0)
	{
		ZIMEVIDEOCLIENT_LOGE("ZIME_SetVideoModeSet failed eRet = %d", code);
		return code;
	}
}


