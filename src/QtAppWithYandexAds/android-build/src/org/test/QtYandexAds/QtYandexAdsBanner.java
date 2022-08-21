package org.test.QtYandexAds;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.util.Log;
import java.util.concurrent.atomic.AtomicBoolean;

import androidx.annotation.NonNull;

import com.yandex.mobile.ads.banner.AdSize;
import com.yandex.mobile.ads.banner.BannerAdEventListener;
import com.yandex.mobile.ads.banner.BannerAdView;
import com.yandex.mobile.ads.common.AdRequest;
import com.yandex.mobile.ads.common.AdRequest.Builder;
import com.yandex.mobile.ads.common.AdRequestError;
import com.yandex.mobile.ads.common.ImpressionData;

public class QtYandexAdsBanner {
    public static int DEFAULT_AD_BANNER_X = 0;

    private int m_BannerId;

    private AdSize m_BannerAdSize = AdSize.BANNER_320x50;

    private int m_BannerX = DEFAULT_AD_BANNER_X;
    private int m_BannerY;

    private BannerAdView m_BannerView;

    private boolean m_IsBannerLoaded = false;

    private QtYandexAdsActivity m_BannerActivity;

    private void InitializeAdBanner(@NonNull Context context) {
        m_BannerView = new BannerAdView(context);

        String curUnitId = m_BannerActivity.GetBannerAdUnitId();

        if (curUnitId.length() <= 0) return;

        m_BannerView.setAdUnitId(curUnitId);
        m_BannerView.setAdSize(m_BannerAdSize);

        m_BannerView.setX(m_BannerX);
        m_BannerView.setY(m_BannerY);

        m_BannerView.setBannerAdEventListener(new BannerAdEventListener() {
            @Override
            public void onAdLoaded() {
                m_IsBannerLoaded = true;

                m_BannerActivity.onBannerLoaded(m_BannerId);
            }

            @Override
            public void onAdClicked() {
                Log.d("QtYandexAdsBanner", "onAdClicked()");
                
                m_BannerActivity.onBannerClicked(m_BannerId);
            }

            @Override
            public void onImpression(ImpressionData impressionData) {

            }

            @Override
            public void onAdFailedToLoad(AdRequestError adRequestError) {
                Log.d("QtYandexAdsBanner", "onAdFailedToLoad()");
                
                m_BannerActivity.onBannerLoadFail(m_BannerId, adRequestError.getCode());
            }

            @Override
            public void onLeftApplication() {
                Log.d("QtYandexAdsBanner", "onLeftApplication()");
                
                ShutdownAdBanner();
            }

            @Override
            public void onReturnedToApplication() {
                Log.d("QtYandexAdsBanner", "onReturnedToApplication()");
                
                //InitializeAdBanner(context);
                RequestAdBannerContent();
            }
        });
    
        Log.d("QtYandexAdsBanner", "Banner initialization ended");
    }

    private void RequestAdBannerContent() {
        AtomicBoolean isOnWaitingForUiThread = new AtomicBoolean(true);
        
        m_BannerActivity.runOnUiThread(new Runnable() {
            public void run() {
                if (m_BannerView == null) return;
                
                m_BannerActivity.onBannerLoading(m_BannerId);
                
                Log.d("QtYandexAdsBanner", "Banner loading started");

                m_BannerView.loadAd(new AdRequest.Builder().build());
                
                Log.d("QtYandexAdsBanner", "Banner loading ended");
                
                isOnWaitingForUiThread.set(false);
            }
        });
    
        while (isOnWaitingForUiThread.get()) { }
    }

    public QtYandexAdsBanner(@NonNull QtYandexAdsActivity context, @NonNull int id) {
        Log.d("QtYandexAdsBanner", "New banner creation");
        
        m_BannerId = id;
        m_BannerActivity = context;

        InitializeAdBanner(context);
        RequestAdBannerContent();
    }

    public int GetBannerId() {
        return m_BannerId;
    }

    public View GetBannerView() {
        return m_BannerView;
    }

    public boolean IsAdBannerLoaded() {
        return m_IsBannerLoaded;
    }

    public boolean IsAdBannerVisible() {
        if (m_BannerView == null) return false;
        
        return (m_BannerView.getVisibility() == View.VISIBLE);
    }

    public int GetAdBannerWidth() {
        if (m_BannerView == null) return -1;
        
        return m_BannerView.getWidth();
    }

    public int GetAdBannerHeight() {
        if (m_BannerView == null) return -1;
        
        return  m_BannerView.getHeight();
    }

    public int GetAdBannerX() {
        return (int)m_BannerView.getX();
    }

    public int GetAdBannerY() {
        return (int)m_BannerView.getY();
    }

    public void ProcessNewAdBannerUnitId() {
//        m_BannerView.setAdUnitId(m_BannerActivity.GetBannerAdUnitId());

        if (m_BannerView == null) return;

        AtomicBoolean isOnWaitingForUiThread = new AtomicBoolean(true);
        
        m_BannerActivity.runOnUiThread(new Runnable() {
            public void run() {
                Log.d("QtYandexAdsBanner", "ProcessNewAdBannerUnitId(). New unitId: " + m_BannerActivity.GetBannerAdUnitId());
                
                //m_BannerView.destroy();
        
                InitializeAdBanner(m_BannerActivity);
                RequestAdBannerContent();
                ShowAdBanner();
                
                isOnWaitingForUiThread.set(false);
            }
        });
    
        while (isOnWaitingForUiThread.get()) { }
            
    }

    public boolean SetAdBannerSize(final int size) {
        Log.d("QtYandexAdsBanner", "Banner size changing. Size: " + size);
        
        AtomicBoolean isOnWaitingForUiThread = new AtomicBoolean(true);
        AtomicBoolean result = new AtomicBoolean(false);
        
        m_BannerActivity.runOnUiThread(new Runnable() {
            public void run() {
                if (m_BannerView == null) return;
        
                AdSize adSize = AdSize.BANNER_320x50;
        
                // FIXME: decide banner ENUM storing issue
        
                switch (size) {
                    case 1: { adSize = AdSize.BANNER_320x50;  break; }
                    case 2: { adSize = AdSize.BANNER_320x100; break; }
                    case 3: { adSize = AdSize.BANNER_400x240; break; }
                    case 4: { adSize = AdSize.BANNER_728x90;  break; }
                    case 5: { adSize = AdSize.FULL_SCREEN;    break; }
                };
        
                m_BannerAdSize = adSize;
        
                //m_BannerView.setAdSize(adSize);
                
                //m_BannerView.destroy();
        
                InitializeAdBanner(m_BannerActivity);
                RequestAdBannerContent();
                ShowAdBanner();
                
                result.set(true);
                isOnWaitingForUiThread.set(false);
            }
        });
    
        while (isOnWaitingForUiThread.get()) { }

        return result.get();
    }

    public boolean SetAdBannerPosition(final int x, final int y) {
        Log.d("QtYandexAdsBanner", "Banner pos changing: " + x + ":" + y);
        
        if (m_BannerView == null) return false;

//        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
//                FrameLayout.LayoutParams.WRAP_CONTENT);

//        m_BannerView.setLayoutParams(layoutParams);

        m_BannerX = x;
        m_BannerY = y;

        m_BannerView.setX(x);
        m_BannerView.setY(y);

        return true;
    }

    public void ShutdownAdBanner() {
        Log.d("QtYandexAdsBanner", "Banner shutdown");
        
        if (m_BannerView == null) return;

        //m_BannerView.destroy();

        m_IsBannerLoaded = false;
    }

    public void ShowAdBanner() {
        if (m_BannerView == null) return;
        
        Log.d("QtYandexAdsBanner", "ShowAdBanner()");

        m_BannerView.setVisibility(View.VISIBLE);
    }

    public void HideAdBanner() {
        if (m_BannerView == null) return;
        
        Log.d("QtYandexAdsBanner", "HideAdBanner()");

        m_BannerView.setVisibility(View.GONE);
    }
}

