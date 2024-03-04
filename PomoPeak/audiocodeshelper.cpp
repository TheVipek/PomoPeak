#include "audiocodeshelper.h"

std::unordered_set<QString> audioCodesHelper::audioFormats;

const std::unordered_set<QString>& audioCodesHelper::getSupportedAudioFormats() {
    // Initialize supported audio formats if not already initialized
    if (audioFormats.empty()) {
        QMetaEnum audioCodecEnum = QMetaEnum::fromType<QMediaFormat::AudioCodec>();
        for (int i =0; i < audioCodecEnum.keyCount(); ++i) {
            int value = audioCodecEnum.value(i);

            const char* key = audioCodecEnum.key(i);
            if(value != -1)
            {
                audioFormats.insert(QString::fromUtf8(key).toLower());
            }
        }
    }

    return audioFormats;
}
