#include "GPTHelper.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QEventLoop>
#include <iostream>
#include <limits>
GPTHelper::GPTHelper()
{
    manager = new QNetworkAccessManager(this);
    engine = std::mt19937(rd());
}

QString GPTHelper::Ask(const QString question, const QString systemInfo)
{
    if(!IsKeySet())
    {
        return "API key is not set or not valid.";
    }
    try
    {
        QNetworkRequest request;
        request.setUrl(QUrl(endpoint + "/chat/completions"));
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

        out["messages"] = messagesArray;
        out["model"] = defaultModel;

        std::uniform_int_distribution<> dist(0, std::numeric_limits<int>::max());
        int randomSeed = dist(engine);

        out["seed"] = randomSeed;
        out["temperature"] = .75f;
        QEventLoop loop;
        QObject::connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

        QNetworkReply* reply = manager->post(request, QJsonDocument(out).toJson());
        loop.exec();

        return HandleCompletionistReply(reply);

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
        return jsonObject["choices"][0]["message"]["content"].toString();
    }
    return reply->errorString();
}
