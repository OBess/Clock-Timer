#ifndef CLOCKTIMERAPP_H
#define CLOCKTIMERAPP_H

#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ClockTimerApp;
    class ClockWidget;
}

class HistoryModel;
class QPushButton;
QT_END_NAMESPACE

class ClockTimerApp : public QWidget
{
    Q_OBJECT

public:
    ClockTimerApp(QWidget *parent = nullptr);
    ~ClockTimerApp();

private slots:
    void clearAll();
    void updateEverySecond();
    void startTimer();
    void stopTimer();
    void restartTimer();

private:
    void setupUi();
    void setupConnections();
    void setupStyle();
    void insertInterval(QTime time);

    void updateDigitTime(QTime time);

    void setupApp();
    void saveApp();

    static int timeToMills(QTime time) noexcept;
    static QTime millsToTime(int mills) noexcept;

    static QPushButton *buildBtn(const char *text, QWidget *parent);

    Ui::ClockTimerApp *ui = nullptr;

    Ui::ClockWidget *_clockWidget = nullptr;
    QPushButton *_startBtn = nullptr;
    QPushButton *_stopBtn = nullptr;
    QPushButton *_restartBtn = nullptr;
    QPushButton *_clearBtn = nullptr;

    HistoryModel *_historyModel = nullptr;

    QTimer* _clock = nullptr;
    QTimer* _timer = nullptr;

    int _selectedMilliseconds = 0;
    bool _timerIsExecuting = false;

    const QString _iniFile;
};
#endif // CLOCKTIMERAPP_H
