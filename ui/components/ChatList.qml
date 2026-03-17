import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
ColumnLayout {
        spacing: 0

        signal chatSelected(string number)
        // Заголовок
        Label {
            text: "Чаты"
            font.bold: true
            font.pixelSize: 20
            Layout.margins: 15
        }
        // Список
        ListView {
            id: chatList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: chatManager.chatList
            clip: true
            spacing: 20

            delegate: ItemDelegate {
                id: delegateRoot
                width: chatList.width
                height: 70
                padding: 10

                property bool isSelected: model.remote_number === chatManager.currentChat.filterNumber

                background: Rectangle {
                    radius: 15
                    color: delegateRoot.isSelected ? "white" : (delegateRoot.hovered ? "#f8f9fa" : "transparent")
                    border.color: isSelected ? theme.accent : "transparent"
                    Rectangle {
                        anchors.left: parent.left
                        anchors.leftMargin: 4
                        anchors.verticalCenter: parent.verticalCenter
                        width: 4
                        height: delegateRoot.isSelected ? parent.height * 0.5 : 0
                        radius: 2
                        color: theme.accent

                        Behavior on height { NumberAnimation { duration: 200; easing.type: Easing.OutBack } }
                    }
                }

                contentItem: RowLayout {
                    spacing: 12
                    // Текстовая информация
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        RowLayout {
                            Label {
                                text: model.remote_number
                                font.bold: true; Layout.fillWidth: true
                                elide: Text.ElideRight
                            }
                            Label {
                                text: model.last_timestamp
                                font.pixelSize: 10; color: "#adb5bd"
                            }
                        }
                        Label {
                            text: model.last_message
                            font.pixelSize: 13; color: "#6c757d"
                            elide: Text.ElideRight; Layout.fillWidth: true
                            maximumLineCount: 1
                        }
                    }

                    // Индикатор непрочитанных сообщений
                    Rectangle {
                        id: badge
                        visible: model.unread_count > 0
                        width: 20; height: 20; radius: 10
                        color: "#fa5252"
                        Layout.alignment: Qt.AlignVCenter

                        Text {
                            anchors.centerIn: parent
                            text: model.unread_count
                            color: "white"; font.pixelSize: 10; font.bold: true
                        }
                    }
                }

                onClicked: {
                    let number = model.remote_number
                    chatSelected(number)
                    chatManager.openChat(number)
                }
            }
        }
        ListModel {
            id: chatListModel

            ListElement {
                remote_number: "+7 (999) 123-45-67"
                last_message: "Привет! Как там наш SIP сервер поживает?"
                last_timestamp: "14:20"
                unread_count: 3
            }

            ListElement {
                remote_number: "Alice"
                last_message: "Файл отправлен. Проверь почту."
                last_timestamp: "Вчера"
                unread_count: 0
            }

            ListElement {
                remote_number: "+44 20 7123 4567"
                last_message: "The call was dropped due to network issues. Please callback."
                last_timestamp: "10:05"
                unread_count: 1
            }

            ListElement {
                remote_number: "7001"
                last_message: "Ожидайте, ваш звонок важен для нас..."
                last_timestamp: "09:15"
                unread_count: 0
            }

            ListElement {
                remote_number: "Unknown Caller"
                last_message: "Это очень длинное сообщение, которое должно обрезаться по ширине панели, иначе оно сломает нам весь красивый дизайн левой части."
                last_timestamp: "Понедельник"
                unread_count: 12
            }
        }
    }
