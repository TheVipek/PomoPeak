#include "pomopeak.h"
#include "./ui_pomopeak.h"

PomoPeak::PomoPeak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PomoPeak)
{
    ui->setupUi(this);
}

PomoPeak::~PomoPeak()
{
    delete ui;
}
