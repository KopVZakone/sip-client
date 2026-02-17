import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Utils.js" as Utils
Item {
    // Элемент списка
    Component {
        id: callDelegate

        ItemDelegate {
            width: parent.width
            height: 70
            padding: 10

            contentItem: RowLayout {
                anchors.fill: parent
                spacing: 15

                // Статус
                // TODO: заменить на иконку
                Text {
                    Layout.preferredWidth: 80
                    text: model.status
                }
                // Номера
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2

                    Label {
                        text: "От: " + model.from_number
                        font.bold: true
                        elide: Text.ElideRight
                    }
                    Label {
                        text: "Кому: " + model.to_number
                        elide: Text.ElideRight
                    }
                }

                // Дата и время
                Label {
                    Layout.preferredWidth: 80
                    text: model.date
                    Layout.alignment: Qt.AlignRight
                    horizontalAlignment: Text.AlignRight
                }

                // Длительность
                Label {
                    Layout.preferredWidth: 60
                    Layout.alignment: Qt.AlignRight
                    text: Utils.formatTime(model.durationSeconds)
                    font.family: "Monospace"
                    horizontalAlignment: Text.AlignRight
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
    ColumnLayout {
        anchors.fill: parent
        // Ввод номера для поиска
        TextField {
            id: searchField
            placeholderText: "Поиск по номеру..."
            Layout.fillWidth: true
        }

        // Кнопки фильтрации
        Row {
            Button { text: "Все"; }
            Button { text: "Пропущенные"; }
        }
        ListModel {
            id: historyModel

            ListElement {
                from_number:    "84950000000"
                to_number:      "81230453000"
                date:           "12.05 14:20"
                durationSeconds:312
                status:         "incoming"
            }

            ListElement {
                from_number:    "84000"
                to_number:      "12400"
                date:           "12.05 15:45"
                durationSeconds:0
                status:         "missed"
            }

            ListElement {
                from_number:    "1249504532132312300"
                to_number:      "12495045300231231231"
                date:           "04.04 10:05"
                durationSeconds:750
                status:         "outgoing"
            }
        }
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: historyModel
            delegate: callDelegate
        }
    }
}
