#include "QmlYandexAdsBanner.h"
#include <QtQml>

QmlYandexAdsBanner::QmlYandexAdsBanner(QObject *parent)
    : QObject{parent}
{
    m_ad = QtYandexAdsManager::generateAdBanner();
    
    if (!m_ad.get())
        throw QString{"Qml banner object creation fail!"};
    
    connect(m_ad.get(), &QtYandexAdsBannerInterface::loaded,       this, &QmlYandexAdsBanner::loaded);
    connect(m_ad.get(), &QtYandexAdsBannerInterface::loading,      this, &QmlYandexAdsBanner::loading);
    connect(m_ad.get(), &QtYandexAdsBannerInterface::clicked,      this, &QmlYandexAdsBanner::clicked);
    connect(m_ad.get(), &QtYandexAdsBannerInterface::closed,       this, &QmlYandexAdsBanner::closed);
    connect(m_ad.get(), &QtYandexAdsBannerInterface::errorOccured, this, &QmlYandexAdsBanner::errorOccured);
}

void QmlYandexAdsBanner::declareQML()
{
    qmlRegisterType<QmlYandexAdsBanner>("com.test.QmlYandexAdsBanner", 1, 0, "YandexAdsBanner");
    qmlRegisterUncreatableType<QtYandexAdsBannerInterface>("com.test.QmlYandexAdsBannerBase", 1, 0, "YandexAdsBannerBase", "");
    qRegisterMetaType<AdError>("AdError");
}

void QmlYandexAdsBanner::setUnitId(const QString &unitId)
{
    emit unitIdChanged(m_ad->setUnitId(unitId));
}

const QString &QmlYandexAdsBanner::unitId() const
{
    return m_ad->unitId();
}

void QmlYandexAdsBanner::setSize(QtYandexAdsBannerInterface::Sizes size)
{
    emit sizeChanged(m_ad->setSize(size));
}

QtYandexAdsBannerInterface::Sizes QmlYandexAdsBanner::size() const
{
    return m_ad->size();
}

QSize QmlYandexAdsBanner::sizeInPixels()
{
    return m_ad->sizeInPixels();
}

int QmlYandexAdsBanner::width()
{
    return sizeInPixels().width();
}

int QmlYandexAdsBanner::height()
{
    return sizeInPixels().height();
}

void QmlYandexAdsBanner::setPosition(const QPoint &position)
{
    emit positionChanged(m_ad->setPosition(position));
}

const QPoint &QmlYandexAdsBanner::position() const
{
    return m_ad->position();
}

void QmlYandexAdsBanner::setX(int x)
{
    emit positionChanged(m_ad->setPosition(QPoint{x, y()}));
}

int QmlYandexAdsBanner::x()
{
    return m_ad->position().x();
}

void QmlYandexAdsBanner::setY(int y)
{
    emit positionChanged(m_ad->setPosition(QPoint{x(), y}));
}

int QmlYandexAdsBanner::y()
{
    return m_ad->position().y();
}

void QmlYandexAdsBanner::setVisible(bool isVisible)
{
    emit visibleChanged(m_ad->setVisible(isVisible));
}

bool QmlYandexAdsBanner::visible()
{
    return m_ad->visible();
}

bool QmlYandexAdsBanner::isLoaded()
{
    return m_ad->isLoaded();
}

