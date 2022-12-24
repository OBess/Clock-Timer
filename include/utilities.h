#ifndef UTILITIES_H
#define UTILITIES_H

#include <QTime>

QT_BEGIN_NAMESPACE
namespace Utils {

    inline int TimeToMills(QTime time) noexcept
    {
        return (time.hour() * 3600 + time.minute() * 60 + time.second()) * 1000;
    }

    inline QTime MillsToTime(int mills) noexcept
    {
        const int seconds = mills / 1000;
        return QTime(seconds / 3600, seconds / 60, seconds % 60);
    }

} // namespace Utils
QT_END_NAMESPACE

#endif // UTILITIES_H
