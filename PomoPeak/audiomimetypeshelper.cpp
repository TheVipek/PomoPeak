#include "audiomimetypeshelper.h"

QList<QMimeType> audioMimeTypesHelper::mimeTypes;

const QList<QMimeType>& audioMimeTypesHelper::getAudioMimeTypes() {
    // Initialize supported audio formats if not already initialized
    if (mimeTypes.empty()) {
        QMimeDatabase db;
        QList<QMimeType> allMimeTypes = db.allMimeTypes();
        for(const QMimeType &mimeType : allMimeTypes)
        {
            if(mimeType.name().startsWith("audio/"))
            {
                mimeTypes.append(mimeType);
            }
        }
    }

    return mimeTypes;
}

bool audioMimeTypesHelper::IsMimeTypeValid(QString suffix)
{
    if(mimeTypes.empty())
    {
        getAudioMimeTypes();
    }

    return std::any_of(audioMimeTypesHelper::mimeTypes.begin(),audioMimeTypesHelper::mimeTypes.end(),[suffix](const QMimeType& type){ return type.preferredSuffix() == suffix; });
}
QString audioMimeTypesHelper::GetAllMimeTypeForFileDialog()
{
    if (mimeTypes.empty())
    {
        getAudioMimeTypes();
    }

    QString filter = QObject::tr("Audio Files") + " (";
    QStringList extensionsList;

    for (const QMimeType& mimeType : mimeTypes)
    {
        if (!mimeType.preferredSuffix().isEmpty())
        {
            extensionsList.append("*." + mimeType.preferredSuffix());
        }
    }

    // Join the extensions with a space and append to the filter string
    filter += extensionsList.join(' ') + ")";

    return filter;
}
