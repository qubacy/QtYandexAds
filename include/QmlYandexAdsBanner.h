#ifndef QMLYANDEXADSBANNER_H
#define QMLYANDEXADSBANNER_H

#include "QtYandexAds_global.h"

#include <QObject>

#include "QtYandexAdsBannerInterface.h"
#include "QtYandexAdsManager.h"

class QTYANDEXADSSHARED_EXPORT QmlYandexAdsBanner : public QObject
{
    Q_PROPERTY(QString unitId READ unitId WRITE setUnitId NOTIFY unitIdChanged)
    Q_PROPERTY(QtYandexAdsBannerInterface::Sizes size READ size WRITE  setSize NOTIFY sizeChanged)
    Q_PROPERTY(QSize sizeInPixels READ sizeInPixels NOTIFY sizeChanged)
    Q_PROPERTY(int width READ width NOTIFY sizeChanged)
    Q_PROPERTY(int height READ height NOTIFY sizeChanged)
    Q_PROPERTY(QPoint position READ position WRITE  setPosition NOTIFY positionChanged)
    Q_PROPERTY(int x READ x WRITE  setX NOTIFY positionChanged)
    Q_PROPERTY(int y READ y WRITE  setY NOTIFY positionChanged)
    Q_PROPERTY(bool visible READ visible WRITE  setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY loaded)

    Q_OBJECT
public:
    explicit QmlYandexAdsBanner(QObject *parent = nullptr);
    ~QmlYandexAdsBanner() = default;

    static void declareQML();

    void setUnitId(const QString& unitId);
    const QString& unitId() const;

    void setSize(QtYandexAdsBannerInterface::Sizes size);
    QtYandexAdsBannerInterface::Sizes size() const;
    QSize sizeInPixels();

    int width();
    int height();

    void setPosition(const QPoint& position);
    const QPoint& position() const;

    void setX(int x);
    int x();

    void setY(int y);
    int y();

    void setVisible(bool isVisible);
    bool visible();

    bool isLoaded();

signals:
    void unitIdChanged  (bool);
    void sizeChanged    (bool);
    void positionChanged(bool);
    void visibleChanged (bool);
    
    void loaded();
    void loading();
    void closed();
    void clicked();
    void errorOccured(AdError err);
    
private:
    std::shared_ptr<QtYandexAdsBannerInterface> m_ad;
};

#endif // QMLYANDEXADSBANNER_H
