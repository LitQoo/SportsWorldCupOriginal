/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.litqoo.SportsWorldCup;

import java.util.ArrayList;
import java.util.StringTokenizer;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import com.android.vending.billing.IInAppBillingService;
import com.example.android.trivialdrivesample.util.IabHelper;
import com.example.android.trivialdrivesample.util.IabResult;
import com.example.android.trivialdrivesample.util.Inventory;
import com.example.android.trivialdrivesample.util.Purchase;
import com.google.ads.AdRequest;
import com.google.ads.AdSize;
import com.google.ads.AdView;
import com.litqoo.lib.FBConnectorBase;
import com.litqoo.lib.KSActivityBase;

import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.widget.LinearLayout;

import com.litqoo.lib.*;

public class SportsWorldCup extends Cocos2dxActivity{
	private AdView adView;
	private static Context activity;
	public static Object getActivity(){ return activity; }
	private Handler handler = new Handler();
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		KSAWrap.obj.setActivity(this);
		//////////////////////////////////////////////////////////////////
		//SnowFilelog.write(KSInAppBase.this, "public key");
		activity = this;
		String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA3dCC/RrWI9mUJDpBwAC2/+Uj0mpYXMOvk2UtlLSpFboREgGQxAJ6ElDu3jUi8hdWnlgwhOjTdI8INM7taik2UenXscF9IjVf0+vLqpaCMOi2t29X54pw3VxH6SBFFT6wD4zfcRVErOQzMHuEOpoMT3pJXFDmj778TpnN3OVvJWThbItSqXtrKakOF4Dge0giHMCMDWjC0Zwr7uIjy78e6apuZ4ljAu8bFhxfkrCCPra2Rm2ifW8ulHFvWaStbxEmB0bVbwX+qTgVk0puWEMLwlYItbkTz/NxDv9oa9zxG4+ClsQeqKdZVY6jB3QpNYi2x5HD7wREbiMUTdssPruEzQIDAQAB";

		mHelper = new IabHelper(this, base64EncodedPublicKey);

		// enable debug logging (for a production application, you should set this to false).
		mHelper.enableDebugLogging(true);

		// Start setup. This is asynchronous and the specified listener
		// will be called once setup completes.
		mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
			public void onIabSetupFinished(IabResult result) {

				if (!result.isSuccess()) {
					// Oh noes, there was a problem.
					mSetupDone = false;
					complain("Problem setting up in-app billing: " + result);
					return;
				}

				// Hooray, IAB is fully set up. Now, let's get an inventory of stuff we own.
				mSetupDone = true;
				Log.d(TAG, "Setup successful. Querying inventory.");
				//mHelper.queryInventoryAsync(mGotInventoryListener);
			}
		});		

		bindService(new 
				Intent("com.android.vending.billing.InAppBillingService.BIND"),
				mServiceConn, Context.BIND_AUTO_CREATE);
//		KSIWrap.obj.setActivity(this);
//		LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
//                getWindowManager().getDefaultDisplay().getWidth(),                        
//                getWindowManager().getDefaultDisplay().getHeight()+getWindowManager().getDefaultDisplay().getHeight()-50);
//
//        adView = new AdView(this, AdSize.BANNER, "a150e0e0ae5009b");
//        adView.setAdListener(this);                            
//        AdRequest request = new AdRequest();
//        request.addTestDevice("34162F3663F54E8A4B036F61F3667A1A");
//        adView.loadAd(request);                
//        // Adding full screen container
//        addContentView(adView, adParams);  
	}
    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// SportsWorldCup should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }     
    
    ///////////////////////////////////////////////////////////////////////////////////////
    private native void sendInAppData(String str, int resultcode, int delekey);
	IInAppBillingService mService;

	ServiceConnection mServiceConn = new ServiceConnection() {
	   @Override
	   public void onServiceDisconnected(ComponentName name) {
	       mService = null;
	   }

	   @Override
	   public void onServiceConnected(ComponentName name, 
	      IBinder service) {
	       mService = IInAppBillingService.Stub.asInterface(service);
	   }
	};
	
	static final String TAG = "BS2";
	static final int RC_REQUEST = 10011;
	protected boolean mSetupDone = false;
	IabHelper mHelper;
	void complain(String message) {
        Log.e(TAG, "**** TrivialDrive Error: " + message);
        alert("Error: " + message);
    }
	void alert(String message) {
        AlertDialog.Builder bld = new AlertDialog.Builder(this);
        bld.setMessage(message);
        bld.setNeutralButton("OK", null);
        Log.d(TAG, "Showing alert dialog: " + message);
        bld.create().show();
    }
	
	// Listener that's called when we finish querying the items we own
    IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            Log.d(TAG, "Query inventory finished.");
            //alert("Query inventory finished.");
            if (result.isFailure()) {
                complain("Failed to query inventory: " + result);
                return;
            }

            Log.d(TAG, "Query inventory was successful.");

            // Do we have the premium upgrade?
            
            // Check for gas delivery -- if we own gas, we should fill up the tank immediately
//            if (inventory.hasPurchase("10ruby")) {
//                mHelper.consumeAsync(inventory.getPurchase("10ruby"), mConsumeFinishedListener);
//            }
//            if (inventory.hasPurchase("25ruby")) {
//                mHelper.consumeAsync(inventory.getPurchase("25ruby"), mConsumeFinishedListener);
//            }
//            if (inventory.hasPurchase("40ruby")) {
//                mHelper.consumeAsync(inventory.getPurchase("40ruby"), mConsumeFinishedListener);
//            }
//            if (inventory.hasPurchase("200ruby")) {
//                mHelper.consumeAsync(inventory.getPurchase("200ruby"), mConsumeFinishedListener);
//            }
//            if (inventory.hasPurchase("free")) {
//                mHelper.consumeAsync(inventory.getPurchase("free"), mConsumeFinishedListener);
//            }
            Log.d(TAG, "Initial inventory query finished; enabling main UI.");
        }
    };
	
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d(TAG, "onActivityResult(" + requestCode + "," + resultCode + "," + data);

        // Pass on the activity result to the helper for handling
        if (!mHelper.handleActivityResult(requestCode, resultCode, data)) {
            // not handled, so handle it ourselves (here's where you'd
            // perform any handling of activity results not related to in-app
            // billing...
            super.onActivityResult(requestCode, resultCode, data);
        }
        else {
            Log.d(TAG, "onActivityResult handled by IABUtil.");
        }
    }
    // Called when consumption is complete
    IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
    	
        public void onConsumeFinished(Purchase purchase, IabResult result) {
            Log.d(TAG, "Consumption finished. Purchase: " + purchase + ", result: " + result);
            //alert("OnConsumeFinishedListener");
            // We know this is the "gas" sku because it's the only one we consume,
            // so we don't check which sku was consumed. If you have more than one
            // sku, you probably should check...
            if (result.isSuccess()) {
                // successfully consumed, so we apply the effects of the item in our
                // game world's logic, which in our case means filling the gas tank a bit
            	
                sendInAppData(purchase.getSku(), 
            			result.getResponse(), Integer.parseInt((purchase.getDeveloperPayload())));
                Log.d(TAG, "Consumption successful. Provisioning.");
            }
            else {
                complain("Error while consuming: " + result);
            }
            Log.d(TAG, "End consumption flow.");
        }
    };
    // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            Log.d(TAG, "Purchase finished: " + result + ", purchase: " + purchase);
            
            	
            
            //alert("OnIabPurchaseFinishedListener");
//            if (result.isFailure()) {
//                // Oh noes!
//                complain("Error purchasing: " + result);
//                return;
//            }
            
            if(purchase == null)
            {
            
            }
            else
            {
            	mHelper.consumeAsync(purchase, mConsumeFinishedListener);
            	
            	
            }
            Log.d(TAG, "Purchase successful.");
//            SnowFilelog.write("OnIabPurchaseFinishedListener111111111\n");
//            SnowFilelog.write(purchase.getSku());
//            SnowFilelog.write(result.getResponse()+"");
//            SnowFilelog.write(purchase.getDeveloperPayload());
//            SnowFilelog.write("OnIabPurchaseFinishedListener2222222222\n");
//            sendInAppData(purchase.getSku(), 
//        			result.getResponse(), Integer.parseInt((purchase.getDeveloperPayload())));
//            SnowFilelog.write("OnIabPurchaseFinishedListener33333333333\n");
        }
    };

  
    @Override
    protected void onDestroy() {
    	if (mServiceConn != null) {
            unbindService(mServiceConn);
         }
        super.onDestroy();
        
    }
    protected String getPrice(String _sku) {
		ArrayList<String> skuList = new ArrayList<String>();
		StringTokenizer st = new StringTokenizer(_sku, "/");
		while (st.hasMoreTokens()) {
			skuList.add(st.nextToken());
		}
		
		Bundle querySkus = new Bundle();
		querySkus.putStringArrayList("ITEM_ID_LIST", skuList);
		Bundle skuDetails = null;
		try {
			skuDetails = mService.getSkuDetails(3, getPackageName(), "inapp",
					querySkus);
		}

		catch (RemoteException e) {
			// TODO Auto-generated catch block
			return new String("0");
			// e.printStackTrace();
		}
		JSONObject resultJson = new JSONObject();
		int response = skuDetails.getInt("RESPONSE_CODE");
		if (response == 0) {
			ArrayList<String> responseList = skuDetails
					.getStringArrayList("DETAILS_LIST");

			for (String thisResponse : responseList) {
				JSONObject object = null;
				try {
					object = new JSONObject(thisResponse);
					resultJson.put(object.getString("productId"), object.getString("price"));
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		return resultJson.toString();
	}
    protected void buyItem(final String itemID, final int delekey) {
		// SnowFilelog.write("buyItemtry");
		// mHelper.launchPurchaseFlow(this, itemID, RC_REQUEST,
		// mPurchaseFinishedListener);
		// SnowFilelog.write("buyItemtry End");

		handler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub

				{
					if (mSetupDone) {
						mHelper.launchPurchaseFlow(SportsWorldCup.this, itemID,
								RC_REQUEST, mPurchaseFinishedListener, ""
										+ delekey);
					} else {
						complain("network error, retry it.");

						mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
							public void onIabSetupFinished(IabResult result) {
								if (!result.isSuccess()) {
									// Oh noes, there was a problem.
									mSetupDone = false;
									complain("Network error, in-app billing");
									return;
								}
								mSetupDone = true;
							}
						});
					}

				} 

			}
		});

		// if( mManagedType != Managed.SUBSCRIPTION )
		// {
		// if( !mBillingService.requestPurchase(mSku, Consts.ITEM_TYPE_INAPP,
		// null) ) // payload = null
		// {
		// showDialog(DIALOG_BILLING_NOT_SUPPORTED_ID);
		// }
		// }
		// ///< ±¸µ¶
		// else
		// {
		// if( !mBillingService.requestPurchase(mSku,
		// Consts.ITEM_TYPE_SUBSCRIPTION, null) ) // payload = null
		// {
		// showDialog(DIALOG_SUBSCRIPTIONS_NOT_SUPPORTED_ID);
		// }
		// }
	}
}
