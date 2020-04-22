import QtQuick 2.9
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

import SizeProvider 1.0
import QmlColorPalette 1.0

Templates.RangeSlider {
    id: root

    implicitWidth: background.implicitWidth + leftPadding + rightPadding
    implicitHeight: background.implicitHeight + topPadding + bottomPadding

    first.handle: CSliderHandle {
        x: root.leftPadding + (horizontal
                               ? root.first.visualPosition * (root.availableWidth - width)
                               : (root.availableWidth - width) / 2)
        y: root.topPadding + (horizontal
                              ? (root.availableHeight - height) / 2
                              : root.first.visualPosition * (root.availableHeight - height))
        value: root.first.value
        handleHasFocus: activeFocus
        handlePressed: first.pressed
        handleHovered: first.hovered
    }

    second.handle: CSliderHandle {
        x: root.leftPadding + (horizontal
                               ? root.second.visualPosition * (root.availableWidth - width)
                               : (root.availableWidth - width) / 2)
        y: root.topPadding + (horizontal
                              ? (root.availableHeight - height) / 2
                              : root.second.visualPosition * (root.availableHeight - height))
        value: root.second.value
        handleHasFocus: activeFocus
        handlePressed: second.pressed
        handleHovered: second.hovered
    }

    background: Rectangle {
        id: backgroundItem

        x: root.leftPadding + (horizontal ? 0 : (root.availableWidth - width) / 2)
        y: root.topPadding + (horizontal ? (root.availableHeight - height) / 2 : 0)
        implicitWidth: horizontal ? SizeProvider.metric(200) : internal.handleWidth
        implicitHeight: horizontal ? internal.handleHeight : SizeProvider.metric(200)
        width: horizontal ? root.availableWidth : bgSize
        height: horizontal ? bgSize : root.availableHeight
        color: QmlColorPalette.normal.background
        scale: horizontal && root.mirrored ? -1 : 1

        readonly property int bgSize: SizeProvider.metric(4)
        readonly property bool horizontal: root.orientation === Qt.Horizontal

        Rectangle {
            x: parent.horizontal ? root.first.visualPosition * parent.width + SizeProvider.metric(1) : 0
            y: parent.horizontal ? 0 : root.second.visualPosition * parent.height + SizeProvider.metric(1)
            width: parent.horizontal
                   ? root.second.visualPosition * parent.width - root.first.visualPosition * parent.width - SizeProvider.metric(2)
                   : backgroundItem.bgSize
            height: parent.horizontal
                    ? backgroundItem.bgSize
                    : root.second.visualPosition * parent.height - root.first.visualPosition * parent.height - SizeProvider.metric(2)

            color: QmlColorPalette.generic.accent
        }
    }

    QtObject {
        id: internal

        readonly property int handleWidth: Math.max(root.first.handle.width, root.second.handle.width)
        readonly property int handleHeight: Math.max(root.first.handle.height, root.second.handle.height)
    }
}
