#ifndef SKIN_H
#define SKIN_H
#include <QString>
#include <QObject>
#include "qobjectdefs.h"
#include "qtmetamacros.h"
#include <map>
class Skin : public QObject
{
    Q_OBJECT
public:
    enum class SkinTypes : int
    {
        White,
        Dark
    };
    const static std::map<QString, SkinTypes> SkinTypesEnumerable;
    Q_ENUM(SkinTypes);
};

#endif // SKIN_H
