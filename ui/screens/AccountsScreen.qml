import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    property int activeAccountId: 1

    // Элемент списка
    Component {
        id: accountDelegate
        ItemDelegate {
            id: delegateRoot
            width: accountsList.width

            readonly property bool isExpanded: model.accId === accountsList.expandedAccountId
            readonly property bool isActive: model.accId === root.activeAccountId

            contentItem: ColumnLayout {
                spacing: 0

                // Всегда видимая часть
                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 70
                    spacing: 15

                    // "лампочка" статуса регистрации
                    Rectangle {
                        id: statusLed
                        width: 10; height: 10; radius: 5
                        Layout.alignment: Qt.AlignVCenter

                        color: {
                            if (model.regStatus === "registered") return "#4CAF50" // Зеленый
                            if (model.regStatus === "registering") return "#FFC107" // Желтый
                            if (model.regStatus === "error") return "#F44336"      // Красный
                            return "#BDBDBD"                                      // Серый (offline)
                        }

                        // Анимация при попытке подключения
                        SequentialAnimation {
                            running: model.regStatus === "registering"
                            loops: Animation.Infinite

                            OpacityAnimator {
                                target: statusLed
                                from: 0.3
                                to: 1
                                duration: 1000
                            }

                            OpacityAnimator {
                                target: statusLed
                                from: 1
                                to: 0.3
                                duration: 1000
                            }
                        }
                    }
                    // Имя аккаунта и номер или ошибка регистрации
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Label {
                            text: model.name; font.bold: true; font.pixelSize: 16
                            color: delegateRoot.isActive ? "#2196F3" : "black"
                        }
                        Label {
                            text: model.regStatus === "error" ? "Ошибка: " + model.lastError : model.number
                            font.pixelSize: 11
                            color: model.regStatus === "error" ? "#F44336" : "#757575"
                        }
                    }
                    // Переключатель для регистрации
                    Switch {
                        id: regSwitchs
                        Layout.alignment: Qt.AlignVCenter

                        onToggled: {
                            // TODO: регистрация
                        }
                    }
                    // Кнопка переключения активного аккаунта
                    Button {
                        text: "Выбрать"
                        visible: !delegateRoot.isActive
                        onClicked: root.activeAccountId = model.accId
                    }

                    Label {
                        text: isActive ? "Активен" : ""
                        color: "#4caf50"; font.pixelSize: 12
                        visible: delegateRoot.isActive
                    }
                }

                // Дополнительная информация
                ColumnLayout {
                    id: detailBox
                    Layout.fillWidth: true

                    visible: delegateRoot.isExpanded

                    Rectangle { Layout.fillWidth: true; height: 1; color: "#eee" }

                    GridLayout {
                        columns: 2
                        columnSpacing: 20
                        Layout.margins: 10

                        Label { text: "Сервер:"; color: "gray" }
                        Label { text: model.server; font.family: "Monospace" }

                        Label { text: "Порт:"; color: "gray" }
                        Label { text: model.port }

                        Label { text: "Протокол:"; color: "gray" }
                        Label { text: model.proto; font.bold: true }
                    }
                }
            }

            onClicked: {
                if (isExpanded)
                    accountsList.expandedAccountId = -1
                else
                    accountsList.expandedAccountId = model.accId
            }

            background: Rectangle {
                color: delegateRoot.isActive ? "#f0f9ff" : "white"
                border.color: delegateRoot.isActive ? "#bae7ff" : "#eee"
                radius: 8
            }
        }
    }

    ListModel {
        id: accountsModel
        ListElement {
            accId: 1
            name: "Рабочий"
            number: "10112312"
            server: "sip.company.com"
            port: 5060
            proto: "UDP"
            regStatus: "registered"
        }
        ListElement {
            accId: 2
            name: "Домашний"
            number: "74951234567"
            server: "sip.provider.net"
            port: 5061
            proto: "TLS"
            regStatus: "registering"
        }
        ListElement {
            accId: 3
            name: "Test"
            number: "test"
            server: "192.168.1.50"
            port: 5060
            proto: "TCP"
            regStatus: "error"
            lastError: "401"
        }
    }

    ListView {
        id: accountsList
        anchors.fill: parent
        model: accountsModel
        spacing: 10
        clip: true

        property int expandedAccountId: -1

        delegate: accountDelegate
    }
}
