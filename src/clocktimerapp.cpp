#include "clocktimerapp.h"
#include "./ui_clocktimerapp.h"

// TODO:: Delete
#include <QDebug>

#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "historymodel.h"
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
    //
    _startBtn = buildBtn("Start", this);
    _restartBtn = buildBtn("Restart", this);
    _clearBtn = buildBtn("Clear", this);

    //
    _historyModel = new HistoryModel(this);

    ui->historyTable->verticalHeader()->hide();
    ui->historyTable->horizontalHeader()->hide();

    ui->historyTable->setModel(_historyModel);

    // Create sub-horizontal layout for history's clear button
    auto *horLayoutHistoryClearBtn = new QHBoxLayout;

    horLayoutHistoryClearBtn->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    horLayoutHistoryClearBtn->addWidget(_clearBtn);
    horLayoutHistoryClearBtn->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    horLayoutHistoryClearBtn->setAlignment(_clearBtn, Qt::AlignCenter);

    // Create vertical layout for history component
    auto *verLayoutHistory = new QVBoxLayout;

    verLayoutHistory->addWidget(ui->historyLbl);
    verLayoutHistory->addWidget(ui->historyTable);
    verLayoutHistory->addLayout(horLayoutHistoryClearBtn);

    // Create vertical layout for a clock, a timer and control buttons
    auto *verLayoutClockTimer = new QVBoxLayout;

    _clockWidget = new Ui::ClockWidget(this);

    verLayoutClockTimer->addWidget(ui->timer);
    verLayoutClockTimer->addWidget(_clockWidget);
    verLayoutClockTimer->addWidget(_startBtn);
    verLayoutClockTimer->addWidget(_restartBtn);

    verLayoutClockTimer->setAlignment(_startBtn, Qt::AlignCenter);
    verLayoutClockTimer->setAlignment(_restartBtn, Qt::AlignCenter);
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

    setupStyle();
}

inline void ClockTimerApp::setupConnections()
{
    QObject::connect(_clearBtn, &QPushButton::released, this, &ClockTimerApp::clearAll);
    QObject::connect(_startBtn, &QPushButton::released, this, &ClockTimerApp::insertInterval);
}

inline void ClockTimerApp::setupStyle()
{
    if (QFile file("://styles/main.css"); file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (file.size() == 0)
            return;

        QTextStream in(&file);
        setStyleSheet(in.readAll());
    }
}

QPushButton *ClockTimerApp::buildBtn(const char *text, QWidget *parent)
{
    QPushButton *newBtn = new QPushButton(parent);

    newBtn->setFixedSize({251, 51});
    newBtn->setText(text);

    QFont font = newBtn->font();
    font.setPointSize(12);
    newBtn->setFont(font);

    newBtn->setCursor(QCursor(Qt::PointingHandCursor));
    newBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    return newBtn;
}

void ClockTimerApp::clearAll()
{
    _historyModel->removeRows(0, _historyModel->rowCount());
}

void ClockTimerApp::insertInterval()
{
    _historyModel->insertRow(_historyModel->rowCount());
}