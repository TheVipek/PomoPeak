#include "pomopeakstats.h"
#include "ui_pomopeakstats.h"
PomopeakStats::PomopeakStats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PomopeakStats)
{
    ui->setupUi(this);

    connect(ui->exitButton, &QPushButton::clicked, this, &PomopeakStats::OnExitClicked);
    connect(ui->monthlyBtn, &QPushButton::clicked, this, &PomopeakStats::OnViewButtonsClick);
    connect(ui->weeklyBtn, &QPushButton::clicked, this, &PomopeakStats::OnViewButtonsClick);
    SwitchViewToWeekly();
}


PomopeakStats::~PomopeakStats()
{
    delete ui;
}

void PomopeakStats::OnExitClicked()
{
    emit OnClose();
}

void PomopeakStats::OnViewButtonsClick()
{
    QObject* obj = sender();

    if(obj == ui->weeklyBtn)
    {
        qDebug() << "Clicked at; " << obj->objectName();
        SwitchViewToWeekly();
    }
    else if(obj == ui->monthlyBtn)
    {
        qDebug() << "Clicked at; " << obj->objectName();
        SwitchViewToMonthly();
    }
}


void PomopeakStats::SwitchViewToMonthly()
{
    ui->monthlyBtn->setEnabled(false);
    ui->weeklyBtn->setEnabled(true);
}
void PomopeakStats::SwitchViewToWeekly()
{
    ui->weeklyBtn->setEnabled(false);
    ui->monthlyBtn->setEnabled(true);
}
