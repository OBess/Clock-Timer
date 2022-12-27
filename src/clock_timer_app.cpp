#include "clock_timer_app.h"
#include "./ui_clocktimerapp.h"

#include <QDebug>
#include <QScreen>
#include <QSettings>
#include <QTextStream>

#include "analog_clock.h"
#include "history_model.h"
#include "settings.h"
#include "int_range_validator.h"
#include "utils.h"

ClockTimerApp::ClockTimerApp(QWidget *parent)
    : QWidget(parent), ui(new Ui::ClockTimerApp)
{
    ui->setupUi(this);

    _clockHandler = new QTimer(this);
    _timer = new QTimer(this);

    _clockHandler->setInterval(50);
    _clockHandler->start();

    setupUi();
    setupApp();
    setupConnections();
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
    QHBoxLayout *widgetClockLayout = new QHBoxLayout(ui->widget_clock);

    // Create AnalogClock instance and add to the previously created layout
    _analogClock = new Ui::AnalogClock(ui->widget_clock);
    widgetClockLayout->addWidget(_analogClock);

    // Load CSS2 stylesheet from internal file and setup style to the current object
    setupStyle();
}

inline void ClockTimerApp::setupConnections()
{
    QObject::connect(ui->btn_start, &QPushButton::clicked, this, &ClockTimerApp::startTimer);
    QObject::connect(ui->btn_stop, &QPushButton::clicked, this, [this]{ stopTimer(false); });
    QObject::connect(ui->btn_restart, &QPushButton::clicked, this, &ClockTimerApp::restartTimer);
    QObject::connect(ui->btn_clear, &QPushButton::clicked, this, &ClockTimerApp::clearHistoryTable);

    QObject::connect(_clockHandler, &QTimer::timeout, this, &ClockTimerApp::updateClocks);
    QObject::connect(_timer, &QTimer::timeout, this, [this]{ stopTimer(true); });
}

inline void ClockTimerApp::setupStyle()
{
    if (QFile file(Settings::Style::MAIN_STYLE); file.open(QIODevice::ReadOnly | QIODevice::Text))
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

void ClockTimerApp::insertIntervalToTable(QTime time)
{
    if(_historyModel->insertRow(_historyModel->rowCount()))
    {
        const int row = _historyModel->rowCount() - 1;

        _historyModel->setData(_historyModel->index(row, 0), QDateTime::currentDateTime().toString());
        _historyModel->setData(_historyModel->index(row, 1), time.toString());
    }
}

void ClockTimerApp::updateDigitTime(QTime time)
{
    ui->le_hour->setText(QString::number(time.hour()));
    ui->le_minute->setText(QString::number(time.minute()));
    ui->le_second->setText(QString::number(time.second()));
}

void ClockTimerApp::setupApp()
{
    QSettings settings(Settings::Ini::FILE_PATH, QSettings::IniFormat);

    settings.beginGroup(Settings::Ini::MAIN_WINDOW);

    if (settings.value(Settings::Ini::IS_MAXIMIZED, false).toBool())
    {
        showMaximized();
    }
    else
    {
        resize(settings.value(Settings::Ini::WIDTH, 640).toInt(),
               settings.value(Settings::Ini::HEIGHT, 480).toInt());

        // Keep the application in the screen area
        const QSize screenSize = screen()->size();

        int pos_x = settings.value(Settings::Ini::POS_X, 0).toInt();
        int pos_y = settings.value(Settings::Ini::POS_Y, 0).toInt();

        if (pos_x + width() > screenSize.width())
        {
            pos_x = screenSize.width() - width();
        }
        else if (pos_x < 0)
        {
            pos_x = 0;
        }

        if (pos_y + height() > screenSize.height())
        {
            pos_y = screenSize.height() - height();
        }
        else if (pos_y < 0)
        {
            pos_y = 0;
        }

        move(pos_x, pos_y);
    }

    settings.endGroup();

    // Read items to add them to table history
    settings.beginGroup(Settings::Ini::HISTORY);

    const int size = settings.beginReadArray(Settings::Ini::ITEM);

    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);

        _historyModel->insertRow(_historyModel->rowCount());

        const int row = _historyModel->rowCount() - 1;

        _historyModel->setData(_historyModel->index(row, 0),
                               settings.value(Settings::Ini::DATETIME, Settings::Ini::INVALID).toString());

        _historyModel->setData(_historyModel->index(row, 1),
                               settings.value(Settings::Ini::INTERVAL, Settings::Ini::INVALID).toString());
    }

    settings.endArray();

    settings.endGroup();
}

void ClockTimerApp::saveApp()
{
    QSettings settings(Settings::Ini::FILE_PATH, QSettings::IniFormat);

    settings.beginGroup(Settings::Ini::MAIN_WINDOW);

    settings.setValue(Settings::Ini::IS_MAXIMIZED, isMaximized());
    settings.setValue(Settings::Ini::WIDTH, width());
    settings.setValue(Settings::Ini::HEIGHT, height());
    settings.setValue(Settings::Ini::POS_X, pos().x());
    settings.setValue(Settings::Ini::POS_Y, pos().y() - 38); // 38 it is the height of title bar

    settings.endGroup();

    // Save history only if there is at least one item,
    // otherwise just remove group if there it is
    if (_historyModel->rowCount() == 0)
    {
        settings.remove(Settings::Ini::HISTORY);
    }
    else
    {
        settings.beginGroup(Settings::Ini::HISTORY);

        settings.beginWriteArray(Settings::Ini::ITEM);

        for (int i = 0; i < _historyModel->rowCount(); ++i)
        {
            settings.setArrayIndex(i);

            settings.setValue(Settings::Ini::DATETIME,
                              _historyModel->data(_historyModel->index(i, 0)).toString());

            settings.setValue(Settings::Ini::INTERVAL,
                              _historyModel->data(_historyModel->index(i, 1)).toString());
        }

        settings.endArray();

        settings.endGroup();
    }
}

void ClockTimerApp::updateClocks()
{
    _analogClock->repaint();

    if (_timerIsExecuting)
    {
        _analogClock->setSelectedTime(Utils::MillsToTime(_timer->remainingTime()));

        updateDigitTime(_analogClock->getSelectedTime());
    }
    else if (_analogClock->focused())
    {
        updateDigitTime(_analogClock->getSelectedTime());
    }
    else
    {
        updateDigitTime(QTime::currentTime());
    }
}

void ClockTimerApp::startTimer()
{
    if (_analogClock->focused() == false)
    {
        return;
    }

    ui->btn_start->hide();
    ui->btn_stop->show();

    _selectedMilliseconds = Utils::TimeToMills(_analogClock->getSelectedTime());

    _timer->start(_selectedMilliseconds);

    _timerIsExecuting = true;
}

void ClockTimerApp::stopTimer(bool withTimeout)
{
    if (_timerIsExecuting == false)
    {
        return;
    }

    if (withTimeout)
    {
        insertIntervalToTable(Utils::MillsToTime(_selectedMilliseconds));
    }
    else
    {
        insertIntervalToTable(Utils::MillsToTime(_selectedMilliseconds - _timer->remainingTime()));
    }

    _timer->stop();

    ui->btn_start->show();
    ui->btn_stop->hide();

    _analogClock->clearSelected();

    _timerIsExecuting = false;
}

void ClockTimerApp::restartTimer()
{
    if (_timerIsExecuting)
    {
        return;
    }

    _timer->start(_selectedMilliseconds);

    ui->btn_start->hide();
    ui->btn_stop->show();

    _timerIsExecuting = true;
}
