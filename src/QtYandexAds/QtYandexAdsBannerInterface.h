#ifndef QTYANDEXADSBANNERINTERFACE_H
#define QTYANDEXADSBANNERINTERFACE_H

#include <QObject>

#include "AdError.h"

class QtYandexAdsBannerInterface : public QObject
{
    Q_OBJECT
public:
    enum Sizes : uint8_t {
        S_SMALL_HORIZONTAL = 0,
        S_MEDIUM_HORIZONTAL,
        S_MEDIUM_RECTANGULAR,
        S_LARGE_HORIZONTAL,
        S_FULL_SCREEN
    };
    Q_ENUM(Sizes)
    
    explicit QtYandexAdsBannerInterface(QObject *parent = nullptr);
    virtual ~QtYandexAdsBannerInterface() = default;
    
    virtual bool initialize() = 0;
    
    virtual void setUnitId(const QString& unitId) = 0;
    virtual const QString& unitId() const = 0;
    
    virtual void setSize(Sizes size) = 0;
    virtual Sizes size() const = 0;
    virtual QSize sizeInPixels() = 0;
    
    virtual void setPosition(const QPoint& position) = 0;
    virtual const QPoint& position() const = 0;
    
    virtual void setVisible(bool isVisible) = 0;
    virtual bool visible() = 0;
    
    virtual bool isLoaded() = 0;
    
signals:
    void loaded ();
    void loading();
    void closed ();
    void clicked();
    void errorOccured(AdError err);
};

#endif // QTYANDEXADSBANNERINTERFACE_H
