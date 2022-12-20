#ifndef TIMEDRAWWIDGET_H
#define TIMEDRAWWIDGET_H

#include <cmath>

#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <QTime>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;

namespace Ui
{
    namespace math
    {

        static inline QPointF rotate(QPointF point, float angle, QPointF center = QPointF(0, 0))
        {
            float s = std::sin(angle);
            float c = std::cos(angle);

            // translate point back to origin:
            point.setX(point.x() - center.x());
            point.setY(point.y() - center.y());

            // rotate point
            float xnew = point.x() * c - point.y() * s;
            float ynew = point.x() * s + point.y() * c;

            // translate point back:
            point.setX(xnew + center.x());
            point.setY(ynew + center.y());

            return point;
        }

    } // namespace math

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

        inline void updateEverySecond() noexcept
        {
            this->repaint();
        }

    private:
        void paintEvent(QPaintEvent *event) override
        {
            constexpr int penWidth = 10;

            QPointF center(width() / 2, height() / 2);
            int radius = center.x() * 0.8;

            QPainter painter(this);
            painter.setFont(QFont("Consolas", width() / 30));

            QPen pen(Qt::black, 1);

            // Draw divisions
            pen.setWidth(penWidth / 4);
            pen.setColor(QColor(136, 136, 136));

            painter.setPen(pen);

            constexpr int divisionsNumber = 60;
            constexpr float angle = qDegreesToRadians(360.0f / static_cast<float>(divisionsNumber));

            QPointF startDivision1(center.x(), center.y() - radius + center.y() * 0.12);
            QPointF startDivision2(center.x(), center.y() - radius + center.y() * 0.07);
            QPointF endDivision(center.x(), center.y() - radius);

            QPointF textPosition(center.x() - (width() / 70.f), center.y() - radius + center.y() * 0.20);

            for (int i = 0; i < divisionsNumber; ++i)
            {
                if (i % (divisionsNumber / 12) == 0)
                {
                    const int number = i == 0 ? 60 : i;

                    painter.drawText(textPosition,
                                     QString::number(static_cast<int>(number / 5)));

                    pen.setWidth(penWidth / 3);
                    pen.setColor(QColor(136, 136, 136));

                    painter.drawLine(startDivision1, endDivision);
                }
                else
                {
                    pen.setWidth(penWidth / 4);
                    pen.setColor(QColor(136, 136, 136));

                    painter.drawLine(startDivision2, endDivision);
                }

                endDivision = math::rotate(endDivision, angle, center);
                startDivision1 = math::rotate(startDivision1, angle, center);
                startDivision2 = math::rotate(startDivision2, angle, center);
                textPosition = math::rotate(textPosition, angle, center);
            }

            // Draw circle
            pen.setWidth(penWidth);
            painter.setPen(pen);

            painter.drawEllipse(center.x() - radius, center.y() - radius,
                                radius * 2, radius * 2);

            const QTime currentTime = QTime::currentTime();

            // Draw seconds
            pen.setWidth(penWidth / 4);
            pen.setColor(Qt::white);
            painter.setPen(pen);

            painter.drawLine(center, math::rotate({center.x(), center.y() - radius + center.y() * 0.15},
                                                  angle * currentTime.second(), center));

            // Draw minutes
            pen.setWidth(penWidth / 3);
            painter.setPen(pen);

            painter.drawLine(center, math::rotate({center.x(), center.y() - radius + center.y() * 0.25},
                                                  angle * currentTime.minute(), center));

            // Draw hours
            pen.setWidth(penWidth / 2);
            painter.setPen(pen);

            painter.drawLine(center, math::rotate({center.x(), center.y() - radius + center.y() * 0.55},
                                                  angle * (currentTime.hour() % 12) * 5, center));
        }
    };

} // namespace Ui
QT_END_NAMESPACE

#endif // TIMEDRAWWIDGET_H
