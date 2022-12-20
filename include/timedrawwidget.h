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
            : QWidget(parent)
        {
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            setMinimumWidth(300);
            setMinimumHeight(300);
        }

        ~ClockWidget() override = default;

    private:
        void paintEvent(QPaintEvent *event) override
        {
            constexpr int penWidth = 10;

            QPoint center{width() / 2, height() / 2};
            int radius = center.x() * 0.8;

            QPainter painter(this);
            QPen pen(Qt::black, 1);

            // Draw divisions
            pen.setWidth(penWidth / 3);
            pen.setColor(QColor("#fff"));
            painter.setPen(pen);

            constexpr int divisionsNumber = 60;
            constexpr double angle = 360 / divisionsNumber;

            QPoint startDivision(center.x(), center.y() - radius + center.y() * 0.15);
            QPoint endDivision(center.x(), center.y() - radius);

            for (int i = 0; i < divisionsNumber; ++i)
            {
                painter.drawLine(startDivision, endDivision);
                // painter.rotate(angle);
            }

            // Draw circle
            pen.setWidth(penWidth);
            pen.setColor(QColor("#888"));
            painter.setPen(pen);

            painter.drawEllipse(center.x() - radius, center.y() - radius,
                                radius * 2, radius * 2);
        }
    };
} // namespace Ui
QT_END_NAMESPACE

#endif // TIMEDRAWWIDGET_H
