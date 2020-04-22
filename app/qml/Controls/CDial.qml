import QtQuick 2.12
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.2

import Theme 1.0
import QmlColorPalette 1.0
import SizeProvider 1.0
import SpecialSymbols 1.0

Templates.Dial {
    id: root

    implicitWidth: SizeProvider.metric(100)
    implicitHeight: implicitWidth

    property alias text: textItem.text
    property color backgroundColor: {
        if (!root.enabled) {
            return QmlColorPalette.disabled.background
        }

        if (root.pressed || root.highlighted) {
            return QmlColorPalette.active.background
        }

        if (root.hovered) {
            return QmlColorPalette.hovered.background
        }

        return QmlColorPalette.normal.background
    }

    background: Canvas {
        id: canvas

        width: root.availableWidth
        height: root.availableHeight

        Connections {
            target: root
            onPositionChanged: canvas.requestPaint()
        }

        readonly property real radius: width / 2.4

        onPaint: {
            var ctx = getContext("2d")
            ctx.reset()

            var centerX = width / 2
            var centerY = height / 2

            ctx.globalAlpha = 0.5
            ctx.beginPath()
            ctx.strokeStyle = root.backgroundColor
            ctx.lineWidth = 5
            var bgStartAngle = 0
            var bgEndAngle = 2 * Math.PI
            ctx.arc(centerX, centerY, radius, bgStartAngle, bgEndAngle, false)
            ctx.stroke()

            ctx.globalAlpha = 1
            ctx.beginPath()
            ctx.strokeStyle = QmlColorPalette.generic.accent
            ctx.lineWidth = 5
            var accentStartAngle = 0.7 * Math.PI
            var accentEndAngle = 1.55 * Math.PI * root.position - 1.25 * Math.PI
            ctx.arc(centerX, centerY, radius, accentStartAngle, accentEndAngle, false)
            ctx.stroke()
        }

        CText {
            id: textItem

            anchors.centerIn: parent
            width: root.availableWidth - canvas.radius
            height: root.availableHeight - canvas.radius
            elide: Text.ElideRight
            wrapMode: TextEdit.Wrap
            color: {
                if (!root.enabled) {
                    return QmlColorPalette.disabled.text
                }

                if (root.pressed || root.visualFocus) {
                    return QmlColorPalette.active.text
                }

                if (root.hovered) {
                    return QmlColorPalette.hovered.text
                }

                return QmlColorPalette.normal.text
            }
        }
    }

    handle: CIconText {
        text: SpecialSymbols.faAngleUp
        x: root.background.x + root.background.width / 2 - root.handle.width / 2
        y: root.background.y + root.background.height / 2 - root.handle.height / 2
        transform: [
            Translate {
                y: -root.background.width / 2 + root.handle.height
            },
            Rotation {
                angle: root.angle
                origin.x: root.handle.width / 2
                origin.y: root.handle.height / 2
            }
        ]
        color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.indication
            }

            if (root.pressed || root.visualFocus) {
                return QmlColorPalette.active.indication
            }

            if (root.hovered) {
                return QmlColorPalette.hovered.indication
            }

            return QmlColorPalette.normal.indication
        }
        implicitWidth: SizeProvider.metric(15)
        implicitHeight: implicitWidth
    }
}
