#ifndef UTILS_H
#define UTILS_H

#include <QtMath>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Utils
{

    /// @brief Gethers data from QTime and converts it to milliseconds
    /// @param time QTime instance
    /// @return time in milliseconds
    inline int TimeToMills(QTime time) noexcept
    {
        return (time.hour() * 3600 + time.minute() * 60 + time.second()) * 1000;
    }

    /// @brief Creates a new QTime instance from got milliseconds
    /// @param mills milliseconds
    /// @return A new instance of QTime
    inline QTime MillsToTime(int mills) noexcept
    {
        const int seconds = mills / 1000;
        return QTime(seconds / 3600, seconds / 60, seconds % 60);
    }

    /// @brief Transforms value x from one range to another using proportion
    /// @param x target value
    /// @param in_min minimum value of origin range
    /// @param in_max maximum value of origin range
    /// @param out_min minimum value of new range
    /// @param out_max maximum value of new range
    /// @return Mapped target value to new range
    constexpr float Map(float x, float in_min, float in_max,
                        float out_min, float out_max) noexcept
    {
        return out_min + ((out_max - out_min) / (in_max - in_min)) * (x - in_min);
    }

    /// @brief Computes vector product
    /// @param lhs left hand side of computation
    /// @param rhs right hand side of computation
    /// @return Result of vector product
    inline float VecProd(QPointF lhs, QPointF rhs) noexcept
    {
        float angle = std::atan2(lhs.x() + rhs.y() - lhs.y() * rhs.x(),
                                 lhs.x() + rhs.x() + lhs.y() * rhs.y());

        angle = qRadiansToDegrees(angle);

        return angle < 0.0f ? angle + 360.f : angle;
    }

} // namespace Utils
QT_END_NAMESPACE

#endif // UTILS_H
