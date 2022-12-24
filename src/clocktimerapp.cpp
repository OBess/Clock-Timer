#include "clocktimerapp.h"
#include "./ui_clocktimerapp.h"

#include <QSettings>
#include <QTextStream>

#include "clockwidget.h"
#include "historymodel.h"
#include "settings.h"
#include "intrangevalidator.h"
#include "utilities.h"

ClockTimerApp::ClockTimerApp(QWidget *parent)
    : QWidget(parent), ui(new Ui::ClockTimerApp)
{
    ui->setupUi(this);

    _clockHandler = new QTimer(this);
    _timer = new QTimer(this);

    _clockHandler->setInterval(50);
    _clockHandler->start();

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
    QHBoxLayout *widgetClockLayout = new QHBoxLayout(ui->widget_clock);

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

    QObject::connect(_clockHandler, &QTimer::timeout, this, &ClockTimerApp::updateClocks);
    QObject::connect(_timer, &QTimer::timeout, this, &ClockTimerApp::stopTimer);
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
    QSettings settings(Settings::INI::FILE_PATH, QSettings::IniFormat);

    settings.beginGroup(Settings::INI::MAIN_WINDOW);

    if (settings.value(Settings::INI::IS_MAXIMIZED, false).toBool())
    {
        showMaximized();
    }
    else
    {
        resize(settings.value(Settings::INI::WIDTH, 640).toInt(),
               settings.value(Settings::INI::HEIGHT, 480).toInt());
    }

    settings.endGroup();

    // Read items to add them to table history
    settings.beginGroup(Settings::INI::HISTORY);

    const int size = settings.beginReadArray(Settings::INI::ITEM);

    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);

        _historyModel->insertRow(_historyModel->rowCount());

        const int row = _historyModel->rowCount() - 1;

        _historyModel->setData(_historyModel->index(row, 0),
                               settings.value(Settings::INI::DATETIME, Settings::INI::INVALID).toString());

        _historyModel->setData(_historyModel->index(row, 1),
                               settings.value(Settings::INI::INTERVAL, Settings::INI::INVALID).toString());
    }

    settings.endArray();

    settings.endGroup();
}

void ClockTimerApp::saveApp()
{
    QSettings settings(Settings::INI::FILE_PATH, QSettings::IniFormat);

    settings.beginGroup(Settings::INI::MAIN_WINDOW);

    settings.setValue(Settings::INI::IS_MAXIMIZED, isMaximized());
    settings.setValue(Settings::INI::WIDTH, width());
    settings.setValue(Settings::INI::HEIGHT, height());

    settings.endGroup();

    // Save history only if there is at least one item,
    // otherwise just remove group if there it is
    if (_historyModel->rowCount() == 0)
    {
        settings.remove(Settings::INI::HISTORY);
    }
    else
    {
        settings.beginGroup(Settings::INI::HISTORY);

        settings.beginWriteArray(Settings::INI::ITEM);

        for (int i = 0; i < _historyModel->rowCount(); ++i)
        {
            settings.setArrayIndex(i);

            settings.setValue(Settings::INI::DATETIME,
                              _historyModel->data(_historyModel->index(i, 0)).toString());

            settings.setValue(Settings::INI::INTERVAL,
                              _historyModel->data(_historyModel->index(i, 1)).toString());
        }

        settings.endArray();

        settings.endGroup();
    }
}

void ClockTimerApp::updateClocks()
{
    _clockWidget->updateEverySecond();

    if (_timerIsExecuting)
    {
        _clockWidget->setSelectedTime(Utils::MillsToTime(_timer->remainingTime()));

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

    _selectedMilliseconds = Utils::TimeToMills(_clockWidget->getSelectedTime());

    _timer->start(_selectedMilliseconds);

    _timerIsExecuting = true;
}

void ClockTimerApp::stopTimer()
{
    if (_timerIsExecuting == false)
    {
        return;
    }

    insertIntervalToTable(Utils::MillsToTime(_selectedMilliseconds - _timer->remainingTime()));

    _timer->stop();

    ui->btn_start->show();
    ui->btn_stop->hide();

    _clockWidget->clear();

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
