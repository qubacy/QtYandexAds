QT     += core gui qml

CONFIG += c++17 console

TARGET = QtYandexAds
TEMPLATE = app

SOURCES += main.cpp

HEADERS  += \

include( ../../common.pri )
include( ../../app.pri )

#LIBS += -lQtLib$${LIB_SUFFIX}

PLATFORMS_DIR = $$PWD/platform

android {
    ANDROID_PACKAGE_SOURCE_DIR = $${PLATFORMS_DIR}/android

    android:QT += androidextras gui-private
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
