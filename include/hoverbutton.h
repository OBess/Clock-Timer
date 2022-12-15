#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QPushButton>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{

    class HoverButton : public QPushButton
    {
        Q_OBJECT

    public:
        HoverButton(QWidget *parent = nullptr)
            : QPushButton(parent) {}

        virtual ~HoverButton() override = default;

    protected:
        virtual void enterEvent(QEvent *) override
        {
            setCursor(Qt::PointingHandCursor);
        }

        virtual void leaveEvent(QEvent *event) override
        {
            setCursor(Qt::ArrowCursor);
        }

    private:
    };

} // namespace Ui
QT_END_NAMESPACE

#endif // HOVERBUTTON_H