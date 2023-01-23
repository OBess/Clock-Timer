//#include "clock_timer_app.h"

//#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    ClockTimerApp w;

//#ifdef Q_OS_ANDROID
//    // This connect exists to call save function
//    QObject::connect(&a, &QGuiApplication::applicationStateChanged, &w, &ClockTimerApp::onAppStateChange);
//#endif // Q_OS_ANDROID

//    w.show();
//    return a.exec();
//}

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "clock_timer_app.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    // Registers classes types
    qmlRegisterType<ClockTimerApp>("ClockTimerApp.ClockTimerApp", 1, 0, "ClockTimerApp");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
         &app, [url](QObject *obj, const QUrl &objUrl) {
             if (!obj && url == objUrl)
                 QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
