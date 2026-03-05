import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SipClient
import "../utils/Utils.js" as Utils
Item {
    id: root
    property alias currentNumber: numberInput.text
    readonly property bool isOngoingCall: callManager.callState === CallManager.Active ||
                                            callManager.callState === CallManager.Paused ||
                                            callManager.callState === CallManager.Ended;
    readonly property bool activeUserRegistered: accountsManager.activeUsername !== ""
    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        //Информация о пользователе
        Label {
            text: !root.activeUserRegistered ? "Активный аккаунт не зарегестрирован"
                                                        : "Активный аккаунт: " + accountsManager.activeUsername
            visible: callManager.callState === CallManager.Idle
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        // Поле ввода номера
        TextField {
            id: numberInput
            placeholderText: "Введите номер..."
            visible: callManager.callState === CallManager.Idle
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }
        // Информация о звонке
        ColumnLayout {
            Layout.alignment: Qt.AlignCenter
            spacing: 10

            Label {
                text: callManager.callState === CallManager.Idle ? "Готов к вызову" :
                      callManager.callState === CallManager.Incoming ? "Входящий звонок" : "Разговор"
                color: "gray"
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: callManager.callState === CallManager.Idle ? "": callManager.remoteCallerNumber
                visible: text !== ""
                font.bold: true; font.pixelSize: 32
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: Utils.formatTime(callManager.callDuration)
                visible: root.isOngoingCall
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
                visible: callManager.callState === CallManager.Idle
                enabled: (numberInput.text !== "") && root.activeUserRegistered
                onClicked: callManager.makeCall(numberInput.text)
                highlighted: true
            }
            Button {
                text: "Отмена"
                visible: callManager.callState === CallManager.Dialing
                palette.button: "#e74c3c";
                onClicked: callManager.abortDialingCall()
                highlighted: true
            }
            // Активный вызов
            Button {
                text: "Пауза"
                visible: root.isOngoingCall
                checkable: true
                onClicked: {
                    if (!checked) {
                        callManager.resumeCall()
                    } else {
                        callManager.pauseCall()
                    }
                }
            }
            Button {
                text: "Сброс"
                visible: root.isOngoingCall
                palette.button: "#e74c3c";
                onClicked: callManager.hangupCall()
            }
            // Входящий вызов
            Button {
                text: "Принять"
                visible: callManager.callState === CallManager.Incoming
                onClicked: callManager.acceptIncomingCall()
            }
            Button {
                text: "Отклонить"
                visible: callManager.callState === CallManager.Incoming
                onClicked: callManager.declineIncomingCall()
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
                text: callManager.callState
            }
        }
    }
}
