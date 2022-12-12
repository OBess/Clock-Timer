#include "clocktimerapp.h"
#include "./ui_clocktimerapp.h"

ClockTimerApp::ClockTimerApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClockTimerApp)
{
    ui->setupUi(this);
}

ClockTimerApp::~ClockTimerApp()
{
    delete ui;
}

