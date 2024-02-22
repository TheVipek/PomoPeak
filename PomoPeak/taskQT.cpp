#include "taskQT.h"
#include "./ui_taskQT.h"
taskQT::taskQT(std::shared_ptr<Task> task, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::taskQT)
    , task(task)
{
    Init();
    connect(ui->okBtn,&QPushButton::clicked, this, &taskQT::OnModify);

}

taskQT::~taskQT()
{
    delete ui;
}
void taskQT::UpdateElapsed(int elapsed)
{
    ui->taskCurrent->setText(QString::number(elapsed));
}
void taskQT::Init()
{
    ui->taskName->setText(task->title);
    ui->taskDescription->setText(task->description);
    ui->taskCurrent->setText(QString::number(task->pomodorosDone));
    ui->taskEstimate->setText(QString::number(task->pomodorodsToDo));

}
void taskQT::OnModify()
{
    task->title = ui->taskName->toPlainText();
    task->description = ui->taskDescription->toPlainText();
    task->pomodorodsToDo = ui->taskCurrent->toPlainText().toInt();
    task->pomodorosDone = ui->taskEstimate->toPlainText().toInt();
}
