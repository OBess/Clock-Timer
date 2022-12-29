#include "clock_timer_app.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClockTimerApp w;
    w.show();
    return a.exec();
}
