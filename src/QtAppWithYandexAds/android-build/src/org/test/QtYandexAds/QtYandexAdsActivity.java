package org.test.QtYandexAds;

import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.ViewGroup;
import android.util.Log;
import android.graphics.Rect;
import org.qtproject.qt5.android.bindings.QtActivity;
import org.qtproject.qt5.android.bindings.QtApplication;
import java.util.ArrayList;
import java.util.Queue;
import java.util.LinkedList;
import android.widget.FrameLayout;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import com.yandex.mobile.ads.common.InitializationListener;
import com.yandex.mobile.ads.common.MobileAds;
import com.yandex.mobile.ads.instream.MobileInstreamAds;
import com.yandex.mobile.ads.banner.AdSize;
import com.yandex.mobile.ads.banner.BannerAdEventListener;
import com.yandex.mobile.ads.banner.BannerAdView;
import com.yandex.mobile.ads.common.AdRequest;
import com.yandex.mobile.ads.common.AdRequestError;
import com.yandex.mobile.ads.common.ImpressionData;

public class QtYandexAdsActivity extends QtActivity {
    private static final String  YANDEX_MOBILE_ADS_TAG                = "YandexMobileAds";
    private static final boolean INSTREAM_AD_GROUP_PRELOADING_ENABLED = true;

    private String m_BannerAdUnitId = "R-M-DEMO-320x50";

    private ArrayList<QtYandexAdsBanner> m_BannerList = new ArrayList<QtYandexAdsBanner>();

    private static QtYandexAdsActivity m_Instance;
    
    private static boolean m_IsSDKInitialized = false;

    public static boolean CheckIsSDKInitialized() {
        return m_IsSDKInitialized;
    }

    public static void CreateInstance() {
        if (m_Instance == null) {
            Log.d(YANDEX_MOBILE_ADS_TAG, "Activity instance creation");
            
            m_Instance = new QtYandexAdsActivity();
        }
    }

    public static int CreateNewBanner(int bannerId) {
        if (bannerId < 0)       return -1;
        if (m_Instance == null) return -1;

        m_Instance.m_BannerList.add(bannerId, new QtYandexAdsBanner(m_Instance, bannerId));

        Log.d(YANDEX_MOBILE_ADS_TAG, "Banner inserted into m_BannerList");
        
        QtYandexAdsBanner newBanner = m_Instance.m_BannerList.get(bannerId);
    
        Log.d(YANDEX_MOBILE_ADS_TAG, "Banner creation ended");

        return bannerId;
    }

    public static void DeleteBanner(int bannerId) {
        if (m_Instance == null) return;
        if (bannerId < 0 || m_Instance.m_BannerList.size() <= bannerId)
            return;
        
        AtomicBoolean isOnWaitingForUiThread = new AtomicBoolean(true);
        
        m_Instance.runOnUiThread(new Runnable() {
            public void run() {
                QtYandexAdsBanner newBanner = m_Instance.m_BannerList.get(bannerId);
                
                if (newBanner == null) return;
                
                newBanner.Destroy();
                m_Instance.m_BannerList.remove(bannerId);
                
                isOnWaitingForUiThread.set(false);
            }
        });
     
        while (isOnWaitingForUiThread.get()) { }
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        m_Instance = this;
    
        Log.d(YANDEX_MOBILE_ADS_TAG, "SDK initialization start");

        MobileInstreamAds.setAdGroupPreloading(INSTREAM_AD_GROUP_PRELOADING_ENABLED);

        MobileAds.initialize(this, new InitializationListener() {
            @Override
            public void onInitializationCompleted() {
                Log.d(YANDEX_MOBILE_ADS_TAG, "SDK initialized");
                
                m_IsSDKInitialized = true;
            }
        });
    }

    private int GetStatusBarHeight() {
        
        AtomicBoolean isOnWaitingForUiThread = new AtomicBoolean(true);
        AtomicInteger result                 = new AtomicInteger(-1);
        
        runOnUiThread(new Runnable() {
            public void run() {
                Rect rectangle = new Rect();
                View decorView = GetMainDecorView();
        
                if (decorView == null) {
                    isOnWaitingForUiThread.set(false);
                    
                    return;
                }
        
                decorView.getWindowVisibleDisplayFrame(rectangle);
        
                //int statusBarHeight = rectangle.top;
                int contentViewTop  = getWindow().findViewById(Window.ID_ANDROID_CONTENT).getTop();
                //int titleBarHeight  = contentViewTop - statusBarHeight;
                
                //Log.d(YANDEX_MOBILE_ADS_TAG, statusBarHeight + ";" + contentViewTop + ";" + titleBarHeight);
        
                //result.set(titleBarHeight);
                result.set(contentViewTop);
                
                isOnWaitingForUiThread.set(false);
            }
        });
            
        while (isOnWaitingForUiThread.get()) { }
        
        return result.get();
    }

    private View GetMainDecorView() {
        // Qt thing to work with (instead of simple layout creation);

        Window mainWindow = getWindow();

        if (mainWindow == null) return null;

        return mainWindow.getDecorView();
    }

    private ViewGroup GetMainLayout() {
        View decorView = GetMainDecorView();

        if (decorView == null) return null;

        View rootView = decorView.getRootView();

        if (rootView == null) return null;

        if (rootView instanceof ViewGroup) {
            int statusBarHeight = GetStatusBarHeight();

            if (statusBarHeight < 0) return null;
            
            return ((ViewGroup) rootView);
        }
    
        return null;
    }

    public boolean RemoveBannerFromLayout(QtYandexAdsBanner banner) {
        if (banner == null) return false;
        
        View bannerView = banner.GetBannerView();
        
        if (bannerView == null) return false;
        
        AtomicBoolean isOnWaitingForUiThread = new AtomicBoolean(true);
        AtomicBoolean result = new AtomicBoolean(false);
        
        Log.d(YANDEX_MOBILE_ADS_TAG, "Banner layout RESET started");
        
        runOnUiThread(new Runnable() {
            public void run() {
                ViewGroup viewGroup = GetMainLayout();
                
                if (viewGroup == null) {
                    isOnWaitingForUiThread.set(false);
                    
                    return;
                }
            
                viewGroup.removeView(bannerView);
                
                result.set(true);
                isOnWaitingForUiThread.set(false);
            }
        });
                
        while (isOnWaitingForUiThread.get()) { }

        return result.get();
    }

    public boolean SetupBannerLayout(QtYandexAdsBanner banner, boolean isInitial) {
        Log.d(YANDEX_MOBILE_ADS_TAG, "Banner layout setup started");
        
        if (banner == null) return false;
        
        View bannerView = banner.GetBannerView();
        
        if (bannerView == null) return false;

        AtomicBoolean isOnWaitingForUiThread = new AtomicBoolean(true);
        AtomicBoolean result = new AtomicBoolean(false);
        
        runOnUiThread(new Runnable() {
            public void run() {
                ViewGroup viewGroup = GetMainLayout();
                
                if (viewGroup == null) {
                    isOnWaitingForUiThread.set(false);
                    
                    return;
                }
            
                if (isInitial) {
                    if (!banner.SetAdBannerPosition(banner.GetAdBannerX(), GetStatusBarHeight())) {
                        isOnWaitingForUiThread.set(false);
                        
                        return;
                    }
                }
            
                ViewGroup.LayoutParams layoutParams = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                    ViewGroup.LayoutParams.WRAP_CONTENT);
                
                bannerView.setLayoutParams(layoutParams);
                
                Log.d(YANDEX_MOBILE_ADS_TAG, "SetupBannerLayout(): banner about to be shown " + bannerView.getWidth());                               
                
                viewGroup.addView(bannerView);
                
                Log.d(YANDEX_MOBILE_ADS_TAG, "SetupBannerLayout(): banner pos: " + bannerView.getX() + ":" + bannerView.getY() + "; size: " + bannerView.getWidth() + ":" + bannerView.getHeight());  
                
                banner.ShowAdBanner();
                
                Log.d(YANDEX_MOBILE_ADS_TAG, "SetupBannerLayout(): banner shown");            
            
                result.set(true);
                isOnWaitingForUiThread.set(false);
            }
        });
    
        while (isOnWaitingForUiThread.get()) { }

        return result.get();
    }

    private static QtYandexAdsBanner GetAdBannerById(final int bannerId) {
        if (m_Instance == null) return null;
        if (m_Instance.m_BannerList.size() <= bannerId || bannerId < 0) return null;

        return m_Instance.m_BannerList.get(bannerId);
    }

    public static void SetAdBannerUnitId(final String adId) {
        if (m_Instance == null || adId.length() <= 0) return;
        
        Log.d(YANDEX_MOBILE_ADS_TAG, "SetAdBannerUnitId(): adId = " + adId);                    

        m_Instance.m_BannerAdUnitId = adId;

        for (int i = 0; i < m_Instance.m_BannerList.size(); ++i) {
            QtYandexAdsBanner curBanner = m_Instance.m_BannerList.get(i);

            if (curBanner == null) return;

            if (!curBanner.ProcessNewAdBannerUnitId()) {
                onBannerLoadFail(curBanner.GetBannerId(), AdRequestError.Code.SYSTEM_ERROR);
            }
        }
    }

    public static String GetBannerAdUnitId() {
        if (m_Instance == null) return "";

        return m_Instance.m_BannerAdUnitId;
    }

    public static void SetAdBannerSize(final int bannerId, final int width, final int height) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);

        if (curBanner == null) return;

        if (!curBanner.SetAdBannerSize(width, height)) {
            // FIXME: HANDLE ERROR CASE
            
            onBannerLoadFail(curBanner.GetBannerId(), AdRequestError.Code.SYSTEM_ERROR);            
        }
    }

    public static int[] GetAdBannerSize(final int bannerId) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);
    
        if (curBanner == null) return null;
        
        int[] intArray = new int[]{curBanner.GetAdBannerWidth(), curBanner.GetAdBannerHeight()};
    
        return intArray;
    }

    public static void SetAdBannerPosition(final int bannerId, final int x, final int y) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);

        if (curBanner == null) return;

        if (!curBanner.SetAdBannerPosition(x, y)) {
            // FIXME: HANDLE ERROR CASE
        }
    }

    public static int[] GetAdBannerPosition(final int bannerId) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);

        if (curBanner == null) return null;
        
        int[] intArray = new int[]{curBanner.GetAdBannerX(), curBanner.GetAdBannerY()};
    
        return intArray;
    }

    public static boolean IsAdBannerLoaded(final int bannerId) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);

        if (curBanner == null) return false;

        return curBanner.IsAdBannerLoaded();
    }

    public static boolean IsAdBannerVisible(final int bannerId) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);

        if (curBanner == null) return false;

        return curBanner.IsAdBannerVisible();
    }

    public static int GetAdBannerWidth(final int bannerId) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);

        if (curBanner == null) return -1;

        return curBanner.GetAdBannerWidth();
    }

    public static int GetAdBannerHeight(final int bannerId) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);

        if (curBanner == null) return -1;

        return curBanner.GetAdBannerHeight();
    }

    public static void ShutdownAdBanner(final int bannerId) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);

        if (curBanner == null) return;

        curBanner.ShutdownAdBanner();
    }

    public static void ShowAdBanner(final int bannerId) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);

        if (curBanner == null) return;

        curBanner.ShowAdBanner();
    }

    public static void HideAdBanner(final int bannerId) {
        QtYandexAdsBanner curBanner = GetAdBannerById(bannerId);

        if (curBanner == null) return;

        curBanner.HideAdBanner();
    }

    public static native void onBannerLoaded(int bannerId);
    public static native void onBannerLoading(int bannerId);
    public static native void onBannerClicked(int bannerId);
    public static native void onBannerLoadFail(int bannerId, int errorCode);
//    private static native void onBannerImpression(String rawImpressionJsonData);
}
