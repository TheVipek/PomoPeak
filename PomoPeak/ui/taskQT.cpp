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
    baseStyleSheet = this->styleSheet();

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
    setStyleSheet(baseStyleSheet + selectedTaskWidgetSheet);
    opacityEffect->setBlurRadius(UNDONE_BLUR);

    ui->taskLayout->setGraphicsEffect(opacityEffect);
    ui->activeBtn->setChecked(false);
    ui->activeBtn->setVisible(false);
    OnTaskTitleChanged();

    OnModify(ModifyState::Enter);
}

void taskQT::SubscribeToEvents()
{
    connect(ui->okBtn, &QPushButton::clicked, this, &taskQT::OnProceedButton);
    connect(ui->delBtn, &QPushButton::clicked, this, &taskQT::OnDeleteButton);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &taskQT::OnCancelButton);
    connect(ui->taskName, &QTextEdit::textChanged, this, &taskQT::OnTaskTitleChanged);
    connect(ui->activeBtn, &QPushButton::clicked, this, &taskQT::SwitchTaskActivation);
    connect(ui->taskStatusBtn, &QPushButton::clicked, this, &taskQT::OnChangeStatus);
    connect(ui->openSettingsBtn, &QPushButton::clicked, this, &taskQT::OpenSettings);
}
\
void taskQT::OpenSettings()
{
    if(CurrentMode != Mode::View && !task->isDone)
    {
        ChangeMode(Mode::View);
    }
}

void taskQT::OnCancelButton()
{
    if(!isCreated)
    {
        OnDelete();
        return;
    }
    if(CurrentMode == Mode::View)
    {
        OnModify(ModifyState::Cancel);
    }
}

void taskQT::OnProceedButton()
{
    OnModify(ModifyState::Proceed);
}

void taskQT::OnDeleteButton()
{
    OnDelete();
}

void taskQT::OnModify(ModifyState state)
{
    switch (state) {
    case ModifyState::Enter:
        ChangeMode(Mode::View);
        break;
    case ModifyState::Proceed:
        ProceedTaskModifications();
        if(!isCreated)
        {
            OnCreate();
        }
        ChangeMode(Mode::None);
        break;
    case ModifyState::Cancel:
        CancelTaskModifications();
        ChangeMode(Mode::None);
        break;
    case ModifyState::Exit:
        //nothingggg
        break;
    default:
        break;
    }
}

void taskQT::OnCreate()
{
    emit CreateRequest(task);
    isCreated = true;
}

void taskQT::OnDelete()
{
    emit DeleteRequest(task, this);
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
    ui->activeBtn->setChecked(isSelected);
}

void taskQT::ChangeMode(Mode mode)
{
    if(CurrentMode == mode)
    {
        return;
    }
    //General
    setStyleSheet(mode != Mode::None ? baseStyleSheet + selectedTaskWidgetSheet : baseStyleSheet + unselectedTaskWidgetSheet);

    //View
    ui->cancelBtn->setVisible(mode == Mode::View);
    ui->okBtn->setVisible(mode == Mode::View);
    ui->delBtn->setVisible(mode == Mode::View);

    ui->taskStatusBtn->setEnabled(mode == Mode::None);
    ui->activeBtn->setVisible(mode == Mode::None);
    ui->openSettingsBtn->setVisible(mode == Mode::None);


    ui->taskName->setTextInteractionFlags(mode == Mode::View ? Qt::TextEditable : Qt::NoTextInteraction);
    ui->taskDescription->setTextInteractionFlags(mode == Mode::View ? Qt::TextEditable : Qt::NoTextInteraction);
    ui->currentSpinBox->setEnabled(mode == Mode::View);
    ui->estimationSpinBox->setEnabled(mode == Mode::View);


    //Post
    if(mode == Mode::View)
    {
        emit OnEnableViewModeRequest(this);
    }

    if(mode == Mode::None)
    {
        emit OnNoneModeRequest(this);
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

