import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import SizeProvider 1.0
import QmlColorPalette 1.0

Item {
    id: root

    implicitHeight: rootColumn.height
    implicitWidth: rootColumn.width

    property int errorColumn: 0
    property alias text: sourceText.text
    property alias errorText: errorText.text

    Component.onCompleted: errorLineCanvas.requestPaint()

    ColumnLayout {
        id: rootColumn

        spacing: SizeProvider.metric(1)

        CText {
            id: sourceText
        }

        Canvas {
            id: errorLineCanvas

            width: sourceTextMetrics.advanceWidth
            height: SizeProvider.metric(5)

            onPaint: {
                var ctx = getContext("2d")
                ctx.strokeStyle = QmlColorPalette.generic.error
                ctx.lineWidth = SizeProvider.metric(1)

                var depth = root.errorColumn !== 0 ? errorLineCanvas.height - 1 : 0
                var cursorWidth = SizeProvider.metric(6)
                var substringWidth = substringTextMetrics.advanceWidth

                ctx.beginPath()
                ctx.moveTo(0, depth)
                ctx.lineTo(substringWidth, depth)
                ctx.moveTo(substringWidth, depth)
                ctx.lineTo(substringWidth + cursorWidth / 2, 0)
                ctx.moveTo(substringWidth + cursorWidth / 2, 0)
                ctx.lineTo(substringWidth + cursorWidth, depth)
                ctx.moveTo(substringWidth + cursorWidth, depth)
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
        id: substringTextMetrics

        text: sourceText.text.substring(0, root.errorColumn)
        font: sourceText.font
    }
}
