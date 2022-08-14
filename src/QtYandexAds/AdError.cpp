#include "AdError.h"

AdError::AdError(const QString &message,
                 const bool isCritical)
    : m_message   {message},
      m_isCritical{isCritical}
{
    
}

const QString &AdError::getMessage() const
{
    return m_message;
}

bool AdError::isCritical() const
{
    return m_isCritical;
}

bool AdError::isValid()
{
    return (!m_message.isEmpty());
}
