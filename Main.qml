import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 600
    height: 800
    visible: true
    title: "SIP Client"
    header: TabBar {
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
        currentIndex: bar.currentIndex
        anchors.fill: parent
        CallScreen {}
        ContactsScreen {}
        HistoryScreen {}
    }
}
