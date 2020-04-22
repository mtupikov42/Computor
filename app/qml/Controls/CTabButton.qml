import QtQuick 2.9
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

import SizeProvider 1.0
import Theme 1.0
import QmlColorPalette 1.0

Templates.TabButton {
    id: root

    implicitWidth: Math.max(background.implicitWidth,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background.implicitHeight,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    padding: SizeProvider.metric(5)

    contentItem: CText {
        text: root.text
        elide: Text.ElideRight
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
    }

    background: Ripple {
        implicitHeight: SizeProvider.metric(50)
        clip: true
        pressed: root.pressed
        anchor: root
        active: root.down || root.visualFocus || root.hovered
        color: Material.rippleColor
    }
}
