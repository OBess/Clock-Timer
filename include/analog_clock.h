#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    /// @brief Extends QWidget class to be shown on the screen
    class AnalogClock : public QWidget
    {
        Q_OBJECT

    public:
        explicit AnalogClock(QWidget *parent = nullptr)
            : QWidget(parent)
        {
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            setMinimumWidth(300);
            setMinimumHeight(300);
        }

        ~AnalogClock() override = default;

        /// @brief Clears selected time and focus of the widget
        inline void clearSelected() noexcept
        {
            _mouseAngel = 0.0;

            _focused = false;
            clearFocus();
        }

        /// @brief Sets selected time from the outside
        ///
        /// @param selectedTime QTime
        inline void setSelectedTime(QTime selectedTime) noexcept
        {
            const float angle = -angleOfDivision();

            _mouseAngel = (angle * selectedTime.hour() * 60) +
                          (angle * selectedTime.minute()) +
                          (angle * selectedTime.second() / 60);
        }

        /// @brief Returns selected time in digital form
        ///
        /// @return QTime
        [[nodiscard]] inline QTime getSelectedTime() const noexcept
        {
            const float selectedDivisions = -_mouseAngel / angleOfDivision();

            return QTime(0, static_cast<int>(selectedDivisions),
                         static_cast<int>(selectedDivisions * 60.f) % 60);
        }

        [[nodiscard]] constexpr bool focused() const noexcept
        {
            return _focused;
        }

    private:
        /// @brief Sets focus on the widget
        /// @param event QMouseEvent
        void mousePressEvent([[maybe_unused]] QMouseEvent *event) override
        {
            setFocus();
            _focused = true;
        }

        /// @brief Computes the mouse angle corresponding to the primary vector
        /// @param event QMouseEvent
        void mouseMoveEvent(QMouseEvent *event) override
        {
            const QPointF mainVec(0, -_radius);
            const QPointF mouseVec(event->localPos() - _center);

            _mouseAngel = -Utils::VecProd(mainVec, mouseVec);
        }

        /// @brief Clears the state of the widget when pressing
        /// the "Escape" keyboard button
        /// @param event QKeyEvent
        void keyPressEvent(QKeyEvent *event) override
        {
            if (event->key() == Qt::Key_Escape)
            {
                clearSelected();
            }
        }

        /// @brief Paints analog clock on the widget
        /// @param event QPaintEvent
        void paintEvent([[maybe_unused]] QPaintEvent *event) override
        {
            constexpr int penWidth = 10;

            // Sets center of the clock and radius corrdespoding to center
            _center = QPointF(width() / 2, height() / 2);
            _radius = _center.x() * 0.8;

            // New instances to draw the clock
            QPainter painter(this);
            QPen pen(Qt::black, 1);

            painter.beginNativePainting();
            painter.setRenderHint(QPainter::Antialiasing, true);

            // Gets angel between divisions
            const float angle = angleOfDivision();

            // Draw divisions
            pen.setWidth(penWidth / 4);
            pen.setColor(QColor(136, 136, 136));

            painter.setPen(pen);

            // Moves painter to center
            painter.translate(_center);

            // It saves the state of the painter to restore it after
            painter.save();

            QPointF startHourDivision(0, -_radius * 0.85);
            QPointF startMinuteDivision(0, -_radius * 0.95);
            QPointF endDivision(0, -_radius);

            for (unsigned i = 0; i < _divisionsNumber; ++i)
            {
                // If 'i' divides by 5 without a remainder,
                // then draw a bolder line
                if (i % (_divisionsNumber / 12) == 0)
                {
                    pen.setWidth(penWidth / 2);
                    pen.setColor(QColor(180, 180, 180));
                    painter.setPen(pen);

                    painter.drawLine(startHourDivision, endDivision);
                }
                // or draw smaller line
                else
                {
                    pen.setWidth(penWidth / 4);
                    pen.setColor(QColor(136, 136, 136));
                    painter.setPen(pen);

                    painter.drawLine(startMinuteDivision, endDivision);
                }

                painter.rotate(angle);
            }

            const QTime currentTime = QTime::currentTime();

            // Draw seconds
            pen.setWidth(penWidth / 4);
            pen.setColor(Qt::white);
            painter.setPen(pen);

            painter.rotate(angle * currentTime.second());
            painter.drawLine(QPointF{0, 0}, {0, -_radius * 0.8});

            // Draw minutes
            pen.setWidth(penWidth / 3);
            painter.setPen(pen);

            painter.rotate(-angle * currentTime.second());
            painter.rotate(angle * currentTime.minute());
            painter.drawLine(QPointF{0, 0}, {0, -_radius * 0.6});

            // Draw hours
            pen.setWidth(penWidth / 2);
            painter.setPen(pen);

            painter.rotate(-angle * currentTime.minute());
            painter.rotate(angle * (currentTime.hour() % 12) * 5);
            painter.drawLine(QPointF{0, 0}, {0, -_radius * 0.4});

            // Restores saved state
            painter.restore();

            // Draw arc
            const float entireArc = 5760.f;
            const float startArc = entireArc / 4.f;

            const QRectF rect(-_radius, -_radius,
                              _radius + _radius,
                              _radius + _radius);
            const float mappedAngel = Utils::Map(_mouseAngel, 0, 360.f,
                                                 0, entireArc);

            pen.setWidth(penWidth);
            pen.setColor(Qt::white);
            pen.setCapStyle(Qt::FlatCap);

            painter.setPen(pen);
            painter.drawArc(rect, startArc, mappedAngel);

            // Draw shadow of the arc
            QLinearGradient shadow(0, 0, 100, 0);
            shadow.setColorAt(0.0, QColor(255, 255, 255, 255));
            shadow.setColorAt(1.0, QColor(255, 255, 255, 20));

            const unsigned numberOfShadows = 5;

            for (unsigned i = 0; i < numberOfShadows; ++i)
            {
                pen.setWidth(penWidth * i);
                pen.setColor(QColor(255, 255, 255, 20 * (numberOfShadows - i)));
                pen.setCapStyle(Qt::FlatCap);

                painter.setPen(pen);
                painter.drawArc(rect, startArc, mappedAngel);
            }

            painter.endNativePainting();
        }

    private:
        /// @brief Divides the angle of the arc by the number of divisions
        /// @return Angle between divisions
        inline float angleOfDivision() const noexcept
        {
            return _angleOfArc / static_cast<float>(_divisionsNumber);
        }

    private:
        QPointF _center;
        float _radius = 0;
        float _mouseAngel = 0;

        bool _focused = false;

        const unsigned _divisionsNumber = 60;
        const float _angleOfArc = 360.f;
    };

} // namespace Ui
QT_END_NAMESPACE

#endif // ANALOGCLOCK_H
