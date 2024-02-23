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
    explicit taskQT(QWidget *parent = nullptr);
    ~taskQT();
    void UpdateElapsed(int elapsed);
signals:
    void DeleteRequest(std::shared_ptr<Task> task);     // connect to it outside of this cpp to get notified when user click it
    void CreateRequest(std::shared_ptr<Task> task);
private slots:
    void OnModify();
    void OnDelete();
    void OnCreate();
private:
    bool isCreated = false;
    Ui::taskQT* ui;
    std::shared_ptr<Task> task;
    void Init();
};
#endif // TASKQT_H
