#include "taskQT.h"
#include "./ui_taskQT.h"
taskQT::taskQT(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::taskQT)
    , filter(new TaskInputFilter)
    , opacityEffect(new QGraphicsBlurEffect(this))
    , task(std::make_shared<Task>())
{
    setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(selectedTaskWidgetSheet);

    ui->setupUi(this);
    opacityEffect->setBlurRadius(UNDONE_BLUR);
    ui->taskLayout->setGraphicsEffect(opacityEffect);
    OnTaskTitleChanged();

    EnableViewMode();


    OnModifyEnter();

    ui->activeBtn->setStyleSheet(unselectedTaskBar);
    ui->activeBtn->setVisible(false);

    connect(ui->okBtn, &QPushButton::clicked, this, &taskQT::OnProceedButton);
    connect(ui->delBtn, &QPushButton::clicked, this, &taskQT::OnCancelButton);
    connect(ui->modifyBtn, &QPushButton::clicked, this, &taskQT::OnModifyButton);
    connect(ui->taskName, &QTextEdit::textChanged, this, &taskQT::OnTaskTitleChanged);
    connect(ui->activeBtn, &QPushButton::clicked, this, &taskQT::SwitchSelectState);
    connect(ui->taskStatusBtn, &QPushButton::clicked, this, &taskQT::OnChangeStatus);
}

taskQT::~taskQT()
{
    delete ui;
    delete filter;
}

void taskQT::OnModifyButton()
{
    if(!isEditMode)
    {
        OnModifyEnter();
    }
}

void taskQT::OnProceedButton()
{
    isEditMode ? OnModifyProceed() : DisableViewMode();
}

void taskQT::OnCancelButton()
{
    if(!isCreated)
    {
        OnDelete();
        return;
    }
    isEditMode ? OnModifyCancel() :  OnDelete();
}

void taskQT::OnModifyProceed()
{
    ProceedTaskModifications();
    if(!isCreated)
    {
        OnCreate();
    }
    OnModifyExit();
}

void taskQT::OnCreate()
{
    emit CreateRequest(task);
    isCreated = true;
}

void taskQT::OnModifyCancel()
{
    CancelTaskModifications();
    OnModifyExit();
}

void taskQT::OnModifyEnter()
{
    EnableEditMode();

    ui->modifyBtn->setEnabled(false);
    UpdateModeLabel(editLabelValue);
}

void taskQT::OnModifyExit()
{
    DisableEditMode();

    ui->modifyBtn->setEnabled(true);
    UpdateModeLabel(viewLabelValue);
}

void taskQT::OnDelete()
{
    if(isCreated)
    {
        emit DeleteRequest(task);
    }
    taskQT::~taskQT();
}

void taskQT::ProceedTaskModifications()
{
    task->title = ui->taskName->toPlainText();
    task->description = ui->taskDescription->toPlainText();

    task->pomodorodsToDo = ui->estimationSpinBox->text().toInt();

    task->pomodorosDone = ui->currentSpinBox->text().toInt();
    UpdateTimeSpent();
}

void taskQT::CancelTaskModifications()
{
    ui->taskName->setText(task->title);
    ui->taskDescription->setText(task->description);
    ui->estimationSpinBox->setValue(task->pomodorodsToDo);
    ui->currentSpinBox->setValue(task->pomodorosDone);
}

void taskQT::SwitchSelectState()
{
    isSelected = !isSelected;
    if(isSelected)
    {
        ui->activeBtn->setStyleSheet(selectedTaskBar);
        emit OnSelectRequest(this);
    }
    else
    {
        ui->activeBtn->setStyleSheet(unselectedTaskBar);
    }
}

void taskQT::EnableViewMode()
{
    if(isViewMode)
        return;
    isViewMode = true;
    this->setStyleSheet(selectedTaskWidgetSheet);
    ui->modifyBtn->setVisible(true);
    ui->okBtn->setVisible(true);
    ui->delBtn->setVisible(true);

    ui->activeBtn->setVisible(false);
    ui->modeLabel->setVisible(true);

    ui->taskStatusBtn->setEnabled(false);

    emit OnEnableViewModeRequest(this);
}

void taskQT::DisableViewMode()
{
    if(!isViewMode)
        return;
    isViewMode = false;
    this->setStyleSheet(unselectedTaskWidgetSheet);
    ui->modifyBtn->setVisible(false);
    ui->okBtn->setVisible(false);
    ui->delBtn->setVisible(false);

    ui->activeBtn->setVisible(true);
    ui->modeLabel->setVisible(false);

    ui->taskStatusBtn->setEnabled(true);
}

void taskQT::EnableEditMode()
{
    isEditMode = true;
    ui->taskName->setTextInteractionFlags(Qt::TextEditable);
    ui->taskDescription->setTextInteractionFlags(Qt::TextEditable);
    ui->currentSpinBox->setEnabled(true);
    ui->estimationSpinBox->setEnabled(true);
    ui->delBtn->setText("Cancel");
}

void taskQT::DisableEditMode()
{
    isEditMode = false;
    ui->taskName->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->taskDescription->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->currentSpinBox->setEnabled(false);
    ui->estimationSpinBox->setEnabled(false);
    ui->delBtn->setText("Delete");
}

void taskQT::OnTaskTitleChanged()
{
    ui->taskName->toPlainText().trimmed().length() > MIN_TITLE_SIZE
        ? ui->okBtn->setEnabled(true)
        : ui->okBtn->setEnabled(false);
}
void taskQT::OnChangeStatus()
{
    task->isDone = !task->isDone;

    if(task->isDone)
    {
        ui->taskStatusBtn->setText("Mark as incompleted");
        SetAsDone();
    }
    else
    {
        ui->taskStatusBtn->setText("Mark as completed");
        SetAsUndone();
    }
}
void taskQT::SetAsDone()
{
    task->isDone = true;

    //TEMPORARY, no idea for another solution, looks like opaciy from parent container doesnt work on QTextEdit objects
    ui->taskName->setStyleSheet(doneTextEditSheet);
    ui->currentSpinBox->setStyleSheet(doneTextEditSheet);
    ui->taskDescription->setStyleSheet(doneTextEditSheet);
    ui->estimationSpinBox->setStyleSheet(doneTextEditSheet);

    opacityEffect->setBlurRadius(DONE_BLUR);
}
void taskQT::SetAsUndone()
{
    task->isDone = false;

    //TEMPORARY, no idea for another solution, looks like opaciy from parent container doesnt work on QTextEdit objects
    ui->taskName->setStyleSheet(undoneTextEditSheet);
    ui->currentSpinBox->setStyleSheet(undoneTextEditSheet);
    ui->taskDescription->setStyleSheet(undoneTextEditSheet);
    ui->estimationSpinBox->setStyleSheet(undoneTextEditSheet);

    opacityEffect->setBlurRadius(UNDONE_BLUR);
}
//Called from pomopeak.cpp when session is finished
void taskQT::ElapsedIncrease()
{
    int v = ui->currentSpinBox->text().toInt();
    ui->currentSpinBox->setValue(v+1);
    task->pomodorosDone++;
    UpdateTimeSpent();

}

void taskQT::UpdateTimeSpent()
{
    //Change it
    //task->spentTime = task->pomodorosDone * PomoSettings::SessionDuration;
    ui->timeSpentOnTask->setText(QString::number(task->spentTime/60));
}
void taskQT::UpdateModeLabel(QString val)
{
    ui->modeLabel->setText(val);
}
