#include "pomopeak.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PomoPeak w;
    w.setWindowIcon(QIcon(":/icon/images/icon.jpg"));
    w.show();
    QApplication::setQuitOnLastWindowClosed(false);
    return a.exec();
}


