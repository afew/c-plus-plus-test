package com.netmarble.elemental;

import java.net.URLDecoder;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import net.netmarble.push.GCMIntentService;

public class GCMCustomIntentService extends GCMIntentService
{
	private String decodeString(String message)
	{
		String returnValue = new String();

		try
		{
			returnValue = URLDecoder.decode(message, "UTF-8");
			return returnValue;
		}
		catch (Exception e)
		{
			return message;
		}
	}

	@Override protected void onMessage(Context context, Intent intent)
	{
		AppUtil.LOGW("GCMCustomIntentService::On Message::Receive");

		if(APPCONST.NSDK_FALSE == AppUtil.isActive() )						// inactive
		{
			super.onMessage(context, intent);								// 반드시 호출합니다.
		}

		boolean isInGamePush = isInGamePush(intent);						// 현재 수신된 푸시 메시지가 인 게임 푸시인지 확인합니다.

		if(false == isInGamePush)
		{
			AppUtil.LOGI("GCMCustomIntentService::On Message::Receive:: isn't ingame push");
			return;
		}

		Bundle extras = intent.getExtras();
		String alert  = extras.getString("alert");
		String message= decodeString(alert);								// alert 값은 URLEncoding 되어 있습니다. Decode하여 사용하세요.

		AppUtil.LOGI("GCMCustomIntentService::onMessage::rcv: " + message);
	}
}

