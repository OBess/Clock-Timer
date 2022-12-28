#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

QT_BEGIN_NAMESPACE
namespace Settings
{
    /// @brief The batch of predefined constants for the settings file
    namespace Ini
    {
        [[maybe_unused]] inline const char *const FILE_PATH = "settings.ini";

        [[maybe_unused]] inline const char *const MAIN_WINDOW = "MainWindow";
        [[maybe_unused]] inline const char *const IS_MAXIMIZED = "IsMaximized";
        [[maybe_unused]] inline const char *const WIDTH = "Width";
        [[maybe_unused]] inline const char *const HEIGHT = "Height";
        [[maybe_unused]] inline const char *const POS_X = "PositionX";
        [[maybe_unused]] inline const char *const POS_Y = "PositionY";

        [[maybe_unused]] inline const char *const HISTORY = "History";
        [[maybe_unused]] inline const char *const ITEM = "Item";
        [[maybe_unused]] inline const char *const DATETIME = "Datetime";
        [[maybe_unused]] inline const char *const INTERVAL = "Interval";

        [[maybe_unused]] inline const char *const INVALID = "Invalid";
    } // namespace Ini

    /// @brief The paths to CSS2 styles files
    namespace Style
    {

        [[maybe_unused]] inline const char *const MAIN_STYLE = "://styles/main.css";

    } // namespace Style

} // namespace Settings
QT_END_NAMESPACE

#endif // SETTINGS_H
