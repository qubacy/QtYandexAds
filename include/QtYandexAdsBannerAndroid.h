#ifndef QTYANDEXADSBANNERANDROID_H
#define QTYANDEXADSBANNERANDROID_H

#include "QtYandexAds_global.h"

#include <QtAndroid>
#include <QAndroidIntent>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>

#include "QtYandexAdsBannerInterface.h"

class QTYANDEXADSSHARED_EXPORT QtYandexAdsBannerAndroid : public QtYandexAdsBannerInterface
{
    Q_OBJECT
    
    constexpr static const char* C_YANDEX_ADS_ACTIVITY_CLASS_NAME = "QtYandexAdsActivity";
    
    enum ErrorCode : jint {
        EC_INVALID = 0,
        EC_INTERNAL_ERROR,
        EC_INVALID_REQUEST,
        EC_NETWORK_ERROR,
        EC_NO_FILL,
        EC_SYSTEM_ERROR,
        EC_COUNT
    };
    
    static const AdError getAdErrorByErrorCode(ErrorCode code);
    
public:
    explicit QtYandexAdsBannerAndroid(QObject *parent = nullptr);
    
public:
    using BannerId = jint;
    
    static bool initializeContext();
    
    static std::shared_ptr<QtYandexAdsBannerAndroid> generateInstance();
    static std::shared_ptr<QtYandexAdsBannerAndroid> getInstanceById (const BannerId id);
    
    virtual ~QtYandexAdsBannerAndroid() override;
    
//    virtual bool initialize() override;
    
    virtual bool setUnitId(const QString &unitId) override;
    virtual const QString &unitId() const override;
    
    virtual bool setSize(Sizes size) override;
    virtual Sizes size() const override;
    virtual QSize sizeInPixels() override;
    
    virtual bool setPosition(const QPoint &position) override;
    virtual const QPoint &position() const override;
    
    virtual bool setVisible(bool isVisible) override;
    virtual bool visible() override;
    
    virtual bool isLoaded() override;
    
    BannerId getBannerId() const;
    
private:
    bool prepareNativeContext(QAndroidJniEnvironment &env,
                              const jclass jActivityClass);
    
    static void processBannerLoading   (JNIEnv *env, jobject thiz, const jint bannerId);
    static void processBannerLoaded    (JNIEnv *env, jobject thiz, const jint bannerId);
    static void processBannerClosed    (JNIEnv *env, jobject thiz, const jint bannerId);
    static void processBannerClicked   (JNIEnv *env, jobject thiz, const jint bannerId);
    static void processBannerLoadFail  (JNIEnv *env, jobject thiz, const jint bannerId, const jint rawErrorCode);
    static void processBannerImpression(JNIEnv *env, jobject thiz, const jint bannerId, const jstring rawImpressionJsonData);
    
    void initializeBannerId();
    
    static std::shared_ptr<QtYandexAdsBannerAndroid> getInstanceByJavaObjectIdWithCheck(JNIEnv *env, jobject thiz, const jint bannerId);
    
    bool isValid() const;
        
private:
    BannerId m_bannerId;
    
    static bool     m_isNativeContextPrepared;
    static BannerId m_curBannerId;
    static std::shared_ptr<QHash<BannerId, std::shared_ptr<QtYandexAdsBannerAndroid>>> m_instances;
};

#endif // QTYANDEXADSBANNERANDROID_H
