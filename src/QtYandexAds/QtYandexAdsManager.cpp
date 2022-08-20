#include "QtYandexAdsManager.h"
#include "qdebug.h"

std::shared_ptr<QtYandexAdsBannerInterface> QtYandexAdsManager::generateAdBanner()
{
    std::shared_ptr<QtYandexAdsBannerInterface> ad{nullptr};
    
#ifdef Q_OS_ANDROID
    ad = generateAdBannerAndroid();
    
#elif Q_OS_IOS
    
#else
    
#endif
    
//    if (!ad->setUnitId(QtYandexAdsBannerInterface::C_TEST_UNIT_ID))
//        return std::shared_ptr<QtYandexAdsBannerInterface>{nullptr};
    
    return ad;
}

std::shared_ptr<QtYandexAdsBannerAndroid> QtYandexAdsManager::generateAdBannerAndroid()
{
    if (!QtYandexAdsBannerAndroid::initializeContext())
        return std::shared_ptr<QtYandexAdsBannerAndroid>(nullptr);
    
    qInfo() << "Qt: " << "creating new banner start";
    
    // instance:
    
    std::shared_ptr<QtYandexAdsBannerAndroid> adAndroid{QtYandexAdsBannerAndroid::generateInstance()};

    if (!adAndroid.get())
        return adAndroid;
    
    return adAndroid;
}
