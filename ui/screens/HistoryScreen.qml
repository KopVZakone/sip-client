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
            width: historyList.width
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
                        text: "От: " + model.fromNumber
                        font.bold: true
                        elide: Text.ElideRight
                    }
                    Label {
                        text: "Кому: " + model.toNumber
                        elide: Text.ElideRight
                    }
                }

                // Дата и время
                Label {
                    Layout.preferredWidth: 80
                    text: model.timestamp
                    Layout.alignment: Qt.AlignRight
                    horizontalAlignment: Text.AlignRight
                }

                // Длительность
                Label {
                    Layout.preferredWidth: 60
                    Layout.alignment: Qt.AlignRight
                    text: Utils.formatTime(model.duration)
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
            onTextChanged: callManager.proxy.filterText = text
        }

        // Кнопки фильтрации
        Row {
            Button {
                text: "Все"
                onClicked: callManager.proxy.filterStatus = ""
            }
            Button {
                text: "Пропущенные"
                onClicked: callManager.proxy.filterStatus = "Пропущен"
            }
            Button {
                text: "Очистить историю"
                onClicked: callManager.model.clearHistory()
            }
        }
        ListView {
            id: historyList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: callManager.proxy
            delegate: callDelegate
        }
    }
}
