#ifndef CLOCKTIMERAPP_H
#define CLOCKTIMERAPP_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ClockTimerApp;
    class ClockWidget;
}
QT_END_NAMESPACE

class ClockTimerApp : public QWidget
{
    Q_OBJECT

public:
    ClockTimerApp(QWidget *parent = nullptr);
    ~ClockTimerApp();

private:
    void setupUi();
    void setupConnections();

    Ui::ClockWidget *_clockWidget;
    Ui::ClockTimerApp *ui;
};
#endif // CLOCKTIMERAPP_H
