#ifndef PATHHELPER_H
#define PATHHELPER_H
#include <QString>
#include <QMediaPlayer>
class PathHelper
{
public:
    static QString GetAudioWithExtension(const QMediaPlayer& player,QString filePath);
};

#endif // PATHHELPER_H
