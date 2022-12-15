#ifndef CLOCKTIMERAPP_H
#define CLOCKTIMERAPP_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ClockTimerApp;
    class ClockWidget;
    class HoverButton;
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
    void setupStyle();

    Ui::HoverButton *buildBtn(const char *text);

    Ui::ClockTimerApp *ui = nullptr;

    Ui::ClockWidget *_clockWidget = nullptr;
    Ui::HoverButton *_startBtn = nullptr;
    Ui::HoverButton *_restartBtn = nullptr;
    Ui::HoverButton *_clearBtn = nullptr;
};
#endif // CLOCKTIMERAPP_H
