#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

QT_BEGIN_NAMESPACE
namespace Settings
{

    namespace Ini
    {
        [[maybe_unused]] const char *const FILE_PATH = "settings.ini";

        [[maybe_unused]] const char *const MAIN_WINDOW = "MainWindow";
        [[maybe_unused]] const char *const IS_MAXIMIZED = "IsMaximized";
        [[maybe_unused]] const char *const WIDTH = "Width";
        [[maybe_unused]] const char *const HEIGHT = "Height";
        [[maybe_unused]] const char *const POS_X = "PositionX";
        [[maybe_unused]] const char *const POS_Y = "PositionY";

        [[maybe_unused]] const char *const HISTORY = "History";
        [[maybe_unused]] const char *const ITEM = "Item";
        [[maybe_unused]] const char *const DATETIME = "Datetime";
        [[maybe_unused]] const char *const INTERVAL = "Interval";

        [[maybe_unused]] const char *const INVALID = "Invalid";
    } // namespace Ini

    namespace Style
    {

        [[maybe_unused]] const char *const MAIN_STYLE = "://styles/main.css";

    } // namespace Style

} // namespace Settings
QT_END_NAMESPACE

#endif // SETTINGS_H
