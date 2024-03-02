#include "pomopeak.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    PomoPeak w;
    w.show();
    return a.exec();
}
