import QtQuick 2.9
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

import SizeProvider 1.0
import QmlColorPalette 1.0
import Theme 1.0

Item {
    id: root

    implicitWidth: SizeProvider.metric(15)
    implicitHeight: SizeProvider.metric(15)

    property var control: parent
    property real value: 0
    property bool handleHasFocus: control.visualFocus
    property bool handlePressed: control.pressed
    property bool handleHovered: control.hovered
    readonly property bool horizontal: control.orientation === Qt.Horizontal

    Rectangle {
        id: handleRect

        width: parent.width
        height: parent.height
        radius: width / 2
        scale: root.handlePressed ? 1.5 : 1
        color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.indication
            }

            if (root.handlePressed) {
                return QmlColorPalette.active.indication
            }

            if (root.handleHovered) {
                return QmlColorPalette.hovered.indication
            }

            return QmlColorPalette.normal.indication
        }

        Behavior on scale { ScaleAnimator { duration: 100 } }
        Behavior on color { ColorAnimation { duration: 100 } }
    }

    Ripple {
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: handleRect.width
        height: handleRect.height
        pressed: root.handlePressed
        active: root.handlePressed || root.handleHasFocus || root.handleHovered
        color: Material.rippleColor
    }
}
