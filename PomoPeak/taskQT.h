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
    void SwitchSelectState();
    void EnableViewMode();
    void DisableViewMode();
signals:
    void DeleteRequest(std::shared_ptr<Task> task);
    void CreateRequest(std::shared_ptr<Task> task);
    void OnEnableViewModeRequest(taskQT* ui);
    void OnSelectRequest(taskQT* ui);
protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if(event->button() == Qt::LeftButton && !isViewMode)
        {
            EnableViewMode();
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
private:
    const int MIN_TITLE_SIZE = 3;
    const QString selectedTaskSheet = "background-color:rgba(0, 255, 0, 96)";
    const QString unselectedTaskSheet = "background-color:rgba(170, 0, 0, 96)";
    const QString viewLabelValue = "Currently in View Mode";
    const QString editLabelValue = "Currently in Edit Mode";

    bool isCreated = false;
    bool isEditMode = false;
    bool isSelected = false;
    bool isViewMode = true;

    Ui::taskQT* ui;
    TaskInputFilter* filter;

    std::shared_ptr<Task> task;

    void OnTaskTitleChanged();

    void EnableEditMode();
    void DisableEditMode();

    void ProceedTaskModifications();
    void CancelTaskModifications();

    void UpdateModeLabel(QString val);
};
#endif // TASKQT_H
