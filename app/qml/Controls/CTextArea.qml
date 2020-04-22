import QtQuick 2.12
import QtQuick.Templates 2.12 as Templates
import QtQuick.Controls 2.2
import QtQuick.Controls.impl 2.2

import SizeProvider 1.0
import Theme 1.0
import QmlColorPalette 1.0

Item {
    id: root

    property alias text: textAreaItem.text
    property alias placeholderText: textAreaItem.placeholderText

    Rectangle {
        id: backgroundItem

        anchors.fill: flickable
        color: QmlColorPalette.normal.background
        radius: Theme.defaultBorderRadius
        border.width: Theme.defaultBorderWidth
        border.color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.border
            }

            if (textAreaItem.focus && !textAreaItem.readOnly) {
                return QmlColorPalette.active.border
            }

            return QmlColorPalette.normal.border
        }

        Behavior on border.color { ColorAnimation { duration: 100 } }
    }

    Flickable {
        id: flickable

        width: root.width
        height: root.height
        boundsBehavior: Flickable.StopAtBounds
        boundsMovement: Flickable.StopAtBounds
        flickableDirection: Flickable.VerticalFlick
        contentWidth: textAreaItem.width
        contentHeight: textAreaItem.height
        clip: true
        focus: true

        function ensureVisible(r) {
            if (contentX >= r.x) {
                contentX = r.x
            } else if (contentX + width <= r.x + r.width) {
                contentX = r.x + r.width - width
            }

            if (contentY >= r.y) {
                contentY = r.y
            } else if (contentY + height <= r.y + r.height) {
                contentY = r.y + r.height - height
            }
        }

        TextArea.flickable: Templates.TextArea {
            id: textAreaItem

            width: root.width
            height: root.height
            color: QmlColorPalette.normal.text
            selectionColor: QmlColorPalette.generic.textSelection
            selectedTextColor: QmlColorPalette.generic.textSelection
            font.family: Theme.defaultFontFamily
            font.pointSize: Theme.defaultFontSize
            wrapMode: TextEdit.Wrap
            padding: SizeProvider.metric(4)

            cursorDelegate: CTextInputCursor {
                font: textAreaItem.font
                color: QmlColorPalette.normal.text
            }

            onCursorRectangleChanged: flickable.ensureVisible(cursorRectangle)

            onPressed: {
                if (event.button === Qt.RightButton) {
                    var selectionStart = textAreaItem.selectionStart
                    var selectionEnd = textAreaItem.selectionEnd
                    menuLoader.active = true
                    menuLoader.item.x = event.x
                    menuLoader.item.y = event.y
                    menuLoader.item.open()
                    textAreaItem.select(selectionStart, selectionEnd)
                    textAreaItem.focus = true
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

                x: flickable.contentX + textAreaItem.leftPadding
                y: flickable.contentY + textAreaItem.topPadding
                width: textAreaItem.width - (textAreaItem.leftPadding + textAreaItem.rightPadding)
                height: textAreaItem.height - (textAreaItem.topPadding + textAreaItem.bottomPadding)
                text: textAreaItem.placeholderText
                font: textAreaItem.font
                color: QmlColorPalette.disabled.text
                verticalAlignment: textAreaItem.verticalAlignment
                elide: Text.ElideRight
                visible: !textAreaItem.length
                         && !textAreaItem.preeditText
                         && (!root.activeFocus || textAreaItem.horizontalAlignment !== Qt.AlignHCenter)
            }

            QtObject {
                id: internal

                property bool focusComesFromContextMenu: false
            }

            Loader {
                id: menuLoader

                sourceComponent: menuComponent
            }

            Component {
                id: menuComponent

                CMenu {
                    CMenuItem {
                        text: qsTr("Cut")
                        enabled: textAreaItem.selectedText.length > 0
                        highlighted: hovered
                        onTriggered: {
                            internal.focusComesFromContextMenu = true
                            textAreaItem.cut()
                        }
                    }

                    CMenuItem {
                        text: qsTr("Copy")
                        enabled: textAreaItem.selectedText.length > 0
                        onTriggered: {
                            internal.focusComesFromContextMenu = true
                            textAreaItem.copy()
                        }
                    }

                    CMenuItem {
                        text: qsTr("Paste")
                        enabled: textAreaItem.canPaste
                        onTriggered: {
                            internal.focusComesFromContextMenu = true
                            textAreaItem.paste()
                        }
                    }
                }
            }
        }

        ScrollBar.vertical: CScrollBar {
            policy: ScrollBar.AsNeeded
            padding: SizeProvider.metric(2)
        }
    }
}
