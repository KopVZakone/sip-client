import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SipClient
import "../components"
Item {
    id: root

    // сигнал для навигации на экран звонка
    signal contactCalled(string number)


    function openEditor(contData = null) {
        if (contData) {
            contactDialog.editingId = contData.id
            contactDialog.title = "Редактировать контакт"
            nameField.text = contData.name
            phoneField.text = contData.phone
            infoField.text = contData.info
        } else {
            contactDialog.editingId = -1
            contactDialog.title = "Новый контакт"
            nameField.clear()
            phoneField.clear()
            infoField.clear()
        }
        contactDialog.open()
    }

    // Элемент списка
    Component {
        id: contactDelegate
        ItemDelegate {
            id: delegateRoot
            width: contactsView.width

            contentItem: RowLayout {
                spacing: 15
                // Информация
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 0
                    spacing: 0

                    // Имя
                    Label {
                        text: model.name
                        font.bold: true
                        font.pixelSize: 15
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                    // Номер
                    Label {
                        text: model.phone
                        color: "#666666"
                        font.pixelSize: 13
                    }
                    // Доп. информация
                    Label {
                        text: model.info
                        color: "#999999"
                        font.pixelSize: 11
                        font.italic: true
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                        maximumLineCount: 1
                    }
                }                
                // Кнопка вызова
                ToolButton {
                    text: "📞"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 40
                    focusPolicy: Qt.NoFocus

                    onClicked: contactCalled(model.phone)
                }
                ToolButton {
                    text: "⋮"
                    onClicked: menu.open()

                    Menu {
                        id: menu
                        MenuItem {
                            text: "Изменить"
                            onTriggered: root.openEditor(model)
                        }
                        MenuItem {
                            text: "Удалить"
                            onTriggered: accountsManager.contactsModel.removeContact(model.id)
                        }
                    }
                }
            }
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "#F0F0F0"
            }
        }
    }

    Dialog {
        id: contactDialog
        anchors.centerIn: parent
        width: Math.min(parent.width * 0.9, 400)
        modal: true

        property int editingId: -1
        header: Rectangle {
            implicitHeight: 70
            color: "white"
            radius: 20
            Column {
                anchors.centerIn: parent
                spacing: 4
                Text {
                    text: contactDialog.title
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
        contentItem: ColumnLayout {
            spacing: 20
            Layout.margins: 25

            DialogTextField {
                id: nameField
                placeholderText: "Имя контакта"
                Layout.fillWidth: true
            }

            DialogTextField {
                id: phoneField
                placeholderText: "Номер"
                inputMethodHints: Qt.ImhDialableCharactersOnly
                Layout.fillWidth: true
            }

            TextArea {
                id: infoField
                placeholderText: "Заметки..."
                Layout.fillWidth: true
                Layout.preferredHeight: 150
                font.pixelSize: 14
                wrapMode: Text.Wrap
                padding: 12

                background: Rectangle {
                    radius: 10
                    color: infoField.activeFocus ? "white" : "#f8f9fa"
                    border.color: infoField.activeFocus ? theme.accent : "#e2e8f0"
                    border.width: infoField.activeFocus ? 2 : 1
                }
            }
        }
        footer: RowLayout {
            Layout.fillWidth: true
            height:50
            spacing: 15
            Layout.margins: 25
            Layout.topMargin: 0
            Layout.bottomMargin: 20

            Button {
                text: "Отмена"
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                flat: true
                onClicked: contactDialog.reject()
                background: Rectangle {
                    color: "transparent"
                    radius: 10
                }
                contentItem: Text {
                    text: parent.text
                    color: theme.textSecondary
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                id: saveBtn
                text: "Сохранить"
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                onClicked: contactDialog.accept()

                background: Rectangle {
                    color: saveBtn.pressed ? Qt.darker(theme.accent, 1.1) : theme.accent
                    radius: 10
                }

                contentItem: Text {
                    text: saveBtn.text
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
            accountsManager.contactsModel.saveContact(
                editingId,
                nameField.text,
                phoneField.text,
                infoField.text
            )
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
                text: "+ Добавить новый контакт"
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
            id: contactsView
            Layout.fillHeight: true;
            Layout.fillWidth: true;
            model: accountsManager.contactsModel
            delegate: contactDelegate
            spacing: 10
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            clip: true
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
        }
    }
}
