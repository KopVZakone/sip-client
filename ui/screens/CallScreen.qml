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

        //Информация о пользователе
        Label {
            text: !root.activeUserRegistered ? "Активный аккаунт не зарегестрирован"
                                                        : "Активный аккаунт: " + selectedAccount.displayName
            visible: callManager.callState === CallManager.Idle
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        // Поле ввода номера
        ComboBox {
            id: numberInput
            Layout.fillWidth: true
            editable: true
            // будущая модель для подсказки
            model: ["101 (Office)", "102 (Manager)", "79991234567"]

            background: Rectangle {
                implicitHeight: 45
                color: "white"
                border.color: theme.sidebarBack
                radius: 10
            }
        }
        // Цифровая клавиатура
        GridLayout {
            id: dialpad
            columns: 3
            rowSpacing: 10
            columnSpacing: 10
            Layout.alignment: Qt.AlignHCenter

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
            Layout.alignment: Qt.AlignCenter
            spacing: 10

            Label {
                text: callManager.callState === CallManager.Idle ? "" :
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
                id: callButton
                Layout.fillWidth: true
                implicitHeight: 60

                background: Rectangle {
                    color: callButton.enabled ? "#27ae60" : "gray"
                    radius: 30
                }

                contentItem: Text {
                    text: "ПОЗВОНИТЬ"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                visible: callManager.callState === CallManager.Idle
                enabled: (numberInput.editText !== "") && root.activeUserRegistered
                onClicked: callManager.makeCall(numberInput.editText)
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
        /* GroupBox {
            title: "Аудио"
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent

                ColumnLayout {
                    Label {
                        text: "Микрофон"
                        font.pixelSize: 10
                    }
                }
                ToolButton {
                    text: audioManager.inputMuted ? "🎤❌" : "🎤"
                    checkable: true
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
                }
            }
        }*/
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
