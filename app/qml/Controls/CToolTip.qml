import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.3

import Theme 1.0
import QmlColorPalette 1.0
import SizeProvider 1.0

import "../Utils.js" as Utils

Item {
    id: root

    visible: false
    implicitWidth: tooltip.implicitWidth
    implicitHeight: tooltip.implicitHeight

    property alias text: tooltip.text
    property alias font: tooltip.font
    property alias color: content.color
    property alias backgroundColor: backgroundItem.color
    property alias delay: tooltip.delay
    property alias timeout: tooltip.timeout

    property int placement: Theme.Top
    property int offset: 0
    property point from: {
        switch (placement) {
        case Theme.Left: return Qt.point(-offset, (parent.height / 2))
        case Theme.Right: return Qt.point(parent.width + offset, (parent.height / 2))
        case Theme.Top: return Qt.point((parent.width / 2), -offset)
        case Theme.Bottom: return Qt.point((parent.width / 2), parent.height + offset)
        }
    }

    function show() {
        visible = true
    }

    function hide() {
        visible = false
    }

    onVisibleChanged: {
        if (visible) {
            internal.updateAppearance()
        }
    }

    QtObject {
        id: internal

        readonly property int tipSize: SizeProvider.metric(5)
        readonly property int verticalPadding: SizeProvider.metric(5)
        readonly property int horizontalPadding: SizeProvider.metric(8)
        readonly property var additionalPadding: {
            "left" : ((placement === Theme.Right) ? tipSize : 0),
            "right" : ((placement === Theme.Left) ? tipSize : 0),
            "top" : ((placement === Theme.Bottom) ? tipSize : 0),
            "bottom" : ((placement === Theme.Top) ? tipSize : 0)
        }
        readonly property int contentAlignment: {
            if (root.placement === Theme.Left || root.placement === Theme.Right) {
                return Text.AlignLeft
            } else {
                return Text.AlignHCenter
            }
        }

        function calculateTopLeft() {
            var topLeftX = 0
            var topLeftY = 0

            switch (root.placement) {
            case Theme.Left:
                topLeftX = from.x - root.implicitWidth
                topLeftY = from.y - (root.implicitHeight / 2)
                break
            case Theme.Right:
                topLeftX = from.x
                topLeftY = from.y - (root.implicitHeight / 2)
                break
            case Theme.Top:
                topLeftX = from.x - (root.implicitWidth / 2)
                topLeftY = from.y - root.implicitHeight
                break
            case Theme.Bottom:
                topLeftX = from.x - (root.implicitWidth / 2)
                topLeftY = from.y
                break
            }

            return clampToVisible(Qt.point(topLeftX, topLeftY))
        }

        function clampToVisible(point) {
            var globalTopLeft = root.parent.mapToItem(null, point.x, point.y)
            globalTopLeft.x = Utils.clamp(globalTopLeft.x, 0, root.Window.width - root.implicitWidth)
            globalTopLeft.y = Utils.clamp(globalTopLeft.y, 0, root.Window.height - root.implicitHeight)
            return root.parent.mapFromItem(null, globalTopLeft.x, globalTopLeft.y)
        }

        function updateAppearance() {
            var topLeft = calculateTopLeft()
            root.x = topLeft.x
            root.y = topLeft.y
        }
    }

    ToolTip {
        id: tooltip

        x: 0
        y: 0
        font.family: Theme.defaultFontFamily
        font.pointSize: Theme.defaultFontSize - 1
        leftPadding: internal.horizontalPadding + internal.additionalPadding.left
        rightPadding: internal.horizontalPadding + internal.additionalPadding.right
        topPadding: internal.verticalPadding + internal.additionalPadding.top
        bottomPadding: internal.verticalPadding + internal.additionalPadding.bottom
        visible: parent ? parent.visible : false
        implicitWidth: leftPadding + rightPadding + content.implicitWidth
        implicitHeight: topPadding + bottomPadding + content.implicitHeight
        delay: Theme.defaultTooltipDelay

        contentItem: CText {
            id: content
            text: root.text
            font: root.font
            color: QmlColorPalette.normal.text
            horizontalAlignment: internal.contentAlignment
        }

        background: CToolTipBackground {
            id: backgroundItem
            borderColor: color
            borderWidth: Theme.defaultBorderWidth
            color: QmlColorPalette.normal.background
            radius: Theme.defaultBorderRadius
            anchors.fill: parent
            anchors.leftMargin: Math.floor(root.parent.x) - root.parent.x
            anchors.rightMargin: Math.floor(root.parent.x) - root.parent.x

            tipLength: internal.tipSize
            tipWidth: 2 * internal.tipSize
            tipPoint: Qt.point(root.from.x - root.x, root.from.y - root.y)

            tipPlacement: {
                switch (root.placement) {
                case Theme.Left: return Theme.Right
                case Theme.Top: return Theme.Bottom
                case Theme.Right: return Theme.Left
                case Theme.Bottom: return Theme.Top
                }
            }
        }

        onVisibleChanged: {
            if (!visible && (root.visible !== visible)) {
                root.visible = visible
            }
        }
    }
}
