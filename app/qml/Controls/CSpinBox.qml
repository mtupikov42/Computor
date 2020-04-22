import QtQuick 2.9
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

import SizeProvider 1.0
import QmlColorPalette 1.0
import SpecialSymbols 1.0
import Theme 1.0

Templates.SpinBox {
    id: root

    implicitWidth: Math.max(background.implicitWidth,
                            contentItem.implicitWidth +
                            up.indicator.implicitWidth +
                            down.indicator.implicitWidth)
    implicitHeight: Math.max(contentItem.implicitHeight + topPadding + bottomPadding,
                             background.implicitHeight,
                             up.indicator.implicitHeight,
                             down.indicator.implicitHeight)

    leftPadding: root.mirrored ? up.indicator.width : down.indicator.width
    rightPadding: root.mirrored ? down.indicator.width : up.indicator.width

    property int controlHeight: SizeProvider.metric(25)

    validator: IntValidator {
        bottom: Math.min(root.from, root.to)
        top: Math.max(root.from, root.to)
    }

    contentItem: TextInput {
        text: root.textFromValue(root.value)
        readOnly: !root.editable
        validator: root.validator
        inputMethodHints: root.inputMethodHints
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        font.family: Theme.defaultFontFamily
        font.pointSize: Theme.defaultFontSize
        color: enabled ? QmlColorPalette.normal.text : QmlColorPalette.disabled.text
        selectionColor: QmlColorPalette.generic.textSelection

        cursorDelegate: CTextInputCursor {
            visible: root.activeFocus
            font: parent.font
            color: QmlColorPalette.normal.text
        }
    }

    up.indicator: CIconText {
        x: root.mirrored ? 0 : parent.width - width
        implicitWidth: root.controlHeight
        implicitHeight: root.controlHeight
        text: SpecialSymbols.faPlus
        color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.text
            }

            if (root.up.pressed) {
                return QmlColorPalette.active.text
            }

            if (root.up.hovered) {
                return QmlColorPalette.hovered.text
            }

            return QmlColorPalette.normal.text
        }

        Behavior on color { ColorAnimation { duration: 100 } }

        Ripple {
            clipRadius: 1
            width: parent.width
            height: parent.height
            pressed: root.up.pressed
            active: root.up.pressed || root.up.hovered || root.visualFocus
            color: Material.rippleColor
        }
    }

    down.indicator: CIconText {
        x: root.mirrored ? parent.width - width : 0
        implicitWidth: root.controlHeight
        implicitHeight: root.controlHeight
        text: SpecialSymbols.faMinus
        color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.text
            }

            if (root.down.pressed) {
                return QmlColorPalette.active.text
            }

            if (root.down.hovered) {
                return QmlColorPalette.hovered.text
            }

            return QmlColorPalette.normal.text
        }

        Behavior on color { ColorAnimation { duration: 100 } }

        Ripple {
            clipRadius: 1
            width: parent.width
            height: parent.height
            pressed: root.down.pressed
            active: root.down.pressed || root.down.hovered || root.visualFocus
            color: Material.rippleColor
        }
    }

    background: Rectangle {
        implicitWidth: SizeProvider.metric(100)
        implicitHeight: root.controlHeight
        radius: Theme.defaultBorderRadius
        color: root.activeFocus ? QmlColorPalette.active.background : QmlColorPalette.normal.background
        border.width: Theme.defaultBorderWidth
        border.color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.border
            }

            if (root.activeFocus) {
                return QmlColorPalette.active.border
            }

            return QmlColorPalette.normal.border
        }

        Behavior on color { ColorAnimation { duration: 100 } }
    }
}
