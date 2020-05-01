import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import SizeProvider 1.0
import QmlColorPalette 1.0

Item {
    id: root

    property int errorColumn: 0
    property alias text: sourceText.text
    property alias errorText: errorText.text

    Component.onCompleted: errorLineCanvas.requestPaint()

    ColumnLayout {
        anchors.fill: parent
        spacing: SizeProvider.metric(1)

        CText {
            id: sourceText
        }

        Canvas {
            id: errorLineCanvas

            width: sourceTextMetrics.advanceWidth
            height: SizeProvider.metric(5)

            readonly property int symWidth: oneSymbolTextMetrics.advanceWidth

            onPaint: {
                var ctx = getContext("2d")
                ctx.strokeStyle = QmlColorPalette.generic.error
                ctx.lineWidth = SizeProvider.metric(1)

                var depth = root.errorColumn !== 0 ? errorLineCanvas.height - 1 : 0
                var symbolWidth = symWidth * 1.5
                var cursorWidth = SizeProvider.metric(6)

                ctx.beginPath()
                ctx.moveTo(0, depth)
                ctx.lineTo(root.errorColumn * symbolWidth, depth)
                ctx.moveTo(root.errorColumn * symbolWidth, depth)
                ctx.lineTo(root.errorColumn * symbolWidth + cursorWidth / 2, 0)
                ctx.moveTo(root.errorColumn * symbolWidth + cursorWidth / 2, 0)
                ctx.lineTo(root.errorColumn * symbolWidth + cursorWidth, depth)
                ctx.moveTo(root.errorColumn * symbolWidth + cursorWidth, depth)
                ctx.lineTo(errorLineCanvas.width, depth)
                ctx.stroke()
            }
        }

        CText {
            id: errorText

            color: QmlColorPalette.generic.error
        }
    }

    TextMetrics {
        id: sourceTextMetrics

        text: sourceText.text
        font: sourceText.font
    }

    TextMetrics {
        id: oneSymbolTextMetrics

        text: " "
        font: sourceText.font
    }
}
