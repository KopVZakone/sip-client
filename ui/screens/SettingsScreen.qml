import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import SipClient
ScrollView {
    id: settingsScroll
    Layout.fillWidth: true
    padding: 20
    contentWidth: availableWidth
    clip: true

    // Компонент заголовка секции меню
    component SettingsHeader : Label {
        property alias text: headerLabel.text
        width: parent.width
        height: 40
        verticalAlignment: Text.AlignBottom
        leftPadding: 15
        bottomPadding: 8
        font.bold: true
        font.pixelSize: 18
        color: "#2196F3"
        id: headerLabel
    }
    component SettingsGroup : ColumnLayout {
            default property alias content: innerContent.data
            spacing: 0
            Layout.fillWidth: true

            Rectangle {
                Layout.fillWidth: true
                implicitHeight: innerContent.implicitHeight + 20
                color: "white"
                radius: 12
                border.color: "#e2e8f0"

                ColumnLayout {
                    id: innerContent
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10
                }
            }
        }
    // Компонент элемента списка настроек
    // Название слева, переключатель справа
    component SettingsItem: RowLayout {
        id: root
        // Название настройки
        property alias text: label.text

        // Переключатель настройки
        default property alias content: container.data

        Layout.fillWidth: true
        spacing: 10

        Label {
            id: label
            Layout.fillWidth: true
            color: theme.textPrimary
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        Row {
            id: container
            spacing: 5
            layoutDirection: Qt.RightToLeft
        }
    }

    ColumnLayout {
        width: settingsScroll.availableWidth
        spacing: 10


        SettingsHeader { text: "Аудио" }
        SettingsGroup {
            SettingsItem {
                text: "Микрофон"
                ComboBox {
                    model: audioManager.inputDevices
                    implicitWidth: 200
                    height:30
                    currentIndex: model.indexOf(audioManager.inputDevice)
                    onActivated: audioManager.inputDevice = currentText
                    background: Rectangle { radius: 8; color: "#f8f9fa"; border.color: "#dcdde1" }
                }
            }
            MenuSeparator { Layout.fillWidth: true }
            SettingsItem {
                text: "Динамики"
                ComboBox {
                    model: audioManager.outputDevices
                    implicitWidth: 200
                    height:30
                    currentIndex: model.indexOf(audioManager.outputDevice)
                    onActivated: audioManager.outputDevice = currentText
                    background: Rectangle { radius: 8; color: "#f8f9fa"; border.color: "#dcdde1" }
                }
            }
        }
        SettingsHeader { text: "Система" }
        SettingsGroup {
            SettingsItem {
                text: "Версия приложения"
                Label {
                    Layout.alignment: Qt.AlignRight
                    text: "v0.1.0"
                    color: "gray"
                }
            }
        }
    }
}
