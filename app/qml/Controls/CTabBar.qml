import QtQuick 2.9
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

import SizeProvider 1.0
import Theme 1.0
import QmlColorPalette 1.0

Templates.TabBar {
    id: root

    implicitWidth: Math.max(background.implicitWidth,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background.implicitHeight,
                             contentHeight + topPadding + bottomPadding)

    spacing: SizeProvider.metric(1)
    contentHeight: SizeProvider.metric(50)

    contentItem: ListView {
        model: root.contentModel
        currentIndex: root.currentIndex

        spacing: root.spacing
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickIfNeeded
        snapMode: ListView.SnapToItem

        highlightMoveDuration: 250
        highlightResizeDuration: 0
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.ApplyRange
        preferredHighlightBegin: SizeProvider.metric(50)
        preferredHighlightEnd: width - SizeProvider.metric(50)

        highlight: Item {
            z: 2

            Rectangle {
                height: 2
                width: parent.width
                y: root.position === Templates.TabBar.Footer ? 0 : parent.height - height
                color: QmlColorPalette.generic.accent
            }
        }
    }

    background: Rectangle {
        color: QmlColorPalette.generic.windowBackground
        border.width: Theme.defaultBorderWidth
        border.color: QmlColorPalette.normal.border
    }
}
