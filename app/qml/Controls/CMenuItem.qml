import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.2

import Theme 1.0
import QmlColorPalette 1.0

MenuItem {
    id: root

    background: Rectangle {
        anchors.fill: parent
        opacity: 0.5
        color: {
            if (root.pressed){
                return QmlColorPalette.active.background
            }

            if (root.hovered) {
                return QmlColorPalette.hovered.background
            }

            return QmlColorPalette.normal.background
        }

        Behavior on color { ColorAnimation { duration: 100 } }
    }

    contentItem: CText {
        text: root.text
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
        color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.text
            }

            if (root.pressed) {
                return QmlColorPalette.active.text
            }

            if (root.hovered) {
                return QmlColorPalette.hovered.text
            }

            return QmlColorPalette.normal.text
        }

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
