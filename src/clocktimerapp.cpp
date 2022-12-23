#include "clocktimerapp.h"
#include "./ui_clocktimerapp.h"

#include <QDebug>   // DELETE
#include <QDate>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QTextStream>
#include <QVBoxLayout>

#include "historymodel.h"
#include "intrangevalidator.h"
#include "timedrawwidget.h"

ClockTimerApp::ClockTimerApp(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ClockTimerApp),
      _iniFile("settings.ini")
{
    _clock = new QTimer(this);
    _timer = new QTimer(this);

    ui->setupUi(this);
    setupUi();
    setupConnections();

    using namespace std::chrono_literals;
    _clock->setInterval(100ms);
    _clock->start();

    setupApp();
}

ClockTimerApp::~ClockTimerApp()
{
    saveApp();

    delete ui;
}

inline void ClockTimerApp::setupUi()
{
    //
    _startBtn = buildBtn("Start", this);
    _stopBtn = buildBtn("Stop", this);
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
    verLayoutClockTimer->addWidget(_stopBtn);
    verLayoutClockTimer->addWidget(_restartBtn);

    verLayoutClockTimer->setAlignment(_startBtn, Qt::AlignCenter);
    verLayoutClockTimer->setAlignment(_stopBtn, Qt::AlignCenter);
    verLayoutClockTimer->setAlignment(_restartBtn, Qt::AlignCenter);
    verLayoutClockTimer->setContentsMargins(0, 0, 0, 0);

    _stopBtn->hide();

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
    QObject::connect(_startBtn, &QPushButton::clicked, this, &ClockTimerApp::startTimer);
    QObject::connect(_stopBtn, &QPushButton::clicked, this, &ClockTimerApp::stopTimer);
    QObject::connect(_restartBtn, &QPushButton::clicked, this, &ClockTimerApp::restartTimer);
    QObject::connect(_clock, &QTimer::timeout, this, &ClockTimerApp::updateEverySecond);
    QObject::connect(_timer, &QTimer::timeout, this, &ClockTimerApp::stopTimer);
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

void ClockTimerApp::insertInterval(QTime time)
{
    _historyModel->insertRow(_historyModel->rowCount());

    _historyModel->setData(_historyModel->index(_historyModel->rowCount() - 1, 0),
                           QDateTime::currentDateTime().toString());
    _historyModel->setData(_historyModel->index(_historyModel->rowCount() - 1, 1),
                           time.toString());
}

void ClockTimerApp::updateDigitTime(QTime time)
{
    ui->le_hour->setText(QString::number(time.hour()));
    ui->le_minute->setText(QString::number(time.minute()));
    ui->le_second->setText(QString::number(time.second()));
}

void ClockTimerApp::setupApp()
{
    QSettings settings(_iniFile, QSettings::IniFormat);

    //
    settings.beginGroup("MainWindow");

    if (settings.value("IsMaximized", false).toBool())
    {
        showMaximized();
    }
    else
    {
        resize(settings.value("Width", 640).toInt(), settings.value("Height", 480).toInt());
    }

    settings.endGroup();

    //
    settings.beginGroup("History");

    const int size = settings.beginReadArray("Item");
    for (int i = 0; i < size; ++i)
    {
        _historyModel->insertRow(_historyModel->rowCount());

        settings.setArrayIndex(i);

        _historyModel->setData(_historyModel->index(_historyModel->rowCount() - 1, 0),
                               settings.value("datetime", "Invalid").toString());
        _historyModel->setData(_historyModel->index(_historyModel->rowCount() - 1, 1),
                               settings.value("interval", "Invalid").toString());
    }
    settings.endArray();

    settings.endGroup();
}

void ClockTimerApp::saveApp()
{
    QSettings settings(_iniFile, QSettings::IniFormat);

    //
    settings.beginGroup("MainWindow");

    settings.setValue("IsMaximized", isMaximized());
    settings.setValue("Width", width());
    settings.setValue("Height", height());

    settings.endGroup();

    //
    if (_historyModel->rowCount() == 0)
    {
        settings.remove("History");
    }
    else
    {
        settings.beginGroup("History");

        settings.beginWriteArray("Item");

        for (int i = 0; i < _historyModel->rowCount(); ++i)
        {
            settings.setArrayIndex(i);

            settings.setValue("datetime", _historyModel->data(_historyModel->index(i, 0)).toString());
            settings.setValue("interval", _historyModel->data(_historyModel->index(i, 1)).toString());
        }

        settings.endArray();

        settings.endGroup();
    }
}

int ClockTimerApp::timeToMills(QTime time) noexcept
{
    return (time.hour() * 3600 + time.minute() * 60 + time.second()) * 1000;
}

QTime ClockTimerApp::millsToTime(int mills) noexcept
{
    mills = mills / 1000;
    return QTime(mills / 3600, mills / 60, mills % 60);
}

void ClockTimerApp::updateEverySecond()
{
    _clockWidget->updateEverySecond();

    if (_timerIsExecuting)
    {
        _clockWidget->setSelectedTime(millsToTime(_timer->remainingTime()));

        updateDigitTime(_clockWidget->getSelectedTime());
    }
    else if (_clockWidget->focused())
    {
        updateDigitTime(_clockWidget->getSelectedTime());
    }
    else
    {
        updateDigitTime(QTime::currentTime());
    }
}

void ClockTimerApp::startTimer()
{
    if (_clockWidget->focused() == false)
    {
        return;
    }

    _startBtn->hide();
    _stopBtn->show();

    _selectedMilliseconds = timeToMills(_clockWidget->getSelectedTime());

    _timer->start(_selectedMilliseconds);

    _timerIsExecuting = true;
}

void ClockTimerApp::stopTimer()
{
    if (_timerIsExecuting == false)
    {
        return;
    }

    insertInterval(millsToTime(_timer->remainingTime()));

    _timer->stop();

    _startBtn->show();
    _stopBtn->hide();

    _clockWidget->clear();

    _timerIsExecuting = false;
}

void ClockTimerApp::restartTimer()
{
    _timer->start(_selectedMilliseconds);

    _startBtn->hide();
    _stopBtn->show();

    _timerIsExecuting = true;
}

