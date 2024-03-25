#ifndef POMOPEAKSTATS_H
#define POMOPEAKSTATS_H

#include <QWidget>

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
};

#endif // POMOPEAKSTATS_H
