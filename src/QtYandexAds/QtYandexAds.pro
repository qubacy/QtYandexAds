include( ../../common.pri )
include( ../../lib.pri )

QT     += core gui qml

CONFIG += c++17

TARGET = QtYandexAds$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += QTYANDEXADS_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += AdError.cpp \
    QmlYandexAdsBanner.cpp \
    QtYandexAdsBannerAndroid.cpp \
    QtYandexAdsBannerInterface.cpp \
    QtYandexAdsManager.cpp

HEADERS  += \
    $${INC_PATH}\QtYandexAds_global.h \
    $${INC_PATH}\AdError.h \
    $${INC_PATH}\QmlYandexAdsBanner.h \
    $${INC_PATH}\QtYandexAdsBannerAndroid.h \
    $${INC_PATH}\QtYandexAdsBannerInterface.h \
    $${INC_PATH}\QtYandexAdsManager.h

PLATFORMS_DIR = $$PWD/platform

android {
    ANDROID_PACKAGE_SOURCE_DIR = $${PLATFORMS_DIR}/android

    QT += androidextras gui-private
}

win32 {
    QMAKE_TARGET_PRODUCT = QtYandexAds Lib
    QMAKE_TARGET_DESCRIPTION = Test QtYandexAds library
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    platform/android/AndroidManifest.xml \
    platform/android/build.gradle \
    platform/android/gradle.properties \
    platform/android/gradle/wrapper/gradle-wrapper.jar \
    platform/android/gradle/wrapper/gradle-wrapper.properties \
    platform/android/gradlew \
    platform/android/gradlew.bat \
    platform/android/res/values/libs.xml \
    platform/android/src/org/test/QtYandexAds/QtYandexAdsActivity.java
