#ifndef QTYANDEXADSBANNERINTERFACE_H
#define QTYANDEXADSBANNERINTERFACE_H

#include <QObject>
#include <QPoint>
#include <QSize>

#include "AdError.h"

class QtYandexAdsBannerInterface : public QObject
{
    Q_OBJECT
    
//public:
//    constexpr static const char* C_TEST_UNIT_ID = "R-M-DEMO-300x250";
    
public:
//    enum Sizes : uint8_t {
//        S_INVALID = 0,
//        S_SMALL_HORIZONTAL,
//        S_MEDIUM_HORIZONTAL,
//        S_MEDIUM_RECTANGULAR,
//        S_LARGE_HORIZONTAL,
//        S_FULL_SCREEN,
//        S_COUNT
//    };
//    Q_ENUM(Sizes)
    
    enum VerticalAttachment : uint8_t {
        VA_INVALID = 0,
        VA_TOP,
        VA_CENTER,
        VA_BOTTOM,
        VA_COUNT
    };
    Q_ENUM(VerticalAttachment)
    
    enum HorizontalAttachment : uint8_t {
        HA_INVALID = 0,
        HA_LEFT,
        HA_CENTER,
        HA_RIGHT,
        HA_COUNT
    };
    Q_ENUM(HorizontalAttachment)
    
    explicit QtYandexAdsBannerInterface(QObject *parent = nullptr);
    virtual ~QtYandexAdsBannerInterface() = default;
    
//    virtual bool initialize() = 0;
    
    virtual bool setUnitId(const QString& unitId);
    virtual const QString& unitId() const = 0;
    
    virtual bool initializeSizeUsingNative() = 0;
    virtual bool setSize(const QSize &size);
    virtual const QSize& size() const = 0;
    
    virtual bool initializePositionUsingNative() = 0;
    virtual bool setPosition(const QPoint& position);
    virtual const QPoint& position() const = 0;
    
    virtual bool setVisible(bool isVisible) = 0;
    virtual bool visible() = 0;
    
    virtual bool isLoaded() = 0;
    
    virtual bool setVerticalAttachment(const VerticalAttachment attachment);
    virtual bool setHorizontalAttachment(const HorizontalAttachment attachment);
    
signals:
    void loaded ();
    void loading();
    void closed ();
    void clicked();
    void errorOccured(AdError err);
    
protected:
    QString m_unitId;
    QSize   m_size;
    QPoint  m_position;
};

#endif // QTYANDEXADSBANNERINTERFACE_H
