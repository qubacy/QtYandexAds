package org.test.QtYandexAds;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;

import com.yandex.mobile.ads.banner.AdSize;
import com.yandex.mobile.ads.banner.BannerAdEventListener;
import com.yandex.mobile.ads.banner.BannerAdView;
import com.yandex.mobile.ads.common.AdRequest;
import com.yandex.mobile.ads.common.AdRequestError;
import com.yandex.mobile.ads.common.ImpressionData;

public class QtYandexAdsBanner {
    public static int DEFAULT_AD_BANNER_X = 0;

    private int m_BannerId;

    private AdSize m_BannerAdSize   = AdSize.BANNER_320x50;

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
                m_BannerActivity.onBannerClicked(m_BannerId);
            }

            @Override
            public void onImpression(ImpressionData impressionData) {

            }

            @Override
            public void onAdFailedToLoad(AdRequestError adRequestError) {
                m_BannerActivity.onBannerLoadFail(m_BannerId, adRequestError.getCode());
            }

            @Override
            public void onLeftApplication() {
                ShutdownAdBanner();
            }

            @Override
            public void onReturnedToApplication() {
                InitializeAdBanner(context);
                RequestAdBannerContent();
            }
        });
    }

    private void RequestAdBannerContent() {
        m_BannerActivity.runOnUiThread(new Runnable() {
            public void run() {
                if (m_BannerView == null) return;

                m_BannerActivity.onBannerLoading(m_BannerId);

                final AdRequest adRequest = new AdRequest.Builder().build();

                m_BannerView.loadAd(adRequest);
            }
        });
    }

    public QtYandexAdsBanner(@NonNull Context context, @NonNull int id) {
        m_BannerId = id;

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
        return (m_BannerView.getVisibility() == View.VISIBLE);
    }

    public int GetAdBannerWidth() {
        return m_BannerView.getWidth();
    }

    public int GetAdBannerHeight() {
        return m_BannerView.getHeight();
    }

    public void ProcessNewAdBannerUnitId() {
        m_BannerView.setAdUnitId(m_BannerActivity.GetBannerAdUnitId());

        RequestAdBannerContent();
    }

    public boolean SetAdBannerSize(final int size) {
        if (m_BannerView == null) return false;

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

        m_BannerView.setAdSize(adSize);

        RequestAdBannerContent();

        return true;
    }

    public boolean SetAdBannerPosition(final int x, final int y) {
        if (m_BannerView == null) return false;

        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);

        m_BannerView.setLayoutParams(layoutParams);

        m_BannerX = x;
        m_BannerY = y;

        m_BannerView.setX(x);
        m_BannerView.setY(y);

        return true;
    }

    public void ShutdownAdBanner() {
        if (m_BannerView == null) return;

        m_BannerView.destroy();

        m_IsBannerLoaded = false;
    }

    public void ShowAdBanner() {
        if (m_BannerView == null) return;

        m_BannerView.setVisibility(View.VISIBLE);
    }

    public void HideAdBanner() {
        if (m_BannerView == null) return;

        m_BannerView.setVisibility(View.GONE);
    }
}

