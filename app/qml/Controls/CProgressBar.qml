import QtQuick 2.12
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

import SizeProvider 1.0
import Theme 1.0
import QmlColorPalette 1.0

Templates.ProgressBar {
    id: root

    implicitWidth: Math.max(background.implicitWidth, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background.implicitHeight, contentItem.implicitHeight + topPadding + bottomPadding)

    contentItem: ProgressBarImpl {
        implicitHeight: SizeProvider.metric(4)
        scale: root.mirrored ? -1 : 1
        progress: root.position
        indeterminate: root.visible && root.indeterminate
        color: QmlColorPalette.generic.accent
    }

    background: Rectangle {
        implicitWidth: SizeProvider.metric(200)
        implicitHeight: SizeProvider.metric(4)
        x: root.leftPadding
        y: root.topPadding + (root.availableHeight - height) / 2
        color: QmlColorPalette.normal.background
    }
}
