#include "clock_timer_app.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClockTimerApp w;

#ifdef Q_OS_ANDROID
    // This connect exists to call save function
    QObject::connect(&a, &QGuiApplication::applicationStateChanged, &w, &ClockTimerApp::onAppStateChange);
#endif // Q_OS_ANDROID

    w.show();
    return a.exec();
}
