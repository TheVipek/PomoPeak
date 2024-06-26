#include "pomopeakstats.h"
#include "ui_pomopeakstats.h"
#include <QToolTip>
#include "daytaskstats.h"
#include <QDate>
PomopeakStats::PomopeakStats(std::shared_ptr<UserStats> stats,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PomopeakStats)
    , stats(stats)
{
    ui->setupUi(this);

    InitializeDataContainer();
    InitializeObjects();
    SubscribeToEvents();


    SwtichChartView(ChartVisibility::Weekly);
    UpdateGlobalStats();
}

PomopeakStats::~PomopeakStats()
{
    UnsubscribeFromEvents();
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

void PomopeakStats::UnsubscribeFromEvents()
{
    disconnect(ui->exitButton, &QPushButton::clicked, this, &PomopeakStats::OnExitClicked);
    disconnect(ui->monthlyBtn, &QPushButton::clicked, this, &PomopeakStats::OnViewButtonsClick);
    disconnect(ui->weeklyBtn, &QPushButton::clicked, this, &PomopeakStats::OnViewButtonsClick);
    disconnect(showTooltipTimer, &QTimer::timeout, this, &PomopeakStats::ShowBarText);
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
        SwtichChartView(ChartVisibility::Weekly);
    }
    else if(obj == ui->monthlyBtn)
    {
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
    QChartView* view = new QChartView(this);

    chart = new QChart();
    chart->setParent(view);

    view->setChart(chart);

    chart->setBackgroundBrush(QBrush(this->palette().color(QPalette::Window)));
    chart->setTitle("Title");

    ui->chartLayout->addWidget(view);
}

void PomopeakStats::UpdateChart(int days)
{
    if(allSets != nullptr)
        chart->removeAllSeries();

    if(daysAxis != nullptr)
        chart->removeAxis(daysAxis);

    allSets = new QBarSeries(this);

    completedTasksSet = new QBarSet("Task Count", this);
    taskTimeSet = new QBarSet("Time Spend (in hours)", this);

    connect(completedTasksSet, &QBarSet::hovered,this, &PomopeakStats::OnHoverBar);
    connect(taskTimeSet, &QBarSet::hovered,this, &PomopeakStats::OnHoverBar);

    QMap<QDate,DayTaskStats> userStats = stats->GetUserStats();
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

    daysAxis = new QBarCategoryAxis(this);
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

void PomopeakStats::ForceUpdate()
{
    UpdateChart(currentVisibility);
    UpdateGlobalStats();
}

void PomopeakStats::UpdateGlobalStats()
{
    QMap<QDate,DayTaskStats> userStats = stats->GetUserStats();
    int completedCount = 0;
    float timeSpend = 0;
    for(auto it = userStats.constBegin(); it != userStats.constEnd(); ++it)
    {
        const auto& val = it.value();
        completedCount += val.TaskCompletedCount;
        timeSpend += val.TimeSpendInHours;
    }

    ui->totalTasksValueLabel->setText(QString::number(completedCount));
    ui->totalMinutesValueLabel->setText(QString::number(timeSpend * 60));
}
