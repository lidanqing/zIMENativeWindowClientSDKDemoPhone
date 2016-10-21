#ifndef __ZIMEAndroidQoSNotify_H__  
#define __ZIMEAndroidQoSNotify_H__  

#include <jni.h>
#include "ZIMEType.h"

class ZIMEAndroidQoSNotify:public ZIME_NetworkQualityNotify
{
public:
	ZIMEAndroidQoSNotify(JavaVM *i_pJavaVM, jint i_JNIVersion)
	{
		m_pJavaVM = i_pJavaVM;
		m_JNIVersion = i_JNIVersion;
		m_Activity = NULL;
		mActivityClass = NULL;
		mToastShowMethodId = NULL;
		m_jVResWidth_SId = NULL;
		m_jVResWidth_RId = NULL;

	}
	
	~ZIMEAndroidQoSNotify()
	{
		
	}
	
	void NotifyDownLinkQOS(int i_s32ChId, T_ZIMEVideoDownLinkQOS& i_tNetworkQualityInfo);
	void NotifyUpLinkQOS(int i_s32ChId, T_ZIMEVideoUpLinkQOS& i_tNetworkQualityInfo);
	void ResolutionSwitchNotify(int i_s32ChId,T_ZIMEResolutionSwitchInfo& i_tResolutionSwitchInfo);
	void SetActivity(jobject i_Activity);
	
private:
	void JNIDetachEnv(bool i_bDoDetach);
	int JNIGetEnv(JNIEnv* &o_pJNIEnv, bool &o_b8DoDetach);
	const char* GetSwitchInfo(E_ZIMEResSwitchTrigger i_eSwitchInfo);

private:
	JavaVM* m_pJavaVM;
	jint m_JNIVersion;
	jobject m_Activity;
	jclass mActivityClass;
	jmethodID mToastShowMethodId;

	jfieldID m_jVResWidth_SId;
	jfieldID m_jVResWidth_RId;

};

#endif
