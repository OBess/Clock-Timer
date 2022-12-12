#ifndef CLOCKTIMERAPP_H
#define CLOCKTIMERAPP_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ClockTimerApp; }
QT_END_NAMESPACE

class ClockTimerApp : public QMainWindow
{
    Q_OBJECT

public:
    ClockTimerApp(QWidget *parent = nullptr);
    ~ClockTimerApp();

private:
    Ui::ClockTimerApp *ui;
};
#endif // CLOCKTIMERAPP_H
