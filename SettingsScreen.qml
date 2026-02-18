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
        SettingsItem {
            text: "Микрофон"
            ComboBox {
                model: settingsManager.inputDevices
                currentIndex: model.indexOf(settingsManager.getVal("input_name", ""))
                onActivated: settingsManager.setInputDeviceByName(currentText)
            }
        }
        MenuSeparator { Layout.fillWidth: true }
        SettingsItem {
            text: "Динамики"
            ComboBox {
                model: ["По умолчанию", "Realtek Audio", "Headset 2"]
            }
        }
        // WIP: другие настройки в зависимости от настроек библиотеки

        SettingsHeader { text: "Система" }
        SettingsItem {
            text: "Версия приложения"
            Label {
                Layout.alignment: Qt.AlignRight
                text: "v0.0.1"
                color: "gray"
            }
        }
    }
}
