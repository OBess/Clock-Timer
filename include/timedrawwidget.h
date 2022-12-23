#ifndef TIMEDRAWWIDGET_H
#define TIMEDRAWWIDGET_H

#include <QDebug> // DELETE
#include <cmath>

#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <QTime>
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

        inline void updateEverySecond()
        {
            this->repaint();
        }

        constexpr void clear() noexcept
        {
            _mouseAngel = 0.0;
        }

        inline QTime getSelectedTime() const noexcept
        {
            const float selectedDivisions = -_mouseAngel / angleOfDivision();

            return QTime(0, static_cast<int>(selectedDivisions),
                         static_cast<int>(selectedDivisions * 60.f) % 60);
        }

        constexpr bool focused() const noexcept
        {
            return _focused;
        }

    private:
        void mousePressEvent(QMouseEvent *event) override
        {
            setFocus();
            _focused = true;
        }

        void mouseMoveEvent(QMouseEvent *event) override
        {
            const QPointF mainVec(QPointF(_center.x(), _center.y() - _radius) - _center);
            const QPointF mouseVec(event->localPos() - _center);

            _mouseAngel = -vecProd(mainVec, mouseVec);

            getSelectedTime();
        }

        void keyPressEvent(QKeyEvent *event) override
        {
            if (event->key() == Qt::Key_Escape)
            {
                _mouseAngel = 0.0f;
                clearFocus();
                _focused = false;
            }
        }

        void paintEvent([[maybe_unused]] QPaintEvent *event) override
        {
            constexpr int penWidth = 10;

            _center = QPointF(width() / 2, height() / 2);
            _radius = _center.x() * 0.8;

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setFont(QFont("Consolas", width() / 30));

            QPen pen(Qt::black, 1);

            const float angle = angleOfDivision();

            // Draw divisions
            pen.setWidth(penWidth / 4);
            pen.setColor(QColor(136, 136, 136));

            painter.setPen(pen);

            QPointF startDivision1(_center.x(), _center.y() - _radius + _center.y() * 0.12);
            QPointF startDivision2(_center.x(), _center.y() - _radius + _center.y() * 0.07);
            QPointF endDivision(_center.x(), _center.y() - _radius);

            QPointF textPosition(_center.x(), _center.y() - _radius + _center.y() * 0.20);

            for (int i = 0; i < _divisionsNumber; ++i)
            {
                if (i % (_divisionsNumber / 12) == 0)
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

                endDivision = rotate(endDivision, angle, _center);
                startDivision1 = rotate(startDivision1, angle, _center);
                startDivision2 = rotate(startDivision2, angle, _center);
                textPosition = rotate(textPosition, angle, _center);
            }

            // Draw circle
            pen.setWidth(penWidth);
            painter.setPen(pen);

            painter.drawEllipse(_center.x() - _radius, _center.y() - _radius,
                                _radius * 2, _radius * 2);

            const QTime currentTime = QTime::currentTime();

            // Draw seconds
            pen.setWidth(penWidth / 4);
            pen.setColor(Qt::white);
            painter.setPen(pen);

            painter.drawLine(_center, rotate({_center.x(), _center.y() - _radius + _center.y() * 0.15},
                                                  angle * currentTime.second(), _center));

            // Draw minutes
            pen.setWidth(penWidth / 3);
            painter.setPen(pen);

            painter.drawLine(_center, rotate({_center.x(), _center.y() - _radius + _center.y() * 0.25},
                                                  angle * currentTime.minute(), _center));

            // Draw hours
            pen.setWidth(penWidth / 2);
            painter.setPen(pen);

            painter.drawLine(_center, rotate({_center.x(), _center.y() - _radius + _center.y() * 0.55},
                                                  angle * (currentTime.hour() % 12) * 5, _center));

            // Draw arc
            const QRectF rect(_center.x() - _radius, _center.y() - _radius, _radius + _radius, _radius + _radius);
            const float mapped = map(_mouseAngel, 0, 6.28318531f, 0, 5760.f);

            pen.setWidth(penWidth);
            pen.setColor(Qt::white);
            pen.setCapStyle(Qt::FlatCap);

            painter.setPen(pen);

            painter.drawArc(rect, 1440, mapped);

            // Draw shadow arc
            QLinearGradient shadow(0, 0, 100, 0);
            shadow.setColorAt(0.0, QColor(255, 255, 255, 255));
            shadow.setColorAt(1.0, QColor(255, 255, 255, 20));

            QPen shadowPen(shadow, penWidth * 3);

            painter.setPen(shadowPen);

            painter.drawArc(rect, 1440, mapped);

            for (unsigned i = 2; i < 5; ++i)
            {
                pen.setWidth(penWidth * i);
                pen.setColor(QColor(255, 255, 255, 20 * (5 - i)));
                pen.setCapStyle(Qt::FlatCap);

                painter.setPen(pen);

                painter.drawArc(rect, 1440, mapped);
            }
        }

    private:
        inline float angleOfDivision() const noexcept
        {
            return qDegreesToRadians(_angleOfArc / static_cast<float>(_divisionsNumber));
        }

        static inline QPointF rotate(QPointF point, float angle, QPointF _center = QPointF(0, 0))
        {
            float s = std::sin(angle);
            float c = std::cos(angle);

            // translate point back to origin:
            point.setX(point.x() - _center.x());
            point.setY(point.y() - _center.y());

            // rotate point
            float xnew = point.x() * c - point.y() * s;
            float ynew = point.x() * s + point.y() * c;

            // translate point back:
            point.setX(xnew + _center.x());
            point.setY(ynew + _center.y());

            return point;
        }

        static constexpr float map(float x, float in_min, float in_max, float out_min, float out_max) noexcept
        {
          return out_min + ((out_max - out_min) / (in_max - in_min)) * (x - in_min);
        }

        static inline float vecProd(const QPointF& lhs, const QPointF& rhs) noexcept
        {
            float angle = std::atan2(lhs.x() + rhs.y() - lhs.y() * rhs.x(),
                                           lhs.x() + rhs.x() + lhs.y() * rhs.y());

            angle = angle * qDegreesToRadians(360.f) / (2 * M_PI);

            if (angle < 0.0f)
                return angle + qDegreesToRadians(360.f);
            else
                return angle;
        }

    private:
        QPointF _center;
        float _radius = 0;
        float _mouseAngel = 0;

        bool _focused = false;

        const int _divisionsNumber = 60;
        const float _angleOfArc = 360.f;
    };

} // namespace Ui
QT_END_NAMESPACE

#endif // TIMEDRAWWIDGET_H
