import QtQuick 2.9
import QtQuick.Templates 2.2 as Templates

import SizeProvider 1.0
import QmlColorPalette 1.0

Templates.Switch {
    id: root

    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding
    implicitHeight: Math.max(contentItem.implicitHeight,
                             indicator.implicitHeight + topPadding + bottomPadding)

    spacing: SizeProvider.metric(5)

    indicator: CSwitchIndicator {
        x: text
           ? (root.mirrored ? root.width - width - root.rightPadding : root.leftPadding)
           : root.leftPadding + (root.availableWidth - width) / 2
        y: root.topPadding + (root.availableHeight - height) / 2
        control: root
    }

    contentItem: CText {
        leftPadding: root.indicator && !root.mirrored ? root.indicator.width + root.spacing : 0
        rightPadding: root.indicator && root.mirrored ? root.indicator.width + root.spacing : 0
        text: root.text
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.text
            }

            if (root.hovered) {
                return QmlColorPalette.hovered.text
            }

            if (root.pressed || root.checked) {
                return QmlColorPalette.active.text
            }

            return QmlColorPalette.normal.text
        }
    }
}
