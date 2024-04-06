#ifndef GPTHELPER_H
#define GPTHELPER_H

#include <QNetworkAccessManager>
#include <random>

class GPTHelper : public QObject
{
    Q_OBJECT
public:
    GPTHelper();
    void SetAPIKey(const QString key);
    QString Ask(const QString question, const QString systemInfo);
private:
    QNetworkAccessManager* manager;
    QString apiKey = "";
    bool IsKeySet();
    QString endpoint = "https://api.openai.com/v1";

    const QString defaultModel = "gpt-3.5-turbo-0125";
    std::random_device rd;
    std::mt19937 engine;

private slots:
    QString HandleCompletionistReply(QNetworkReply* reply);
};

#endif // GPTHELPER_H
