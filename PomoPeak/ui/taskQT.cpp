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
    ui->setupUi(this);

    InitializeDataContainer();
    InitializeObjects();
    SubscribeToEvents();
}

taskQT::~taskQT()
{
    delete ui;
    delete filter;
    delete opacityEffect;
}

void taskQT::InitializeDataContainer()
{

}

void taskQT::InitializeObjects()
{
    setStyleSheet(selectedTaskWidgetSheet);
    opacityEffect->setBlurRadius(UNDONE_BLUR);

    ui->taskLayout->setGraphicsEffect(opacityEffect);
    ui->activeBtn->setStyleSheet(unselectedTaskBar);
    ui->activeBtn->setVisible(false);

    OnTaskTitleChanged();

    ChangeMode(Mode::View);

    OnModify(ModifyState::Enter);
}

void taskQT::SubscribeToEvents()
{
    connect(ui->okBtn, &QPushButton::clicked, this, &taskQT::OnProceedButton);
    connect(ui->delBtn, &QPushButton::clicked, this, &taskQT::OnCancelButton);
    connect(ui->modifyBtn, &QPushButton::clicked, this, &taskQT::OnModifyButton);
    connect(ui->taskName, &QTextEdit::textChanged, this, &taskQT::OnTaskTitleChanged);
    connect(ui->activeBtn, &QPushButton::clicked, this, &taskQT::SwitchTaskActivation);
    connect(ui->taskStatusBtn, &QPushButton::clicked, this, &taskQT::OnChangeStatus);
}

void taskQT::OnModifyButton()
{
    if(CurrentMode != Mode::Edit)
    {
        OnModify(ModifyState::Enter);
    }
}

void taskQT::OnProceedButton()
{
    CurrentMode == Mode::Edit ? OnModify(ModifyState::Proceed) : ChangeMode(Mode::None);
}

void taskQT::OnCancelButton()
{
    if(!isCreated)
    {
        OnDelete();
        return;
    }
    CurrentMode == Mode::Edit ? OnModify(ModifyState::Cancel) :  OnDelete();
}

void taskQT::OnModify(ModifyState state)
{
    switch (state) {
    case ModifyState::Enter:
        ChangeMode(Mode::Edit);

        ui->modifyBtn->setEnabled(false);
        UpdateModeLabel(editLabelValue);
        break;
    case ModifyState::Proceed:
        ProceedTaskModifications();
        if(!isCreated)
        {
            OnCreate();
        }
        break;
    case ModifyState::Cancel:
        CancelTaskModifications();
        break;
    case ModifyState::Exit:
        //nothingggg
        break;
    default:
        break;
    }

    if(state == ModifyState::Proceed || state == ModifyState::Cancel || state == ModifyState::Exit)
    {
        ChangeMode(Mode::View);
        ui->modifyBtn->setEnabled(true);
        UpdateModeLabel(viewLabelValue);
    }
}

void taskQT::OnCreate()
{
    emit CreateRequest(task);
    isCreated = true;
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

void taskQT::SwitchTaskActivation()
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

void taskQT::ChangeMode(Mode mode)
{
    if(CurrentMode == mode)
    {
        return;
    }
    //General
    setStyleSheet(mode != Mode::None ? selectedTaskWidgetSheet : unselectedTaskWidgetSheet);

    //View
    ui->modifyBtn->setVisible(mode != Mode::None);
    ui->okBtn->setVisible(mode != Mode::None);
    ui->delBtn->setVisible(mode != Mode::None);
    ui->modeLabel->setVisible(mode != Mode::None);

    ui->taskStatusBtn->setEnabled(mode == Mode::None);
    ui->activeBtn->setVisible(mode == Mode::None);


    //Edit

    if(mode != Mode::None)
    {
        ui->taskName->setTextInteractionFlags(mode == Mode::Edit ? Qt::TextEditable : Qt::NoTextInteraction);
        ui->taskDescription->setTextInteractionFlags(mode == Mode::Edit ? Qt::TextEditable : Qt::NoTextInteraction);
        ui->currentSpinBox->setEnabled(mode == Mode::Edit);
        ui->estimationSpinBox->setEnabled(mode == Mode::Edit);
        ui->delBtn->setText(mode == Mode::Edit ? deleteButtonText[0] : deleteButtonText[1]);
    }

    //Post
    if(mode == Mode::View)
    {
        emit OnEnableViewModeRequest(this);
    }
    CurrentMode = mode;
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

    ui->taskStatusBtn->setText(done ? taskStatusText[0] : taskStatusText[1]);

    ui->taskName->setStyleSheet(done ? doneTextEditSheet : undoneTextEditSheet);
    ui->currentSpinBox->setStyleSheet(done ? doneTextEditSheet : undoneTextEditSheet);
    ui->taskDescription->setStyleSheet(done ? doneTextEditSheet : undoneTextEditSheet);
    ui->estimationSpinBox->setStyleSheet(done ? doneTextEditSheet : undoneTextEditSheet);

    opacityEffect->setBlurRadius(done ? DONE_BLUR : UNDONE_BLUR);
}

//Called from pomopeak.cpp when session is finished
void taskQT::IncreasePomodorosDone()
{
    ui->currentSpinBox->setValue(ui->currentSpinBox->text().toInt() + 1);
    task->pomodorosDone++;
}

void taskQT::UpdateModeLabel(QString val)
{
    ui->modeLabel->setText(val);
}
