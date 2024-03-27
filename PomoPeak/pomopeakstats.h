#ifndef POMOPEAKSTATS_H
#define POMOPEAKSTATS_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include "userstats.h"


#include <QTimer>
namespace Ui {
class PomopeakStats;
}

class PomopeakStats : public QWidget
{
    Q_OBJECT

public:
    explicit PomopeakStats( UserStats& stats, QWidget *parent = nullptr);
    ~PomopeakStats();

signals:
    void OnClose();
private slots:
    void OnExitClicked();
private:
    Ui::PomopeakStats *ui;

    UserStats& stats;
    QChart* chart;
    QBarSeries* allSets;
    QBarSet* completedTasksSet;
    QBarSet* taskTimeSet;
    QBarCategoryAxis* daysAxis;
    QTimer* showTooltipTimer;
    QString tooltipText;
    void OnViewButtonsClick();
    void SwitchViewToMonthly();
    void SwitchViewToWeekly();
    void InitializeChart();
    void OnHoverBar(bool status, int index);
    void ShowBarText();
    void UpdateChart(int days);
};

#endif // POMOPEAKSTATS_H
