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
        None
    };

    void ChangeMode(Mode mode);
    void IncreasePomodorosDone();
    void SwitchTaskActivation();

signals:
    void DeleteRequest(std::shared_ptr<Task> task, taskQT* taskU);
    void CreateRequest(std::shared_ptr<Task> task);
    void OnEnableViewModeRequest(taskQT* ui);
    void OnNoneModeRequest(taskQT* ui);
    void OnSelectRequest(taskQT* ui);
    void OnStatusChanged(bool done);
protected:
    void InitializeDataContainer() override;
    void InitializeObjects() override;
    void SubscribeToEvents() override;
    void UnsubscribeFromEvents() override;

private slots:
    void OnProceedButton();
    void OnCancelButton();
    void OnDeleteButton();
    void OnModify(ModifyState state);
    void OnDelete();
    void OnCreate();

    void OnChangeStatus();

    void OpenSettings();
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

    bool isCreated = false;
    bool isSelected = false;
    Mode CurrentMode = Mode::None;

    Ui::taskQT* ui;
    QGraphicsBlurEffect* opacityEffect;
    std::shared_ptr<Task> task;

    void OnTaskTitleChanged();

    void ProceedTaskModifications();
    void CancelTaskModifications();
    void SetState(bool done);
};
#endif // TASKQT_H
