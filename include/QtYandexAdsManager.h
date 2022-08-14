#ifndef QTYANDEXADSMANAGER_H
#define QTYANDEXADSMANAGER_H

#include "QtYandexAds_global.h"

#include "QtYandexAdsBannerAndroid.h"

class QTYANDEXADSSHARED_EXPORT QtYandexAdsManager
{
public:
    QtYandexAdsManager() = delete;
    
    static std::shared_ptr<QtYandexAdsBannerInterface> generateAdBanner();
    
private:
    static std::shared_ptr<QtYandexAdsBannerAndroid> generateAdBannerAndroid();
};

#endif // QTYANDEXADSMANAGER_H
