import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Item {
    // Элемент списка
    Component {
        id: contactDelegate
        ItemDelegate {
            id: root
            width: parent.width

            contentItem: RowLayout {
                spacing: 15
                // Информация
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 0

                    // Имя
                    Label {
                        text: model.name
                        font.bold: true
                        font.pixelSize: 15
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                    // Номер
                    Label {
                        text: model.number
                        color: "#666666"
                        font.pixelSize: 13
                    }
                    // Доп. информация
                    Label {
                        text: model.info
                        color: "#999999"
                        font.pixelSize: 11
                        font.italic: true
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                        maximumLineCount: 1
                    }
                }
                // Кнопка вызова
                ToolButton {
                    text: "📞"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 40
                    focusPolicy: Qt.NoFocus

                    onClicked: {
                        // TODO: Переход на главную с подстановкой номера в поле ввода
                    }
                }
            }
        }

    }
    ListModel {
        id: contactModel
        ListElement {
            name: "Alice";
            number: "123-456"
            info: "Some info"
        }
        ListElement {
            name: "Bob"
            number: "987-654"
            info:   "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum faucibus volutpat gravida. Sed mollis tellus risus,
                    sed rhoncus ipsum volutpat eget. Sed a commodo ante, et tincidunt odio.
                    Quisque elementum quam non mi sollicitudin congue. Vestibulum ante ipsum
                    primis in faucibus orci luctus et ultrices posuere cubilia curae;
                    Phasellus in semper quam, in viverra risus. Pellentesque scelerisque
                    consectetur lectus, eu pretium erat tempus ac. Curabitur augue tellus,
                    luctus non posuere finibus, finibus vel libero. Ut lobortis, nibh a accumsan dignissim,
                    purus erat condimentum libero, in ultricies nibh nunc hendrerit leo. Aenean iaculis ultrices
                    est id vehicula. Etiam sit amet tincidunt ipsum. Vivamus vel ex tincidunt,
                    lacinia lacus maximus, tristique orci. Fusce varius risus nec accumsan hendrerit. "
        }
        ListElement {
            name: "Charlie a daosijoidjaoisj xzi cwsdasd"
            number: "555-111"
            info: ""
        }
    }

    ListView {
        anchors.fill: parent
        model: contactModel
        delegate: contactDelegate
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true
    }
}
