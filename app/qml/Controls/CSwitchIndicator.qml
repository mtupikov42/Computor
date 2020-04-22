import QtQuick 2.9
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

import QmlColorPalette 1.0
import SizeProvider 1.0
import Theme 1.0

Item {
    id: root

    implicitWidth: SizeProvider.metric(30)
    implicitHeight: SizeProvider.metric(15)

    property Item control
    property alias handle: handle

    Rectangle {
        width: parent.width
        height: SizeProvider.metric(14)
        radius: height / 2
        y: parent.height / 2 - height / 2
        color: {
            if (!root.control.enabled) {
                return QmlColorPalette.disabled.background
            }

            if (root.control.hovered) {
                return QmlColorPalette.hovered.background
            }

            if (root.control.checked) {
                return QmlColorPalette.active.background
            }

            return QmlColorPalette.normal.background
        }
    }

    Rectangle {
        id: handle

        x: Math.max(0, Math.min(parent.width - width, root.control.visualPosition * parent.width - (width / 2)))
        y: (parent.height - height) / 2
        width: SizeProvider.metric(15)
        height: SizeProvider.metric(15)
        radius: width / 2
        color: {
            if (!root.control.enabled) {
                return QmlColorPalette.disabled.indication
            }

            if (root.control.pressed || root.control.checked) {
                return QmlColorPalette.generic.accent
            }

            if (root.control.hovered) {
                return QmlColorPalette.hovered.indication
            }

            return QmlColorPalette.normal.indication
        }

        Behavior on x {
            enabled: !root.control.pressed
            SmoothedAnimation {
                duration: 300
            }
        }

        Behavior on color { ColorAnimation { duration: 100 } }

    }
}
