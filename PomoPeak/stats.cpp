#include "stats.h"
#include "ui_stats.h"
Stats::Stats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Stats)
{
    ui->setupUi(this);

    connect(ui->exitButton, &QPushButton::clicked, this, &Stats::OnExitClicked);
}

Stats::~Stats()
{
    delete ui;
}

void Stats::OnExitClicked()
{
    emit OnClose();
}
