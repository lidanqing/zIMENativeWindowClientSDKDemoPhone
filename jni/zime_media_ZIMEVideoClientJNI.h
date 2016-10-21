/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class zime_media_ZIMEVideoClientJNI */

#ifndef _Included_zime_media_ZIMEVideoClientJNI
#define _Included_zime_media_ZIMEVideoClientJNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    Create
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_Create
  (JNIEnv *, jclass);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    Destroy
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_Destroy
  (JNIEnv *, jclass);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    Init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_Init
  (JNIEnv *, jclass);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    Terminate
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_Terminate
  (JNIEnv *, jclass);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    Exit
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_Exit
  (JNIEnv *, jclass);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetLogLevel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetLogLevel
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    CreateChannel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_CreateChannel
  (JNIEnv *, jclass, jboolean UsingEncodeCAllback, jint CodecType);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    DeleteChannel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_DeleteChannel
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    MaxNumOfChannels
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_MaxNumOfChannels
  (JNIEnv *, jclass);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetLocalReceiver
 * Signature: (IIIIILzime/media/ZIMEVideoClientJNI/StrPtr;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetLocalReceiver
  (JNIEnv *, jclass, jint, jint, jint, jint, jint, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetLocalReceiver
 * Signature: (ILzime/media/ZIMEVideoClientJNI/IntPtr;Lzime/media/ZIMEVideoClientJNI/IntPtr;Lzime/media/ZIMEVideoClientJNI/IntPtr;Lzime/media/ZIMEVideoClientJNI/IntPtr;Lzime/media/ZIMEVideoClientJNI/StrPtr;I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetLocalReceiver
  (JNIEnv *, jclass, jint, jobject, jobject, jobject, jobject, jobject, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetSendDestination
 * Signature: (IIIIILzime/media/ZIMEVideoClientJNI/StrPtr;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSendDestination
  (JNIEnv *, jclass, jint, jint, jint, jint, jint, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetSendDestination
 * Signature: (ILzime/media/ZIMEVideoClientJNI/StrPtr;ILzime/media/ZIMEVideoClientJNI/IntPtr;Lzime/media/ZIMEVideoClientJNI/IntPtr;Lzime/media/ZIMEVideoClientJNI/IntPtr;Lzime/media/ZIMEVideoClientJNI/IntPtr;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetSendDestination
  (JNIEnv *, jclass, jint, jobject, jint, jobject, jobject, jobject, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    StartListen
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartListen
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    StopListen
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopListen
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    StartPlayout
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartPlayout
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    StopPlayout
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopPlayout
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    StartSend
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartSend
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    StopSend
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopSend
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetNumOfAudioCodecs
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetNumOfAudioCodecs
  (JNIEnv *, jclass);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetNumOfVideoCodecs
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetNumOfVideoCodecs
  (JNIEnv *, jclass);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetAudioCodec
 * Signature: (ILzime/media/ZIMEVideoClientJNI/ZIMEAudioCodeInfo;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetAudioCodec
  (JNIEnv *, jclass, jint, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetVideoCodec
 * Signature: (ILzime/media/ZIMEVideoClientJNI/ZIMEVideoCodeInfo;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetVideoCodec
  (JNIEnv *, jclass, jint, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetSendCodec
 * Signature: (ILzime/media/ZIMEVideoClientJNI/ZIMEAudioCodeInfo;Lzime/media/ZIMEVideoClientJNI/ZIMEVideoCodeInfo;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSendCodec
  (JNIEnv *, jclass, jint, jobject, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetSendCodec
 * Signature: (ILzime/media/ZIMEVideoClientJNI/ZIMEAudioCodeInfo;Lzime/media/ZIMEVideoClientJNI/ZIMEVideoCodeInfo;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetSendCodec
  (JNIEnv *, jclass, jint, jobject, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetRecPayloadType
 * Signature: (ILzime/media/ZIMEVideoClientJNI/ZIMEAudioCodeInfo;Lzime/media/ZIMEVideoClientJNI/ZIMEVideoCodeInfo;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetRecPayloadType
  (JNIEnv *, jclass, jint, jobject, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetRecCodec
 * Signature: (ILzime/media/ZIMEVideoClientJNI/ZIMEAudioCodeInfo;Lzime/media/ZIMEVideoClientJNI/ZIMEVideoCodeInfo;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetRecCodec
  (JNIEnv *, jclass, jint, jobject, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetSourceFilter
 * Signature: (IIILzime/media/ZIMEVideoClientJNI/StrPtr;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSourceFilter
  (JNIEnv *, jclass, jint, jint, jint, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetSendSSRC
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSendSSRC
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetSendSSRC
 * Signature: (ILzime/media/ZIMEVideoClientJNI/IntPtr;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetSendSSRC
  (JNIEnv *, jclass, jint, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetAGCStatus
 * Signature: (ZI)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetAGCStatus
  (JNIEnv *, jclass, jboolean, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetAGCStatus
 * Signature: (Lzime/media/ZIMEVideoClientJNI/BoolPtr;Lzime/media/ZIMEVideoClientJNI/IntPtr;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetAGCStatus
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetECStatus
 * Signature: (Z)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetECStatus
  (JNIEnv *, jclass, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetECStatus
 * Signature: (Lzime/media/ZIMEVideoClientJNI/BoolPtr;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetECStatus
  (JNIEnv *, jclass, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetNSStatus
 * Signature: (Z)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetNSStatus
  (JNIEnv *, jclass, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    GetNSStatus
 * Signature: (Lzime/media/ZIMEVideoClientJNI/BoolPtr;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetNSStatus
  (JNIEnv *, jclass, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetSpeakerMode
 * Signature: (Z)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSpeakerMode
  (JNIEnv *, jclass, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetSampleRate
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSampleRate
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetFECStatus
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetFECStatus
  (JNIEnv *, jclass, jint, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetFECStatus
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetNACKStatus
  (JNIEnv *, jclass, jint, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetIframeInterval
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetIframeInterval
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SendOneIFrame
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_ConfigVideoIntraFrameRefresh
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetCCE
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetCCE
  (JNIEnv *, jclass, jint, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetDEE
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetDEE
  (JNIEnv *, jclass, jint, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetDeBlock
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetDeBlock
  (JNIEnv *, jclass, jint, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetBLE
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetBLE
  (JNIEnv *, jclass, jint, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetDNO
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetDNO
  (JNIEnv *, jclass, jint, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetVideoDisplayWnd
 * Signature: (Ljava/lang/Object;Ljava/lang/Object;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoDisplayWnd
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetCallBack
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetCallBack
  (JNIEnv *, jclass);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetVideoCallBack
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoCallBack
  (JNIEnv *, jclass);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetVideoEncodeFun
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoEncodeFun(JNIEnv *env, jclass, jint i_s32ChId);


/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetVideoDecodeFun
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoDecodeFun(JNIEnv *env, jclass, jint i_s32ChId);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetCodecType
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVendorType
  (JNIEnv *, jclass, jint, jint);


/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetModeSubset
 * Signature:
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetModeSubset
  (JNIEnv *, jclass, jint, jbyteArray, jint, jboolean);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetLogPath
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetLogPath
  (JNIEnv *, jclass, jstring);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetVideoDevCapSize
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoDevCapSize
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetVideoDevices
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoDevices
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetInputMute
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetInputMute
  (JNIEnv *, jclass, jint, jboolean);


/*
 * Class:     zime_video_ZIMEVideoClientJNI
 * Method:    StartCapFileAsCamera
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartCapFileAsCamera
  (JNIEnv *, jclass, jint, jstring);

/*
 * Class:     zime_video_ZIMEVideoClientJNI
 * Method:    StopCapFileAsCamera
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopCapFileAsCamera
  (JNIEnv *, jclass, jint);

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetNetworkQualityNotify
 (JNIEnv *, jclass, jint);

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetActivity
 (JNIEnv *, jclass, jobject);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetCapaBilityType
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetCapaBilitySet
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    DisconnectDevice
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_DisconnectDevice
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    ConnectDevice
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_ConnectDevice
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetVQEScene
 * Signature: (Z)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVQEScene
  (JNIEnv *, jclass, jint);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetSendDTMFPayloadType
 * Signature: (I/B)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSendDTMFPayloadType
(JNIEnv *, jclass, jint i_s32ChId, jbyte i_u8PT);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SendDTMF
 * Signature: (I/B/I/I)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SendDTMF
(JNIEnv *, jclass, jint i_s32ChId, jint i_s32EvtNum, jboolean i_bOutBand,
		jint i_s32LenMs = 160, jint i_s32Level = 10);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetDTMFFeedbackStatus
 * Signature: (B/B)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetDTMFFeedbackStatus
(JNIEnv *, jclass, jboolean i_bEnable, jboolean i_bDirectFeedback = false);


JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetAudioQosStat
(JNIEnv *, jclass, jint i_s32ChId, jobject, jobject);

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_GetVideoQosStat
(JNIEnv *, jclass, jint i_s32ChId, jobject, jobject);

// �ⲿ�շ���صĽӿ� ǰ�����ӿ�Ϊstartʱ����
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetExternalTransport
(JNIEnv *env, jclass, jint i_s32ChId, jint i_s32LocalRTPPort, jobject jPeerAddr, jint i_s32PeerRTPPort, jboolean i_bIsHaveVideo);
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartRTPExternalTransport
(JNIEnv *, jclass, jboolean i_bIsStartAudio, jboolean i_bIsStartVideo);
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartRTCPExternalTransport
(JNIEnv *, jclass, jboolean i_bIsStartAudio, jboolean i_bIsStartVideo);

// ����˳�ͨ������
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopExternalTransportRecv
	(JNIEnv *, jclass);
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StopExternalTransportSend
	(JNIEnv *, jclass);

// ����Ƶ��ת�ӿ�
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_ToAudio( JNIEnv *, jclass, jint ChId);
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_ToAudioAndVideo(JNIEnv *env, jclass, jint ChId,jint i_s32LocalRTPPort, jint i_s32PeerRTPPort, jobject jPeerAddr);
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_StartAudio(JNIEnv *env, jclass, jint ChId);

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoQualityLevelSet
 (JNIEnv *pEnv, jclass, int i_s32ChId, int nNum, jobjectArray jlevels_array);

/*
 * Class:     zime_media_ZIMEVideoClientJNI
 * Method:    SetSynchronization
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetSynchronization
  (JNIEnv *, jclass, jint, jboolean);

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoEncoderAbility
 (JNIEnv *env, jclass, int i_s32ChId, int i_s32NaluNum, int i_s32EncodeProfile);

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetLogCallBack
	(JNIEnv *env, jclass);

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_LogExit
	(JNIEnv *env, jclass);

JNIEXPORT jint JNICALL Java_zime_media_ZIMEVideoClientJNI_SetVideoModeSet
	(JNIEnv *env, jclass);

#ifdef __cplusplus
}
#endif
#endif