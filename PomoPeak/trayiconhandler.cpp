#include "trayiconhandler.h"
#include <QAction>
TrayIconHandler::TrayIconHandler() : icon(":/icon/data/images/icon.jpg"), trayIcon(), menu()
{
    trayIcon.setIcon(icon);
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
void TrayIconHandler::SendMessage(const QString title, const QString message, const int msDuration)
{
    trayIcon.showMessage(title, message, icon, msDuration);
}
void TrayIconHandler::OnOpenTrigger()
{
    emit Open();
}
void TrayIconHandler::OnExitTrigger()
{
    emit Exit();
}
