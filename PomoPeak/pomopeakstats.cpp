#include "pomopeakstats.h"
#include "ui_pomopeakstats.h"
#include <QToolTip>
PomopeakStats::PomopeakStats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PomopeakStats)
{
    ui->setupUi(this);

    connect(ui->exitButton, &QPushButton::clicked, this, &PomopeakStats::OnExitClicked);
    connect(ui->monthlyBtn, &QPushButton::clicked, this, &PomopeakStats::OnViewButtonsClick);
    connect(ui->weeklyBtn, &QPushButton::clicked, this, &PomopeakStats::OnViewButtonsClick);
    SwitchViewToWeekly();
    InitializeChart();

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
void PomopeakStats::InitializeChart()
{
    chart = new QChart();
    chart->setTitle("Title");
    QBarSet* tasksCount = new QBarSet("Task Count");
    *tasksCount  << 1 << 2 << 3 << 4 << 5;

    QBarSet* timeSpent = new QBarSet("Time Spent");
    *timeSpent << 1 << 2 << 3 << 4 << 5;
    QBarSeries* series = new QBarSeries();

    series->append(tasksCount );
    series->append(timeSpent);
    QObject::connect(series, &QBarSeries::hovered, [=](bool status, int index, QBarSet *set) {
        if (status) {
            // Assuming you want to show the set name and value in the tooltip
            QToolTip::showText(QCursor::pos(), QString("%1: %2").arg(set->label()).arg(set->at(index)));
        }
    });
    chart->addSeries(series);
    QChartView* view = new QChartView(chart);
    ui->chartLayout->addWidget(view);


}
