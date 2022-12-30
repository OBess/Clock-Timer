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

/// @brief Extends QWidget class to show other widgets on the screen. 
/// The main object of the application
class ClockTimerApp : public QWidget
{
    Q_OBJECT

public:
    explicit ClockTimerApp(QWidget *parent = nullptr);
    ~ClockTimerApp();

private slots:
    void clearHistoryTable();

    /// @brief Repaints the analog clock and updates the digital clock
    void updateClocks();

    /// @brief Starts timer corresponding to the saved selected time
    void startTimer();

    void stopTimer(bool withTimeout);

    /// @brief Restarts the timer corresponding to the saved selected time
    void restartTimer();

    /// @brief Updates the analog clock selected time corresponding to the digital clock
    void digitClockEdited(const QString &text);

    /// @brief Clears all focus on clocks and selected time
    void unselect();

private:
    void setupUi();
    void setupConnections();

    /// @brief Loads CSS2 styles from files and setups them for widgets
    void setupStyle();

    /// @brief Starts timer if selected milliseconds are greater than zero
    void startTimerIfSelected();

    /// @brief Gets time and sets it to like an interval into the table
    /// @param time QTime
    void insertIntervalToTable(QTime time);

    /// @brief Updates line editors corresponding to the clock or selected time
    /// @param time QTime
    void updateDigitTime(QTime time);

    void clearFocusOnDigitTimer();

    bool digitTimerHasFocus() noexcept;

    /// @brief Loads data from the .ini file and setups properties to the widget
    void setupApp();

    /// @brief Saves properties of the widget to the .ini file
    void saveApp();

    void timerEvent(QTimerEvent *event) override;

    /// @brief Generated class from .ui file
    Ui::ClockTimerApp *ui = nullptr;

    Ui::AnalogClock *_analogClock = nullptr;

    HistoryModel *_historyModel = nullptr;

    /// @brief Timer identification for clock updates
    int _clockHandler = 0;
    
    QTimer *_timer = nullptr;

    int _selectedMilliseconds = 0;

    int _intervalForClockHandler = 50;

    bool _timerIsExecuting = false;
};
#endif // CLOCKTIMERAPP_H
