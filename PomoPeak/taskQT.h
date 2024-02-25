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
    void ElapsedIncrease();
signals:
    void DeleteRequest(std::shared_ptr<Task> task);
    void CreateRequest(std::shared_ptr<Task> task);

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if(event->button() == Qt::LeftButton && !isActive)
        {
            Active();
        }
    }
private slots:

    void OnModifyButton();
    void OnProceedButton();
    void OnCancelButton();

    void OnModifyEnter();
    void OnModifyProceed();
    void OnModifyCancel();
    void OnModifyExit();

    void OnDelete();
    void OnCreate();

    void OnChangeSelectState();
private:
    const int MIN_TITLE_SIZE = 3;

    bool isCreated = false;

    bool isEditMode = false;
    bool isSelected = false;
    bool isActive = false;

    Ui::taskQT* ui;
    TaskInputFilter* filter;

    std::shared_ptr<Task> task;



    void OnTaskTitleChanged();

    void EnableEditMode();
    void DisableEditMode();

    void ProceedTaskModifications();
    void CancelTaskModifications();

    void Active();
    void Deactive();


};
#endif // TASKQT_H
