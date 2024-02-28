#ifndef POMOPEAKSETTINGS_H
#define POMOPEAKSETTINGS_H

#include <QWidget>

namespace Ui {
class pomopeaksettings;
}

class pomopeaksettings : public QWidget
{
    Q_OBJECT

public:
    explicit pomopeaksettings(QWidget *parent = nullptr);
    ~pomopeaksettings();

private:
    Ui::pomopeaksettings *ui;
};

#endif // POMOPEAKSETTINGS_H
