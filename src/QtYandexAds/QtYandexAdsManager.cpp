#include "QtYandexAdsManager.h"

std::shared_ptr<QtYandexAdsBannerInterface> QtYandexAdsManager::generateAdBanner()
{
    std::shared_ptr<QtYandexAdsBannerInterface> ad{nullptr};
    
#ifdef Q_OS_ANDROID
    ad = generateAdBannerAndroid();
    
#elif Q_OS_IOS
    
#else
    
#endif
    return ad;
}

std::shared_ptr<QtYandexAdsBannerAndroid> QtYandexAdsManager::generateAdBannerAndroid()
{
    std::shared_ptr<QtYandexAdsBannerAndroid> adAndroid{std::make_shared<QtYandexAdsBannerAndroid>()};

    if (!adAndroid->initialize())
        return std::shared_ptr<QtYandexAdsBannerAndroid>{nullptr};
    
    return adAndroid;
}
