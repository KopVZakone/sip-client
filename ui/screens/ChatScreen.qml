import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SipClient
import "../components"
import "../utils"
Item{
    id: root
    readonly property var selectedAccount : accountsManager.selectedAccount
    readonly property bool activeUserRegistered:  !!selectedAccount
                                                  && selectedAccount.regStatus === "registered";
    Connections {
        target: chatManager.currentChat

        function onFilterNumberChanged() {
            messageInput.text = ""
        }
    }
    onVisibleChanged: {
        if (!visible)
        {
            chatManager.closeChat()
        }
    }
    ColumnLayout {
        id: chatPage
        anchors.fill: parent
        spacing: 0
        // --- 1. ШАПКА (С кем переписка) ---
        Rectangle {
            Layout.fillWidth: true
            height: 65
            color: "white"

            Rectangle { height: 1; color: "#e2e8f0"; anchors.bottom: parent.bottom; width: parent.width }

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 15
                Column {
                    Layout.fillWidth: true
                    Label { text: chatManager.currentChat.filterNumber; font.bold: true; font.pixelSize: 16 }
                    // Label { text: "в сети"; font.pixelSize: 12; color: "#2ecc71" }
                }
            }
        }

        ListView {
            id: messageListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 12

            onCountChanged: scrollTimer.restart()
            Timer { id: scrollTimer; interval: 50; onTriggered: messageListView.positionViewAtEnd() }

            model: chatManager.currentChat
            //     ListModel {
            //     id: mockMessageModel
            //     ListElement { text: "Привет! Как дела с SIP-клиентом?"; isOutgoing: false; time: "10:01" }
            //     ListElement { text: "Почти закончил экран чата, добавляю анимации."; isOutgoing: true; time: "10:05" }
            //     ListElement { text: "Круто! А базу данных уже подключил?"; isOutgoing: false; time: "10:06" }
            //     ListElement { text: "Да, использую SQLite и группировку по номеру."; isOutgoing: true; time: "10:07" }
            // }

            delegate: Item {
                width: messageListView.width - 40
                height: bubble.height + 10

                Rectangle {
                    id: bubble
                    // Свои — справа, чужие — слева
                    anchors.right: model.isOutgoing ? parent.right : undefined
                    anchors.left: model.isOutgoing ? undefined : parent.left

                    width: Math.min(msgText.implicitWidth + 50, parent.width * 0.8)
                    height: msgText.implicitHeight + 24
                    radius: 16

                    // Стиль баббла
                    color: model.isOutgoing ? theme.accent : "#f1f3f5"

                    Text {
                        id: msgText
                        text: model.text
                        width: parent.width - 24
                        anchors.centerIn: parent
                        wrapMode: Text.Wrap
                        color: model.isOutgoing ? "white" : "#1a1a1a"
                        font.pixelSize: 14
                    }

                    Label {
                        text: Utils.formatTimestamp(model.timestamp)
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.margins: 5
                        font.pixelSize: 9
                        color: model.isOutgoing ? "#e0e0e0" : "#868e96"
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 70
            color: "white"

            Rectangle { height: 1; color: "#e2e8f0"; anchors.top: parent.top; width: parent.width }

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                TextField {
                    id: messageInput
                    Layout.fillWidth: true
                    placeholderText: "Напишите сообщение..."
                    padding: 12
                    background: Rectangle {
                        radius: 20
                        color: "#f8f9fa"
                        border.color: messageInput.activeFocus ? theme.accent : "#dee2e6"
                    }

                    Keys.onReturnPressed: sendBtn.clicked()
                }

                Button {
                    id: sendBtn
                    text: "➤"
                    Layout.preferredWidth: 45
                    Layout.preferredHeight: 45
                    enabled: root.activeUserRegistered && messageInput.text !== ""
                    background: Rectangle {
                        radius: 22.5
                        color: sendBtn.enabled ? theme.accent : "#adb5bd"
                    }

                    contentItem: Text {
                        text: parent.text; color: "white"; font.pixelSize: 18
                        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        chatManager.sendMessageInCurrentChat(messageInput.text)
                        messageInput.text = ""
                    }
                }
            }
        }
    }
}
