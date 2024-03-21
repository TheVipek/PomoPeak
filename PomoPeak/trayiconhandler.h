#ifndef TRAYICONHANDLER_H
#define TRAYICONHANDLER_H
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
class TrayIconHandler : public QObject
{

public:
    TrayIconHandler();
    void Show();
    void Hide();
    void SendMessage(const QString title, const QString message, const QSystemTrayIcon::MessageIcon priority, const int msDuration);
private:
    QSystemTrayIcon trayIcon;
    QMenu menu;
signals:
    void Open();
    void Exit();
private slots:
    void OnOpen();
    void OnExit();
};

#endif // TRAYICONHANDLER_H
