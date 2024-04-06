#ifndef GPTHELPER_H
#define GPTHELPER_H

#include <QNetworkAccessManager>
class GPTHelper : public QObject
{
    Q_OBJECT
public:
    GPTHelper();
    void SetAPIKey(const QString key);
    QString Ask(const QString question, const QString systemInfo);
private:
    QNetworkAccessManager* manager;
    bool IsKeySet();
    QString endpoint = "https://api.openai.com/v1/chat";
    QString apiKey = "";
    const QString defaultModel = "gpt-3.5-turbo-0125";
private slots:
    QString HandleCompletionistReply(QNetworkReply* reply);
};

#endif // GPTHELPER_H
