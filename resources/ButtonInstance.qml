import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property string text: "Click Me"

    Styles { id: styles }

    width: 251
    height: 51

    Button {
        id: button
        text: root.text
        anchors.fill: parent
        font.pointSize: 12
        font.family: styles.font_family
    }
}
