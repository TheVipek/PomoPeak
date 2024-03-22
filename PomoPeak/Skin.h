#ifndef SKIN_H
#define SKIN_H
#include <QString>
#include <QObject>
#include "qobjectdefs.h"
#include "qtmetamacros.h"
class Skin : public QObject
{
    Q_OBJECT
public:
    enum class SkinTypes : int
    {
        White,
        Dark
    };
    Q_ENUM(SkinTypes);
};

#endif // SKIN_H
