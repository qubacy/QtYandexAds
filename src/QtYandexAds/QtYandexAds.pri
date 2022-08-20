DEFINES += QTYANDEXADS_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += $$PWD/AdError.cpp \
    $$PWD/QmlYandexAdsBanner.cpp \
    $$PWD/QtYandexAdsBannerAndroid.cpp \
    $$PWD/QtYandexAdsBannerInterface.cpp \
    $$PWD/QtYandexAdsManager.cpp

HEADERS  += \
    $$PWD/AdError.h \
    $$PWD/QmlYandexAdsBanner.h \
    $$PWD/QtYandexAdsBannerAndroid.h \
    $$PWD/QtYandexAdsBannerInterface.h \
    $$PWD/QtYandexAdsManager.h

PLATFORMS_DIR = $$PWD/platform

android {
    ANDROID_PACKAGE_SOURCE_DIR = $${PLATFORMS_DIR}/android

    QT += androidextras gui-private
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    $$PWD/platform/android/AndroidManifest.xml \
    $$PWD/platform/android/build.gradle \
    $$PWD/platform/android/src/org/test/QtYandexAds/QtYandexAdsActivity.java \
    $$PWD/platform/android/src/org/test/QtYandexAds/QtYandexAdsBanner.java
