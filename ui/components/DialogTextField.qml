import QtQuick 2.15
import QtQuick.Controls
TextField {
    id: field
    font.pixelSize: 15
    leftPadding: 15
    selectByMouse: true
    background: Rectangle {
        implicitHeight: 45
        radius: 10
        color: field.activeFocus ? "white" : "#f8f9fa"
        border.color: field.activeFocus ? theme.accent : "#e2e8f0"
        border.width: field.activeFocus ? 2 : 1
        Behavior on color { ColorAnimation { duration: 300 } }
    }
}
