#ifndef TASKQT_H
#define TASKQT_H

#include <QWidget>
#include "task.h"
#include <memory>
QT_BEGIN_NAMESPACE
namespace Ui {
class taskQT;
}
QT_END_NAMESPACE

class taskQT: public QWidget
{
    Q_OBJECT

public:
    explicit taskQT(std::shared_ptr<Task> task, QWidget *parent = nullptr);
    ~taskQT();

    void UpdateElapsed(int elapsed);
signals:
    void DeleteRequest();     // connect to it outside of this cpp to get notified when user click it
private slots:
    void OnModify();
    void OnDelete();
private:
    Ui::taskQT* ui;
    std::shared_ptr<Task> task;
    void Init();
};
#endif // TASKQT_H
