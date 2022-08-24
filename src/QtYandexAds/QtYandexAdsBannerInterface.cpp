#include "QtYandexAdsBannerInterface.h"

QtYandexAdsBannerInterface::QtYandexAdsBannerInterface(QObject *parent)
    : QObject {parent},
      m_unitId  {QString{}},
      m_size    {QSize{}},
      m_position{QPoint{}}
{
    
}

bool QtYandexAdsBannerInterface::setUnitId(const QString &unitId)
{
    if (unitId.isEmpty()) return false;
    
    m_unitId = unitId;
    
    return true;
}

bool QtYandexAdsBannerInterface::setSize(const QSize& size)
{
    if (!size.isValid()) return false;
    
    m_size = size;
    
    return true;
}

bool QtYandexAdsBannerInterface::setPosition(const QPoint &position)
{
    if (position.isNull()) return false;
    
    m_position = position;
    
    return true;
}

bool QtYandexAdsBannerInterface::setVerticalAttachment(const VerticalAttachment attachment)
{
    return (attachment > VerticalAttachment::VA_INVALID && attachment < VerticalAttachment::VA_COUNT);
}

bool QtYandexAdsBannerInterface::setHorizontalAttachment(const HorizontalAttachment attachment)
{
    return (attachment > HorizontalAttachment::HA_INVALID && attachment < HorizontalAttachment::HA_COUNT);    
}
