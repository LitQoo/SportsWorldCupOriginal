package com.litqoo.lib;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.StringTokenizer;
import java.util.UUID;
import java.util.concurrent.Semaphore;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnClickListener;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.RemoteException;
import android.os.Vibrator;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;


import com.example.android.trivialdrivesample.util.IabHelper;
import com.example.android.trivialdrivesample.util.IabResult;
import com.google.ads.Ad;
import com.google.ads.AdListener;
import com.google.ads.AdRequest;
import com.google.ads.AdRequest.ErrorCode;
import com.google.ads.AdView;
import com.litqoo.SportsWorldCup.R;


@SuppressWarnings("unused")
abstract class LQRunnable implements Runnable
{
	protected final String totalSource;
	protected final int delekey;
	LQRunnable(String str, int k)
	{
		totalSource = str;
		delekey = k;
	}
}

public class KSActivityBase
{
	protected Cocos2dxGLSurfaceView mGLView;
	private final Semaphore semaphore = new Semaphore(1);
	private Handler handler = new Handler();
	
	protected String deviceId;
	private Activity newActivity;
	public void setActivity(Activity ac)
	{
		newActivity = ac;
		
		final TelephonyManager tm = (TelephonyManager) newActivity.getBaseContext().getSystemService(Context.TELEPHONY_SERVICE);
        final String tmDevice, tmSerial, androidId;
        tmDevice = "" + tm.getDeviceId();
        tmSerial = "" + tm.getSimSerialNumber();
        androidId = "" + android.provider.Settings.Secure.getString(newActivity.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
        UUID deviceUuid = new UUID(androidId.hashCode(), ((long)tmDevice.hashCode() << 32) | tmSerial.hashCode());
        deviceId = deviceUuid.toString();
	}
	protected String getUDID()
	{
		Log.i("asdasd", "asasd");
		return deviceId;
	}
	
	protected String getDeviceInfo()
	{
		String dinfo = android.os.Build.VERSION.RELEASE + '/';
		dinfo += android.os.Build.MODEL;
		return dinfo;
	}
	private static Context activity;

	// //////////////////////////////////////////////////////
	public static Object getActivity() {
		Log.i("cppCall", "return activity~~~~!!!");
		return activity;
	}
	
	
	
	
	// //////////////////////////////////////////////////////////////////////////////////

	private void openApp(final String app_name)
	{
		getHandler().post(new Runnable() {
			public void run() {
				try {
					semaphore.acquire();
					Intent market = new Intent(
							Intent.ACTION_VIEW,
							Uri.parse("market://details?id=" + app_name));
					newActivity.startActivity(market);
					
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}
			}
		});
	}
	private void showMoreApps() {
//		final String cCompany = company;
		getHandler().post(new Runnable() {
			public void run() {
				try {
					semaphore.acquire();
					Intent goToMarket;
					goToMarket = new Intent(Intent.ACTION_VIEW, Uri
							.parse("market://search?q=pub:LitQoo"));
					newActivity.startActivity(goToMarket);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}
	private void showMoreApps(final String company) {
//		final String cCompany = company;
		getHandler().post(new Runnable() {
			public void run() {
				try {
					semaphore.acquire();
					Intent goToMarket;
					goToMarket = new Intent(Intent.ACTION_VIEW, Uri
							.parse("market://search?q=pub:" + company));
					newActivity.startActivity(goToMarket);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}
	private void vib() {
		getHandler().post(new Runnable() {

			public void run() {
				// TODO Auto-generated method stub
				Vibrator vibe = (Vibrator) newActivity.getSystemService(Context.VIBRATOR_SERVICE);
				vibe.vibrate(500);
				
			}
		});
	}

	

	private String getUTF8String(String _strKey) {
		int resourceID = newActivity.getResources().getIdentifier(_strKey, "string",
				newActivity.getPackageName());
		// Log.i("tt", "" + resourceID);
		if (resourceID != 0)
			return newActivity.getResources().getString(resourceID);
		else {
			return _strKey;
		}
	}

	public Handler getHandler() {
		return handler;
	}

	public void setHandler(Handler handler) {
		this.handler = handler;
	}
}
