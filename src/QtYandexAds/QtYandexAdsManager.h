#ifndef QTYANDEXADSMANAGER_H
#define QTYANDEXADSMANAGER_H

#include "QtYandexAdsBannerAndroid.h"

class QtYandexAdsManager
{
public:
    QtYandexAdsManager() = delete;
    
    static std::shared_ptr<QtYandexAdsBannerInterface> generateAdBanner();
    
private:
    static std::shared_ptr<QtYandexAdsBannerAndroid> generateAdBannerAndroid();
};

#endif // QTYANDEXADSMANAGER_H
