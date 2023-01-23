import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property alias model: table.model

    implicitWidth: groupContents.implicitWidth
    height: groupContents.implicitHeight

    Styles { id: style }

    Column {
        id: groupContents
        anchors.fill: parent
        spacing: 20

        Label {
            text: "History"
            font.family: style.font_family
            font.pointSize: 24
        }

        Item {
            height: 300
            width: parent.width
            clip: true

            ScrollView {
                anchors.fill: parent

                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                TableView {
                    id: table

                    anchors.fill: parent
                    delegate: ItemDelegate {
                        text: "Item " + (index + 1)
                        width: parent.width
                    }
                }
            }
        }

        ButtonInstance {
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
