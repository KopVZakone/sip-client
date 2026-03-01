import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../utils/Utils.js" as Utils
Item {
    id: root
    property string callState: "idle"
    property int callDurationSeconds: 100
    property string remoteNumber: "79991234567"


    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        // Поле ввода номера
        TextField {
            id: numberInput
            placeholderText: "Введите номер..."
            visible: root.callState === "idle"
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }
        // Информация о звонке
        ColumnLayout {
            Layout.alignment: Qt.AlignCenter
            spacing: 10

            Label {
                text: root.callState === "idle" ? "Готов к вызову" :
                      root.callState === "incoming" ? "Входящий звонок" : "Разговор"
                color: "gray"
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: root.callState === "idle" ? "": root.remoteNumber
                visible: text !== ""
                font.bold: true; font.pixelSize: 32
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: Utils.formatTime(root.callDurationSeconds)
                visible: root.callState === "active"
                font.family: "Monospace"
                font.pixelSize: 20
                Layout.alignment: Qt.AlignHCenter
            }
        }
        // Кнопки управления
        RowLayout {
            Layout.alignment: Qt.AlignCenter
            spacing: 20
            // Исходящий вызов
            Button {
                text: "Вызов"
                visible: root.callState === "idle"
                onClicked: root.callState = "active"
                highlighted: true
            }
            // Активный вызов
            Button {
                text: "Пауза"
                visible: root.callState === "active"
                checkable: true
            }
            Button {
                text: "Сброс"
                visible: root.callState === "active"
                palette.button: "#e74c3c";
                onClicked: {
                    root.callState = "idle"
                }
            }
            // Входящий вызов
            Button {
                text: "Принять"
                visible: root.callState === "incoming"
                onClicked: root.callState = "active"
            }
            Button {
                text: "Отклонить"
                visible: root.callState === "incoming"
                onClicked: root.callState = "idle"
            }
        }
        // Громкость и mute
        GroupBox {
            title: "Аудио"
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent

                ColumnLayout {
                    Label {
                        text: "Микрофон"
                        font.pixelSize: 10
                    }
                    Slider {
                        id: micSlider
                        from: 0
                        to: 100
                        value: audioManager.inputVolume
                        onMoved: audioManager.inputVolume = value
                        Layout.fillWidth: true
                    }
                }
                ToolButton {
                    text: audioManager.inputMuted ? "🎤❌" : "🎤"
                    checkable: true
                    checked: audioManager.inputMuted
                    onClicked: audioManager.inputMuted = checked
                }

                Item { Layout.preferredWidth: 20 }

                ColumnLayout {
                    Label {
                        text: "Динамик"
                        font.pixelSize: 10
                    }
                    Slider {
                        id: spkSlider
                        from: 0
                        to: 100
                        value: audioManager.outputVolume
                        onMoved: audioManager.outputVolume = value
                        Layout.fillWidth: true
                    }
                }
                ToolButton {
                    text: audioManager.outputMuted ? "🔇" : "🔊"
                    checkable: true
                    checked: audioManager.outputMuted
                    onClicked: audioManager.outputMuted = checked
                }
            }
        }
    }
    // Debug панель
    GroupBox {
        title: "Debug"
        anchors.top: parent.top
        anchors.right: parent.right
        RowLayout {
            spacing: 10
            Label {
                text: root.callState
            }
            ComboBox {
                id: debugState
                model: ["idle", "active", "incoming"]
                currentIndex: 0
                onCurrentTextChanged: root.callState = currentText
            }
        }
    }
}
