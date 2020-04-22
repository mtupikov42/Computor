import QtQuick 2.9
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.2

import Theme 1.0
import QmlColorPalette 1.0
import SizeProvider 1.0

Templates.Button {
    id: root

    hoverEnabled: true
    font.family: Theme.defaultFontFamily
    font.pointSize: Theme.defaultFontSize
    padding: SizeProvider.metric(4)
    implicitHeight: Theme.defaultButtonHeight
    implicitWidth: contentText.implicitWidth + leftPadding + rightPadding

    property alias radius: backgroundItem.radius
    property alias border: backgroundItem.border

    property color color: {
        if (!root.enabled) {
            return QmlColorPalette.disabled.text
        }

        if (root.pressed) {
            return QmlColorPalette.active.text
        }

        if (root.hovered || root.highlighted) {
            return QmlColorPalette.hovered.text
        }

        return QmlColorPalette.normal.text
    }

    property color backgroundColor: {
        if (!root.enabled) {
            return QmlColorPalette.disabled.background
        }

        if (root.pressed || root.highlighted) {
            return QmlColorPalette.active.background
        }

        if (root.hovered) {
            return QmlColorPalette.hovered.background
        }

        return QmlColorPalette.normal.background
    }

    background: Rectangle {
        id: backgroundItem
        radius: Theme.defaultBorderRadius
        border.width: Theme.defaultBorderWidth
        border.color: QmlColorPalette.normal.border
        color: root.backgroundColor

        Behavior on color { ColorAnimation { duration: 100 } }
    }

    contentItem: CText {
        id: contentText
        text: root.text
        color: root.color
        font: root.font
        visible: text.length > 0
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter

        Behavior on color { ColorAnimation { duration: 100 } }
    }

    Ripple {
        clipRadius: 1
        width: parent.width
        height: parent.height
        pressed: root.pressed
        anchor: root
        active: root.down || root.visualFocus || root.hovered
        color: Material.rippleColor
    }
}
