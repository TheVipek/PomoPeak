#include "taskQT.h"
#include "./ui_taskQT.h"
taskQT::taskQT(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::taskQT)
    , filter(new TaskInputFilter)
    , task(std::make_shared<Task>())
{
    ui->setupUi(this);

    OnTaskTitleChanged();

    isActive = true;




    EnableEditMode();

    ui->activeBtn->setStyleSheet(unselectedTaskSheet);
    ui->activeBtn->setVisible(false);

    connect(ui->okBtn,&QPushButton::clicked, this, &taskQT::OnProceedButton);

    connect(ui->delBtn,&QPushButton::clicked, this, &taskQT::OnCancelButton);

    connect(ui->modifyBtn,&QPushButton::clicked, this, &taskQT::OnModifyButton);

    connect(ui->taskName,&QTextEdit::textChanged, this, &taskQT::OnTaskTitleChanged);

    connect(ui->activeBtn,&QPushButton::clicked, this, &taskQT::OnChangeSelectState);


    ui->taskEstimate->installEventFilter(filter);
    ui->taskCurrent->installEventFilter(filter);
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
    isEditMode ? OnModifyProceed() : Deactive();
}

void taskQT::OnCancelButton()
{
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
}
void taskQT::OnModifyExit()
{
    DisableEditMode();
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

    QString toDoText = ui->taskEstimate->toPlainText();
    if(toDoText.isEmpty())
    {
        ui->taskEstimate->setText("0");
    }
    task->pomodorodsToDo = toDoText.toInt();

    QString doneText = ui->taskCurrent->toPlainText();
    if(doneText.isEmpty())
    {
        ui->taskCurrent->setText("0");
    }
    task->pomodorosDone = doneText.toInt();
}
void taskQT::CancelTaskModifications()
{
    ui->taskName->setText(task->title);
    ui->taskDescription->setText(task->description);
    ui->taskEstimate->setText(QString::number(task->pomodorodsToDo));
    ui->taskCurrent->setText(QString::number(task->pomodorosDone));

}
void taskQT::OnChangeSelectState()
{
    isSelected = !isSelected;
    isSelected ?  ui->activeBtn->setStyleSheet(selectedTaskSheet) :  ui->activeBtn->setStyleSheet(unselectedTaskSheet);
}

void taskQT::Active()
{
    if(isActive)
        return;
    isActive = true;

    ui->modifyBtn->setVisible(true);
    ui->okBtn->setVisible(true);
    ui->delBtn->setVisible(true);

    ui->activeBtn->setVisible(false);
}
void taskQT::Deactive()
{
    if(!isActive)
        return;
    isActive = false;

    ui->modifyBtn->setVisible(false);
    ui->okBtn->setVisible(false);
    ui->delBtn->setVisible(false);

    ui->activeBtn->setVisible(true);
}

void taskQT::EnableEditMode()
{
    isEditMode = true;
    ui->taskName->setTextInteractionFlags(Qt::TextEditable);
    ui->taskDescription->setTextInteractionFlags(Qt::TextEditable);
    ui->taskCurrent->setTextInteractionFlags(Qt::TextEditable);
    ui->taskEstimate->setTextInteractionFlags(Qt::TextEditable);

    ui->modifyBtn->setEnabled(false);
}

void taskQT::DisableEditMode()
{
    isEditMode = false;
    ui->taskName->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->taskDescription->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->taskCurrent->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->taskEstimate->setTextInteractionFlags(Qt::NoTextInteraction);

    ui->modifyBtn->setEnabled(true);
}

void taskQT::OnTaskTitleChanged()
{
    ui->taskName->toPlainText().trimmed().length() > MIN_TITLE_SIZE
        ? ui->okBtn->setEnabled(true)
        : ui->okBtn->setEnabled(false);
}


void taskQT::ElapsedIncrease()
{
    int v = ui->taskCurrent->toPlainText().toInt();
    ui->taskCurrent->setText(QString::number(v+1));
}
