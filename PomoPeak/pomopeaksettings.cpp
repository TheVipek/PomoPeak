#include "pomopeaksettings.h"
#include "ui_pomopeaksettings.h"

pomopeaksettings::pomopeaksettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pomopeaksettings)
{
    ui->setupUi(this);
}

pomopeaksettings::~pomopeaksettings()
{
    delete ui;
}
