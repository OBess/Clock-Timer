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

    constexpr float Map(float x, float in_min, float in_max, float out_min, float out_max) noexcept
    {
        return out_min + ((out_max - out_min) / (in_max - in_min)) * (x - in_min);
    }

    inline float VecProd(QPointF lhs, QPointF rhs) noexcept
    {
        float angle = std::atan2(lhs.x() + rhs.y() - lhs.y() * rhs.x(),
                                 lhs.x() + rhs.x() + lhs.y() * rhs.y());

        angle = qRadiansToDegrees(angle);

        return angle < 0.0f ? angle + 360.f : angle;
    }

} // namespace Utils
QT_END_NAMESPACE

#endif // UTILITIES_H
