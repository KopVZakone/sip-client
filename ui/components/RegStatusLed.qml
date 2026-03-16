import QtQuick 2.15

Rectangle {
    id: statusLed
    property string status: "offline"
    property real size: 10

    width: size
    height: size
    radius: size / 2

    readonly property color colorRegistered: "#4CAF50"
    readonly property color colorRegistering: "#FFC107"
    readonly property color colorError: "#F44336"
    readonly property color colorOffline: "#BDBDBD"

    color: {
        switch (status) {
            case "registered":  return colorRegistered
            case "registering": return colorRegistering
            case "error":       return colorError
            default:            return colorOffline
        }
    }

    // Анимация при попытке подключения
    SequentialAnimation {
        running: status === "registering"
        alwaysRunToEnd: true
        loops: Animation.Infinite

        OpacityAnimator {
            target: statusLed
            from: 1
            to: 0.3
            duration: 1000
        }
        OpacityAnimator {
            target: statusLed
            from: 0.3
            to: 1
            duration: 1000
        }
    }
}
