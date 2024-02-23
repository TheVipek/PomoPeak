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
    connect(ui->okBtn,&QPushButton::clicked, this, &taskQT::OnModify);
    connect(ui->delBtn,&QPushButton::clicked, this, &taskQT::OnDelete);
    connect(ui->taskName,&QTextEdit::textChanged, this, &taskQT::OnTaskTitleChanged);

    ui->taskEstimate->setText("0");
    ui->taskEstimate->setAlignment(Qt::AlignCenter);
    ui->taskCurrent->setText("0");
     ui->taskCurrent->setAlignment(Qt::AlignCenter);
    ui->taskEstimate->installEventFilter(filter);
    ui->taskCurrent->installEventFilter(filter);
}

taskQT::~taskQT()
{
    delete ui;
    delete filter;
}
void taskQT::ElapsedResponse()
{
    int v = ui->taskCurrent->toPlainText().toInt();
    ui->taskCurrent->setText(QString::number(v+1));
}
void taskQT::OnModify()
{
    task->title = ui->taskName->toPlainText();
    task->description = ui->taskDescription->toPlainText();
    QString toDoText = ui->taskEstimate->toPlainText();

    //should be changed in future
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

    if(!isCreated)
    {
        OnCreate();
    }
    ui->taskName->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->taskDescription->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->taskCurrent->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->taskEstimate->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->okBtn->setVisible(false);
    ui->delBtn->setVisible(false);
}
void taskQT::OnDelete()
{
    emit DeleteRequest(task);
}
void taskQT::OnCreate()
{
    emit CreateRequest(task);
    isCreated = true;
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
