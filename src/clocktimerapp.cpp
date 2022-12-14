#include "clocktimerapp.h"
#include "./ui_clocktimerapp.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

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
    // Create horizontal layout for the menu bar
    auto *horLayoutMenuBar = new QHBoxLayout;

    horLayoutMenuBar->setSpacing(0);
    horLayoutMenuBar->setMargin(0);

    horLayoutMenuBar->addWidget(ui->icon);
    horLayoutMenuBar->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    horLayoutMenuBar->addWidget(ui->title);
    horLayoutMenuBar->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    horLayoutMenuBar->addWidget(ui->hideBtn);
    horLayoutMenuBar->addWidget(ui->maximazeBtn);
    horLayoutMenuBar->addWidget(ui->normalizeBtn);
    horLayoutMenuBar->addWidget(ui->quitBtn);

    ui->icon->setContentsMargins(20, 0, 0, 0);
    ui->hideBtn->setContentsMargins(0, 0, 0, 0);
    ui->maximazeBtn->setContentsMargins(0, 0, 0, 0);
    ui->normalizeBtn->setContentsMargins(0, 0, 0, 0);
    ui->quitBtn->setContentsMargins(0, 0, 0, 0);

    ui->maximazeBtn->setFocusPolicy(Qt::NoFocus);
    ui->normalizeBtn->setFocusPolicy(Qt::NoFocus);

    if (isMaximized())
    {
        ui->maximazeBtn->hide();
    }
    else
    {
        ui->normalizeBtn->hide();
    }

    // Create sub-horizontal layout for history's clear button
    auto *horLayoutHistoryClearBtn = new QHBoxLayout;

    horLayoutHistoryClearBtn->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    horLayoutHistoryClearBtn->addWidget(ui->clearBtn);
    horLayoutHistoryClearBtn->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Create vertical layout for history component
    auto *verLayoutHistory = new QVBoxLayout;

    verLayoutHistory->addWidget(ui->historyLbl);
    verLayoutHistory->addWidget(ui->historyTable);
    verLayoutHistory->addLayout(horLayoutHistoryClearBtn);

    // Create vertical layout for a clock, a timer and control buttons
    auto *verLayoutClockTimer = new QVBoxLayout;

    verLayoutClockTimer->addWidget(ui->timer);
    verLayoutClockTimer->addWidget(ui->startBtn);
    verLayoutClockTimer->addWidget(ui->restartBtn);

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

    verLayoutMain->addLayout(horLayoutMenuBar);
    verLayoutMain->addLayout(horLayoutForComp);

    setLayout(verLayoutMain);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

inline void ClockTimerApp::setupConnections()
{
    QObject::connect(ui->quitBtn, &QPushButton::released, [this]
                     { close(); });

    QObject::connect(ui->maximazeBtn, &QPushButton::released, [this]
                     { 
                        showMaximized(); 
                        ui->maximazeBtn->hide();
                        ui->normalizeBtn->show(); });

    QObject::connect(ui->hideBtn, &QPushButton::released, [this]
                     { showMinimized(); });

    QObject::connect(ui->normalizeBtn, &QPushButton::released, [this]
                     { 
                        showNormal();
                        ui->maximazeBtn->show();
                        ui->normalizeBtn->hide(); });
}