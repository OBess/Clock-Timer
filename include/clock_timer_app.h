#ifndef CLOCKTIMERAPP_H
#define CLOCKTIMERAPP_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ClockTimerApp;
    class AnalogClock;
}

class HistoryModel;
QT_END_NAMESPACE

class QKeyEvent;
class QSettings;
class QTimer;

/// @brief Extends QWidget class to show other widgets on the screen.
/// The main object of the application
class ClockTimerApp : public QWidget
{
    Q_OBJECT

public:
    explicit ClockTimerApp(QWidget *parent = nullptr);
    ~ClockTimerApp();

public slots:
    void onAppStateChange(Qt::ApplicationState state);

private slots:
    /// @brief Updates the analog clock selected time corresponding to the digital clock
    void digitClockEdited(const QString &text);

    /// @brief Repaints the analog clock and updates the digital clock
    void updateClocks();

    void clearHistoryTable();

    /// @brief Clears all focus on clocks and selected time
    void unselect();

    /// @brief Starts timer corresponding to the saved selected time
    void startTimer();

    /// @brief Restarts the timer corresponding to the saved selected time
    void restartTimer();

    void stopTimer(bool withTimeout);

private:
    void timerEvent(QTimerEvent *event) override;

    /// @brief Clears the state of the widget when pressing
    /// the "Escape" keyboard button
    /// @param event QKeyEvent
    void keyPressEvent(QKeyEvent *event) override;

    /// @brief Gets time and sets it to like an interval into the table
    /// @param time QTime
    void insertIntervalToTable(QTime time);

    /// @brief Updates line editors corresponding to the clock or selected time
    /// @param time QTime
    void updateDigitTime(QTime time);

    void clearFocusOnDigitTimer();

    bool digitTimerHasFocus() noexcept;

    /// @brief Starts timer if selected milliseconds are greater than zero
    void startTimerIfSelected();

    void setupUi();

    void setupConnections();

    /// @brief Loads CSS2 styles from files and setups them for widgets
    void setupStyle();

    /// @brief Loads data from the .ini file and setups properties to the widget
    void setupApp();

    /// @brief The sub-function to save data from the history table
    /// @param settings Where to save
    void saveHistory(QSettings &settings);

    /// @brief Saves properties of the widget and history table to the .ini file
    void saveApp();

    /// @brief Check if there are any changes and call saveApp
    void saveAndroidApp();

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

    // The flag (for Android) marks if there are any changes in the table or not
    // True was set in the functions ("insertIntervalToTable", "clearHistoryTable")
    // False was set in the function ("saveApp")
    bool _dataChanged = false;
};
#endif // CLOCKTIMERAPP_H
