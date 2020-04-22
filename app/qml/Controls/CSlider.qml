import QtQuick 2.9
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

import SizeProvider 1.0
import QmlColorPalette 1.0

Templates.Slider {
    id: root

    implicitWidth: background.implicitWidth + leftPadding + rightPadding
    implicitHeight: background.implicitHeight + topPadding + bottomPadding

    handle: CSliderHandle {
        x: root.leftPadding + (horizontal
                               ? root.visualPosition * (root.availableWidth - width)
                               : (root.availableWidth - width) / 2)
        y: root.topPadding + (horizontal
                              ? (root.availableHeight - height) / 2
                              : root.visualPosition * (root.availableHeight - height))
        value: root.value
        control: root
    }

    background: Rectangle {
        id: backgroundItem

        x: root.leftPadding + (horizontal ? 0 : (root.availableWidth - width) / 2)
        y: root.topPadding + (horizontal ? (root.availableHeight - height) / 2 : 0)
        implicitWidth: horizontal ? SizeProvider.metric(200) : handle.width
        implicitHeight: horizontal ? handle.height : SizeProvider.metric(200)
        width: horizontal ? root.availableWidth : bgSize
        height: horizontal ? bgSize : root.availableHeight
        color: QmlColorPalette.normal.background
        scale: horizontal && root.mirrored ? -1 : 1

        readonly property int bgSize: SizeProvider.metric(4)
        readonly property bool horizontal: root.orientation === Qt.Horizontal

        Rectangle {
           x: parent.horizontal ? 0 : (parent.width - width) / 2
           y: parent.horizontal ? (parent.height - height) / 2 : root.visualPosition * parent.height
           width: parent.horizontal ? root.position * parent.width : backgroundItem.bgSize
           height: parent.horizontal ? backgroundItem.bgSize : root.position * parent.height
           color: QmlColorPalette.generic.accent
        }
    }
}
