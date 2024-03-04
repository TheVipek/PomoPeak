#ifndef AUDIOCODESHELPER_H
#define AUDIOCODESHELPER_H
#include<QMediaFormat>
#include <QMetaEnum>
#include <unordered_set>
class audioCodesHelper
{
public:
    static const std::unordered_set<QString>& getSupportedAudioFormats();

private:
    static std::unordered_set<QString> audioFormats;
};

#endif // AUDIOCODESHELPER_H
