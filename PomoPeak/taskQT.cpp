#include "taskQT.h"
#include "./ui_taskQT.h"
taskQT::taskQT(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::taskQT)
    , task(std::make_shared<Task>())
{
    ui->setupUi(this);
    OnTaskTitleChanged();
    connect(ui->okBtn,&QPushButton::clicked, this, &taskQT::OnModify);
    connect(ui->delBtn,&QPushButton::clicked, this, &taskQT::OnDelete);
    connect(ui->taskName,&QTextEdit::textChanged, this, &taskQT::OnTaskTitleChanged);
}

taskQT::~taskQT()
{
    delete ui;
}
void taskQT::UpdateElapsed(int elapsed)
{
    ui->taskCurrent->setText(QString::number(elapsed));
}
void taskQT::OnModify()
{
    task->title = ui->taskName->toPlainText();
    task->description = ui->taskDescription->toPlainText();
    task->pomodorodsToDo = ui->taskCurrent->toPlainText().toInt();
    task->pomodorosDone = ui->taskEstimate->toPlainText().toInt();
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
    qDebug() << ui->taskName->toPlainText().trimmed().length();
    if(ui->taskName->toPlainText().trimmed().length() > MIN_TITLE_SIZE)
    {
        ui->okBtn->setEnabled(true);
    }
    else
    {
        ui->okBtn->setEnabled(false);
    }
}
