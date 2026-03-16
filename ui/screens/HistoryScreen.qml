import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SipClient
import "../utils/Utils.js" as Utils
Item {
    // Элемент списка
    Component {
        id: callDelegate

        ItemDelegate {
            height: 70
            width: historyList.width
            padding: 10
            contentItem: Rectangle {
                radius: 10
                // Чередование цветов для "зебры"
                color: index % 2 === 0 ? "white" : "#edf2f7"
                border.color: "#e2e8f0"
                RowLayout {
                    anchors.fill: parent
                    spacing: 15

                    // Статус
                    Text {
                        Layout.preferredWidth: 80
                        text: model.status
                    }
                    // Номера
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2

                        Label {
                            text: "От: " + model.fromNumber
                            font.bold: true
                            elide: Text.ElideRight
                        }
                        Label {
                            text: "Кому: " + model.toNumber
                            elide: Text.ElideRight
                        }
                    }

                    ColumnLayout {
                        Layout.preferredWidth: 60
                        spacing: 2
                        Layout.alignment: Qt.AlignRight

                        Label {
                            text: Utils.formatTimestamp(model.timestamp)
                            font.pixelSize: 11
                            Layout.alignment: Qt.AlignRight
                            color: theme.textSecondary
                        }

                        Label {
                            text: Utils.formatTime(model.duration)
                            font.family: "Monospace"
                            font.pixelSize: 12
                            font.bold: true
                            Layout.alignment: Qt.AlignRight
                            color: theme.accent
                        }
                    }
                }
            }
        }
    }
    ColumnLayout {
        anchors.fill: parent
        // Ввод номера для поиска
        TextField {
            id: searchField
            placeholderText: "Поиск по номеру..."
            Layout.fillWidth: true

            font.pixelSize: 14

            background: Rectangle {
                implicitHeight: 40
                radius: 20
                color: "white"
                border.color: searchField.activeFocus ? theme.accent : "#dcdde1"
                border.width: searchField.activeFocus ? 2 : 1
            }
            onTextChanged: callManager.proxy.filterText = text

        }

        // Кнопки фильтрации
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            component FilterButton : Button {
                property bool active: false
                id: control
                flat: true

                contentItem: Text {
                    text: control.text
                    font.bold: true
                    font.pixelSize: 12
                    color: control.active ? "white" : theme.textSecondary
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 32
                    radius: 16
                    color: control.active ? theme.accent : "#e0e0e0"

                    Behavior on color { ColorAnimation { duration: 150 } }
                }
            }

            FilterButton {
                text: "ВСЕ"
                active: callManager.proxy.filterStatus === ""
                onClicked: callManager.proxy.filterStatus = ""
            }

            FilterButton {
                text: "ПРОПУЩЕННЫЕ"
                active: callManager.proxy.filterStatus === "Пропущен"
                onClicked: callManager.proxy.filterStatus = "Пропущен"
            }

            // Разделитель
            Item { Layout.fillWidth: true }

            // Кнопка очистки (отличается по стилю)
            Button {
                id: clearBtn
                flat: true
                contentItem: Text {
                    text: "Очистить историю"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: clearBtn.pressed ? Qt.darker("#e74c3c", 1.2) : "#e74c3c"
                }

                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 32
                    radius: 16
                    color: clearBtn.pressed ? "#c0392b" :
                           (clearBtn.hovered ? "#fdecea" : "transparent")

                    border.color: "#e74c3c"
                    border.width: 1
                }
                onClicked: callManager.model.clearHistory()
            }
        }
        ListView {
            id: historyList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: callManager.proxy
            delegate: callDelegate

            clip: true

            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar { }
        }
    }
}
