import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    width: 600
    height: 800
    visible: true
    title: "SIP Client"
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            id: bar
            width: parent.width
            Repeater {
                model: ["Вызов", "Контакты","История", "Аккаунты", "Настройки"]

                TabButton {
                    text: modelData
                }
            }
        }

        StackLayout {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: bar.currentIndex
            CallScreen {}
        }
    }
}
