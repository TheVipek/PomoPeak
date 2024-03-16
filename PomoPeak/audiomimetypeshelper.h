#ifndef AUDIOMIMETYPESHELPER_H
#define AUDIOMIMETYPESHELPER_H
#include<QMediaFormat>
#include <QMetaEnum>
#include <unordered_set>
#include <QMimeDatabase>
#include <QMimeType>
class audioMimeTypesHelper
{
public:

    static bool IsMimeTypeValid(QString format);
    static QString GetAllMimeTypeForFileDialog();

private:
    static const QList<QMimeType>& getAudioMimeTypes();
    static QList<QMimeType> mimeTypes;
};

#endif // AUDIOMIMETYPESHELPER_H
