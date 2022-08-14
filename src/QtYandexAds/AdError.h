#ifndef ADERROR_H
#define ADERROR_H

#include <QString>
#include <QMetaType>

class AdError
{
    Q_GADGET
    
public:
    AdError(const QString &message = QString{},
            const bool isCritical = false);
    
    const QString& getMessage() const;
    bool isCritical() const;
    
    bool isValid();
    
private:
    QString m_message;
    bool m_isCritical;
};

Q_DECLARE_METATYPE(AdError)

#endif // ADERROR_H
