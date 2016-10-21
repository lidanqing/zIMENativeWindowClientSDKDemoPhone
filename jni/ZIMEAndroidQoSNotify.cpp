#include "ZIMEAndroidQoSNotify.h"
#include <android/log.h>
#include <stdio.h>
#include <string.h>

#define ZIMENOTIFY_LOGI(...) __android_log_print(ANDROID_LOG_INFO, "ZIMENOTIFYJNI", __VA_ARGS__)
#define ZIMENOTIFY_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "ZIMENOTIFYJNI", __VA_ARGS__)

void ZIMEAndroidQoSNotify::NotifyDownLinkQOS(int i_s32ChId, T_ZIMEVideoDownLinkQOS& i_tNetworkQualityInfo)
{
// 	ZIMENOTIFY_LOGE("network notify callback, pkt lost:[%d], rtt:[%d], dec err ratio:[%d], stat:[%d]",
// 		i_tNetworkQualityInfo.s32LostRate, 
// 		i_tNetworkQualityInfo.s32RoundTripTime, 
// 		i_tNetworkQualityInfo.s32MacroDecodeErr,
// 		i_tNetworkQualityInfo.ePlayStat);
	return;
}

void ZIMEAndroidQoSNotify::NotifyUpLinkQOS(int i_s32ChId, T_ZIMEVideoUpLinkQOS& i_tNetworkQualityInfo)
{
	ZIMENOTIFY_LOGE("network notify callback, pkt lost:[%d], jitter:[%d], rtt:[%d]",
		i_tNetworkQualityInfo.s32LostRate,
		i_tNetworkQualityInfo.s32Jitter,
		i_tNetworkQualityInfo.s32RoundTripTime);
	return;
}

void ZIMEAndroidQoSNotify::ResolutionSwitchNotify(int i_s32ChId,T_ZIMEResolutionSwitchInfo& i_tResolutionSwitchInfo)
{
	ZIMENOTIFY_LOGE("eSwitchView = %d, NewRes = %d * %d, OldRes = %d * %d, ResSwitchTrigger = %d",
			i_tResolutionSwitchInfo.eSwitchView,
			i_tResolutionSwitchInfo.tNewRes.iWidth, i_tResolutionSwitchInfo.tNewRes.iHeight,
			i_tResolutionSwitchInfo.tOldRes.iWidth, i_tResolutionSwitchInfo.tOldRes.iHeight,
			i_tResolutionSwitchInfo.eResSwitchTrigger);


	if (m_Activity != NULL)
	{
		JNIEnv *pEnv;
		bool bDoDetach;
		int s32Ret = JNIGetEnv(pEnv, bDoDetach);
		if (s32Ret < 0)
		{
			return;
		}
		else
		{
			jstring toastText;
			char* pToastText = new char[1024];
			memset(pToastText, 0, 1024);

			sprintf(pToastText, "eSwitchView=%s, NewRes=%d*%d  OldRes=%d*%d, eResSwitchTrigger=%s",
					i_tResolutionSwitchInfo.eSwitchView == 0 ? "LocalView" : "RemoteView",
					i_tResolutionSwitchInfo.tNewRes.iWidth, i_tResolutionSwitchInfo.tNewRes.iHeight,
					i_tResolutionSwitchInfo.tOldRes.iWidth, i_tResolutionSwitchInfo.tOldRes.iHeight,
					GetSwitchInfo(i_tResolutionSwitchInfo.eResSwitchTrigger));
			
			int nVResWidth_S = 0;
			int nVResWidth_R = 0;
			if(i_tResolutionSwitchInfo.eSwitchView == 0)
			{
				nVResWidth_S = i_tResolutionSwitchInfo.tNewRes.iWidth;
				pEnv->SetIntField(m_Activity, m_jVResWidth_SId, nVResWidth_S);
			}
			else
			{
				nVResWidth_R = i_tResolutionSwitchInfo.tNewRes.iWidth;
				pEnv->SetIntField(m_Activity, m_jVResWidth_RId, nVResWidth_R);
			}

			toastText = pEnv->NewStringUTF(pToastText);
			pEnv->CallVoidMethod(m_Activity, mToastShowMethodId, toastText);
 			pEnv->DeleteLocalRef(toastText);
			delete[] pToastText;
			JNIDetachEnv(bDoDetach);

		}
	}

	return;
}

const char* ZIMEAndroidQoSNotify::GetSwitchInfo(E_ZIMEResSwitchTrigger i_eSwitchInfo)
{
	switch(i_eSwitchInfo)
	{
	case enumZIME_SenderPerfHungry:
		return "SenderPerfHungry";
	case enumZIME_RecverPerfHungry:
		return "RecverPerfHungry";
	case enumZIME_NetWorkBad:
		return "NetWorkBad";
	case enumZIME_NetWorkGood:
		return "NetWorkGood";
	case enumZIME_ResSwitchMax:
	default:
		return "ResSwitchMax";
	}
}

void ZIMEAndroidQoSNotify::JNIDetachEnv(bool i_bDoDetach)
{
	if (i_bDoDetach)
	{
		m_pJavaVM->DetachCurrentThread();
	}
}

int ZIMEAndroidQoSNotify::JNIGetEnv(JNIEnv* &o_pJNIEnv, bool &o_b8DoDetach)
{
	o_b8DoDetach = false;
	jint iResult = m_pJavaVM->GetEnv((void**)&o_pJNIEnv, m_JNIVersion);
	if (JNI_OK != iResult)
	{
		iResult = m_pJavaVM->AttachCurrentThread(&o_pJNIEnv, NULL);
		if (iResult < 0)
		{
			return iResult;
		}
		o_b8DoDetach = true;
	}

	return 0;
}

void ZIMEAndroidQoSNotify::SetActivity(jobject i_Activity)
{
	m_Activity = i_Activity;
	JNIEnv *pEnv;
	bool bDoDetach;
	int s32Ret = JNIGetEnv(pEnv, bDoDetach);
	if (s32Ret < 0)
	{
		return;
	}
	else
	{
		ZIMENOTIFY_LOGE("before FindClass");
		mActivityClass = pEnv->FindClass("zime/ui/ZIMEAVDemoActivity");
		ZIMENOTIFY_LOGE("after FindClass");
 		mToastShowMethodId = pEnv->GetMethodID(mActivityClass, "ToastShow", "(Ljava/lang/String;)V");
 		m_jVResWidth_SId = pEnv->GetFieldID(mActivityClass, "mVResWidth_S", "I");
 		m_jVResWidth_RId = pEnv->GetFieldID(mActivityClass, "mVResWidth_R", "I");

	}
}

