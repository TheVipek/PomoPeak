#ifndef TRAYICONHANDLER_H
#define TRAYICONHANDLER_H
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
class TrayIconHandler : public QObject
{
    Q_OBJECT
public:
    TrayIconHandler();
    void Show();
    void Hide();
    void SendMessage(const QString title, const QString message, const int msDuration);
signals:
    void Open();
    void Exit();
private:
    QIcon icon;
    QSystemTrayIcon trayIcon;
    QMenu menu;
public slots:
    void OnOpenTrigger();
    void OnExitTrigger();
};

#endif // TRAYICONHANDLER_H
