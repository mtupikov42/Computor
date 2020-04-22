import QtQuick 2.9
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

import SizeProvider 1.0
import QmlColorPalette 1.0
import Theme 1.0

Templates.Drawer {
    id: root

    implicitWidth: Math.max(background ? background.implicitWidth : 0, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0, contentHeight + topPadding + bottomPadding)

    contentWidth: contentItem.implicitWidth || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    topPadding: !dim && edge === Qt.BottomEdge && Material.elevation === 0
    leftPadding: !dim && edge === Qt.RightEdge && Material.elevation === 0
    rightPadding: !dim && edge === Qt.LeftEdge && Material.elevation === 0
    bottomPadding: !dim && edge === Qt.TopEdge && Material.elevation === 0

    enter: Transition { SmoothedAnimation { velocity: 5 } }
    exit: Transition { SmoothedAnimation { velocity: 5 } }

    Material.elevation: !interactive && !dim ? 0 : SizeProvider.metric(16)

    background: Rectangle {
        color: QmlColorPalette.generic.windowBackground

        Rectangle {
            readonly property bool horizontal: root.edge === Qt.LeftEdge || root.edge === Qt.RightEdge

            width: horizontal ? SizeProvider.metric(1) : parent.width
            height: horizontal ? parent.height : SizeProvider.metric(1)
            color: Material.dividerColor
            x: root.edge === Qt.LeftEdge ? parent.width - SizeProvider.metric(1) : 0
            y: root.edge === Qt.TopEdge ? parent.height - SizeProvider.metric(1) : 0
            visible: !root.dim && root.Material.elevation === 0
        }

        layer.enabled: root.position > 0
        layer.effect: ElevationEffect {
            elevation: Material.elevation
            fullHeight: true
        }
    }
}
