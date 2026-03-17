import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SipClient;
import "components"
ApplicationWindow {
    id: root
    width: 600
    height: 800
    visible: true
    title: "SIP Client"

    property bool sidebarOpen: true

    QtObject {
        id: theme
        readonly property color backgroundLight: "#f0f7ff"
        readonly property color sidebarBack: "#e3effd"
        readonly property color sidebarActive: "#34495e"
        readonly property color accent: "#0078d4"
        readonly property color textPrimary: "#2c3e50"
        readonly property color textSecondary: "#636e72"
        readonly property color textOnDark: "#ffffff"
    }
    background: Rectangle {
        color: theme.backgroundLight
    }
    RowLayout {
        anchors.fill: parent
        spacing: 0
        Rectangle {
            id: sidebar
            Layout.fillHeight: true
            Layout.preferredWidth: root.sidebarOpen ? 200 : 0
            clip: true
            color: theme.sidebarBack
            Behavior on Layout.preferredWidth {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }
            ColumnLayout {
                anchors.fill: parent
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 10
                spacing: 5

                SelectedAccountInfo {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                }

                // Список кнопок для навигации
                Repeater {
                    model: [
                        { name: "Вызов", icon: "phone" },
                        { name: "Контакты", icon: "contacts" },
                        { name: "История", icon: "history" },
                        { name: "Аккаунты", icon: "person" },
                        { name: "Настройки", icon: "settings" }
                    ]

                    delegate: Button {
                        Layout.fillWidth: true
                        id: delegateRoot
                        flat: true
                        text: modelData.name

                        // Подсветка активной кнопки
                        background: Rectangle {
                            color: mainStack.currentIndex === index ? "white" : (delegateRoot.hovered ? "#f8f9fa" : "transparent")
                            radius: 5
                            border.color: mainStack.currentIndex === index ? theme.accent : "transparent"
                        }

                        onClicked: mainStack.currentIndex = index
                    }
                }
                ChatList {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onChatSelected: (number) => {
                        mainStack.currentIndex = 5
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }
        Rectangle {
            color: "black"
            Layout.fillHeight: true
            Layout.preferredWidth: root.sidebarOpen ? 1 : 0
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                Button {
                    id: toggleButton
                    Layout.preferredHeight: 35
                    Layout.preferredWidth: 35
                    Layout.margins: 10
                    flat: true

                    text: root.sidebarOpen ? "◀" : "▶"

                    background: Rectangle {
                        color: theme.accent
                        radius: 18
                        border.color: theme.sidebarBack
                        opacity: 0.8
                    }

                    onClicked: root.sidebarOpen = !root.sidebarOpen
                }

                CallBar {
                    Layout.fillWidth: true
                    show: mainStack.currentIndex !== 0
                }
            }

            StackLayout {
                id: mainStack
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 5
                Layout.topMargin: 0
                onCurrentIndexChanged: fadeIn.restart()

                NumberAnimation {
                    id: fadeIn
                    target: mainStack
                    property: "opacity"
                    from: 0; to: 1; duration: 400
                }

                CallScreen { id: callScreen }
                ContactsScreen {
                    onContactCalled: (number) => {
                        callScreen.currentNumber = number
                        mainStack.currentIndex = 0
                    }
                    onChatSelected: (number) => {
                        mainStack.currentIndex = 5
                    }
                }
                HistoryScreen {}
                AccountsScreen {}
                SettingsScreen {}
                ChatScreen { id: chatScreen }
            }
        }
    }
}
