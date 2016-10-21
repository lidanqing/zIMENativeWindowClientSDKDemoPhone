package zime.ui;


import android.media.AudioManager;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import zime.media.ZIMEClientJni;
import zime.media.ZIMEVideoClientJNI;
import zime.media.ZIMEVideoClientJNI.T_ZIMEAudioDownlinkStat;
import zime.media.ZIMEVideoClientJNI.T_ZIMEAudioUplinkStat;
import zime.media.ZIMEVideoClientJNI.T_ZIMEVideoDownlinkStat;
import zime.media.ZIMEVideoClientJNI.T_ZIMEVideoUplinkStat;

public class ZIMEJniThread extends Thread
{
	private static final String ZIMETAG = ZIMEJniThread.class.getCanonicalName();
	private ZIMEMedia mZIMEMedia = null;
	private Handler mMsgHandler  = null;
	private ZIMEAudio mZIMEAudio = null;
	private ZIMEConfig mConf = null;
	private ZIMEVideoClientJNI mVJNI = null;
	private ZIMEClientJni mAJNI = null;
	private static final int ifGetQos = 1;
	private Object mActivity;
	private AudioManager am = null;

	final Handler handler = new Handler();
	Runnable runnable=new Runnable(){
		public void run() {
			handler.postDelayed(this, 1000);
			// TODO Auto-generated method stub
			//要做的事情
			if(ifGetQos == 1)
			{
				T_ZIMEAudioUplinkStat   uplinkAudioQosStat   = new T_ZIMEAudioUplinkStat(0,0,0,0,0,0);
				T_ZIMEAudioDownlinkStat downlinkAudioQosStat = new T_ZIMEAudioDownlinkStat(0,0,0,0,0,0);
				T_ZIMEVideoUplinkStat   uplinkVideoQosStat   = new T_ZIMEVideoUplinkStat(0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1);
				T_ZIMEVideoDownlinkStat downlinkVideoQosStat = new T_ZIMEVideoDownlinkStat(0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1, -1);

				String DownLinkQos = "";
				if(!ZIMEConfig.mIsOnlyAudio)
				{
					mZIMEMedia.GetAudioQosStat(uplinkAudioQosStat,downlinkAudioQosStat);
					mZIMEMedia.GetVideoQosStat(uplinkVideoQosStat,downlinkVideoQosStat);

					DownLinkQos = "DownQoS" + "\n" +
							"Jitter A/V:" +  downlinkAudioQosStat.iCurJitter + "/" + downlinkVideoQosStat.iCurJitter + "\n" +
							"Lost A/V:" + downlinkAudioQosStat.iCurFractionLost +"/" + downlinkVideoQosStat.iCurFractionLost + "\n" +
							"RTT A/V:" +  downlinkAudioQosStat.iCurRTT +"/" + downlinkVideoQosStat.iCurRTT + "\n" +
							"Audio \n" +
							"kbps S/R:" + uplinkAudioQosStat.iCurBitrate + "/" +  downlinkAudioQosStat.iCurBitrate + "\n" +
							"Video \n" +
							"FPS C/S/R/D:" + uplinkVideoQosStat.iRealCapFrameRate + "/" + uplinkVideoQosStat.iRealFrameRate + "/" + downlinkVideoQosStat.iRecvFrameRate + "/" +  downlinkVideoQosStat.iDisplayFrameRate + "\n" +
							"Res S/R:" + uplinkVideoQosStat.iWidth + "/" +  downlinkVideoQosStat.iWidth + "\n" +
							"Trigger S/R:" + GetSwitchInfo(uplinkVideoQosStat.iResSwitchTrigger) + "/" +  GetSwitchInfo(downlinkVideoQosStat.iResSwitchTrigger) + "\n" +
							"Total kbps S/R:" + uplinkVideoQosStat.iCurBitrate + "/" +  downlinkVideoQosStat.iCurBitrate + "\n" +
							"Encode kbps S:" + uplinkVideoQosStat.iExpectedESBitRate  + "\n" +
							"ES kbps S/R:" + uplinkVideoQosStat.iRealESBitRate_Cur + "/" +  downlinkVideoQosStat.iESBitRate_Cur + "\n" +
							"Red kbps S/R:" + uplinkVideoQosStat.iRedundantBitRate_Cur + "/" +  downlinkVideoQosStat.iRedundantBitRate_Cur + "\n" +
							"Network Lost:" + downlinkVideoQosStat.iCurFractionLost + "%" + "\n" +
							"Real Lost:" + downlinkVideoQosStat.iRealPktLostRate + "%" + "\n"
					;
				}
				else {
					mZIMEAudio.GetAudioQosStat(uplinkAudioQosStat,downlinkAudioQosStat);

					DownLinkQos = "DownQoS" + "\n" +
							"Jitter :" +  downlinkAudioQosStat.iCurJitter  + "\n" +
							"Lost :" + downlinkAudioQosStat.iCurFractionLost  + "\n" +
							"RTT :" +  downlinkAudioQosStat.iCurRTT  + "\n" +
							"kbps S/R:" + uplinkAudioQosStat.iCurBitrate + "/" +  downlinkAudioQosStat.iCurBitrate + "\n" ;
				}



				((ZIMEAVDemoActivity) mActivity).GetDownLinkQosToShow(DownLinkQos);

//		    	Log.e(ZIMETAG, "############# 音视频上行参数：" );
//		    	Log.e(ZIMETAG, "############# A码率" + " " + uplinkAudioQosStat.iCurBitrate + " ,V码率" + uplinkVideoQosStat.iCurBitrate);
//		    	Log.e(ZIMETAG, "############# A抖动" + " " + uplinkAudioQosStat.iCurJitter + " ,V抖动" + uplinkVideoQosStat.iCurJitter);
//		    	Log.e(ZIMETAG, "############# A丢包率" + " " + uplinkAudioQosStat.iCurFractionLost + " ,V丢包率" + uplinkVideoQosStat.iCurFractionLost);
//		    	Log.e(ZIMETAG, "############# A丢包总数" + " " + uplinkAudioQosStat.iTotalFractionLost + " ,V丢包总数" + uplinkVideoQosStat.iTotalFractionLost);
//		    	Log.e(ZIMETAG, "############# A平均码率" + " " + uplinkAudioQosStat.iAvgBitrate + " ,V平均码率" + uplinkVideoQosStat.iAvgBitrate);
//		    	Log.e(ZIMETAG, "############# A RTT" + " " + uplinkAudioQosStat.iCurRTT + " ,V RTT" + uplinkVideoQosStat.iCurRTT);
//		    	Log.e(ZIMETAG, "############# V 期望帧率" + " " + uplinkVideoQosStat.iExpectedFrameRate);
//		    	Log.e(ZIMETAG, "############# V 实际帧率" + " " + uplinkVideoQosStat.iRealFrameRate);
//		    	Log.e(ZIMETAG, "    ");
//		    	Log.e(ZIMETAG, "############# 音视频下行参数：" );
//		    	Log.e(ZIMETAG, "############# A码率" + " " + downlinkAudioQosStat.iCurBitrate + " ,V码率" + downlinkVideoQosStat.iCurBitrate);
//		    	Log.e(ZIMETAG, "############# A抖动" + " " + downlinkAudioQosStat.iCurJitter + " ,V抖动" + downlinkVideoQosStat.iCurJitter);
//		    	Log.e(ZIMETAG, "############# A丢包率" + " " + downlinkAudioQosStat.iCurFractionLost + " ,V丢包率" + downlinkVideoQosStat.iCurFractionLost);
//		    	Log.e(ZIMETAG, "############# A丢包总数" + " " + downlinkAudioQosStat.iTotalFractionLost + " ,V丢包总数" + downlinkVideoQosStat.iTotalFractionLost);
//		    	Log.e(ZIMETAG, "############# A平均码率" + " " + downlinkAudioQosStat.iAvgBitrate + " ,V平均码率" + downlinkVideoQosStat.iAvgBitrate);
//		    	Log.e(ZIMETAG, "############# A RTT" + " " + downlinkAudioQosStat.iCurRTT + " ,V RTT" + downlinkVideoQosStat.iCurRTT);
//		    	Log.e(ZIMETAG, "############# V 接收帧率" + " " + downlinkVideoQosStat.iRecvFrameRate);
//		    	Log.e(ZIMETAG, "############# V 显示帧率" + " " + downlinkVideoQosStat.iDisplayFrameRate);		    	
//		    	Log.e(ZIMETAG, "    ");
			}




		}
	};

	public String GetSwitchInfo(int i_eSwitchInfo)
	{
		switch(i_eSwitchInfo)
		{
			case 0: //enumZIME_SenderPerfHungry
				return "SHungry";
			case 1: //enumZIME_RecverPerfHungry
				return "RHungry";
			case 2: //enumZIME_NetWorkBad
				return "NWBad";
			case 3: //enumZIME_NetWorkGood
				return "NWGood";
			case 4: //enumZIME_ResSwitchMax
			default:
				return "-1";
		}
	}

	public ZIMEJniThread(ZIMEVideoClientJNI i_VClientJni, ZIMEClientJni i_AClientJni) {
		mVJNI = i_VClientJni;
		mAJNI = i_AClientJni;

		mZIMEMedia = new ZIMEMedia(mVJNI);
		mZIMEMedia.SetLogCallBack();

		// 创建引擎	
		int resultCode = ZIMEVideoClientJNI.SetLogPath("/sdcard");
		Log.e(ZIMETAG, "ZIMEVideoClient set log path: resultCode = " + resultCode);

		resultCode = ZIMEVideoClientJNI.Create();
		if(0 != resultCode){
			Log.e(ZIMETAG, "ZIMEVideoClient Create failed!!, Ret = " + resultCode);
			return;
		}

		//设置初始默认的日志级别
		ZIMEVideoClientJNI.SetLogLevel(ZIMEMedia.ZIME_L_DEBUG);

		resultCode = ZIMEClientJni.Create();
		if(0 != resultCode){
			Log.e(ZIMETAG, "ZIMEAudioClient Create failed!!, Ret = " + resultCode);
			return;
		}
		mZIMEAudio = new ZIMEAudio(mAJNI);
		//mZIMEAudio.setAudioMan(am);

		Log.e(ZIMETAG, "ZIMEJniThread Constructor--------- ");
	}

	public void SetActivity(Object activity)
	{
		mActivity = activity;
	}

	public int Input(int msgWhat, Object msgObj){
		Message msg = mMsgHandler.obtainMessage();
		msg.what = msgWhat;
		msg.obj = msgObj;
		mMsgHandler.sendMessage(msg);
		return 0;
	}

	private void SetConf(ZIMEConfig i_ZIMEConfig){
		Log.e(ZIMETAG, "SetConf, ip = " + i_ZIMEConfig.mRecvIP);

		if(!ZIMEConfig.mIsOnlyAudio)
		{
			mZIMEMedia.init(mVJNI);
			mZIMEMedia.SetConf(i_ZIMEConfig);
			ZIMEVideoClientJNI.SetActivity(mActivity);
		}
		else
		{
			mZIMEAudio.SetConf(i_ZIMEConfig);
		}

		return;
	}

	private void SetVideoConf(ZIMEConfig i_ZIMEConfig){
		Log.e(ZIMETAG, "SetVideoConf, ip = " + i_ZIMEConfig.mRecvIP);
		if(!ZIMEConfig.mIsOnlyAudio)
		{
			mZIMEMedia.SetVideoConf(i_ZIMEConfig);
		}
		return;
	}

	private void SetAudioConf(ZIMEConfig i_ZIMEConfig){
		Log.e(ZIMETAG, "SetAudioConf, ip = " + i_ZIMEConfig.mRecvIP);
		if(!ZIMEConfig.mIsOnlyAudio)
		{
			mZIMEMedia.init(mVJNI);
			mZIMEMedia.SetAudioConf(i_ZIMEConfig);
			ZIMEVideoClientJNI.SetActivity(mActivity);
		}
		return;
	}

	private void Start(){
		Log.e(ZIMETAG, "Start" + "--" + mConf.mRecvIP);

		if(mConf.mSaveH26XFile)
		{
			mZIMEMedia.SetVideoModeSet();
		}

		if(!ZIMEConfig.mIsOnlyAudio)
		{
			//mZIMEMedia.init(mVJNI);
			mZIMEMedia.setAudioMan(am);
			mZIMEMedia.Start();
		}
		else
		{
			//mZIMEAudio.init(mAJNI);
			mZIMEAudio.setAudioMan(am);
			mZIMEAudio.Start();
		}

		handler.postDelayed(runnable, 4000);
		return;
	}

	private void Stop(){
		Log.e(ZIMETAG, "Stop");

		if (mZIMEAudio == null && mZIMEMedia == null) {
			return;
		}
		handler.removeCallbacks(runnable);
		if(!ZIMEConfig.mIsOnlyAudio)
		{
			mZIMEMedia.Stop();
		}
		else
		{
			mZIMEAudio.Stop();
		}

		Log.d(ZIMETAG, "hangup OK");

		return;
	}

	private void Exit(){
		Log.e(ZIMETAG, "Exit");
		mZIMEMedia.LogExit();
		if (mZIMEAudio == null && mZIMEMedia == null) {
			return;
		}
		if(!ZIMEConfig.mIsOnlyAudio)
		{
			mZIMEMedia.Exit();

			mZIMEMedia = null;

		}
		else
		{
			mZIMEAudio.Exit();
			mZIMEMedia = null;
		}

		return;
	}
	private void StartSend(){
		Log.e(ZIMETAG, "StartSend");

		if(!ZIMEConfig.mIsOnlyAudio)
		{
			mZIMEMedia.StartSend();
		}

		handler.postDelayed(runnable, 4000);

		return;
	}

	private void StartRecv(){
		Log.e(ZIMETAG, "StartRecv");

		if(mConf.mSaveH26XFile)
		{
			mZIMEMedia.SetVideoModeSet();
		}

		if(!ZIMEConfig.mIsOnlyAudio)
		{
			mZIMEMedia.StartRecv();
		}

		handler.postDelayed(runnable, 4000);

		return;
	}

	public void setAudioMan(AudioManager amm){
		am = amm;

		return;
	}

	public void StartAByAVClientInterface(){
		Log.e(ZIMETAG, "Start Audio by ClientInterface" + "--" + mConf.mRecvIP);
		if(!ZIMEConfig.mIsOnlyAudio)
		{
			//mZIMEMedia.init(mVJNI);
			mZIMEMedia.setAudioMan(am);
			mZIMEMedia.StartAudio();
		}

		handler.postDelayed(runnable, 4000);
		return;
	}

	public void toa(){
		mZIMEMedia.ToAudio();
		return;
	}

	public void toAV(){
		mZIMEMedia.toAV();
		return;
	}

	@Override
	public void run(){
		Looper.prepare();

		mMsgHandler = new Handler(){
			@Override
			public void handleMessage(Message msg){
				Log.e("ZIMETAG", "handle Message---" + msg.what);


				switch (msg.what)
				{
					case ZIMEConfig.SET_PARAM:
					{
						if(null != (ZIMEConfig)msg.obj)
						{
							mConf = (ZIMEConfig)msg.obj;
							SetConf(mConf);
						}

						break;
					}
					case ZIMEConfig.SET_VIDEOPARAM:
					{
						if(null != (ZIMEConfig)msg.obj)
						{
							mConf = (ZIMEConfig)msg.obj;
							SetVideoConf(mConf);
						}

						break;
					}
					case ZIMEConfig.SET_AUDIOPARAM_BEFORSTART:
					{
						if(null != (ZIMEConfig)msg.obj)
						{
							mConf = (ZIMEConfig)msg.obj;
							SetAudioConf(mConf);
						}

						break;
					}
					case ZIMEConfig.START:
						Start();
						break;
					case ZIMEConfig.STOP:
						Stop();
						break;
					case ZIMEConfig.EXIT:
						Exit();
						break;
					case ZIMEConfig.STARTSEND:
						StartSend();
						break;
					case ZIMEConfig.STARTRECV:
						StartRecv();
						break;

					case ZIMEConfig.ASTART_BY_AVCLIENTINTERFACE:
						StartAByAVClientInterface();
						break;
					case ZIMEConfig.TOA:
						toa();
						break;
					case ZIMEConfig.TOAV:
						toAV();
						break;
					default:
						break;
				}
			}
		};
		Looper.loop();
	}


}
