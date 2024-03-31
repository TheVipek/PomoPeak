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

    ChangeViewModeState(true);

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
    delete opacityEffect;
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
    isEditMode ? OnModifyProceed() : ChangeViewModeState(false);
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
    ChangeEditModeState(true);

    ui->modifyBtn->setEnabled(false);
    UpdateModeLabel(editLabelValue);
}

void taskQT::OnModifyExit()
{
    ChangeEditModeState(false);

    ui->modifyBtn->setEnabled(true);
    UpdateModeLabel(viewLabelValue);
}

void taskQT::OnDelete()
{
    if(isCreated)
    {
        emit DeleteRequest(task, this);
    }
}

void taskQT::ProceedTaskModifications()
{
    task->title = ui->taskName->toPlainText();
    task->description = ui->taskDescription->toPlainText();

    task->pomodorodsToDo = ui->estimationSpinBox->text().toInt();

    task->pomodorosDone = ui->currentSpinBox->text().toInt();
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

void taskQT::ChangeViewModeState(bool enabled)
{
    if(isViewMode == enabled)
        return;

    isViewMode = enabled;
    this->setStyleSheet(enabled ? selectedTaskWidgetSheet : unselectedTaskWidgetSheet);
    ui->modifyBtn->setVisible(enabled);
    ui->okBtn->setVisible(enabled);
    ui->delBtn->setVisible(enabled);

    ui->activeBtn->setVisible(!enabled);
    ui->modeLabel->setVisible(enabled);

    ui->taskStatusBtn->setEnabled(!enabled);

    if(enabled)
    {
        emit OnEnableViewModeRequest(this);
    }
}

void taskQT::ChangeEditModeState(bool enabled)
{
    isEditMode = enabled;
    ui->taskName->setTextInteractionFlags(enabled ? Qt::TextEditable : Qt::NoTextInteraction);
    ui->taskDescription->setTextInteractionFlags(enabled ? Qt::TextEditable : Qt::NoTextInteraction);
    ui->currentSpinBox->setEnabled(enabled);
    ui->estimationSpinBox->setEnabled(enabled);
    ui->delBtn->setText(enabled ? deleteButtonText[0] : deleteButtonText[1]);
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

    SetState(task->isDone);
    emit OnStatusChanged(task->isDone);
}

void taskQT::SetState(bool done)
{
    task->isDone = done;

    //TEMPORARY, no idea for another solution, looks like opaciy from parent container doesnt work on QTextEdit objects
    ui->taskStatusBtn->setText(done ? taskStatusText[0] : taskStatusText[1]);

    ui->taskName->setStyleSheet(done ? doneTextEditSheet : undoneTextEditSheet);
    ui->currentSpinBox->setStyleSheet(done ? doneTextEditSheet : undoneTextEditSheet);
    ui->taskDescription->setStyleSheet(done ? doneTextEditSheet : undoneTextEditSheet);
    ui->estimationSpinBox->setStyleSheet(done ? doneTextEditSheet : undoneTextEditSheet);

    opacityEffect->setBlurRadius(done ? DONE_BLUR : UNDONE_BLUR);
}

//Called from pomopeak.cpp when session is finished
void taskQT::ElapsedIncrease()
{
    ui->currentSpinBox->setValue(ui->currentSpinBox->text().toInt() + 1);
    task->pomodorosDone++;
}

void taskQT::UpdateModeLabel(QString val)
{
    ui->modeLabel->setText(val);
}
