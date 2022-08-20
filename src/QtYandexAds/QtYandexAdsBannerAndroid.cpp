#include "QtYandexAdsBannerAndroid.h"
#include "qdebug.h"

bool QtYandexAdsBannerAndroid::m_isNativeContextPrepared = false;
QtYandexAdsBannerAndroid::BannerId QtYandexAdsBannerAndroid::m_curBannerId = 0;
std::shared_ptr<QHash<QtYandexAdsBannerAndroid::BannerId, std::shared_ptr<QtYandexAdsBannerAndroid>>> QtYandexAdsBannerAndroid::m_instances = std::make_shared<QHash<QtYandexAdsBannerAndroid::BannerId, std::shared_ptr<QtYandexAdsBannerAndroid>>>();

const AdError QtYandexAdsBannerAndroid::getAdErrorByErrorCode(ErrorCode code)
{
    if (code >= ErrorCode::EC_COUNT || code <= EC_INVALID)
        return AdError{QString{"Unknown error!"}, true};
    
    static const QHash<ErrorCode, AdError> errorCodeAdErrorHash = {
        {ErrorCode::EC_INTERNAL_ERROR,  AdError{QString{"Internal error!"},   true}},
        {ErrorCode::EC_INVALID_REQUEST, AdError{QString{"Invalid request!"},  true}},
        {ErrorCode::EC_NETWORK_ERROR,   AdError{QString{"Network error!"},    true}},
        {ErrorCode::EC_NO_FILL,         AdError{QString{"No ads available!"}, false}},
        {ErrorCode::EC_SYSTEM_ERROR,    AdError{QString{"System error!"},     true}}
    };
    
    return errorCodeAdErrorHash.value(code);
}

QtYandexAdsBannerAndroid::QtYandexAdsBannerAndroid(QObject *parent)
    : QtYandexAdsBannerInterface{parent}
{
    initializeBannerId();
}

bool QtYandexAdsBannerAndroid::initializeContext()
{
    QAndroidJniEnvironment env{};
    
    if (env->ExceptionCheck()) return false;
    
    if (!prepareNativeContext(env))
        return false;
    
    QAndroidJniObject::callStaticMethod<void>("org.test.QtYandexAds/QtYandexAdsActivity", "CreateInstance", "()V");
    
    // checking for SDK init status...
    
    auto endTime = QTime::currentTime().addMSecs(C_SDK_INIT_TIMEOUT_MS);
    bool isInit {false};
    
    while (QTime::currentTime() < endTime) {
        isInit = QAndroidJniObject::callStaticMethod<jboolean>("org.test.QtYandexAds/QtYandexAdsActivity", "CheckIsSDKInitialized", "()Z");
    
        if (isInit) {
            qInfo() << "QtYandexAdsBannerAndroid Context initialized";
            
            break;
        }
    }
    
    return isInit;
}

std::shared_ptr<QtYandexAdsBannerAndroid> QtYandexAdsBannerAndroid::generateInstance()
{
    qInfo() << "QtYandexAdsBannerAndroid::generateInstance() started";
    
    auto newInstance = std::make_shared<QtYandexAdsBannerAndroid>();
    
    m_instances->insert(newInstance->m_bannerId, newInstance); // FIXME: ILLEGAL
    
    jint insertedBannerId = QAndroidJniObject::callStaticMethod<jint>("org.test.QtYandexAds/QtYandexAdsActivity", "CreateNewBanner", "(I)I", static_cast<jint>(newInstance->m_bannerId));    
    
    qInfo() << "QtYandexAdsBannerAndroid::generateInstance() insertedBannerId: " << insertedBannerId;
    
    if (insertedBannerId < 0) {
        m_instances->remove(newInstance->m_bannerId);
        
        return std::shared_ptr<QtYandexAdsBannerAndroid>();
    }
    
    qInfo() << "QtYandexAdsBannerAndroid::generateInstance() ended";
    
    return newInstance;
}

std::shared_ptr<QtYandexAdsBannerAndroid> QtYandexAdsBannerAndroid::getInstanceById(const QtYandexAdsBannerAndroid::BannerId id)
{
    if (!m_instances->contains(id)) 
        return std::shared_ptr<QtYandexAdsBannerAndroid>(nullptr);
    
    return m_instances->value(id);
}

QtYandexAdsBannerAndroid::~QtYandexAdsBannerAndroid()
{
    // FIXME: ad disposing...
    
    m_instances->remove(m_bannerId);
    
    setVisible(false);
    
    if (isValid())
        QAndroidJniObject::callStaticMethod<void>("org.test.QtYandexAds/QtYandexAdsActivity", "ShutdownAdBanner", "(I)V", static_cast<jint>(m_bannerId));
}

//bool QtYandexAdsBannerAndroid::initialize()
//{
//    QAndroidJniEnvironment env{};
    
//    if (env->ExceptionCheck()) return false;
    
//    QAndroidJniObject::callStaticMethod<void>("org.test.QtYandexAds/QtYandexAdsActivity", "CreateInstance", "()V");
    
    
////    QAndroidJniObject activity = QtAndroid::androidActivity();
    
////    if (!activity.isValid()) return false;
    
////    m_jniActivity = std::make_unique<QAndroidJniObject>(activity);
    
    
    
////    jclass  jActivityClass {env->FindClass(C_YANDEX_ADS_ACTIVITY_CLASS_NAME)};
    
////    if (env->ExceptionCheck()) return false;
    
////    if (!m_isNativeContextPrepared) {
////        if (!prepareNativeContext(env, jActivityClass)) return false;
        
////        m_isNativeContextPrepared = true;
////    }
    
////    if (env->ExceptionCheck()) return false;
    
////    jmethodID jActivityClassInitializer{env->GetMethodID(jActivityClass, "InitializeAdBanner", "(I)V")};
    
////    if (env->ExceptionCheck()) return false;
    
////    jobject jActivityObject{env->NewObject(jActivityClass, jActivityClassInitializer, m_bannerId)};
    
////    if (env->ExceptionCheck()) return false;
    
////    m_jniActivity = std::make_unique<QAndroidJniObject>(jActivityObject);
    
//    return true;
//}

bool QtYandexAdsBannerAndroid::setUnitId(const QString &unitId)
{
    if (!isValid()) return false;
    if (!QtYandexAdsBannerInterface::setUnitId(unitId)) 
        return false;
    
    QAndroidJniObject jUnitIdObj{QAndroidJniObject::fromString(unitId)};
    
    QAndroidJniObject::callStaticMethod<void>("org.test.QtYandexAds/QtYandexAdsActivity", "SetAdBannerUnitId", "(Ljava/lang/String;)V", jUnitIdObj.object<jstring>());

    return true;
}

const QString &QtYandexAdsBannerAndroid::unitId() const
{
    return m_unitId;
}

bool QtYandexAdsBannerAndroid::setSize(Sizes size)
{
    if (!isValid()) return false;
    if (!QtYandexAdsBannerInterface::setSize(size)) 
        return false;
    
    QAndroidJniObject::callStaticMethod<void>("org.test.QtYandexAds/QtYandexAdsActivity", "SetAdBannerSize", "(II)V", static_cast<jint>(m_bannerId), static_cast<jint>(size));    

    return true;
}

QtYandexAdsBannerInterface::Sizes QtYandexAdsBannerAndroid::size() const
{
    return m_size;
}

QSize QtYandexAdsBannerAndroid::sizeInPixels()
{
    int width {QAndroidJniObject::callStaticMethod<jint>("org.test.QtYandexAds/QtYandexAdsActivity", "GetAdBannerWidth", "(I)I", static_cast<jint>(m_bannerId))};    
    int height{QAndroidJniObject::callStaticMethod<jint>("org.test.QtYandexAds/QtYandexAdsActivity", "GetAdBannerHeight", "(I)I", static_cast<jint>(m_bannerId))};
   
    return QSize(width, height);
}

bool QtYandexAdsBannerAndroid::setPosition(const QPoint &position)
{
    if (!isValid()) return false;
    if (!QtYandexAdsBannerInterface::setPosition(position))
        return false;
    
    QAndroidJniObject::callStaticMethod<void>("org.test.QtYandexAds/QtYandexAdsActivity", "SetAdBannerPosition", "(III)V", m_bannerId, position.x(), position.y());
    
    return QtYandexAdsBannerInterface::setPosition(position);
}

const QPoint &QtYandexAdsBannerAndroid::position() const
{
    return m_position;
}

bool QtYandexAdsBannerAndroid::setVisible(bool isVisible)
{
    if (!isValid()) return false;
   
    if (isVisible)
        QAndroidJniObject::callStaticMethod<void>("org.test.QtYandexAds/QtYandexAdsActivity", "ShowAdBanner", "(I)V", static_cast<jint>(m_bannerId));            
    else
        QAndroidJniObject::callStaticMethod<void>("org.test.QtYandexAds/QtYandexAdsActivity", "HideAdBanner", "(I)V", static_cast<jint>(m_bannerId));                    
    
    return true;
}

bool QtYandexAdsBannerAndroid::visible()
{
    if (!isValid()) return false;
    
    bool isVisible = QAndroidJniObject::callStaticMethod<jboolean>("org.test.QtYandexAds/QtYandexAdsActivity", "IsAdBannerVisible", "(I)Z", m_bannerId);
    
    return isVisible;
}

bool QtYandexAdsBannerAndroid::isLoaded()
{
    if (!isValid()) return false;
    
    bool isLoaded = QAndroidJniObject::callStaticMethod<jboolean>("org.test.QtYandexAds/QtYandexAdsActivity", "IsAdBannerLoaded", "(I)Z", m_bannerId);
    
    return isLoaded;
}

QtYandexAdsBannerAndroid::BannerId QtYandexAdsBannerAndroid::getBannerId() const
{
    return m_bannerId;
}

bool QtYandexAdsBannerAndroid::prepareNativeContext(QAndroidJniEnvironment &env)
{
    if (env->ExceptionCheck()) return false;
    
    JNINativeMethod methods[] {{"onBannerLoaded",     "(I)V",                   reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerLoaded)},
                               {"onBannerLoading",    "(I)V",                   reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerLoading)},
                               {"onBannerClicked",    "(I)V",                   reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerClicked)},
                               {"onBannerLoadFail",   "(II)V",                  reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerLoadFail)}/*,
                               {"onBannerImpression", "(Ljava/lang/String;)V", reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerImpression)}*/};
    
    jclass jActivityClass = env->FindClass("org/test/QtYandexAds/QtYandexAdsActivity");
    
    if (env->ExceptionCheck()) return false;
    
    env->RegisterNatives(jActivityClass,
                         methods,
                         sizeof(methods) / sizeof(methods[0]));
    
    return !(env->ExceptionCheck());
}

void QtYandexAdsBannerAndroid::processBannerLoading(JNIEnv *env, jobject thiz, const jint bannerId)
{
    qInfo() << "QtYandexAdsBannerAndroid::processBannerLoading() started";
    
    auto curBannerInstance = getInstanceByJavaObjectIdWithCheck(env, thiz, bannerId);
    
    emit curBannerInstance->loading();
}

void QtYandexAdsBannerAndroid::processBannerLoaded(JNIEnv *env, jobject thiz, const jint bannerId)
{
    auto curBannerInstance = getInstanceByJavaObjectIdWithCheck(env, thiz, bannerId);
    
    emit curBannerInstance->loaded();
}

void QtYandexAdsBannerAndroid::processBannerClosed(JNIEnv *env, jobject thiz, const jint bannerId)
{
    auto curBannerInstance = getInstanceByJavaObjectIdWithCheck(env, thiz, bannerId);
    
    emit curBannerInstance->closed();
}

void QtYandexAdsBannerAndroid::processBannerClicked(JNIEnv *env, jobject thiz, const jint bannerId)
{
    auto curBannerInstance = getInstanceByJavaObjectIdWithCheck(env, thiz, bannerId);
    
    emit curBannerInstance->clicked();
}

void QtYandexAdsBannerAndroid::processBannerLoadFail(JNIEnv *env, jobject thiz, const jint bannerId, const jint rawErrorCode)
{
    auto curBannerInstance = getInstanceByJavaObjectIdWithCheck(env, thiz, bannerId);
    
    emit curBannerInstance->errorOccured(getAdErrorByErrorCode(static_cast<ErrorCode>(rawErrorCode)));
}

void QtYandexAdsBannerAndroid::processBannerImpression(JNIEnv *env, jobject thiz, const jint bannerId, const jstring rawImpressionJsonData)
{
    auto curBannerInstance = getInstanceByJavaObjectIdWithCheck(env, thiz, bannerId);
    
    // FIXME: ad impression processing case...
}

void QtYandexAdsBannerAndroid::initializeBannerId()
{
    m_bannerId = m_curBannerId;
    
    ++m_curBannerId;
}

std::shared_ptr<QtYandexAdsBannerAndroid> QtYandexAdsBannerAndroid::getInstanceByJavaObjectIdWithCheck(JNIEnv *env, jobject thiz, const jint bannerId)
{
    if (bannerId < 0) return std::shared_ptr<QtYandexAdsBannerAndroid>(nullptr);
    if (env->ExceptionCheck()) {
        // FIXME: exception case processing...
        
        throw QString{"JNI exception was occured!"};
    }
    
    auto curBannerInstance = getInstanceById(bannerId);
    
    if (!curBannerInstance.get()) {
        // FIXME: invalid ptr case processing...
        
        throw QString{"Banner instance can't be found!"};
    }
    
    return curBannerInstance;
}

bool QtYandexAdsBannerAndroid::isValid() const
{
    return (true);
}
