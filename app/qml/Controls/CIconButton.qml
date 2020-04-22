import Theme 1.0

CButton {
    id: root

    property alias tooltip: toolTip

    font: Theme.iconFontSolid
    implicitWidth: Theme.defaultButtonHeight
    implicitHeight: Theme.defaultButtonHeight

    CToolTip {
        id: toolTip

        visible: root.hovered && text.length !== 0
    }
}
