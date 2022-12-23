#include "clocktimerapp.h"
#include "./ui_clocktimerapp.h"

#include <QDebug>   // DELETE
#include <QDate>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextStream>
#include <QVBoxLayout>

#include "historymodel.h"
#include "intrangevalidator.h"
#include "timedrawwidget.h"

ClockTimerApp::ClockTimerApp(QWidget *parent)
    : QWidget(parent), ui(new Ui::ClockTimerApp)
{
    _clock = new QTimer(this);

    ui->setupUi(this);
    setupUi();
    setupConnections();

    using namespace std::chrono_literals;
    _clock->setInterval(100ms);
    _clock->start();
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

    ui->historyTable->setMinimumWidth(340);
    ui->historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->historyTable->horizontalHeader()->setStretchLastSection(true);

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

    // Create digital clock
    auto *horLayoutDigitalTimer = new QHBoxLayout;

    horLayoutDigitalTimer->addWidget(ui->le_hour);
    horLayoutDigitalTimer->addWidget(ui->sep1);
    horLayoutDigitalTimer->addWidget(ui->le_minute);
    horLayoutDigitalTimer->addWidget(ui->sep2);
    horLayoutDigitalTimer->addWidget(ui->le_second);

    ui->le_hour->setValidator(new IntRangeValidator(0, 24, this));
    ui->le_minute->setValidator(new IntRangeValidator(0, 60, this));
    ui->le_second->setValidator(new IntRangeValidator(0, 60, this));

    // Create vertical layout for a clock, a timer and control buttons
    auto *verLayoutClockTimer = new QVBoxLayout;

    _clockWidget = new Ui::ClockWidget(this);

    verLayoutClockTimer->addLayout(horLayoutDigitalTimer);
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
    QObject::connect(_clearBtn, &QPushButton::clicked, this, &ClockTimerApp::clearAll);
    QObject::connect(_startBtn, &QPushButton::clicked, this, &ClockTimerApp::insertInterval);
    QObject::connect(_clock, &QTimer::timeout, this, &ClockTimerApp::updateEverySecond);
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

    _historyModel->setData(_historyModel->index(_historyModel->rowCount() - 1, 0),
                           QDateTime::currentDateTime().toString());
    _historyModel->setData(_historyModel->index(_historyModel->rowCount() - 1, 1),
                           "00:00:00");
}

void ClockTimerApp::updateEverySecond()
{
    _clockWidget->updateEverySecond();

    if (_clockWidget->focused())
    {
        const QTime selectedTime = _clockWidget->getSelectedTime();

        ui->le_hour->setText(QString::number(selectedTime.hour()));
        ui->le_minute->setText(QString::number(selectedTime.minute()));
        ui->le_second->setText(QString::number(selectedTime.second()));
    }
    else
    {
        const QTime currentTime = QTime::currentTime();

        ui->le_hour->setText(QString::number(currentTime.hour()));
        ui->le_minute->setText(QString::number(currentTime.minute()));
        ui->le_second->setText(QString::number(currentTime.second()));
    }
}

