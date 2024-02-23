#ifndef TASKQT_H
#define TASKQT_H

#include <QWidget>
#include "task.h"
#include <memory>
#include "TaskInputFilter.h"
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
    void ElapsedResponse();
signals:
    void DeleteRequest(std::shared_ptr<Task> task);
    void CreateRequest(std::shared_ptr<Task> task);


protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        return true;
    }
private slots:
    void OnModify();
    void OnDelete();
    void OnCreate();
private:
    Ui::taskQT* ui;
    TaskInputFilter* filter;
    const int MIN_TITLE_SIZE = 3;

    bool isCreated = false;

    std::shared_ptr<Task> task;

    void OnTaskTitleChanged();
};
#endif // TASKQT_H
