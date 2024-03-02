#include "pomopeaksettings.h"
#include "ui_pomopeaksettings.h"

pomopeaksettings::pomopeaksettings(Settings& _settings, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pomopeaksettings)
    , settings(_settings)
{
    ui->setupUi(this);
}

pomopeaksettings::~pomopeaksettings()
{
    delete ui;
}
