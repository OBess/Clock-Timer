#ifndef CLOCKTIMERAPP_H
#define CLOCKTIMERAPP_H

#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ClockTimerApp;
    class AnalogClock;
}

class HistoryModel;
QT_END_NAMESPACE

class ClockTimerApp : public QWidget
{
    Q_OBJECT

public:
    ClockTimerApp(QWidget *parent = nullptr);
    ~ClockTimerApp();

private slots:
    void clearHistoryTable();
    void updateClocks();
    void startTimer();
    void stopTimer(bool withTimeout);
    void restartTimer();

private:
    void setupUi();
    void setupConnections();
    void setupStyle();

    void insertIntervalToTable(QTime time);

    void updateDigitTime(QTime time);

    void setupApp();
    void saveApp();

    Ui::ClockTimerApp *ui = nullptr;

    Ui::AnalogClock *_analogClock = nullptr;

    HistoryModel *_historyModel = nullptr;

    QTimer *_clockHandler = nullptr;
    QTimer *_timer = nullptr;

    int _selectedMilliseconds = 0;

    bool _timerIsExecuting = false;
};
#endif // CLOCKTIMERAPP_H
