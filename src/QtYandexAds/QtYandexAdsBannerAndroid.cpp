#include "QtYandexAdsBannerAndroid.h"

bool QtYandexAdsBannerAndroid::m_isNativeContextPrepared = false;
QtYandexAdsBannerAndroid::BannerId QtYandexAdsBannerAndroid::m_curBannerId = 0;
std::shared_ptr<QHash<QtYandexAdsBannerAndroid::BannerId, std::shared_ptr<QtYandexAdsBannerAndroid>>> QtYandexAdsBannerAndroid::m_instances = std::shared_ptr<QHash<QtYandexAdsBannerAndroid::BannerId, std::shared_ptr<QtYandexAdsBannerAndroid>>>();

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
    : QtYandexAdsBannerInterface{parent},
      m_jniActivity{nullptr}
{
    initializeBannerId();
}

std::shared_ptr<QtYandexAdsBannerAndroid> QtYandexAdsBannerAndroid::generateInstance()
{
    auto newInstance = std::make_shared<QtYandexAdsBannerAndroid>();
    
    m_instances->insert(newInstance->m_bannerId, newInstance);
    
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
}

bool QtYandexAdsBannerAndroid::initialize()
{
    QAndroidJniEnvironment env{};
    
    if (env->ExceptionCheck()) return false;
    
    jclass  jActivityClass {env->FindClass(C_YANDEX_ADS_ACTIVITY_CLASS_NAME)};
    
    if (env->ExceptionCheck()) return false;
    
    if (!m_isNativeContextPrepared) {
        if (!prepareNativeContext(env, jActivityClass)) return false;
        
        m_isNativeContextPrepared = true;
    }
    
    if (env->ExceptionCheck()) return false;
    
    jmethodID jActivityClassInitializer{env->GetMethodID(jActivityClass, "InitializeAdBanner", "(I)V")};
    
    if (env->ExceptionCheck()) return false;
    
    jobject jActivityObject{env->NewObject(jActivityClass, jActivityClassInitializer, m_bannerId)};
    
    if (env->ExceptionCheck()) return false;
    
    m_jniActivity = std::make_unique<QAndroidJniObject>(jActivityObject);
    
    return true;
}

bool QtYandexAdsBannerAndroid::prepareNativeContext(QAndroidJniEnvironment &env, 
                                                    const jclass jActivityClass)
{
    if (env->ExceptionCheck()) return false;
    
    JNINativeMethod methods[] {{"onBannerLoaded",     "(V)V",                  reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerLoaded)},
                               {"onBannerLoading",    "(V)V",                  reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerLoading)},
                               {"onBannerClicked",    "(V)V",                  reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerClicked)},
                               {"onBannerClosed",     "(V)V",                  reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerClosed)},
                               {"onBannerLoadFail",   "(I)V",                  reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerLoadFail)},
                               {"onBannerImpression", "(Ljava/lang/String;)V", reinterpret_cast<void *>(QtYandexAdsBannerAndroid::processBannerImpression)}};
    
    env->RegisterNatives(jActivityClass,
                         methods,
                         sizeof(methods) / sizeof(methods[0]));
    
    return !(env->ExceptionCheck());
}

void QtYandexAdsBannerAndroid::processBannerLoading(JNIEnv *env, jobject thiz)
{
    auto curBannerInstance = getInstanceByJavaObjectWithCheck(env, thiz);
    
    emit curBannerInstance->loading();
}

void QtYandexAdsBannerAndroid::processBannerLoaded(JNIEnv *env, jobject thiz)
{
    auto curBannerInstance = getInstanceByJavaObjectWithCheck(env, thiz);
    
    emit curBannerInstance->loaded();
}

void QtYandexAdsBannerAndroid::processBannerClosed(JNIEnv *env, jobject thiz)
{
    auto curBannerInstance = getInstanceByJavaObjectWithCheck(env, thiz);
    
    emit curBannerInstance->closed();
}

void QtYandexAdsBannerAndroid::processBannerClicked(JNIEnv *env, jobject thiz)
{
    auto curBannerInstance = getInstanceByJavaObjectWithCheck(env, thiz);
    
    emit curBannerInstance->clicked();
}

void QtYandexAdsBannerAndroid::processBannerLoadFail(JNIEnv *env, jobject thiz, const jint rawErrorCode)
{
    auto curBannerInstance = getInstanceByJavaObjectWithCheck(env, thiz);
    
    emit curBannerInstance->errorOccured(getAdErrorByErrorCode(static_cast<ErrorCode>(rawErrorCode)));
}

void QtYandexAdsBannerAndroid::processBannerImpression(JNIEnv *env, jobject thiz, const jstring rawImpressionJsonData)
{
    auto curBannerInstance = getInstanceByJavaObjectWithCheck(env, thiz);
    
    // FIXME: ad impression processing case...
}

void QtYandexAdsBannerAndroid::initializeBannerId()
{
    m_bannerId = m_curBannerId;
    
    ++m_curBannerId;
}

std::shared_ptr<QtYandexAdsBannerAndroid> QtYandexAdsBannerAndroid::getInstanceByJavaObjectWithCheck(JNIEnv *env, jobject thiz)
{
    if (env->ExceptionCheck()) {
        // FIXME: exception case processing...
        
        throw QString{"JNI exception was occured!"};
    }
    
    QAndroidJniObject jObj{thiz};
    auto bannerId = jObj.callMethod<QtYandexAdsBannerAndroid::BannerId>("GetBannerId", "(V)I");
   
    auto curBannerInstance = getInstanceById(bannerId);
    
    if (!curBannerInstance.get()) {
        // FIXME: invalid ptr case processing...
        
        throw QString{"Banner instance can't be found!"};
    }
    
    return curBannerInstance;
}
