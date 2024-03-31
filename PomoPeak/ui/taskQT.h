#ifndef TASKQT_H
#define TASKQT_H

#include <QWidget>
#include "task.h"
#include <memory>
#include "TaskInputFilter.h"
#include <QGraphicsBlurEffect>
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
    void ChangeViewModeState(bool enabled);
signals:
    void DeleteRequest(std::shared_ptr<Task> task, taskQT* taskU);
    void CreateRequest(std::shared_ptr<Task> task);
    void OnEnableViewModeRequest(taskQT* ui);
    void OnSelectRequest(taskQT* ui);
    void OnStatusChanged(bool done);
protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if(event->button() == Qt::LeftButton && !isViewMode && !task->isDone)
        {
            ChangeViewModeState(true);
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

    void OnChangeStatus();
private:
    const int MIN_TITLE_SIZE = 3;
    const float UNDONE_BLUR = 0;
    const float DONE_BLUR = 20.0f;
    const QString selectedTaskWidgetSheet =
        R"(#taskQT
        {
            background-color: rgba(34, 34, 34, 128);
            border-style: solid;
            border-color: gray;
            border-width:2px;
        })";

    const QString unselectedTaskWidgetSheet = R"(#taskQT
        {
            background-color: rgba(34, 34, 34, 64);
            border-style: solid;
            border-color: gray;
            border-width:2px;
        })";

    const QString doneTextEditSheet = R"(QWidget
        {
            background: rgba(0,0,0,0); color: rgba(0, 0, 0, 127);

        })";
    const QString undoneTextEditSheet = R"(QWidget
        {
            background: rgba(255,255,255,255); color: rgba(0, 0, 0, 255);

        })";
    const QString selectedTaskBar = R"(#activeBtn
        {
            background-color:rgba(0, 255, 0, 96)

        })";
    const QString unselectedTaskBar = R"(#activeBtn
        {
            background-color:rgba(255, 0, 0, 96)

        })";
    const QString viewLabelValue = "Currently in View Mode";
    const QString editLabelValue = "Currently in Edit Mode";
    const QString deleteButtonText[2] = { "Cancel", "Delete" };
    const QString taskStatusText[2] = {  "Mark as incompleted", "Mark as completed" };
    bool isCreated = false;
    bool isEditMode = false;
    bool isSelected = false;
    bool isViewMode = false;

    Ui::taskQT* ui;
    TaskInputFilter* filter;
    QGraphicsBlurEffect* opacityEffect;
    std::shared_ptr<Task> task;

    void OnTaskTitleChanged();

    void ChangeEditModeState(bool value);

    void ProceedTaskModifications();
    void CancelTaskModifications();

    void UpdateModeLabel(QString val);
    void SetState(bool done);
};
#endif // TASKQT_H
