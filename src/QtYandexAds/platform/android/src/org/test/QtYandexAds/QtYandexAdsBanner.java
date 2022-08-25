package org.test.QtYandexAds;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.util.Log;
import java.util.concurrent.atomic.AtomicBoolean;
import android.util.DisplayMetrics;
import android.graphics.Point;

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

    private AdSize m_BannerAdSize = AdSize.flexibleSize(320, 50);

    private int m_BannerX = DEFAULT_AD_BANNER_X;
    private int m_BannerY;
    
    private int m_HorizontalAlignment = 0;
    private int m_VerticalAlignment = 0;

    private BannerAdView m_BannerView;

    private boolean m_IsBannerLoaded = false;

    private QtYandexAdsActivity m_BannerActivity;
    
    public int GetHorizontalAlignment() {
        return m_HorizontalAlignment;
    }

    public int GetVerticalAlignment() {
        return m_VerticalAlignment;
    }
    
    public void SetHorizontalAlignment(final int horizontalAlignment, final boolean isLandscape) {
        if (m_BannerActivity == null) return;
        
        Point actualAppPoint = m_BannerActivity.GetActualAppSize(isLandscape);
        
        if (actualAppPoint == null) {
            m_BannerActivity.onBannerLoadFail(m_BannerId, AdRequestError.Code.SYSTEM_ERROR);
            
            return;
        }
        
        int actualAppWidth = actualAppPoint.x;
        
        if (actualAppWidth < 0) {
            m_BannerActivity.onBannerLoadFail(m_BannerId, AdRequestError.Code.SYSTEM_ERROR);
            
            return;
        }
    
        boolean result = false;
        
        switch (horizontalAlignment) {
            case 1: {
                result = SetAdBannerPosition(0, GetAdBannerY());
                    
                break;
            } // right
            case 2: { 
                int midBannerX = actualAppWidth / 2 - GetAdBannerWidth() / 2;
                
                result = SetAdBannerPosition(midBannerX, GetAdBannerY());                
                
                break;
            } // center
            case 3: {
                int leftBannerX = actualAppWidth - GetAdBannerWidth();
                
                result = SetAdBannerPosition(leftBannerX, GetAdBannerY());                
                
                break;
            } // left
        }
        
        if (!result)
            m_BannerActivity.onBannerLoadFail(m_BannerId, AdRequestError.Code.SYSTEM_ERROR);
            
        m_HorizontalAlignment = horizontalAlignment;
    }

    public void SetVerticalAlignment(final int verticalAlignment, final boolean isLandscape) {
        if (m_BannerActivity == null) return;
        
        Point actualAppPoint = m_BannerActivity.GetActualAppSize(isLandscape);
        
        if (actualAppPoint == null) {
            m_BannerActivity.onBannerLoadFail(m_BannerId, AdRequestError.Code.SYSTEM_ERROR);
            
            return;
        }
        
        int actualAppHeight = actualAppPoint.y - GetAdBannerHeight();
        
        if (actualAppHeight < 0) {
            m_BannerActivity.onBannerLoadFail(m_BannerId, AdRequestError.Code.SYSTEM_ERROR);
            
            return;
        }
    
        boolean result = false;
        
        switch (verticalAlignment) {
            case 1: {
                result = SetAdBannerPosition(GetAdBannerX(), m_BannerActivity.GetStatusBarHeight());
                    
                break;
            } // top
            case 2: { 
                int midBannerY = actualAppHeight / 2 + GetAdBannerHeight() / 2 - m_BannerActivity.GetStatusBarHeight() / 2;
                
                result = SetAdBannerPosition(GetAdBannerX(), midBannerY);                
                
                break;
            } // center
            case 3: {
                result = SetAdBannerPosition(GetAdBannerX(), actualAppHeight);                
                
                break;
            } // bottom
        }
        
        if (!result)
            m_BannerActivity.onBannerLoadFail(m_BannerId, AdRequestError.Code.SYSTEM_ERROR);
            
        m_VerticalAlignment = verticalAlignment;
    }


    private void InitializeAdBanner(@NonNull Context context, boolean isInitial) {
        m_BannerView = new BannerAdView(context);

        String curUnitId = m_BannerActivity.GetBannerAdUnitId();

        if (curUnitId.length() <= 0) return;

        m_BannerView.setAdUnitId(curUnitId);
        m_BannerView.setAdSize(m_BannerAdSize);
        m_BannerView.setId(m_BannerId);
        
        Log.d("QtYandexAdsBanner", "InitializeAdBanner() banner size: " + m_BannerAdSize);

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
                Log.d("QtYandexAdsBanner", "onAdFailedToLoad() Code: " + adRequestError.getCode() + "; Msg: " + adRequestError.getDescription());
                
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
                
                RequestAdBannerContent();
            }
        });
    
        RequestAdBannerContent();
    
        // new banner layout setup:
    
        if (!m_BannerActivity.SetupBannerLayout(this, isInitial)) {
            m_BannerActivity.onBannerLoadFail(m_BannerId, AdRequestError.Code.SYSTEM_ERROR);
    
            return;
        }
    
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

        InitializeAdBanner(context, true);
    }

    public void Destroy() {
        if (m_BannerActivity != null) {
            m_BannerActivity.RemoveBannerFromLayout(this);
        }
    
        m_BannerView.destroy();
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
        if (m_BannerView == null)     return -1;
        if (m_BannerActivity == null) return -1;
                
        int density = (int)(m_BannerActivity.getResources().getDisplayMetrics().density);
        int width   = (int)(m_BannerAdSize.getWidth(m_BannerActivity) * density);
        
        Log.d("QtYandexAdsBanner", "Actual banner width: " + width);
        
        return width;
    }

    public int GetAdBannerHeight() {
        if (m_BannerView == null)     return -1;
        if (m_BannerActivity == null) return -1;
        
        int density = (int)(m_BannerActivity.getResources().getDisplayMetrics().density);
        int height  = (int)(m_BannerAdSize.getHeight(m_BannerActivity) * density);
        
        Log.d("QtYandexAdsBanner", "Actual banner height: " + height);
        
        return height;
    }

    public int GetAdBannerX() {
        return (int)m_BannerView.getX();
    }

    public int GetAdBannerY() {
        return (int)m_BannerView.getY();
    }

    private boolean ResetBanner() {
        if (!m_BannerActivity.RemoveBannerFromLayout(this)) 
            return false;
        
        m_BannerView.destroy();
        InitializeAdBanner(m_BannerActivity, false);

        return true;
    }

    public boolean ProcessNewAdBannerUnitId() {
        if (m_BannerView == null) return false;

        Log.d("QtYandexAdsBanner", "ProcessNewAdBannerUnitId(). New unitId: " + m_BannerActivity.GetBannerAdUnitId());

        if (!ResetBanner()) return false;
        
        return true;
    }

    public boolean SetAdBannerSize(final int widthInPixels, final int heightInPixels) {
        Log.d("QtYandexAdsBanner", "Banner size changing. Size: " + widthInPixels + ":" + heightInPixels);
        
        if (widthInPixels <= 0 || heightInPixels <= 0) return false;
        if (m_BannerActivity == null) return false;
        if (m_BannerView == null) return false;
        
        int density = (int)(m_BannerActivity.getResources().getDisplayMetrics().density);
        int width   = (int)(widthInPixels / density);
        int height  = (int)(heightInPixels / density);

        m_BannerAdSize = AdSize.flexibleSize(width, height);

        if (!ResetBanner()) return false;
                
        return true;
    }

    public boolean SetAdBannerPosition(final int x, final int y) {
        Log.d("QtYandexAdsBanner", "Banner pos changing: " + x + ":" + y);
        
        if (m_BannerView == null) return false;

        m_BannerX = x;
        m_BannerY = y;

        m_BannerView.setX(x);
        m_BannerView.setY(y);

        return true;
    }

    public void ShutdownAdBanner() {
        Log.d("QtYandexAdsBanner", "Banner shutdown");
        
        if (m_BannerView == null) return;

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

