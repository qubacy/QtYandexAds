#ifndef QTYANDEXADSBANNERINTERFACE_H
#define QTYANDEXADSBANNERINTERFACE_H

#include "QtYandexAds_global.h"

#include <QObject>
#include <QPoint>
#include <QSize>

#include "AdError.h"

class QTYANDEXADSSHARED_EXPORT QtYandexAdsBannerInterface : public QObject
{
    Q_OBJECT
    
public:
    constexpr static const char* C_TEST_UNIT_ID = "R-M-DEMO-300x250";
    
public:
    enum Sizes : uint8_t {
        S_INVALID = 0,
        S_SMALL_HORIZONTAL,
        S_MEDIUM_HORIZONTAL,
        S_MEDIUM_RECTANGULAR,
        S_LARGE_HORIZONTAL,
        S_FULL_SCREEN,
        S_COUNT
    };
    Q_ENUM(Sizes)
    
    explicit QtYandexAdsBannerInterface(QObject *parent = nullptr);
    virtual ~QtYandexAdsBannerInterface() = default;
    
    virtual bool initialize() = 0;
    
    virtual bool setUnitId(const QString& unitId);
    virtual const QString& unitId() const = 0;
    
    virtual bool setSize(Sizes size);
    virtual Sizes size() const = 0;
    virtual QSize sizeInPixels() = 0;
    
    virtual bool setPosition(const QPoint& position);
    virtual const QPoint& position() const = 0;
    
    virtual bool setVisible(bool isVisible) = 0;
    virtual bool visible() = 0;
    
    virtual bool isLoaded() = 0;
    
signals:
    void loaded ();
    void loading();
    void closed ();
    void clicked();
    void errorOccured(AdError err);
    
protected:
    QString m_unitId;
    Sizes   m_size;
    QPoint  m_position;
};

#endif // QTYANDEXADSBANNERINTERFACE_H
