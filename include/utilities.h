#ifndef UTILITIES_H
#define UTILITIES_H

#include <QtMath>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Utils
{

    inline int TimeToMills(QTime time) noexcept
    {
        return (time.hour() * 3600 + time.minute() * 60 + time.second()) * 1000;
    }

    inline QTime MillsToTime(int mills) noexcept
    {
        const int seconds = mills / 1000;
        return QTime(seconds / 3600, seconds / 60, seconds % 60);
    }

    inline QPointF Rotate(QPointF point, float angle, QPointF center = QPointF(0, 0))
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

    constexpr float Map(float x, float in_min, float in_max, float out_min, float out_max) noexcept
    {
        return out_min + ((out_max - out_min) / (in_max - in_min)) * (x - in_min);
    }

    inline float VecProd(QPointF lhs, QPointF rhs) noexcept
    {
        float angle = std::atan2(lhs.x() + rhs.y() - lhs.y() * rhs.x(),
                                 lhs.x() + rhs.x() + lhs.y() * rhs.y());

        angle = angle * qDegreesToRadians(360.f) / (2 * M_PI);

        return angle < 0.0f ? angle + qDegreesToRadians(360.f) : angle;
    }

} // namespace Utils
QT_END_NAMESPACE

#endif // UTILITIES_H
