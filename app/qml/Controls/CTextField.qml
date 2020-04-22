import QtQuick 2.12
import QtQuick.Templates 2.12 as Templates
import QtQuick.Controls 2.2
import QtQuick.Controls.impl 2.2

import SizeProvider 1.0
import Theme 1.0
import QmlColorPalette 1.0

Templates.TextField {
    id: root

    implicitHeight: Theme.defaultTextFieldHeight
    font.family: Theme.defaultFontFamily
    font.pointSize: Theme.defaultFontSize
    color: QmlColorPalette.normal.text
    selectionColor: QmlColorPalette.generic.textSelection
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft
    leftPadding: SizeProvider.metric(4)
    rightPadding: SizeProvider.metric(4)

    property alias backgroundRadius: backgroundItem.radius
    property alias backgroundBorder: backgroundItem.border
    property alias tooltip: toolTip

    background: Rectangle {
        id: backgroundItem

        color: QmlColorPalette.normal.background
        radius: Theme.defaultBorderRadius
        border.width: Theme.defaultBorderWidth
        border.color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.border
            }

            if (root.focus && !readOnly) {
                return QmlColorPalette.active.border
            }

            return QmlColorPalette.normal.border
        }

        Behavior on border.color { ColorAnimation { duration: 100 } }
    }

    cursorDelegate: CTextInputCursor {
        font: root.font
        color: QmlColorPalette.normal.text
    }

    onPressed: {
        if (event.button === Qt.RightButton) {
            var selectionStart = root.selectionStart
            var selectionEnd = root.selectionEnd
            menuLoader.active = true
            menuLoader.item.x = event.x
            menuLoader.item.y = event.y
            menuLoader.item.open()
            root.select(selectionStart, selectionEnd)
            root.focus = true
        }
    }

    onActiveFocusChanged: {
        if (internal.focusComesFromContextMenu) {
            internal.focusComesFromContextMenu = false
            return
        }

        if (focus) {
            selectAll()
        }
    }

    PlaceholderText {
        id: placeholder

        x: root.leftPadding
        y: root.topPadding
        width: root.width - (root.leftPadding + root.rightPadding)
        height: root.height - (root.topPadding + root.bottomPadding)
        text: root.placeholderText
        font: root.font
        color: QmlColorPalette.disabled.text
        verticalAlignment: root.verticalAlignment
        elide: Text.ElideRight
        visible: !root.length
                 && !root.preeditText
                 && (!root.activeFocus || root.horizontalAlignment !== Qt.AlignHCenter)
    }

    QtObject {
        id: internal

        property bool focusComesFromContextMenu: false
    }

    Loader {
        id: menuLoader

        sourceComponent: menuComponent
    }

    CToolTip {
        id: toolTip

        visible: root.hovered && text.length !== 0
    }

    Component {
        id: menuComponent

        CMenu {
            CMenuItem {
                text: qsTr("Cut")
                enabled: root.selectedText.length > 0
                highlighted: hovered
                onTriggered: {
                    internal.focusComesFromContextMenu = true
                    root.cut()
                }
            }

            CMenuItem {
                text: qsTr("Copy")
                enabled: root.selectedText.length > 0
                onTriggered: {
                    internal.focusComesFromContextMenu = true
                    root.copy()
                }
            }

            CMenuItem {
                text: qsTr("Paste")
                enabled: root.canPaste
                onTriggered: {
                    internal.focusComesFromContextMenu = true
                    root.paste()
                }
            }
        }
    }
}
