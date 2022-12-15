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
            : QWidget(parent), _pen(Qt::black, 3)
        {
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        }

        virtual ~ClockWidget() override = default;

        inline void setPen(const QColor &color) noexcept
        {
            _pen = QPen(color);
        }

        inline void setPen(QPen &&pen) noexcept
        {
            _pen = std::move(pen);
        }

        inline void setPen(Qt::PenStyle style) noexcept
        {
            _pen = QPen(style);
        }

        inline void setWidth(int width) noexcept
        {
            _pen.setWidth(width);
        }

        inline void setColor(const QColor &color) noexcept
        {
            _pen.setColor(color);
        }

    protected:
        virtual void paintEvent(QPaintEvent *event) override
        {
            QPainter painter(this);
            painter.setPen(_pen);

            QRect size = event->rect();

            painter.drawEllipse(0, 0, size.height() * 0.6, size.height() * 0.6);
            painter.drawRect(size);
        }

    private:
        QPen _pen;
    };
} // namespace Ui
QT_END_NAMESPACE

#endif // TIMEDRAWWIDGET_H
