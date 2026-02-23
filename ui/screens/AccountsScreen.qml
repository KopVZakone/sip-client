import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    property int activeAccountId: 1

    function openEditor(accData = null) {
        if (accData) {
            accountDialog.editingIndex = accData.id
            accountDialog.title = "Редактировать аккаунт"
            displayNameField.text = accData.displayName
            usernameField.text = accData.username
            passwordField.text = accData.password
            domainField.text = accData.domain
            portField.text = accData.port.toString()
            protocolField.currentIndex = protocolField.model.indexOf(accData.protocol);
        } else {
            accountDialog.editingIndex = -1
            accountDialog.title = "Новый аккаунт"
            displayNameField.clear()
            usernameField.clear()
            passwordField.clear()
            domainField.text = "sip.test.com"
            portField.text = "5060"
            protocolField.currentIndex = 0;

        }
        accountDialog.open()
    }


    // Элемент списка
    Component {
        id: accountDelegate
        ItemDelegate {
            id: delegateRoot
            width: accountsList.width

            readonly property bool isExpanded: model.id === accountsList.expandedAccountId
            readonly property bool isActive: model.id === root.activeAccountId

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
                        Layout.preferredWidth: 10
                        Layout.preferredHeight: 10
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
                        spacing: 2
                        Label {
                            text: model.displayName; font.bold: true; font.pixelSize: 16
                            color: delegateRoot.isActive ? "#2196F3" : "black"
                        }
                        Label {
                            text: model.regStatus === "error" ? "Ошибка: " + model.lastError : model.username
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
                        Layout.alignment: Qt.AlignVCenter
                        onClicked: root.activeAccountId = model.id
                    }

                    Label {
                        text: isActive ? "Активен" : ""
                        color: "#4caf50"; font.pixelSize: 12
                        visible: delegateRoot.isActive
                        Layout.alignment: Qt.AlignVCenter
                    }
                    // Пустое место для alignment right
                    Item {
                        Layout.fillWidth: true
                    }
                    // Кнопка изменения аккаунта
                    Button {
                        text: "🔧"
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        flat: true
                        onClicked: root.openEditor(model)
                    }
                    // Кнопка удаления аккаунта
                    Button {
                        text: "x"
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        flat: true
                        onClicked: accountsModel.removeAccount(model.id)
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
                        Label { text: model.domain; font.family: "Monospace" }

                        Label { text: "Порт:"; color: "gray" }
                        Label { text: model.port }

                        Label { text: "Протокол:"; color: "gray" }
                        Label { text: model.protocol; font.bold: true }
                    }
                }
            }

            onClicked: {
                if (isExpanded)
                    accountsList.expandedAccountId = -1
                else
                    accountsList.expandedAccountId = model.id
            }

            background: Rectangle {
                color: delegateRoot.isActive ? "#f0f9ff" : "white"
                border.color: delegateRoot.isActive ? "#bae7ff" : "#eee"
                radius: 8
            }
        }
    }
    Dialog {
        id: accountDialog
        anchors.centerIn: parent
        width: parent.width * 0.8
        modal: true
        standardButtons: Dialog.Save | Dialog.Cancel
        property int editingIndex: -1

        ColumnLayout {
            width: parent.width
            TextField { id: displayNameField; placeholderText: "Название"; Layout.fillWidth: true }
            TextField { id: usernameField; placeholderText: "Номер/Логин"; Layout.fillWidth: true }
            TextField { id: passwordField; echoMode: TextInput.PasswordEchoOnEdit; placeholderText: "Пароль"; Layout.fillWidth: true }
            RowLayout {
                TextField { id: domainField; placeholderText: "Сервер"; Layout.fillWidth: true }
                TextField { id: portField; placeholderText: "Порт"; Layout.preferredWidth: 80; inputMethodHints: Qt.ImhDigitsOnly }
                ComboBox { id: protocolField; model: ["UDP", "TCP", "TLS"]; }
            }
        }

        onAccepted: {
            accountsModel.saveAccount(editingIndex,
                                      displayNameField.text,
                                      usernameField.text,
                                      passwordField.text,
                                      domainField.text,
                                      parseInt(portField.text),
                                      protocolField.currentText)
        }
    }    
    // ListModel {
    //     id: accountsModel
    //     ListElement {
    //         accId: 1
    //         name: "Рабочий"
    //         number: "10112312"
    //         server: "sip.company.com"
    //         port: 5060
    //         proto: "UDP"
    //         regStatus: "registered"
    //     }
    //     ListElement {
    //         accId: 2
    //         name: "Домашний"
    //         number: "74951234567"
    //         server: "sip.provider.net"
    //         port: 5061
    //         proto: "TLS"
    //         regStatus: "registering"
    //     }
    //     ListElement {
    //         accId: 3
    //         name: "Test"
    //         number: "test"
    //         server: "192.168.1.50"
    //         port: 5060
    //         proto: "TCP"
    //         regStatus: "error"
    //         lastError: "401"
    //     }
    // }
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        Button {
            text: "+ Добавить аккаунт"
            Layout.fillWidth: true
            onClicked: root.openEditor()
        }
        ListView {
            id: accountsList
            Layout.fillHeight: true;
            Layout.fillWidth: true;
            model: accountsModel
            spacing: 10
            clip: true

            property int expandedAccountId: -1

            delegate: accountDelegate
        }
    }
}
