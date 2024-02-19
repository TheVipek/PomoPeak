#ifndef POMOPEAK_H
#define POMOPEAK_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class PomoPeak;
}
QT_END_NAMESPACE

class PomoPeak : public QMainWindow
{
    Q_OBJECT

public:
    PomoPeak(QWidget *parent = nullptr);
    ~PomoPeak();

private:
    Ui::PomoPeak *ui;
};
#endif // POMOPEAK_H
