/*
** Copyright 2013-2017 Zumero, LLC
** All rights reserved. 
 */

package com.zumero.cordova;

// Uncomment these four imports for PhoneGap 2.x
// Comment them out for PhoneGap 3.x
// import org.apache.cordova.api.CallbackContext;
// import org.apache.cordova.api.CordovaPlugin;
// import org.apache.cordova.api.CordovaWebView;
// import org.apache.cordova.api.CordovaInterface;

// Uncomment these four imports for PhoneGap 3.x
// Comment them out for PhoneGap 2.x
import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.PluginResult;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import java.lang.reflect.Method;

import com.zumero.ZumeroClient;
import com.zumero.ZumeroClient.SyncDetails;
import com.zumero.ZumeroException;
import com.zumero.SyncProgressListener;

import android.webkit.WebView;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.app.Activity;

public class ZumeroPlugin extends CordovaPlugin {

	//The worker thread, so that operations can run in the background.
	private ZumeroWorkerThread worker = null;
	private CountDownLatch lock = new CountDownLatch(1);
	private CallbackContext jsSender = null;
		
	public ZumeroPlugin() {		
	 	super();

	 	zumeroSetupPG2();
	}

	//	later initialization, where supported
	//
	public void initialize(CordovaInterface cordova, CordovaWebView webView) {
		super.initialize(cordova, webView);

		zumeroSetup();
	}

	// initialize at construction under earlier API versions
	// 
	private void zumeroSetupPG2()
	{
		boolean isPG3 = false;

		try
		{
			Class sup = super.getClass();
			Class [] args = new Class[2];
			args[0] = CordovaInterface.class;
			args[1] = CordovaWebView.class;

			Method m = sup.getMethod("initialize", args);

			isPG3 = true;
		}
		catch (NoSuchMethodException e)
		{
			isPG3 = false;
		}

		if (! isPG3)
		{
			zumeroSetup();
		}
	}

	private void zumeroSetup()
	{
		if (this.worker != null)
			return;

		this.worker = new ZumeroWorkerThread(lock);
		this.worker.start();

		try
		{
			lock.await(2000, TimeUnit.MILLISECONDS);
		}
		catch (java.lang.InterruptedException e)
		{
			Log.e("ZumeroPhonegap", "await interrupted", e);
		}
	}

	@Override
	public boolean execute(final String action, final JSONArray args,
		final CallbackContext callbackContext) throws JSONException {

		if ("sync".equals(action)) {
			//This operation happens on the background.
			this.worker.getHandler().post(new Runnable() {
				@Override
				public void run() {
					try {
						sync(args, callbackContext);
					} catch (Exception e) {
						Log.e("ZumeroPhonegap","Exception: ", e );
					}
				}
			});
			return true;
		}
		else if ("sync2".equals(action)) {
			//This operation happens on the background.
			this.worker.getHandler().post(new Runnable() {
				@Override
				public void run() {
					try {
						sync2(args, callbackContext);
					} catch (Exception e) {
						Log.e("ZumeroPhonegap","Exception: ", e );
					}
				}
			});
			return true;
		}
		else if ("sync3".equals(action)) {
			//This operation happens on the background.
			this.worker.getHandler().post(new Runnable() {
				@Override
				public void run() {
					try {
						sync2(args, callbackContext);
					} catch (Exception e) {
						Log.e("ZumeroPhonegap","Exception: ", e );
					}
				}
			});
			return true;
		}
		else if ("syncQuarantine".equals(action)) {
			//This operation happens on the background.
			this.worker.getHandler().post(new Runnable() {
				@Override
				public void run() {
					try {
						SyncQuarantine(args, callbackContext);
					} catch (Exception e) {
						Log.e("ZumeroPhonegap","Exception: ", e );
					}
				}
			});
			return true;
		}
		else if ("quarantineSinceLastSync".equals(action)) {
			//This operation happens on the background.
			this.worker.getHandler().post(new Runnable() {
				@Override
				public void run() {
					try {
						QuarantineSinceLastSync(args, callbackContext);
					} catch (Exception e) {
						Log.e("ZumeroPhonegap","Exception: ", e );
					}
				}
			});
			return true;
		}
		else if ("deleteQuarantine".equals(action)) {
			//This operation happens on the background.
			this.worker.getHandler().post(new Runnable() {
				@Override
				public void run() {
					try {
						DeleteQuarantine(args, callbackContext);
					} catch (Exception e) {
						Log.e("ZumeroPhonegap","Exception: ", e );
					}
				}
			});
			return true;
		}
		else if ("cancel".equals(action)) {
			try {
				Cancel(args, callbackContext);
			} catch (Exception e) {
				Log.e("ZumeroPhonegap","Exception: ", e );
			}
			return true;
		}
		else if ("setupJSPassthrough".equals(action))
		{
			jsSender = callbackContext;
			sendJavascript(";");
			return true;
		}
		else
			return super.execute(action, args, callbackContext);
	}
	
	private void HandleException(Exception e, CallbackContext callbackContext)
	{
		JSONObject resultObj = new JSONObject();
		try {
			if (e instanceof ZumeroException)
				resultObj.put("code", ((ZumeroException)e).getErrCode());
			resultObj.put("message", e.getMessage());
			callbackContext.error(resultObj);
		} catch (JSONException e1) {
			Log.e("ZumeroPhonegap","JSONException: ", e1 );
			callbackContext.error("sync failed");
		}
	}
	
	private void sync(JSONArray args, final CallbackContext callbackContext) throws JSONException {
		
		try {
				ZumeroClient.sync(cordova.getActivity().getApplicationContext(),
					args.getString(0), //full path
					args.isNull(1) ? null : args.getString(1), //Encryption key 
					args.getString(2), //URL
					args.getString(3), //DBfile
					args.isNull(4) ? null : args.getString(4), //scheme
					args.isNull(5) ? null : args.getString(5), //user
					args.isNull(6) ? null : args.getString(6)  //password
				);
		}
		catch (Exception e) {
			HandleException(e, callbackContext); return;
		}
		callbackContext.success();
	}

	private void sync2(JSONArray args, final CallbackContext callbackContext) throws JSONException {
		
		try {
				SyncProgressListener progress_callback = null;
				if (!args.isNull(7)) //Callback function
				{
					final double callback_token = args.getDouble(7);
					final Activity act = this.cordova.getActivity();
					progress_callback = new SyncProgressListener()
					{
						@Override
						public void onSyncProgress(final int cancellation_token, final int phase, final long bytesSoFar, final long bytesTotal)
						{
							act.runOnUiThread(new Runnable() {
								public void run() {
									sendJavascript("zumero_global_progress_callback_function(" + callback_token + ", " + cancellation_token + ", " + phase + ", " + bytesSoFar + ", " + bytesTotal + ");");
								}
							});
						}
					};
				}
				ZumeroClient.sync(cordova.getActivity().getApplicationContext(),
					args.getString(0), //full path
					args.isNull(1) ? null : args.getString(1), //Encryption key 
					args.getString(2), //URL
					args.getString(3), //DBfile
					args.isNull(4) ? null : args.getString(4), //scheme
					args.isNull(5) ? null : args.getString(5), //user
					args.isNull(6) ? null : args.getString(6),  //password
					progress_callback);
		}
		catch (Exception e) {
			HandleException(e, callbackContext);
			return;
		}
		callbackContext.success();
	}
	
	private void sync3(JSONArray args, final CallbackContext callbackContext) throws JSONException {

		int syncId = -1;
		
		try {
				SyncProgressListener progress_callback = null;
				if (!args.isNull(7)) //Callback function
				{
					final double callback_token = args.getDouble(7);
					final Activity act = this.cordova.getActivity();
					progress_callback = new SyncProgressListener()
					{
						@Override
						public void onSyncProgress(final int cancellation_token, final int phase, final long bytesSoFar, final long bytesTotal)
						{
							act.runOnUiThread(new Runnable() {
								public void run() {
									sendJavascript("javascript:zumero_global_progress_callback_function(" + callback_token + ", " + cancellation_token + ", " + phase + ", " + bytesSoFar + ", " + bytesTotal + ");");
								}
							});
						}
					};
				}

				String opts = args.isNull(8) ? null: args.getString(8);
				SyncDetails d = new SyncDetails();
				d.syncId = -1;

				ZumeroClient.sync(cordova.getActivity().getApplicationContext(),
					args.getString(0), //full path
					args.isNull(1) ? null : args.getString(1), //Encryption key 
					args.getString(2), //URL
					args.getString(3), //DBfile
					args.isNull(4) ? null : args.getString(4), //scheme
					args.isNull(5) ? null : args.getString(5), //user
					args.isNull(6) ? null : args.getString(6),  //password
					opts, d,
					progress_callback);

				syncId = d.syncId;
		}
		catch (Exception e) {
			HandleException(e, callbackContext);
			return;
		}
		callbackContext.success(syncId);
	}
	
	private void QuarantineSinceLastSync(JSONArray args, final CallbackContext callbackContext) throws JSONException {
		try {
			long qid = ZumeroClient.quarantineSinceLastSync(cordova.getActivity().getApplicationContext(),
					args.getString(0), //full path
					args.isNull(1) ? null : args.getString(1) //Encryption key 
					);
			JSONObject resultObj = new JSONObject();
			resultObj.put("quarantineID", qid);
			callbackContext.success(resultObj);
		}
		catch (Exception e) {
			HandleException(e, callbackContext);
		}
	}
	
	private void SyncQuarantine(JSONArray args, final CallbackContext callbackContext) throws JSONException {
		try {
			ZumeroClient.syncQuarantine(cordova.getActivity().getApplicationContext(),
					args.getString(0), //full path
					args.isNull(1) ? null : args.getString(1), //Encryption key
					args.getLong(2),
					args.getString(3), //URL
					args.getString(4), //DBfile
					args.isNull(5) ? null : args.getString(5), //scheme
					args.isNull(6) ? null : args.getString(6), //user
					args.isNull(7) ? null : args.getString(7)  //password
					);
		}
		catch (Exception e) {
			HandleException(e, callbackContext);
			return;
		}
		callbackContext.success();
	}
	
	private void DeleteQuarantine(JSONArray args, final CallbackContext callbackContext) throws JSONException {
		try {
			ZumeroClient.deleteQuarantine(cordova.getActivity().getApplicationContext(),
					args.getString(0), //full path
					args.isNull(1) ? null : args.getString(1), //Encryption key
					args.getLong(2)	//quarantineID
					);
		}
		catch (Exception e) {
			HandleException(e, callbackContext);
		}
	}

	private void Cancel(JSONArray args, final CallbackContext callbackContext) throws JSONException {
		try {
			int cancel_token = args.getInt(0);
			ZumeroClient.cancel(
					cancel_token
					);
		}
		catch (Exception e) {
			HandleException(e, callbackContext);
		}
	}

	private void sendJavascript(String code)
	{
		PluginResult pr = new PluginResult(PluginResult.Status.OK, code);
		pr.setKeepCallback(true);
		jsSender.sendPluginResult(pr);
	}
	
	//These two classes handle the background thread.
	//In this case, ZumeroHandler makes sure operations happen one at a time.
	private static class ZumeroHandler extends Handler {
		public void handleMessage(Message msg) {
		}
	}
	
	private static class ZumeroWorkerThread extends Thread {
		private static ZumeroHandler handler;
		private CountDownLatch lock;
		
		public Handler getHandler() {
			return handler;
		}
		
		@Override
	    public void run() {
			Looper.prepare();
			this.lock.countDown();
			Looper.loop();
		}
		
		public ZumeroWorkerThread(CountDownLatch lock) {
			super();
			this.lock = lock;
			handler = new ZumeroHandler();
		}
	}
}
