#include "GPTHelper.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
GPTHelper::GPTHelper()
{
    manager = new QNetworkAccessManager(this);
}

QString GPTHelper::Ask(const QString question, const QString systemInfo)
{
    try
    {
        if(IsKeySet())
        {
            QNetworkRequest request;
            request.setUrl(QUrl(endpoint + "/completions"));
            QString content = "application/json";
            request.setHeader(QNetworkRequest::ContentTypeHeader, content.toUtf8());

            QString authVal = QString("Bearer %1").arg(apiKey);
            request.setRawHeader("Authorization",authVal.toUtf8());

            QJsonObject system;
            system["content"] = systemInfo;
            system["role"] = "system";

            QJsonObject user;
            user["content"] = question;
            user["role"] = "user";

            QJsonArray messagesArray;
            messagesArray.append(system);
            messagesArray.append(user);

            QJsonObject out;
            //out["max_tokens"] = 5;

            out["messages"] = messagesArray;
            out["model"] = defaultModel;

            QObject::connect(manager, &QNetworkAccessManager::finished, this, &GPTHelper::HandleCompletionistReply);

            manager->post(request, QJsonDocument(out).toJson());
        }
        throw std::invalid_argument("API key is not assigned.");
    }
    catch(std::exception& ex)
    {
        return ex.what();
    }
}
void GPTHelper::SetAPIKey(const QString key)
{
    apiKey = key;
}
bool GPTHelper::IsKeySet()
{
    return apiKey != "";
}
QString GPTHelper::HandleCompletionistReply(QNetworkReply* reply)
{
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    if (!jsonResponse.isNull() && jsonResponse.isObject() && reply->error() == QNetworkReply::NetworkError::NoError )
    {
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << jsonObject["choices"][0]["message"]["content"].toString();
        return jsonObject["choices"][0]["message"]["content"].toString();
    }
    qDebug() << reply->errorString();
    return reply->errorString();
}
