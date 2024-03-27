#include "pomopeakstats.h"
#include "ui_pomopeakstats.h"
#include <QToolTip>
#include "daytaskstats.h"
#include <QDate>
PomopeakStats::PomopeakStats(UserStats& stats,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PomopeakStats)
    , stats(stats)
{
    ui->setupUi(this);

    connect(ui->exitButton, &QPushButton::clicked, this, &PomopeakStats::OnExitClicked);
    connect(ui->monthlyBtn, &QPushButton::clicked, this, &PomopeakStats::OnViewButtonsClick);
    connect(ui->weeklyBtn, &QPushButton::clicked, this, &PomopeakStats::OnViewButtonsClick);

    InitializeChart();
    SwitchViewToWeekly();


    connect(completedTasksSet, &QBarSet::hovered,this, &PomopeakStats::ShowBarText);
    connect(taskTimeSet, &QBarSet::hovered,this, &PomopeakStats::ShowBarText);
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
    UpdateChart(30);
}

void PomopeakStats::SwitchViewToWeekly()
{
    ui->weeklyBtn->setEnabled(false);
    ui->monthlyBtn->setEnabled(true);
    UpdateChart(7);
}

void PomopeakStats::InitializeChart()
{
    chart = new QChart();
    chart->setTitle("Title");

    allSets = new QBarSeries();
    chart->addSeries(allSets);

    QChartView* view = new QChartView(chart);
    ui->chartLayout->addWidget(view);
}
void PomopeakStats::UpdateChart(int days)
{
    chart->removeAllSeries();

    allSets = new QBarSeries();

    completedTasksSet = new QBarSet("Task Count");
    taskTimeSet = new QBarSet("Time Spend (in hours)");

    QMap<QDate,DayTaskStats> userStats = stats.GetUserStats();
    QDate startDate = QDateTime::currentDateTime().date().addDays(-days);
    for(auto it = userStats.constBegin(); it != userStats.constEnd(); ++it)
    {
        if(it.key() >= startDate)
        {
            const auto& val = it.value();

            completedTasksSet->insert(completedTasksSet->count(), val.TaskCompletedCount);
            taskTimeSet->insert(taskTimeSet->count(), val.TimeSpendInHours);
        }
    }

    allSets->append(completedTasksSet);
    allSets->append(taskTimeSet);

    chart->addSeries(allSets);
}

void PomopeakStats::ShowBarText(bool status, int index)
{

    QBarSet* set = qobject_cast<QBarSet*>(sender());
    if(status)
    {
        QToolTip::showText(QCursor::pos(), QString("%1: %2").arg(set->label()).arg(set->at(index)));
    }
}
