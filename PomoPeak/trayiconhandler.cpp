#include "trayiconhandler.h"
TrayIconHandler::TrayIconHandler() : trayIcon(nullptr)
{
    trayIcon.setIcon(QIcon(":/icon/data/images/icon.jpg"));
}

void TrayIconHandler::Show()
{
    trayIcon.show();
}
void TrayIconHandler::Hide()
{
    trayIcon.hide();
}
void TrayIconHandler::SendMessage(const QString title, const QString message, const QSystemTrayIcon::MessageIcon priority, const int msDuration)
{
    trayIcon.showMessage(title, message, priority, msDuration);
}
