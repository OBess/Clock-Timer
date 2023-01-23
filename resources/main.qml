import QtQuick 2.15
import QtQuick.Controls 2.15

import ClockTimerApp.ClockTimerApp 1.0

ApplicationWindow {
    title: "ClockTimerApp"
    minimumWidth: content.implicitWidth
    minimumHeight: content.implicitHeight
    width: content.implicitWidth
    height: content.implicitHeight
    visible: true

    ClockTimerApp { id: app }

    Row {
        id: content
        anchors.fill: parent
        spacing: 20
        padding: 20

        HistoryItem {
            id: history
            model: app.historyModel
        }

    }
}
