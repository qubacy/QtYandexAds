#include "QtYandexAdsBannerInterface.h"

QtYandexAdsBannerInterface::QtYandexAdsBannerInterface(QObject *parent)
    : QObject {parent},
      m_unitId  {QString{}},
      m_size    {Sizes::S_SMALL_HORIZONTAL},
      m_position{QPoint{}}
{
    
}

bool QtYandexAdsBannerInterface::setUnitId(const QString &unitId)
{
    if (unitId.isEmpty()) return false;
    
    m_unitId = unitId;
    
    return true;
}

bool QtYandexAdsBannerInterface::setSize(Sizes size)
{
    if (size >= Sizes::S_COUNT || size <= Sizes::S_INVALID)
        return false;
    
    m_size = size;
    
    return true;
}

bool QtYandexAdsBannerInterface::setPosition(const QPoint &position)
{
    if (position.isNull()) return false;
    
    m_position = position;
    
    return true;
}
