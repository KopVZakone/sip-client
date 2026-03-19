import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import SipClient
import "../utils"
Rectangle {
    id: root
    height: 50
    color: "#ffffff"
    radius: 10

    border.color: "#e2e8f0"
    border.width: 1

    property bool show: true
    visible: show && callManager.callState !== CallManager.Idle

    readonly property bool isOngoingCall: callManager.callState === CallManager.Active ||
                                            callManager.callState === CallManager.Paused ||
                                            callManager.callState === CallManager.Ended;
    readonly property var selectedAccount : accountsManager.selectedAccount
    readonly property bool activeUserRegistered:  !!selectedAccount
                                                  && selectedAccount.regStatus === "registered";
    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 10
        spacing: 15

        // Индикатор статуса
        Rectangle {
            width: 10; height: 10; radius: 5
            color: callManager.callState === CallManager.Active ? "#2ecc71" :
                   (callManager.callState === CallManager.Paused ? "#f39c12" : "#3498db")

            SequentialAnimation on opacity {
                loops: Animation.Infinite
                NumberAnimation { from: 1.0; to: 0.4; duration: 600 }
                NumberAnimation { from: 0.4; to: 1.0; duration: 600 }
            }
        }

        // Информация об абоненте и таймер
        ColumnLayout {
            spacing: 0
            Layout.fillWidth: true

            Label {
                text: callManager.remoteCallerNumber
                font.bold: true
                font.pixelSize: 14
                elide: Text.ElideRight
            }

            Label {
                text: callManager.callState === CallManager.Paused ? "На паузе" : Utils.formatTime(callManager.callDuration)
                font.pixelSize: 12
                font.family: "Monospace"
                color: "#64748b"
            }
        }

        // Кнопки управления
        RowLayout {
            spacing: 8
            Layout.alignment: Qt.AlignRight

            ToolButton {
                visible: root.isOngoingCall
                text: callManager.callState === CallManager.Paused ? "▶" : "⏸"
                onClicked: {
                    if (callManager.callState === CallManager.Paused) callManager.resumeCall()
                    else callManager.pauseCall()
                }

                background: Rectangle {
                    implicitWidth: 36; implicitHeight: 36; radius: 18
                    color: parent.pressed ? "#f1f5f9" : "transparent"
                }
            }


            Button {
                id: actionBtn
                implicitWidth: 40; implicitHeight: 40

                background: Rectangle {
                    radius: 20
                    color: {
                        if (callManager.callState === CallManager.Incoming) return "#2ecc71" // Зеленый принять
                        return "#e74c3c" // Красный сброс/отмена
                    }
                }

                contentItem: Text {
                    text: callManager.callState === CallManager.Incoming ? "✔" : "📞"
                    color: "white"
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    rotation: callManager.callState === CallManager.Incoming ? 0 : 135 // Поворот трубки для сброса
                }

                onClicked: {
                    if (callManager.callState === CallManager.Incoming) callManager.acceptIncomingCall()
                    else if (callManager.callState === CallManager.Dialing) callManager.abortDialingCall()
                    else callManager.hangupCall()
                }
            }
            Button {
                id: declineBtn
                implicitWidth: 40; implicitHeight: 40
                visible: callManager.callState === CallManager.Incoming
                background: Rectangle {
                    radius: 20
                    color:  "#e74c3c"
                }

                contentItem: Text {
                    text: "x"
                    color: "white"
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                }

                onClicked: callManager.declineIncomingCall()
            }
        }
    }

    Behavior on opacity { NumberAnimation { duration: 200 } }
}
