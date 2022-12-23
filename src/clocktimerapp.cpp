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
    ui->setupUi(this);

    _clock = new QTimer(this);
    _timer = new QTimer(this);

    _clock->setInterval(100);
    _clock->start();

    setupUi();
    setupConnections();
    setupApp();
}

ClockTimerApp::~ClockTimerApp()
{
    saveApp();

    delete ui;
}

inline void ClockTimerApp::setupUi()
{
    // Hide "Stop" button, we will see this when clicked "Start" button
    ui->btn_stop->hide();

    // Add some stretching to cells in History table
    ui->table_history->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table_history->horizontalHeader()->setStretchLastSection(true);

    // Create and set History model to History table
    _historyModel = new HistoryModel(this);
    ui->table_history->setModel(_historyModel);

    // Setup validator to line editos that represent digital clock
    ui->le_hour->setValidator(new IntRangeValidator(0, 24, this));
    ui->le_minute->setValidator(new IntRangeValidator(0, 60, this));
    ui->le_second->setValidator(new IntRangeValidator(0, 60, this));

    // Set layout to dummy widget
    QHBoxLayout* widgetClockLayout = new QHBoxLayout(ui->widget_clock);

    // Create ClockWidget instance and add to the previously created layout
    _clockWidget = new Ui::ClockWidget(ui->widget_clock);
    widgetClockLayout->addWidget(_clockWidget);

    // Load CSS2 stylesheet from internal file and setup style to the current object
    setupStyle();
}

inline void ClockTimerApp::setupConnections()
{
    QObject::connect(ui->btn_start, &QPushButton::clicked, this, &ClockTimerApp::startTimer);
    QObject::connect(ui->btn_stop, &QPushButton::clicked, this, &ClockTimerApp::stopTimer);
    QObject::connect(ui->btn_restart, &QPushButton::clicked, this, &ClockTimerApp::restartTimer);
    QObject::connect(ui->btn_clear, &QPushButton::clicked, this, &ClockTimerApp::clearHistoryTable);

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

void ClockTimerApp::clearHistoryTable()
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

    ui->btn_start->hide();
    ui->btn_stop->show();

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

    ui->btn_start->show();
    ui->btn_stop->hide();

    _clockWidget->clear();

    _timerIsExecuting = false;
}

void ClockTimerApp::restartTimer()
{
    if (_clockWidget->focused() == false)
    {
        return;
    }

    _timer->start(_selectedMilliseconds);

    ui->btn_start->hide();
    ui->btn_stop->show();

    _timerIsExecuting = true;
}

