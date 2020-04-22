import QtQuick 2.12
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

import SizeProvider 1.0
import QmlColorPalette 1.0

Templates.RadioButton {
    id: root

    implicitHeight: contentItem.implicitHeight + topPadding + bottomPadding
    implicitWidth: contentItem.implicitWidth + indicator.implicitWidth + leftPadding + rightPadding
    spacing: SizeProvider.metric(5)
    topPadding: SizeProvider.metric(5)
    bottomPadding: SizeProvider.metric(5)
    hoverEnabled: true

    indicator: CRadioIndicator {
        x: root.leftPadding
        y: parent.height / 2 - height / 2
        control: root

        Ripple {
            clipRadius: 1
            width: parent.width
            height: parent.height
            anchor: root
            pressed: root.pressed
            active: root.down || root.visualFocus || root.hovered
            color: Material.rippleColor
        }
    }

    contentItem: CText {
        id: textItem

        leftPadding: root.indicator.width
        rightPadding: root.indicator.width
        text: root.text
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.text
            }

            if (root.pressed || root.checked) {
                return QmlColorPalette.active.text
            }

            if (root.hovered) {
                return QmlColorPalette.hovered.text
            }

            return QmlColorPalette.normal.text
        }

        Behavior on color { ColorAnimation { duration: 100 } }
    }
}
