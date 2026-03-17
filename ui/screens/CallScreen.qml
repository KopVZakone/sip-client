import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SipClient
import "../utils/Utils.js" as Utils
Item {
    id: root
    property alias currentNumber: numberInput.editText
    readonly property bool isOngoingCall: callManager.callState === CallManager.Active ||
                                            callManager.callState === CallManager.Paused ||
                                            callManager.callState === CallManager.Ended;
    readonly property var selectedAccount : accountsManager.selectedAccount
    readonly property bool activeUserRegistered:  !!selectedAccount //преобразование в bool для проверки и на undefined, и null
                                                  && selectedAccount.regStatus === "registered";
    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        // Поле ввода номера
        ComboBox {
            id: numberInput
            Layout.fillWidth: true
            editable: true
            font.pixelSize: 22
            font.bold: true
            visible: callManager.callState === CallManager.Idle
            // будущая модель для подсказки
            model: ["100", "102", "79991234567"]
            contentItem: TextField {
                text: numberInput.editText
                font: numberInput.font
                color: theme.textPrimary
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                leftPadding: 15
                rightPadding: 0
                background: Item {}
            }
            background: Rectangle {
                implicitHeight: 45
                color: "white"
                border.color: theme.sidebarBack
                radius: 10
            }
            delegate: ItemDelegate {
                width: numberInput.width
                contentItem: Text {
                    text: modelData
                    font.pixelSize: 18
                    verticalAlignment: Text.AlignVCenter
                }
                highlighted: numberInput.highlightedIndex === index
            }
        }
        // Цифровая клавиатура
        GridLayout {
            id: dialpad
            columns: 3
            rowSpacing: 10
            columnSpacing: 10
            Layout.alignment: Qt.AlignHCenter
            visible: callManager.callState === CallManager.Idle
            Repeater {
                model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "0", "#"]
                delegate: Button {
                    id: dialButton
                    implicitWidth: 70
                    implicitHeight: 70

                    contentItem: Text {
                        text: modelData
                        font.pixelSize: 24
                        color: theme.textPrimary
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    background: Rectangle {
                        color: dialButton.pressed ? Qt.darker(theme.backgroundLight, 1.1) : "white"
                        radius: 35
                        border.color: theme.sidebarBack
                    }

                    onClicked: numberInput.editText += modelData
                }
            }
        }
        // Информация о звонке
        ColumnLayout {
            id: callInfoContainer
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            spacing: 12

            visible: callManager.callState !== CallManager.Idle

            Label {
                text: {
                    switch(callManager.callState) {
                        case CallManager.Incoming: return "ВХОДЯЩИЙ ВЫЗОВ"
                        case CallManager.Dialing:  return "НАБОР НОМЕРА..."
                        case CallManager.Active:   return "РАЗГОВОР"
                        case CallManager.Paused:   return "НА ПАУЗЕ"
                        default: return ""
                    }
                }
                font.pixelSize: 24
                font.letterSpacing: 1.5
                font.bold: true
                color: callManager.callState === CallManager.Incoming ? "#3498db" : "#95a5a6"
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: callManager.remoteCallerNumber
                font.pixelSize: 42
                font.weight: Font.Light
                color: "#2c3e50"
                Layout.alignment: Qt.AlignHCenter

                Behavior on text {
                    SequentialAnimation {
                        NumberAnimation { target: parent; property: "opacity"; to: 0; duration: 100 }
                        PropertyAction { }
                        NumberAnimation { target: parent; property: "opacity"; to: 1; duration: 200 }
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                implicitWidth: timerLabel.implicitWidth + 30
                implicitHeight: 34
                radius: 17
                color: "#f8f9fa"
                visible: root.isOngoingCall || callManager.callState === CallManager.Paused

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 6

                    Label {
                        id: timerLabel
                        text: Utils.formatTime(callManager.callDuration)
                        font.family: "Monospace"
                        font.pixelSize: 18
                        font.weight: Font.Medium
                        color: callManager.callState === CallManager.Paused ? "#e67e22" : "#2c3e50"
                    }
                }
            }
        }
        // Кнопки управления
        RowLayout {
            Layout.alignment: Qt.AlignCenter
            spacing: 20
            component ControlButton : Button {
                id: btn
                property color baseColor: "#2ecc71"

                Layout.fillWidth: true
                implicitHeight: 60

                background: Rectangle {
                    radius: 30
                    color: btn.enabled ? (btn.pressed ? Qt.darker(baseColor, 1.2) :
                                         (btn.hovered ? Qt.lighter(baseColor, 1.1) : baseColor))
                                       : "#bdc3c7"

                    Behavior on color { ColorAnimation { duration: 100 } }
                }
                contentItem: Text {
                    text: btn.text
                    font.bold: true
                    font.pixelSize: 16
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            // Исходящий вызов
            ControlButton {
                text: "ПОЗВОНИТЬ"
                baseColor: "#27ae60"
                visible: callManager.callState === CallManager.Idle
                enabled: (numberInput.editText !== "") && root.activeUserRegistered
                onClicked: callManager.makeCall(numberInput.editText)
            }
            ControlButton {
                text: "ОТМЕНА"
                baseColor: "#e74c3c"
                visible: callManager.callState === CallManager.Dialing
                onClicked: callManager.abortDialingCall()
            }
            // Активный вызов
            ControlButton {
                text: "ПРИНЯТЬ"
                baseColor: "#2ecc71"
                visible: callManager.callState === CallManager.Incoming
                onClicked: callManager.acceptIncomingCall()
            }
            ControlButton {
                text: "ОТКЛОНИТЬ"
                baseColor: "#e74c3c"
                visible: callManager.callState === CallManager.Incoming
                onClicked: callManager.declineIncomingCall()
            }
            ControlButton {
                id: pauseBtn
                text: checked ? "ПРОДОЛЖИТЬ" : "ПАУЗА"
                baseColor: checked ? "#f39c12" : "#3498db"
                visible: root.isOngoingCall
                checkable: true
                onClicked: {
                    if (!checked) callManager.resumeCall()
                    else callManager.pauseCall()
                }
            }
            ControlButton {
                text: "СБРОС"
                baseColor: "#e74c3c"
                visible: root.isOngoingCall
                onClicked: callManager.hangupCall()
            }
        }
        // Громкость и mute
        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 120
            color: theme.sidebarBack
            radius: 15
            Layout.topMargin: 10

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                // Слайдер динамика
                RowLayout {
                    Text { text: "🔊"; font.pixelSize: 18; Layout.preferredWidth: 25 }
                    Slider {
                        id: spkSlider
                        Layout.fillWidth: true
                        from: 0; to: 100;
                        value: audioManager.outputVolume
                        onMoved: audioManager.outputVolume = value
                    }
                    Button {
                        id: muteOutputBtn
                        checkable: true
                        checked: audioManager.outputMuted
                        onClicked: audioManager.outputMuted = checked

                        contentItem: Text {
                            text: muteOutputBtn.checked ? "MUTED" : "MUTE"
                            color: muteOutputBtn.checked ? "white" : theme.accent
                            font.bold: true
                            font.pixelSize: 10
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                        }

                        background: Rectangle {
                            implicitWidth: 60
                            implicitHeight: 30
                            radius: 15
                            color: muteOutputBtn.checked ? "#e74c3c" : "transparent"
                            border.color: muteOutputBtn.checked ? "#e74c3c" : theme.accent
                            border.width: 1
                        }
                    }
                }

                // Слайдер микрофона
                RowLayout {
                    Text { text: "🎤"; font.pixelSize: 18; Layout.preferredWidth: 25 }
                    Slider {
                        id: micSlider
                        Layout.fillWidth: true
                        from: 0; to: 100;
                        value: audioManager.inputVolume
                        onMoved: audioManager.inputVolume = value
                    }
                    Button {
                        id: muteInputBtn
                        checkable: true
                        checked: audioManager.inputMuted
                        onClicked: audioManager.inputMuted = checked

                        contentItem: Text {
                            text: muteInputBtn.checked ? "MUTED" : "MUTE"
                            color: muteInputBtn.checked ? "white" : theme.accent
                            font.bold: true
                            font.pixelSize: 10
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                        }

                        background: Rectangle {
                            implicitWidth: 60
                            implicitHeight: 30
                            radius: 15
                            color: muteInputBtn.checked ? "#e74c3c" : "transparent"
                            border.color: muteInputBtn.checked ? "#e74c3c" : theme.accent
                            border.width: 1
                        }
                    }
                }
            }
        }
    }
    // Debug панель
    // GroupBox {
    //     title: "Debug"
    //     anchors.top: parent.top
    //     anchors.right: parent.right
    //     RowLayout {
    //         spacing: 10
    //         Label {
    //             text: callManager.callState
    //         }
    //     }
    // }
}
