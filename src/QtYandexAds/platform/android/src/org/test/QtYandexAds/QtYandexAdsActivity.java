package org.test.QtYandexAds;

import com.yandex.mobile.ads.common.AdRequest;
import com.yandex.mobile.ads.common.AdRequestError;
import com.yandex.mobile.ads.common.ImpressionData;
import com.yandex.mobile.ads.banner.AdSize;
import com.yandex.mobile.ads.banner.BannerAdView;
import com.yandex.mobile.ads.banner.BannerAdEventListener;


import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.ViewGroup;
import android.util.Log;
import android.graphics.Rect;
import org.qtproject.qt5.android.bindings.QtActivity;
import org.qtproject.qt5.android.bindings.QtApplication;
import java.util.ArrayList;
import android.widget.FrameLayout;

public class QtYandexAdsActivity extends QtActivity {
    private ViewGroup m_ViewGroup;

    private BannerAdView m_BannerAdView = null;

    private boolean m_IsAdBannerShowed = false;
    private boolean m_IsAdBannerLoaded = false;

//    private ArrayList<String> m_TestDevices = new ArrayList<String>();

    private int m_AdBannerWidth = 0;
    private int m_AdBannerHeight = 0;
    private int m_StatusBarHeight = 0;

    private int m_ReadyToRequest = 0x00;

    private int GetStatusBarHeight() {
        Rect rectangle = new Rect();
        Window window = getWindow();

        window.getDecorView().getWindowVisibleDisplayFrame(rectangle);

        int statusBarHeight = rectangle.top;
        int contentViewTop  = window.findViewById(Window.ID_ANDROID_CONTENT).getTop();
        int titleBarHeight  = contentViewTop - statusBarHeight;

        return titleBarHeight;
    }

    public void SetAdBannerUnitId(final String adId) {
        runOnUiThread(new Runnable() {
            public void run() {
                m_BannerAdView.setAdUnitId(adId);

                m_ReadyToRequest |= 0x01;

                if (m_ReadyToRequest == 0x03 && !IsAdBannerLoaded())
                    RequestBanner();
            }
        });
    }

    public void SetAdBannerSize(final int size)
    {
        final QtYandexAdsActivity self = this;

        runOnUiThread(new Runnable() {
            public void run() {
                AdSize adSize;

                switch (size) {
                    case 0:  { adSize = AdSize.BANNER_320x100; break; }
                    case 1:  { adSize = AdSize.BANNER_320x50;  break; }
                    case 2:  { adSize = AdSize.BANNER_400x240; break; }
                    case 3:  { adSize = AdSize.BANNER_728x90;  break; }
                    case 4:  { adSize = AdSize.FULL_SCREEN;    break; }
                    default: { adSize = AdSize.BANNER_320x50;}
                };

                m_BannerAdView.setAdSize(adSize);

                m_AdBannerWidth  = adSize.getWidthInPixels(self);
                m_AdBannerHeight = adSize.getHeightInPixels(self);

                m_ReadyToRequest |= 0x02;

                if (m_ReadyToRequest == 0x03 && !IsAdBannerLoaded())
                    RequestBanner();
            }
        });
    }

    public void SetAdBannerPosition(final int x, final int y) {
        runOnUiThread(new Runnable() {
            public void run() {
                FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                                                                                     FrameLayout.LayoutParams.WRAP_CONTENT);
                m_BannerAdView.setLayoutParams(layoutParams);

                m_BannerAdView.setX(x);
                m_BannerAdView.setY(y);
            }
        });
    }

//    public void AddAdTestDevice(final String deviceId) {
//        runOnUiThread(new Runnable() {
//            public void run() {
//                m_TestDevices.add(deviceId);
//            }
//        });
//    }

    public boolean IsAdBannerShowed() {
        return m_IsAdBannerShowed && m_IsAdBannerLoaded;
    }

    public boolean IsAdBannerLoaded() {
        return m_IsAdBannerLoaded;
    }

    public int GetAdBannerWidth()
    {
        return m_AdBannerWidth;
    }

    public int GetAdBannerHeight()
    {
        return m_AdBannerHeight;
    }

    public void ShowAdBanner() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (IsAdBannerShowed()) return;

                if (m_ReadyToRequest == 0x03 && !IsAdBannerLoaded())
                    RequestBanner();

                m_BannerAdView.setVisibility(View.VISIBLE);
                m_IsAdBannerShowed = true;
            }
        });
    }

    private void RequestBanner() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                onBannerLoading();

                final AdRequest adRequest = new AdRequest.Builder().build();

                m_BannerAdView.loadAd(adRequest);
            }
        });
    }

    public void HideAdBanner() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (!IsAdBannerShowed()) return;

                m_BannerAdView.setVisibility(View.GONE);

                m_IsAdBannerShowed = false;
            }
        });
    }

    public void InitializeAdBanner() {
        final QtYandexAdsActivity self = this;

        runOnUiThread(new Runnable() {
            public void run() {
                if (m_BannerAdView != null) return;

                m_StatusBarHeight = GetStatusBarHeight();

                m_BannerAdView = new BannerAdView(self);

                m_BannerAdView.setAdSize(AdSize.BANNER_320x50); // FIXME: move to consts
                m_BannerAdView.setVisibility(View.GONE);

                View view = getWindow().getDecorView().getRootView();

                if (view instanceof ViewGroup) {
                    m_ViewGroup = (ViewGroup) view;

                    FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                            FrameLayout.LayoutParams.WRAP_CONTENT);

                    m_BannerAdView.setLayoutParams(layoutParams);
                    m_BannerAdView.setX(0);
                    m_BannerAdView.setY(m_StatusBarHeight);

                    m_ViewGroup.addView(m_BannerAdView);

                    m_BannerAdView.setBannerAdEventListener(new BannerAdEventListener() {
                        @Override
                        public void onAdLoaded() {
                            m_IsAdBannerLoaded = true;

                            onBannerLoaded();
                        }
                    
                        @Override
                        public void onAdFailedToLoad(AdRequestError adRequestError) {
                            m_IsAdBannerLoaded = false;
                            
                            onBannedLoadFail(adRequestError.getCode());
                        }
                    
                        @Override
                        public void onAdClicked() {
                            onBannerClicked();
                        }
                    
                        @Override
                        public void onLeftApplication() {
                            ShutdownAdBanner();
                        }
                    
                        @Override
                        public void onImpression(ImpressionData impressionData) {
                            // FIXME: define it
                            
                            onBannerImpression(impressionData.getRawData());
                        }
                    
                       @Override
                       public void onReturnedToApplication() {
                           // FIXME: define it
                           
                           ShowAdBanner();
                       }
                    });
                }
            }
        });
    }

    public void ShutdownAdBanner()
    {
        runOnUiThread(new Runnable() {
            public void run() {
                if (m_BannerAdView == null) return;

                m_ViewGroup.removeView(m_BannerAdView);

                m_IsAdBannerShowed = false;
                m_BannerAdView     = null;
            }
        });
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onPause() {
        super.onPause();
        
        // FIXME: does it matter?
        
//        if (m_BannerAdView != null)
//            m_BannerAdView.pause();
    }
    @Override
    public void onResume() {
        super.onResume();
        
        // FIXME: does it matter?
        
//        if (m_BannerAdView != null)
//            m_BannerAdView.resume();
    }
    @Override
    public void onDestroy() {
        super.onDestroy();
        
        if (m_BannerAdView != null)
            m_BannerAdView.destroy();
    }

    private static native void onBannerLoaded();
    private static native void onBannerLoading();
    private static native void onBannerClosed();
    private static native void onBannerClicked();
    private static native void onBannedLoadFail(int errorCode);
    private static native void onBannerImpression(String rawImpressionJsonData);
}
