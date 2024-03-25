#ifndef POMOPEAKSTATS_H
#define POMOPEAKSTATS_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
namespace Ui {
class PomopeakStats;
}

class PomopeakStats : public QWidget
{
    Q_OBJECT

public:
    explicit PomopeakStats(QWidget *parent = nullptr);
    ~PomopeakStats();

signals:
    void OnClose();
private slots:
    void OnExitClicked();
private:
    Ui::PomopeakStats *ui;
    QChart* chart;
    void OnViewButtonsClick();
    void SwitchViewToMonthly();
    void SwitchViewToWeekly();
    void InitializeChart();
};

#endif // POMOPEAKSTATS_H
