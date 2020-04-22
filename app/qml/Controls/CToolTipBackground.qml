import QtQuick 2.9

import Theme 1.0
import SizeProvider 1.0

Item {
    id: root

    property point tipPoint: Qt.point(0, 0)
    property int tipPlacement: Theme.Bottom
    property int tipLength: SizeProvider.metric(5)
    property int tipWidth: SizeProvider.metric(10)
    property color color: "#ffffff"
    property color borderColor: "#000000"
    property real borderWidth: 0.
    property real radius: 0.

    property real topLeftRadius: 0.
    property real topRightRadius: 0.
    property real bottomLeftRadius: 0.
    property real bottomRightRadius: 0.

    onTipPointChanged: {
        canvas.requestPaint()
    }

    onTipPlacementChanged: {
        canvas.requestPaint()
    }

    onTipLengthChanged: {
        canvas.requestPaint()
    }

    onTipWidthChanged: {
        canvas.requestPaint()
    }

    onColorChanged: {
        canvas.requestPaint()
    }

    onBorderColorChanged: {
        canvas.requestPaint()
    }

    onBorderWidthChanged: {
        canvas.requestPaint()
    }

    onRadiusChanged: {
        topLeftRadius = radius
        topRightRadius = radius
        bottomLeftRadius = radius
        bottomRightRadius = radius
    }

    onTopLeftRadiusChanged: {
        canvas.markDirty(Qt.rect(0, 0, topLeftRadius, topLeftRadius))
        canvas.requestPaint()
    }

    onTopRightRadiusChanged: {
        canvas.markDirty(Qt.rect(width - topRightRadius, 0, topRightRadius, topRightRadius))
        canvas.requestPaint()
    }

    onBottomRightRadiusChanged: {
        canvas.markDirty(Qt.rect(width - bottomRightRadius, height - bottomRightRadius, bottomRightRadius, bottomRightRadius))
        canvas.requestPaint()
    }

    onBottomLeftRadiusChanged: {
        canvas.markDirty(Qt.rect(0, height - bottomLeftRadius, bottomLeftRadius, bottomLeftRadius))
        canvas.requestPaint()
    }

    Canvas {
        id: canvas
        anchors.fill: parent
        antialiasing: true
        smooth: true

        onPaint: {
            var ctx = getContext("2d")
            ctx.reset()

            ctx.fillStyle = root.color
            ctx.strokeStyle = root.borderColor
            ctx.lineWidth = root.borderWidth

            var offset = root.borderWidth
            var leftOffset = (root.tipPlacement === Theme.Left) ? root.tipLength + offset : offset
            var topOffset = (root.tipPlacement === Theme.Top) ? root.tipLength + offset : offset
            var rightOffset = (root.tipPlacement === Theme.Right) ? root.tipLength + offset : offset
            var bottomOffset = (root.tipPlacement === Theme.Bottom) ? root.tipLength + offset : offset

            ctx.beginPath()

            ctx.moveTo(leftOffset, root.topLeftRadius + topOffset)
            ctx.arc(root.topLeftRadius + leftOffset,
                    root.topLeftRadius + topOffset,
                    topLeftRadius, -Math.PI, -Math.PI * 0.5, false);

            var center = 0
            if (root.tipPlacement === Theme.Top) {
                center = tipPoint.x - offset
                ctx.lineTo(center - tipWidth * .5, topOffset)
                ctx.lineTo(center, offset)
                ctx.lineTo(center + tipWidth * .5, topOffset)
            }
            ctx.lineTo(parent.width - root.topRightRadius - rightOffset, topOffset)

            ctx.arc(parent.width - root.topRightRadius - rightOffset,
                    root.topRightRadius + topOffset,
                    topRightRadius, -Math.PI * 0.5, 0, false);

            if (root.tipPlacement === Theme.Right) {
                center = tipPoint.y - offset
                ctx.lineTo(parent.width - rightOffset, center - tipWidth * .5)
                ctx.lineTo(parent.width - offset, center)
                ctx.lineTo(parent.width - rightOffset, center + tipWidth * .5)
            }
            ctx.lineTo(parent.width - rightOffset, parent.height - root.bottomRightRadius - bottomOffset)

            ctx.arc(parent.width - root.bottomRightRadius - rightOffset,
                    parent.height - root.bottomRightRadius - bottomOffset,
                    bottomRightRadius, 0, Math.PI * 0.5, false);

            if (root.tipPlacement === Theme.Bottom) {
                center = tipPoint.x - offset
                ctx.lineTo(center + tipWidth * .5, parent.height - bottomOffset)
                ctx.lineTo(center, parent.height - offset)
                ctx.lineTo(center - tipWidth * .5, parent.height - bottomOffset)
            }
            ctx.lineTo(root.bottomLeftRadius + leftOffset, parent.height - bottomOffset)

            ctx.arc(root.bottomLeftRadius + leftOffset,
                    parent.height - root.bottomLeftRadius - bottomOffset,
                    bottomLeftRadius, Math.PI * 0.5, Math.PI, false);

            if (root.tipPlacement === Theme.Left) {
                center = tipPoint.y - offset
                ctx.lineTo(leftOffset, center + tipWidth * .5)
                ctx.lineTo(offset, center)
                ctx.lineTo(leftOffset, center - tipWidth * .5)
            }
            ctx.closePath()

            ctx.fill()

            if (root.borderWidth) {
                ctx.stroke()
            }
        }
    }
}
