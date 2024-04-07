#ifndef TASKQT_H
#define TASKQT_H

#include <QWidget>
#include "task.h"
#include <memory>
#include "TaskInputFilter.h"
#include <QGraphicsBlurEffect>
#include "QObjectInitialization.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class taskQT;
}

QT_END_NAMESPACE

class taskQT: public QWidget, public QObjectInitialization
{
    Q_OBJECT




public:
    explicit taskQT(QWidget *parent = nullptr);
    ~taskQT();

    // Enter -> Enter edit mode
    // Exit -> Exit edit mode
    // Cancel -> Cancel all changes made in edit mode and exit
    // Proceed -> Proceed all changes made in edit mode and exit
    enum class ModifyState
    {
        Enter,
        Exit,
        Cancel,
        Proceed
    };

    // Represents currently active mode
    enum class Mode
    {
        View,
        Edit,
        None
    };

    void ChangeMode(Mode mode);
    void IncreasePomodorosDone();
    void SwitchTaskActivation();

signals:
    void DeleteRequest(std::shared_ptr<Task> task, taskQT* taskU);
    void CreateRequest(std::shared_ptr<Task> task);
    void OnEnableViewModeRequest(taskQT* ui);
    void OnSelectRequest(taskQT* ui);
    void OnStatusChanged(bool done);
protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if(event->button() == Qt::LeftButton && CurrentMode != Mode::View && !task->isDone)
        {
            ChangeMode(Mode::View);
        }
    }
    void InitializeDataContainer() override;
    void InitializeObjects() override;
    void SubscribeToEvents() override;

private slots:

    void OnModifyButton();
    void OnProceedButton();
    void OnCancelButton();

    void OnModify(ModifyState state);
    void OnDelete();
    void OnCreate();

    void OnChangeStatus();
private:
    const int MIN_TITLE_SIZE = 3;
    const float UNDONE_BLUR = 0;
    const float DONE_BLUR = 20.0f;
    QString baseStyleSheet;
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
    const QString viewLabelValue = "Currently in View Mode";
    const QString editLabelValue = "Currently in Edit Mode";
    const QString deleteButtonText[2] = { "Cancel", "Delete" };
    const QString taskStatusText[2] = {  "Mark as incompleted", "Mark as completed" };

    bool isCreated = false;
    bool isSelected = false;
    Mode CurrentMode = Mode::None;

    Ui::taskQT* ui;
    TaskInputFilter* filter;
    QGraphicsBlurEffect* opacityEffect;
    std::shared_ptr<Task> task;

    void OnTaskTitleChanged();

    void ProceedTaskModifications();
    void CancelTaskModifications();

    void UpdateModeLabel(QString val);
    void SetState(bool done);
};
#endif // TASKQT_H
