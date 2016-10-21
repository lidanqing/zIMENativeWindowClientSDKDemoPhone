#include <string.h>
#include <jni.h>

#include <stdlib.h>
#include <unistd.h>

#include "ZIMEClient.h"
#include "ZIMEType.h"
#include "zime_audio_ZIMEClientJni.h"
#include "ZIMEAudioDevCallBackInterface.h"
#include "ZIMECallBackCommon.h"
static ZIMEClient* g_pAudioClient = NULL;
// 和外部声卡相关的部分
static JNIEnv *s_env = NULL;
static jobject s_audioCallBack;
static T_ZIMEAudioCallBack s_tAudioCallBack;
static JavaVM *s_javavm = NULL; 


#include <android/log.h>
#define SMNDK_LOGI(...) __android_log_print(ANDROID_LOG_INFO, "ZIMEClientJni", __VA_ARGS__)
#define SMNDK_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "ZIMEClientJni", __VA_ARGS__)

void ZIME_JNI_CreateJavVM(JNIEnv *i_env)
{
	i_env->GetJavaVM(&s_javavm);
	if (NULL == s_javavm)
	{
		SMNDK_LOGE("Get javavm failed");
	}
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_Create(JNIEnv * env, jclass clazc)
{
	SMNDK_LOGE("CreateClient Engine");
	
	g_pAudioClient = ZIMEClient::Create();
	if (g_pAudioClient == NULL)
	{
		return -1;
	}
	ZIME_JNI_CreateJavVM(env);
	int nRet = ZIME_CallBack_Init(s_javavm, JNI_VERSION_1_6);	
	return nRet;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_Destroy( JNIEnv *, jclass )
{
	jint code = ZIMEClient::Destroy(g_pAudioClient);
	g_pAudioClient = NULL;
  ZIME_CallBack_UnInit();  	
	return code;
}
JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_Exit( JNIEnv *, jclass )
{
	if (s_env != NULL)
	{
		s_env->DeleteGlobalRef(s_audioCallBack);
	}
	
	jint code = ZIMEClient::Exit();
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_Init( JNIEnv *, jclass )
{
	jint code = g_pAudioClient->ZIME_Init();
	 return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_Terminate( JNIEnv *, jclass )
{
	jint code = 0;
	if (g_pAudioClient != NULL)
	{
		code = g_pAudioClient->ZIME_Terminate();
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetLogLevel
  (JNIEnv *, jclass, jint iLogLevel)
{
	jint code = 0;
	if (g_pAudioClient != NULL)
	{
		code = g_pAudioClient->ZIME_SetLogLevel(iLogLevel);
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_GetAudioQosStat
(JNIEnv *env, jclass, jint i_s32ChId, jobject o_uplinkQosStat, jobject o_downlinkQosStat )
{
	T_ZIMEAudioUplinkStat tAudioUplinkQosStat;
	T_ZIMEAudioDownlinkStat tAudioDownlinkQosStat;
	jint code = 0;
	if(g_pAudioClient != NULL)
	{
		{
			SMNDK_LOGE("ZIME_GetAudioUplinkQosStat");
			code =  g_pAudioClient->ZIME_GetAudioUplinkQosStat( i_s32ChId, &tAudioUplinkQosStat);
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
			SMNDK_LOGE("ZIME_GetAudioDownlinkQosStat");
			code =  g_pAudioClient->ZIME_GetAudioDownlinkQosStat( i_s32ChId, &tAudioDownlinkQosStat);
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

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_CreateChannel( JNIEnv *, jclass )
{
	jint code = g_pAudioClient->ZIME_CreateChannel();
	 return code;	 
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_DeleteChannel( JNIEnv *, jclass, jint ChId)
{
	jint code;
	code = g_pAudioClient->ZIME_DeleteChannel(ChId);
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_MaxNumOfChannels( JNIEnv *, jclass )
{
	jint code = g_pAudioClient->ZIME_MaxNumOfChannels();
	  return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetLocalReceiver( JNIEnv *env, jclass, jint ChId, jint VoiceRTPPort, jint VoiceRTCPPort)
{	  	  
	jint code = g_pAudioClient->ZIME_SetLocalReceiver(ChId,
		VoiceRTPPort, VoiceRTCPPort);	
	  return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetSendDestination( JNIEnv *env, jclass, jint ChId, jint VoiceRTPPort, jstring DstAddr, jint  VoiceRTCPPort)
{
	const char* dstAddr = env->GetStringUTFChars(DstAddr, false);
	jint code = g_pAudioClient->ZIME_SetSendDestination(ChId,
		VoiceRTPPort, dstAddr, VoiceRTCPPort);
	env->ReleaseStringUTFChars(DstAddr, dstAddr);
	  return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_StartListen( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pAudioClient->ZIME_StartListen(ChId);
      return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_StopListen( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pAudioClient->ZIME_StopListen(ChId);
      return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_StartPlayout( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pAudioClient->ZIME_StartPlayout(ChId);
      return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_StopPlayout( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pAudioClient->ZIME_StopPlayout(ChId);
      return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_StartSend( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pAudioClient->ZIME_StartSend(ChId);
	 return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_StopSend( JNIEnv *, jclass, jint ChId)
{
	jint code = g_pAudioClient->ZIME_StopSend(ChId);
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_GetNumOfCodecs( JNIEnv *, jclass )
{
	jint code = g_pAudioClient->ZIME_GetNumOfCodecs();
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_GetNumOfVideoCodecs( JNIEnv *, jclass )
{
	
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_GetCodec( JNIEnv *env, jclass, jint CodecIdx, jobject VoiceCodecInfo)
{
	T_ZIMEAudioCodecInfo codecInfo;
    jint code = g_pAudioClient->ZIME_GetCodec(CodecIdx, codecInfo);
	
	jclass clazz = env->GetObjectClass(VoiceCodecInfo);
	
	jfieldID s32PTId = env->GetFieldID(clazz, "s32PT", "I");
    env->SetIntField(VoiceCodecInfo, s32PTId, codecInfo.s32PT);
	
	jfieldID s32SampleRateId = env->GetFieldID(clazz, "s32SampleRate", "I");
    env->SetIntField(VoiceCodecInfo, s32SampleRateId, codecInfo.s32SampleRate);
	
	jfieldID s32PacketSizeId = env->GetFieldID(clazz, "s32PacketSize", "I");
    env->SetIntField(VoiceCodecInfo, s32PacketSizeId, codecInfo.s32PacketSize);
	
	jfieldID s32ChannelNumId = env->GetFieldID(clazz, "s32ChannelNum", "I");
    env->SetIntField(VoiceCodecInfo, s32ChannelNumId, codecInfo.s32ChannelNum);
	
	jfieldID s32BitRateId = env->GetFieldID(clazz, "s32BitRate", "I");
    env->SetIntField(VoiceCodecInfo, s32BitRateId, codecInfo.s32BitRate);
	
	jfieldID aPTNameId = env->GetFieldID(clazz, "aPTName", "Ljava/lang/String;");
	jstring nameTemp = env->NewStringUTF(codecInfo.aPTName);
    env->SetObjectField(VoiceCodecInfo, aPTNameId, (jobject)nameTemp);

    jfieldID eExtnTypeID = env->GetFieldID(clazz, "eExtnType", "I");
    env->SetIntField(VoiceCodecInfo, eExtnTypeID, codecInfo.eExtnType);

	jfieldID codecExInfoID = env->GetFieldID(clazz, "codecExInfo", "I");
	env->SetIntField(VoiceCodecInfo, codecExInfoID, codecInfo.uCodecExInfo.cAMRPackerMode);
	
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetSendCodec( JNIEnv *env, jclass, jint ChId, jobject VoiceCodecInfo)
{
	
	jclass clazzOne = env->GetObjectClass(VoiceCodecInfo);
	T_ZIMEAudioCodecInfo codecInfo;
	
	jfieldID s32PTId = env->GetFieldID(clazzOne, "s32PT", "I"); 
	codecInfo.s32PT = env->GetIntField(VoiceCodecInfo, s32PTId);
	
	jfieldID s32SampleRateId = env->GetFieldID(clazzOne, "s32SampleRate", "I"); 
	codecInfo.s32SampleRate = env->GetIntField(VoiceCodecInfo, s32SampleRateId);
	
	jfieldID s32PacketSizeId = env->GetFieldID(clazzOne, "s32PacketSize", "I"); 
	codecInfo.s32PacketSize = env->GetIntField(VoiceCodecInfo, s32PacketSizeId);
	
	jfieldID s32ChannelNumId = env->GetFieldID(clazzOne, "s32ChannelNum", "I"); 
	codecInfo.s32ChannelNum = env->GetIntField(VoiceCodecInfo, s32ChannelNumId);
	
	jfieldID s32BitRateId = env->GetFieldID(clazzOne, "s32BitRate", "I"); 
	codecInfo.s32BitRate = env->GetIntField(VoiceCodecInfo, s32BitRateId);
	
	jfieldID aPTNameId = env->GetFieldID(clazzOne, "aPTName", "Ljava/lang/String;"); 
	jstring nameTemp = (jstring)env->GetObjectField(VoiceCodecInfo, aPTNameId);
	const char* strName = env->GetStringUTFChars(nameTemp, false);
	 strcpy(codecInfo.aPTName, strName); 

	 jfieldID eAudioExtnTypeId = env->GetFieldID(clazzOne, "eExtnType", "I");
	 int iAudioExtnType = env->GetIntField(VoiceCodecInfo, eAudioExtnTypeId);
	 codecInfo.eExtnType = (E_ZIMERTPExtType)iAudioExtnType;

	jfieldID codecExInfoId = env->GetFieldID(clazzOne, "codecExInfo", "I");
	int icodecExInfo = env->GetIntField(VoiceCodecInfo, codecExInfoId);
	codecInfo.uCodecExInfo.cAMRPackerMode = icodecExInfo;

	jint code = g_pAudioClient->ZIME_SetSendCodec(ChId, codecInfo);
	env->ReleaseStringUTFChars(nameTemp, strName);
     return code;	 
	
     return 0;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetRecPayloadType( JNIEnv *env, jclass, jint ChId, jobject VoiceCodecInfo)
{
	jclass clazzOne = env->GetObjectClass(VoiceCodecInfo); 
	T_ZIMEAudioCodecInfo codecInfo;
	
	jfieldID s32PTId = env->GetFieldID(clazzOne, "s32PT", "I"); 
	codecInfo.s32PT = env->GetIntField(VoiceCodecInfo, s32PTId);
	
	jfieldID s32SampleRateId = env->GetFieldID(clazzOne, "s32SampleRate", "I"); 
	codecInfo.s32SampleRate = env->GetIntField(VoiceCodecInfo, s32SampleRateId);
	
	jfieldID s32PacketSizeId = env->GetFieldID(clazzOne, "s32PacketSize", "I"); 
	codecInfo.s32PacketSize = env->GetIntField(VoiceCodecInfo, s32PacketSizeId);
	
	jfieldID s32ChannelNumId = env->GetFieldID(clazzOne, "s32ChannelNum", "I"); 
	codecInfo.s32ChannelNum = env->GetIntField(VoiceCodecInfo, s32ChannelNumId);
	
	jfieldID s32BitRateId = env->GetFieldID(clazzOne, "s32BitRate", "I"); 
	codecInfo.s32BitRate = env->GetIntField(VoiceCodecInfo, s32BitRateId);
	
	jfieldID aPTNameId = env->GetFieldID(clazzOne, "aPTName", "Ljava/lang/String;"); 
	jstring nameTemp = (jstring)env->GetObjectField(VoiceCodecInfo, aPTNameId);
	const char* strName = env->GetStringUTFChars(nameTemp, false);
	strcpy(codecInfo.aPTName, strName);	 
	
	 jfieldID eAudioExtnTypeId = env->GetFieldID(clazzOne, "eExtnType", "I");
	 int iAudioExtnType = env->GetIntField(VoiceCodecInfo, eAudioExtnTypeId);
	 codecInfo.eExtnType = (E_ZIMERTPExtType)iAudioExtnType;

	jfieldID codecExInfoId = env->GetFieldID(clazzOne, "codecExInfo", "I");
	int icodecExInfo = env->GetIntField(VoiceCodecInfo, codecExInfoId);
	codecInfo.uCodecExInfo.cAMRPackerMode = icodecExInfo;

	jint code = g_pAudioClient->ZIME_SetRecPayloadType(ChId, codecInfo);
	env->ReleaseStringUTFChars(nameTemp, strName);
     return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetAGCStatus( JNIEnv *, jclass, jboolean Enable, jint Mode)
{
	jint code = g_pAudioClient->ZIME_SetAGCStatus(Enable, (E_ZIMEAGCMode)Mode);
      return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetECStatus( JNIEnv *, jclass, jboolean Enable)
{
	jint code = g_pAudioClient->ZIME_SetECStatus(Enable);
      return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetNSStatus( JNIEnv *, jclass, jboolean Enable)
{
	jint code = g_pAudioClient->ZIME_SetNSStatus(Enable);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetAGCStatus( JNIEnv *, jclass, jboolean Enable)
{
	jint code = g_pAudioClient->ZIME_SetAGCStatus(Enable, enumZIME_AGC_SOFTWARE);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetSpeakerMode( JNIEnv *, jclass, jboolean Enable)
{
	jint code = g_pAudioClient->ZIME_SetSpeakerMode(Enable);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_test(JNIEnv *env, jclass)
{
	return 1;
}

E_ZIMEResultCode ZIME_CallBack_ProducerStart()
{
	jclass AudioDeviceClass = s_env->GetObjectClass(s_audioCallBack); 
	jmethodID MethodID = s_env->GetMethodID(AudioDeviceClass, "ProducerStart", "()I");	
	if (MethodID == NULL)
	{
		return enumZIME_FAIL;
	}
	int iRet = s_env->CallIntMethod(s_audioCallBack, MethodID);
	if (iRet != 0)
	{
		return enumZIME_FAIL;
	}	
	return enumZIME_OK;
}

E_ZIMEResultCode ZIME_CallBack_ProducerStop()
{
	jclass AudioDeviceClass = s_env->GetObjectClass(s_audioCallBack); 
	jmethodID MethodID = s_env->GetMethodID(AudioDeviceClass, "ProducerStop", "()I");	
	if (MethodID == NULL)
	{
		return enumZIME_FAIL;
	}	
	int iRet = s_env->CallIntMethod(s_audioCallBack, MethodID);
	if (iRet != 0)
	{
		return enumZIME_FAIL;
	}
	return enumZIME_OK;
}

E_ZIMEResultCode ZIME_CallBack_ConsumerStart()
{
	
	jclass AudioDeviceClass = s_env->GetObjectClass(s_audioCallBack); 
	jmethodID MethodID = s_env->GetMethodID(AudioDeviceClass, "ConsumerStart", "()I");	
	if (MethodID == NULL)
	{
		return enumZIME_FAIL;
	}	
	int iRet = s_env->CallIntMethod(s_audioCallBack, MethodID);
	if (iRet != 0)
	{
		return enumZIME_FAIL;
	}
	return enumZIME_OK;
}

E_ZIMEResultCode ZIME_CallBack_ConsumerStop()
{
	jclass AudioDeviceClass = s_env->GetObjectClass(s_audioCallBack); 
	jmethodID MethodID = s_env->GetMethodID(AudioDeviceClass, "ConsumerStop", "()I");	
	if (MethodID == NULL)
	{
	return enumZIME_FAIL;
	}	
	int iRet = s_env->CallIntMethod(s_audioCallBack, MethodID);
	if (iRet != 0)
	{
		return enumZIME_FAIL;
	}	
	return enumZIME_OK;
}

static jbyteArray s_arrayFrameBuf = NULL;
static jmethodID  s_WriteFrameMethodID = NULL;
static JNIEnv *s_WriteEnv = NULL;
E_ZIMEResultCode ZIME_CallBack_WriteFrame(const unsigned char* i_pFrameBuf, unsigned int i_iBufLen)
{
	
	// 先获取该线程的ENV
	if (NULL == s_WriteEnv)
	{
		jint iResult = s_javavm->AttachCurrentThread(&s_WriteEnv, NULL);
		if (iResult < 0)
		{
			SMNDK_LOGE("get writeframe env err!");
		}
	}	

	if (s_arrayFrameBuf == NULL)
	{
		SMNDK_LOGI("begin::::ZIME_CallBack_WriteFrame----is null");
		jbyteArray arrayTmp = s_WriteEnv->NewByteArray(i_iBufLen);
		s_arrayFrameBuf = (jbyteArray)s_WriteEnv->NewGlobalRef(arrayTmp);
		s_WriteEnv->DeleteLocalRef(arrayTmp);	
	}	
	
	s_WriteEnv->SetByteArrayRegion(s_arrayFrameBuf, 0, (jsize)i_iBufLen, (jbyte*)i_pFrameBuf);	
	

	if (s_WriteFrameMethodID == NULL)
	{
		jclass AudioDeviceClass = s_WriteEnv->GetObjectClass(s_audioCallBack);
		s_WriteFrameMethodID = s_WriteEnv->GetMethodID(AudioDeviceClass, "WriteFrame", "([BI)I");
		if (s_WriteFrameMethodID == NULL)
		{
			SMNDK_LOGE("ZIME_CallBack_WriteFrame----NULL");
			return enumZIME_FAIL;
		}
	}
	
	int iRet = s_WriteEnv->CallIntMethod(s_audioCallBack, s_WriteFrameMethodID, s_arrayFrameBuf, i_iBufLen);
	if (iRet != 0)
	{
		return enumZIME_FAIL;
	}

// 	if (NULL != s_WriteEnv)
// 	{
// 		s_javavm->DetachCurrentThread();
// 		s_WriteEnv = NULL;
// 	}

	return enumZIME_OK;
}

void ZIME_CallBack_WriteFrameDetachEnv()
{
	if (NULL != s_WriteEnv)
	{
		s_javavm->DetachCurrentThread();
		s_WriteEnv = NULL;
	}
}

static jbyteArray s_arrayReadFrameBuf = NULL;
static jmethodID  s_ReadFrameMethodID = NULL;
static JNIEnv *s_ReadEnv = NULL;
E_ZIMEResultCode ZIME_CallBack_GetFrame(const unsigned char* io_pReadBuf, unsigned int *io_u32BufLen)
{

	// 先获取该线程的ENV
	if (NULL == s_ReadEnv)
	{
		jint iResult = s_javavm->AttachCurrentThread(&s_ReadEnv, NULL);
		if (iResult < 0)
		{
			SMNDK_LOGE("get writeframe env err!");
		}
	}	
	
	if (s_arrayReadFrameBuf == NULL)
	{
		SMNDK_LOGI("begin::::ZIME_CallBack_ReadFrame----is null");
		jbyteArray arrayTmp = s_ReadEnv->NewByteArray((jsize)*io_u32BufLen);
		s_arrayReadFrameBuf = (jbyteArray)s_ReadEnv->NewGlobalRef(arrayTmp);
		s_ReadEnv->DeleteLocalRef(arrayTmp);	
	}
	
	if (s_ReadFrameMethodID == NULL)
	{
		jclass AudioDeviceClass = s_ReadEnv->GetObjectClass(s_audioCallBack);
		s_ReadFrameMethodID = s_ReadEnv->GetMethodID(AudioDeviceClass, "ReadFrame", "([BI)I");
		if (s_ReadFrameMethodID == NULL)
		{
			SMNDK_LOGE("ZIME_CallBack_ReadFrame----NULL");
			return enumZIME_FAIL;
		}
	}

	int iReadLen = s_ReadEnv->CallIntMethod(s_audioCallBack, s_ReadFrameMethodID, s_arrayReadFrameBuf, *io_u32BufLen);
	if (iReadLen == 0)
	{
		SMNDK_LOGE("ReadFrame len is zero!!!!");
		return enumZIME_FAIL;
	}
	*io_u32BufLen = iReadLen;
	s_ReadEnv->GetByteArrayRegion(s_arrayReadFrameBuf, 0, iReadLen, (jbyte*)io_pReadBuf);

	if (NULL != s_ReadEnv)
	{
		s_javavm->DetachCurrentThread();
		s_ReadEnv = NULL;
	}
	return enumZIME_OK;
}

void ZIME_CallBack_GetFrameDetachEnv()
{
	if (NULL != s_ReadEnv)
	{
		s_javavm->DetachCurrentThread();
		s_ReadEnv = NULL;
	}
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetAudioCallBack(JNIEnv *env, jclass, jobject audioCallBack)
{	

	s_tAudioCallBack.ProducerOpen  = ZIME_AudioCallBack_ProducerOpen;	
	s_tAudioCallBack.ProducerStart = ZIME_AudioCallBack_ProducerStart;
	s_tAudioCallBack.GetFrame	   = ZIME_AudioCallBack_GetFrame;
	s_tAudioCallBack.ProducerStop  = ZIME_AudioCallBack_ProducerStop;
	s_tAudioCallBack.ProducerClose = ZIME_AudioCallBack_ProducerClose;

	s_tAudioCallBack.ConsumerOpen  =  ZIME_AudioCallBack_ConsumerOpen;
	s_tAudioCallBack.ConsumerStart = ZIME_AudioCallBack_ConsumerStart;
	s_tAudioCallBack.WriteFrame    = ZIME_AudioCallBack_WriteFrame;
	s_tAudioCallBack.ConsumerStop  = ZIME_AudioCallBack_ConsumerStop;
	s_tAudioCallBack.ConsumerClose = ZIME_AudioCallBack_ConsumerClose;

	jint code = g_pAudioClient->ZIME_SetAudioCallBack(&s_tAudioCallBack);
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetSampleRate
  (JNIEnv *env, jclass, jint i_sample)
{
	jint code = g_pAudioClient->ZIME_SetSampleRate(i_sample);
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetInputMute( JNIEnv *env, jclass classTmp, jint iChannel, jboolean Enable)
{
	jint code = g_pAudioClient->ZIME_SetInputMute(iChannel, Enable);
    return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_DisconnectDevice
  (JNIEnv *, jclass, jint i_s32ChId, jint i_nDeviceType)
{
	jint code = 0;
	if(g_pAudioClient != NULL)
	{
		 code = g_pAudioClient->ZIME_DisconnectDevice(i_s32ChId, (E_ZIMEDeviceType)i_nDeviceType);
	}
	return code;
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_ConnectDevice
  (JNIEnv *, jclass, jint i_s32ChId, jint i_nDeviceType)
{
	jint code = 0;
	if(g_pAudioClient != NULL)
	{
		code = g_pAudioClient->ZIME_ConnectDevice(i_s32ChId, (E_ZIMEDeviceType)i_nDeviceType);
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetSendDTMFPayloadType
(JNIEnv *, jclass, jint i_s32ChId, jbyte i_u8PT)
{
	jint code = 0;
	if(g_pAudioClient != NULL)
	{
		code = g_pAudioClient->ZIME_SetSendDTMFPayloadType(i_s32ChId, i_u8PT);
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SendDTMF
(JNIEnv *, jclass, jint i_s32ChId, jint i_s32EvtNum, jboolean i_bOutBand, jint i_s32LenMs, jint i_s32Level)
{
	jint code = 0;
	if(g_pAudioClient != NULL)
	{
		code = g_pAudioClient->ZIME_SendDTMF(i_s32ChId, i_s32EvtNum, i_bOutBand, i_s32LenMs, i_s32Level);
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetDTMFFeedbackStatus
(JNIEnv *, jclass, jboolean i_bEnable, jboolean i_bDirectFeedback)
{
	jint code = 0;
	if(g_pAudioClient != NULL)
	{
		code = g_pAudioClient->ZIME_SetDTMFFeedbackStatus(i_bEnable, i_bDirectFeedback);
	}
	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetVQEScene( JNIEnv *, jclass, jint i_eScene)
{
	jint code = 0;
	code = g_pAudioClient->ZIME_SetVQEScene((E_ZIMEVQEScene)i_eScene);
	return code;
}


JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetModeSubset(JNIEnv *env, jclass, jint i_s32ChId, jbyteArray i_aSubset, jint i_s32Size, jboolean i_b8UseMin)
{
	unsigned char aSubSet[20];
	jbyte aTmp[20];
	env->GetByteArrayRegion(i_aSubset, 0, i_s32Size,  aTmp);
	memcpy(aSubSet, aTmp, 20);
	jint code = 0;
	code = g_pAudioClient->ZIME_SetModeSubset(i_s32ChId, aSubSet, i_s32Size, i_b8UseMin);

	return code;
}

JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_SetSourceFilter(JNIEnv *env, jclass, jint ChId, jint VoiceRTPPort, jstring DstAddr)
{
	jint code = 0;
	const char* dstAddr = env->GetStringUTFChars(DstAddr, false);
	code = g_pAudioClient->ZIME_SetSourceFilter(ChId, VoiceRTPPort, dstAddr);
	env->ReleaseStringUTFChars(DstAddr, dstAddr);
	return code;
}

/*JNIEXPORT jint JNICALL Java_zime_media_ZIMEClientJni_GetAudioQosStat
(JNIEnv *env, jclass, jint i_s32ChId, jobject o_upstreamQosStat, jobject o_downstreamQosStat )
{
	T_ZIMEAudioQosStat tAudioQosStat;
		jint code = 0;
		if(g_pAudioClient != NULL)
		{
			code =  g_pAudioClient->ZIME_GetAudioQosStat( i_s32ChId, &tAudioQosStat);

			jclass clazz = env->GetObjectClass(o_upstreamQosStat);
			jfieldID iBitrate = env->GetFieldID(clazz, "iBitrate", "I");
			env->SetIntField(o_upstreamQosStat, iBitrate, tAudioQosStat.tUpstreamStat.tCommonStat.iBitrate);
			jfieldID iJitter = env->GetFieldID(clazz, "iJitter", "I");
			env->SetIntField(o_upstreamQosStat, iJitter, tAudioQosStat.tUpstreamStat.tCommonStat.iJitter);
			jfieldID iFractionLost = env->GetFieldID(clazz, "iFractionLost", "I");
			env->SetIntField(o_upstreamQosStat, iFractionLost, tAudioQosStat.tUpstreamStat.tCommonStat.iFractionLost);
			jfieldID iTotalFractionLost = env->GetFieldID(clazz, "iTotalFractionLost", "I");
			env->SetIntField(o_upstreamQosStat, iTotalFractionLost, tAudioQosStat.tUpstreamStat.tCommonStat.iTotalFractionLost);

			jclass downclazz = env->GetObjectClass(o_downstreamQosStat);
			jfieldID idownBitrate = env->GetFieldID(downclazz, "iBitrate", "I");
			env->SetIntField(o_downstreamQosStat, iBitrate, tAudioQosStat.tDownstreamStat.tCommonStat.iBitrate);
			jfieldID idownJitter = env->GetFieldID(downclazz, "iJitter", "I");
			env->SetIntField(o_downstreamQosStat, iJitter, tAudioQosStat.tDownstreamStat.tCommonStat.iJitter);
			jfieldID idownFractionLost = env->GetFieldID(downclazz, "iFractionLost", "I");
			env->SetIntField(o_downstreamQosStat, iFractionLost, tAudioQosStat.tDownstreamStat.tCommonStat.iFractionLost);
			jfieldID idownTotalFractionLost = env->GetFieldID(downclazz, "iTotalFractionLost", "I");
			env->SetIntField(o_downstreamQosStat, iTotalFractionLost, tAudioQosStat.tDownstreamStat.tCommonStat.iTotalFractionLost);
		}
		return code;
}*/

