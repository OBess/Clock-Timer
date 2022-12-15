#include "clocktimerapp.h"
#include "./ui_clocktimerapp.h"

// TODO:: Delete
#include <QDebug>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "timedrawwidget.h"

ClockTimerApp::ClockTimerApp(QWidget *parent)
    : QWidget(parent), ui(new Ui::ClockTimerApp)
{
    ui->setupUi(this);
    setupUi();
    setupConnections();
}

ClockTimerApp::~ClockTimerApp()
{
    delete ui;
}

inline void ClockTimerApp::setupUi()
{
    // Create sub-horizontal layout for history's clear button
    auto *horLayoutHistoryClearBtn = new QHBoxLayout;

    ui->clearBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    horLayoutHistoryClearBtn->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    horLayoutHistoryClearBtn->addWidget(ui->clearBtn);
    horLayoutHistoryClearBtn->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    horLayoutHistoryClearBtn->setAlignment(ui->clearBtn, Qt::AlignCenter);

    // Create vertical layout for history component
    auto *verLayoutHistory = new QVBoxLayout;

    verLayoutHistory->addWidget(ui->historyLbl);
    verLayoutHistory->addWidget(ui->historyTable);
    verLayoutHistory->addLayout(horLayoutHistoryClearBtn);

    // Create vertical layout for a clock, a timer and control buttons
    auto *verLayoutClockTimer = new QVBoxLayout;

    _clockWidget = new Ui::ClockWidget(this);

    ui->startBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->restartBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    verLayoutClockTimer->addWidget(ui->timer);
    verLayoutClockTimer->addWidget(_clockWidget);
    verLayoutClockTimer->addWidget(ui->startBtn);
    verLayoutClockTimer->addWidget(ui->restartBtn);

    verLayoutClockTimer->setAlignment(ui->startBtn, Qt::AlignCenter);
    verLayoutClockTimer->setAlignment(ui->restartBtn, Qt::AlignCenter);
    verLayoutClockTimer->setContentsMargins(0, 0, 0, 0);

    // Create horizontal layout for history and clock/timer components
    auto *horLayoutForComp = new QHBoxLayout;

    horLayoutForComp->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    horLayoutForComp->addLayout(verLayoutHistory);
    horLayoutForComp->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    horLayoutForComp->addLayout(verLayoutClockTimer);
    horLayoutForComp->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    horLayoutForComp->setStretch(0, 1);
    horLayoutForComp->setStretch(1, 3);
    horLayoutForComp->setStretch(2, 1);
    horLayoutForComp->setStretch(3, 6);
    horLayoutForComp->setStretch(4, 5);

    horLayoutForComp->setContentsMargins(0, 30, 0, 30);

    // Create main vertical layout for widget
    auto *verLayoutMain = new QVBoxLayout;

    verLayoutMain->setContentsMargins(0, 0, 0, 0);

    verLayoutMain->addLayout(horLayoutForComp);

    setLayout(verLayoutMain);
}

inline void ClockTimerApp::setupConnections()
{
    
}