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

    InitializeDataContainer();
    InitializeObjects();
    SubscribeToEvents();


    SwtichChartView(ChartVisibility::Weekly);
}

PomopeakStats::~PomopeakStats()
{
    delete ui;
}

void PomopeakStats::InitializeDataContainer()
{
    InitializeChart();
}

void PomopeakStats::InitializeObjects()
{
    showTooltipTimer = new QTimer(this);
    showTooltipTimer->setSingleShot(true);
}

void PomopeakStats::SubscribeToEvents()
{
    connect(ui->exitButton, &QPushButton::clicked, this, &PomopeakStats::OnExitClicked);
    connect(ui->monthlyBtn, &QPushButton::clicked, this, &PomopeakStats::OnViewButtonsClick);
    connect(ui->weeklyBtn, &QPushButton::clicked, this, &PomopeakStats::OnViewButtonsClick);
    connect(showTooltipTimer, &QTimer::timeout, this, &PomopeakStats::ShowBarText);
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
        SwtichChartView(ChartVisibility::Weekly);
    }
    else if(obj == ui->monthlyBtn)
    {
        qDebug() << "Clicked at; " << obj->objectName();
        SwtichChartView(ChartVisibility::Monthly);
    }
}

void PomopeakStats::SwtichChartView(ChartVisibility visibility)
{
    currentVisibility = visibility;
    if(visibility == ChartVisibility::Weekly)
    {
        ui->weeklyBtn->setEnabled(false);
        ui->monthlyBtn->setEnabled(true);
    }
    else if(visibility == ChartVisibility::Monthly)
    {
        ui->monthlyBtn->setEnabled(false);
        ui->weeklyBtn->setEnabled(true);
    }
    UpdateChart(VisibilitySettings[visibility]);
}

void PomopeakStats::InitializeChart()
{
    chart = new QChart();
    chart->setTitle("Title");

    QChartView* view = new QChartView(chart);
    ui->chartLayout->addWidget(view);
}
void PomopeakStats::UpdateChart(int days)
{
    if(allSets != nullptr)
        chart->removeAllSeries();

    if(daysAxis != nullptr)
        chart->removeAxis(daysAxis);

    allSets = new QBarSeries();

    completedTasksSet = new QBarSet("Task Count");
    taskTimeSet = new QBarSet("Time Spend (in hours)");

    connect(completedTasksSet, &QBarSet::hovered,this, &PomopeakStats::OnHoverBar);
    connect(taskTimeSet, &QBarSet::hovered,this, &PomopeakStats::OnHoverBar);

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

    QStringList dates = QStringList();

    for(int i = 0;i <= days; i++)
    {
        dates.append(startDate.toString("MM-dd"));
        startDate = startDate.addDays(1);
    }

    daysAxis = new QBarCategoryAxis();
    daysAxis->append(dates);


    chart->addSeries(allSets);
    chart->addAxis(daysAxis, Qt::AlignBottom);
    allSets->attachAxis(daysAxis);
}

void PomopeakStats::OnHoverBar(bool status, int index)
{

    QBarSet* set = qobject_cast<QBarSet*>(sender());
    if(status)
    {
        tooltipText = QString("%1: %2").arg(set->label()).arg(set->at(index));
        showTooltipTimer->start(500);
    }
    else
    {
        showTooltipTimer->stop();
        QToolTip::hideText();
    }
}
void PomopeakStats::ShowBarText()
{
    QToolTip::showText(QCursor::pos(), tooltipText);
}

void PomopeakStats::ForceUpdateChart()
{
    UpdateChart(currentVisibility);
}
