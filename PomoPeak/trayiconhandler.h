#ifndef TRAYICONHANDLER_H
#define TRAYICONHANDLER_H
#include <QMessageBox>
#include <QSystemTrayIcon>
class TrayIconHandler
{
public:
    TrayIconHandler();
    void Show();
    void Hide();
    void SendMessage(const QString title, const QString message, const QSystemTrayIcon::MessageIcon priority, const int msDuration);
private:
    QSystemTrayIcon trayIcon;
};

#endif // TRAYICONHANDLER_H
