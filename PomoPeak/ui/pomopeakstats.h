#ifndef POMOPEAKSTATS_H
#define POMOPEAKSTATS_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include "userstats.h"
#include "QObjectInitialization.h"

#include <QTimer>
namespace Ui {
class PomopeakStats;
}

class PomopeakStats : public QWidget, public QObjectInitialization
{
    Q_OBJECT

    enum ChartVisibility
    {
        Weekly,
        Monthly
    };

public:
    explicit PomopeakStats(std::shared_ptr<UserStats> stats, QWidget *parent = nullptr);
    ~PomopeakStats();
    void ForceUpdate();

protected:
    void InitializeDataContainer() override;
    void InitializeObjects() override;
    void SubscribeToEvents() override;
    void UnsubscribeFromEvents() override;

signals:
    void OnClose();

private slots:
    void OnExitClicked();

private:

    const QMap<ChartVisibility, int> VisibilitySettings =
    {
        { PomopeakStats::Weekly, 7 },
        { PomopeakStats::Monthly, 30}
    };

    ChartVisibility currentVisibility;


    Ui::PomopeakStats *ui;
    std::shared_ptr<UserStats> stats;
    QChart* chart;
    QBarSeries* allSets;
    QBarSet* completedTasksSet;
    QBarSet* taskTimeSet;
    QBarCategoryAxis* daysAxis;
    QTimer* showTooltipTimer;
    QString tooltipText;
    void OnViewButtonsClick();
    void SwtichChartView(ChartVisibility visibility);
    void InitializeChart();
    void OnHoverBar(bool status, int index);
    void ShowBarText();
    void UpdateChart(int days);
    void UpdateGlobalStats();
};

#endif // POMOPEAKSTATS_H
