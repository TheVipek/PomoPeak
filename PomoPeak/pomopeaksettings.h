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

private:
    Ui::pomopeaksettings *ui;
    Settings& settings;
};

#endif // POMOPEAKSETTINGS_H
