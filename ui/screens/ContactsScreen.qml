import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SipClient
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
                ToolButton {
                    text: "🔧"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 40
                    onClicked: root.openEditor(model)
                }

                ToolButton {
                    text: "❌"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 40
                    onClicked: accountsManager.contactsModel.removeContact(model.id)
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
        width: parent.width * 0.9
        modal: true
        standardButtons: Dialog.Save | Dialog.Cancel

        property int editingId: -1

        ColumnLayout {
            width: parent.width
            spacing: 10

            TextField {
                id: nameField
                placeholderText: "Имя контакта"
                Layout.fillWidth: true
            }
            TextField {
                id: phoneField
                placeholderText: "Номер телефона"
                inputMethodHints: Qt.ImhDialableCharactersOnly
                Layout.fillWidth: true
            }
            TextArea {
                id: infoField
                placeholderText: "Дополнительная информация"
                Layout.fillWidth: true
                implicitHeight: 80
            }
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
            text: "+ Добавить контакт"
            Layout.fillWidth: true
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
            focus: true
        }
    }
}
