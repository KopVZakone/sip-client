import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import SipClient
import "../components"
Item {
    id: root

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
            implicitHeight: isExpanded ? 160 : 80
            readonly property bool isExpanded: model.id === accountsList.expandedAccountId
            readonly property bool isActive: model.id === accountsManager.selectedAccountIndex

            Behavior on implicitHeight { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad } }

            contentItem: ColumnLayout {
                spacing: 0
                clip: true
                // Всегда видимая часть
                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 70
                    Layout.leftMargin: delegateRoot.isActive ? 15 : 10
                    Layout.rightMargin: 10
                    spacing: 15

                    // "лампочка" статуса регистрации
                    RegStatusLed {
                        status: model.regStatus
                        Layout.preferredHeight: 10
                        Layout.preferredWidth: 10
                        Layout.alignment: Qt.AlignVCenter
                    }
                    // Имя аккаунта и номер или ошибка регистрации
                    ColumnLayout {
                        spacing: 0
                        Layout.fillWidth: true
                        Label {
                            text: model.displayName
                            font.bold: true; font.pixelSize: 16
                            color: theme.textPrimary
                        }
                        Label {
                            text: model.regStatus === "error" ? model.lastError : model.username
                            font.pixelSize: 12
                            color: model.regStatus === "error" ? "#e74c3c" : theme.textSecondary
                            elide: Text.ElideRight
                        }
                    }
                    // Переключатель для регистрации
                    Switch {
                        id: regSwitch
                        checked: model.regStatus !== "offline" && model.regStatus !== "error"
                        scale: 0.8
                        onToggled: {
                            checked ? accountsManager.registerAccount(model.id) :
                                      accountsManager.unregisterAccount(model.id)
                        }
                    }              
                    // Пустое место для alignment right
                    Item {
                        Layout.fillWidth: true
                    }
                    ToolButton {
                        text: "⭐"
                        font.pixelSize: 16
                        onClicked: accountsManager.selectedAccountIndex = model.id
                        visible: !delegateRoot.isActive
                        ToolTip.text: "Сделать основным"
                        ToolTip.visible: hovered
                    }
                    ToolButton {
                        text: "⋮"
                        onClicked: menu.open()
                        Layout.alignment: Qt.AlignRight
                        Menu {
                            id: menu
                            MenuItem {
                                text: "Изменить"
                                onTriggered: root.openEditor(model)
                            }
                            MenuItem {
                                text: "Удалить"
                                onClicked: accountsManager.removeAccount(model.id)
                            }
                        }
                    }
                }
                // Дополнительная информация
                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: "#f1f2f6"
                    visible: delegateRoot.isExpanded
                }

                GridLayout {
                    columns: 4
                    Layout.fillWidth: true
                    Layout.margins: 15
                    columnSpacing: 10
                    rowSpacing: 5
                    visible: delegateRoot.isExpanded
                    opacity: delegateRoot.isExpanded ? 1.0 : 0.0
                    Behavior on opacity { NumberAnimation { duration: 200 } }

                    Label { text: "Сервер:"; color: theme.textSecondary; font.pixelSize: 11 }
                    Label { text: model.domain; font.family: "Monospace"; Layout.fillWidth: true }

                    Label { text: "Порт:"; color: theme.textSecondary; font.pixelSize: 11 }
                    Label { text: model.port; font.pixelSize: 11 }

                    Label { text: "Протокол:"; color: theme.textSecondary; font.pixelSize: 11 }
                    Label { text: model.protocol; font.bold: true; font.pixelSize: 11 }
                }
            }

            onClicked: {
                if (isExpanded)
                    accountsList.expandedAccountId = -1
                else
                    accountsList.expandedAccountId = model.id
            }

            background: Rectangle {
                color: "white"
                radius: 12
                border.color: delegateRoot.isActive ? theme.accent : "#e2e8f0"
                border.width: delegateRoot.isActive ? 2 : 1

                // Индикатор активного аккаунта
                Rectangle {
                    width: 6;
                    height: delegateRoot.isActive ? parent.height - 20 : 0
                    radius: 3
                    color: theme.accent
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    anchors.verticalCenter: parent.verticalCenter

                    Behavior on height { NumberAnimation { duration: 200; easing.type: Easing.OutBack } }
                }
            }
        }
    }
    Dialog {
        id: accountDialog
        anchors.centerIn: parent
        width: Math.min(parent.width * 0.9, 450)
        modal: true
        property int editingIndex: -1

        header: Rectangle {
            implicitHeight: 70
            color: "white"
            radius: 20
            Column {
                anchors.centerIn: parent
                spacing: 4
                Text {
                    text: accountDialog.title
                    font.pixelSize: 20
                    font.bold: true
                    color: theme.textPrimary
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Rectangle {
                    width: 40; height: 3
                    color: theme.accent
                    radius: 1.5
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
        background: Rectangle {
            color: "white"
            radius: 20
        }
        contentItem: Flickable {
            implicitHeight: contentColumn.height + 40
            contentHeight: contentColumn.height
            clip: true
            interactive: false // Отключаем скролл, если всё влезает

            ColumnLayout {
                id: contentColumn
                width: parent.width
                spacing: 15
                Layout.margins: 25

                // Основные поля
                DialogTextField {
                    id: displayNameField
                    placeholderText: "Название аккаунта"
                    Layout.fillWidth: true
                }

                DialogTextField {
                    id: usernameField
                    placeholderText: "Номер или Логин"
                    Layout.fillWidth: true
                }

                DialogTextField {
                    id: passwordField
                    placeholderText: "Пароль"
                    echoMode: TextInput.PasswordEchoOnEdit
                    Layout.fillWidth: true
                }

                // Группа технических настроек
                RowLayout {
                    spacing: 10
                    Layout.fillWidth: true

                    DialogTextField {
                        id: domainField
                        placeholderText: "Сервер"
                        Layout.fillWidth: true
                    }

                    DialogTextField {
                        id: portField
                        placeholderText: "Порт"
                        inputMethodHints: Qt.ImhDialableCharactersOnly
                        Layout.preferredWidth: 100
                    }

                    ComboBox {
                        id: protocolField
                        model: ["UDP", "TCP", "TLS"]
                        Layout.preferredHeight: 45
                        Layout.preferredWidth: 100
                        background: Rectangle {
                            radius: 10
                            color: "#f8f9fa"
                            border.color: "#e2e8f0"
                        }
                    }
                }
            }
        }
        footer: RowLayout {
            Layout.fillWidth: true
            spacing: 15
            Layout.margins: 25
            Layout.topMargin: 0
            Layout.bottomMargin: 20

            Button {
                text: "Отмена"
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                flat: true
                onClicked: accountDialog.reject()

                background: Rectangle { color: "transparent" }
                contentItem: Text {
                    text: parent.text
                    color: theme.textSecondary
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                id: accSaveBtn
                text: "Сохранить"
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                onClicked: accountDialog.accept()

                background: Rectangle {
                    color: accSaveBtn.pressed ? Qt.darker(theme.accent, 1.1) : theme.accent
                    radius: 10
                }
                contentItem: Text {
                    text: accSaveBtn.text
                    color: "white"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 300 }
            NumberAnimation { property: "scale"; from: 0.9; to: 1.0; duration: 300; easing.type: Easing.OutBack }
        }

        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 200 }
            NumberAnimation { property: "scale"; from: 1.0; to: 0.9; duration: 200 }
        }
        onAccepted: {
            accountsManager.saveAccount(editingIndex,
                                      displayNameField.text,
                                      usernameField.text,
                                      passwordField.text,
                                      domainField.text,
                                      parseInt(portField.text),
                                      protocolField.currentText)
        }
    }    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        Button {
            id: addBtn
            Layout.fillWidth: true
            highlighted: true

            contentItem: Text {
                text: "+ Добавить новый SIP-аккаунт"
                color: "white"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            background: Rectangle {
                color: addBtn.pressed ? Qt.darker(theme.accent, 1.1) : theme.accent
                radius: 10
            }
            onClicked: root.openEditor()
        }
        ListView {
            id: accountsList
            Layout.fillHeight: true;
            Layout.fillWidth: true;
            model: accountsManager.model
            spacing: 10
            clip: true

            property int expandedAccountId: -1

            delegate: accountDelegate
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
        }
    }
}
