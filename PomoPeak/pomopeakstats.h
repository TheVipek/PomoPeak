#ifndef POMOPEAKSTATS_H
#define POMOPEAKSTATS_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include "userstats.h"
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
    void OnViewButtonsClick();
    void SwitchViewToMonthly();
    void SwitchViewToWeekly();
    void InitializeChart();
    void ShowBarText(bool status, int index);
    void UpdateChart(int days);
};

#endif // POMOPEAKSTATS_H
