#include "trayiconhandler.h"
#include <QAction>
TrayIconHandler::TrayIconHandler() : trayIcon(nullptr), menu(nullptr)
{
    trayIcon.setIcon(QIcon(":/icon/data/images/icon.jpg"));
    QAction* openAction = menu.addAction("Open");
    QAction* exitAction = menu.addAction("Exit");
    trayIcon.setContextMenu(&menu);
    connect(openAction, &QAction::triggered, this, &TrayIconHandler::OnOpenTrigger);
    connect(exitAction, &QAction::triggered, this, &TrayIconHandler::OnExitTrigger);
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
void TrayIconHandler::OnOpenTrigger()
{
    emit Open();
}
void TrayIconHandler::OnExitTrigger()
{
    emit Exit();
}
