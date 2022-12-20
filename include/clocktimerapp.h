#ifndef CLOCKTIMERAPP_H
#define CLOCKTIMERAPP_H

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
    void insertInterval();

private:
    void setupUi();
    void setupConnections();
    void setupStyle();

    static QPushButton *buildBtn(const char *text, QWidget *parent);

    Ui::ClockTimerApp *ui = nullptr;

    Ui::ClockWidget *_clockWidget = nullptr;
    QPushButton *_startBtn = nullptr;
    QPushButton *_restartBtn = nullptr;
    QPushButton *_clearBtn = nullptr;

    HistoryModel *_historyModel = nullptr;
};
#endif // CLOCKTIMERAPP_H
