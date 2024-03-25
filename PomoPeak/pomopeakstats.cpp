#include "pomopeakstats.h"
#include "ui_pomopeakstats.h"
PomopeakStats::PomopeakStats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PomopeakStats)
{
    ui->setupUi(this);

    connect(ui->exitButton, &QPushButton::clicked, this, &PomopeakStats::OnExitClicked);
}

PomopeakStats::~PomopeakStats()
{
    delete ui;
}

void PomopeakStats::OnExitClicked()
{
    emit OnClose();
}
