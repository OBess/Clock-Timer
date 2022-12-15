#ifndef TIMEDRAWWIDGET_H
#define TIMEDRAWWIDGET_H

// TODO:: Delete
#include <QDebug>

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ClockWidget : public QWidget
    {
        Q_OBJECT

    public:
        ClockWidget(QWidget *parent = nullptr)
            : QWidget(parent), _pen(Qt::black, 1)
        {
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            setMinimumWidth(300);
            setMinimumHeight(300);
        }

        virtual ~ClockWidget() override = default;

        inline void setColor(const QColor &color) noexcept
        {
            _pen.setColor(color);
        }

    protected:
        virtual void paintEvent(QPaintEvent *event) override
        {
            QPainter painter(this);

            _pen.setWidth(5);
            _pen.setColor(QColor("#3399FF"));

            painter.setPen(_pen);

            QPoint center{width() / 2, height() / 2};
            int radius = center.x() * 0.8;

            painter.drawEllipse(center.x() - radius, center.y() - radius - center.y() * 0.1,
                                radius * 2, radius * 2);
        }

    private:
        QPen _pen;
    };
} // namespace Ui
QT_END_NAMESPACE

#endif // TIMEDRAWWIDGET_H
