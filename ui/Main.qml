import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SipClient
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
        readonly property color accent: "#0078d4"
        readonly property color textPrimary: "#2c3e50"
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

                RowLayout {
                    id: selectedAccountInfo
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                    spacing: 15
                    readonly property var selectedAccount : accountsManager.selectedAccount
                    // "лампочка" статуса регистрации
                    RegStatusLed {
                        status: selectedAccountInfo.selectedAccount ?
                                    selectedAccountInfo.selectedAccount.regStatus :
                                    "offline";
                        Layout.preferredHeight: 10
                        Layout.preferredWidth: 10
                        Layout.alignment: Qt.AlignVCenter
                    }
                    // Имя аккаунта и номер или ошибка регистрации
                    ColumnLayout {
                        spacing: 2
                        Label {
                            text: selectedAccountInfo.selectedAccount ?
                                      selectedAccountInfo.selectedAccount.displayName :
                                      "Нет аккаунта"
                            font.bold: true; font.pixelSize: 16
                            color: "black"
                        }
                        Label {
                            text: selectedAccountInfo.selectedAccount  ? (
                                selectedAccountInfo.selectedAccount.regStatus === "error" ?
                                      "Ошибка: " + selectedAccountInfo.selectedAccount.lastError
                                    : selectedAccountInfo.selectedAccount.username) : "";
                            font.pixelSize: 11
                            color: selectedAccountInfo.selectedAccount && selectedAccountInfo.selectedAccount.regStatus === "error" ? "#F44336" : "#757575"
                        }
                    }
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
                        flat: true
                        text: modelData.name

                        // Подсветка активной кнопки
                        background: Rectangle {
                            color: mainStack.currentIndex === index ? "white" : "transparent"
                            radius: 5
                            border.color: mainStack.currentIndex === index ? theme.accent : "transparent"
                        }

                        onClicked: mainStack.currentIndex = index
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            StackLayout {
                id: mainStack
                anchors.fill: parent
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
                }
                HistoryScreen {}
                AccountsScreen {}
                SettingsScreen {}
            }
            Button {
                id: toggleButton
                x: 10
                y: 10
                z: 10 // поверх интерфейса
                width: 40
                height: 40
                flat: true

                text: root.sidebarOpen ? "◀" : "▶"

                background: Rectangle {
                    color: "white"
                    radius: 20
                    border.color: theme.sidebarBack
                    opacity: 0.8
                }

                onClicked: root.sidebarOpen = !root.sidebarOpen
            }
        }
    }
}
