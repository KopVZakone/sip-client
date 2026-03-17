import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
RowLayout {
    id: selectedAccountInfo
    spacing: 15
    readonly property var selectedAccount : accountsManager.selectedAccount
    // "лампочка" статуса регистрации
    RegStatusLed {
        status: selectedAccountInfo.selectedAccount ?
                    selectedAccountInfo.selectedAccount.regStatus :
                    "offline";
        Layout.preferredHeight: 20
        Layout.preferredWidth: 20
        radius: 10
        Layout.alignment: Qt.AlignVCenter
    }
    // Имя аккаунта и номер или ошибка регистрации
    ColumnLayout {
        spacing: 2
        Label {
            text: selectedAccountInfo.selectedAccount ?
                      selectedAccountInfo.selectedAccount.displayName :
                      "Нет аккаунта"
            font.bold: true; font.pixelSize: 22
            Layout.fillWidth: true
            elide: Text.ElideRight
            color: "black"
        }
        Label {
            text: selectedAccountInfo.selectedAccount  ? (
                selectedAccountInfo.selectedAccount.regStatus === "error" ?
                      "Ошибка: " + selectedAccountInfo.selectedAccount.lastError
                    : selectedAccountInfo.selectedAccount.username) : "";
            Layout.fillWidth: true
            font.pixelSize: 16
            color: selectedAccountInfo.selectedAccount && selectedAccountInfo.selectedAccount.regStatus === "error" ? "#F44336" : "#757575"
        }
    }
}
