#include "taskQT.h"
#include "./ui_taskQT.h"
taskQT::taskQT(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::taskQT)
    , task(std::make_shared<Task>())
{
    filter = new TaskInputFilter;
    ui->setupUi(this);


    OnTaskTitleChanged();
    EnableEditMode();

    connect(ui->okBtn,&QPushButton::clicked, this, &taskQT::OnModify);
    connect(ui->delBtn,&QPushButton::clicked, this, &taskQT::OnDelete);
    connect(ui->taskName,&QTextEdit::textChanged, this, &taskQT::OnTaskTitleChanged);

    ui->taskEstimate->setText("0");
    ui->taskCurrent->setText("0");

    ui->taskEstimate->setAlignment(Qt::AlignCenter);
    ui->taskCurrent->setAlignment(Qt::AlignCenter);

    ui->taskEstimate->installEventFilter(filter);
    ui->taskCurrent->installEventFilter(filter);
}

taskQT::~taskQT()
{
    delete ui;
    delete filter;
}

void taskQT::OnCreate()
{
    emit CreateRequest(task);
    isCreated = true;
}

void taskQT::ElapsedResponse()
{
    int v = ui->taskCurrent->toPlainText().toInt();
    ui->taskCurrent->setText(QString::number(v+1));
}

void taskQT::OnDelete()
{
    //User may want to click 'delete' before actually creating task, so no need to inform the rest of stuff
    if(isCreated)
    {
        emit DeleteRequest(task);
    }
    taskQT::~taskQT();
}

void taskQT::OnModify()
{
    UpdateTask();
    if(!isCreated)
    {
        OnCreate();
    }
    DisableEditMode();
}

void taskQT::UpdateTask()
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

void taskQT::EnableEditMode()
{
    isEditMode = true;
    ui->taskName->setTextInteractionFlags(Qt::TextEditable);
    ui->taskDescription->setTextInteractionFlags(Qt::TextEditable);
    ui->taskCurrent->setTextInteractionFlags(Qt::TextEditable);
    ui->taskEstimate->setTextInteractionFlags(Qt::TextEditable);
    ui->okBtn->setVisible(true);
    ui->delBtn->setVisible(true);
}

void taskQT::DisableEditMode()
{
    isEditMode = false;
    ui->taskName->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->taskDescription->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->taskCurrent->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->taskEstimate->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->okBtn->setVisible(false);
    ui->delBtn->setVisible(false);
}

void taskQT::OnTaskTitleChanged()
{
    if(ui->taskName->toPlainText().trimmed().length() > MIN_TITLE_SIZE)
    {
        ui->okBtn->setEnabled(true);
    }
    else
    {
        ui->okBtn->setEnabled(false);
    }
}
