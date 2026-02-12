import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property string callState: "idle"
    property int callDurationSeconds: 100
    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Поле ввода номера
        TextField {
            id: numberInput
            placeholderText: "Введите номер..."
            enabled: root.callState === "idle"
            Layout.fillWidth: true
        }

        // Исходящий вызов
        RowLayout {
            spacing: 10
            visible: root.callState === "idle"
            Button { text: "Вызов"; onClicked: root.callState = "active" }
        }

        // Активный вызов
        ColumnLayout {
            visible: root.callState === "active"
            spacing: 10
            RowLayout {
                spacing: 10
                Button { text: "Пауза" }
                Button {
                    text: "Сброс"
                    onClicked: {
                        root.callState = "idle"
                        callTimer.stop()
                        root.callDuration = 0
                    }
                }
            }
            Label {
                text: "Длительность звонка: " +
                      Math.floor(root.callDurationSeconds / 60) + ":" +
                      (root.callDurationSeconds % 60 < 10 ? "0" : "") +
                      (root.callDurationSeconds % 60)
                font.bold: true
            }
        }

        // Входящий вызов
        ColumnLayout {
            visible: root.callState === "incoming"
            spacing: 10
            Label { text: "Входящий звонок: " + "79991234567" }
            RowLayout {
                spacing: 10
                Button { text: "Принять"; onClicked: root.callState = "active" }
                Button { text: "Отклонить"; onClicked: root.callState = "idle" }
            }
        }

        // Громкость и mute
        GroupBox {
            title: "Аудио"
            Layout.fillWidth: true
            ColumnLayout {
                spacing: 10

                RowLayout {
                    Label { text: "Микрофон" }
                    Slider { from: 0; to: 100; value: 50 }
                    Button { text: "Mute" }
                }

                RowLayout {
                    Label { text: "Динамики" }
                    Slider { from: 0; to: 100; value: 50 }
                    Button { text: "Mute" }
                }
            }
        }
    }
    // Debug панель
    GroupBox {
        title: "Debug"
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
