#ifndef POMOPEAKSETTINGS_H
#define POMOPEAKSETTINGS_H

#include <QWidget>
#include "settings.h"
namespace Ui {
class pomopeaksettings;
}

class pomopeaksettings : public QWidget
{
    Q_OBJECT

public:
    explicit pomopeaksettings(Settings& _settings, QWidget *parent = nullptr);
    ~pomopeaksettings();
    void setVisible(bool visible) override
    {
        QWidget::setVisible(visible);
        IsOpened  = visible;
    }
    bool IsOpened = false;
signals:
    void OnClose();
private slots:
    void OnStartSliderMoved(int value);
    void OnEndBreakSliderMoved(int value);
    void OnExitClicked();
private:
    Ui::pomopeaksettings *ui;
    Settings& settings;

};

#endif // POMOPEAKSETTINGS_H
